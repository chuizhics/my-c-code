#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int ch,count = 0;
    FILE *fptr = NULL;

    while (1)
    {
        fptr = fopen("test.txt", "r");

        if (fptr == NULL)
        {
            // fprintf(stderr, "fopen() failed! errno = %d\n", errno);
             perror("fopen failed!");

            //fprintf(stderr, "fopen() failed!:%s\n", strerror(errno));
            break;
        }
        count++;
    }

    printf("count = %d\n",count);

    // fclose(fptr);
    exit(0);
}