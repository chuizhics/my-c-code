#include <stdio.h>
#include <stdlib.h>

void reverse(int *arr, int size)
{
    int i = 0, j = size - 1;
    int temp;
    while (i < j)
    {
        temp = *(arr + i);
        *(arr + i) = * (arr + j);
        *(arr + j) = temp;
        i++;
        j--;
    }
}

void print_arr(int *p, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        printf("%d ", *(p+i));
    }
    printf("\n");
}



int main()
{
    int a[] = {1, 3, 5, 7, 9};
    //int *p = a;
    print_arr(a, sizeof(a)/sizeof(*a));

    reverse(a, sizeof(a) / sizeof(*a));

    print_arr(a, sizeof(a)/sizeof(*a));

    return 0;
}