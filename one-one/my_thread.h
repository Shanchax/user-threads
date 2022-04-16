#include "thrd_struct.h"

//Defination for Thread create
int thread_create(mythread *, void *(*target_funtion)(void *), void *);
//Defination of Thread Join
int thread_join(mythread *, void **);
//Defination for Thread Exit
void thread_exit(void *);
//Defination for Thread Kill
int thread_kill(mythread, int);