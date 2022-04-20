#include "thrd_struct.h"


#define MAXSTACKSIZE (64 * 1024)



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
//Function for setting thread and returning in thread create 
my_thread *set_thrd(my_thread* thread,void *(*target_func)(void *), void *args,char* mystack);
//Responsible for returning thread block 
my_thread *mythread_ret_val();
//Function responsible for Thread Joinong purpose
int mythread_join(my_thread *thread, void **return_value);