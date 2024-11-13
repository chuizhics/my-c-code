#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * success: 0
 * 
 * error: -1 dest == NULL || src == NULL
 *        -2 capacity is limit
 * 
*/
int mystrncpy(char *dest,size_t capacity,const char *src, size_t n)
{
    size_t  src_len = 0;
    if(dest == NULL || src == NULL)
    {
        return -1;
    }
    src_len = strlen(src);
    n = src_len > n ? n : src_len;
    if(n + 1 > capacity)
    {
        fprintf(stderr,"capacity is limit!\n");
        return -2;
    }
    else
    {   
        strncpy(dest,src,n);
        dest[n] = '\0';
    }
    return 0;
}


/**
 * success: 0
 * 
 * error: -1 dest == NULL || src == NULL
 *        -2 capacity is limit
 * 
*/
int mystrcpy(char *dest,size_t capacity,const char *src)
{
    size_t  src_len = 0;

    if(dest == NULL || src == NULL)
    {
        return -1;
    }
    src_len = strlen(src);
    if(src_len + 1 > capacity)
    {
        fprintf(stderr,"capacity is limit!\n");
        return -2;
    }
    else
    {   
        strcpy(dest,src);
    }
    return 0;
}

/**
 * success: 0
 * 
 * error: -1 dest == NULL || src == NULL
 *        -2 capacity is limit
 * 
*/

int mystrncat(char *dest,size_t capacity,const char *src, size_t n)
{
    size_t  dest_len = 0;
    size_t  src_len = 0;

    if(dest == NULL || src == NULL)
    {
        return -1;
    }
    dest_len = strlen(dest);
    src_len = strlen(src);
    n = src_len > n ? n : src_len;
    if(dest_len + n + 1 > capacity)
    {
        fprintf(stderr,"capacity is limit!\n");
        return -2;
    }
    else
    {   
        strncat(dest,src,n);
        dest[dest_len + n] = '\0';
    }
    return 0;
}

/**
 * success: 0
 * 
 * error: -1 dest == NULL || src == NULL
 *        -2 capacity is limit
 * 
*/
int mystrcat(char *dest,size_t capacity,const char *src)
{
    size_t  dest_len = 0;
    size_t  src_len = 0;

    if(dest == NULL || src == NULL)
    {
        return -1;
    }
    dest_len = strlen(dest);
    src_len = strlen(src);
    if(dest_len + src_len + 1 > capacity)
    {
        fprintf(stderr,"capacity is limit!\n");
        return -2;
    }
    else
    {   
        strcat(dest,src);
    }
    return 0;
}