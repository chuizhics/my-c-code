#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

#define NAMESIZE 32
 
typedef struct score_st
{
    int id;
    char name[NAMESIZE];
    int math;
    int chinese;
    
} score_st;

void print_score(const void *p)
{
    const score_st *ptr = p;
    printf("%d %s %d %d\n", ptr->id, ptr->name, ptr->math, ptr->chinese);
}

int id_compare(const void * key, const void *record)
{
    score_st *r = record;
    int *id = key;
    return (*id - r->id);
}

int main()
{

    LLIST *handler = NULL;
    int i, id;
    score_st tmp, *fid;
    handler = llist_create(sizeof(score_st));
    
    for (i = 0; i < 7; i++)
    {
        tmp.id = i;
        snprintf(tmp.name, NAMESIZE, "stu%d", i);
        tmp.math = rand() % 100;
        tmp.chinese = rand() % 100;
        //llist_insert(handler, &tmp, LLIST_FORWARD);
        llist_insert(handler, &tmp, LLIST_BACKWARD);
    }

    llist_travel(handler, print_score);
    printf("total: %d\n", llist_getsize(handler));
    id = 8;
    fid = llist_find(handler, &id, id_compare);

    if(fid != NULL)
    {
        printf("\n");
        print_score(fid);
    }
    else
    {
        printf("\ncan not find!\n");
    }

    id = 5;
    if(llist_fetch(handler, &id, id_compare, &tmp) == 0)
    {
        printf("\n");
        print_score(&tmp);
        printf("\n");
    }
    else
    {
        printf("\ncan not find!\n");
    }

    llist_travel(handler,print_score);
    printf("total: %d\n", llist_getsize(handler));
    
    llist_destroy(handler);
    exit(0);
}