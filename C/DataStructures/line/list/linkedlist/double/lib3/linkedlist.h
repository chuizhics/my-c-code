#ifndef LINKEDLIST_H__
#define LINKEDLIST_H__

#define LLIST_FORWARD 1
#define LLIST_BACKWARD 2

typedef void llist_op(const void *);

typedef int llist_compare(const void *, const void *);

typedef struct llist_node_st
{
    struct llist_node_st *prev;
    struct llist_node_st *next;
    char data[0]; // 0  need c99

} llist_node_st;

typedef struct llist_head_st
{
    int datasize;
    int length;
    llist_node_st head;
    int (*insert)(struct llist_head_st *, const void *, int);
    void *(*find)(struct llist_head_st *, const void *, llist_compare *);
    int (*delete)(struct llist_head_st *, const void *, llist_compare *);
    int (*fetch)(struct llist_head_st *, const void *, llist_compare *, void *);
    void (*travel)(struct llist_head_st *, llist_op *);
    void (*destroy)(struct llist_head_st *);
} LLIST;

LLIST *llist_create(int datasize);



#endif