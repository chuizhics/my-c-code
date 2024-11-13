#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int create(linkedlisth **llh)
{
    *llh = malloc(sizeof(**llh));
    if ((*llh) == NULL)
        return -1;
    (*llh)->data = 0;
    (*llh)->next = NULL;
    return 0;
}

linkedlisth *create1()
{
    linkedlisth *p = malloc(sizeof(linkedlisth));
    if (p == NULL)
        return NULL;
    p->data = 0;
    p->next = NULL;
    return p;
}

int isempty(const linkedlisth *llh)
{
    if (NULL == llh)
        return -1;
    if (llh->data == 0 && llh->next == NULL)
        return 0;
    else
        return 1;
}

int setempty(linkedlisth *llh)
{
    if (llh == NULL)
        return -1;
    while (llh->next != NULL)
    {
        deletei(llh, 0);
    }
    return 0;
}

int insert(linkedlisth *llh, const int i, const datatype *data)
{
    int j;
    llhnode *pp = llh;
    llhnode *pi = malloc(sizeof(llhnode));
    if (llh == NULL)
        return -1;
    if (i < 0 || i > llh->data)
        return -2;
    for (j = 0; j < i; j++)
    {
        pp = pp->next;
    }
    pi->next = pp->next;
    pp->next = pi;
    pi->data = *data;

    llh->data++;

    return 0;
}

int insert_order(linkedlisth *llh, const datatype *data)
{
    
    llhnode *pp = llh;
    llhnode *pi = malloc(sizeof(llhnode));
    if(llh == NULL)
        return -1;
    while (pp->next != NULL)
    {
        if(pp->next->data < *data)
        {
            pp = pp->next;
        }
        else
        {
            break;
        }
    }
    pi->next = pp->next;
    pi->data = *data;
    pp->next = pi;

    llh->data++;
    return 0;
}

int append(linkedlisth *llh, const datatype *data)
{
    if (llh == NULL)
        return -1;
    return insert(llh, llh->data, data);
}

int deletei(linkedlisth *llh, const int i)
{
    int j, ret;
    llhnode *pp = llh;
    llhnode  *pi = NULL;
    if (llh == NULL)
        return -1;
    if (i < 0 || i >= llh->data)
        return -2;

    for (j = 0; j < i; j++)
    {
        pp = pp->next;
    }
    pi = pp->next;
    pp->next = pi->next;

    ret = pi->data;
    free(pi);

    llh->data--;

    return ret;
}

int deleted(linkedlisth *llh, const datatype *data)
{
    int i, j;
    llhnode  *pp = llh;
    llhnode  *pi = NULL;
    if (isempty(llh) != 1)
        return -1;
    while (pp->next != NULL)
    {
        if ((pp->next)->data == *data)
        {
            pi = pp->next;
            pp->next = pi->next;
            free(pi);
            llh->data--;
        }
        else
        {
            pp = pp->next;
        }
    }
    return 0;
}

llhnode* findip(const linkedlisth *llh, const int i)
{
    int j;
    llhnode *pp = llh;
    if (llh != NULL && (i >= 0 && i < llh->data))
    {
        for (j = 0; j < i; j++)
        {
            pp = pp->next;
        }
        return pp;
    }
    return NULL;
}


llhnode* finddp(const linkedlisth *llh, const datatype *data)
{
    llhnode *pp = llh;
    if (llh != NULL)
    {
        while (pp->next != NULL)
        {
            if ((pp->next)->data == *data)
            {
                return pp;
            }
            else
            {
                pp = pp->next;
            }
        }
    }
    return NULL;
}


int changei(linkedlisth *llh, const int i, const datatype * data)
{
    llhnode *node = findip(llh, i);
    if(node == NULL)
        return -1;
    node->next->data = *data;
    return 0;
}

int changed(linkedlisth *llh, const datatype *olddata, const datatype *newdata)
{
    llhnode *nodep = finddp(llh,olddata);
    if(nodep == NULL)
        return -1;
    nodep->next->data = *newdata;
    return 0;
}

datatype findi(const linkedlisth *llh, const int i)
{
    llhnode *node = findip(llh,i);
    if(node != NULL)
        return node->next->data;
    return NULL;
}

int findd(const linkedlisth *llh, const datatype *data)
{
    int i = -1;
    llhnode  *pp = llh;
    if(llh != NULL)
    {
        while (pp->next != NULL)
        {
            if(pp->next->data == *data)
            {
                return i + 1;
            }
            else
            {
                pp = pp->next;
                i++;
            }
        }
    }
    return -1;
}

int getsize(const linkedlisth *llh)
{
    if (llh == NULL)
        return -1;
    return llh->data;
}

int display(const linkedlisth *llh)
{
    if(llh == NULL)
        return -1;
    llhnode  *pp = llh;
    int count = 0;
    while (pp->next != NULL)
    {
        printf("%d ", (pp->next)->data);
        count++;
        pp = pp->next;
    }
    printf("    count = %d, size = %d\n", count, llh->data);
    return count;
}

int destroy(linkedlisth *llh)
{
    if(llh == NULL)
        return -1;
    while (llh->next != NULL)
    {
        deletei(llh, 0);
    }
    free(llh);
    llh = NULL;
    return 0;
}