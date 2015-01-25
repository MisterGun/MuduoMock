#include "stdafx.h"
#include "test.h"



TestCases& TestCases::instance()
{
    static TestCases testCases;
    return testCases;
}

void TestCases::addTestFunc( void (*testFunc)() )
{
    testFuncs_.push_back(testFunc);
}

void TestCases::runAllTests()
{
    for(size_t i = 0; i < testFuncs_.size(); i++)
    {
        testFuncs_[i]();
    }
}