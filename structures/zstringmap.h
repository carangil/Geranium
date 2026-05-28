#include "ztypes.h"
#include "zmem.h"
#include "zlist.h"
#include "zstring.h"




typedef struct zstringmapS{
    struct      stringmapentryS** buckets;  //zarray of zarray of elements
	zbool		own_elements; //default true.  Will 'free' all the element pointers automatically when destroyed
}zstringmapT;



zstringmapT* zstringmap_mk(int nb);

zbool  zstringmap_put(zstringmapT* map, char* key, void* value);
void  zstringmap_delete(zstringmapT* map, char* key);
void* zstringmap_get(zstringmapT* map, char* key);
zstringmapT*  zstringmap_disown(zstringmapT* map); 

//Usage: zstringmap_nextkey(map, &key, &item, &cursor);  where item is pointer to a pointer variable, cursor is unsigned int*
//zbool zstringmap_nextkey2( zstringmapT* map, char** key, void* voidvoidstar, unsigned int* cursor);


typedef unsigned int  zstringmap_cursorT;
zbool zstringmap_nextkey( zstringmapT* map, char** key_out, void* vitem_out, unsigned int* cursor);




