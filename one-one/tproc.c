#include"trpoc.h"
#include<stdlib.h>

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
	if(block->has_dynamic_stack){
		free(block->context.uc_stack.ss_sp);
	}

	free(block);
}

