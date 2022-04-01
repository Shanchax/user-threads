#include "thread.h"
#include "tproc.h"
#include "queue.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ucontext.h>



int thread_create(void * (*target_function)(void *), void *argument){
    return 0;
}
int thread_join(int thread_id, void **retval){
    return 0;
}
void thread_kill(void* result)){

}
