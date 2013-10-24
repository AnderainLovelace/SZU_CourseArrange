#ifndef _LIST_H_
#define _LIST_H_

typedef struct tagNODE
{
	void			*	pdata;
	struct tagNODE	*	next;
}
NODE;

typedef struct
{
	NODE	 	*head,*last;
	int			size;
}
LIST;

void list_init		(LIST * l);
void list_destory	(LIST * l);
void list_push		(LIST * l,void * p);

#endif