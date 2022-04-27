
#include "tproc.h"
#include <stdlib.h>
#include <sys/mman.h>

#define STACKSIZE (64*1024)

//function to create a "thread control block", named as tproc by us(named after struct proc in xv6.)
//@params:void
//@returns:pointer to the newly created "tproc"
TPROC *create_tcb(void){
    static int next_id = 1;
    TPROC *new;

    if ((new = calloc(1, sizeof(TPROC))) == NULL) {
	    return NULL;
    }
    
    new->id = next_id++;
    return new;
}

//function to delete given tproc
//@params: pointer to the tproc to be deleted
//@returns:(idealy we intended to use mprotect to return errno on failure. 
//however it's causing segfault)
void delete_tcb(TPROC *block)
{
    if (block->has_dynamic_stack) {
    munmap(block->context.uc_stack.ss_sp,STACKSIZE);
    }

    free(block);
}

