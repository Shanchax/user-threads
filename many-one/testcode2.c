// #include <stdio.h>
// #include <unistd.h>
// #include "threads.h"
// #include "lock.h"

// int counter, limit = 100000;
// cthread_spinlock lock;

// void *count_to_big(void *arg)
// {
//     for (int i = 0; i < limit; i++)
//     {
//         //cthread_spinlock_lock(&lock);
//         counter++;
//         printf("now it is %d",counter);
//         //cthread_spinlock_unlock(&lock);
//     }
//     //cthread_exit(NULL);
// }

// int main()
// {
//     //cthread_init(1);
//     //cthread_spinlock_init(&lock);
//     int pid1 = mythread_create(count_to_big, NULL);
//     printf("%d",pid1);
//     int pid2 = mythread_create(count_to_big, NULL);
//     printf("%d",pid2);
//     int pid3 = mythread_create(count_to_big, NULL);
//     printf("%d",pid3);

//     void* result = NULL;
//     mythread_join(pid1 , result);
//     mythread_join(pid2 , result);
//     mythread_join(pid3 , result);
//     printf("counter is %d\n", counter);
//     return 0;
// }

#include "threads.h"
#include "lock.h"
#include <stdio.h>
#include <stdlib.h>


static long sum = 0;

static int cnt = 1;
thread_spinlock lock;

static void *count_to_big(void *arg)
{
    for (int i = 0; i < 100000; i++)
    {
        thread_spinlock_lock(&lock);
        cnt++;
        //printf("now it is %d",counter);
        thread_spinlock_unlock(&lock);
    }
    //cthread_exit(NULL);
}
/*
static void *magic = NULL;
static unsigned count;


static void *thread0(void *arg)
{
    for (;;) {
	if (magic != arg) {
	    printf("Hello, this is thread %lu with count %u.\n", (unsigned long) arg, count);
	    magic = arg;

	    count += 1;
	}
    }

    return NULL;
}
*/

// static void *thread1(void *arg)
// {
//     if ((unsigned long) arg % 2 == 0) {
// 	return arg;
//     } else {
// 	for (;;) {
// 	}
//     }
// }

static void *thread1(void *arg)
{

    sum = sum + (int)arg;
	

}

void *thread2(void *arg)
{
    cnt *= 2;
    //mythread_exit(NULL);
}


int main(void)
{
    puts("Hello, this is main().");

    /*

    for (unsigned long i = 0; i < 8; ++i) {
	void *arg = (void *) i;

	if (mythread_create(thread0, arg) == -1) {
	    perror("mythread_create");
	    exit(EXIT_FAILURE);
	}
    }

    for (;;) {
	if (magic != 0x0) {
	    puts("Hello, this is main().");
	    magic = 0x0;
	}
    }
    
    */
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
		//printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}

	
    }

	printf("count is %d", cnt);

	mythread_exit(NULL);
}
