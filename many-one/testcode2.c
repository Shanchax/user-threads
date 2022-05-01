


#include "threads.h"
#include "lock.h"
#include <stdio.h>
#include <stdlib.h>


static long sum = 0;

static int cnt = 0;
int count = 0;
thread_spinlock lock;

static void *count_to_big(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        thread_spinlock_lock(&lock);
        cnt++;
        
        thread_spinlock_unlock(&lock);
    }
    
}

static long count_to_big_without_thread(){

	for(int i = 0 ; i < 8 ; i ++){

		for(int j = 0 ; j <100000;j++){
			count++;
		}
	}
}


static void *thread1(void *arg)
{

    sum = sum + (int)arg;
	

}

void *thread2(void *arg)
{
    cnt *= 2;
    
}


int main(void)
{
    puts("Counting to a big number [STRESS TEST]");

    
    thread_spinlock_init(&lock);
    
    int threads[8];

    for (unsigned long i = 0; i < 8; ++i) {
	void *arg = (void *) i;

	if ((threads[i] = mythread_create(count_to_big, arg)) == -1) {
	    perror("mythread_create");
	    exit(EXIT_FAILURE);
	}
    }

    for (int i = 0; i < 8; ++i) {
	int id = threads[i];

	while (1) {
	    void *res;
	    if (mythread_join(id, &res) > 0) {
		
		break;
	    }
	}

    }

	printf("count using thread is  %d", cnt);

	int count = count_to_big_without_thread();
	printf("\n count without using threads is  %d", count);

	if(cnt == count){
		printf("\n TEST PASSED");
	}else{
		printf("TEST FAILED");
	}

	mythread_exit(NULL);
}
