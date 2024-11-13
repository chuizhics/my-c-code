#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sqstack.h"


sqstack *st_create()
{
    sqstack *stack = malloc(sizeof(sqstack));
    if(stack == NULL)
    {
        return NULL;
    }
    stack->top = -1;
    //printf("[%s:%d]--stack = %p, stack->top = %d \n", __FUNCTION__,__LINE__,stack, stack->top);
    return stack;
}

int st_isempty(sqstack * stack)
{   
    return (stack->top == -1);
}

int st_push(sqstack * stack,datatype * data)
{
    
    if(stack == NULL)
    {
        return -1;   // NULL
    }
    if(stack->top == STACKSIZE - 1)
    {
        return -2;   // Stack overflow
    }
     memcpy(&stack->data[++stack->top],data,sizeof(datatype));
    //printf("[%s:%d]--stack = %p, stack->top = %d stack->data[stack->top] = %d\n", __FUNCTION__,__LINE__,stack, stack->top,stack->data[stack->top]);
    return 0;

}

int st_pop(sqstack * stack , datatype *data)
{
    if(stack == NULL)
    {
        return -1;   // NULL
    }
    if(st_isempty(stack))
    {
        return -2; //empty
    }
    //*data = stack->data[stack->top--];
    //printf("[%s:%d]--stack = %p, stack->top = %d, *data = %d\n", __FUNCTION__,__LINE__,stack, stack->top, *data);
    memcpy(data,&stack->data[stack->top--],sizeof(datatype));

    
    return 0;
}

int st_top(sqstack * stack, datatype * data)
{
    if(stack == NULL)
    {
        return -1; // NULL
    }

    if(st_isempty(stack))
    {
        return -2; // empty
    }
    memcpy(data,&stack->data[stack->top],sizeof(datatype));
    return 0;
}

void st_travel(sqstack * stack, sqk *op)
{
    int i= 0;
    for(i = stack->top; i >= 0; i--)
    {
        op(&stack->data[i]);
    }
}

void st_destroy(sqstack *stack)
{
    free(stack);
}

