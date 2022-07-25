#ifndef __HYCLONE_SEMAPHORE_H__
#define __HYCLONE_SEMAPHORE_H__

// This file is named hsemaphore to avoid naming
// conflicts with the system "semaphore.h" header.

#include <atomic>
#include <mutex>

#include "haiku_sem.h"

class Semaphore
{
    friend class System;
private:
    haiku_sem_info _info;
    std::mutex _countLock;
    std::atomic<bool> _registered = false;
    int _count;
public:
    Semaphore(int pid, int count, const char* name);
    ~Semaphore() = default;

    int GetCount() const { return _info.count; }
    int GetId() const { return _info.sem; }
    const char* GetName() const { return _info.name; }
    int GetOwningTeam() const { return _info.team; }

    void Acquire(int tid, int count);
    int TryAcquire(int tid, int count);
    int TryAcquireFor(int tid, int count, int64_t timeout);
    int TryAcquireUntil(int tid, int count, int64_t timestamp);
    void Release(int count);
};

#endif // __HYCLONE_SEMAPHORE_H__