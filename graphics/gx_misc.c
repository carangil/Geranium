// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

//This is where to put random graphics stuff that doesn't have enough stuff to give it its own file




#include "../ztypes.h"
#include "../vmath.h"

#include "glstuff.h"

#include "gx_sys.h"
#include <math.h>"
#include <float.h>

void gx_camera_pos(float x, float y, float z)
{
	glLoadIdentity();
	glTranslatef(-x,-y,-z);

}

void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis)
{
	zfloat32 matr[]={
					     xaxis->vec3x, yaxis->vec3x, -zaxis->vec3x,0,
					     xaxis->vec3y, yaxis->vec3y, -zaxis->vec3y,0,
					     xaxis->vec3z, yaxis->vec3z, -zaxis->vec3z,0,
					     0,0,0,1};
	glLoadIdentity();
	
	glMultMatrixf((float*)&matr);
				
	glTranslatef( -position->vec3x, -position->vec3y, -position->vec3z);
}

//spin crap
void gx_spin(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward)
{

	//roll

	if (roll != 0.0)
	{
		//add a little bit of the right vector to the up vector:
		
		vec3madd(*up, roll, *right);

		//make the new up vector unit-length
		vec3scale(  *up,  1.0/  sqrt( vec3abs_sq( *up ) ) ); 
		
	//cross product to give new right vector
		vec3cross( *right, *forward, *up  );
	}

	//yaw
	if (yaw != 0.0)
	{
		//add some 'right' to 'forward'
		vec3madd( *forward, yaw, *right);

		//make forward unit-length
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake right vector;
		vec3cross( *right, *forward, *up  );
	}

	if (pitch != 0.0)
	{
		//add some 'up' to the forward vector
		vec3madd( *forward, pitch, *up);

		//normalize the new forward vector
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake the up vector
		vec3cross( *up, *right, *forward  );
	}


}
