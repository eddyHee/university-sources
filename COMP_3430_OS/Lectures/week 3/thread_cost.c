#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 100//000

void *my_sleep(void *arg)
{
    printf("Hi i am a thread\n");
    return NULL;
}

int main(void)
{

    pthread_t p[THREADS];
    for (int i = 0; i < THREADS; i++)
    {
        pthread_create(&p[i], NULL, my_sleep, NULL);
    }
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(p[i], NULL);
    }

    return EXIT_SUCCESS;
}
