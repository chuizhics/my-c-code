#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#define LEFT 30000000
#define RIGHT 30000200

int main()
{
    pid_t pid;
    int i, j, mark;

    for (i = LEFT; i < RIGHT; i++)
    {
        pid = fork();

        if (pid < 0)
        {
            perror("fork()");
            exit(1);
        }
        else if (pid == 0) // child process
        {
            mark = 1;
            for (j = 2; j <= i / 2; j++)
            {
                if (i % j == 0)
                {
                    mark = 0;
                    break;
                }
            }
            if (mark)
            {
                printf("%d is a primer.\n", i);
            }

            //sleep(1000);
            /* !!!!! */
            exit(0);
        }
        else
        {
        }
    }
    //sleep(1000);

    exit(0);
}