#include "stdafx.h"
#include <list>
#include "CSLock.h"
#include "Condition.h"
#include "Thread.h"
#include "test.h"

using namespace std;


list<int> g_intList;
CSLock    g_csLock;
Condition g_cond(g_csLock);

void threadFunc1()
{
    for(int i = 0; i < 10000; i++)
    {
        {
            CSGuard(g_csLock);
            g_intList.push_back(rand()%1000);
        }

        g_cond.notify();

        Sleep(50);
    }
}

void threadFunc2()
{
    DWORD dwOldTickCount = GetTickCount();
    while(1)
    {
        CSGuard(g_csLock);
        while(g_intList.empty())
        {
            if(!g_cond.wait(2000))
            {
                goto threadend;
            }
        }

        while(!g_intList.empty())
        {
            int val = g_intList.front();
            printf("%03d, %d\n", val, GetTickCount() - dwOldTickCount);
            dwOldTickCount = GetTickCount();
            g_intList.pop_front();
        }
    }
threadend:
    return;
}

void testThreadFunc()
{

    Thread thread1(threadFunc1);
    Thread thread2(threadFunc2);

    thread1.start();
    thread2.start();

    thread1.join();
    thread2.join();
}

struct Init{
    Init() {
        //addTestFunc(testThreadFunc);
    }
} initer;