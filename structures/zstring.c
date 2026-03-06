#include <stdio.h>
#include <stdarg.h>
#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"
#include "zstring.h"
#include "zarray.h"


#undef tracef
#define tracef(...)

//allocates space (plus null terminator) for string n bytes long
char* zstr_mk(zsize ns)
{
	char* x;
	zuint32 n = (zuint32)ns;

	x = zarray_alloc(char , n+2 );
	x[0]=0;
	zarray_use(x, 1); //terminator is here
	x[n+1]='$';  //'canary' for debugging

	return x;
}

void zstr_debug(char* a, char* label){

	printf("zstring debug %s:", label);
	if (a == NULL)
		printf("string '' is NULL\n");
	else if(!ram_shadow(a))
		printf("string '%s' is not a zstring\n", a);
	else {
		int size = zarray_size(a);
		printf("string '%s' is zstring... char zarray of size %d, %d used\n", a, size, zarray_count(a));

		size_t len = strlen(a);	

		printf(" end of string: %x:%c %c %x:%c   end of array: %x %x:%c\n", 
			a[len-1], a[len-1], a[len], a[len+1], a[len+1], a[size-2], a[size-1], a[size-1]);

	}

}

char* zstrndup(char* a, zsize n) {

	char *z;
	
	if (a == NULL)
        	return zstr_mk(ZSTRING_INITSIZE);
   
	if (n == ZSTRING_ALL) {
		n = strlen(a);
	}
	
	z = zstr_mk(n);  //string for n chars

	if (z){
		
		strncpy(z, a, n);
		z[n]='\0';
		zarray_use(z, (zuint32)strlen(z) + 1);  //include terminator in byte count of array
	}
	
	return z;
	
}
	//copies  src[start] up to, not including, src[start+count]  to the end of dest;
	//if count ==-1, it copies to the end of the string
	
	/*	dest		src		what
	 * null			null	null
	 * 1ref			null	dest
	 * 1ref			strchr	dest+str
	 *
	 *
	 * */



//dest is NULL or a zstring
//src is a cstring or zstring or null
//dest returned has only reference (makes copy if needed for that to be true)

char* zstrcatsub(char* dest, char* src, zsize start, zsize count){

	if (count == ZSTRING_ALL){
		if (src)
			count = ((zuint32)strlen(src)) - start;
		else
			count=0;
	}

	int newsize=0;

	if (dest) {

		if (!zarray_space(dest, count+2)){
			tracef("zstringcatsub 3 :  need to resize \n");
			newsize = count +2 + zarray_count(dest);

			if (newsize < zarray_count(dest)*2){
				tracef("zstringcatsub 4:  need to resize 2x \n");
				//if newsize is less than double, then double
				newsize = zarray_count(dest)*2;

			}

		}

	} else {
		tracef("zstringcatsub 5:  dest is null, need to copy src \n");
		return zstrndup(src+start, count);
	}

	if (dest && (ram_numrefs(dest) != 1) ) {

		tracef("zstringcatsub 6:  dest has %d references, need to copy \n", ram_numrefs(dest));

		//more than one reference, make a copy

		//if we don't have a grow size, then use whatever size original had'
		if (!newsize){

			tracef("zstringcatsub 7:  dest didn't need resize, so use its original size\n");
			newsize = zarray_size(dest); //copy will be size of original buffer *
		}

		char* newstr = zstr_mk( newsize);

		if (newstr){
			int pos = zarray_count(dest);

			//copy original
			memcpy( newstr, dest, pos);
			zarray_use(newstr, pos);
		}

		ram_free(dest);
		tracef("dec dest ref %d\n", ram_numrefs(dest));
		dest = newstr;

	} else if (newsize){
		tracef("zstringcatsub 8:  dest is resizing\n");
		dest = zarray_resize(dest, newsize, NULL);
	}

	int pos = zarray_count(dest);

	if (pos>0)
	    pos--;


	if (src == NULL){
		printf("src is null\n");
	}
	memcpy(dest+pos, src+start, count);
	dest[pos+count]=0;
	zarray_use(dest, (zuint32) pos+count+1); //include terminator in byte count
	
	return dest;
}


void zstr_reset(char* s) {
	s[0] = 0;
	zarray_use(s, 1);
}


char* zstrdup2(char* left, char* right) {
	char* ns = zstrndup(left, strlen(left) + strlen(right));
	ns = zstrcat(ns, right);
	return ns;
}

//split string into vector of string, by delim.
//if delim not found, returns vector of 1 string
zvecT*  zstrsplit(zvecT* v, char* str, char delim){
	char* p;
	
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

char* zstrbuild(zvecT* v, char delim){
	size_t sz=0;
	zuint32 i;
	char dl[2];
	dl[0]=delim;
	dl[1]=0;

	if (v==NULL)
		return NULL;

	if (delim)
		sz += zvec_count(v);

	for (i=0;i<zvec_count(v);i++){
		if ( zvec_elements(v)[i])
			sz += strlen(zvec_elements(v)[i]);
	}
	char* str = zstr_mk(sz);	//make string of this length
	
	for (i=0;i<zvec_count(v);i++){

		if ((i>0) && delim)
			str = zstrcat(str, dl);
		//printf("%s\n", str);

		if ( zvec_elements(v)[i])
			str = zstrcat(str, zvec_elements(v)[i]);
		//printf("%s\n", str);
	}

	return str;

}

//Like printf.  Initial is optional zstring to append to.
char* zstrprintf(char* initial, char* format, ...) {
	va_list args, copy;
	va_start(args, format);
	va_copy(copy, args);

	int offset = 0;
	int s = vsnprintf(NULL, 0, format, copy);
	char* str;

	if (initial == NULL) {
		str = zstr_mk(s);
	} else {
		//printf(" %d count, %d size, %d more\n", zarray_count(initial), zarray_size(initial), s);

		if (ram_numrefs(initial) > 1){
			errorf("Appending by zstrprintf to zstring with more than 1 ref is not supported!");
		}
		
		offset = zarray_count(initial) - 1; 

		if (!zarray_space(initial, s)) {
			int olds = zarray_size(initial);
			
			int ns = olds * 2;  //double size 
			if (ns < (   zarray_count(initial) +s  ) ) {  //if not enough, 
				ns = zarray_count(initial) + s  ;   //exactly size
				offset = zarray_count(initial) - 1; //offset will overwrite the null terminator
				if (offset < 0) {
					printf("string error: element count should be at least 0 (for the terminator-1) it is %d\n", offset);
					offset = 0;
				}
 			}

			str = zarray_resize(initial, ns, NULL);

		}
		else
			str = initial;

	}

	vsnprintf(str+offset, s + 1, format, args);

	zarray_use(str, offset + s + 1);

	va_end(args);
	va_end(copy);

	return str;
}


//a very common string hash function
zuint32 zstr_hash(char* s){

	unsigned int hash=0;
	while (*s){
			hash=hash*31 + (((unsigned int)(*s)) & 0xFF);
			s++;
	}

	return hash;
}
