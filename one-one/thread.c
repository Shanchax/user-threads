
#include "thread.h"
#include "tproc.h"
#include "queue.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>

#include<signal.h>

#include <sys/mman.h>

#define ERR_SIGPROF 9001 //random number
#define INTERVAL 100000
#define STACKSIZE (64*1024) 

//will point to the thread that is currently running;
static TPROC *current_running;

static QUEUE *ready_queue;

static QUEUE *terminated_queue;


//prototypes:
int thread_create(void * (*target_function)(void *), void *argument);
int thread_join(int thread_id, void **retval);
void thread_kill(void* result));
void thread_exit(void* result);
static void thread_run(void);
static int timer_init(void);
static void new_sigprof_handler(int signum , siginfo_t *nfo , void* context);

static void sigprof_lock(void);
static void sigprof_unlock(void);

static int queue_init(void);

static int context_of_first_thread(void);
static int allocstack(TPROC* block);



int thread_join(int thread_id, void **retval){
    return 0;
}
void thread_kill(void* result){

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

//blocking SIGPROF is a must while creation of thread!!
//why? because we don't want thread creation to be 
//interrupteed by SIGPROF. So, we will use "sigprocmask",under a 
//wrapper functions : sigprof_lock, sigprof_unlock


static void sigprof_lock(void){
    sigset_t *set;
    sigemptyset(set);
    sigaddset(set, SIGPROF);

    ret_val = sigprocmask(SIG_BLOCK , set , NULL);
    if(ret_val == -1)
        perror();
        return errno;

    
}

static void sigprof_unlock(void){
    sigset_t *set;
    sigemptyset(set);
    sigaddset(set, SIGPROF);

    ret_val = sigprocmask(SIG_UNBLOCK , set , NULL);
    if(ret_val == -1)
        perror();
        return errno;

    
}

//before creating thread, read queue and terminated queue need
//to be initialized. this is done here, in 

static int queue_init(void){
    ready_queue = queue_create();

    terminated_queue = queue_create();

    if(ready_queue == NULL)
        return 0;
    
    if(terminated_queue == NULL)
        delete_queue(ready_queue);
        return 0;
    

    return 1;

}

//now that we have our queues initialized, we need to think 
//about the context of the very first thread tha will run!!!

static int context_of_first_thread(void){
    TPROC* dummy;
    dummy = create_tcb();
    if(dummy == NULL)
        return 0;

    ret_val = getcontext(&dummy->thread_context);
    if(ret_val == -1)
        delete_tcb(dummy);
        perror("getcontext failed");
        return errno;
    
    current_running = dummy;

    return true;


}

//allocating stack for newly created thread.

static int allocstack(TPROC* block){

    void* stack = NULL;
    stack = mmap(0, STACKSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if(stack == MAP_FAILED){
        perror();
        return errno;
        //return 0?
    }
    if(mprotect(stack , STACKSIZE , PROT_NONE)){
        munmap(stack, STACKSIZE);
        perror();
        return errno;
        //return 0?
    }

    block->thread_context.uc_stack.ss_flags = 0;
    block->thread_context.uc_stack.ss_size = STACKSIZE;
    block->thread_context.uc_stack.ss_sp = stack;
    block->has_dynamic_stack = 1;

    //not to sure how to assign uc_link!!Ask sir

    return 1;
    
}

//statically incrementing id of thread. id is not allocated using clone
int thread_create(void * (*target_function)(void *), void *argument){
    sigprof_lock();

    static int initval = 0;

    if(!initval){
        if(!queue_init()){
            perror();
            return errno;
        }
        if(!context_of_first_thread()){
            perror();
            return errno;

        }
        if(!timer_init()){
            perror();
            return errno;
        }

        initval = 1;
  
    }

    TPROC* new;
    new = create_tcb();
    if(new == NULL){
        perror("failed to create new tproc");
        return errno;
    }

    int ret_val = getcontext(&new->thread_context);
    if(ret_val == -1){
        perror("failed to get context of dummy thread");
        delete_tcb(new);
        return errno;
    }

    if(!allocstack(new)){
        perror("failed to allocate stack for thread");
        delete_tcb(new);
        return errno;

    }

    makecontext(&new->thread_context,thread_run, 1, new->id);

    new->target_function = target_function;
    new->argument = argument;

    int ret = enqueue(ready_queue , new);
    if(ret != 0){
        perror("failed to add to ready_queue");
        return -1;
    }

    sigprof_unlock();
    return new->id;

}

static void thread_run(void){
    sigprof_lock();
    TPROC* local = current_running;
    sigprof_unlock();

    //result is pointer to the return value of target function!
    void *result = local->target_function(local->argument);
    thread_exit(result);

}

void thread_exit(void *result){

    if(current_running != NULL){
        sigprof_lock();

        current_running->return_value = result;

        int ret_val = enqueue(terminated_queue , current_running);
        if(ret_val != 0){
            perror();
            abort();
        }

        current_running = dequeue(ready_queue);
        if(current_running == NULL){
            exit();
        }

        setcontext(&current_running->thread_context);
        
    }else{
        exit();
    }
    

}

































