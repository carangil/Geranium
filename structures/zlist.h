

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
	zlistnodeT* head;
	zlistnodeT* tail;
} zlistT;

#define zlist_head(ZLIST) ((void*) ((ZLIST)->head))
#define zlist_tail(ZLIST) ((void*) ((ZLIST)->tail))
#define zlist_next(ZNODE) ((void*) ((ZNODE)->zlistnode.next))
#define zlist_prev(ZNODE) ((void*) ((ZNODE)->zlistnode.prev))

void* zlist_addhead(zlistT* list, zlistnodeT* node);
void* zlist_addtail(zlistT* list, zlistnodeT* tail);
void zlist_remove(zlistT* list, zlistnodeT* node);
void* zlist_remove_mid( zlistnodeT* node);

zbool zlist_cleanup(zlistT* list);
