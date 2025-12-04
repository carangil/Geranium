#include "ztypes.h"
#include "zmem.h"
#include "zlist.h"
#include "zstring.h"




typedef struct zstringmapS{
    struct      stringmapentryS** buckets;  //zarray of zarray of elements
    zlistT      ordered;
}zstringmapT;



zstringmapT* zstringmap_mk(int nb);

zbool  zstringmap_put(zstringmapT* map, char* key, void* value);
void  zstringmap_delete(zstringmapT* map, char* key);
void* zstringmap_get(zstringmapT* map, char* key);


zbool zstringmap_nextkey(zstringmapT* map, char** key, char** value, void** cursor);





