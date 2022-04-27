#ifndef THREADS_H
#define THREADS_H

int mythread_create(void *(*target_function) (void *), void *argument);

void mythread_exit(void *result);

int mythread_join(int id, void **result);


#endif
