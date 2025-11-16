#include <stdio.h>
#include "boo.h"

#include "ztypes.h"
#include "zmem.h"

int main(int argc, char** args){

	char* a = ram_strdup("butt");

	printf("boo\n");

	boo(7, a);

	ram_free(a);

	char* x = ram_loadstr("a.c");
	printf("%s\n", x);
	ram_free(x);


	ram_allocs();

	return 0;
}
