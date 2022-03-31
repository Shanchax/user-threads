#include "queue.h"
#include<stdlib.h>
#include<errno.h>

struct node{
	TPROC* thread;
	struct node *next;
};

struct queue{
	struct node* head;
	size_t size;
};

QUEUE* queue_create(void){
	QUEUE* new;

	if((new = calloc(1,sizeof(QUEUE))) == NULL){
		return NULL;
	}
	
	return new;
}

void delete_queue(QUEUE* queue){

	struct node* prev;
	struct node* cur = queue->head;

	while(cur!=NULL){
		prev = cur;
		cur = cur->next;
		delete_tcb(prev->thread);
		free(prev);
	}

	free(queue);
}


size_t queue_size(const QUEUE* queue){

	return queue->size;
}

int enqueue(QUEUE* queue, TPROC* elem){

	struct node* new;

	if((new= malloc(sizeof(struct node))) == NULL){
		perror("malloc failed");
		return errno;
	}

	new->thread = elem;
	new->next = NULL;

	if(queue->head == NULL){
		queue->head = new;
	}else{
		struct node* parent = queue->head;
		while(parent->next!= NULL){
			parent = parent->next;
		}
		parent->next = new;
	}

	queue_>size = queue->size + 1;

	return 0;
}

TPROC dequeue*(QUEUE* queue){
	struct node* old_head = queue->head;
	if(old_head == NULL || queue->size == 0){
		return NULL;
	}

	queue->head = queue-head->next;

	queue->size = queue->size - 1;

	TPROC* popped_thread = old_head->thread;

	free(old_head);
	return popped_thread;
}

TPROC *queue_remove_id(QUEUE* queue, int id){
	if(queue->head == NULL || queue->size == 0){
		return NULL;
	}

	struct node* prev = NULL;
	struct node* cur = queue->head;

	while(cur!= NULL){
		if(cur->thread->id == id){
			if(prev == NULL){
				queue->head = cur->next;
			} else {
				prev->next = cur->next;
			}

			TPROC* popped_thread = cur->thread;
			free(cur);
			return popped_thread;
		}

		prev = cur;
		cur = cur->next;

	}

	return NULL;

}
