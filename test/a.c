#include <stdio.h>
#include "boo.h"

#include "ztypes.h"
#include "zmem.h"

#include "zstring.h"
#include "zstringmap.h"


void dumpmap(zstringmapT* map){

	char* key=NULL;
	char* value=NULL;
	void* cursor=NULL;

	printf(">>");
	while( zstringmap_nextkey(map, &key, &value, &cursor)){
			printf("(%s:%s/%s)->", key, value, zstringmap_get(map, key) );
	}
	printf("\n");

}


int main(int argc, char** args){

	char* a = ram_strdup("butt");

	printf("boo\n");

	boo(7, a);

	ram_free(a);

	char* x = ram_loadstr("a.c");
	printf("%s\n", x);
	ram_free(x);


	ram_allocs();

	char * aa = zstrdup("Hello");

	printf("%d\n", zstr_hash(aa));

	ram_free(aa);


	//make a stringmap
	zstringmapT* map = zstringmap_mk(256);
	dumpmap(map);

	zstringmap_put(map, "A", zstrdup("aaa"));
	dumpmap(map);

	zstringmap_put(map, "B", zstrdup("bbb"));
	dumpmap(map);

	zstringmap_put(map, "C", zstrdup("ccc"));
	dumpmap(map);

	//delete one
	zstringmap_delete(map, "A");
	dumpmap(map);

	zstringmap_put(map, "D", zstrdup("ddd"));
	dumpmap(map);


	zstringmap_put(map, "C", zstrdup("ccc2"));
	dumpmap(map);





	ram_free(map);



	ram_allocs();

	return 0;
}
