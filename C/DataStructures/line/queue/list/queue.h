#ifndef QUEUE_H__
#define QUEUE_H__

#include "linkedlist.h"

typedef LLIST QUEUE;


QUEUE *queue_create(int );

int queue_enqueue(QUEUE *, const void *);

int queue_dequeue(QUEUE *, void *);

int queue_isempty(QUEUE *);

int queue_getlength(QUEUE *);

void queue_destroy(QUEUE *);


#endif
