#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#define BUFSIZE 1024

static long getsize(const char *p)
{
    return sizeof(*p);
}

static void memcpychar(char *dest,const unsigned int _n, const char *src)
{
    memcpy(dest,src,_n);
}

int main()
{
    char *ptr = NULL;
    char buf[BUFSIZE] = {};

    printf("getsize(ptr) = %ld\n",getsize(ptr));
    printf("getsize(buf) = %ld\n",getsize(buf));

    memcpychar(ptr,5,"ABCDEFG");  // Segmentation fault 
    puts(ptr);

    memcpychar(buf,5,"ABCDEFG");
    puts(buf);


    exit(0);
}