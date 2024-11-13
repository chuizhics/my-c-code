#include <stdio.h>
#include <stdlib.h>

#include "list.h"

static void printdata(datatype *data)
{
    if (data != NULL)
    {
        printf("%d %s %d %d\n", data->id, data->name, data->math, data->chinese);
    }
    else
        printf("invalid!\n");
    printf("\n");
}

int main()
{
    int i;
    stlist *list = NULL;
    datatype tmp;
    datatype *pdata = malloc(sizeof(datatype));
    for (i = 0; i < 7; i++)
    {
        tmp.id = i;
        snprintf(tmp.name, NAMESIZE, "stu%d", i);
        tmp.math = rand() % 100;
        tmp.chinese = rand() % 100;
        // list_insert_head(&list, &tmp);
        list_insert_tail(&list, &tmp);
    }

    list_display(list);

    list_findbyid(list,4,pdata);

    printdata(pdata);

    free(pdata);

    list_delete_byid(&list, 4);

    list_display(list);    


    list_destroy(list);
    exit(0);
}