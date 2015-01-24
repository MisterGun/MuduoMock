#pragma once

#define NO_COPY_AND_ASSIGN(T) \
    private: \
    T(const T&); \
    T& operator=(const T&);

#define STRINGIFY1(x) #x
#define STRINGIFY2(x) STRINGIFY1(x)

#define CATENATE1(x, y) x ## y
#define CATENATE2(x, y) CATENATE1(x, y)