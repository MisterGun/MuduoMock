#pragma once

#include "CSLock.h"
#include "Condition.h"


class CountDownLatch
{
    NO_COPY_AND_ASSIGN(CountDownLatch)
public:
    explicit CountDownLatch(int count) 
        : count_(count)
        , lck_()
        , condition_(lck_)
    {
    }

    void wait()
    {
        CSGuard(lck_);

        while(count_ > 0)
        {
            condition_.wait();
        }
    }

    void countDown()
    {
        CSGuard(lck_);

        --count_;
        if(count_ == 0)
        {
            condition_.notifyAll();
        }
    }

    int getCount()
    {
        CSGuard(lck_);

        return count_;
    }

private:
    int count_;
    CSLock lck_;
    Condition condition_;
};