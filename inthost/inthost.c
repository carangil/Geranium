#include <stdio.h>
#include <strings.h>



#include "ztypes.h"
#include "zmem.h"
#include "int.h"


int main(int argc, char** args){




	char* src = NULL;
	char* filename = "start.zz";

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
