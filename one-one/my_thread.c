#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <setjmp.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/futex.h>
#include <syscall.h>
#include "queue.h"
#include "lock.h"

//Function resposible for execution of function passed to thread
int mythread_run(void *cur_thrd) {
    my_thread *new_thrd = (my_thread *)cur_thrd;
    enqueue(new_thrd);
    if (setjmp(new_thrd->environment) != 0) {
        printf("Executing Thread with thread_id- %d , exited safely\n", new_thrd->thread_id);
        new_thrd->is_completed = 1;
    }
    else{
        new_thrd->ret_val = new_thrd->target_function(new_thrd->args);
        new_thrd->is_completed = 1;
    }
    return 0;
}

//Thread Initialiation Supportive function
void mythread_init(){
    init_queue();
}

my_thread *set_thrd(my_thread* thread,void *(*target_func)(void *), void *args,char* mystack){
    //char *my_stack_head_ptr;
    //my_stack_head_ptr = mystack  +  MAXSTACKSIZE  -  1;
    //thread->size_of_stack = MAXSTACKSIZE;
    thread->target_function = target_func;
    thread->args = args;
    thread->is_completed = 0;
    thread->parent_id = getpid();  
    thread->next = NULL;
}
//Thread Creation with routine Function
int mythread_create(my_thread *thread, void *(*target_func)(void *), void *args) {
    char *mystack;
    char *my_stack_head_ptr;
    mystack = (char *)malloc(MAXSTACKSIZE);
    my_stack_head_ptr = mystack + MAXSTACKSIZE  -  1;
    

    if (!mystack) {
        fprintf(stderr, "Unalbe to allocate stack.\n");
        exit(1);
    }
    thread->stack_head = my_stack_head_ptr;
    thread=set_thrd(thread,target_func,args,mystack);
    
    
    thread->thread_id = clone(mythread_run, my_stack_head_ptr, 
                                    CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM | 
                                    CLONE_SIGHAND | CLONE_THREAD | CLONE_PARENT_SETTID | 
                                    CLONE_CHILD_CLEARTID, thread, &thread->futex_block, thread, &thread->futex_block);
    if (thread->thread_id == -1) {
        printf("Not Able to Create User Thread\n");
        fprintf(stderr, "Error Generated While Clonning: \n");
        free(mystack);
        exit(1);
    }
    return thread->thread_id;
}

my_thread *mythread_ret_val() {
    int thread_id = gettid();
    if (thread_id== getpid())
        return NULL;
    my_thread *thrd = get_thrd_node(thread_id);
    return thrd;
}

int mythread_join(my_thread *thread, void **return_value) {
    if (thread->is_completed != 1){
        futex_halt_till(&thread->futex_block, thread->thread_id);
        if (return_value != NULL)
            *return_value = thread->ret_val;
        free((void *)(thread->stack_head + 1 - MAXSTACKSIZE));
    }
    return 0;
}