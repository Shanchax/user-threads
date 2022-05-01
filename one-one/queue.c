#include "queue.h"
//Circular Queue Implementation

// Initialization of queue
void init_queue(){
    queue_of_threads = (queue *)malloc(sizeof(queue));
    queue_of_threads->front = NULL;
    queue_of_threads->rear = NULL;
    //queue_of_threads->count = 0;
}


//Enqueue operation for pushing in the thread block 
void enqueue(my_thread *thrd) {
    queue *q = queue_of_threads;
    if (q->front == NULL){
        q->rear = thrd;
        q->front = thrd;
        return;  
    }
    q->rear->next = thrd;
    q->rear = thrd;
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
    queue *q = queue_of_threads;
    my_thread *th = q->front;
    while(th)
    {
        printf("executing with thread id = %d\n", th->thread_id);
        th = th->next;
    }
}

//Thread with defined thread_id return else
my_thread *get_thrd_node(int thread_id) {
    queue *q = queue_of_threads;
    my_thread *th = q->front;
    while(th != q->rear)
    {
        if(th->thread_id == thread_id)
            return th;
        th = th->next;
    }
    return NULL;  
}
