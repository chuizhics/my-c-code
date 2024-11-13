#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "btree.h"

btree *btree_create(int initsize)
{
    btree *bt = NULL;
    if (initsize > 0)
    {
        bt = malloc(sizeof(btree));
        if (bt == NULL)
        {
            return NULL;
        }
        bt->datasize = initsize;
        bt->length = 0;
        bt->headnode = NULL;
    }
    return bt;
}

static node_st *_insert(node_st *root, void *data, int datasize, compare *cp)
{
    if (root == NULL)
    {
        root = malloc(sizeof(node_st) + datasize);
        if (root == NULL)
        {
            return NULL;
        }
        memcpy(root->data, data, datasize);
        root->left = NULL;
        root->right = NULL;
        return root;
    }
    if (cp(data, root->data) > 0)
    {
        root->right = _insert(root->right, data, datasize, cp);
    }
    else
    {
        root->left = _insert(root->left, data, datasize, cp);
    }
    return root;
}

int btree_insert(btree *bt, void *data, compare *cp)
{
    if (bt == NULL || data == NULL)
    {
        return -1;
    }
    bt->headnode = _insert(bt->headnode, data, bt->datasize, cp);
    if (bt->headnode != NULL)
    {
        bt->length++;
        return 0;
    }
    return -2;
}

static int _find(node_st *root, const void *key, compare *cp, void *data)
{
    // if(root == NULL)
    // {
    //     return -1;
    // }
    // if(cp(key,root->data))   // database ?  can not do it
    return 0;
}

int btree_find(btree *bt, const void *key, compare *cp, void *data)
{
    if (bt == NULL)
    {
        return -1;
    }
    return _find(bt->headnode, key, cp, data);
}

static void _draw(node_st *root, btree_op *op, int level)
{
    int i = 0;
    if (root == NULL)
        return;
    _draw(root->right,op,level + 1);
    for(i = 0; i < level; i++)
    {
        printf("    ");
    }
    op(root->data);
    _draw(root->left, op,level + 1);
}

void btree_draw(btree *bt, btree_op *op)
{
    if (bt != NULL)
        _draw(bt->headnode, op, 0);
}

static void _displayinorder(node_st *headnode, btree_op *op)
{
    if (headnode != NULL)
    {
        _displayinorder(headnode->left, op);
        op(headnode->data);
        _displayinorder(headnode->right, op);
    }
}

static void _displaypreorder(node_st *headnode, btree_op *op)
{
    if (headnode != NULL)
    {
        op(headnode->data);
        _displaypreorder(headnode->left, op);
        _displaypreorder(headnode->right, op);
    }
}

static void _displaypostorder(node_st *headnode, btree_op *op)
{
    if (headnode != NULL)
    {
        _displaypostorder(headnode->left, op);
        _displaypostorder(headnode->right, op);
        op(headnode->data);
    }
}

int btree_displayinorder(btree *bt, btree_op *op)
{
    if (bt == NULL)
    {
        return -1;
    }
    _displayinorder(bt->headnode, op);
    return 0;
}

int btree_displaypreorder(btree *bt, btree_op *op)
{
    if (bt == NULL)
    {
        return -1;
    }
    _displaypreorder(bt->headnode, op);
    return 0;
}

int btree_displaypostorder(btree *bt, btree_op *op)
{
    if (bt == NULL)
    {
        return -1;
    }
    _displaypostorder(bt->headnode, op);
    return 0;
}

static void _destroy(node_st **headnode)
{
    if ((*headnode) != NULL)
    {
        if ((*headnode)->left == NULL && (*headnode)->right == NULL)
        {
            free((*headnode));
            *headnode = NULL;
            return;
        }
        _destroy(&((*headnode)->left));
        _destroy(&((*headnode)->right));
        _destroy(headnode);
    }
}

void btree_destroy(btree *bt)
{
    if (bt != NULL)
    {
        if (bt->headnode != NULL)
        {
            _destroy(&bt->headnode);
        }
        free(bt);
    }
}