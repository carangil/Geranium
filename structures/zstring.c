#include <stdio.h>
#include "../ztypes.h"
#include "../memory/zmem.h"
#include "zvector.h"
#include "zstring.h"




//cats left and right, optionally frees either or both

char* zstrcat(char* left, char* right, zbool freeleft, zbool freeright) {
	
	int lsize,rsize;
	char * news = NULL;
	lsize = strlen(left);
	rsize = strlen(right);
	
	news = ram_alloc( lsize + rsize + 1, NULL);
	if (news) {
		strcpy(news, left);
		strcpy(news+lsize, right);
		if (freeleft)
			ram_free(left);
		if (freeright)
			ram_free(right);
	}
	
	return news;
}

char* zstrndup(char* str, int count) {
	char * news;	
	int len = strlen(str);
	if (len > count)
		len = count;
	
	news = ram_alloc(len+1, NULL);
	if (news){
		strncpy(news, str, len);
		news[len]= '\0';
	}
	
	return news;
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
		printf("start str is %s, p is %s\n", str, p);

		if (!p) {
			//no delims left
			zvec_add_or_free(v, ram_strdup(str));
			break;
		}
	
		zvec_add_or_free(v, zstrndup(str, p-str));
		
		str = p+1;
		printf("end str is %s, p is %s\n", str, p);
	}
	return v;
	
}
