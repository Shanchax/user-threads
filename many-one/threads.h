#ifndef THREADS_H
#define THREADS_H

#include "tproc.h"

int mythread_create(void *(*target_function) (void *), void *argument);

void mythread_exit(void *result);

int mythread_join(int id, void **result);

int mythread_yield(void);

int mythread_kill(int id, int sig);


#endif
