#pragma once

#include <windows.h>
#include "Utils.h"

class CSLock
{
    NO_COPY_AND_ASSIGN(CSLock)
public:
    CSLock(void)
    {
        InitializeCriticalSection(&cs_);
    }

    ~CSLock(void)
    {
        DeleteCriticalSection(&cs_);
    }

    void lock() 
    {
        EnterCriticalSection(&cs_);
    }

    void unlock()
    {
        LeaveCriticalSection(&cs_);
    }

    CRITICAL_SECTION* getCS()
    {
        return &cs_;
    }

private:
    CRITICAL_SECTION cs_;
};


template<typename LockType>
class LockGuard
{
    NO_COPY_AND_ASSIGN(LockGuard)
public:
    LockGuard(LockType& lck)
        :lck_(lck)
    {
        lck_.lock();
    }

    ~LockGuard()
    {
        lck_.unlock();
    }



private:
    LockType& lck_;
};

#define CSGuard(cs) LockGuard<CSLock> CATENATE2(UniqueLockVariable, __COUNTER__)(cs);