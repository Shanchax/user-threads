#include <stdio.h>
#include <stdlib.h>
#include <linux/futex.h>
#include <unistd.h>
#include <sys/syscall.h>

typedef struct mythread_spinlock {
    /*This is flag responsible for Spinlock
        flag : 1 == Spinlock Aquired
        flag : 0 == Spinlock Released or Unblocked 
    */
    int flag; 
} mythread_spinlock;

typedef struct myhread_mutex {
    /*This is flag responsible for Spinlock
        flag : 1 == Spinlock Aquired
        flag : 0 == Spinlock Released or Unblocked 
    */
    int flag;
} mythread_mutex;



//Mutex Lock Supportive Function Definations

//Mutex Lock Initialization
void mythread_mutex_init(mythread_mutex *);
//Mutex Lock Aquire Function Defination
int mythread_mutex_lock_aquire(mythread_mutex *);
//Mutex Lock Released Function Defination
int mythread_mutex_lock_released(mythread_mutex *);

// Spinlock prototypes

//Spinlock Initialization
void mythread_spinlock_init(mythread_spinlock *);
//Spinlock Aquire Function Defination
int mythread_spinlock_lock_aquire(mythread_spinlock *);
//Spinlock Released Function Defination
int mythread_spinlock_unlock_released(mythread_spinlock *);




