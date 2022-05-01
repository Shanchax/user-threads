#include <stdio.h>
#include <unistd.h>
#include "queue.h"
#include "lock.h"

void *loop(void *args)
{
    int i = 4;
    while (i > 0)
    {
        printf("From thread\n");
        sleep(1);
        i--;
    }
    return NULL;
}

int main()
{
    mythread_init();
    my_thread c1;
    int pid1 = mythread_create(&c1, loop, NULL);
    mythread_join(&c1, NULL);
    printf("Back to main\n");
    return 0;
}