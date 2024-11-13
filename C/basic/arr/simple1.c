#include <stdio.h>
#include <stdlib.h>

#define M 3


int main()
{
    //int arr[M];
    int arr[M] = {1,2,3};
    int i;
    printf("sizeof(arr) = %ld\n",sizeof(arr));
    printf("arr = %p \n", arr);
    for (i = 0; i < M; i++)
    {
        printf("%p --> %d\n", &arr[i], arr[i]);
    }
    exit(0);
}