
#ifndef TCB_H
#define TCB_H


#include <stdbool.h>
#include <stdint.h>
#include <ucontext.h>

#define JOINABLE 1
#define DETACHED 2

#define RUNNING 1
#define RUNNABLE 2
#define TERMINATED 3
#define WAITING 4
#define JOINED 5


typedef struct {
    //thread id of the htread ie TPROC
    int id;

    //context of the thread
    ucontext_t context;

    //set to 1(true) if stack allocation is successful. 0 otherwise
    int has_dynamic_stack;

    //pointer to target function of the thrad.
    void *(*target_function) (void *);

    //JOINABLE or DETACHED
    int join_state;

    //thread id of the thread that;s waiting for this thread.
    int wait_id;

    //status of thread : RUNNING, RUNNABLE..
    int thread_status;

    //arguments to the target function
    void *argument;

    //ret value 
    void *return_value;

    //before killing(sending any signal to the thread) , store the pending signals here, 
    //so that when the thread is scheduled again, the signal will be executed.
    sigset_t pending_signals;

} TPROC;

TPROC *create_tcb(void);

void delete_tcb(TPROC *block);

#endif
