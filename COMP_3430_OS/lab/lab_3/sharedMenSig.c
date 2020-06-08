#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int *shared;

void handle(int arg) 
{      
    printf("%d\n", shared[1] + shared[0]);
    // fflush(stdout);
    exit(EXIT_SUCCESS);
}

int main(void) 
{
    int size = 100 * sizeof(int);  
    void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("Mapped at %p\n", addr);

    shared = addr;
    
    pid_t pid = fork();

    if (!pid)
    {
        signal(SIGINT, handle);

        while(1){}
    }
    else
    {
        printf("In the parent, going to sleep\n");
        shared[0] = 10;
        shared[1] = 20;
        sleep(5);
        printf("Sending a signal.\n");

        kill(pid, SIGINT);

    }
    
    munmap(addr,size);
    return 0;
}

