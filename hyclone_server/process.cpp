#include <cstring>
#include <iostream>
#include <utility>

#include "area.h"
#include "haiku_errors.h"
#include "haiku_fcntl.h"
#include "process.h"
#include "server_native.h"
#include "server_servercalls.h"
#include "system.h"
#include "thread.h"

Process::Process(int pid, int uid, int gid, int euid, int egid)
    : _pid(pid), _uid(uid), _gid(gid), _euid(euid), _egid(egid),
    _forkUnlocked(false)
{
    memset(&_info, 0, sizeof(_info));
    _info.team = pid;
    _info.uid = uid;
    _info.gid = gid;
    _info.debugger_nub_thread = -1;
    _info.debugger_nub_port = -1;

    _debuggerPort = -1;
    _debuggerPid = -1;
    _debuggerWriteLock = -1;
}

std::weak_ptr<Thread> Process::RegisterThread(int tid)
{
    auto ptr = std::make_shared<Thread>(_pid, tid);
    _threads[tid] = ptr;
    _info.thread_count = _threads.size();
    return ptr;
}

std::weak_ptr<Thread> Process::RegisterThread(const std::shared_ptr<Thread>& thread)
{
    _threads[thread->GetTid()] = thread;
    _info.thread_count = _threads.size();
    return thread;
}

std::weak_ptr<Thread> Process::GetThread(int tid)
{
    auto it = _threads.find(tid);
    if (it == _threads.end())
        return std::weak_ptr<Thread>();
    return it->second;
}

size_t Process::UnregisterThread(int tid)
{
    _threads.erase(tid);
    _info.thread_count = _threads.size();
    return _threads.size();
}

int Process::RegisterImage(const haiku_extended_image_info& image)
{
    int id = _images.Add(image);
    _images.Get(id).basic_info.id = id;
    _info.image_count = _images.size();
    return id;
}

haiku_extended_image_info& Process::GetImage(int image_id)
{
    return _images.Get(image_id);
}

int Process::NextImageId(int image_id)
{
    return _images.NextId(image_id);
}

bool Process::IsValidImageId(int image_id)
{
    return _images.IsValidId(image_id);
}

size_t Process::UnregisterImage(int image_id)
{
    _images.Remove(image_id);
    _info.image_count = _images.size();
    return _images.Size();
}

std::weak_ptr<Area> Process::RegisterArea(const std::shared_ptr<Area>& area)
{
    return _areas[area->GetAreaId()] = area;
}

std::weak_ptr<Area> Process::GetArea(int area_id)
{
    auto it = _areas.find(area_id);
    if (it == _areas.end())
        return std::weak_ptr<Area>();
    return it->second;
}

int Process::GetAreaIdFor(void* address)
{
    auto& system = System::GetInstance();

    for (const auto& [areaId, area] : _areas)
    {
        if ((uint8_t*)area->GetInfo().address <= address &&
            address < (uint8_t*)area->GetInfo().address + area->GetInfo().size)
            return area->GetAreaId();
    }
    return -1;
}

int Process::GetNextAreaIdFor(void* address)
{
    auto& system = System::GetInstance();
    int id = -1;
    uintptr_t min = (uintptr_t)-1;

    for (const auto& [areaId, area] : _areas)
    {
        if ((uintptr_t)area->GetInfo().address > (uintptr_t)address)
        {
            if ((uintptr_t)area->GetInfo().address < min)
            {
                min = (uintptr_t)area->GetInfo().address;
                id = area->GetAreaId();
            }
        }
    }
    return id;
}

int Process::NextAreaId(int area_id)
{
    auto it = _areas.upper_bound(area_id);
    if (it == _areas.end())
        return -1;
    return it->first;
}

bool Process::IsValidAreaId(int area_id)
{
    return _areas.contains(area_id);
}

size_t Process::UnregisterArea(int area_id)
{
    _areas.erase(area_id);
    _info.area_count = _areas.size();
    return _areas.size();
}

size_t Process::RegisterFd(int fd, const std::filesystem::path& path)
{
    _fds[fd] = path;
    return _fds.size();
}

const std::filesystem::path& Process::GetFd(int fd)
{
    return _fds.at(fd);
}

bool Process::IsValidFd(int fd)
{
    return _fds.contains(fd);
}

size_t Process::UnregisterFd(int fd)
{
    _fds.erase(fd);
    return _fds.size();
}

void Process::Fork(Process& child)
{
    auto& system = System::GetInstance();

    // No, child has new PID.
    // child._pid = _pid;
    // No, child has its own threads.
    // child._threads = _threads;
    child._info = _info;

    child._images = _images;
    // Fork is called in a system lock
    // so we can safely access the areas.
    for (const auto& [areaId, area] : _areas)
    {
        auto registeredArea = std::make_shared<Area>(*area);
        registeredArea->Unshare();
        registeredArea->GetInfo().team = child._pid;
        registeredArea = system.RegisterArea(registeredArea).lock();
        child._areas[registeredArea->GetInfo().area] = registeredArea;
        if (area->IsShared() && area->GetMapping() == REGION_PRIVATE_MAP)
        {
            auto& memService = system.GetMemoryService();
            auto lock = memService.Lock();
            std::string hostPath;
            if (memService.CloneSharedFile(std::to_string(registeredArea->GetAreaId()),
                area->GetEntryRef(), hostPath))
            {
                EntryRef ref;
                if (memService.OpenSharedFile(hostPath, registeredArea->IsWritable(), ref))
                {
                    registeredArea->Share(ref, area->GetOffset());
                }
            }
        }
    }

    child._fds = _fds;
    child._cwd = _cwd;

    // No, semaphores don't seem to be inherited.
    // child._owningSemaphores = _owningSemaphores;

    child._forkUnlocked = true;
}

void Process::Exec()
{
    _isExecutingExec = true;
}

void Process::FinishExec()
{
    _isExecutingExec = false;
}

size_t Process::ReadMemory(void* address, void* buffer, size_t size)
{
    return server_read_process_memory(_pid, address, buffer, size);
}

size_t Process::WriteMemory(void* address, const void* buffer, size_t size)
{
    return server_write_process_memory(_pid, address, buffer, size);
}

status_t Process::ReadDirFd(int fd, const void* userBuffer, size_t userBufferSize, bool traverseLink, std::filesystem::path& output)
{
    std::string path(userBufferSize, '\0');
    if (ReadMemory((void*)userBuffer, path.data(), userBufferSize) != userBufferSize)
    {
        return B_BAD_ADDRESS;
    }

    if (path[0] != '/')
    {
        if (fd != HAIKU_AT_FDCWD)
        {
            if (!IsValidFd(fd))
            {
                return HAIKU_POSIX_EBADF;
            }
            output = GetFd(fd);
        }
        else
        {
            if (_cwd.empty())
            {
                // TODO: This should not happen, but somehow it does when running python3.10
                std::cerr << _info.team << " tried to access a VFS function before registering its cwd!" << std::endl;
                return HAIKU_POSIX_EBADF;
            }
            output = GetCwd();
        }

        if (!path.empty())
        {
            if (!output.has_filename())
            {
                output = output.parent_path() / path;
            }
            else
            {
                output /= path;
            }
        }
    }
    else
    {
        output = path;
    }

    if (traverseLink)
    {
        auto& vfsService = System::GetInstance().GetVfsService();
        auto lock = vfsService.Lock();
        vfsService.RealPath(output);
    }

    output = output.lexically_normal();

    return B_OK;
}

intptr_t server_hserver_call_register_fd(hserver_context& context, int fd, int parentFd,
    const char* userPath, size_t userPathSize, bool traverseSymlink)
{
    std::filesystem::path requestPath;

    status_t status = B_OK;

    {
        auto lock = context.process->Lock();
        status = context.process->ReadDirFd(parentFd, userPath, userPathSize, traverseSymlink, requestPath);
    }

    if (status == B_OK)
    {
        requestPath = requestPath.lexically_normal();
        context.process->RegisterFd(fd, requestPath);

        auto& vfsService = System::GetInstance().GetVfsService();
        auto lock = vfsService.Lock();

        haiku_stat stat;
        status = vfsService.ReadStat(requestPath, stat);

        if (status == B_OK)
        {
            auto entryRef = EntryRef(stat.st_dev, stat.st_ino);
            vfsService.RegisterEntryRef(entryRef, requestPath);
        }
    }

    return status;
}

intptr_t server_hserver_call_register_fd1(hserver_context& context, int fd,
    unsigned long long dev, unsigned long long ino, const char* userName, size_t userNameSize)
{
    std::filesystem::path requestPath;

    {
        std::string requestPathString;
        auto& vfsService = System::GetInstance().GetVfsService();
        auto lock = vfsService.Lock();
        if (!vfsService.GetEntryRef(EntryRef(dev, ino), requestPathString))
        {
            return B_ENTRY_NOT_FOUND;
        }
        requestPath = requestPathString;
    }

    {
        std::string name(userNameSize, '\0');
        auto lock = context.process->Lock();
        if (context.process->ReadMemory((void*)userName, name.data(), userNameSize) != userNameSize)
        {
            return B_BAD_ADDRESS;
        }
        if (!name.empty() && name != ".")
        {
            requestPath /= name;
        }
    }

    {
        auto lock = context.process->Lock();
        context.process->RegisterFd(fd, requestPath.lexically_normal());
    }

    return B_OK;
}

intptr_t server_hserver_call_register_parent_dir_fd(hserver_context& context, int fd, int originalFd, char* userName, size_t userNameSize)
{
    std::filesystem::path requestPath;

    {
        auto lock = context.process->Lock();
        if (!context.process->IsValidFd(originalFd))
        {
            return HAIKU_POSIX_EBADF;
        }

        const auto& originalPath = context.process->GetFd(originalFd);
        requestPath = originalPath.parent_path();
        std::string name = originalPath.filename().string();

        if (name.size() >= userNameSize)
        {
            return B_BUFFER_OVERFLOW;
        }

        if (context.process->WriteMemory(userName, name.c_str(), name.size() + 1) != name.size() + 1)
        {
            return B_BAD_ADDRESS;
        }

        context.process->RegisterFd(fd, requestPath);
    }

    status_t status;
    haiku_stat stat;
    auto& vfsService = System::GetInstance().GetVfsService();
    auto vfsLock = vfsService.Lock();

    status = vfsService.ReadStat(requestPath, stat);

    if (status == B_OK)
    {
        auto entryRef = EntryRef(stat.st_dev, stat.st_ino);
        vfsService.RegisterEntryRef(entryRef, requestPath);
    }

    return B_OK;
}

intptr_t server_hserver_call_register_dup_fd(hserver_context& context, int fd, int oldFd)
{
    auto lock = context.process->Lock();
    if (!context.process->IsValidFd(oldFd))
    {
        return HAIKU_POSIX_EBADF;
    }
    context.process->RegisterFd(fd, context.process->GetFd(oldFd));
    return B_OK;
}

intptr_t server_hserver_call_register_attr_fd(hserver_context& context, int fd, int parentFd,
    void* userPathAndSize, void* userNameAndSize, bool traverseSymlink)
{
    std::pair<const char*, size_t> pathAndSize;
    std::pair<const char*, size_t> nameAndSize;
    std::filesystem::path requestPath;
    std::string name;

    status_t status;

    {
        auto lock = context.process->Lock();
        if (context.process->ReadMemory(userPathAndSize, &pathAndSize, sizeof(pathAndSize)) != sizeof(pathAndSize))
        {
            return B_BAD_ADDRESS;
        }

        status = context.process->ReadDirFd(parentFd, pathAndSize.first, pathAndSize.second, traverseSymlink, requestPath);

        if (status != B_OK)
        {
            return status;
        }

        if (context.process->ReadMemory(userNameAndSize, &nameAndSize, sizeof(nameAndSize)) != sizeof(nameAndSize))
        {
            return B_BAD_ADDRESS;
        }

        name.resize(nameAndSize.second);
        if (context.process->ReadMemory((void*)nameAndSize.first, name.data(), name.size()) != name.size())
        {
            return B_BAD_ADDRESS;
        }
    }

    {
        auto& vfsService = System::GetInstance().GetVfsService();
        auto lock = vfsService.Lock();

        status = vfsService.GetAttrPath(requestPath, name, 0, false, false);
    }

    if (status != B_OK)
    {
        return status;
    }

    {
        auto lock = context.process->Lock();
        context.process->RegisterFd(fd, requestPath);
    }

    return B_OK;
}

intptr_t server_hserver_call_unregister_fd(hserver_context& context, int fd)
{
    auto lock = context.process->Lock();
    context.process->UnregisterFd(fd);
    return B_OK;
}

intptr_t server_hserver_call_setcwd(hserver_context& context, int fd, const char* userPath, size_t userPathSize)
{
    std::filesystem::path requestPath;

    status_t status = B_OK;

    {
        auto lock = context.process->Lock();
        status = context.process->ReadDirFd(fd, userPath, userPathSize, true, requestPath);

        if (status == B_OK)
        {
            // This is a hook called after the real setcwd syscall, so no check needs to be done.
            context.process->SetCwd(requestPath.lexically_normal());
        }
    }

    return status;
}

intptr_t server_hserver_call_getcwd(hserver_context& context, char* userBuffer, size_t userSize)
{
    auto lock = context.process->Lock();
    auto cwdString = context.process->GetCwd().string();
    if (cwdString.size() >= userSize)
    {
        return B_BUFFER_OVERFLOW;
    }

    size_t writeBytes = cwdString.size() + 1;
    if (context.process->WriteMemory(userBuffer, cwdString.c_str(), writeBytes) != writeBytes)
    {
        return B_BAD_ADDRESS;
    }

    return B_OK;
}

intptr_t server_hserver_call_get_team_info(hserver_context& context, int team_id, void* userTeamInfo)
{
    haiku_team_info info;

    if (team_id == B_SYSTEM_TEAM)
    {
        memset(&info, 0, sizeof(info));
        info.debugger_nub_thread = -1;
        info.debugger_nub_port = -1;
        server_fill_team_info(&info);
    }
    else
    {
        std::shared_ptr<Process> process;

        if (team_id == B_CURRENT_TEAM)
        {
            process = context.process;
        }
        else
        {
            auto& system = System::GetInstance();
            auto lock = system.Lock();
            process = system.GetProcess(team_id).lock();
        }

        if (!process)
        {
            return B_BAD_TEAM_ID;
        }

        {
            auto lock = process->Lock();
            info = process->GetInfo();
        }
        server_fill_team_info(&info);
    }

    if (server_write_process_memory(context.pid, userTeamInfo, &info, sizeof(haiku_team_info))
        != sizeof(haiku_team_info))
    {
        return B_BAD_ADDRESS;
    }

    return B_OK;
}

intptr_t server_hserver_call_get_next_team_info(hserver_context& context, int32_t* userCookie, void* userTeamInfo)
{
    int32_t cookie;
    if (server_read_process_memory(context.pid, userCookie, &cookie, sizeof(cookie))
        != sizeof(cookie))
    {
        return B_BAD_ADDRESS;
    }

    if (cookie == 0)
    {
        server_hserver_call_get_team_info(context, B_SYSTEM_TEAM, userTeamInfo);
        cookie = -1;
    }
    else
    {
        {
            auto& system = System::GetInstance();
            auto lock = system.Lock();
            cookie = system.NextProcessId(cookie);
            if (cookie < 0)
            {
                return B_BAD_VALUE;
            }
        }

        status_t status = server_hserver_call_get_team_info(context, cookie, userTeamInfo);

        if (status != B_OK)
        {
            return status;
        }
    }

    if (server_write_process_memory(context.pid, userCookie, &cookie, sizeof(cookie))
        != sizeof(cookie))
    {
        return B_BAD_ADDRESS;
    }

    return B_OK;
}

intptr_t server_hserver_call_register_team_info(hserver_context& context, void* userTeamInfo)
{
    haiku_team_info info;
    if (server_read_process_memory(context.pid, userTeamInfo, &info, sizeof(haiku_team_info))
        != sizeof(haiku_team_info))
    {
        return B_BAD_ADDRESS;
    }

    if (info.team != context.pid)
    {
        return B_NOT_ALLOWED;
    }

    {
        auto lock = context.process->Lock();
        context.process->GetInfo() = info;
    }

    return B_OK;
}

intptr_t server_hserver_call_getuid(hserver_context& context, bool effective)
{
    auto lock = context.process->Lock();
    return effective ? context.process->GetEuid() : context.process->GetUid();
}

intptr_t server_hserver_call_getgid(hserver_context& context, bool effective)
{
    auto lock = context.process->Lock();
    return effective ? context.process->GetEgid() : context.process->GetGid();
}