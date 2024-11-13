#include <stdio.h>
#include <stdlib.h>


#define BUFSIZE 4096

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    long i = 0;
    int nwrite = 0;
    long loop = 1024L*1024L;
    char buf[BUFSIZE];
    if (argc < 2)
    {
        fprintf(stderr, "Usage....\n");
        exit(1);
    }

    fp = fopen(argv[1],"w");
    if(fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }

    for(i = 0; i < BUFSIZE - 1; i++)
    {
        buf[i] = (('a' + i) % 127) + 1;  //防止出现'\0' fputs遇见'\0'结束
    }
    buf[i] = '\0';   // 必要的

    while (loop--)
    {
        nwrite = fputs(buf,fp);   // nwrite = 1
        //printf("ftell(fp)= %ld\n",ftell(fp));
        if(nwrite == EOF)
            break;
    }
    
    fclose(fp);

    exit(0);
}