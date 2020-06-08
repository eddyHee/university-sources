#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*To finish this handle, need about 10 seconds*/
void handle(int arg) 
{
    int i = 0;

    while(i < 10){
	    printf("%d oh hi mark\n", i);
        sleep(1);
        i++;
    }
    printf("\n ---->  FINISH SIGNAL%d\n", arg);
}


int main(void) 
{
    int i;
    struct
    {
        unsigned int count:2;
    } counter;
    char indicators[] = {'-', '\\', '|', '/'};

    pid_t pid = fork();

    if (!pid)
    {
        signal(SIGUSR1, handle);
        signal(SIGUSR2, handle);

       i = 0;
        while(i<10)
        {
            printf("\rIn the child, waiting: %c.", indicators[counter.count++]);
            fflush(stdout);
            sleep(1);
            i++;
        }
        printf("\nFINISH ALL TASKS!\n");
        exit(EXIT_SUCCESS);
    }
    else
    {

        sleep(3);

        printf("\nSending 1st signal SIGUSR1.\n");
        kill(pid, SIGUSR1);

        sleep(3);
        printf("\nSending 2nd signal SIGUSR1.\n");
        kill(pid, SIGUSR1);

        sleep(3);
        printf("\nSending 3rd signal SIGUSR2.\n");
        kill(pid, SIGUSR2);
                
        sleep(3);
        printf("\nSending 4th signal SIGUSR2.\n");
        kill(pid, SIGUSR2);

        sleep(3);
        printf("\nSending 5th signal SIGUSR2.\n");
        kill(pid, SIGUSR2);
    }

    wait(NULL);
	return EXIT_SUCCESS;
}

