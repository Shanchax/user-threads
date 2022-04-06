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

//prototypes:
int thread_create(void * (*target_function)(void *), void *argument);
int thread_join(int thread_id, void **retval);
void thread_kill(void* result));
static int timer_init(void);


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
     

    alarm.sa_sigaction = new_signal_handler;
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