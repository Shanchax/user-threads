#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "lock.h"



void *target_function(void *args){
    for(int k=0;k<3;k++) {
        printf("Executing From thread for Testcase-1\n");
        sleep(1);
    }
}

   
int main(){
    int mythread_id;
    mythread_init();
    my_thread t1;
    mythread_id = mythread_create(&t1, target_function, NULL);
    mythread_join(&t1, NULL);
    if(mythread_id!=-1){
        printf("\nTestCase-1 PASSED\n");
    }else{
        printf("\nTestCase-1 FAILED\n");
    }
    return 0;
}