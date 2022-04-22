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
// Traverse all the Queue of threads and printing all threads with their cooresponding id's
void print_cur_thrds();


// Queue for threads created
queue *queue_of_threads;

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