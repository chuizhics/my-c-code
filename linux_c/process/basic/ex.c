#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

/**
 * 
 * date +%s
 * 
*/
int main()
{
    puts("Begin!");
    /*!!!!!!*/
    fflush(NULL);
    /**repalce   新的进程映像替换旧的进程映像*/
    execl("/usr/bin/date","date","+%s",NULL);
    perror("execl()");
    exit(1);

    puts("End!");
    exit(0);
}