#include <stdio.h>
#include <unistd.h>
#include "../queue.h"
#include "../lock.h"
//thread exit case

int result_val;


void *increment_one(void *args){
    int *num = (int *)args;
    result_val = *num+1;
    mythread_exit(&result_val);
}

int main() {
    mythread_init();
    my_thread c1;
    int i=1;
    printf("Expected Value is: %d\n", i+1);
    int mythrd_id = mythread_create(&c1, increment_one, &i);
    //print_cur_thrds();
    sleep(4);//time for executing other processes
    printf("From Main Reuturn Value is: %d\n", result_val);
    if(result_val==i+1)
        printf("\nTestCase-5 PASSED\n");
    else
        printf("\nTestCase-5 FAILED\n");
    return 0;
}   