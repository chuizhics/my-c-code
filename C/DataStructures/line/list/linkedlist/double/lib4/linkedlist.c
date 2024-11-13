#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"

typedef struct llist_node_st
{
    struct llist_node_st *prev;
    struct llist_node_st *next; 
    char data[0];  // 0  need c99
    
}llist_node_st;
 
struct llist_head_st
{
    int datasize;
    int length;
    llist_node_st head;

}; 


LLIST *llist_create(int datasize)
{
    struct llist_head_st *ptr = NULL;
    ptr = malloc(sizeof(*ptr));
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

int llist_insert(LLIST *lt, const void *data, int mode)
{
    struct llist_head_st *list = lt;
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


void llist_travel(LLIST *lt, llist_op *op)
{
    struct llist_head_st *list = lt;
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

static llist_node_st *find_(struct llist_head_st *list , const void *key, llist_compare *compare)
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

void *llist_find(LLIST * lt, const void * key, llist_compare * compare)
{
    struct llist_head_st *list = lt;
    llist_node_st *node = find_(list,key,compare);
    if(node != &list->head)
        return node->data;
    return NULL;
}

int llist_delete(LLIST *lt, const void * key, llist_compare *compare)
{
    struct llist_head_st *list = lt;
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

int llist_fetch(LLIST *lt, const void *key, llist_compare *compare , void * data)
{
    struct llist_head_st *list = lt;

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


int llist_getsize(LLIST *lt)
{
    if(lt == NULL)
        return -1;
    struct llist_head_st *list = lt;
    return list->length;
}

void llist_destroy(LLIST *lt)
{
    struct llist_head_st *list = lt;
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