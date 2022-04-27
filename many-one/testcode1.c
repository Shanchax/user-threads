#include "threads.h"

#include <stdio.h>
#include <stdlib.h>


static long sum = 0;

//static int cnt = 1;


static void *thread1(void *arg)
{

    sum = sum + (int)arg;
	

}


int main(void)
{
    puts("Hello, this is main().");



    int threads[8];

    for (unsigned long i = 0; i < 8; ++i) {
	void *arg = (void *) i;

	if ((threads[i] = mythread_create(thread1, arg)) == -1) {
	    perror("mythread_create");
	    exit(EXIT_FAILURE);
	}
    }

    for (int i = 0; i < 8; ++i) {
	int id = threads[i];

	while (1) {
	    void *res;

	    if (mythread_join(id, &res) > 0) {
		printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}

	
    }

	printf("sum of the first 7 natural numbers is %d", sum);

	mythread_exit(NULL);
}
