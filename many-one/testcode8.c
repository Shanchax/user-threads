#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "threads.h"
#include "lock.h"

//stack smashing was faced when we started dealing with numbers more than 20.
// thread args
struct _thread_data_t {
    int tid;
    unsigned long long floor;
    unsigned long long ceiling;
    unsigned long long thread_sum;
};
typedef struct _thread_data_t thread_data_t;

unsigned long long sum;

thread_spinlock lock;

void error( char * msg )
{
    perror( msg );
    exit( 1 );
}


void * thr_func( void * arg )
{
    thread_data_t * data = (thread_data_t *) arg;

    
    // do computation and do to thread
    for( data->floor; data->floor <= data->ceiling; ++(data->floor) )
        data->thread_sum = data->floor * data->thread_sum;

    thread_spinlock_lock(&lock);
    sum = data->thread_sum * sum;
    
    thread_spinlock_lock(&lock);

}

void init_longs( unsigned long long to_factorialize, unsigned long long * half, unsigned long long * three_fourths, unsigned long long * seven_eighths )
{
    unsigned long long two, three, four, seven, eight;
    two = 2;
    three = 3;
    four = 4;
    seven = 7;
    eight = 8;
    *half = to_factorialize / two;
    *three_fourths = (to_factorialize * three) / four;
    *seven_eighths  = (to_factorialize * seven) / eight;
}

int main( int argc, char *argv[] ) 
{
    
    int threads[4];
    thread_data_t thr_data[4];
    unsigned long long to_factorialize;
    unsigned long long half, three_fourths, seven_eighths;
    int i, thr_status;

    sum = 1;
    
    thread_spinlock_init(&lock);

    to_factorialize = strtoull( argv[1], NULL, 10 );
    if( errno )
        error( "Converting to ull failed" );
    if( to_factorialize < 1 || to_factorialize > 20 ) 
        error( "Invalide range: Can only accept numbers from [1,20]" );
    init_longs( to_factorialize, &half, &three_fourths, &seven_eighths );

    //so, here's what we're doing : 
    // four threads, handle from 0-0.5, 0.5-0.75, 0.75-0.875. 0.875-1 fractions!!
        
    // thread1: 1.0      - (.5)
    // thread2: (.5+1)   - (.75)
    // thread3: (.75+1)  - (.875)
    // thread4: (.875+1) - 1.0

    
    for( i = 0; i < 4; ++i )
    {
        thr_data[i].tid = i;
        thr_data[i].thread_sum = 1;
    }

    thr_data[0].floor   = 1;
    thr_data[0].ceiling = half;

    thr_data[1].floor   = half + 1;
    thr_data[1].ceiling = three_fourths;

    thr_data[2].floor   = three_fourths + 1;
    thr_data[2].ceiling = seven_eighths;

    thr_data[3].floor   = seven_eighths + 1;
    thr_data[3].ceiling = to_factorialize;

    // create threads, add thread id to array
    for( i = 0; i < 4; ++i )
    {
        
        threads[i] = mythread_create(thr_func , &thr_data[i]);
        
    }

        //JOINING ON ALL THREADS.
        for (int i = 0; i < 4; ++i) {
	        int id = threads[i];

	        while (1) {
	        void *res;

	         if (mythread_join(id, &res) > 0) {
		
	      	break;
	        }
      	}  

        }  

    printf( "FACTORIAL IS %llu\n", sum );

  





    return 0;
}