#include "thrd_struct.h"


#define MAXSTACKSIZE (1024 * 1024)



//Threads wrapper for initialization of threads queue
void mythread_init();
//Defination for Thread create
int mythread_create(my_thread* , void *(*target_funtion)(void *), void *);
//Function resposible for execution of function passed to thread
int mythread_run(void *);
//Defination of Thread Join
int mythread_join(my_thread*, void **);
//Defination for Thread Exit
void mythread_exit(void *);
//Defination for Thread Kill
int mythread_kill(my_thread, int);