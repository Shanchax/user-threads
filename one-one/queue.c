#include "queue.h"
//Circular Queue Implementation

// Initialization of queue
void init_queue(){
    queue_of_threads = (queue *)malloc(sizeof(queue));
    queue_of_threads->front = NULL;
    queue_of_threads->rear = NULL;
}


//Enqueue operation for pushing in the thread block 
void enqueue(my_thread *thrd) {
    queue *q = queue_of_threads;
    if (q->front == NULL){
        q->rear = thrd;
        q->front = thrd;  
    }
    else {
        (q->rear)->next = thrd;
        q->rear = thrd;
    }
    q->rear->next = q->front;
}

//Check whether Queue of threads is empty or not
/*
        queue_of_threads->front : NULL     == Queue is Empty
        queue_of_threads->front : not NULL == Queue is Not Empty (Contain thread blocks)
*/
int queue_Empty() {
    return (queue_of_threads->front == NULL);
}