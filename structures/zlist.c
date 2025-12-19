#include "ztypes.h"
#include "zmem.h"
#include "zlist.h"

void zlist_init(zlistT* list){
	memset(list, 0, sizeof(*list));
	list->sentinal_head.next = &list->sentinal_tail;	
	list->sentinal_tail.prev = &list->sentinal_head;

	//list->sentinal_head.DEBUG = 'H';
	//list->sentinal_tail.DEBUG = 'T';

	//printf("inited list\n");
}

zlistT* zlist_check_init(zlistT* list){
	if (!list->sentinal_head.next){
		list->sentinal_head.next = &list->sentinal_tail;	
		list->sentinal_tail.prev = &list->sentinal_head;
		//list->sentinal_head.DEBUG = 'H';
		//list->sentinal_tail.DEBUG = 'T';
	}
	//printf("check inited list\n");
	return list;
}

void* zlist_head(zlistT* list){
	
	if (!list)
		return NULL;
		
	//return NULL if the head's next is just the tail
	if (list->sentinal_head.next == &list->sentinal_tail)
		return NULL;
	
	return list->sentinal_head.next;
	
}

void* zlist_tail(zlistT* list){
	
	if (!list)
		return NULL;
	
	//return NULL if the tail's previous is just the head
	if (list->sentinal_tail.prev == &list->sentinal_head)
		return NULL;
	
	return list->sentinal_tail.prev;
	
}

//remove a node from the list. returns the node back

void* zlist_remove_mid( zlistnodeT* node){
	
	if (!node || !node->prev || !node->next){
	    fprintf(stdout, "Node %p: null or has null prev/next\n", node);
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

//inserts newnode AFTER node.
void* zlist_insert_node_after( zlistnodeT* node, zlistnodeT* newnode){
	
	if (!node || !node->next){
	    fprintf(stdout, "Node %p: null or has null next.  To insert at end of list use addtail\n", node);
	    exit(1);
	}

	if ( newnode->prev || newnode->next){
		fprintf(stdout, "Can't add node that already has next/prev; its already in another list!\n");
		exit(1);
	}
	
	newnode->prev = node;
	newnode->next = node->next;
	
	node->next->prev = newnode;
	node->next = newnode;
		
	return newnode;
}

//inserts new node before
void* zlist_insert_node_before(zlistnodeT* node, zlistnodeT* newnode) {

	if (!node || !node->prev) {
		fprintf(stdout, "Node %p: null or has null prev.\n", node);
		exit(1);
	}
//	if (node->prev->prev == NULL) {
	//	printf("setinal case\n");
	//}

	return zlist_insert_node_after(node->prev, newnode);//  OK if prev is the setinal

}

//returns end of list OR sentinal for an empty list.  This can be used to append to any list, even empty list.
void* zlist_head_for_insert(zlistT* list) {
	if (!list)
		return NULL;
	zlist_check_init(list);

	return list->sentinal_tail.prev;  //if list is empty, the sentinal_head is returned
}


zbool zlist_cleanup(zlistT* list) {
	zlistnodeT* node = zlist_head(list);
	//printf("list clean\n");
	while(node){
	
		//printf("\t node %p     next %p    sentinal %p\n", node, node->next, &list->sentinal_tail);
		
		zlistnodeT* next = node->next;
		
		if (node == &list->sentinal_tail)
			break;
		
		ram_free(zlist_remove_mid(node));
		
		
		node=next;
		
	}
	//printf(" end list\n");
	
	return ZTRUE;
}
