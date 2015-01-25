#pragma once

#include <Windows.h>
#include <functional>
#include "Utils.h"

using std::string;

class Thread
{
    NO_COPY_AND_ASSIGN(Thread)

public:
    typedef std::function<void ()> ThreadFunc;

    explicit Thread(const ThreadFunc& threadFunc, const string& threadname = string());

    ~Thread();

    void start();

    bool join();

    bool started() const;
    
    DWORD tid() const;
    
    const string& name() const;
  
    static int numCreated();
    
private:
    HANDLE          threadHandle_;
    ThreadFunc      threadFunc_;
    string          name_;
    unsigned int    tid_;
    static LONG     numCreated_;
};