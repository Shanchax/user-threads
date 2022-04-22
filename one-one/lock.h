#include <stdio.h>
#include <stdlib.h>
#include <linux/futex.h>
#include <unistd.h>
#include <sys/syscall.h>

typedef struct mythread_spinlock {
    /*This is lock_status responsible for Spinlock
        lock_status : 1 == Spinlock Aquired
        lock_status : 0 == Spinlock Released or Unblocked 
    */
    int lock_status; 
} mythread_spinlock;

typedef struct myhread_mutex {
    /*This is lock_status responsible for Spinlock
        lock_status : 1 == Spinlock Aquired
        lock_status : 0 == Spinlock Released or Unblocked 
    */
    int lock_status;
} mythread_mutex;


// Spinlock prototypes

//Spinlock Initialization
void mythread_spinlock_init(mythread_spinlock *);
//Spinlock Aquire Function Defination
int mythread_spinlock_lock_aquire(mythread_spinlock *);
//Spinlock Released Function Defination
int mythread_spinlock_unlock_released(mythread_spinlock *);

//Mutex Lock Supportive Function Definations

//Mutex Lock Initialization
void mythread_mutex_init(mythread_mutex *);
//Mutex Lock Aquire Function Defination
int mythread_mutex_lock_aquire(mythread_mutex *);
//Mutex Lock Released Function Defination
int mythread_mutex_lock_released(mythread_mutex *);



//Reference - https://github.com/mit-pdos/xv6-public (XV6 Context Saving and then switching)

//Responsible for Flag in currently passed context and return previously set value
static int set_context_xchg(int *flag_cxt) {
    int old_set_value;
    int set_bit = 1;
    asm("lock xchgl %0, %1":"+m"(*flag_cxt),"=a"(old_set_value):"1"(set_bit):"cc");
    return old_set_value;
}


//Responsile for waking up single process in futex mass
static int futex_wake_proc(void *address) {
    return syscall(SYS_futex, address, FUTEX_WAKE, 1, NULL, NULL, 0);
}
//Responsible system call to make parent waiting till futex_block not changes to zero
static int futex_halt_till(void *address, int thread_id) {
    return syscall(SYS_futex, address,FUTEX_WAIT, thread_id, NULL, NULL, 0);
}