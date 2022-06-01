#include "ztypes.h"
#include "zmem.h"
#include "zlist.h"

void* zlist_addhead(zlistT* list, zlistnodeT* node)
{
	
	if (!list || !node)
		return NULL;

	node->next = list->head;

	if (list->head)
		list->head->prev = node;

	list->head = node;

	if (!list->tail)  //also tail if short list
		list->tail = node;
	
	return node;
}

void* zlist_addtail(zlistT* list, zlistnodeT* node)
{
	if (!list || !node)
		return NULL;

	node->prev = list->tail;

	if (list->tail)
		list->tail->next = node;

	list->tail = node;

	if (!list->head)
		list->head = node;
	
	return  node;
}

void zlist_remove(zlistT* list, zlistnodeT* node)
{
	
	if (list->head == node)
		list->head = node->next;

	if (list->tail == node)
		list->tail = node->prev;

	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;

	node->next = NULL;
	node->prev = NULL;
}


void* zlist_remove_mid( zlistnodeT* node){
	
	if (!node || !node->prev || !node->next){
	    fprintf(stdout, "Node %p: null or has null prev/next\n");
	    exit(1);
	}

	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;

	node->next = NULL;
	node->prev = NULL;
	
	return node;
}

zbool zlist_cleanup(zlistT* list) {
	zlistnodeT* node;
	
	for (node = list->head ; node; node = list->head) {
			zlist_remove(list, node);
			ram_free(node);
	}
	return ZTRUE;
}
