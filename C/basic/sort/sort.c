#include <stdio.h>
#include <stdlib.h>

#define N 7

static void print(int* arr, int arrsize)
{   
    int i;
    for ( i = 0; i < arrsize; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return;
}

static void bubble()
{
    int arr[N], i, j, t;
    printf("Please enter %d numbers:\n",  N);
    for ( i = 0; i < N; i++)
    {
        if(scanf("%d", &arr[i]) != 1)
        {
            printf("[%s:%d]input error!\n", __FUNCTION__,__LINE__);
        }
    }

    for(i = 0; i < N-1; i++)
    {
        for(j = 1; j < N - i; j++)
        {
            if(arr[j] < arr[j - 1])
            {
                t = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = t;
            }
        }
    }
    printf("bubble sort:\n");
    print(arr, N);

    return;
}

static void compare()
{
    int arr[N],i,j,min,t;

    printf("Please enter %d numbers:\n",  N);
    for ( i = 0; i < N; i++)
    {
        if(scanf("%d", &arr[i]) != 1)
        {
            printf("[%s:%d]input error!\n", __FUNCTION__,__LINE__);
        }
    }

    for(i = 0; i < N - 1 ; i++)
    {
        min = arr[i];
        for(j = i + 1; j < N; j++)
        {
            if(arr[j] < min)
            {
                min = arr[j];
                t = j;
            }
        }
        arr[t] = arr[i];
        arr[i] = min; 
    }

    printf("compare sort:\n");
    print(arr, N);
    return;
}

static void swap()
{
    int arr[N], i, j, tmp;
    printf("Please enter %d numbers:\n",  N);
    for ( i = 0; i < N; i++)
    {
        if(scanf("%d", &arr[i]) != 1)
        {
            printf("[%s:%d]input error!\n", __FUNCTION__,__LINE__);
        }
    }

    for(i = 0; i < N-1;i++)
    {
        for(j = 0; j < N-i; j++)
        {
            if(arr[j+1] < arr[j])
            {
                tmp = arr[j+1];
                arr[j + 1] = arr[j];
                arr[j] = tmp;
            }
        }
    }
    
    printf("swap sort:\n");
    print(arr, N);

    return;

}

int main()
{
    
   // bubble();
    //compare();
    swap();
    exit(0);
}