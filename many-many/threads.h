/* Required specs for project 1 */
void mythread_init(int numKernelThreads);
int  mythread_create(void (* func)());
void mythread_yield();
void mythread_exit();
