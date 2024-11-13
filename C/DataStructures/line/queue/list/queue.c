
#include "queue.h"

#include "linkedlist.h"


static int always_math(const void *,const void *);


QUEUE *queue_create(int datasize)
{
    return llist_create(datasize);
}

int queue_enqueue(QUEUE *queue, const void *data)
{
    return llist_insert(queue,data,LLIST_BACKWARD);
}

static int always_math(const void *p,const void *q)
{
    return 0;
}


int queue_dequeue(QUEUE *queue, void *data)
{
    return llist_fetch(queue,(void *)0, always_math, data);
}


int queue_isempty(QUEUE *queue)
{
    return llist_isempty(queue);
}

int queue_getlength(QUEUE *queue)
{
    return llist_getlength(queue);

}

void queue_destroy(QUEUE *queue)
{  
    llist_destroy(queue);
}
