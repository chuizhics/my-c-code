#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t pid;
    puts("Begin!");

    fflush(NULL);
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid == 0)   // child process
    {
        //execl("/usr/bin/sleep","sleep","100",NULL);

        /* 程序会忽略argv[0]  函数名隐藏   dangerous*/
        execl("/usr/bin/sleep","httpd","100",NULL);
        perror("execl()");
        exit(1);
    }

    wait(NULL);

    puts("End!");
    exit(0);
}