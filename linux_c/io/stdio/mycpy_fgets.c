#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 1024

FILE *fpsrc = NULL;
FILE *fpdest = NULL;

static void close_fpsrc(void)
{
    puts("close_fpsrc().....");
    fclose(fpsrc);
}

static void close_fpdest(void)
{
    puts("close_fpdest().....");
    fclose(fpdest);
}

int main(int argc, char *argv[])
{

    char buf[BUFSIZE];

    if (argc != 3)
    {
        fprintf(stderr, "Usage:%s <src_file> <dest_file>\n", argv[0]);
        exit(1);
    }

    fpsrc = fopen(argv[1], "r");
    if (fpsrc == NULL)
    {
        fprintf(stderr, "fopen() failed!:%s\n", strerror(errno));
        exit(1);
    }

    atexit(close_fpsrc);

    fpdest = fopen(argv[2], "w");
    if (fpdest == NULL)
    {
        fprintf(stderr, "fopen() failed!:%s\n", strerror(errno));
        exit(1);
    }
    atexit(close_fpdest);
    while (fgets(buf, BUFSIZE, fpsrc) != NULL)
    {
        fputs(buf, fpdest);
    }

    exit(0);
}