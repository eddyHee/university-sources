#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*
Define password here
*/
#define PASSWORD "3969429"

/*
define how many digit does the password have
*/
#define DIGIT 7

/*
define how many character in one line.
*/
#define PASSWORD_LEN 100

struct Argument
{
    int start_line;
    int work_per_process;
};

/*
This function will compute the difference between
start time and end time.
*/
struct timespec diff(struct timespec start, struct timespec end)
{
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

/*
This function will compare my_password with password
if they are the same, return mt_password.
Otherwise return NULL.
*/
char* compare(char my_password[PASSWORD_LEN])
{
    my_password = strtok(my_password, "\n");
    if(strcmp(my_password, PASSWORD) == 0)
    {
        return my_password;
    }
    return NULL;
}

/*
This function get 2 arguments stored in arg:
start_work: where to start working
work_period: how much work todo.

This function will generate data start from start_work, to the next work_period
for each data, it use compare function to get the result.

If compare return NULL, then keep going.
If compare return some other char pointer, then
print out that char pointer and quit. We found the password.
*/
void get_data_and_compare(void *arg)
{
    struct Argument* my_arg = (struct Argument*) arg;
    int current_work = my_arg->start_line;
    int work_period = my_arg->work_per_process;

    char my_password[PASSWORD_LEN];
    int i;
    for(i = 0; i < work_period; i++)
    {
        char* result;
        current_work = current_work + 1;
        sprintf(my_password, "%07d", current_work);
        result = compare(my_password);
        if(result != NULL)
        {
            printf("The password is %s\n", result);
        }
    }
    free(my_arg);
}

int main(void)
{
    int task_start = 0;
    int i;
    struct timespec time1, time2;
    /*
    Store the child process id in here
    */
    pid_t *children;
    pid_t pid;

    /*
    Compute the total work need to be done,
    and the work need to be done for each worker.

    Start counting the system time.
    */
    int work_per_process;
    int total_work = pow(10, DIGIT);

    int process = 1;
    printf("Please enter the number of processes you wnat to have: \n");
    scanf("%d", &process);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    work_per_process = (int) ceil((double)total_work / (double)process);
    children = malloc(sizeof(pid_t) * process);

    for (i = 0 ; i < process; i++)
    {
        struct Argument *arg = malloc(sizeof(struct Argument));
        arg->start_line = task_start;
        arg->work_per_process = work_per_process;

        pid = fork();

        if (pid == 0)
        {
            get_data_and_compare(arg);
            exit(0);
        }
        else
        {
            children[i] = pid;
            task_start += work_per_process;
        }
    }

    for (i = 0; i < process; i++)
    {
        int status;
        waitpid(children[i], &status, 0);
    }

    /*
    When finish everything, get the system time again.
    */
    clock_gettime(CLOCK_MONOTONIC, &time2);

    /*
    Summary the run time and number of thread.
    */
    printf("The number of process: %d\n", process);
    printf("Elapsed time equals %.4f seconds.\n", diff(time1,time2).tv_sec + diff(time1,time2).tv_nsec/1e9);

    return EXIT_SUCCESS;
}
