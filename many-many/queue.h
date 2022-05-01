#ifndef QUEUE_H
#define QUEUE_H

//priority queue, each thread will have a priority assosciated with it.
typedef struct QUEUE{
    int size;
    int max;
    int* q;
} QUEUE;
/* queue initialization function*/
QUEUE*  queue_init();

/* queue management functions */
void        enqueue(QUEUE* q_ptr, int t_index);
int         dequeue(QUEUE* q_ptr);
void        swap(QUEUE* q_ptr, int i, int j);
int         pri(QUEUE* q_ptr, int i);

#endif