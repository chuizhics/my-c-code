#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>


#define BUFSIZE 32


int main()
{
    // char *ptr = "abc";
    // //ptr[0] = 'x';

    // printf("%s\n", ptr);

    FILE *fpcreate = NULL;
    FILE *fpcpy = NULL;
    char buf[BUFSIZE];

    fpcreate = fopen("test.txt","w");

    if(fpcreate == NULL)
    {
        fprintf(stderr,"fopne() failed!:%s\n",strerror(errno));
        exit(1);
    }

    fputs("abcdefg\n", fpcreate);
    fputs("cdefg\n", fpcreate);
    fputs("kk\n",fpcreate);

    fclose(fpcreate);

    fpcreate = fopen("test.txt","r");

    
    if(fpcreate == NULL)
    {
        fprintf(stderr,"fopne() failed!:%s\n",strerror(errno));
        exit(1);
    }
    fpcpy = fopen("test_cpy.txt","w");
    
    if(fpcpy == NULL)
    {
        fprintf(stderr,"fopne() failed!:%s\n",strerror(errno));
        exit(1);
    }

    printf("-------------\n");
    while ((fgets(buf,BUFSIZE,fpcreate) != NULL))
    {
        fprintf(stdout,"buf[]: %s", buf);
        fputs(buf,fpcpy);
    }

    
    fclose(fpcpy);

    fclose(fpcreate);

    printf("--------------\n");
    return 0;
}