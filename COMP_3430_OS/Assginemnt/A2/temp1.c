#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void handle(int arg) 
{
	printf("I've been called to serve!\n");
    for (int i=5; i>0;i-- ){
        printf("I'm tired I'll leave in %d minutes\n",i);
        sleep(1);
    }

    printf("Shoot, I fell asleep, leaving now.");
    // exit(EXIT_SUCCESS);
}
int main(void) 
{
    //int counter = 1;

    pid_t pid = fork();

    if (!pid)
    {
        signal(SIGUSR1, handle);
        signal(SIGUSR2, handle);
        // signal(SIGINT, handle);
        // signal(SIGTRAP, handle);
        printf("I'm a soldier waiting on the Wall.\n");
        while(1)
        {
        }
    }
    else
    {
        printf("I'm the commander calling the shots.\n");
        printf("Sending the signals 1.\n");
        kill(pid, SIGUSR1);
        sleep(2);

        printf("Sending the signals 1 again.\n");
        kill(pid, SIGUSR1);
        sleep(2);        


        printf("Sending the signals .\n");
        kill(pid, SIGUSR2);
        sleep(2);        
        // kill(pid, SIGTRAP);
        sleep(2);
        // kill(pid, SIGUSR1);

    }
	return EXIT_SUCCESS;
}

