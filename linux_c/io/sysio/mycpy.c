#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int src_fd, dest_fd;
    int pos = 0;
    char buf[BUFSIZE];
    ssize_t nread = 0;
    ssize_t nwrite = 0;
    if (argc < 3)
    {
        fprintf(stderr, "Usage:%s <src_file> <dest_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd < 0)
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (dest_fd < 0)
    {
        close(src_fd);
        perror("open()");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        nread = read(src_fd, buf, BUFSIZE);
        if (nread < 0) // nread == -1
        {
            perror("read()");
            break;
        }
        if (nread == 0)
            break;
            
        pos = 0;
        while (nread > 0)
        {
            nwrite = write(dest_fd, buf + pos, nread);

            if (nwrite < 0)
            {
                close(dest_fd);
                close(src_fd);
                perror("write()");
                exit(EXIT_FAILURE);
            }
            nread -= nwrite;
            pos += nwrite;
        }
    }
 


    close(dest_fd);
    close(src_fd);

    exit(EXIT_SUCCESS);
}