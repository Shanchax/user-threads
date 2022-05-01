#include <stdio.h>
#include <unistd.h>
#include "../queue.h"
#include "../lock.h"

//Race condition Spinlock
#define arr_size 5

int counter=0;
int bound = 1000000;
mythread_spinlock spin_lk;


void *race_condition(void *arg) {
    int i=0;
    while (i < bound){
        mythread_spinlock_lock_aquire(&spin_lk);
        counter++;
        mythread_spinlock_lock_released(&spin_lk);
        i++;
    }
}

int main() {
    mythread_init();
    mythread_spinlock_init(&spin_lk);
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
        printf("\nTestCase-8 PASSED\n");
    else
        printf("\nTestCase-8 FAILED\n");
    return 0;
}