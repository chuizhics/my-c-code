#include <stdio.h>
#include <stdlib.h>

int factorial1(int n)
{
    if (n < 0)
        return -1;
    if (n == 0)
        return 1;
    return n * factorial1(n - 1);
}

int factorial2(int n, int acc)
{
    if (n < 0)
        return -1;
    if (n == 0)
        return acc;
    return factorial2(n - 1, n * acc);

}


int main()
{
    const int i = 14;

    printf("factorial1(%d) = %d\n",i, factorial1(i));

    printf("factorial2(%d) = %d\n",i, factorial2(i,1));

    return 0;
}