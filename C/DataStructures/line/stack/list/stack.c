#include <stdio.h>

#include "stack.h"

#include "linkedlist.h"

static int always_match(const void *datap, const void *dataq);


STACK *stack_create(int initsize)
{
    return llist_create(initsize);
}

int stack_push(STACK *stack, const void *data)
{
    int ret = llist_insert(stack, data, LLIST_FORWARD);
    // if (ret == 0)
    //     STACKSIZE++;
    return ret;
}

static int always_match(const void *datap, const void *dataq)
{
    return 0;
}

int stack_pop(STACK *stack, void *data)
{
    int ret = llist_fetch(stack, (void *)0, always_match, data);
    // if (ret == 0)
    //     STACKSIZE--;
    return ret;
}

int stack_top(STACK *stack, void *data)
{
    data = llist_find(stack, (void *)0, always_match);

    return 0;
}

int stack_isempty(STACK *stack)
{
    return llist_getlength(stack) == 0;
}

int stack_getsize(STACK *stack)
{
    return llist_getlength(stack);
}

void stack_destroy(STACK *stack)
{
    llist_destroy(stack);
}