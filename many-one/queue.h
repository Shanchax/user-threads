
#ifndef QUEUE_H
#define QUEUE_H

#include "tproc.h"

#include <unistd.h>

typedef struct queue QUEUE;

//protoypes of queue functions. functions defined in queue.c

QUEUE *queue_create(void);

void delete_queue(QUEUE *queue);

size_t queue_size(const QUEUE *queue);

int enqueue(QUEUE *queue, TPROC *elem);

TPROC *dequeue(QUEUE *queue);

TPROC *dequeue_id(QUEUE *queue, int id);

#endif