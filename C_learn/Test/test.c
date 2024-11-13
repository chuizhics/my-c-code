#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int swap(int a, int *b)
{
    b = &a;

    printf("a = %d, b = %p, *b = %d\n", a, b,*b);
    return 0;
}


int main(void)
{
    float f = 3.9;
    int i = f;

    bool bf = false;
    bool bt = true;
    printf("sizeof(short) = %d\n", sizeof(short));

    printf("sizeof(long) = %d\n", sizeof(long));
    long a = 2147483647;
    long b = 1;

    printf("2147483647 +1 = %ld\n", a + b);

    printf("sizeof(int) = %d\n", sizeof(int));

    printf("2147483647 +1 = %d\n", 2147483647 +1);

    printf("sizeof(char) = %d\n", sizeof(char));

    printf("sizeof(long long) = %d\n", sizeof(long long));

    long long c = 2147483647;
    long long d = 1;
    printf("long long c (2147483647)  + long long d (1)  =>  %lld\n",  c + d);

    printf("i = %d\n",  i);

    printf("bl = %d\n", bf);

    printf("bt = %d\n", bt);



    a = 4;
    b = 5;
    int *x = &b; 
    printf("a = %d, b = %d, x = %p, *x = %d\n",a,b,x,*x);
    swap(a,x);

    printf("a = %d, b = %d, x = %p, *x = %d\n",a,b,x,*x);
    return 0;
}