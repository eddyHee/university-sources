#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void handle(int arg) 
{
	printf("oh hi mark\n");
    exit(EXIT_SUCCESS);
}

int main(void) 
{
    struct
    {
        unsigned int count:2; // This is telling compiler to only set 2 bits for this unsigned int
    } counter;
    char indicators[] = {'-', '\\', '|', '/'};

    pid_t pid = fork();

    if (!pid)
    {
        /*
        Child process as soon as it start, called this funciton.
        
        This line will do the following things:
            1. tells kernel, it is interesing in SIGINT events.
               when its happends, run function "handle".
            
        */
        signal(SIGINT, handle);

        /*
        Then the chile process will do the following things and on its own.
        */
        while(1)
        {
            printf("\rIn the child, waiting: %c.", indicators[counter.count++]);
            fflush(stdout);
            sleep(1);
        }
    }
    else
    {
        printf("In the parent, going to sleep\n");
        sleep(5);
        printf("Sending a signal.\n");
        /*
        2 argument:
        first one is process id
        second is the signel to it.

        bascally it is telling:
        kernel send this message(SIGINT) to the child process.
        */
        kill(pid, SIGINT);

        /*
        The parent is waiting the child died, then exit.
        */
    }
	return EXIT_SUCCESS;
}

