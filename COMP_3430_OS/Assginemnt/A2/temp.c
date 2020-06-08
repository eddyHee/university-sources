#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void handle() 
{
	printf("I've been called to serve!\n");
    sleep(1);

    printf("Shoot, I fell asleep, leaving now.");
    
}
int main(void) 
{
    //int counter = 1;

    pid_t pid = fork();

    if (!pid)
    {
        signal(SIGUSR1, handle);
        signal(SIGUSR2, handle);
        signal(SIGINT, handle);
        signal(SIGTRAP, handle);
        printf("I'm a soldier waiting on the Wall.\n");
        while(1)
        {
        }
    }
    else
    {
        printf("I'm the commander calling the shots.\n");
        printf("Sending the signals.\n");
        kill(pid, SIGUSR1);
        sleep(1);
        kill(pid, SIGUSR2);
        sleep(1);        
        kill(pid, SIGINT);
        sleep(1);
        kill(pid, SIGTRAP);

    }
    wait(NULL);
	return EXIT_SUCCESS;
}

