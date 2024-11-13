#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"



LLIST *llist_create(int datasize)
{
    LLIST *ptr = malloc(sizeof(LLIST));
    if (ptr == NULL)
    {
        //exit(1);
        return NULL;
    }
    ptr->datasize = datasize;
    ptr->head.prev = &ptr->head;
    ptr->head.next = &ptr->head;
    ptr->length = 0;
    return ptr;
}

int llist_insert(LLIST *list, const void *data, int mode)
{
    llist_node_st *newnode = NULL;
    llist_node_st *head = NULL;
    if (list == NULL)
    { 
        return -1;
    }
    head = &list->head;

    newnode = malloc(sizeof(llist_node_st) + list->datasize);
    if(newnode == NULL)
    {
        return -2;
    }

    memcpy(newnode->data,data,list->datasize);

    newnode->prev = newnode;
    newnode->next = newnode;

    if(mode == LLIST_FORWARD)
    {
        newnode->prev = head;
        newnode->next = head->next;

        // head->next = newnode;
        // newnode->next->prev = newnode;

    }
    else if(mode == LLIST_BACKWARD)
    {
        newnode->prev = head->prev;
        newnode->next = head;

        // head->prev = newnode;
        // newnode->prev->next = newnode;
    }
    else
    {
        return -4;
    }
    newnode->prev->next = newnode;
    newnode->next->prev = newnode;
    list->length++;
    return 0;
}


void llist_travel(LLIST *list, llist_op *op)
{
    llist_node_st *head = NULL;
    llist_node_st *curnode = NULL;
    if(list != NULL)
    {
        head = &list->head;
        curnode = head->next;
        while (curnode != head)
        {
            op(curnode->data);
            curnode = curnode->next;   
        }
    }
}

static llist_node_st *find_(LLIST *list, const void *key, llist_compare *compare)
{
    llist_node_st *head = NULL;
    llist_node_st *curnode = NULL;
    if(list == NULL)
        return NULL;
    head = &list->head;
    curnode = head->next;
    while (curnode != head)
    {
        if(compare(key, curnode->data) == 0)
        {
            break;
        }
        curnode = curnode->next;
    }
    return curnode;
}

void *llist_find(LLIST * list, const void * key, llist_compare * compare)
{
    llist_node_st *node = find_(list,key,compare);
    if(node != NULL && node != &list->head)
        return node->data;
    return NULL;
}

int llist_delete(LLIST *list, const void * key, llist_compare *compare)
{
    llist_node_st *node = find_(list,key,compare);
    if(node == NULL)
        return -1; //error
    if(node == &(list->head))
        return -2; // not found
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    list->length--;
    return 0;
}

int llist_fetch(LLIST *list, const void *key, llist_compare *compare , void * data)
{
    llist_node_st *node = find_(list,key,compare);
    if(node == NULL)
        return -1;
    if(node == &(list->head))
        return -2; // not found
    if(data != NULL)
        memcpy(data, node->data, list->datasize);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
    list->length--;
    return 0;
}


int llist_isempty(LLIST *list)
{
    return list->length == 0;
}

int llist_getlength(LLIST *list)
{
    return list->length;
}



void llist_destroy(LLIST *list)
{
    llist_node_st *curnode = NULL;
    llist_node_st *tmp = NULL;
    //printf("[%s:%d]---list = %p\n",__FUNCTION__,__LINE__, list);
    if(list != NULL)
    {
        curnode = list->head.next;
        //printf("[%s:%d]---curnode = %p, curnode->prev = %p\n",__FUNCTION__,__LINE__, curnode, curnode->prev);
        while (curnode != &list->head)
        {
            tmp = curnode;
            curnode = curnode->next;
            //printf("[%s:%d]---tmp = %p, curnode = %p\n",__FUNCTION__,__LINE__, tmp, curnode);
            free(tmp);
        }
        free(list);
    }
}