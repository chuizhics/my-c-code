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
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        fd = open("/dev/null", O_RDWR);
        if (fd < 0)
        {
            perror("open()");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        execl("/usr/bin/mpg123", "mpg123", "-", NULL);
        perror("execl()");
        // read、阻塞调用
        // nread = read(pipefd[0], buf, BUFSIZE);
        // write(STDOUT_FILENO, buf, nread);
        exit(1);
    }
    else // parent write
    {
        close(pipefd[0]);
        //write(pipefd[1], "Hello!\n", 7);
        //父进程从网上收数据，往管道中写
        
        close(pipefd[1]);
        wait(NULL);
        exit(0);
    }
}