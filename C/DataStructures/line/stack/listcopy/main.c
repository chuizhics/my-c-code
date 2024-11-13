#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define NAMESIZE 32

typedef struct score_st
{
    int id;
    char name[NAMESIZE];
    int math;
    int chinese;

}score_st;

static void print_data(score_st * record)
{
    printf("%d %s %d %d\n",record->id,record->name,record->math,record->chinese);
}

int main(int argv, char* args[])
{
    int i,ret;
    score_st tmp;
    STACK *stack = stack_create(sizeof(score_st)); 
    if(stack == NULL)
        exit(1);

    STACK *stack2 = stack_create(sizeof(score_st)); 
    
    if(stack2 == NULL)
        exit(1);

    for (i = 0; i < 7; i++)
    {
        tmp.id = i;
        snprintf(tmp.name,NAMESIZE,"stu%d",i);
        tmp.math = rand()%100;
        tmp.chinese = rand()%100;
        if(stack_push(stack,&tmp)!=0)
        {
            exit(1);
        }
    }
    printf("stack1.size: %d\n", stack_getsize(stack));

    
    for (i = 0; i < 4; i++)
    {
        tmp.id = i*5;
        snprintf(tmp.name,NAMESIZE,"stu%d",i*5);
        tmp.math = rand()%100;
        tmp.chinese = rand()%100;
        if(stack_push(stack2,&tmp)!=0)
        {
            exit(1);
        }
    }
    printf("stack2.size: %d\n", stack_getsize(stack2));

    while (1)
    {
        ret = stack_pop(stack, &tmp);
        if(ret != 0)
            break;
        print_data(&tmp);
    }
    printf("\n");
    while (1)
    {
        ret = stack_pop(stack2, &tmp);
        if(ret != 0)
            break;
        print_data(&tmp);
    }
    


    stack_destroy(stack);
    stack_destroy(stack2);

    exit(0);
}