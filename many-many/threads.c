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
//not expecting this to run
#define DEFAULT_Q 20
#define STACKSIZE (64*1024)

// structure for all user thread related data
typedef struct TPROC{
    /* priority assosciated with this thread*/
    int id;
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
int    thread_from_pid(int pid);
void   mythread_run(int t_index);


void   delete_tcb();
void   delete_queue(QUEUE* q_ptr);

void mythread_init(int numKernelThreads) {
    // allocate global thread pointer 
    TPROC_PTR = (TPROC_M*)malloc(sizeof(TPROC_M));
    /* allocate thread list */
    TPROC_PTR->threads = (TPROC**)malloc(DEFAULT_Q*sizeof(TPROC*));
    /* initial size should be zero */
    TPROC_PTR->size = 0;
    TPROC_PTR->kactive = 1;
    TPROC_PTR->kmax = numKernelThreads;
    thread_spinlock_init( &TPROC_PTR->lock);
    TPROC_PTR->q = queue_init();
}

int mythread_create(void (* func)()) {

    static id = 0;
    
    thread_spinlock_lock(&TPROC_PTR->lock);
    TPROC* t = (TPROC*)malloc(sizeof(TPROC));
    t->priority = TPROC_PTR->q->size;
    t->pid = -1;
    t->func = func;
    t->id = id + 1;
    id++;

    // initialize a context for the user thread
    t->ctx = (ucontext_t*)malloc(sizeof(ucontext_t));
    getcontext(t->ctx);
    t->ctx->uc_stack.ss_sp = (void*)malloc(STACKSIZE);
    t->ctx->uc_stack.ss_size = STACKSIZE; 
    makecontext(t->ctx,t->func,0);

    // place thread reference in the thread list
    int t_index = TPROC_PTR->size;
    TPROC_PTR->threads[t_index] = t;
    TPROC_PTR->size++;

    if (TPROC_PTR->kactive < TPROC_PTR->kmax){
         // start a kernal thread if there are open, active threads
        t->start = time(NULL);
        mythread_run(t_index);
    } else {
        // place the context in the queue otherwise
        enqueue(TPROC_PTR->q, t_index);
        thread_spinlock_unlock(&TPROC_PTR->lock);
    }

    
}

void mythread_join(int id){
    


}



void mythread_exit() {
    // if the queue is empty, we can exit the thread
    thread_spinlock_lock(&TPROC_PTR->lock);
    int t_index = thread_from_pid(getpid());
    if (t_index != -1) {
        TPROC* t = get_mythread(t_index);
        t->pid = -1;
        t->priority = -1;
    }
    if (TPROC_PTR->q->size == 0) {  
        
        thread_spinlock_lock(&TPROC_PTR->lock);
        TPROC_PTR->kactive--;
        //so , we tried to do some cleanup freeing the queue and the tcb that is, but it was giving
        //
        if (TPROC_PTR->kactive == 0) {
            delete_tcb();
        }
        exit(0);
    }

    // set a thread's id, and start timer
    int index = dequeue(TPROC_PTR->q);
    TPROC* t = get_mythread(index);
    t->pid = getpid();
    t->start = time(NULL);
    thread_spinlock_unlock(&TPROC_PTR->lock);
    // set context to this one since we're done with the old
    setcontext(t->ctx);
}


void mythread_yield() {

    thread_spinlock_lock(&TPROC_PTR->lock);
    int t_index = thread_from_pid(getpid());
    time_t y_time = time(NULL);
    if (t_index == -1) {
        printf("no thread with pid %d\n",getpid());

        thread_spinlock_unlock(&TPROC_PTR->lock);
        return;
    }
    // reset values of the current thread, for the queue
    TPROC* t = get_mythread(t_index);
    t->pid = -1;
    t->priority += y_time - t->start;
    enqueue(TPROC_PTR->q, t_index);
    // get the next highest priority thread and activate it
    int next_t_index = dequeue(TPROC_PTR->q);
    TPROC* next_t = get_mythread(next_t_index);
    next_t->pid = getpid();
    next_t->start = y_time;
    
    thread_spinlock_unlock(&TPROC_PTR->lock);
    if (t_index == next_t_index) {
        return;
    }
    // swap the current and next thread's context
    swapcontext(t->ctx,next_t->ctx);
    //now that we are in the next thread's context, we have yielded the previous one and running the next one

}




TPROC* get_mythread(int t_index) {
    return TPROC_PTR->threads[t_index];
}

//simply calls get_mythread
int pri(QUEUE* q_ptr, int i) {
    return get_mythread(q_ptr->q[i])->priority;
}

// searches through the current threads references for one matching the
// provided pid
int thread_from_pid(int pid) {
    for (int i=0;i<TPROC_PTR->size;i++) {
        if (get_mythread(i)->pid == pid) {
            return i;
        }
    }
    return -1;
}


void mythread_run(int t_index) {

    TPROC* t = get_mythread(t_index);
    if (t->pid != -1) {
        return;
    }

    // this kernel thread is going to be active, so show it
    TPROC_PTR->kactive++;
    // create a new kernel thread to run this thread
    thread_spinlock_unlock(&TPROC_PTR->lock);
    t->pid = clone((int(*)(void*))t->func, t->ctx->uc_stack.ss_sp+t->ctx->uc_stack.ss_size,CLONE_VM, NULL);
    if (t->pid == -1) {
        // some notification if we error when cloning
        perror("clone failed, ughh");

        return;
    }
}

void delete_tcb() {
    for (int i=0;i<TPROC_PTR->size;i++) {
        free(TPROC_PTR->threads[i]->ctx->uc_stack.ss_sp);
        free(TPROC_PTR->threads[i]->ctx);
        free(TPROC_PTR->threads[i]);
    }
    free(TPROC_PTR->threads);
    delete_queue(TPROC_PTR->q);
    free(TPROC_PTR);
}

