#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

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
    int work_per_thread;
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
void *get_data_and_compare(void *arg)
{
    struct Argument* my_arg = (struct Argument*) arg;
    int current_work = my_arg->start_line;
    int work_period = my_arg->work_per_thread;

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

    return NULL;
}

int main(void)
{   
    int task_start = 0;
    int i;
    struct timespec time1, time2;
    /*
    Store the threads in here
    */
    pthread_t *p;

    /*
    Compute the total work need to be done,
    and the work need to be done for each worker.

    Start counting the system time.
    */
    int work_per_thread;
    int total_work = pow(10, DIGIT);

    int threads = 1;
    printf("Please enter the number of threads you wnat to have: \n");
    scanf("%d", &threads);

    clock_gettime(CLOCK_MONOTONIC, &time1);
    work_per_thread = (int) ceil((double)total_work / (double)threads);
    p = malloc(sizeof(pthread_t) * threads);

    for (i = 0; i < threads; i++)
    {
        struct Argument *arg = malloc(sizeof(struct Argument));
        arg->start_line = task_start;
        arg->work_per_thread = work_per_thread;

        pthread_create(&p[i], NULL, get_data_and_compare, arg);

        task_start += work_per_thread;
        
    }

    for (i = 0; i < threads; i++)
    {
        pthread_join(p[i], NULL);
    }

    free(p);
    /*
    When finish everything, get the system time again.
    */
    clock_gettime(CLOCK_MONOTONIC, &time2);

    /*
    Summary the run time and number of thread.
    */
    printf("The number of threads: %d\n", threads);
    printf("Elapsed time equals %.4f seconds.\n", diff(time1,time2).tv_sec + diff(time1,time2).tv_nsec/1e9);

    return EXIT_SUCCESS;
}
