#include <stdio.h>
#include <strings.h>



#include "ztypes.h"
#include "zmem.h"
#include "int.h"
#include "zstring.h"


void stringtest(){

		char* a = zstrdup("Banana");
		char* b = zstrcatsub( ram_addref(a) , "Apple", 0, 4);

		char* c = NULL;
		char* d =NULL;

		printf("a+b %s\n", a);


		a = zstrcatsub(a, "Need a longer append", 0, -1);

		printf("a+b %s\n", a);;


		c = ram_addref(a);

		d = zstrcatsub( NULL, "beebeebee", 1, 5);
		printf("a:%s  c:%s  d:%s\n", a ,c, d);

		ram_free(d);
		d = zstrcatsub( ram_addref(a), "X", 0, -1);
		d[2]= 'X';

		printf("a:%s  c:%s  d:%s\n", a ,c, d);


		ram_free(a);
		ram_free(b);
		ram_free(c);
		ram_free(d);

		ram_allocs();
		exit(1);
}

int main(int argc, char** args){

	//stringtest();

	char* src = NULL;
	char* filename = "start.src";

	if (argc > 1){
		filename = args[1];

	}

	src = ram_loadstr(filename);
	if (src){
		int_run_str(src, args[1]);
		ram_free(src);
	} else {
			errorf("Cannot load %s\n", filename);
	}


	ram_allocs();

	return 0;
}
