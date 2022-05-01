#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h> 
#include <ucontext.h>
#include <sys/types.h>
#include <time.h>
#include "lock.h"
#include "queue.h"

//using a static sized queue here, not ideal at all. but this was just a trial run on many-many.
#define DEFAULT_Q 20

// structure for all user thread related data
typedef struct TPROC{
    /* priority assosciated with this thread*/
    int priority;
    time_t start;
    int pid;
    void (*func)();
    ucontext_t* ctx;
} TPROC;



// managed active and waiting threads
typedef struct TPROC_M{
    QUEUE* q;
    TPROC** threads;
    int size;
    int kactive;
    int kmax;
    thread_spinlock lock;
    
} TPROC_M;
TPROC_M* TPROC_PTR;


TPROC*    get_mythread(int t_index);
int         thread_from_pid(int pid);
void        mythread_run(int t_index);


void        delete_tcb();
void        delete_queue(QUEUE* q_ptr);