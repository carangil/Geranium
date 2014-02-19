

typedef struct zlistnode_s
{
	struct zlistnode_s* next;
	struct zlistnode_s* prev;
} zlistnode_t;


/*
Intended use is to embed a zlistnode_t as first element inside a structure

typedef struct something {
	zlistnode_t* zlistnode;  //must be first
	char* str;
} stringlist_t;

the listnodes themselves form a linked list.  listnode->next connects to the next item's listnode

*/

typedef struct zlist_s
{
	zlistnode_t* head;
	zlistnode_t* tail;
} zlist_t;

#define zlist_head(ZLIST) ((void*) ((ZLIST)->head))
#define zlist_tail(ZLIST) ((void*) ((ZLIST)->tail))
#define zlist_next(ZNODE) ((void*) ((ZNODE)->zlistnode.next))
#define zlist_prev(ZNODE) ((void*) ((ZNODE)->zlistnode.prev))

void zlist_addhead(zlist_t* list, zlistnode_t* node);
void zlist_addtail(zlist_t* list, zlistnode_t* tail);
void zlist_remove(zlist_t* list, zlistnode_t* node);
