#ifndef STACK_H__
#define STACK_H__

#include "linkedlist.h"

typedef LLIST STACK;

STACK *stack_create(int );

int stack_push(STACK *,const void *data);

int stack_pop(STACK*, void *data);

int stack_top(STACK *, void * data);

int stack_isempty(STACK *);

int stack_getsize(STACK *);

void stack_destroy(STACK *);



#endif