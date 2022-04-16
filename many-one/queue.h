
#include"tproc.h"

#include<unistd.h>

typedef struct queue QUEUE;

QUEUE *queue_create(void);

void delete_queue(QUEUE *queue);

size_t queue_size(const QUEUE *queue);

int enqueue(QUEUE* queue, TPROC* elem);

TPROC* dequeue(QUEUE* queue);

TPROC* dequeue_id(QUEUE* queue, int id);

