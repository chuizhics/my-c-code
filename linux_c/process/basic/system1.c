#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
    pid_t pid;

    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    if(pid == 0)   // child process
    {
        //execl("/usr/bin/date","date","+%s",NULL);
        execl("/bin/sh","sh","-c","date +%s", NULL);
        perror("execl()");
        exit(1);
    }

    wait(NULL);

    exit(0);
}