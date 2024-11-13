#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *ptr = NULL;

    printf("sizeof(char) = %ld\n", sizeof(char));

    printf("sizeof(int) = %ld\n", sizeof(int));

    printf("sizeof(long) = %ld\n", sizeof(long));

    printf("sizeof(long long) = %ld\n", sizeof(long long));

    printf("sizeof(float) = %ld\n", sizeof(float));

    printf("sizeof(double) = %ld\n", sizeof(double));

    printf("sizeof(ptr) = %ld\n", sizeof(ptr));

    printf("__INT_MAX__ = %ld\n",__INT_MAX__);

    printf("__INT8_MAX__ = %ld\n",__INT8_MAX__);

    printf("__LONG_MAX__ = %ld\n",__LONG_MAX__);

    printf("sizeof(char) = %ld\n", sizeof(char));

    
    exit(0);
}