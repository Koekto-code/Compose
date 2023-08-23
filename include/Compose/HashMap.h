#ifndef COMPOSE_HASH_MAP_H
#define COMPOSE_HASH_MAP_H

typedef struct
{
	void *branches[256];
}
CmHashMapNode;

typedef struct
{
	HashMapNode *root;
}
CmHashMap;

static CmHashMap CmHashMapCreate()
{
	CmHashMap map = { NULL };
	return map;
}

void 

#endif