#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "lock.h"


void *increment_one(void *args){
    int *num = (int *)args;
    int *res = (int *)malloc(sizeof(int));
    *res = *num+1;
    return res;
}

int main() {
    mythread_init();
    my_thread c1;
    int *ret_val;
    int i=1;
    printf("Expected Value is: %d\n", i+1);
    int mythrd_id = mythread_create(&c1, increment_one, &i);
    mythread_join(&c1, (void *)&ret_val);
    printf("From Main Reuturn Value is: %d\n", *ret_val);
    if(*ret_val==i+1)
        printf("\nTestCase-3 PASSED\n");
    else
        printf("\nTestCase-3 FAILED\n");
    return 0;
}   