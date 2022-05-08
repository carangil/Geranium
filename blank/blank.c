#include "ztypes.h"
#include "zstring.h"

char* blank_example(char* a){
	char* copy = zstrdup(a);
	copy = zstrcat(copy,a);
	return copy;
}

