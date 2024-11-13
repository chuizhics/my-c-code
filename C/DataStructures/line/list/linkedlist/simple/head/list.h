#ifndef LIST_H__
#define LIST_H__

typedef int datatype;

typedef struct llhnode
{
    datatype data;

    struct llhnode *next;

} linkedlisth, llhnode;

int create(linkedlisth **);

linkedlisth *create1();

int isempty(const linkedlisth *);

int setempty(linkedlisth *);

int insert(linkedlisth *, const int i, const datatype *);

int insert_order(linkedlisth *, const datatype *);

int append(linkedlisth *, const datatype *);

int deletei(linkedlisth *, const int i);

int deleted(linkedlisth *, const datatype *);

int changei(linkedlisth *, const int i, const datatype *);

int changed(linkedlisth *, const datatype *, const datatype *);

datatype findi(const linkedlisth *,const int i);

int findd(const linkedlisth *,const datatype *);

int getsize(const linkedlisth *);

int display(const linkedlisth *);

int destroy(linkedlisth *);

#endif