#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>

int main()
{
    pid_t cpid = 0;
    pid_t ppid = 0;

    cpid = getpid();
    ppid = getppid();

    printf("cpid = %d, ppid = %d\n", cpid, ppid);

    exit(0);
}