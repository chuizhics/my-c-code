#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void)
{
    float f = 3.9;
    int i = f;

    bool bf = false;
    bool bt = true;
   
    printf("sizeof(short) = %ld\n", sizeof(short));

    printf("sizeof(long) = %ld\n", sizeof(long));
    long a = 2147483647;
    long b = 1;

    printf("2147483647 +1 = %ld\n", a + b);

    printf("sizeof(int) = %ld\n", sizeof(int));

    //printf("2147483647 +1 = %ld\n", 2147483647 +1);

    printf("sizeof(char) = %ld\n", sizeof(char));

    printf("sizeof(double) = %ld\n", sizeof(double));

    printf("sizeof(long long) = %ld\n", sizeof(long long));

    long long c = 2147483647;
    long long d = 1;
    printf("long long c (2147483647)  + long long d (1)  =>  %lld\n",  c + d);

    printf("i = %d\n",  i);

    printf("bl = %d\n", bf);

    printf("bt = %d\n", bt);
	
    exit(0);
}
