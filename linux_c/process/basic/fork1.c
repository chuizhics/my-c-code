#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>


int main()
{
    pid_t pid = 0;

    printf("[%d]:Begin!\n",getpid());
    //  全缓冲与行缓冲   如果不加，则重定向输出到文件就会有问题
    // ****在fork()之前应该刷新所有该刷新的流!!!
    // !!!!!
    fflush(NULL);
    


    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if(pid == 0)   /* child */
    {
        printf("[%d]:Child is working!\n",getpid());

    }
    else               /*parent*/
    {
        // sleep(1);
        printf("[%d]:Parent is working!\n",getpid());
    }


    printf("[%d]:End!\n",getpid());

    // getchar();

    exit(0);
}