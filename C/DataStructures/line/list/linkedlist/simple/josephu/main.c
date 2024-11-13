#include <stdio.h>
#include <stdlib.h>

#define N 7

typedef struct jose_st
{
    int id;
    struct jose_st *next;
} jose_st;

int create(jose_st **ptr, int n)
{
    jose_st *newnode = NULL;
    jose_st *curtail = NULL;
    int i = 1;
    if(n <= 0)
        return -1;
    else
    {
        newnode = malloc(sizeof(jose_st));
        newnode->id = i;
        newnode->next = newnode;
        *ptr = newnode;
        curtail = newnode;
        for(; i < n;)
        {
            newnode = malloc(sizeof(jose_st));
            newnode->id = ++i;
            newnode->next = *ptr;
            curtail->next = newnode;
            curtail = newnode;
        }
    }
    return 0;
}

int create1(jose_st **ptr, int n)
{
    jose_st *newnode = NULL;
    jose_st *curhead = NULL;
    jose_st *tail = NULL;
    if (n <= 0)
        return -1;
    while (n--)
    {
        newnode = malloc(sizeof(jose_st));
        if (newnode == NULL)
            return -2;
        newnode->id = n + 1;
        newnode->next = curhead;
        if (curhead == NULL)
            tail = newnode;
        curhead = newnode;
    }
    tail->next = curhead;

    *ptr = curhead;

    return 0;
}

void show(jose_st *ptr)
{
    jose_st *curnode = NULL;
    if (ptr != NULL)
    {
        curnode = ptr;
        if (curnode->next == curnode)
        {
            printf("only one member left: %d\n", curnode->id);
        }
        else
        {
            while (curnode->next != ptr)
            {
                printf("%d ", curnode->id);
                curnode = curnode->next;
            }
            printf("%d ", curnode->id);
            printf("\n");
        }
    }
}

int kill(jose_st **ptr, int k)
{
    int i;
    jose_st *curnode = NULL;
    jose_st *tmp = NULL;
    if (k <= 0 || *ptr == NULL)
    {
        return -1;
    }
    else if (k == 1)
    {
        for (curnode = *ptr; curnode->next != *ptr; curnode = tmp)
        {
            tmp = curnode->next;
            printf("%d ", curnode->id);
            free(curnode);
        }
        curnode->next = curnode;
    }
    else
    {
        for (curnode = *ptr; curnode->next != curnode; curnode = curnode->next)
        {
            for (i = 1; i < k - 1; i++)
            {
                curnode = curnode->next;
            }
            tmp = curnode->next;
            curnode->next = tmp->next;
            printf("%d ", tmp->id);
            free(tmp);
        }
    }
    printf("\n");
    *ptr = curnode;
    return 0;
}

void destroy(jose_st *ptr)
{
    int i;
    jose_st *cur = NULL, *tmp = NULL;
    if (ptr != NULL)
    {
        if (ptr->next == ptr)
        {
            free(ptr);
        }
        else
        {
            for (cur = ptr; cur->next != ptr; cur = tmp)
            {
                tmp = cur->next;
                free(cur);
            }
            free(cur);
        }
    }
}

int main()
{
    jose_st *pjose = NULL;

    //create1(&pjose, N);

    create(&pjose, N);

    show(pjose);

    kill(&pjose, 7);

    show(pjose);

    destroy(pjose);
    
    
    exit(0);
}