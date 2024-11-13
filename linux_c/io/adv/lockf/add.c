#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string.h>

#define PRONUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

static void func_add(void)
{
    int fd;
    FILE *fp = NULL;
    char line[LINESIZE];

    fp = fopen(FNAME, "r+");
    if (fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }

    fd = fileno(fp);
    if(fd < 0)
    {
        perror("fileno()");
        exit(1);
    }
    
    lockf(fd, F_LOCK, 0);

    fgets(line, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET);
    // sleep(1);
    fprintf(fp,"%d\n",atoi(line) + 1);

    fflush(fp);
    lockf(fd,F_ULOCK, 0);

    fclose(fp);
    
    return;
}

int main()
{
    int i, j;
    pid_t pid[PRONUM];

    for (i = 0; i < PRONUM; i++)
    {
        pid[i] = fork();
        if (pid[i] < 0)
        {
            perror("fork()");
            for(j = 0; j < i; j++)
            {
                wait(NULL);
            }
            exit(1);
        }
        if(pid[i] == 0)
        {
            func_add();
            exit(0);
        }
    }

    for (i = 0; i < PRONUM; i++)
    {
        wait(NULL);
    }


    exit(0);
}