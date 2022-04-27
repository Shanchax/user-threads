#include "queue.h"


#include <errno.h>
#include <stdlib.h>

//A node in queue is nothing but a TPROC* and pointer to next node
struct node {
    TPROC *thread;
    struct node *next;
};

//ADT queue.
struct queue {
    struct node *head;
    size_t size;
};

//function to create queue
//@param: void
//@returns : pointer to newly created QUEUE
QUEUE *queue_create(void)
{
    QUEUE *new;

    if ((new = calloc(1, sizeof(QUEUE))) == NULL) {
	return NULL;
    }

    return new;
}


//function to delete/destroy a queue completely
//@param: queue to be destroyed
//@returns:void
void delete_queue(QUEUE *queue)
{
    struct node *prev = NULL;
    struct node *cur = queue->head;

    while (cur != NULL) {
	prev = cur;
	cur = cur->next;

	delete_tcb(prev->thread);
	free(prev);
    }

    free(queue);
}



//function to get size of queue
//@param: queue whose size is to be found
//@returns: size of queue
size_t queue_size(const QUEUE *queue)
{
    return queue->size;
}


//function to enqueue a node( thread block ) to queue
//@params : queue to which we will enqueue , element to be enqueued
//@returns : errno in case of failure, 0 in case of success 
int enqueue(QUEUE *queue, TPROC *elem)
{

    struct node *new;

    if ((new = malloc(sizeof(struct node))) == NULL) {
	return errno;
    }

    new->thread = elem;
    new->next = NULL;

    if (queue->head == NULL) {
	queue->head = new;
    } else {
	struct node *parent = queue->head;
	while (parent->next != NULL) {
	    parent = parent->next;
	}
	parent->next = new;
    }

    queue->size += 1;
    return 0;
}


//function to dequeue a thread control block from given queue
//@params: queue from which we wish to dequeue
//@returns: NULL in case of failure, pointer to popped thread in case of success
TPROC *dequeue(QUEUE *queue)
{
    struct node *old_head = queue->head;
    if (old_head == NULL || queue->size == 0) {
        return NULL;
    }
    queue->head = queue->head->next;
    queue->size -= 1;

    TPROC *popped_thread = old_head->thread;
    free(old_head);

    return popped_thread;
}


//function to dequeue a tproc representing thread with given id
//@params: thread id of the thread which is to be dequeued
//@returns:pointer to popped tproc.
TPROC *dequeue_id(QUEUE *queue, int id)
{
    if (queue->head == NULL) {
	return NULL;
    }

    struct node *prev = NULL;
    struct node *cur = queue->head;

    while (cur != NULL) {
	if (cur->thread->id == id) {
	    if (prev == NULL) {
		queue->head = cur->next;
	    } else {
		prev->next = cur->next;
	    }

	    TPROC *popped_thread = cur->thread;
	    free(cur);
	    return popped_thread;
	}

	prev = cur;
	cur = cur->next;
    }

    return NULL;
}
