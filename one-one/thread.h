#ifndef THREADS_H
#define THREADS_H



int thread_create(void * (*target_function)(void *), void *argument);
int thread_join(int thread_id, void **retval);
void thread_kill(void* result));




#endif