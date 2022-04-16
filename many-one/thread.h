#ifndef THREADS_H
#define THREADS_H

//#include "tproc.h"



int thread_create(void * (*target_function)(void *), void *argument);
//int thread_join(TPROC* thread, void **retval);
void thread_kill(void* result);
void thread_exit(void* result);
static void thread_run(void);



#endif