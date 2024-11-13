#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STRSIZE 4


int main()
{   
    char str[STRSIZE];

    strncpy(str,"helloworld!",STRSIZE - 1);

    puts(str);
    printf("strlen(str) = %ld\n",strlen(str)); 
    printf("sizeof(str) = %ld\n",sizeof(str));

    int len = strlen(str);
    strcat(str," ");
    strncat(str,"helloworld!",STRSIZE-len-1);

    puts(str);

    //printf("str[3] = %c\tstr[4] = %c\n",str[3],str[4]);



    // char str[] = "hello";
    // printf("%ld\n",strlen(str)); // 5 以'\0'结束
    // printf("%ld\n",sizeof(str)); // 6
#if 0
    int i;
    char str[5] = "Hello";
    // for ( i = 0; i < 5; i++)
    // {
    //     printf("%c ",str[i]);
    // }
    char str2[4] = {'a',str[5],'d'};
    puts(str);
    puts(str2);
    printf("%c\n",str[4]);
#endif

    exit(0);
}