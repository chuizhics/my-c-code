#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>


#define FNAME "/tmp/out"

int main()
{
    int fd;
    int nwrite = 0;
    // fclose(stdout);
    // fopen(FNAME,"w");
    // close(1);

    fd = open(FNAME,O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(fd < 0)
    {
        perror("open()");
        exit(1);
    }

    //close(1);
    //dup(fd);
    //   以上两步相当于下面这一步
    dup2(fd,1);

    if(fd != 1)
        close(fd);



/************************************/
    nwrite = puts("Hello World!");

    fprintf(stderr,"nwrite = %d stdout->_fileno = %d  fd = %d\n", nwrite, stdout->_fileno, fd);

    fflush(stdout);    // necessary

    close(stdout->_fileno);

    // Restore standard output to terminal
    fd = open("/dev/tty", O_WRONLY);
    
    fprintf(stderr,"newfd = %d\n",fd);

    // Write to terminal
    printf("Hello World!\n");


    exit(0);
}