#include "int.h"


void set_platform_constants(){

	int_add_c_val32("SEEK_SET", SEEK_SET);
	int_add_c_val32("SEEK_CUR", SEEK_CUR);
	int_add_c_val32("SEEK_END", SEEK_END);

	//can also set struct sizes and member offsets (just makes the integer number available to program, doesn't yet modift the type itself)
	//INT_STRUCT_SIZE( tokenT);
	//INT_STRUCT_MEMBER(tokenT, zlistnode);
	//INT_STRUCT_MEMBER(tokenT, str);

}

