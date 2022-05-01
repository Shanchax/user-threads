#include "threads.h"
#include "lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define print(str) write(1, str, strlen(str))

long long c = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, running = 1;

thread_spinlock lock;

void *thread1(void *arg)
{
    while (running == 1)
    {
        thread_spinlock_lock(&lock);
        sleep(1);
        c++;
        thread_spinlock_unlock(&lock);
        c1++;
        //cmythread_yield();
    }
    //cthread_exit(NULL);
}

void *thread2(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        c++;
        sleep(1);
        thread_spinlock_unlock(&lock);
        c2++;
        //cmythread_yield();
    }
    //cthread_exit(NULL);
}

void *thread3(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        c++;
        sleep(1);
        thread_spinlock_unlock(&lock);
        c3++;
        //cmythread_yield();
    }
    //cthread_exit(NULL);
}

// void *thread4(void *arg)
// {
//     while (running == 1)
//     {

//         thread_spinlock_lock(&lock);
//         c++;
//         thread_spinlock_unlock(&lock);
//         c4++;
//         //cmythread_yield();
//     }
//     //cthread_exit(NULL);
// }

// void *thread5(void *arg)
// {
//     while (running == 1)
//     {

//         thread_spinlock_lock(&lock);
//         c++;
//         thread_spinlock_unlock(&lock);
//         c5++;
//         //cmythread_yield();
//     }
//     //cthread_exit(NULL);
// }

int main(int argc, char **argv)
{
    // int tids[6];

    int tids[6];

    fprintf(stdout, "    Thread Spinlocks\n");

    //cthread_init(1);
    thread_spinlock_init(&lock);
    int i = 0;
    void *arg = (void *) i;

    tids[1] = mythread_create(thread1, arg);
    tids[2] = mythread_create(thread2, arg);
    //tids[3] = mythread_create(thread3, arg);
    // tids[4] = mythread_create(thread4, arg);
    // tids[5] = mythread_create(thread5, arg);

    
    fprintf(stdout, "    Created 2 threads\n");
    fprintf(stdout, "    Letting threads run\n");

    for (long long int i = 0; i < 1000000000; i++)
        ;

    running = 0;
    void *res;
    mythread_join(tids[1] ,&res );
    mythread_join(tids[2] , &res);
    //mythread_join(tids[3] , &res);
    // mythread_join(tids[4] , &res);
    // mythread_join(tids[5] , &res);

    for (int i = 0; i < 2; ++i) {
	int id = tids[i];

	while (1) {
	    void *res;

	    if (mythread_join(id ,&res )) {
		printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}

    }
    fprintf(stdout, "    Joined on all 2 threads\n");

    fprintf(stdout, "    Thread 1 		= %lld\n", c1);
    fprintf(stdout, "    Thread 2 		= %lld\n", c2);
    //fprintf(stdout, "    Thread 3 		= %lld\n", c3);
    // fprintf(stdout, "    Thread 4 		= %lld\n", c4);
    // fprintf(stdout, "    Thread 5 		= %lld\n", c5);
    fprintf(stdout, "    t1 + t2 + t3 + t4 + t5  = %lld\n", c1 + c2  );
    fprintf(stdout, "    Shared Variable         = %lld\n", c );
    if (c1 + c2   == c )
    {
        fprintf(stdout, "    TEST PASSED\n");
    }
    else
    {
        fprintf(stdout, "    Test failed\n");
    }
    fflush(stdout);

    return 0;
}