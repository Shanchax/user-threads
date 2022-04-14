#include "thread.h"
#include "tproc.h"
#include "queue.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>

#include<signal.h>

#define ERR_SIGPROF 9001 //random number
#define INTERVAL 100000

//will point to the thread that is currently running;
static TPROC *current_running;

static QUEUE *ready_queue;

static QUEUE *terminated_queue;


//prototypes:
int thread_create(void * (*target_function)(void *), void *argument);
int thread_join(int thread_id, void **retval);
void thread_kill(void* result));
static int timer_init(void);
static void new_sigprof_handler(int signum , siginfo_t *nfo , void* context);


int thread_create(void * (*target_function)(void *), void *argument){
    return 0;
}
int thread_join(int thread_id, void **retval){
    return 0;
}
void thread_kill(void* result)){

}


//https://stackoverflow.com/questions/2086126/need-programs-that-illustrate-use-of-settimer-and-alarm-functions-in-gnu-c
static int timer_init(void){
    
    sigset_t allsignals;

    sigfillset(&allsignals);

    const struct sigaction alarm;

    struct sigaction oldact;

    const struct itimerval timeout_val;

    timeout_val.it_interval.tv_sec = 0;
    timeout_val.it_interval.tv_usec = INTERVAL;
    timeout_val.it_value.tv_sec = 0;
    timeout_val.it_value.tv_usec = 1;
     

    alarm.sa_sigaction = new_sigprof_handler;
    alarm.sa_mask = allsignals;
    alarm.sa_flags = SA_SIGINFO;

    int ret = sigaction(SIGPROF , &alarm , &oldact);

    if(ret = -1)
        perror("Changing of sigaction on receipt of SIGPROF failed");
        return ERR_SIGPROF;

    int ret_timer = setitimer(ITIMER_PROF, &timeout_val , NULL)
    if(ret_timer == -1){
        if(sigaction(SIGPROF , &alarm , &oldact) == -1)
            perror("Ooops");
            return ERR_SIGPROF;
            exit();

        return 0;    
    }

    return 1;


}

static void new_sigprof_handler(int signum , siginfo_t *nfo , void* context){

    //first, we will store the current context of signalhandler
    ucontext_t *backup = &current_running->thread_context;
    ucontext_t *new = (ucontext_t *)context;

    //"backup" is where we will store current thread's context
    backup->uc_flags = new->uc_flags;
    backup->uc_link = new->uc_link;
    backup->uc_mcontext = new->uc_mcontext;
    backup->uc_sigmask = new->uc_sigmask;

    if( enqueue(ready_queue , current_running) != 0)
        perror();
        return errno;

    if( (current_running = dequeue(ready_queue))==NULL)
        perror();
        return errno;

    int ret_val = setcontext(&current_running->thread_context;
    if(ret_val == -1){
        perror();
        return errno;

    }







}

