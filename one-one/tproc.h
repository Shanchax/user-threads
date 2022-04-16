

#include<stdint.h>
#include<ucontext.h>
#include<setjmp.h>

//define state which will decide whether our thread is
//joinable or not
//join-state

#define JOINABLE 1
#define DETACHED 2

//status of thread: could be running, runnable, waiting, 

#define RUNNING 1
#define RUNNABLE 2
#define TERMINATED 3
#define WAITING 4
#define JOINED 5


typedef struct {
	int id;
	
	ucontext_t thread_context;

	int has_dynamic_stack;

	void *(*target_function)(void*);

	void *argument;

	int join_state;

	int thread_status;

	int wait_id;
	
	sigjmp_buf env;

	void *return_value;
}TPROC;

TPROC* create_tcb(void);

void delete_tcb(TPROC *block);


