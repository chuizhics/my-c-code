#ifndef BTREE_H__
#define BTREE_H__


typedef void btree_op(const void *);

typedef int compare(const void *, const void *);

typedef struct node_st
{
    struct node_st *left;
    struct node_st *right;
    char data[0];
} node_st;

typedef struct head_node_st
{
    int length;
    int datasize;
    struct node_st *headnode;
}btree;

btree *btree_create(int initsize);

int btree_insert(btree *bt, void *data, compare*);

void btree_draw(btree *bt, btree_op *op);

int btree_find(btree *bt, const void *key, compare *, void *data);

int btree_displayinorder(btree *bt, btree_op*);

int btree_displaypreorder(btree *bt, btree_op*);

int btree_displaypostorder(btree *bt, btree_op*);

void btree_destroy(btree *bt);

#endif