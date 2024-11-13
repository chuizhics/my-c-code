#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/mman.h>



int main(int argc, char *argv[])
{
    int fd;
    int64_t i;
    struct stat buff;
    int64_t count = 0;

    char *str;

    if(argc < 2)
    {
        fprintf(stderr,"Usage:%s <file>\n", argv[0]);
        exit(1);
    }
    fd = open(argv[1],O_RDONLY);
    if(fd < 0)
    {
        perror("open()");
        exit(1);
    }

    if(fstat(fd, &buff) < 0)
    {
        close(fd);
        perror("fstat()");
        exit(1);
    }

    str = mmap(NULL,buff.st_size, PROT_READ,MAP_SHARED,fd, SEEK_SET);
    if(str == MAP_FAILED)
    {
        close(fd);
        perror("mmap()");
        exit(1);
    }
    close(fd);
    
    for(i = 0; i < buff.st_size; i++)
    {
        if(str[i] == 'a')
            count++;
    }

    printf("count = %ld\n", count);

    munmap(str,buff.st_size);

    exit(0);
}