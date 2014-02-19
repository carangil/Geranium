#include "../ztypes.h"
#include "vmath.h"
#include <stdlib.h>
#include <math.h>

void srandf(float x)
{
	int seed = x * RAND_MAX;
	int seed2 = x* 17*5;
	seed += seed2;
	seed = seed % RAND_MAX;
	srand(seed);

}

float randf()
{
	
	return  rand() /  (float)RAND_MAX;
	
}


//random float -1.0 to 1.0
float randfs()
{
	return -1.0 + 2*(rand()&511) / 511.0;
}

//normalize a vector
void vec3normalize( vec3* p)
{
	float d = sqrtf( vec3abs_sq(*p)  );
	vec3scale( *p, (1/d) );
}



