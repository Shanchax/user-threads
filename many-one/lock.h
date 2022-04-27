#ifndef LOCK_H
#define LOCK_H


typedef struct thread_spinlock
{
    int flag; //Tells if the lock is locked=1 or unlocked=0
} thread_spinlock;

void thread_spinlock_init(thread_spinlock *lock);

int thread_spinlock_lock(thread_spinlock *sl);

int thread_spinlock_unlock(thread_spinlock *sl);

#endif