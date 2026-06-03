#include <stdio.h>
#include <strings.h>

#include <ctype.h>
#include "int.h"


int scanmain(int argc, char** args);

//set any platform-specific constants
void set_platform_constants();

int main(int argc, char** args){

	//scanmain is in hscan.c
	//hscan is used to make inc files for C functions

	if (argc>1 && !strcmp(args[1], "-scan"))
		return scanmain(argc-1, args+1);


	char* src = NULL;
	char* filename = "start.src";  //default program to start

	if (argc > 1){
		filename = args[1];
	}

	set_platform_constants();

	src = ram_loadstr(filename);
	if (src){
		int_run_str(src, filename);
		ram_free(src);
	} else {
			errorf("Cannot load %s\n", filename);
	}

	ram_allocs();

	return 0;
}
