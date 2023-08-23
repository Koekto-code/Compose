#include "../../include/Compose/Vector.h"

#include <malloc.h>
#include <string.h>

static size_t growFn(size_t current) {
	// nearly current * 1.625
	return current + (current >> 1) + (current >> 3) + 1;
}

static size_t shrinkFn(size_t current) {
	// nearly current * 0.625
	return current - ((current >> 2) + (current >> 3));
}

void CmVectorDestroy(CmVector *self)
{
	if (self->data)
		free(self->data);
	self->length = 0;
	self->capacity = 0;
}

void CmVectorResize(CmVector *self, size_t newlen)
{
	if (self->capacity >= newlen)
	{
		self->length = newlen;
		return;
	}
	
	void *prev = self->data;
	size_t newCap = growFn(self->capacity);
	while (newCap < newlen)
		newCap = growFn(newCap);
	
	self->data = NULL;
	while (!self->data)
		self->data = realloc(prev, newCap * self->objsize);
	
	self->length = newlen;
}

void CmVectorPushBack(CmVector *self, const void *obj)
{
	size_t pos = self->length * self->objsize;
	CmVectorResize(self, self->length + 1);
	memcpy(self->data + pos, obj, self->objsize);
}

void CmVectorPopBack(CmVector *self)
{
	CmVectorResize(self, self->length - 1);
}
