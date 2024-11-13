#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int src_fd, dest_fd = STDOUT_FILENO;
    int pos = 0;
    char buf[BUFSIZE];
    ssize_t nread = 0;
    ssize_t nwrite = 0;
    if (argc < 2)
    {
        fprintf(stderr, "Usage:%s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
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

        nread = read(src_fd, buf, BUFSIZE);
        if (nread < 0) // nread == -1
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

        pos = 0;
        while (nread > 0)
        {
            nwrite = write(dest_fd, buf + pos, nread);

            if (nwrite < 0)
            {
                if(errno == EINTR)
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

    exit(EXIT_SUCCESS);
}