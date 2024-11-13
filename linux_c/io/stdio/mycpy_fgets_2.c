#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUFSIZE 1024



static void close_fpsrc(FILE *fpsrc)
{
    puts("close_fpsrc().....");
    fclose(fpsrc);
}

static void close_fpdest(FILE *fpdest)
{
    puts("close_fpdest().....");
    fclose(fpdest);
}

int main(int argc, char *argv[])
{

    FILE *fpsrc = NULL;
    FILE *fpdest = NULL;
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

    //atexit((void(*)(void))close_fpsrc,fpsrc);

    fpdest = fopen(argv[2], "w");
    if (fpdest == NULL)
    {
        fclose(fpsrc);
        fprintf(stderr, "fopen() failed!:%s\n", strerror(errno));
        exit(1);
    }
    //atexit(close_fpdest);
    while (fgets(buf, BUFSIZE, fpsrc) != NULL)
    {
        fputs(buf, fpdest);
    }
    fclose(fpdest);
    fclose(fpsrc);

    exit(0);
}