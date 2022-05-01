#include "../queue.h"
#include "../lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
// maximum size of matrix
#define MAX 10
 
// maximum number of threads
#define MAX_THREAD 10
 
int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
int rslt[MAX][MAX];
int step_i = 0;

float mulMat(int mat1[][MAX], int mat2[][MAX]);
void* multi(void* arg);
 
void* multi(void* arg)
{
    int i = step_i++; //i denotes row number of resultant matC
   
    for (int j = 0; j < MAX; j++)
      for (int k = 0; k < MAX; k++)
        matC[i][j] += matA[i][k] * matB[k][j];
}

//matrix mult without threads
float mulMat(int mat1[][MAX], int mat2[][MAX]) {

    clock_t start1, end1;
    double cpu_time_used1;
    start1 = clock();
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            rslt[i][j] = 0;
 
            for (int k = 0; k < MAX; k++) {
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
 
            printf("%d ",rslt[i][j]);
        }
 
        printf("\n");
    }

    end1 = clock();
    cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;

    return cpu_time_used1;
}

 
// Driver Code
int main()
{
    // Generating random values in matA and matB
    mythread_init();
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }
    //Displaying matrix A:
    printf("Matrix A : \n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            
            printf("%d " , matA[i][j]);
        
        printf("\n");
    }
 
    // Displaying matB
    
    printf(" Matrix B \n");
         
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)

            printf("%d ",matB[i][j]);    
        
        printf("\n");

    }
 
    // declaring 10 threads
    
    my_thread threads[MAX_THREAD];
    int tid[MAX_THREAD];
 
    // Creating 10 threads, each evaluating its own part
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (int i = 0; i < MAX_THREAD; i++) {
        int* p;
        
        tid[i] = mythread_create( &threads[i],multi, (void*)(p));
    }
 
    for (int i = 0; i < MAX_THREAD; i++) {
	int id = tid[i];

	while (1) {
	    void *res;
	    if (mythread_join(&threads[i], &res) > 0) {
		
		break;
	    }
	}

    }


    //ending clock here after join because all threads will have finished execution bynow.
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Multiplication of matrix A and B with threads is :\n");

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            
            printf("%d ",matC[i][j] ) ;     
        
        printf("\n");
    }

    printf("Matrix multiplication of matrix A and B  without threads:");
    printf("\n");

    // clock_t nothreadbegin = clock();
    // clock_t t;
    // t = clock();
    struct timeval start2, end2;
    gettimeofday(&start2, NULL); 
    float time = mulMat(matA , matB);
    gettimeofday(&end2, NULL);
    long seconds = (end2.tv_sec - start2.tv_sec);
    float micros = ((seconds * 1000000) + end2.tv_usec) - (start2.tv_usec);
    micros = micros / 1000000;
    printf("\n\nTime elapsed without threads: %f seconds\n", micros);
    // t = clock() - t;
    // double time_taken = ((double)t)/CLOCKS_PER_SEC; 
    // clock_t nothreadend = clock();


    int flag = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            
            if(matC[i][j] != rslt[i][j]){

                printf(" \n %d  %d",matC[i][j] , rslt[i][j]);

                flag = 1;
                break;
                
            }  
        if(flag == 1){
            break;
        }    

        
        
    }
    printf("\nTime consumed using threads  : %f ",cpu_time_used);
    printf("\n");
    if(flag == 1){
        printf(" \nTestCase-6 FAILED\n");
    }else{
        printf("\nTestCase-6 PASSED\n");
    }

    
    
    

    return 0;
}