#include <stdio.h>
#include "boo.h"

#include "ztypes.h"
#include "zmem.h"


#include "zarray.h"
#include "zstring.h"
#include "zstringmap.h"


void dumpmap(zstringmapT* map){

	char* key=NULL;
	char* value=NULL;
	int cursor=0;

	printf(">>");
	while( zstringmap_nextkey(map, &key, &value, &cursor)){
			printf("(%s:%s)->", key, value );
		//	printf("(%s:%s/%s)->", key, value, zstringmap_get(map, key) );
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
	zstringmapT* map = zstringmap_mk(1);
	zstringmap_disown(map);
	dumpmap(map);

	zstringmap_put(map, "a", "apple");
	zstringmap_put(map, "b", "bbb");
	zstringmap_put(map, "c", "ccc");
	zstringmap_put(map, "d", "ddd");
	zstringmap_put(map, "e", "eee");
	zstringmap_put(map, "fe", "ffeee");
	zstringmap_put(map, "ge", "eege");
	zstringmap_put(map, "h", "hheee");
	zstringmap_delete(map, "e");


	dumpmap(map);
	ram_free(map);


	//test some appends

#if 0
	int* y = zarray_alloc(int, 3);

	zarray_append(y, 1);
	zarray_append(y, 2);

	zarray_append(y, 3);
	y=zarray_more(y,2,NULL);

	zarray_append(y,4);

	printf(" %d/%d\n", zarray_count(y), zarray_size(y));
	for (int i=0;i< zarray_count(y); i++){
			printf("%d\n", y[i]);
	}


#endif



	ram_allocs();

	return 0;
}
