#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <signal.h>

#include "anytimer.h"

static int job1 = -1, job2 = -1, job3 = -1;


static void f1(void *arg)
{
    printf("f1():%s\n", (char *)arg);
}

static void f2(void *arg)
{
    printf("f2():%s\n", (char *)arg);
}

static void f3(void *arg)
{
    printf("f3():%s\n", (char *)arg);
}

static void safe_exit(void)
{
    if(job1 >= 0)
    {
        at_waitjob(job1);
        job1 = -1;
        printf("job1 exit.\n");
    }
    if(job2 >= 0)
    {
        at_waitjob(job2);
        job2 = -1;
        printf("job2 exit.\n");
    }
    if(job3 >= 0)
    {
        at_waitjob(job3);
        job3 = -1;
        printf("job3 exit.\n");
    }
}

static void int_handler(int sig)
{
    safe_exit();
    _exit(1);
}

int main()
{

    puts("Begin!");

    atexit(safe_exit);

    signal(SIGINT,int_handler);

    job1 = at_addjob(5, f1, "aaa");
    if(job1 < 0)
    {
        fprintf(stderr,"at_addjob():%s\n",strerror(-job1));
        exit(1);
    }

    job2 = at_addjob(7, f2, "bbb");
    if(job2 < 0)
    {
        fprintf(stderr,"at_addjob():%s\n",strerror(-job2));
        exit(1);
    }

    job3 = at_addjob(3, f3, "ccc");
    if(job3 < 0)
    {
        fprintf(stderr,"at_addjob():%s\n",strerror(-job3));
        exit(1);
    }

    puts("End!");


    while(1)
    {
        sleep(1);
        write(STDOUT_FILENO,".",1);
    }

    exit(0);
}