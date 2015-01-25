#pragma once

#include <vector>

class TestCases
{
public:
    static TestCases& instance();
    void addTestFunc(void (*testFunc)());
    void runAllTests();

private:
    typedef void (*TestFunc)();
    std::vector<TestFunc> testFuncs_;

};
