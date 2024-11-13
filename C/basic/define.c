#include <stdio.h>
#include <stdlib.h>

#define PI 3.1416
#define ADD (2+3)
#define MAX(a,b) ({typeof(a) A = a, B = b; ((A) > (B) ? (A) : (B));})

int max(int a, int b)
{
    return a > b ? a : b;
}

int main()
{
    int i = 5, j = 3;

    printf("i= %d\tj=%d\n",i,j);

    printf("%d\n", MAX(i++,j++));

    printf("i= %d\tj=%d\n",i,j);

    printf("%d\n", max(i++,j++));

    printf("i= %d\tj=%d\n",i,j);

    exit(0);
}
