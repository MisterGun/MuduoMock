#pragma once

#include "CSLock.h"
#include "Utils.h"
#include <assert.h>

class Condition
{
    NO_COPY_AND_ASSIGN(Condition)
public:
    
    explicit Condition(CSLock& csLck)
        : csLck_(csLck)
    {
        InitializeConditionVariable(&cond_);
    }

    bool wait(DWORD ms = INFINITE)
    {
        BOOL bWaitOK = SleepConditionVariableCS(&cond_, csLck_.getCS(), ms);
        assert(bWaitOK || GetLastError() == ERROR_TIMEOUT);

        return bWaitOK ? true : false;
    }

    void notify()
    {
        WakeConditionVariable(&cond_);
    }

    void notifyAll()
    {
        WakeAllConditionVariable(&cond_);
    }

private:
    CSLock& csLck_;
    CONDITION_VARIABLE  cond_;
};

