#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i = 0;
    printf("Before while()");

    fflush(stdout);

    while (1)
    {
        /* code */
    }
    
    printf("After while()");
    fflush(NULL);

    exit(0);
}