#pragma once

typedef struct zlistnode_s
{
	struct zlistnode_s* next;
	struct zlistnode_s* prev;
} zlistnodeT;


/*
Intended use is to embed a zlistnodeT as first element inside a structure

typedef struct something {
	zlistnodeT* zlistnode;  //must be first
	char* str;
} stringlist_t;

the listnodes themselves form a linked list.  listnode->next connects to the next item's listnode

*/

typedef struct zlist_s
{
	zlistnodeT sentinal_head; //sentinal next is head
	zlistnodeT sentinal_tail; //sentinal prev is tail
	
	/* The are TWO sentinal nodes.  WHY?
	 * If starting in the middle of a linked list, you can keep going next->next until you get to NULL.
	 * With sentinal nodes, you wrap back, unless you check against the list pointer.  If you are in the middle of a list, the list pointer
	 * might not be handy.  So when getting ->next, if next->next is NULL, then ->next is sentinal.  This check also 'prefetches' the next next node.
	 * Not sure if that's useful behavior or not.
	 */
	
} zlistT;

//#define zlist_head(ZLIST) ((void*) ((ZLIST)->sentinal_head.next))
//#define zlist_head(ZLIST) ((void*) ((ZLIST)->sentinal_head.next))

void* zlist_head(zlistT*);

void* zlist_tail(zlistT*);

//(ZLIST)->sentinal_head != &(ZLIST)->sentinal

//#define zlist_tail(ZLIST) ((void*) ((ZLIST)->sentinal_prev.prev))

//returns next/prev pointer OR NULL if bumping into the sentinal node
#define zlist_next(ZNODE)  (((ZNODE)->zlistnode.next->next)? ((void*) ((ZNODE)->zlistnode.next)) : NULL)
#define zlist_prev(ZNODE)  (((ZNODE)->zlistnode.prev->prev)? ((void*) ((ZNODE)->zlistnode.prev)) : NULL)



zlistT* zlist_check_init(zlistT* list);

#define zlist_addhead(ZLIST,ZNODE)      zlist_insert_node_after( &(zlist_check_init(ZLIST)->sentinal_head), ZNODE)
#define zlist_addtail(ZLIST,ZNODE)      zlist_insert_node_after( zlist_check_init(ZLIST)->sentinal_tail.prev, ZNODE)


//skips the init check:  only for performance places like internal ram.c, etc maybe the interpreter if init is forced other ways
#define zlist_addhead_nocheck(ZLIST,ZNODE)      zlist_insert_node_after( &((ZLIST)->sentinal_head), ZNODE)
#define zlist_addtail_nocheck(ZLIST,ZNODE)      zlist_insert_node_after( (ZLIST)->sentinal_tail.prev, ZNODE)

void* zlist_remove_mid( zlistnodeT* node);
void* zlist_insert_node_after( zlistnodeT* node, zlistnodeT* newnode);
zbool zlist_cleanup(zlistT* list);
