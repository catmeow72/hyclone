#include <cassert>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include "BeDefs.h"
#include "haiku_errors.h"
#include "port.h"
#include "process.h"
#include "server_native.h"
#include "server_servercalls.h"
#include "server_workers.h"
#include "system.h"

const int kSleepTimeMicroseconds = 100 * 1000; // 100ms, in microseconds.

Port::Port(int pid, int capacity, const char* name)
{
    _info.capacity = capacity;
    _info.team = pid;
    strncpy(_info.name, name, sizeof(_info.name) - 1);
    _info.name[sizeof(_info.name) - 1] = '\0';
    _info.queue_count = 0;
    _info.total_count = 0;
    _info.port = 0;
}

void Port::Write(int code, std::vector<char>&& message)
{
    _messages.emplace(code, std::move(message));
    ++_info.queue_count;
}

std::pair<int, std::vector<char>> Port::Read()
{
    assert(_info.queue_count > 0);
    --_info.queue_count;
    ++_info.total_count;
    std::vector<char> resultData = std::move(_messages.front().second);
    int resultCode = _messages.front().first;
    _messages.pop();
    return std::make_pair(resultCode, std::move(resultData));
}

intptr_t server_hserver_call_create_port(hserver_context& context, int32 queue_length, const char *name, size_t portNameLength)
{
    if (queue_length < 1 || queue_length > HAIKU_PORT_MAX_QUEUE_LENGTH)
    {
        return B_BAD_VALUE;
    }

    auto buffer = std::string(portNameLength, '\0');
    if (server_read_process_memory(context.pid, (void*)name, buffer.data(), portNameLength) != portNameLength)
    {
        return B_BAD_ADDRESS;
    }
    auto newPort = std::make_shared<Port>(context.pid, queue_length, buffer.c_str());

    int id;

    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();

        id = system.RegisterPort(std::move(newPort));
    }

    {
        auto lock = context.process->Lock();
        context.process->AddOwningPort(id);
    }

    return id;
}

intptr_t server_hserver_call_delete_port(hserver_context& context, int portId)
{
    auto& system = System::GetInstance();

    std::shared_ptr<Port> port;

    {
        auto lock = system.Lock();

        port = system.GetPort(portId).lock();
        if (!port)
        {
            return B_BAD_PORT_ID;
        }

    }

    {
        auto lock = context.process->Lock();
        context.process->RemoveOwningPort(portId);
    }

    {
        auto lock = system.Lock();
        system.UnregisterPort(portId);
    }
    return B_OK;
}

intptr_t server_hserver_call_find_port(hserver_context& context, const char *port_name, size_t portNameLength)
{
    auto buffer = std::string(portNameLength, '\0');
    if (server_read_process_memory(context.pid, (void*)port_name, buffer.data(), portNameLength) != portNameLength)
    {
        return B_BAD_ADDRESS;
    }

    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();

        int result = system.FindPort(buffer);

        if (result < 0)
        {
            return B_NAME_NOT_FOUND;
        }

        return result;
    }
}

intptr_t server_hserver_call_get_port_info(hserver_context& context, port_id id, void *info)
{
    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();

        auto port = system.GetPort(id).lock();

        if (!port)
        {
            return B_BAD_PORT_ID;
        }

        if (server_write_process_memory(context.pid, info, &port->GetInfo(), sizeof(haiku_port_info))
            != sizeof(haiku_port_info))
        {
            return B_BAD_ADDRESS;
        }

        return B_OK;
    }
}

intptr_t server_hserver_call_port_count(hserver_context& context, port_id id)
{
    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();

        auto port = system.GetPort(id).lock();

        if (!port)
        {
            return B_BAD_PORT_ID;
        }

        return port->GetInfo().queue_count;
    }
}

intptr_t server_hserver_call_port_buffer_size_etc(hserver_context& context, port_id id,
    uint32 flags, unsigned long long timeout)
{
    bool useTimeout = (bool)(flags & B_TIMEOUT);
    intptr_t result;

    std::weak_ptr<Port> weak_port;

    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();
        weak_port = system.GetPort(id).lock();
    }

    {
        auto port = weak_port.lock();

        if (!port)
        {
            return B_BAD_PORT_ID;
        }

        {
            auto lock = port->Lock();
            if (port->GetInfo().queue_count > 0)
            {
                result = port->GetBufferSize();
                goto good;
            }
        }
    }

    if (useTimeout && (timeout == 0))
    {
        return B_WOULD_BLOCK;
    }

    while (!useTimeout || (timeout > 0))
    {
        server_worker_sleep(kSleepTimeMicroseconds);
        {
            auto port = weak_port.lock();

            if (!port)
            {
                return B_BAD_PORT_ID;
            }

            {
                auto lock = port->Lock();
                if (port->GetInfo().queue_count > 0)
                {
                    result = port->GetBufferSize();
                    goto good;
                }
            }
        }
        if (timeout > kSleepTimeMicroseconds)
        {
            timeout -= kSleepTimeMicroseconds;
        }
        else
        {
            return B_TIMED_OUT;
        }
    }

    // Should not reach here.
    std::cerr << "server_hserver_call_port_buffer_size_etc: Impossible code path reached." << std::endl;
    return B_BAD_DATA;

good:
    return result;
}

intptr_t server_hserver_call_set_port_owner(hserver_context& context, port_id id, team_id team)
{
    std::shared_ptr<Port> port;
    std::shared_ptr<Process> oldOwner;
    std::shared_ptr<Process> newOwner;
    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();

        port = system.GetPort(id).lock();

        if (!port)
        {
            return B_BAD_PORT_ID;
        }

        oldOwner = system.GetProcess(port->GetInfo().team).lock();

        // Owner dead so port probably dead?
        if (!oldOwner)
        {
            std::cerr << "Port " << port->GetName() << " owned by dead process " << port->GetInfo().team << std::endl;
            return B_BAD_PORT_ID;
        }

        newOwner = system.GetProcess(team).lock();

        if (!newOwner)
        {
            return B_BAD_TEAM_ID;
        }
    }

    {
        auto lock = oldOwner->Lock();
        oldOwner->RemoveOwningPort(id);
    }

    {
        auto lock = newOwner->Lock();
        newOwner->AddOwningPort(id);
    }

    {
        auto lock = port->Lock();
        port->SetOwner(team);
    }

    return B_OK;
}

intptr_t server_hserver_call_write_port_etc(hserver_context& context, port_id id, int32 messageCode, const void *msgBuffer,
    size_t bufferSize, uint32 flags, unsigned long long timeout)
{
    std::weak_ptr<Port> weak_port;

    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();
        weak_port = system.GetPort(id);
    }

    std::vector<char> message(bufferSize);
    if (server_read_process_memory(context.pid, (void*)msgBuffer, message.data(), bufferSize)
        != bufferSize)
    {
        return B_BAD_ADDRESS;
    }

    {
        auto port = weak_port.lock();

        if (!port)
        {
            return B_BAD_PORT_ID;
        }

        {
            auto lock = port->Lock();
            if (port->GetInfo().queue_count < port->GetInfo().capacity)
            {
                port->Write(messageCode, std::move(message));
                return B_OK;
            }
        }
    }

    bool useTimeout = (bool)(flags & B_TIMEOUT);

    if (useTimeout && (timeout == 0))
    {
        return B_WOULD_BLOCK;
    }

    // TODO: Use some kind of condition variable?
    while (!useTimeout || (timeout > 0))
    {
        server_worker_sleep(kSleepTimeMicroseconds);
        {
            auto port = weak_port.lock();

            if (!port)
            {
                return B_BAD_PORT_ID;
            }

            {
                auto lock = port->Lock();
                if (port->GetInfo().queue_count < port->GetInfo().capacity)
                {
                    port->Write(messageCode, std::move(message));
                    return B_OK;
                }
            }
        }
        if (timeout > kSleepTimeMicroseconds)
        {
            timeout -= kSleepTimeMicroseconds;
        }
        else
        {
            return B_TIMED_OUT;
        }
    }

    // Should not reach here.
    std::cerr << "server_hserver_call_write_port_etc: Impossible code path reached." << std::endl;
    return B_BAD_DATA;
}

intptr_t server_hserver_call_read_port_etc(hserver_context& context,
    port_id id, int32* userMessageCode, void *msgBuffer,
    size_t bufferSize, uint32 flags, unsigned long long timeout)
{
    bool useTimeout = (bool)(flags & B_TIMEOUT);

    std::weak_ptr<Port> weak_port;

    {
        auto& system = System::GetInstance();
        auto lock = system.Lock();
        weak_port = system.GetPort(id);
    }

    int32 messageCode;
    std::vector<char> message;

    {
        auto port = weak_port.lock();

        if (!port)
        {
            return B_BAD_PORT_ID;
        }

        {
            auto lock = port->Lock();
            if (port->GetInfo().queue_count > 0)
            {
                std::tie(messageCode, message) = port->Read();
                goto good;
            }
        }
    }

    if (useTimeout && (timeout == 0))
    {
        return B_WOULD_BLOCK;
    }

    while (!useTimeout || (timeout > 0))
    {
        server_worker_sleep(kSleepTimeMicroseconds);
        {
            auto port = weak_port.lock();

            if (!port)
            {
                return B_BAD_PORT_ID;
            }

            {
                auto lock = port->Lock();
                if (port->GetInfo().queue_count > 0)
                {
                    std::tie(messageCode, message) = port->Read();
                    goto good;
                }
            }
        }
        if (timeout > kSleepTimeMicroseconds)
        {
            timeout -= kSleepTimeMicroseconds;
        }
        else
        {
            return B_TIMED_OUT;
        }
    }

    // Should not reach here.
    std::cerr << "server_hserver_call_read_port_etc: Impossible code path reached." << std::endl;
    return B_BAD_DATA;

good:
    if (context.process->WriteMemory(&userMessageCode, &messageCode, sizeof(messageCode)) != sizeof(int32))
    {
        return B_BAD_ADDRESS;
    }

    if (context.process->WriteMemory(msgBuffer, message.data(), std::min(message.size(), bufferSize)) != message.size())
    {
        return B_BAD_ADDRESS;
    }

    return B_OK;
}
