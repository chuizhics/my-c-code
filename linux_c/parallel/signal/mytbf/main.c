#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "mytbf.h"

#define CPS 10

#define BUFSIZE 1024

#define BURST 100 

int main(int argc, char *argv[])
{
    int src_fd;
    int dest_fd = STDOUT_FILENO;
    int pos = 0;
    char buf[BUFSIZE];
    ssize_t nread = 0;
    ssize_t nwrite = 0;

    mytbf_t *tbf = NULL;
    int token = 0;

    if (argc < 2)
    {
        fprintf(stderr, "Usage:%s <filename>\n", argv[0]);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    tbf = mytbf_init(CPS,BURST);
    if(tbf == NULL)
    {
        fprintf(stderr,"mytbf_init() failed!\n");
        fflush(stderr);
        exit(1);
    }

    do
    {
        src_fd = open(argv[1], O_RDONLY);

        if (src_fd < 0)
        {
            if (errno != EINTR)
            {
                perror("open()");
                exit(EXIT_FAILURE);
            }
        }

    } while (src_fd < 0);

    while (1)
    {
        token =  mytbf_fetchtoken(tbf,BUFSIZE);
        if(token < 0)
        {
            fprintf(stderr,"mytbf_fetchtoken():%s\n", strerror(-token));
            close(src_fd);
            exit(1);
        }

        while ((nread = read(src_fd, buf, token)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("read()");
                break;
            }
        }

        if (nread == 0)
            break;


        if(token - nread > 0)
        {
            mytbf_returntoken(tbf,token - nread);
        }

        pos = 0;
        while (nread > 0)
        {
            while ((nwrite = write(dest_fd, buf + pos, nread)) < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                close(src_fd);
                perror("write()");
                exit(EXIT_FAILURE);
            }
            nread -= nwrite;
            pos += nwrite;
        }

    }

    close(src_fd);

    mytbf_destroy(tbf);

    exit(EXIT_SUCCESS);
}