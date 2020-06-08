#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int pipefd[2]; // fd => file Descriptor
    pid_t pid;
    char buf[3];
    ssize_t amt_read;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <message to send between processes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)  // <= writes into array.
                             // [ read | write]
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork(); 
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {    /* Child reads from pipe */
        close(pipefd[1]);          /* Close unused write end */
        // show that read returns 0 when it is done reading
        amt_read = read(pipefd[0], &buf, 1);
        while (amt_read > 0)
        {
            // printf("%ld\n", amt_read);
            buf[1] = '\n';
            buf[2] = '\0';
            write(STDOUT_FILENO, &buf, 1);
            amt_read = read(pipefd[0], &buf, 1);
        }
        // printf("%ld\n", amt_read);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } 
    else
    {            /* Parent writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}

