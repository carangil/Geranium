#include <stdio.h>
#include "../ztypes.h"
#include "../memory/zmem.h"
#include "zvector.h"
#include "zstring.h"



//increases storage of a string to accomodate extra space
char* zstr_grow(char* dest, int additional) {
	
	int xlen;
	int newcap;
	
	zstring_shadow_t* sh = ram_shadow(dest);
	
	if (!sh) {
		printf(" Cannot grow non-shadow string\n");
		return NULL;
	}
	
	xlen = additional + sh->len + 1; 
	
	if (xlen >= sh->capacity ) {
		
		
		//try doubling capacity
		newcap = sh->len * 2;
		
		//if not good enough, at least fit what we have
		if (newcap <  xlen)
			newcap = xlen;
				
		dest = ram_resize(dest, newcap);
		if (dest == NULL)
			return NULL;// resize failed
		
		sh = ram_shadow(dest); //follow the shadow buffer to the new location
		sh->capacity = newcap; 
	
	}
	
	return dest;
}


char* zstr_cat(char* dest, char* src)
{
	int srclen;
	int newcap;
	zstring_shadow_t* sh;
	
	
	srclen = strlen(src);
	
	dest = zstr_grow(dest, strlen(src) );
	
	if (!dest)
		return NULL;
	
	sh = ram_shadow(dest);
	
	if (!sh)
		printf(" WHERE DID THE SHADOW GO?\n");
	
			
	//it should now fit
	if (dest[sh->len] != '\0') {
		printf("NULL TERMINATOR MISSING IN TARGET STRING... THIS IS BAD\n");
	}
	
	strcpy(dest + sh->len , src);
	sh->len += srclen;
	
	return dest;
	
}



//0 capacity means copy all 
char* zstr_mk(int capacity, int use)
{
	char* x;
	zstring_shadow_t* zs;
	
	if (use > capacity)
		capacity = use;
	
	if (capacity < 0)
		return NULL;
	
	capacity ++; //null terminator
	
	x = ram_alloc_shadow( capacity, NULL, sizeof(zstring_shadow_t));
	
	zs = ram_shadow(x);
	
	if (zs) {
		zs->len = use;
		zs->capacity = capacity ; //, including null terminator
	}
	
	return x;
}


char* zstrndup(char* a, int n) {
	int len;
	char *z;
	
	if (n==0)
		len=0;
	else if (n>0) 
		len = strnlen(a, n);
	else 
		len= strlen(a);
	
	if (n < len)
		n = len;
	
	
	z = zstr_mk(n, len); //n is capacity of string, len is length to be used
	printf("mk %d, %d\n", len,n);
	
	if(z) {
		strncpy(z,a,len); 
		z[len]='\0';
	}
	return z;
	
}




void zstr_debug(char* x) {
	zstring_shadow_t* sh;
	
	if (x == NULL) {
		printf(" %p is null\n", x);
		return;
	}
	
	sh = ram_shadow(x);
	if (!sh) {
		printf(" %p is c string %s\n", x, x);
		return;
	}
//	printf("len:%d\n", sh->len);
	//printf("capacity:%d\n", sh->capacity);
	
	printf(" %p has shadow %p: %d sh->len    %d strlen     %d capacity %s\n", x, sh, sh->len, strlen(x) ,  sh->capacity,   x);
	
}


//split string into vector of string, by delim.
//if delim not found, returns vector of 1 string
zvec_t*  zsplit(zvec_t* v, char* str, char delim){
	char* p;
	int len;
	char* tmp;
	
	//create a vector is none was specified (otherwise we are appending to existing one)
	if (v == NULL)
		v = zvec_mk(NULL, 4);
	
	if (!v)
		return NULL;
	
	
	for (;;) {
		
		p= strchr(str, delim);
	//	printf("start str is %s, p is %s\n", str, p);

		if (!p) {
			//no delims left
			zvec_add_or_free(v, zstrndup(str, ZSTRING_ALL));
			break;
		}
	
		zvec_add_or_free(v, zstrndup(str, p-str));
		
		str = p+1;
		//printf("end str is %s, p is %s\n", str, p);
	}
	return v;
	
}
