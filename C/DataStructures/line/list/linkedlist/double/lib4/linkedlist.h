#ifndef LINKEDLIST_H__
#define LINKEDLIST_H__


#define LLIST_FORWARD  1
#define LLIST_BACKWARD 2

typedef void LLIST;  

typedef void llist_op(const void *);
 
typedef int llist_compare(const void *, const void *);


LLIST *llist_create(int datasize);

int llist_insert(LLIST *,const void * , int mode);

void *llist_find(LLIST *, const void *, llist_compare *);

int llist_delete(LLIST *, const void * key, llist_compare *);

int llist_fetch(LLIST *, const void *key, llist_compare *, void * data);

void llist_travel(LLIST *, llist_op *);

int llist_getsize(LLIST *);

void llist_destroy(LLIST *);

#endif