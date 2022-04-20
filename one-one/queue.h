#include "my_thread.h"
//Structure of Queue
typedef struct queue {
    my_thread *front;
    my_thread *rear;
} queue;


//Definations for corresponding Supportive functions of Queue

//Initiallization of queue
void init_queue();
//For insertion of node structure in queue
void enqueue(my_thread *);
//Checking whether que is empty or not
int queue_Empty();


// Queue for threads created
queue *queue_of_threads;

//Thread with defined thread_id return else
my_thread *get_thrd_node(int thread_id) {
    queue *que = queue_of_threads;
    my_thread *ptr;
    ptr = que->front;
    if (ptr->thread_id == thread_id)
            return ptr;
    else{
        ptr=ptr->next;
    }
    while (ptr != que->front){
        if (ptr->thread_id == thread_id)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}