#include "threads.h"
#include "tproc.h"
#include "queue.h"

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <ucontext.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/mman.h>

#define PROT_FLAGS (PROT_READ|PROT_WRITE)
#define MMAP_FLAGS (MAP_PRIVATE|MAP_ANONYMOUS)

#define INTERVAL 10000

#define STACKSIZE (64*1024)


static TPROC *current_running;

static QUEUE *ready_queue;
static QUEUE *terminated_queue;

static int timer_init(void);
static void new_sigprof_handler(int, siginfo_t *, void *);

static int queue_init(void);
static int context_of_first_thread(void);



static void mythread_run();
int mythread_equal( TPROC thread1 , TPROC thread2);
int mythread_yield(void);

static int allocstack(TPROC *);

static void sigprof_lock(void);
static void sigprof_unlock(void);



static int timer_init(void)
{
   

    sigset_t allsignals;

    sigfillset(&allsignals);

    struct sigaction alarm;

    struct sigaction oldact;

    struct itimerval timeout_val;


    alarm.sa_sigaction = new_sigprof_handler;
    alarm.sa_mask = allsignals;
    alarm.sa_flags = SA_SIGINFO | SA_RESTART;

    timeout_val.it_interval.tv_sec = 0;
    timeout_val.it_interval.tv_usec = INTERVAL;
    timeout_val.it_value.tv_sec = 0;
    timeout_val.it_value.tv_usec = 1;

    int ret_val = sigaction(SIGPROF, &alarm, &oldact);
    if (ret_val == -1) {
	perror("sigaction");
	abort();
    }

    int ret_timer = setitimer(ITIMER_PROF, &timeout_val, NULL);
    if ( ret_timer== - 1) {
	if (sigaction(SIGPROF, &oldact, NULL) == -1) {
	    perror("sigaction");
	    abort();
	}

	return 0;
    }

    return 1;
}


static void new_sigprof_handler(int signum, siginfo_t *nfo, void *context)
{
    //int old_errno = errno;

    if (current_running != NULL && queue_size(ready_queue) != 0) {

        //first, we will store the current context of signalhandler
        ucontext_t *backup = &current_running->context;
        ucontext_t *new = (ucontext_t *) context;

        //"backup" is where we will store current thread's context
        backup->uc_flags = new->uc_flags;
        backup->uc_link = new->uc_link;
        backup->uc_mcontext = new->uc_mcontext;
        backup->uc_sigmask = new->uc_sigmask;

        int ret_val = enqueue(ready_queue, current_running);
        if ( ret_val!= 0) {
	        abort();
        }

        current_running = dequeue(ready_queue);
        
        if ( current_running == NULL) {
	        abort();
        }

        //errno = old_errno;
        int ret_val2 = setcontext(&current_running->context);
        if ( ret_val2 == -1) {
	        abort();
        }
	    
    }else{
        exit(EXIT_FAILURE);
    }

    
}

static int context_of_first_thread(void)
{
    TPROC *dummy;
    dummy = create_tcb();
    if (dummy == NULL) {
	return 0;
    }

    int ret_val = getcontext(&dummy->context);
    if ( ret_val== -1) {
	delete_tcb(dummy);
	return 0;
    }

    current_running = dummy;
    return 1;
}


static int allocstack(TPROC *block)
{

    void* stack = NULL;
    //stack = mmap(0, STACKSIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    stack = mmap(0, STACKSIZE, PROT_FLAGS, MMAP_FLAGS, -1, 0);
    if(stack == MAP_FAILED){
        perror("error");
        //return errno;
        return 0;
    }

    // if(mprotect(stack , STACKSIZE , PROT_NONE)){
    //     munmap(stack, STACKSIZE);
    //     perror("error");
    //     //return errno;
    //     return 0;
    // }

    block->context.uc_stack.ss_flags = 0;
    block->context.uc_stack.ss_size = STACKSIZE;
    block->context.uc_stack.ss_sp = stack;
    block->has_dynamic_stack = 1;

    return 1;
}

static void sigprof_lock(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPROF);

    int ret_val = sigprocmask(SIG_BLOCK, &set, NULL);
    if ( ret_val == -1) {
	perror("sigprof ock failed");
	abort();
    }
}

static void sigprof_unlock(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPROF);

    int ret_val = sigprocmask(SIG_UNBLOCK, &set, NULL);
    if (ret_val == -1) {
	perror("sigprof unlock failed");
	abort();
    }
}

static int queue_init(void)
{
    ready_queue = queue_create();
    if (ready_queue == NULL) {
	    return 0;
    }

    terminated_queue = queue_create();
    if (terminated_queue == NULL) {
	    delete_queue(ready_queue);
	    return 0;
    }

    return 1;
}

/* Create a new thread. func is the function that will be run once the
   thread starts execution and arg is the argument for that
   function. On success, returns an id equal or greater to 0. On
   failure, errno is set and -1 is returned. */
int mythread_create(void *(*target_function) (void *), void *arg)
{
    sigprof_lock();

    // Init if necessary

    static int initval;

    if (! initval) {
	if (! queue_init()) {
        perror("error in initializing queue");
	    abort();
	}

	if (! context_of_first_thread()) {
        perror("error in getting current context");
	    abort();
	}

	if (! timer_init()) {
        perror("error in initializing profiling timer");
	    abort();
	}
	
	initval = 1;
    }

    // Create a thread control block for the newly created thread.

    TPROC *new;
    new = create_tcb();
    if ( new == NULL) {
        perror("failed to create tcb");
	    return -1;
    }

    int ret_val = getcontext(&new->context);
    if ( ret_val == -1) {
	delete_tcb(new);
	return -1;
    }

    if (! allocstack(new)) {
        perror("failed to allocate stack");
	delete_tcb(new);
	return -1;
    }

    makecontext(&new->context, mythread_run, 1, new->id);

    new->join_state = JOINABLE;
    new->thread_status = RUNNABLE;
    new->wait_id = -1;
    new->target_function = target_function;
    new->argument = arg;

    // Enqueue the newly created stack
    int ret = enqueue(ready_queue, new);
    if ( ret != 0) {
        perror("error in adding to ready queue");
	delete_tcb(new);
	return -1;
    }

    sigprof_unlock();
    return new->id;
}

static void mythread_run(void)
{
    sigprof_lock();
    TPROC *local = current_running;
    local->thread_status = RUNNING;
    sigprof_unlock();

    void *result = local->target_function(local->argument);
    mythread_exit(result);
}


void mythread_exit(void *result)
{
    if (current_running != NULL) {
        sigprof_lock();
        current_running->return_value = result;
        current_running->thread_status = TERMINATED;
        current_running->join_state = DETACHED;

        if (enqueue(terminated_queue, current_running) != 0) {
	    abort();
        }

        current_running = dequeue(ready_queue);
        if ( current_running == NULL) {
	    exit(EXIT_SUCCESS);
        }

        if(setcontext(&current_running->context) == -1){
            perror("setcontext failed");
            //errno = EINVAL;
            abort();

        }
	
    }else{
        exit(EXIT_SUCCESS);
    }

}
/* Wait for the thread with matching id to finish execution, that is,
   for it to call mythread_exit. On success, the threads result is
   written into result and id is returned. If no terminated_queue thread with
   matching id exists, 0 is returned. On error, -1 is returned and
   errno is set. */
   //@params: thread id and result
   //@returns: see above comment.
//cause of seg fault!!!
int mythread_join(int id, void **result)
{   
    
    if (id > 0) {

        sigprof_lock();
       
        TPROC *block = dequeue_id(terminated_queue, id);
        
        sigprof_unlock();
        //sigprof_lock();
        

        if (block == NULL ) {
	        return 0;
        } else {

            sigprof_lock();

            // if(block->join_state == DETACHED){
            //     sigprof_unlock();
            //     errno = EINVAL;
            //     return -1;
            // }

            if(block->id == current_running->wait_id){
                perror("deadlock");
                errno = EDEADLK;
                return -1;
            }

            if(block->wait_id != -1){
                perror("already joined");
                errno = EINVAL;
                return -1;
            }else{
                block->wait_id = current_running->id;
            }

            sigprof_unlock();
            
            current_running->thread_status = WAITING;
            while(block->thread_status != TERMINATED){
                sigprof_lock();
            }
	        *result = block->return_value;

            sigprof_unlock();
	        delete_tcb(block);
	        return id;
        }
	
    }else{

        errno = EINVAL;
	        return -1;

    }

    
}

int mythread_equal( TPROC thread1 , TPROC thread2){

    if( &thread1 && &thread2){

        return( thread1.id == thread2.id);

    }

    
}
//returns 0 on success, EINVAL on failure.
int mythread_yield(void){

    if(raise(SIGPROF) != 0){

        perror("failed to send signal and yield the processor");
        errno = EINVAL;
        return errno ;

    }

    return 0;


    //raise(SIGPROF);
}
//sending a signal to thread.
//not just killing! sending any generic signals
int thread_kill(TPROC thread, int sig
){
    //block_interrupt();
    sigprof_lock();
    if(sig < 0 || sig > 64)
        return EINVAL;
    
    if(thread.id == current_running->id )
        raise(sig);
    
   
    //block_interrupt();
    sigprof_unlock();
    TPROC *target_thread = dequeue_id(terminated_queue ,thread.id);
    if(target_thread == NULL) {
        // unblock_interrupt();
        sigprof_unlock();
        return EINVAL;
    }
    //store pending signals
    sigaddset(&target_thread->pending_signals, sig);
    //unblock_interrupt();
    sigprof_unlock();
    return 0;
}










	 







