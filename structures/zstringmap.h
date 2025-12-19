#include "ztypes.h"
#include "zmem.h"
#include "zlist.h"
#include "zstring.h"




typedef struct zstringmapS{
    struct      stringmapentryS** buckets;  //zarray of zarray of elements
    zlistT      ordered;
	zbool		own_elements; //default true.  Will 'free' all the element pointers automatically when destroyed
}zstringmapT;



zstringmapT* zstringmap_mk(int nb);

zbool  zstringmap_put(zstringmapT* map, char* key, void* value);
void  zstringmap_delete(zstringmapT* map, char* key);
void* zstringmap_get(zstringmapT* map, char* key);
zstringmapT*  zstringmap_disown(zstringmapT* map); 

//Usage: zstringmap_nextkey(map, &key, &item, &cursor);  where item is pointer to a pointer variable, cursor is void*
zbool zstringmap_nextkey( zstringmapT* map, char** key, void* voidvoidstar, void** cursor);





