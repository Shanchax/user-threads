

#include<stdint.h>
#include<ucontext.h>
#include<setjmp.h>

typedef struct {
	int id;
	
	ucontext_t thread_context;

	int has_dynamic_stack;

	void *(*target_function)(void*);

	void *argument;

	sigjmp_buf env;

	void *return_value;
}TPROC;

TPROC* create_tcb(void);

void delete_tcb(TPROC *block);


