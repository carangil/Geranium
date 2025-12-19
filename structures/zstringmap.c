#include "zstringmap.h"
#include "zarray.h"

#undef debugf
#define debugf(...)


typedef struct stringmapentryS{
    zlistnodeT zlistnode;
    char* key;
    void* item;
}mapentryT;


zbool mapclean(void* v){
    zstringmapT* map = v;

    //clean all items from all buckets;

    for (int b=0;b<zarray_count(map->buckets); b++){

        if (map->buckets[b]){
            for (int i=0;i<zarray_count(map->buckets[b]); i++){
                ram_free(map->buckets[b][i].key);

				if (map->own_elements)
	                ram_free(map->buckets[b][i].item);

            }
            ram_free(map->buckets[b]);
        }
    }

    ram_free(map->buckets);

    return ZTRUE;
}


zstringmapT* zstringmap_mk(int nb){


    zstringmapT* map = ram_alloc(sizeof(zstringmapT), mapclean);

    map->buckets = zarray_alloc( mapentryT* , nb);
    zarray_use(map->buckets, nb);

    zlist_init(&map->ordered); //keep a linked list of elements in order

	map->own_elements = ZTRUE; //free elements when map freed

    return map;

}

zstringmapT* zstringmap_disown(zstringmapT* map){
	map->own_elements = ZFALSE;
}

mapentryT* zstringmap_find(zstringmapT* map, char* key, zbool create) {

    unsigned int hash = zstr_hash(key);
    int numbuckets = zarray_count(map->buckets);
    unsigned int bn = hash % numbuckets;

	debugf("bucket %d\n", bn);

    if (! map->buckets[bn]){
        debugf("No bucket %d\n", bn);

        if (create) {
            debugf("Create bucket %d\n", bn);
            map->buckets[bn]= zarray_alloc(mapentryT, 8); //create the bucket
            zarray_use(map->buckets[bn],1);
            zlist_addtail(&map->ordered, &(map->buckets[bn][0].zlistnode)); //add to end of list

            return &(map->buckets[bn][0]);//return first element in bucket
        } else {

            return NULL;
        }

    }
    debugf("bucket %d exists, must search\n", bn);

    int i;
    for (i=0; i<zarray_count(map->buckets[bn]);i++){
        debugf("compare %d/%d/%d  %s %s\n", i, zarray_count(map->buckets[bn]), zarray_size(map->buckets[bn]), map->buckets[bn][i].key, key);
        if ( map->buckets[bn][i].key && !strcmp( map->buckets[bn][i].key, key )){
			debugf("found existing entry\n");
            return &(map->buckets[bn][i]);
        }
    }

	if (!create){
		debugf("skipping create\n");
		return NULL;
	}


    int len = zarray_count(map->buckets[bn]);
    debugf("creating new entry for %s at position %d\n", key, len);
    map->buckets[bn] = zarray_more( map->buckets[bn], 1, NULL); //make sure there's space for 1 more
    zarray_use(map->buckets[bn], len+1); //reset the sizeof
    zlist_addtail(&map->ordered, &(map->buckets[bn][len].zlistnode)); //add to end of list



    return &(map->buckets[bn][len]);


}


//creates an entry.  Replaces an existing entry.
zbool  zstringmap_put(zstringmapT* map, char* key, void* value){

    if (!key || !value)
        return ZFALSE;
debugf("put start %s\n", key);
    mapentryT* e = zstringmap_find(map, key, ZTRUE);
    if (!e){
        errorf("Cannot add map %p key %s\n", map, key);
		exit(1);
        return ZFALSE;
    }

    if (e->key){ //if replacing item
            ram_free(e->key);

			if (map->own_elements)
	            ram_free(e->item);
    }

	debugf("copying key\n");
    e->key = zstrdup(key);


    e->item = value;
	debugf("put end %s\n", key);
    return ZTRUE;

}



void*  zstringmap_get(zstringmapT* map, char* key){

    if (!key)
        return NULL;

    mapentryT* e = zstringmap_find(map, key, ZFALSE);
    if (!e){
        debugf("not found %s\n", key);
        return NULL;
    }

    return e->item;

}

//for now just remove it
void zstringmap_delete(zstringmapT* map, char* key){

    mapentryT* e = zstringmap_find(map, key, ZFALSE);
    if (!e){
        errorf("Cannot find to delete\n");
        return;
    }

   zlist_remove_mid(&e->zlistnode); //remove from the list

	if (map->own_elements)
	    ram_free(e->item);

    ram_free(e->key);
    e->key=ram_strdup("DELETED");
    e->item=NULL;

}

//Iterates all items.  'cursor' is a pointer to a NULL void* to get the first item.
//Additional calls to zstringmap_nextkey will return later items.  State of iteration
//is tracked by this function storing temp values in *cursor.
//function returns ZFALSE for null map, empty map, or when no more values
//if ZTRUE is returned, key and value will point to valid data.
//No additional reference counts are added.
//Result is undefined if there are additions or deletions to the map while iterating.
//ex:   void* cursor = NULL;
//      while( zstringmap_nextkey(map, &key, &value, &cursor)){ ... }
zbool zstringmap_nextkey( zstringmapT* map, char** key, void* vitem, void** cursor){

	void** item = vitem;

    mapentryT* e = NULL;


    if (!map){
        return ZFALSE;
    }


    if (*cursor == NULL){
        e = zlist_head(&map->ordered);
    } else {
        e = *cursor;
        e = zlist_next(e);
    }

    if (!e){
		debugf("return empty\n");
        return ZFALSE;
    }

	debugf(" key is %s\n", e->key);
    *key = e->key;
    *item = e->item;
    *cursor = e;

    return ZTRUE;
}
