#include <stdio.h>
#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"
#include "zstring.h"
#include "zarray.h"

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
        	return zstr_mk(0);
   
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
	
char* zstrcatsub(char* dest, char* src, zsize start, zsize count){

	if (dest==NULL){
		fprintf(stderr, " Can't append to null string\n");
		exit(1);
	}


	if (ram_numrefs(dest) != 1){
		fprintf(stderr, " Can't append to string with multiple references\n");
		exit(1);
		return dest;

	}

	if (src == NULL)
		return dest;

	if (count == ZSTRING_ALL){
		count = ((zuint32)strlen(src)) - start;
	}

	//zstr_debug(dest, "dest");
	//zstr_debug(src, "src");
   
	if (!zarray_space(dest, count+2)){

		int newsize = count +2 + zarray_count(dest);

		if (newsize < zarray_count(dest)*2){
			//if newsize is less than double, then double
			newsize = zarray_count(dest)*2;

		}
		dest = zarray_resize(dest, newsize, NULL);

	}

	int pos = zarray_count(dest);
	if (pos>0)
	    pos--;
	memcpy(dest+pos, src+start, count);
	dest[pos+count]=0;
	zarray_use(dest, (zuint32) pos+count+1);
	
	return dest;
}


void zstr_reset(char* s) {
	s[0] = 0;
	zarray_use(s, 1);
}

char* zstrcombine(char* left, char* right){
	char * newleft = zstrcat(left,right);
	ram_free(right);
	return newleft;
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




