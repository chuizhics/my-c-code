#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


static off_t flen(const char *filename)
{
    struct stat buf;
    
    if(stat(filename,&buf) != 0)
    {
        perror("stat()");
        return -1L;
    }

    return buf.st_size; 
}

int main(int argc, char* argv[])
{
    long length = 0;

    if(argc < 2)
    {
        fprintf(stderr,"Usage.....\n");
        exit(1);
    }

    length = flen(argv[1]);

    if(length < 0)
    {
        fprintf(stderr,"file error!\n");
        exit(1);
    }
    printf("total size: %ld\n", length);

    exit(0);
}