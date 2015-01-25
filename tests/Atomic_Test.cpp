#include "stdafx.h"
#include "test.h"
#include "Atomic.h"
#include <assert.h>

void testAtomic()
{
    AtomicInt32 i;
    assert(i.get() == 0);

    i.add(3);
    assert(i.get() == 3);

    assert(i.getAndSet(0) == 3);

    assert(i.addAndGet(4) == 4);

    assert(i.getAndSet(0) == 4);

    assert(i.getAndAdd(4) == 0);

    assert(i.get() == 4);

    i.getAndSet(8);
    assert(i.incrementAndGet() == 9);

    assert(i.decrementAndGet() == 8);

    i.increment();
    assert(i.get() == 9);

    i.decrement();
    assert(i.get() == 8);

    printf("%s pass\n", __FUNCTION__);
}

struct Initializer
{
    Initializer()
    {
        TestCases::instance().addTestFunc(testAtomic);
    }
} initializer;
