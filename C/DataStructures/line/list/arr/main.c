#include <stdio.h>
#include <stdlib.h>
#include "sqlist.h"

int main()
{

    int i, err, k;
    sqlist *list = NULL;
    sqlist *list_1 = NULL;
    datatype arr[] = {12, 24, 15, 78, 34, 25};
    datatype arr_1[] = {12, 24, 15, 78, 34, 25};
    list = sqlist_create();
    // sqlist_create1(&list);

    if (list == NULL)
    {
        fprintf(stderr, "sqlist_create() failed!\n");
        exit(1);
    }
    // sqlist_append();
    // printf("[%s:%d]: -----Test-------\n",__FUNCTION__,__LINE__);
    for (i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        // // if((err = sqlist_insert(list,i, &arr[i])) != 0)
        // if ((err = sqlist_insert(list, 0, &arr[i])) != 0)
        // {
        //     if (err == -1)
        //         fprintf(stderr, "The arr is full.\n");
        //     else if (err == -2)
        //         fprintf(stderr, "The pos you want to insert is wrong.\n");
        //     else
        //         fprintf(stderr, "Error!\n");
        //     exit(1);
        // }

        if(sqlist_append(list,&arr[i]) != 0)
        {
            fprintf(stderr, "The arr is full.\n");
            exit(1);
        }
    }

    sqlist_display(list);

    k = 22;
    sqlist_insert(list,2, &k);

    sqlist_display(list);

    sqlist_delete(list, 1);

    sqlist_display(list);

    printf("%d is at %dth positon.\n",k, sqlist_find(list, &k));

    sqlist_display(list);

    sqlist_create1(&list_1);

    if(list_1 == NULL)
    {
        fprintf(stderr, "sqlist_create() failed!\n");
        exit(1);
    }
    // sqlist_append();
    // printf("[%s:%d]: -----Test-------\n",__FUNCTION__,__LINE__);
    for (i = 0; i < sizeof(arr_1) / sizeof(*arr_1); i++)
    {
        // if((err = sqlist_insert(list,i, &arr[i])) != 0)
        if ((err = sqlist_insert(list_1, 0, &arr_1[i])) != 0)
        {
            if (err == -1)
                fprintf(stderr, "The arr is full.\n");
            else if (err == -2)
                fprintf(stderr, "The pos you want to insert is wrong.\n");
            else
                fprintf(stderr, "Error!\n");
            exit(1);
        }

        // if(sqlist_append(list,&arr[i]) != 0)
        // {
        //     fprintf(stderr, "The arr is full.\n");
        //     exit(1);
        // }
    }
    //printf("[%s:%d]----\n",__FUNCTION__,__LINE__);
    sqlist_display(list_1);

    sqlist_union(list,list_1);


    //printf("[%s:%d]----\n",__FUNCTION__,__LINE__);
    sqlist_display(list);

    sqlist_destroy(list);
    sqlist_destroy(list_1);
    exit(0);
}
