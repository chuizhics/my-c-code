#include <stdio.h>
#include <stdlib.h>

int** transpose(int** arr, int rows, int cols) 
{   
    int** new_arr = (int**)malloc(cols * sizeof(int*));
    for (int i = 0; i < cols; i++) {
        new_arr[i] = (int*)malloc(rows * sizeof(int));
        for (int j = 0; j < rows; j++) {
            printf("[%s:%d]: %p-->%p",__FUNCTION__,__LINE__, new_arr, arr);
            *(new_arr[i]+ j) = *(arr+j * cols+i);
            printf("[%s:%d]: %p-->%d",__FUNCTION__,__LINE__, new_arr, arr[i][j]); 
        }
    }
    return new_arr;
}

int main()
{
    int arr[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    int rows = 3;
    int cols = 2;
    printf("[%s:%d]: -------------", __FUNCTION__,__LINE__);
    

    printf("Original array:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    int** new_arr = transpose((int**)arr, rows, cols);

    printf("\nTransposed array:\n");

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            printf("%d ", new_arr[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < cols; i++) {
        free(new_arr[i]);
    }
    free(new_arr);

    return 0;
}
