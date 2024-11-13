#include <stdio.h>
#include <stdlib.h>

#include "sqstack.h"

void printdata(datatype *data)
{
    printf("%d ", *data);
}

int main(int argv,char *args[])
{
    int i = 0;
    datatype tmp;
    sqstack *stack = NULL; 
    stack = st_create();
    if (stack == NULL)
    {
        exit(1);
    }
    

    for(i = 0; i < 7; i++)
    {
        tmp = i*3;
        if(st_push(stack, &tmp) != 0)
        {
            exit(1);
        }
    }
    //printf("[%s:%d]--stack = %p, stack->top = %d stack->data[stack->top] = %d\n", __FUNCTION__,__LINE__,stack, stack->top,stack->data[stack->top]);

    st_travel(stack, printdata);
    printf("\n");

    if(st_top(stack, &tmp) == 0)
    {
        printf("current top data:  ");
        printdata(&tmp);
        printf("\n");
    }
    else
    {
        printf("stack is empty!\n");
    }

    while (st_pop(stack, &tmp) == 0)
    {
        printf("current top data:  ");
        printdata(&tmp);
        printf("\n");
        st_travel(stack, printdata);
        printf("\n");
    }
    
    
    if(st_top(stack, &tmp) == 0)
    {
        printf("current top data:  ");
        printdata(&tmp);
        printf("\n");
    }
    else
    {
        printf("stack is empty!\n");
    }


    st_destroy(stack);
    exit(0);
}