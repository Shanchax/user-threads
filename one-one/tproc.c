#include"trpoc.h"
#include<stdlib.h>
#include <sys/mman.h>

#define STACKSIZE (1024*1024)

TPROC* create_tcb(void){
	static int next_id = 1;
	TPROC* new;

	if((new = calloc(1,sizeof(TPROC))) == NULL){
		return NULL;
	}
	
	new->id = next_id + 1;
	return new;
}

void delete_tcb(TPROC*block){
	if(block->has_dynamic_stack==1){
		//free(block->context.uc_stack.ss_sp);
		munmap(block->thread_context.uc_stack.ss_sp,STACKSIZE);
	}

	free(block);
}

