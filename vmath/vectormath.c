#include "ztypes.h"
#include <math.h>
#include "zvectormath.h"
#include <stdlib.h>

//normalize a vector
void vec3normalize( vec3* p)
{
	float d = sqrtf( vec3abs_sq(*p)  );
	vec3scale( *p, (1/d) );
}


zbool vec3_point_in_box( vec3* min, vec3* point, vec3* max, float border)
{
	int j;

	for (j=0;j<3;j++)
	{

		if (point->array[j]- border < min->array[j])
			return ZFALSE;


		if (point->array[j] + border > max->array[j])
			return ZFALSE;

	}

	return ZTRUE;

}




