// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2017 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

// This contains a partial replacement for the legacy openGL matrix stack.


#include "../ztypes.h"
#include "../vmath/zmath.h"

#include "glheaders.h"

#include "../memory/zmem.h"
//#include "../structures/vector.h"


#include "gx_sys.h"
#include <math.h>
#include <float.h>


#include "../structures/zvector.h"
#include "../structures/zlist.h"
#include "gx_image.h"
#include "gx_buffers.h"
#include "gx_drawstyle.h"

#include "gx_trans.h"
//#include "gx_mesh.h"
//#include "gx_light.h"

#include <stdio.h>

int transmode= GX_TRANSFORM_INTERNAL_TO_GL;

void printMatrix44(char* name, float* m);

//spin crap
//FLIP switches the order
// TRUE for camera matrices, FALSE for object matrices
//
// object matrices define the identity matrix as no rotation.
// when using a matrix for a camera, the Z axis is taken to be the LOOK direction, which is along the -Z axis.  An initialized camera has the Z axis flipped

void gx_spin(zbool flip, zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gx_mat_3x3* rot)
{

	vec3* up = &rot->y_axis;
	vec3* right = &rot->x_axis;
	vec3* forward = &rot->z_axis;

	
	
	//roll

	//if (roll != 0.0)
	{
		//add a little bit of the right vector to the up vector:
		
		vec3madd(*up, roll, *right);

		//make the new up vector unit-length
		vec3scale(  *up,  1.0/  sqrt( vec3abs_sq( *up ) ) ); 
		
	//cross product to give new right vector

		if (flip)
		{
			vec3cross( *right, *forward, *up  );
		}
		else
		{
			vec3cross( *right, *up, *forward  );
		}
		
	}

	//yaw
	//if (yaw != 0.0)
	{
		//add some 'right' to 'forward'
		vec3madd( *forward, yaw, *right);

		//make forward unit-length
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake right vector;
		if (flip)
		{
			vec3cross( *right, *forward, *up  );
		}
		else
		{
			vec3cross( *right, *up, *forward  );
		}
	}

	//if (pitch != 0.0)
	{
		//add some 'up' to the forward vector
		vec3madd( *forward, pitch, *up);

		//normalize the new forward vector
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake the up vector
		if (flip)
		{
			vec3cross( *up, *right, *forward  );
		}
		else 
		{
			vec3cross( *up, *forward, *right  );
		}
	}


}

void gx_camera_init(gx_camera_t* cam)
{
	if (cam)
	{
		vec3set( cam->rot.x_axis,		1.0f, 0.0f, 0.0f);
		vec3set( cam->rot.y_axis,		0.0f, 1.0f, 0.0f);
		vec3set( cam->rot.z_axis,		0.0f, 0.0f, -1.0f);
		vec3set( cam->pos,			0.0f, 0.0f, 0.0f);
	}
}


//matrix replacement 

typedef struct gx_transform_s {
	gx_mat_3x3 rot;		//contains rotation and possible scaling
	vec3 pos;		
} gx_transform_t;


static gx_transform_t	modelview;
static vec3			modelview_camera_pos;

static zint32 matrix_version=0;  //incremeneted whenever changed
static zint32 ff_matrix_version=-1;
//each shader also tracks their own matrix version


//projection matrix:
static float proj_matrix[16];

void gxi_trans_set_perspective_matrix (zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist) {

	float f = 1/tan( fovy/180.0*3.141 / 2);
	
	float matr[] = 
			{
				f/aspect, 0 , 0, 0,
				0, f, 0, 0,
				0, 0, (fardist+neardist)/(neardist-fardist), -1,
				0, 0, (2 * fardist * neardist) / (neardist - fardist), 0
				
			};

	memcpy(proj_matrix, matr, sizeof(matr));
	
	
	matrix_version++;	
}



/* transfers the current matrix to opengl
 * Either through glLoadMatrix for fixed function
 * OR as a uniform when we are doing shaders in the future 
 */

void gxi_refresh_matrix(gx_shader_t* shader) {

	
	/* load our 3x3 matrix and translation vector as a 4x4 matrix to opengl */
	
	if (shader && shader->matrix_version == matrix_version)  {
		gxdprintf(" Skip redundent matrix upload\n");
		return;
		
	}
	
	
	
	
	zfloat32 matr[]={
	     modelview.rot.x_axis.VX,    modelview.rot.x_axis.VY,     modelview.rot.x_axis.VZ, 0,
	     modelview.rot.y_axis.VX,    modelview.rot.y_axis.VY,     modelview.rot.y_axis.VZ, 0,
	     modelview.rot.z_axis.VX,    modelview.rot.z_axis.VY,     modelview.rot.z_axis.VZ, 0,
	     modelview.pos.VX,	      modelview.pos.VY,	modelview.pos.VZ,    1 
	};
	
	
	
	if (!shader) {
		if (ff_matrix_version == matrix_version) {
				gxdprintf("skip same ff matrix\n");
				return ;
				
		}
		gxdprintf(" upload FF matrix\n");
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(proj_matrix);

		glMatrixMode(GL_MODELVIEW);
	
		glLoadIdentity();
		glLoadMatrixf(matr);
		//printMatrix44("ff", matr);
		
		ff_matrix_version= matrix_version;
				
	}
	
	if (shader && shader->modelview_uloc != -1) {
		//ff_matrix_version = -1; //if we turn shaders off, we will have to resend the fixed function matrix

		gxdprintf(" upload shader matrix to ver %d\n", matrix_version);
		
		glUniformMatrix4fv(shader->modelview_uloc, 1, 0, matr);	
		
		glUniformMatrix4fv(shader->projection_uloc, 1, 0, proj_matrix);	
		
		shader->matrix_version = matrix_version;
		
		if (shader->camera_pos_uloc != -1) {
			//printf(" upload campos %d   %f %f %f\n", shader->camera_pos_uloc, modelview_camera_pos.VX,modelview_camera_pos.VY,modelview_camera_pos.VZ);
			glUniform3fv(shader->camera_pos_uloc, 1, modelview_camera_pos.array);	
		}
		
		
	}
	
	
}


/* replaces the current matrix with one that draws the words from the point of view of a camera
 * position: The position in world space of the camera
 * xaxis:  Vector pointing director to the right of the camera point of view
 * yaxis:  Vector pointing directly up from the camera's point of view
 * minus_zaxis:  Vector pointing in the direction the camera is looking. This is called minus Z, because in openGL -Z is the view direction.
 */


void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* minus_zaxis) {
	gx_mat_3x3 trans;
	vec3 offset;

	if (position && xaxis && yaxis && minus_zaxis) {

		/* top part of this matrix is 3x3 matrix.  It is transpose of the camera's matrix.  Sign is flipped on Z because the look direction is along -Z axis. */
		
		vec3set(trans.x_axis,	xaxis->vec3x, yaxis->vec3x, -minus_zaxis->vec3x);
		vec3set(trans.y_axis,	xaxis->vec3y, yaxis->vec3y, -minus_zaxis->vec3y);
		vec3set(trans.z_axis,	xaxis->vec3z, yaxis->vec3z, -minus_zaxis->vec3z);

		/* bottom part is translation */
		/* the dot products project the position into the camera space */

		vec3set (offset, -vec3dot(*position, *xaxis), -vec3dot(*position, *yaxis), vec3dot(*position, *minus_zaxis));

		modelview_camera_pos = *position;
		
		gx_load_transform(&trans,&offset);  /* replace the current matrix */
	} 
}


int trans_debug = 1;
#define debugf  if (trans_debug) printf

void gx_identity(){
	
	if (transmode == GX_TRANSFORM_GL) {
		debugf("glLoadIdentity\n");
		glLoadIdentity();
	} else {

		vec3set(modelview.rot.x_axis, 1, 0, 0);
		vec3set(modelview.rot.y_axis, 0, 1, 0);
		vec3set(modelview.rot.z_axis, 0, 0, 1);
		vec3set(modelview.pos,    0, 0, 0);

		matrix_version++;
		
	}
}



void transpose( gx_mat_3x3* dst, gx_mat_3x3* src) {

	/* transpose the 3x3 section of the transform */

	vec3set( dst->x_axis,  src->x_axis.VX, src->y_axis.VX, src->z_axis.VX);
	vec3set( dst->y_axis,  src->x_axis.VY, src->y_axis.VY, src->z_axis.VY);
	vec3set( dst->z_axis,  src->x_axis.VZ, src->y_axis.VZ, src->z_axis.VZ);

}


 //transforms a point by the modelview matrix
void gx_trans_vec3(vec3* po) {
	gx_mat_3x3 t;
	vec3 p;
	
	transpose (&t, &modelview.rot);  //transpose the view matrix
		
	vec3set(p, vec3dot(t.x_axis, *po), vec3dot(t.y_axis, *po), vec3dot(t.z_axis, *po));
	
	vec3add(p, modelview.pos);
	
	*po = p;
}

//transforms a direction by the modelview matrix
void gx_trans_dir_vec3(vec3* po) {
	gx_mat_3x3 t;
	vec3 p;
	
	transpose (&t, &modelview.rot);  //transpose the view matrix
		
	vec3set(p, vec3dot(t.x_axis, *po), vec3dot(t.y_axis, *po), vec3dot(t.z_axis, *po));
	
	
	
	*po = p;
}



void gx_translate(vec3* delta) {

	gx_mat_3x3 t;
	vec3 p;

	if (transmode == GX_TRANSFORM_GL) {
		debugf("glTranslatef");
		glTranslatef(delta->VX, delta->VY, delta->VZ);

	} else {
		transpose (&t, &modelview.rot);  //transpose the view matrix

		/* the amount we are translating by needs to be projected by the current rotation axes */
		vec3set(p, vec3dot(t.x_axis, *delta), vec3dot(t.y_axis, *delta), vec3dot(t.z_axis, *delta));

		vec3add( modelview.pos, p);

		matrix_version++;
	}
}

void gx_translate3(float x, float y, float z){
	vec3 p;
	vec3set(p, x, y, z);
	gx_translate(&p);
	
}

/* a function to help with debugging
 * If I suspect one of the transforms in this file is wrong, I can make opengl do the transform, and copy the result back into our modelview struct. */

 
void getmatrix(){
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	vec3set(modelview.rot.x_axis, m[0],m[1],m[2]);
	vec3set(modelview.rot.y_axis, m[4],m[5],m[6]);
	vec3set(modelview.rot.z_axis, m[8],m[9],m[10]);
	vec3set(modelview.pos, m[12],m[13],m[14]);

}


void printMatrix44(char* name, float* m){
	int i;
	gxdprintf("[ %s ", name);
	for (i=0;i<16;i++){
		if (! (i&3) ) 
			gxdprintf("\n");
		gxdprintf("%f ", m[i]);
	}
	gxdprintf(" ]\n");
}




void gx_rotate_3x3 (gx_mat_3x3* rot) {

	float r[16];
	float m[16];

	gx_mat_3x3 n;
	gx_mat_3x3 t;

	if (transmode == GX_TRANSFORM_GL) {

		debugf("glMultMatrix");
		//glGetFloatv(GL_MODELVIEW_MATRIX, m);
		//printMatrix44("before rot", m);

		r[0]= rot->x_axis.VX; r[1]= rot->x_axis.VY; r[2]= rot->x_axis.VZ; r[3]=0;
		r[4]= rot->y_axis.VX; r[5]= rot->y_axis.VY; r[6]= rot->y_axis.VZ; r[7]=0;
		r[8]= rot->z_axis.VX; r[9]= rot->z_axis.VY; r[10]= rot->z_axis.VZ; r[11]=0;
		r[12]=0;r[13]=0;r[14]=0;r[15]=1;

		glMultMatrixf(r);
		//glGetFloatv(GL_MODELVIEW_MATRIX, m);
		//printMatrix44("after rot", m);
		getmatrix();

	} else {

		transpose(&t, &modelview.rot); 

		vec3set(n.x_axis,  vec3dot(rot->x_axis, t.x_axis) , vec3dot(rot->x_axis, t.y_axis) , vec3dot(rot->x_axis, t.z_axis));
		vec3set(n.y_axis,  vec3dot(rot->y_axis, t.x_axis) , vec3dot(rot->y_axis, t.y_axis) , vec3dot(rot->y_axis, t.z_axis));
		vec3set(n.z_axis,  vec3dot(rot->z_axis, t.x_axis) , vec3dot(rot->z_axis, t.y_axis) , vec3dot(rot->z_axis, t.z_axis));

		modelview.rot = n;
		
	}
	matrix_version++;
}




void gx_load_transform(gx_mat_3x3* m, vec3* p){
	if (m)
		modelview.rot = *m;
	else
		gx_identity();


	if(p)
		modelview.pos = *p;
	else
		vec3set(modelview.pos, 0,0,0);


	matrix_version++;
}

void gx_rotate_y(float rad) {
	float m[16];
	gx_mat_3x3 rot;

	if (transmode == GX_TRANSFORM_GL) {
		glRotatef(rad/DEGREE, 0, 1, 0);
		debugf("glRotatef Y");
		getmatrix();
	} else {
		vec3set(rot.x_axis, cos(rad), 0,-sin(rad));
		vec3set(rot.y_axis, 0, 1, 0);
		vec3set(rot.z_axis, sin(rad), 0,cos(rad));

		gx_rotate_3x3(&rot);
	}
	matrix_version++;
}

void gx_rotate_z(float rad) {
	float m[16];
	gx_mat_3x3 rot;

	if (transmode == GX_TRANSFORM_GL) {
		glRotatef(rad/DEGREE, 0, 0, 1);
		debugf("glRotatef Z");
		getmatrix();

	} else {
		vec3set(rot.x_axis, cos(rad), sin(rad),0);
		vec3set(rot.y_axis, -sin(rad), cos(rad),0);
		vec3set(rot.z_axis, 0, 0,1);

		gx_rotate_3x3(&rot);
	}
	matrix_version++;
}


void gx_rotate_x(float rad) {
	float m[16];
	gx_mat_3x3 rot;

	if (transmode == GX_TRANSFORM_GL) {
		glRotatef(rad/DEGREE, 1,0,0);
		debugf("glRotatef X");
		getmatrix();
	}
	else {
		vec3set(rot.x_axis, 1, 0,0);
		vec3set(rot.y_axis, 0, cos(rad), sin(rad));
		vec3set(rot.z_axis, 0, -sin(rad), cos(rad));

		gx_rotate_3x3(&rot);
	}
	matrix_version++;
}

void gx_scale3(float x, float y, float z) {

	if (transmode == GX_TRANSFORM_GL) {
		glScalef(x,y,z);
		debugf("glScalef Y");
		getmatrix();
	}
	else {
		vec3scale(modelview.rot.x_axis, x);
		vec3scale(modelview.rot.y_axis, y);
		vec3scale(modelview.rot.z_axis, z);
		
	}
	matrix_version++;
}
