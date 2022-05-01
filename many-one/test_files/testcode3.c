
#include "../lock.h"
#include "../threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define print(str) write(1, str, strlen(str))

//one shared variable and five single variable
long long d = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0 , d7 = 0 , d8 = 0 , d9 = 0 , d10 = 0 ,  running = 1;

thread_spinlock lock;
//
//will increment d and d1
void *thread1(void *arg)
{
    while (running == 1)
    {
        thread_spinlock_lock(&lock);
        d++;
        thread_spinlock_unlock(&lock);
        d1++;
        mythread_yield();
    }

}
//
//will increment d and d2
void *thread2(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        d++;
        thread_spinlock_unlock(&lock);
        d2++;
        mythread_yield();
    }
    
}

//will increment d and d3
void *thread3(void *arg)
{
    while (running == 1)
    {

        thread_spinlock_lock(&lock);
        d++;
        thread_spinlock_unlock(&lock);
        d3++;
        mythread_yield();
    }
    
}


int main(int argc, char **argv)
{
    int tids[6];

    puts("RACE CONDITION PROBLEM");
    printf("\n");

    thread_spinlock_init(&lock);


    tids[1] = mythread_create(thread1, NULL);
    tids[2] = mythread_create(thread2, NULL);
    tids[3] = mythread_create(thread3, NULL);
    


    
    puts("3 threads have been created");
    // fprintf(stdout, "    Letting threads run\n");
    puts(" Waiting for a while, allowing them to run");

    for (long long int i = 0; i < 100000000000; i++)
        ;

    running = 0;


    for (int i = 1; i < 4; ++i) {
	int id = tids[i];

	while (1) {
	    void *res;

	    if (mythread_join(id, &res) > 0) {
		printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}
    }
    printf( "Joined on all 3 threads \n");

    printf("    VALUE of d1 		= %lld\n", d1);
    printf( "    VALUE of d2 		= %lld\n", d2);
    printf( "    VALUE of d3 		= %lld\n", d3);
    
    printf( "    Sum of all di  = %lld\n", d1 + d2 + d3 );
    printf( "    Shared Variable         = %lld\n", d);
    if (d1 + d2 + d3  == d)
    {
        printf(" \nTestCase-3 PASSED\n");
    }
    else
    {
        printf(" \nTestCase-3 FAILED\n");
    }
    fflush(stdout);

    return 0;
}