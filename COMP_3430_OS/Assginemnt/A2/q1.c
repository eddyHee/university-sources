#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define BUFSIZE 3
#define ARGMAXSIZE 3
#define PIPEFAIL -1
#define FORKFAIL -1
#define ISCHILD 0
#define READ 0
#define WRITE 1
#define is_c "c"
#define is_C "C"
#define is_a "a"
#define is_A "A"
#define is_t "t"
#define is_T "T"
#define is_d "d"
#define is_D "D"
#define is_o "o"
#define is_O "O"
#define is_g "g"
#define is_G "G"
#define DOG "dog"
#define CAT "cat"
#define DEL 8

bool tocat = false;
bool todog = true;

void dtoc() { /*SIGUSR1*/
    tocat = true;
    todog = false;
}

void ctod() { /*SIGUSR2*/
    tocat = false;
    todog = true;
}

void normal() { /*SIGALRM*/
    tocat = false;
    todog = false;
}

int main(int argc, char **argv) {
    int pipefd[2];
    pid_t pid;
    char buf[BUFSIZE]; 
    ssize_t amt_read;
    char *myargs[ARGMAXSIZE];

    bool findfirst = false;
    bool findsecond = false;
    
    if (argc != ARGMAXSIZE) {
        fprintf(stderr, "Usage: %s <message to send between processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == PIPEFAIL) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); 
    if (pid == FORKFAIL) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == ISCHILD) {
        close(pipefd[READ]);
        dup2(pipefd[WRITE], STDOUT_FILENO);

        myargs[0] = strdup(argv[1]);
        myargs[1] = strdup(argv[2]);
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
        
        close(pipefd[WRITE]);
        exit(EXIT_SUCCESS);
    } 
    else {            
        close(pipefd[WRITE]);
        signal(SIGUSR1, dtoc);
        signal(SIGUSR2, ctod);
        signal(SIGALRM, normal);

        amt_read = read(pipefd[READ], &buf, 1);
        while (amt_read > 0) {
            write(STDOUT_FILENO, &buf, 1);
            if (todog) {
                if (!findfirst && strncasecmp(&buf[0], is_C, 1) == 0){
                    findfirst = true;
                }else if (findfirst && !findsecond && strncasecmp(&buf[0], is_A, 1) == 0) {
                    findsecond = true;
                }else if (findfirst && findsecond && strncasecmp(&buf[0], is_T, 1) == 0) {
                    buf[0] = DEL;
                    write(STDOUT_FILENO, &buf, 1);
                    write(STDOUT_FILENO, &buf, 1);
                    write(STDOUT_FILENO, &buf, 1);
                    write(STDOUT_FILENO, DOG, sizeof(DOG));
                    findfirst = false;
                    findsecond = false;
                }else {
                    findfirst = false;
                    findsecond = false;
                }
            }
            if (tocat) {
                if (!findfirst && strncasecmp(&buf[0], is_D, 1) == 0){
                    findfirst = true;
                }else if (findfirst && !findsecond && strncasecmp(&buf[0], is_O, 1) == 0) {
                    findsecond = true;
                }else if (findfirst && findsecond && strncasecmp(&buf[0], is_G, 1) == 0) {
                    buf[0] = DEL;
                    write(STDOUT_FILENO, &buf, 1);
                    write(STDOUT_FILENO, &buf, 1);
                    write(STDOUT_FILENO, &buf, 1);
                    write(STDOUT_FILENO, CAT, sizeof(CAT));
                    findfirst = false;
                    findsecond = false;
                }else {
                    findfirst = false;
                    findsecond = false;
                }
            }
            
            amt_read = read(pipefd[READ], &buf, 1);
        }

        wait(NULL);

        close(pipefd[READ]);
        exit(EXIT_SUCCESS);
    }
}

