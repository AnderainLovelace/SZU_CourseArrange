#include <stdlib.h>
#include "LIST.h"

void list_init (LIST * l)
{
	l->head	= NULL;
	l->last	= NULL;
	l->size	= 0;
}

void list_destory (LIST * l)
{
	NODE *node1,*node2;

	node1 = l->head;
	
	while (node1)
	{
		node2 = node1;
		node1 = node1->next ;
		
		free (node2->pdata);
		free (node2);
	}

	l->head = l->last = NULL;
	l->size = 0;
}

void list_push (LIST * l,void * p)
{
	if (l->size == 0)
	{
		l->head			= (NODE*)calloc(sizeof(NODE),1);
		l->last			= l->head;
	}
	else
	{
		NODE * n = l->last;
		l->last->next	= (NODE*)calloc(sizeof(NODE),1);
		l->last			= l->last->next;
	}

	l->last->pdata = p;

	l->size ++;

}
