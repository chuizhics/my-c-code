#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    if(argc < 2)
    {
        fprintf(stderr,"Usage:%s <file>\n", argv[0]);
        exit(1);
    }
    fp = fopen(argv[1],"r");
    if(fp == NULL)
    {
        perror("fopen failed!\n");
        exit(1);
    }
    //rewind(fp);
    fseek(fp,0L,SEEK_END);

    printf("%ld\n",ftell(fp));


    fclose(fp);

    exit(0);
}