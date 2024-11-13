#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 1024

int main()
{
    int pipefd[2];
    pid_t pid;
    char buf[BUFSIZE];
    ssize_t nread;
    int fd;
    if (pipe(pipefd) < 0)
    {
        perror("pipe()");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if (pid == 0) // child read
    {
        close(pipefd[1]); /* Close unused write end */
        // read、阻塞调用
        nread = read(pipefd[0], buf, BUFSIZE);
        write(STDOUT_FILENO, buf, nread);
        close(pipefd[0]);
        exit(0);
    }
    else // parent write
    {
        close(pipefd[0]);
        write(pipefd[1], "Hello!\n", 7);

        close(pipefd[1]);
        wait(NULL);
        exit(0);
    }
}