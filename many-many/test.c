#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "threads.h"

void th1() {
    int i;
    for(i=0;i<8;i++){
        printf("Thread  1:  run.\n");
        sleep(1);   //note: during  sleep   the user-level  thread  is  still   mapped
        printf("Thread  1:  yield.\n");
        mythread_yield();
    }
    printf("Thread  1:  exit.\n");
    mythread_exit();
}

void th2() {
    int i;
    for(i=0;i<4;i++){
        printf("Thread  2:  run.\n");
        sleep(2);
        printf("Thread  2:  yield.\n");
        mythread_yield();
    }
    printf("Thread  2:  exit.\n");
    mythread_exit();
}

void th3()  {
    int i;
    for(i=0;i<2;i++){
        printf("Thread  3:  run.\n");
        sleep(4);
        printf("Thread  3:  yield.\n");
        mythread_yield();
    }
    printf("Thread  3:  exit.\n");
    mythread_exit();
}

//simply calling thread create, yield and exit repeatedly. no point of doing this ither than to 
//check if thread is getting created or not
int main() {

    //running on 5 kernel thread tops
    //creating th1 7 times and yielding it 7 times in th1.
    //creating th2 5 times and yielding it 5 times in th2.
    
    mythread_init(5);
    mythread_create(th1);
    mythread_create(th2);
    mythread_create(th3);
    
    mythread_create(th1);
    mythread_create(th1);
    mythread_create(th2);

    mythread_create(th3);
    mythread_create(th1);
    mythread_create(th1);
    mythread_create(th1);
    mythread_create(th2);

    mythread_create(th3);
    mythread_create(th1);    
    mythread_exit();
    return 0;
}
