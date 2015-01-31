// MuduoMock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CSLock.h"
#include "Condition.h"
#include "Atomic.h"
#include "Thread.h"
#include "TimeStamp.h"
#include "tests\test.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <stdlib.h>

using namespace std;

extern void testThreadPool();

int _tmain(int argc, _TCHAR* argv[])
{
    //TestCases::instance().runAllTests();
    testThreadPool();
	return 0;
}

