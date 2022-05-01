#include "lock.h"

// Sets number in the passed address to 1 and returns the previous value
static int test_and_set(int *flag)
{
    int result, set = 1;
    asm("lock xchgl %0, %1"
        : "+m"(*flag), "=a"(result)
        : "1"(set)
        : "cc");
    return result;
}

void thread_spinlock_init(thread_spinlock *lock)
{
    lock->flag = 0;
}

int thread_spinlock_lock(thread_spinlock *sl)
{
    while (1)
    {
        int flag_status = test_and_set(&sl->flag);
        if (flag_status == 0)
        {
            break;
        }
    }
    return 1;
}

int thread_spinlock_unlock(thread_spinlock *sl)
{
    sl->flag = 0;
    return 1;
}