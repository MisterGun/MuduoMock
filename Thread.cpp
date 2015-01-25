#include "stdafx.h"
#include "Thread.h"
#include "process.h"
#include "StringHelper.h"
#include <assert.h>

namespace threadDetail
{
    unsigned __stdcall threadProc(void *arg)
    {
        Thread::ThreadFunc* pFunc = (Thread::ThreadFunc*)(arg);
        if(!pFunc)
        {
            return 0;
        }

        (*pFunc)();

        return 0;
    }
}

LONG Thread::numCreated_ = 0;

Thread::Thread(const ThreadFunc& threadFunc, const string& name /*= string()*/)
    : threadHandle_(NULL)
    , threadFunc_(threadFunc)
    , name_(name)
    , tid_(0)
{
    InterlockedIncrement(&numCreated_);
    if(name_.empty())
    {
        name_ = StringHelper::Format("thread_%d", numCreated_);
    }
}

Thread::~Thread()
{
    if(threadHandle_)
    {
        CloseHandle(threadHandle_);
        threadHandle_ = NULL;
    }
}

void Thread::start()
{
    threadHandle_ = (HANDLE)_beginthreadex(NULL, 0, threadDetail::threadProc, (void*)(&threadFunc_), 0, &tid_);
    
    assert(threadHandle_ != 0);
}

bool Thread::join()
{
    DWORD ret = WaitForSingleObject(threadHandle_, INFINITE);
    return ret == WAIT_OBJECT_0;
}

bool Thread::started() const
{
    return threadHandle_ != 0;
}

DWORD Thread::tid() const
{ 
    return tid_; 
}

const string& Thread::name() const
{ 
    return name_; 
}

int Thread::numCreated()
{ 
    return numCreated_; 
}
