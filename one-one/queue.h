//Structure of Queue
typedef struct queue {
    thread *front;
    thread *rear;
} queue;


//Definations for corresponding Supportive functions of Queue

//Initiallization of queue
void init_queue();
//For insertion of node structure in queue
void enqueue(node *);
//Checking whether que is empty or not
int queue_Empty();


// Queue for threads created
queue *queue_of_threads;