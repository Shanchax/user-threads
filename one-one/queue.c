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

// Traverse all the Queue of threads and printing all threads with their cooresponding id's
void print_cur_thrds() {
    my_thread *thrd_blk;
    queue *que = queue_of_threads;
    thrd_blk = que->front;
    if (thrd_blk->is_completed == 1){
            printf("Executing...My Thread id is: %d\n", thrd_blk->thread_id);
            thrd_blk = thrd_blk->next;
    }
    else{
        thrd_blk=thrd_blk->next;
    }
    while (thrd_blk != que->front){
        if (thrd_blk->is_completed == 1)
            printf("Executing...My Thread id is: %d\n", thrd_blk->thread_id);
        thrd_blk = thrd_blk->next;
    }
}

//Thread with defined thread_id return else
my_thread *get_thrd_node(int thread_id) {
    if(!thread_id)
        return NULL;
    my_thread *thrd_blk;
    queue *que = queue_of_threads;
    thrd_blk = que->front;
    if (thrd_blk->thread_id == thread_id)
            return thrd_blk;
    else{
        thrd_blk=thrd_blk->next;
    }
    while (thrd_blk != que->front){
        if (thrd_blk->thread_id == thread_id)
            return thrd_blk;
        thrd_blk = thrd_blk->next;
    }
    return NULL;
}
