#include <stdio.h>
#include <stdlib.h>

#define M 2
#define N 3

// static int* trans_arr(int *arr, int rows, int cols)
// {
//     int ret[cols][rows],i,j;
//     for ( i = 0; i < cols; i++)
//     {
//         for(j = 0; j < rows; j++)
//         {
//             ret[cols][rows] = *(arr + i * cols + j);
//         }
//     }
//     return ret;
// }

// static void printarr(int (*p)[N]);

static void printarr(int a[M][N])
{
    int i, j;
    printf("sizeof(a) = %ld\n", sizeof(a));
    printf("a=%p, *a = %p, **a = %d\n", a, *a, **a);
    printf("a + 1=%p, *(a + 1) = %p, *(*(a + 1)) = %d\n", a + 1, *(a + 1), *(*(a + 1)));
    printf("a + 2=%p, *(a + 2) = %p, *(*(a + 2)) = %d\n", a + 2, *(a + 2), *(*(a + 2)));
    printf("a + 3=%p, *(a + 3) = %p, *(*(a + 3)) = %d\n", a + 3, *(a + 3), *(*(a + 3)));
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            // printf("a+%d*%d+%d=%p-->%d\t", i,cols,j,a + i * cols + j,*(a + i * cols + j));
            printf("%p-->%d\t", (*(a + i) + j), *(*(a + i) + j));
            // printf("%d\t", a[i][j]);
        }
        printf("\n");
    }
    printf("sizeof(a) = %ld\n", sizeof(a));
    return;
}
int main()
{
    int arr[M][N], i, j;
    printf("please enter for arr:\n");
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            scanf("%d", &arr[i][j]);
        }
    }
    printf("sizeof(arr) = %ld\n", sizeof(arr));
    printf("arr=%p,*arr=%p,**arr=%d\n", arr, *arr, **arr);
    printf("arr + 1 = %p, *(arr + 1) = %p, *(*arr + 1) = %d\n", (arr + 1), *(arr + 1), *(*arr + 1));
    printf("\n");
    printarr(arr);

    // printf("arr = %p\tarr+1 = %p\n", arr, arr+1);
    // for ( i = 0; i < M; i++)
    // {
    //     for ( j = 0; j < N; j++)
    //     {
    //         printf("%p-->%d\n", &arr[i][j], arr[i][j]);
    //     }
    //     printf("\n");
    // }

    // printarr(trans_arr(arr, M, N), N, M);

    exit(0);
}