#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

/*
Can only use control z to stop this program.
control c only keep print "oh hi mark."

but contol z is only telling the program to stop.
not kill.
fg will tell it keep going again.

kill -SIGKILL [the process number] will actually kills it.

*/
void handle(int arg) 
{
	printf("oh hi mark\n");
    // exit(EXIT_SUCCESS);
}
int main(void) 
{
    struct
    {
        unsigned int count:2;
    } counter;
    char indicators[] = {'-', '\\', '|', '/'};

	signal(SIGINT, handle);
	while (1)
    {
        printf("\r%c", indicators[counter.count++]);
        fflush(stdout);
        sleep(1);
    }
	return EXIT_SUCCESS;
}

