#include "../include/Compose/SLList.h"
#include "../include/Compose/Memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	CmSLList q = CmSLListCreate();
	CmSLListPush(&q, "Hello World!", sizeof("Hello World!"));
	CmSLListPush(&q, "Bye World!", sizeof("Bye World!"));
	
	CmSLNode *node = q.first;
	while (node)
	{
		printf("%s\n", node->data);
		node = node->next;
	}
	CmSLListDestroy(&q);
}