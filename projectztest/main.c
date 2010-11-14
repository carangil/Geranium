// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include "../ztypes.h"
#include "../memory/ram.h"



void systest_main();
void graphtest_main();
void graphtest_main2();

int main(int argc, char** argv)
{

	//systest_main();  //do simple systems test
	graphtest_main();
	//graphtest_main2();

	printf("allocations left: %d\n", ram_allocs());
	return 0;
}
