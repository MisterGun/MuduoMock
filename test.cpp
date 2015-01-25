#include "stdafx.h"
#include "test.h"

using namespace std;

typedef void (*testFunc)();


vector<testFunc>* pTestFuncs = NULL;

void addTestFunc(testFunc t)
{
    static vector<testFunc> g_testFuncs;
    pTestFuncs = &g_testFuncs;

    g_testFuncs.push_back(t);
}

void runAllTests()
{
    for(size_t i = 0; i < (*pTestFuncs).size(); i++)
    {
        (*pTestFuncs)[i]();
    }
}
