#include "../threads.h"

#include <stdio.h>
#include <stdlib.h>
#define GREEN   "\033[32m"
#define BOLDRED     "\033[1m\033[31m"
#define RESET   "\033[0m"


static long sum = 0;



static void *thread1(void *arg)
{

    sum = sum + (long int )arg;
	

}

int sumofnaturalnumbers(int n){

	int sumnew = 0;

	for(int i = 0 ; i < n ; i++){

		sumnew = sumnew + i;

	}

	return sumnew;
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
		//printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}

    }
	 int normalsum = sumofnaturalnumbers(8);
	printf("sum of the first 8 natural numbers is %ld \n", sum);

	if(sum == normalsum){

		printf(" \n TEST PASSED");
	}else{
		printf("\n TEST FAILED");
	}

	

	mythread_exit(NULL);
}
