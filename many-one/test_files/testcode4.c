#include<stdio.h>
#include "../threads.h"
#include<math.h>
#include<stdlib.h>
#define SIZE 7
#define NO_OF_THREADS	8
	
int a[SIZE]={0,3,5,7,10,1,2};

void* unique_pair(void *param)
{
	long start = (long)param;

	int i, j, k, flag;
	//each thread will start one iteration of the outmost for loop and will increment by 8 since
	//the rest 7 threads will do the previous 7 iterations
	for(i=start; i<SIZE-1; i+=8)
	{

		for(j=i+1; j<SIZE; j++)
		{
			//flag is initially set to 1 before starting comparing
			flag=1;
			for(k=0; k<SIZE; k++)
			{
				if(a[i]<a[j] && a[k]>a[i] && a[k]<a[j] )//if a[i]<a[j] and a[k] is in bet a[i] and a[j]
				{
					//flag = 0 if value any value is found between a[i] and a[j]
					flag=0;
					break;
				}

				else if(a[j]<a[i] && a[k]<a[i] && a[k]>a[j])//if a[i]>a[j] and a[k] is in bet a[i] and a[j]
				{
					//flag = 0 if value any value is found between a[i] and a[j]
					flag=0;
					break;
				}

			}
			//if flag still stays 1 throughout the iterations that means no value is found in bet a[i] and a[j]
			//and a[i] and a[j] are unique pairs
			if(flag ==1)
				printf("(%d,%d) or (%d,%d) = %d\n",a[i],a[j],a[j],a[i],abs(a[i]-a[j]));

		}
	}
	return NULL;
}


int main()
{
	

    int tid[NO_OF_THREADS];
	int i;
	for(i =0; i<SIZE; i++)
		printf("%d\t",a[i]);
	printf("\n");
	
	long j;
	
	for(j=0; j<NO_OF_THREADS; j++)
		tid[j] = mythread_create(unique_pair, (void *)j);//each thread will be created and will start executing unique_pair()
	
	// 	mythread_join(tid[j], NULL);//all the threads 

	for (int i = 0; i < NO_OF_THREADS; ++i) {
	int id = tid[i];

	while (1) {
	    void *res;
	    if (mythread_join(id, &res) > 0) {

		break;
	    }
	}


	}
	printf(" \nTestCase-4 PASSED\n");
	return 0;
}