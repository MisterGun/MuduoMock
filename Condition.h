#pragma once

#include "CSLock.h"

class Condition
{
public:
    
    explicit Condition(CSLock& csLck)
        : csLck_(csLck)
    {
        InitializeConditionVariable(&cond_);
    }

    void wait(DWORD ms = INFINITE)
    {
        SleepConditionVariableCS(&cond_, csLck_.getCS(), ms);
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
    CSLock csLck_;
    CONDITION_VARIABLE  cond_;
};

