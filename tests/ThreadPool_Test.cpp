#include "stdafx.h"
#include "ThreadPool.h"
#include "test.h"


void threadFunc()
{
    Sleep(50);
    printf("current thread id : %d\n", GetCurrentThreadId());
}

void testThreadPool()
{
    ThreadPool threadPool;
    threadPool.start(50);
    for(int i = 0; i < 100; i++)
    {
        threadPool.add(std::bind(&threadFunc));
    }
}
