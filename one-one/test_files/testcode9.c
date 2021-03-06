//Signal Raising and Testing

#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include "../queue.h"
#include "../lock.h"
#include <unistd.h>
#include <errno.h>
#include <error.h>

struct sigaction new_sig_handler;

int count=0;

void *dummy_func(void *args){
    for(int i=5; i<0;i++)
        sleep(1);
    return NULL;
}

void sig_handle_usr_1(int sig) {
    fflush(stdout);
    count++;
    pid_t thrd_id=gettid();
    printf("Signal received by thread %d\n", thrd_id);
}

int main() {
    mythread_init();
    my_thread mythread;
    int mythrd_id;
    new_sig_handler.sa_handler = &sig_handle_usr_1;
    sigemptyset(&new_sig_handler.sa_mask);
    sigaction(SIGUSR1, &new_sig_handler, NULL);
    mythrd_id = mythread_create(&mythread, dummy_func, NULL);
    printf("Thread Started with Thread_id: %d\n",mythrd_id);
    sleep(1);
    printf("Signal Send using USR1 to Thread with Thread_id: %d\n", mythrd_id);
    sleep(1);
    mythread_kill(mythread, SIGUSR1);
    printf("Killed");
    sleep(1);
    mythread_join(&mythread, NULL);
    if(count==0)
        printf("\nTestCase-9 PASSED\n");
    else{
        printf("%d",count);
        perror("Not Able to raise Signal");
        printf("\nTestCase-9 FAILED\n");
    }
    return 0;
}