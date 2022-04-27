
#include<stdio.h>
#include<threads.h>

 
// maximum size of matrix
#define MAX 4
 
// maximum number of threads
#define MAX_THREAD 4
 
int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
int step_i = 0;
 
void* multi(void* arg)
{
    int i = step_i++; //i denotes row number of resultant matC
   
    for (int j = 0; j < MAX; j++)
      for (int k = 0; k < MAX; k++)
        matC[i][j] += matA[i][k] * matB[k][j];
}
 
// Driver Code
int main()
{
    // Generating random values in matA and matB
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }


    printf("Matrix A : \n");
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            // cout << matA[i][j] << " ";
            printf("%d" , matA[i][j]);
        // cout << endl;
        printf("\n");
    }
 
    // Displaying matB
    // cout << endl
    printf(" Matrix B \n");
         
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            // cout << matB[i][j] << " "; 
            printf("%d",matB[i][j]);    
        // cout << endl;
        printf("\n");

    }
 
    // declaring four threads
    // pthread_t threads[MAX_THREAD];
    int threads[MAX_THREAD];
 
    // Creating four threads, each evaluating its own part
    for (int i = 0; i < MAX_THREAD; i++) {
        int* p;
        //pthread_create(&threads[i], NULL, multi, (void*)(p));
        threads[i] = mythread_create( multi, (void*)(p));
    }
 
    
    for (int i = 0; i < MAX_THREAD; ++i) {
	int id = threads[i];

	while (1) {
	    void *res;

	    if (mythread_join(id, &res) > 0) {
		printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}

    }
 

    printf(" Multiplication of matrix A and B is \n:");

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)
            // cout << matC[i][j] << " "; 
            printf("%d",matC[i][j] ) ;     
        // cout << endl;
        printf("\n");
    }
    return 0;
}