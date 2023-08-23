#include "../../include/Compose/SLList.h"
#include "../../include/Compose/Memory.h"

#include <malloc.h>
#include <string.h>
#include <assert.h>

void CmSLListDestroy(CmSLList *self)
{
	while (self->length)
		CmSLListPop(self);
}

void CmSLListPush(CmSLList *self, void *object, size_t objsize)
{
	CmSLNode *node = CmAlloc(sizeof(CmSLNode) + objsize);
	memcpy(node->data, object, objsize);
	
	node->next = NULL;
	if (self->last)
	{
		self->last->next = node;
		self->last = node;
		++self->length;
	}
	else self->first = self->last = node;
}

void CmSLListPop(CmSLList *self)
{
	assert(self->length && self->first && self->last);
	--self->length;
	
	CmSLNode *node = self->first;
	self->first = node->next;
	
	if (!self->first)
		self->last = NULL;
	
	free(node);
}

CmSLNode *CmSLListGet(CmSLList *self, size_t index)
{
	assert(index < self->length);
	
	CmSLNode *iter = self->first;
	while (index)
	{
		iter = iter->next;
		--index;
	}
	return iter;
}

CmSlNode *CmSLListDetach(CmSLList *self, size_t index)
{
	assert(self->length && self->first && self->last);
	--self->length;
	
	CmSLNode *node = self->first;
	
	if (!index)
	{
		self->first = node->next;
		if (!self->first)
			self->last = NULL;
		
		return node;
	}
	
	while (--index)
		node = node->next;
	
	CmSLNode *detached = node->next;
	assert(detached);
	
	node->next = detached->next;
	return detached;
}
