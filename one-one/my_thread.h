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
my_thread *set_thrd(my_thread* ,void *(*target_func)(void *), void *,char* );
//Responsible for returning thread block
my_thread *mythread_ret_val();
//Base Primitive for which calls by mythread_kill for killing thread;
static int primitive_thread_kill(int thread_id, int signal) {
    return syscall(SYS_tgkill, getpid(), thread_id, signal);
}