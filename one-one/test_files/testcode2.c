#include <stdio.h>
#include <unistd.h>
#include "../queue.h"
#include "../lock.h"


int flag=0;

void check_retvalue(int ret){
    if(ret!=0){
        printf("Error-Code expected=%d\n",ret);
        printf("\nTestCase-2 PASSED\n");

    }
    else{
        if(flag!=0)
            printf("\nTestCase-2 FAILED\n");
    }
    flag++;
}
void *target_function(void *args){
    int a;
    a=1;
}

int main(){
    mythread_init();
    my_thread t2;
    int mythread_id = mythread_create(&t2, target_function, NULL);
    check_retvalue(mythread_join(&t2, NULL));
    check_retvalue(mythread_join(&t2, NULL));
    return 0;
}