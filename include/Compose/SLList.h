#ifndef COMPOSE_QUEUE_H
#define COMPOSE_QUEUE_H

#include <stddef.h>
#include <malloc.h>

typedef struct CmSLNode CmSLNode;

struct CmSLNode
{
	CmSLNode *next;
	char data[];
};

typedef struct
{
	CmSLNode *first;
	CmSLNode *last;
	size_t length;
}
CmSLList;

static CmSLList CmSLListCreate()
{
	CmSLList queue;
	queue.first = NULL;
	queue.last = NULL;
	queue.length = 0;
	return queue;
}

void CmSLListDestroy(CmSLList *self);

void CmSLListPush(CmSLList *self, void *object, size_t objsize);

void CmSLListPop(CmSLList *self);

CmSLNode *CmSLListGet(CmSLList *self, size_t index);

CmSlNode *CmSLListDetach(CmSLList *self, size_t index);

#define CmSLListDelete(self, index) free(CmSLListDetach(self, index))

#endif
