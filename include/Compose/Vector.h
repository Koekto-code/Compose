#ifndef COMPOSE_VECTOR_H
#define COMPOSE_VECTOR_H

#include <stddef.h>
#include <assert.h>

typedef struct
{
	void *data;
	size_t length;
	size_t capacity;
	size_t objsize;
}
CmVector;

static CmVector CmVectorCreate(size_t objsize)
{
	CmVector vec;
	vec.data = NULL;
	vec.length = 0;
	vec.capacity = 0;
	vec.objsize = objsize;
	return vec;
}

#define CmVectorCreateM(T) CmVectorCreate(sizeof(T))

void CmVectorDestroy(CmVector *self);

void CmVectorResize(CmVector *self, size_t newlen);

#define CmVectorResizeM(S, A) CmVectorResize(&(S), (S).length A)

void CmVectorPushBack(CmVector *self, const void *obj);

void CmVectorPopBack(CmVector *self);

static void *CmVectorBack(CmVector *self)
{
	assert(self->length);
	return (void *)((size_t)self->data + (self->length - 1) * self->objsize);
}

#endif
