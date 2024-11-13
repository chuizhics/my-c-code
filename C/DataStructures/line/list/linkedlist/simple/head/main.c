#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main()
{
    int i, tmp;
    int arr[] = {7, 8, 24, 35, 12, 57, 6, 8, 11};
    linkedlisth *listh = NULL;
    create(&listh);
    for (i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        append(listh, &arr[i]);
    }
    display(listh);

    deletei(listh, 5);
    deletei(listh, 5);
    display(listh);

    tmp = 33;
    changei(listh, 4, &tmp);
    display(listh);

    printf("%d\n", findi(listh, 3));

    setempty(listh);
    display(listh);

    for (i = 0; i < sizeof(arr) / sizeof(*arr); i++)
    {
        tmp = arr[i] / 2;
        // append(listh, &tmp);
        insert_order(listh,&tmp);
    }
    display(listh);

    destroy(listh);
    exit(0);
}