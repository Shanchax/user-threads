#include "lock.h"

void mythread_spinlock_init(mythread_spinlock *my_lock) {
    my_lock->flag=0;
}



void mythread_mutex_init(mythread_mutex *my_lock) {
    my_lock->flag=0;
}


