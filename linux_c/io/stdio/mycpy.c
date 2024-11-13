#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{

    FILE *fpsrc = NULL;
    FILE *fpdest = NULL;
    int ch;

    if(argc != 3)
    {
        fprintf(stderr,"Usage:%s <src_file> <dest_file>\n",argv[0]);
        exit(1);
    }

    fpsrc = fopen(argv[1],"r");
    if(fpsrc == NULL)
    {
        fprintf(stderr,"fopen() failed!:%s\n",strerror(errno));
        exit(1);
    }
    fpdest = fopen(argv[2],"w");
    if(fpdest == NULL)
    {
        fprintf(stderr,"fopen() failed!:%s\n",strerror(errno));
        fclose(fpsrc);
        exit(1);
    }
    while ((ch = fgetc(fpsrc)) != EOF)
    {
        fputc(ch, fpdest);
    }


    fclose(fpdest);
    fclose(fpsrc);

    exit(0);
}