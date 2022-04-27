#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "threads.h"
#include "tproc.h"
#include <time.h>

static void debug(const char *msg) {
	time_t t = time(NULL);
	fprintf(stderr, "%s%s\n\n", ctime(&t), msg);
}

static void handler(int signum)
{
	//pthread_exit(NULL);
    mythread_exit(NULL);
    
}

void *thread1(void *arg)
{
	static sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGUSR1);

	debug("Blocking SIGUSR1 in thread...");

    sigprocmask(SIG_BLOCK , &mask , NULL);

	// if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
	// 	perror("pthread_sigmask");
	// 	exit(1);
	// }

	debug("Do something...");

	sleep(10);

	debug("Unblocking SIGUSR1 in thread...");

	// if (pthread_sigmask(SIG_UNBLOCK, &mask, NULL) != 0) {
	// 	perror("pthread_sigmask");
	// 	exit(1);
	// }

    sigprocmask(SIG_UNBLOCK , &mask , NULL);    

	debug("Will not reach here...");

	return NULL;
}

int main(int argc, char *argv[])
{

    int tid;

	signal(SIGUSR1, handler);


    int i = rand()%10;
    void *arg = (void *) i;
    tid = mythread_create(thread1, arg);

    

	sleep(5);

	debug("Sending SIGUSR1 to thread...");

    thread_kill(tid,SIGUSR1);
 

    while (1) {
	    void *res;

	    if (mythread_join(tid, &res) > 0) {
		//printf("joined thread %d with result %p\n", tid, res);
		
		break;
	    }
	}
	debug("Thread exited.");

    

	return 0;
}