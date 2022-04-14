typedef struct thread_mutex{
    int flag;
} thread_mutex;
int thread_mutex_lock(thread_mutex *);
int thread_mutex_unlock(thread_mutex *);