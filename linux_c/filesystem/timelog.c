#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>

#define FNAME "/tmp/time.log"

#define BUFSIZE 1024

int main()
{
    FILE *fp = NULL;
    char buf[BUFSIZE] = {};
    __uint32_t count = 0;
    time_t stamp = 0;
    struct tm *tmp = NULL;

    fp = fopen(FNAME, "a+");

    if (fp == NULL)
    {
        perror("fopen()");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);

    while (fgets(buf, BUFSIZE, fp) != NULL)
    {
        count++;
    }

    while (1)
    {
        time(&stamp);
        tmp = localtime(&stamp);

        fprintf(fp,"%-6d %d-%d-%d %d:%d:%d\n",++count,\
        tmp->tm_year + 1900,tmp->tm_mon + 1,tmp->tm_mday,tmp->tm_hour,tmp->tm_min,tmp->tm_sec);

        fflush(fp);  // necessary
    
        sleep(1);
    }

    fclose(fp);

    exit(0);
}