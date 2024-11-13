#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT 30000000
#define RIGHT 30000200
/*  [LEFT,RIGHT) */

#define N 3

static int isprimer(u_int32_t number)
{
    u_int32_t i = 0;

    if (number < 2)
        return 0;
    for (i = 2; i * i <= number; i++)
    {
        if (number % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    pid_t pid;
    int i, j, n, mark;
    for (n = 0; n < N; n++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            for (i = 0; i < n; i++)
            {
                wait(NULL);
            }
            exit(1);
        }
        if (pid == 0) // child process
        {
            for (i = LEFT + n; i < RIGHT; i += N)
            {
                if (isprimer(i))
                {
                    printf("[%d] %d %d is a primer.\n", n,getpid(),i);
                }
            }
            exit(0);
        }
        else
        {
            // do nothing
        }
    }

    for (n = 0; n < N; n++)
    {
        wait(NULL);
    }

    exit(0);
}