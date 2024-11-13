#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

#define NAMESIZE 32

typedef struct score_st
{
    int id;
    char name[NAMESIZE];
    int math;
    int Chinese;
}score_st;

int comparebyid(const void *p,const void *q)
{
    score_st *ptr1 = p;
    score_st *ptr2 = q;

    return ptr1->id - ptr2->id;
}

void print_score(const void *p)
{
    score_st *ptr = p;
    if(ptr != NULL)
    {
        printf("id=%d name=%s math=%d Chinese=%d\n",ptr->id,ptr->name,ptr->math,ptr->Chinese);
    }
}

int main()
{
    int arr[] = {1,2,3,7,6,5,9,8,4};
    int id;
    int i = 0;
    btree *tree = btree_create(sizeof(score_st));
    score_st tmp;

    for (i = 0; i < sizeof(arr)/sizeof(*arr); i++)
    {
        tmp.id = arr[i];
        snprintf(tmp.name,NAMESIZE,"stu%d",arr[i]);
        tmp.math = rand() %100;
        tmp.Chinese = rand()%100;
        btree_insert(tree, &tmp, comparebyid);
    }
    
    // id = 5;
    // if(btree_find(tree,&id,comparebyid, &tmp) == 0)
    // {
    //     printf("id = %d:\n", id)
    //     print_score(&tmp);
    // }
    // else
    // {
    //     printf("id = %d can not find!\n", id);
    // }

    printf("draw:\n");
    btree_draw(tree,print_score);

    printf("inorder:\n");
    btree_displayinorder(tree, print_score);

    printf("preorder:\n");
    btree_displaypreorder(tree, print_score);

    printf("postorder:\n");
    btree_displaypostorder(tree, print_score);

    btree_destroy(tree);


    exit(0);
}