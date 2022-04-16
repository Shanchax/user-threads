#ifndef THREADS_H
#define THREADS_H



int thread_create(void * (*target_function)(void *), void *argument);
int thread_join(TPROC* thread, void **retval);
void thread_kill(void* result);




#endif