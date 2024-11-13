#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char*argv[])
{
    int ch;
    FILE *fptr = NULL;

    fptr = fopen("test.txt", "r");

    if(fptr == NULL)
    {
        //fprintf(stderr, "fopen() failed! errno = %d\n", errno);
        //perror("fopen failed!");

        fprintf(stderr, "fopen() failed!:%s\n",strerror(errno));
    


        exit(1);
    }
    
    while((ch = fgetc(fptr)) != EOF)
    {
        printf("%c",ch);
    }


    //puts("OK");

    fclose(fptr);
    exit(0);
}