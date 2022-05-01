#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "threads.h"
#include "lock.h"

static void debug(const char *msg) {
	time_t t = time(NULL);
	fprintf(stderr, "%s%s\n\n", ctime(&t), msg);
}

static void handler(int signum)
{
	mythread_exit(NULL);
}

static void sigseg_lock(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGSEGV);

    int ret_val = sigprocmask(SIG_BLOCK, &set, NULL);
    if ( ret_val == -1) {
	perror("sigprof ock failed");
	abort();
    }
}

static void sigseg_unlock(void)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGSEGV);

    int ret_val = sigprocmask(SIG_UNBLOCK, &set, NULL);
    if (ret_val == -1) {
	perror("sigprof unlock failed");
	abort();
    }
}

void *thread(void *arg)
{
	static sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGSEGV);

	debug("Blocking SIGUSR1 in thread...");


	sigseg_lock();

	debug("Do something...");

	sleep(1);

	debug("Unblocking SIGUSR1 in thread...");

	

	sigseg_unlock();

	sleep(10);
	debug("Will not reach here...");

	return NULL;
}

int main(int argc, char *argv[])
{


	int tid;

	signal(SIGUSR1, handler);


	int i = rand()%10;
    void *arg = (void *) i;
	arg = NULL;
	tid = mythread_create(thread, arg);

	sleep(5);

	debug("Sending SIGUSR1 to thread...");
	mythread_kill(tid,SIGSEGV);

	while (1) {
	    void *res;

	    if (mythread_join(tid, &res) > 0) {
		
		
		break;
	    }
	}

	debug("Thread exited.");

	return 0;
}