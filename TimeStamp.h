#pragma once

#include <string>
#include <algorithm>
#include <time.h>
#include "StringHelper.h"


using std::string;

// Time stamp in UTC, in microseconds resolution.
class Timestamp
{
public:
    enum { kMicroSecondsPerSecond = 1000*1000 };

    Timestamp()
        : microSecondsSinceEpoch_(0)
    {
        
    }

    explicit Timestamp(int64_t microSecondsSinceEpoch)
        : microSecondsSinceEpoch_(microSecondsSinceEpoch)
    {
    }

    void swap(Timestamp& that)
    {
        std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
    }

    // default copy/assignment/dtor are Okay

    string toString() const
    {
        int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
        int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
        return StringHelper::Format("%lld.%lld", seconds, microseconds);
    }

    string toFormattedString(bool showMicroseconds = true) const
    {
        time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
        tm tmTime;
        _gmtime64_s(&tmTime, &seconds);

        if (showMicroseconds)
        {
            int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
            return StringHelper::Format("%4d%02d%02d %02d:%02d:%02d.%06d",
                tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
                tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec,
                microseconds);
        }
        else
        {
            return StringHelper::Format("%4d%02d%02d %02d:%02d:%02d",
                tmTime.tm_year + 1900, tmTime.tm_mon + 1, tmTime.tm_mday,
                tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
        }
    }

    bool valid() const 
    { 
        return microSecondsSinceEpoch_ > 0; 
    }

    // for internal usage.
    int64_t microSecondsSinceEpoch() const 
    { 
        return microSecondsSinceEpoch_; 
    }

    time_t secondsSinceEpoch() const
    { 
        return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); 
    }

    ///
    /// Get time of now.
    ///
    static Timestamp now()
    {
        SYSTEMTIME utcTime;
        GetLocalTime(&utcTime);
        tm tmUTC;
        tmUTC.tm_year = utcTime.wYear - 1900;
        tmUTC.tm_mon = utcTime.wMonth - 1;
        tmUTC.tm_mday = utcTime.wDay;
        tmUTC.tm_hour = utcTime.wHour;
        tmUTC.tm_min = utcTime.wMinute;
        tmUTC.tm_sec = utcTime.wSecond;
        time_t secs = mktime(&tmUTC);
        return Timestamp(secs * kMicroSecondsPerSecond + 1000 * utcTime.wMilliseconds);
    }

    static Timestamp invalid()
    {
        return Timestamp();
    }

private:
    int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}