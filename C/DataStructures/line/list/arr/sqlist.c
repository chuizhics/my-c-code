#include <stdio.h>
#include <stdlib.h>
#include "sqlist.h"

sqlist *sqlist_create()
{
	sqlist *me;
	me = malloc(sizeof(*me));
	if (me == NULL)
		return NULL;
	me->last = -1;

	return me;
}

void sqlist_create1(sqlist **ptr)
{
	*ptr = malloc(sizeof(**ptr));
	if (*ptr == NULL)
		return;

	(*ptr)->last = -1;
	return;
}
int sqlist_append(sqlist *me, datatype *data)
{
	if (me->last == DATASIZE - 1)
		return -1;
	me->data[me->last + 1] = *data;
	me->last++;
	return 0;
}

int sqlist_insert(sqlist *me, int i, datatype *data)
{

	int j;
	if (me->last == DATASIZE - 1)
		return -1;
	if (i < 0 || i > me->last + 1)
		return -2;

	for (j = me->last + 1; j > i; j--)
	{
		me->data[j] = me->data[j - 1];
	}

	me->data[i] = *data;
	me->last++;
	return 0;
}

int sqlist_delete(sqlist *me, int i)
{
	int j;
	if (sqlist_isempty(me) || i < 0 || i > me->last)
		return -1;
	for (j = i; j < me->last; j++)
	{
		me->data[j] = me->data[j + 1];
	}
	me->last--;
	return 0;
}

int sqlist_find(sqlist *me, datatype *data)
{
	int i;
	if (sqlist_isempty(me))
		return -1;
	for (i = 0; i <= me->last; i++)
	{
		if (me->data[i] == *data)
		{
			return i;
		}
	}
	return -1;
}

int sqlist_isempty(sqlist *me)
{
	if (me == NULL || me->last == -1)
		return 1;
	else
		return 0;
}

int sqlist_setempty(sqlist *me)
{
	if (sqlist_isempty(me))
		return 0;
	me->last = -1;
	return 0;
}

int sqlist_getnum(sqlist *me)
{
	if (sqlist_isempty(me))
		return 0;
	return me->last + 1;
}

void sqlist_display(sqlist *me)
{
	int i;
	if (sqlist_isempty(me))
		return;
	for (i = 0; i <= me->last; i++)
	{
		printf("%d ", me->data[i]);
	}
	printf("\n");
	return;
}

int sqlist_destroy(sqlist *me)
{
	free(me);
	return 0;
}

int sqlist_union(sqlist *list1, sqlist *list2)
{
	int i;
	if (list1 == NULL)
		return -1;
	if (list2 != NULL)
	{
		for (i = 0; i <= list2->last; i++)
		{
			if(sqlist_find(list1,&list2->data[i]) < 0)
			{
				if(sqlist_append(list1, &list2->data[i]) == 0)
				{
					return 0;
				}
				else 
					return -1;
			}
		}
	}
	return 0;
}
