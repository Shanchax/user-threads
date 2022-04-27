#include "lock.h"
//SpinLock Lock Initialization
void mythread_spinlock_init(mythread_spinlock *my_lock) {
    my_lock->lock_status=UNLOCKED;
}


//Critical Secrion Locking with Spinlock's help [Wrapper Locking]
int mythread_spinlock_lock_aquire(mythread_spinlock *spin_lk) {
    int true_status=1;
    int flag_sts;
    while (true_status) {
        flag_sts = set_context_xchg(&spin_lk->lock_status);
        //When flag_sts (status) flipped to 0, we can aquire resource. 
        if (flag_sts == UNLOCKED){
            break;
        }
        futex_halt_till(&spin_lk->lock_status, LOCKED);
    }
    return 1;
}

//Critical Secrion Unlocking with Spinlock's help [Wrapper Unlocking]
int mythread_spinlock_lock_released(mythread_spinlock *spin_lk) {
    int success_ret=1;
    spin_lk->lock_status = UNLOCKED;
    futex_wake_proc(&spin_lk->lock_status);
    return success_ret;
}

//Mutex Lock Initialization
void mythread_mutex_init(mythread_mutex *my_lock) {
    my_lock->lock_status=UNLOCKED;
}



//Critical Section Locking with Mutex's help [Wrapper Locking]
int mythread_mutex_lock_aquire(mythread_mutex *mutex_lk) {
    int true_status=1;
    int flag_sts;
    while (true_status) {
        flag_sts = set_context_xchg(&mutex_lk->lock_status);
        //When flag_sts (status) flipped to 0, we can aquire resource. 
        if (flag_sts == UNLOCKED){
            break;
        }
        futex_halt_till(&mutex_lk->lock_status, LOCKED);
    }
    return 1;
}

//Critical Section Unlocking with Mutex's help [Wrapper Unlocking]
int mythread_mutex_lock_released(mythread_mutex *mutex_lk) {
    int success_ret=1;
    mutex_lk->lock_status = UNLOCKED;
    futex_wake_proc(&mutex_lk->lock_status);
    return success_ret;
}


