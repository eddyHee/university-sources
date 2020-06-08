// reminder - compile with -lpthread
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h> // for int max

// global variable
// what could possibly go wrong
int sum = 0;
pthread_mutex_t sumLock;

void* threading_task(void* ignored)
{
    // max in divided by 2, one for each thread
    const int myAmount = INT_MAX/2;
    int i;
    int roundedInt = INT_MAX/2*2;
    for (i = 0; i < myAmount; i++)
    {
        pthread_mutex_lock(&sumLock);
        sum = sum + 1;
        printf("%d / %d. %.2f%%\n", sum, roundedInt, sum/(float)roundedInt);
        pthread_mutex_unlock(&sumLock);
    }

    pthread_exit(0);
}


int main()
{
    pthread_t my_thread1;
    pthread_t my_thread2;
    pthread_mutex_init(&sumLock, NULL);

    // init the thread
    pthread_create( &my_thread1, NULL, threading_task, NULL );
    pthread_create( &my_thread2, NULL, threading_task, NULL );

    pthread_join(my_thread1, NULL);
    pthread_join(my_thread2, NULL);

    pthread_mutex_destroy(&sumLock);

    // back to the mainline
    printf("done - total is %d, should be %d\n", sum, INT_MAX/2*2);
    printf("%f\n", (sum)/(float)INT_MAX);

}