#include <stdio.h>
#include <unistd.h>
#include "../queue.h"
#include "../lock.h"

//Race condition Mutex
#define arr_size 5

int counter=0;
int bound = 1000000;
mythread_mutex mutex_lk;


void *race_condition(void *arg)
{
    int i=0;
    while (i < bound){
        mythread_mutex_lock_aquire(&mutex_lk);
        counter++;
        mythread_mutex_lock_released(&mutex_lk);
        i++;
    }
}

int main() {
    mythread_init();
    mythread_mutex_init(&mutex_lk);
    my_thread my_thrd_arr[arr_size];
    for(int i = 0; i < arr_size; i++) {
        mythread_create(&my_thrd_arr[i], race_condition, NULL);
    }
    for(int i = 0; i < arr_size; i++) {
        mythread_join(&my_thrd_arr[i], NULL);
    }
    int expected_val=arr_size*bound;
    printf("Expected counter value: %d\n", expected_val);
    printf("Using Threads Couter value: %d\n", counter);

    if(expected_val==counter)
        printf("\nTestCase-7 PASSED\n");
    else
        printf("\nTestCase-7 FAILED\n");
    return 0;
}