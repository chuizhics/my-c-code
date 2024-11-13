#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{   
    int i;

    printf("argc = %d\n", argc);
 //   for ( i = 0; i < argc; i++)
    for(i = 0; argv[i] != NULL; i++)
    {
        printf("%s\t", argv[i]);
    }
    
    printf("\n");

    //printf("Hello!\n");
    return 0;
}