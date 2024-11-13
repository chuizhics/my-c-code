#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFSIZE 5



int main()
{
    int i = 0;
    char buf[BUFSIZE];
    FILE *fp = NULL;

    for(i = 0; i < BUFSIZE; i++)
    {
        buf[i] = 'a' + i;
    }

    fp = fopen("/tmp/test_char_buffer.out","w");
    
    fputs(buf,fp);

    buf[BUFSIZE] = 'k';
    buf[BUFSIZE + 1] = 'G';

    fputs(buf,fp);  //遇到‘\0’停止，所以buf需要添加‘\0’ 防止意外情况发生

    fclose(fp);

    exit(0);
}