#include "stdafx.h"
#include "test.h"
#include <vector>

using namespace std;

typedef void (*TestFunc)();
typedef vector<TestFunc> TestFuncArray;


static std::unique_ptr<TestFuncArray> s_spTestFuncs;

void addTestFunc(TestFunc t)
{
    if(!s_spTestFuncs)
    {
        s_spTestFuncs.reset(new TestFuncArray);
    }

    s_spTestFuncs->push_back(t);
}

void runAllTests()
{
    for(size_t i = 0; i < (*s_spTestFuncs).size(); i++)
    {
        (*s_spTestFuncs)[i]();
    }
}
