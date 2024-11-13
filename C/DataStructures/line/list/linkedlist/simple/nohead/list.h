#ifndef LIST_H__
#define LIST_H__

#define NAMESIZE 32

typedef struct score_st
{
    int id;
    char name[NAMESIZE];
    int math;
    int chinese;
} datatype;

typedef struct node_st
{
    datatype data;
    struct node_st *next;
} stlist, stlistnode;


int list_insert_head(stlist **, const datatype *);

int list_insert_tail(stlist **, const datatype *);

int list_delete_byid(stlist **, int );

datatype list_delete_byname(stlist **, char *);

int list_findbyid(stlist *, int , datatype *);

datatype list_findbyname(stlist*, char *);

int list_display(stlist *const);


void list_destroy(stlist *);


#endif