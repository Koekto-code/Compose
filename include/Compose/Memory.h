#ifndef COMPOSE_MEMORY_H
#define COMPOSE_MEMORY_H

#include <stddef.h>
#include <malloc.h>

static void *CmAlloc(size_t memsize)
{
	void *mem;
	do { mem = malloc(memsize); } while (!mem);
	return mem;
}

#endif
 
