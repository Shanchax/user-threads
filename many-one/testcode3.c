
#include "lock.h"
#include "threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define print(str) write(1, str, strlen(str))

//one shared variable and five single variable
long long c = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0, running = 1;

thread_spinlock lock;

void *thread1(void *arg)
{
    while (running == 1)
    {
        thread_spinlock_lock(&lock);
        c++;
        thread_spinlock_unlock(&lock);
        c1++;
        mythread_yield();
    }

}

void *thread2(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        c++;
        thread_spinlock_unlock(&lock);
        c2++;
        mythread_yield();
    }
    //cthread_exit(NULL);
}

void *thread3(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        c++;
        thread_spinlock_unlock(&lock);
        c3++;
        mythread_yield();
    }
    //cthread_exit(NULL);
}

void *thread4(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        c++;
        thread_spinlock_unlock(&lock);
        c4++;
        mythread_yield();
    }
    //cthread_exit(NULL);
}

void *thread5(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        c++;
        thread_spinlock_unlock(&lock);
        c5++;
        mythread_yield();
    }
    //cthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int tids[6];

    fprintf(stdout, "    Thread Spinlocks\n");

    //cthread_init(1);
    //cthread_spinlock_init(&lock);
    thread_spinlock_init(&lock);


    tids[1] = mythread_create(thread1, NULL);
    tids[2] = mythread_create(thread2, NULL);
    tids[3] = mythread_create(thread3, NULL);
    tids[4] = mythread_create(thread4, NULL);
    tids[5] = mythread_create(thread5, NULL);

    // for (unsigned long i = 0; i < 8; ++i) {
	// void *arg = (void *) i;

	// if ((tids[i] = mythread_create(thread1, arg)) == -1) {
	//     perror("mythread_create");
	//     exit(EXIT_FAILURE);
	// }
    // }
    fprintf(stdout, "    Created 5 threads\n");
    fprintf(stdout, "    Letting threads run\n");

    for (long long int i = 0; i < 1000000000; i++)
        ;

    running = 0;

    // cthread_join(1);
    // cthread_join(2);
    // cthread_join(3);
    // cthread_join(4);
    // cthread_join(5);
    // void *res;
    // mythread_join(tids[0], &res)

    for (int i = 1; i < 6; ++i) {
	int id = tids[i];

	while (1) {
	    void *res;

	    if (mythread_join(id, &res) > 0) {
		printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}
    }
    fprintf(stdout, "    Joined on all 5 threads\n");

    fprintf(stdout, "    Thread 1 		= %lld\n", c1);
    fprintf(stdout, "    Thread 2 		= %lld\n", c2);
    fprintf(stdout, "    Thread 3 		= %lld\n", c3);
    fprintf(stdout, "    Thread 4 		= %lld\n", c4);
    fprintf(stdout, "    Thread 5 		= %lld\n", c5);
    fprintf(stdout, "    t1 + t2 + t3 + t4 + t5  = %lld\n", c1 + c2 + c3 + c4 + c5);
    fprintf(stdout, "    Shared Variable         = %lld\n", c);
    if (c1 + c2 + c3 + c4 + c5 == c)
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