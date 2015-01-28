#include "stdafx.h"
#include "test.h"
#include "Date.h"
#include <assert.h>
#include <iostream>

using namespace std;


void testDate()
{
    // Invalid Date
    {
        Date d(0, 1, 1);
        cout << d.julianDayNumber() << endl;
        cout << d.year() << endl;
        cout << d.month() << endl;
        cout << d.day() << endl;
        cout << d.weekDay() << endl;
        cout << d.toIsoString() << endl;
        cout << d.kJulianDayOf1970_01_01 << endl;
    }
    
}


struct InitTestDate
{
    InitTestDate()
    {
        TestCases::instance().addTestFunc(testDate);
    }
} a;
// 
// 
// struct Init
// {
//     Init() 
//     {
//         TestCases::instance().addTestFunc(testDate);
//     }
// } tmp;