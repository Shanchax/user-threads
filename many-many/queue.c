
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_Q 20

QUEUE* queue_init(){
    QUEUE* q = (QUEUE*)malloc(sizeof(QUEUE));
    q->max = DEFAULT_Q;
    q->size = 0;
    q->q = (int*)malloc(q->max*sizeof(int*));
}

void enqueue(QUEUE* q_ptr, int t_index) {
    // we can't add to a full queue
    if (q_ptr->size >= q_ptr->max) {
        printf("ERROR: queue is full, increase DEFAULT_Q.\n");
        return;
    }

    // basic move up for a heap based on priority.
    int i=q_ptr->size;
    int parent = (i-1)/2;
    q_ptr->q[i] = t_index;
    q_ptr->size++;
    while (i>0 && pri(q_ptr,i) < pri(q_ptr,parent)) {
        swap(q_ptr,i,parent);
        i = parent;
        parent = (i-1)/2;
    }
}

int dequeue(QUEUE* q_ptr) {
    //empty queue is empty
    if (q_ptr->size == 0){
        return -1;
    }

    // basic pop then go  down based on priority
    int t_index = q_ptr->q[0];
    q_ptr->size--;
    q_ptr->q[0] = q_ptr->q[q_ptr->size];
    int i=0;
    while (i < q_ptr->size) {
        if (2*i+1 >= q_ptr->size) {
            break;
        }
        if (pri(q_ptr,i) < pri(q_ptr,2*i+1) && pri(q_ptr,i) < pri(q_ptr,2*i+2)){
            break;
        } else {
            if (pri(q_ptr,2*i+1) < pri(q_ptr,2*i+2)){
                swap(q_ptr,i,2*i+1);
                i = 2*i+1;
            } else {
                swap(q_ptr,i,2*i+2);
                i = 2*i+2;                
            }
        }
    }

    return t_index;
}

// swap two items in the queue at positions i & j
//
void swap(QUEUE* q_ptr, int i, int j) {
    int p = q_ptr->q[i];
    q_ptr->q[i] = q_ptr->q[j];
    q_ptr->q[j] = p;
}



