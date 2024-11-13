#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define NAMESIZE 32

typedef struct score_st
{
    int id;
    char name[NAMESIZE];
    int math;
    int chinese;
} score_st;

static void print_score(const void *p)
{
    const score_st *ptr = p;
    printf("%d %s %d %d\n", ptr->id, ptr->name, ptr->math, ptr->chinese);
}



int main(int argc, char *argv[])
{
    int i = 0;
    score_st tmp;
    QUEUE *queue = queue_create(sizeof(score_st));
    if(queue == NULL)
        exit(1);
    for (i = 0; i < 7; i++)
    {
        tmp.id = i;
        snprintf(tmp.name, NAMESIZE, "stu%d", i);
        tmp.math = rand() % 100;
        tmp.chinese = rand() % 100;
        queue_enqueue(queue, &tmp);
    }

    printf("queue's length is: %d\n", queue_getlength(queue));

    while (queue_dequeue(queue,&tmp) == 0)
    {
        print_score(&tmp);
    }


    queue_destroy(queue);
    exit(0);
}
