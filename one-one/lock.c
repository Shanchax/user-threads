#include "lock.h"
//SpinLock Lock Initialization
void mythread_spinlock_init(mythread_spinlock *my_lock) {
    my_lock->lock_status=0;
}


//Mutex Lock Initialization
void mythread_mutex_init(mythread_mutex *my_lock) {
    my_lock->lock_status=0;
}



//Critical Secrion Locking with Mutex's help [Wrapper Locking]
int mythread_mutex_lock_aquire(mythread_mutex *mutex_lk) {
    int true_status=1;
    int flag_sts;
    while (true_status) {
        flag_sts = set_context_xchg(&mutex_lk->lock_status);
        //When flag_sts (status) flipped to 0, we can aquire resource. 
        if (flag_sts == 0){
            break;
        }
        futex_halt_till(&mutex_lk->lock_status, 1);
    }
    return 1;
}

//Critical Secrion Unlocking with Mutex's help [Wrapper Unlocking]
int mythread_mutex_lock_released(mythread_mutex *mutex_lk) {
    mutex_lk->lock_status = 0;
    futex_wake_proc(&mutex_lk->lock_status);
    return 1;
}


