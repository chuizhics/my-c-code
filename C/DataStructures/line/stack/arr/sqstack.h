#ifndef SQSTACK_H__
#define SQSTACK_H__

#define STACKSIZE 32

typedef int datatype;

typedef void sqk(datatype *);

typedef struct sqstack_st
{
    datatype data[STACKSIZE];
    int top;
}sqstack;

sqstack *st_create();

int st_isempty(sqstack *);

int st_push(sqstack *,datatype *);

int st_pop(sqstack *, datatype *);

int st_top(sqstack *, datatype *);

void st_travel(sqstack *, sqk *);

void st_destroy(sqstack *);

#endif