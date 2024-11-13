#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int list_insert_head(stlist **me, const datatype *data)
{
    stlist *newnode = malloc(sizeof(*newnode));
    if (newnode == NULL)
        return -1;

    newnode->data = *data; // struct assign can use "="
    // newnode->data.id = data->id;
    // snprintf(newnode->data.name, NAMESIZE, data->name);
    // newnode->data.math = data->math;
    // newnode->data.chinese = data->chinese;
    newnode->next = *me;

    *me = newnode;

    return 0;
}

int list_insert_tail(stlist **me, const datatype *data)
{
    stlist *newnode = malloc(sizeof(*newnode));
    stlist *curnode = *me;
    if (newnode == NULL)
        return -1;
    newnode->data = *data;
    newnode->next = NULL;
    if (curnode == NULL)
    {
        *me = newnode;
    }
    else
    {
        while (curnode->next != NULL)
        {
            curnode = curnode->next;
        }
        curnode->next = newnode;
    }
    return 0;
}


/**
 * -1 error
 * -2 not found
 *  0 right found
 * 
*/
static int _findbyid_pre(stlist *me, int id, stlistnode **prenode)
{
    *prenode = NULL;
    stlistnode *curnode = NULL;
    if (me == NULL)
        return -1; // error
    if (me->data.id == id)
        return 0;
    if (me->next == NULL)
        return -2; // not found
    for (curnode = me; curnode->next != NULL && curnode->next->data.id != id; curnode = curnode->next)
        ;

    if (curnode->next == NULL)
        return -2;
    else
    {
        *prenode = curnode;
    }
    return 0;
}

static stlistnode *_findbyid(stlist *me, int id)
{
    stlistnode *prenode = NULL;
    if (_findbyid_pre(me, id, &prenode) == 0)
    {
        if (prenode == NULL)
            return me;
        return prenode->next;
    }
    return NULL;
}

int list_delete_byid(stlist **me, int id)
{
    stlistnode *prenode = NULL;
    stlist *tmp = NULL;
    if( _findbyid_pre(*me, id, &prenode) == 0)
    {
        if(prenode == NULL)
        {  
            tmp = *me;
            *me = (*me)->next;
        }
        else
        {
            tmp = prenode->next;
            prenode->next = prenode->next->next;
        }
        free(tmp);
        return 0;
    }
    return -1;
}

datatype list_delete_byname(stlist **me, char *name)
{
}

int list_findbyid(stlist *me, int id, datatype *data)
{
    const stlistnode *node = _findbyid(me, id);
    if (node != NULL)
    {
        *data = node->data;
        return 0;
    }
    return -1;
}

datatype list_findbyname(stlist *me, char *name)
{
}

int list_display(stlist *const me)
{
    stlist const *cur = me;
    if (me == NULL)
        return -1;
    while (cur != NULL)
    {
        printf("%d %s %d %d\n", cur->data.id, cur->data.name, cur->data.math, cur->data.chinese);
        cur = cur->next;
    }
    printf("\n");
    return 0;
}

void list_destroy(stlist *me)
{
    stlist *curnode = NULL, *tmp = NULL;
    if (me != NULL)
    {
        for (curnode = me; curnode != NULL; curnode = tmp)
        {
            tmp = curnode->next;
            free(curnode);
        }
    }
}