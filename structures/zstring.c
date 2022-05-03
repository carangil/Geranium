#include <stdio.h>
#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"
#include "zstring.h"
#include "zarray.h"


#if 0
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
				
		dest = ram_resize(dest, void zstr_debug(char* a)newcap);
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

#endif

//allocates space (plus null terminator) for string n bytes long
char* zstr_mk(int n)
{
	char* x;

	x = zarray_alloc(char , n+2 );
	x[0]=0;
	zarray_use(x, 1); //terminator is here
	x[n+1]='$';  //'canary' for debugging

	return x;
}

void zstr_debug(char* a){

	if (a == NULL)
		printf("string '' is NULL\n");
	else if(!ram_shadow(a))
		printf("string '%s' is not a zstring\n", a);
	else {
		int size = zarray_size(a);
		printf("string '%s' is zstring... char zarray of size %d, %d used\n", a, size, zarray_count(a));

		int len = strlen(a);	

		printf(" end of string: %x:%c %c %x:%c   end of array: %x %x:%c\n", 
			a[len-1], a[len-1], a[len], a[len+1], a[len+1], a[size-2], a[size-1], a[size-1]);

	}

}

char* zstrndup(char* a, int n) {
	int len;
	char *z;
	
	if (a == NULL)
        	return zstr_mk(0);
   
	if (n== ZSTRING_ALL){
		len = strlen(a);
		n = len;
	} else {
		len = strnlen(a, n);
	}

	if (n < len)
		n = len;

	z = zstr_mk(n);  //string for n chars

	if (z){
		strncpy(z, a, n);
		z[n]='\0';
		zarray_count(z)=n+1; //include terminator in byte count of array
	}
	
	return z;
	
}
	//copies  src[start] up to, not including, src[start+count]  to the end of dest;
	//if count ==-1, it copies to the end of the string
	
char* zstrcatsub(char* dest, char* src, int start, int count){

	if (count == ZSTRING_ALL){
		count = strlen(src) - start;
	}
    
	if (!zarray_space(dest, count+2)){
		dest = zarray_expand(dest);
	}

	int pos = zarray_count(dest);
	
	if (pos>0)
	    pos--;

	memcpy(dest+pos, src+start, count);
	zarray_use(dest, pos+count);
	
	return dest;
}



#if 1

//split string into vector of string, by delim.
//if delim not found, returns vector of 1 string
zvecT*  zsplit(zvecT* v, char* str, char delim){
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
#endif
