#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "lock.h"

#define arr_size 5

int *int_ptr;
int count=arr_size;
//Test 4: Joining on more than one thread

void *target_func(){
    int r = 1;
    int_ptr = &r;
    return int_ptr;
}
int main(){
    mythread_init();
    my_thread my_thrd_arr[arr_size];
    printf("\nThread-Creation\n");
    for(int i = 0; i < arr_size; i++) {
        mythread_create(&my_thrd_arr[i], target_func, NULL);
        printf("Thread %d created with thread_id: %d\n", i+1,my_thrd_arr[i].thread_id);
    }
    printf("\nThread-Joining\n");
    for(int i = 0; i < 5; i++) {
        mythread_join(&my_thrd_arr[i], NULL);
        printf("Thread %d created with thread_id: %d\n", i+1,my_thrd_arr[i].thread_id);
        count--;
    }
    if(count==0)
        printf("\nTestCase-4 PASSED\n");
    else
        printf("\nTestCase-4 FAILED\n");
}