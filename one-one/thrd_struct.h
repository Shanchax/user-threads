#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <syscall.h>
#include <linux/futex.h>
#include <setjmp.h>
#include <unistd.h>


typedef struct my_thread{
    //thread id unique for each thread
    int thread_id;
    //parent id for calling process stored
    int parent_id;
    //function pointer for target function
    void *(*target_function)(void *);
    //pointer for arguments for target function
    void *args;
    //pointer to return value of target function
    void *ret_val;
    /*
      Flag to check whether thread is running/executing or already executed
        is_completed == 0 : Thread already running or executing
        is completed == 1 : Thread completed its execution
    */
    int is_completed;
    //SP and PC are saved when sigsetjump called
    sigjmp_buf environment;    
    //necessary procedure (must) to be taken to perform blocking
    u_int32_t futex_block;
    //maximum size of stack allocated for thread stack     
    int size_of_stack;
    //pointer pointing to start of stack :- head of stack 
    void *stack_head;
    //pointer pointing to next thread block in queue
    struct my_thread *next;
} my_thread;