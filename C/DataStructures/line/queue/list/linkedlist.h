#ifndef LINKEDLIST_H__
#define LINKEDLIST_H__

#define LLIST_FORWARD  1
#define LLIST_BACKWARD 2


typedef void llist_op(const void *);
 
typedef int llist_compare(const void *, const void *);


typedef struct llist_node_st
{
    struct llist_node_st *prev;
    struct llist_node_st *next; 
    char data[0];  // 0  need c99   pointer Placeholder
}llist_node_st;

typedef struct llist_head_st
{
    int datasize;
    int length;
    
    llist_node_st head;

}LLIST; 


LLIST *llist_create(int datasize);

int llist_insert(LLIST *,const void * , int mode);

void *llist_find(LLIST *, const void *, llist_compare *);

int llist_delete(LLIST *, const void * key, llist_compare *);

int llist_fetch(LLIST *, const void *key, llist_compare *, void * data);

void llist_travel(LLIST *, llist_op *);

int llist_isempty(LLIST *);

int llist_getlength(LLIST *);

void llist_destroy(LLIST *);

#endif