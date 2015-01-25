#pragma once

#include <stdint.h>

struct DateTime
{
    int				    year;
    int				    month;
    int				    dayOfWeek;
    int				    day;
    int				    hour;
    int				    minute;
    int				    second;
    int				    milliseconds;

    int64_t			    totalMilliseconds;
    int64_t			    filetime;

    static DateTime		LocalTime();
    static DateTime		UtcTime();
    static DateTime		FromDateTime(int _year, int _month, int _day, int _hour = 0, int _minute = 0, int _second = 0, int _milliseconds = 0);
    static DateTime		FromFileTime(__int64 filetime);

    DateTime();

    DateTime			ToLocalTime();
    DateTime			ToUtcTime();
    DateTime			Forward(int64_t milliseconds);
    DateTime			Backward(int64_t milliseconds);

    bool operator==(const DateTime& value)const { return filetime == value.filetime; }
    bool operator!=(const DateTime& value)const { return filetime != value.filetime; }
    bool operator<(const DateTime& value)const { return filetime < value.filetime; }
    bool operator<=(const DateTime& value)const { return filetime <= value.filetime; }
    bool operator>(const DateTime& value)const { return filetime > value.filetime; }
    bool operator>=(const DateTime& value)const { return filetime >= value.filetime; }

private:
    static DateTime SystemTimeToDateTime(const SYSTEMTIME& systemTime);
    static SYSTEMTIME DateTimeToSystemTime(const DateTime& dateTime);
};



DateTime DateTime::SystemTimeToDateTime(const SYSTEMTIME& systemTime)
{
    DateTime dateTime;
    dateTime.year = systemTime.wYear;
    dateTime.month = systemTime.wMonth;
    dateTime.dayOfWeek = systemTime.wDayOfWeek;
    dateTime.day = systemTime.wDay;
    dateTime.hour = systemTime.wHour;
    dateTime.minute = systemTime.wMinute;
    dateTime.second = systemTime.wSecond;
    dateTime.milliseconds = systemTime.wMilliseconds;

    FILETIME fileTime;
    SystemTimeToFileTime(&systemTime, &fileTime);
    ULARGE_INTEGER largeInteger;
    largeInteger.HighPart = fileTime.dwHighDateTime;
    largeInteger.LowPart = fileTime.dwLowDateTime;
    dateTime.filetime = largeInteger.QuadPart;
    dateTime.totalMilliseconds = dateTime.filetime / 10000;

    return dateTime;
}

SYSTEMTIME DateTime::DateTimeToSystemTime(const DateTime& dateTime)
{
    ULARGE_INTEGER largeInteger;
    largeInteger.QuadPart = dateTime.filetime;
    FILETIME fileTime;
    fileTime.dwHighDateTime = largeInteger.HighPart;
    fileTime.dwLowDateTime = largeInteger.LowPart;

    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);
    return systemTime;
}

DateTime DateTime::LocalTime()
{
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);
    return SystemTimeToDateTime(systemTime);
}

DateTime DateTime::UtcTime()
{
    SYSTEMTIME utcTime;
    GetSystemTime(&utcTime);
    return SystemTimeToDateTime(utcTime);
}

DateTime DateTime::FromDateTime(int _year, int _month, int _day, int _hour, int _minute, int _second, int _milliseconds)
{

    SYSTEMTIME systemTime;
    memset(&systemTime, 0, sizeof(systemTime));
    systemTime.wYear = (WORD)_year;
    systemTime.wMonth = (WORD)_month;
    systemTime.wDay = (WORD)_day;
    systemTime.wHour = (WORD)_hour;
    systemTime.wMinute = (WORD)_minute;
    systemTime.wSecond = (WORD)_second;
    systemTime.wMilliseconds = (WORD)_milliseconds;

    FILETIME fileTime;
    SystemTimeToFileTime(&systemTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &systemTime);
    return SystemTimeToDateTime(systemTime);

}

DateTime DateTime::FromFileTime(__int64 filetime)
{
    ULARGE_INTEGER largeInteger;
    largeInteger.QuadPart = filetime;
    FILETIME fileTime;
    fileTime.dwHighDateTime = largeInteger.HighPart;
    fileTime.dwLowDateTime = largeInteger.LowPart;

    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);
    return SystemTimeToDateTime(systemTime);
}

DateTime::DateTime()
    : year(0)
    , month(0)
    , day(0)
    , hour(0)
    , minute(0)
    , second(0)
    , milliseconds(0)
    , filetime(0)
{
}

DateTime DateTime::ToLocalTime()
{
    SYSTEMTIME utcTime = DateTimeToSystemTime(*this);
    SYSTEMTIME localTime;
    SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime);
    return SystemTimeToDateTime(localTime);
}

DateTime DateTime::ToUtcTime()
{
    SYSTEMTIME localTime = DateTimeToSystemTime(*this);
    SYSTEMTIME utcTime;
    TzSpecificLocalTimeToSystemTime(NULL, &localTime, &utcTime);
    return SystemTimeToDateTime(utcTime);
}

DateTime DateTime::Forward(int64_t milliseconds)
{
    return FromFileTime(filetime + milliseconds * 10000);
}

DateTime DateTime::Backward(int64_t milliseconds)
{
    return FromFileTime(filetime - milliseconds * 10000);
}