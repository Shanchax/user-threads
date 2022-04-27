
#include "threads.h"

#include "lock.h"
#include<time.h>
#include <stdio.h>
#include<string.h>

//code credits = https://github.com/wasimusu/cpp_multithreading;


//std::atomic<bool> capture(false);

int capture = 0;
thread_spinlock lock;

void capture_image(const int device_id) {
    while (!capture) {
        // std::this_thread::yield();
        mythread_yield();
    }

    // unique_lock<mutex> lock(mu);
    thread_spinlock_lock(&lock);
    // std::cout << "Camera " << device_id << " clicked at " << chrono::system_clock::now().time_since_epoch().count()
    //           << std::endl;

    time_t mytime = time(NULL);
    char * time_str = ctime(&mytime);
    time_str[strlen(time_str)-1] = '\0';
    printf("Current Time : %s\n", time_str);

    printf("Camera = %d \n clicked at = %s",device_id ,time_str);

    thread_spinlock_unlock(&lock);

    // Pretend to do some work (capture image)
    // ..
    // this_thread::sleep_for(chrono::milliseconds(rand() % 1000));
    sleep(1);
    // ..
}

void* runner( void* arg){

    capture_image((int)arg);
}

int main() {
    // Syncing two cameras. We can sync multiple cameras as well.
    // thread t1(capture_image, 1);
    // thread t2(capture_image, 2);

    int threads[19];

    for (unsigned long i = 0; i < 19; ++i) {
	void *arg = (void *) i;

	if ((threads[i] = mythread_create(runner, arg)) == -1) {
	    perror("mythread_create");
	    //exit(EXIT_FAILURE);
	}
    }

    capture = 1;

    for (int i = 0; i < 19; ++i){
        int id = threads[i];

    while (1) {
	    void *res;

	    if (mythread_join(id, &res) > 0) {
		printf("joined thread %d with result %p\n", id, res);
		
		break;
	    }
	}

    }


    return 0;

}