// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2017 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

// This contains a partial replacement for the legacy openGL matrix stack.

#define GFXINTERNAL
#include "gfx_gl.h"

#include "math.h"

#include <stdio.h>






void printMatrix44(char* name, float* m);

//rotate a matrix relative to itself
//
// object matrices define the identity matrix as no rotation.
// when using a matrix for a camera, the Z axis is taken to be the opposite of the LOOK direction, because the camera points to -1 direction in opengl

//when used on a camera, and yaw/pitch values are from mouse motion, and 'roll' is input from some other user control, 
//this has the effect of the 'Descent' spaceship rotation system....  If forward, back, left, right, up, down strafe/slide controls are 
//also then used to add weighted amounts of the 'forward' 'up' and 'right' vectors to the camera position, you get full 6DOF
//there are not quaternions or other things used here, this is all just straight-up 3D vector math

//Zdef proc gfx_spin_matrix Spin3x3
void gfx_spin_matrix(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gfx_mat_3x3* rot)
{

 	vec3* up = &rot->y_axis.vec3;
	vec3* right = &rot->x_axis.vec3;
	vec3* forward = &rot->z_axis.vec3;

	
	
	//roll
	//add a little bit of the right vector to the up vector:
		
		vec3madd(*up, roll, *right);

		//make the new up vector unit-length
		vec3scale(  *up,  1.0f/  sqrtf( vec3abs_sq( *up ) ) ); 
		
		//cross product to give new right vector

		
		vec3cross( *right, *up, *forward  );
		
		


	//yaw
	//add some 'right' to 'forward'
		vec3madd( *forward, yaw, *right);

		//make forward unit-length
		vec3scale(  *forward,  1.0f/  sqrtf( vec3abs_sq( *forward ) ) ); 

		//remake right vector;
		
		vec3cross( *right, *up, *forward  );
		
	
	//pitch
	//add some 'up' to the forward vector
		vec3madd( *forward, pitch, *up);

		//normalize the new forward vector
		vec3scale(  *forward,  1.0f/  sqrtf( vec3abs_sq( *forward ) ) ); 

		//remake the up vector
				
		vec3cross( *up, *forward, *right  );
			
}




void gfx_trans_init(gfx_transformT* t)
{
	if (t)
	{
		vec4set(t->rot.x_axis, 1.0f, 0.0f, 0.0f, 0.0f);
		vec4set(t->rot.y_axis, 0.0f, 1.0f, 0.0f, 0.0f);
		vec4set(t->rot.z_axis, 0.0f, 0.0f, 1.0f, 0.0f);
		vec4set(t->pos, 0.0f, 0.0f, 0.0f, 1.0f);
	}
}
void gfx_camera_init(gfx_cameraT * t) { //will later add the ability to have a starting position and starting look angle
	gfx_trans_init(t);
}
//matrix replacement for opengl fixed function
//originally there was some pass-thru to the opengl stack when fixed function is ued
//because my old graphics library actually ran on old hardware too

static gfx_transformT	modelview;
static vec3				modelview_camera_pos;	//point, in world space, where the camera is.  updated on calls to the camera, but not 

zint32 matrix_version=0;  //incremeneted whenever changed

//each shader also tracks their own matrix version


//projection matrix:
static float proj_matrix[16];

void gfx_projection3d (zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist) {

	float f = 1/tanf( fovy/180.0f*3.141f / 2);
	
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

float* gfx_get_projection_matrix() {
		return proj_matrix;
}

void gfx_projection2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom) {

	//the x axis is scaled so that left to right is mapped to a range of '2', and is offset by the middle of that range. (which is right +left)/2 AKA average.  That the has to be scaled by 2/length of the range
	//the y axis is scaled similar
	//the z axis is not adjusted at all, and stays at 0
	float matr[] =
	{
		2/(right-left),										0,									0,					0,
		0,													2 / (top - bottom),					0,					0,
		0,													0,									-1,					0,										
		-(right + left) / (right - left),					-(top + bottom) / (top - bottom),	0,					1
	};
	

	memcpy(proj_matrix, matr, sizeof(matr));


	matrix_version++;

}

/* transfers the current matrix to opengl
 * Either through glLoadMatrix for fixed function
 * OR as a uniform when we are doing shaders in the future 
 */

void gxi_refresh_matrix(gx_shader_variantT* shader ) {

	
	
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
			
	//printMatrix44("modelview", matr);
	

	if (!shader) {	
		ff_update_matrix(proj_matrix, &matr);
		printf("updated ff\n");
		return;
	}
	
	
	if (shader->modelview_uloc != -1) {
	
		gxdtracef(" upload shader matrix to ver %d\n", matrix_version);
		checkGL();
		glUniformMatrix4fv(shader->modelview_uloc, 1, 0, matr);	
		checkGL();
		glUniformMatrix4fv(shader->projection_uloc, 1, 0, proj_matrix);	
		
	//	shader->matrix_version = matrix_version;
		checkGL();
		if (shader->camera_pos_uloc != -1) {
			//printf(" upload campos %d   %f %f %f\n", shader->camera_pos_uloc, modelview_camera_pos.VX,modelview_camera_pos.VY,modelview_camera_pos.VZ);
			glUniform3fv(shader->camera_pos_uloc, 1, modelview_camera_pos.array);	
		}
		
		checkGL();
		return;
	}
	
	printf(" Unhandled matrix case\n");
	exit(0);
	
}


/* replaces the current matrix with one that draws the words from the point of view of a camera
 * position: The position in world space of the camera
 * xaxis:  Vector pointing director to the right of the camera point of view
 * yaxis:  Vector pointing directly up from the camera's point of view
 * minus_zaxis:  Vector pointing in the direction the camera is looking. This is called minus Z, because in openGL -Z is the view direction.
 */


void gfx_camera_view(  gfx_cameraT* cam) {

	gfx_transformT trans;


	if (cam) {

		/* top part of this matrix is 3x3 matrix.  It is transpose of the camera's matrix. */
		
		vec4set(trans.rot.x_axis,  cam->rot.x_axis.VX, cam->rot.y_axis.VX, cam->rot.z_axis.VX,0);
		vec4set(trans.rot.y_axis,  cam->rot.x_axis.VY, cam->rot.y_axis.VY, cam->rot.z_axis.VY,0);
		vec4set(trans.rot.z_axis,  cam->rot.x_axis.VZ, cam->rot.y_axis.VZ, cam->rot.z_axis.VZ,0);

		/* bottom part is translation */
		/* the dot products project the position into the camera space */

		vec3set (trans.pos , -vec3dot(cam->pos, cam->rot.x_axis), -vec3dot(cam->pos, cam->rot.y_axis), -vec3dot(cam->pos, cam->rot.z_axis));
		trans.pos.named.w = 1;

		modelview_camera_pos = cam->pos.vec3;  //track the camera position (will need it internally for lighting)
		

		gfx_load_transform(&trans);  /* replace the current matrix */
	}
	else {
		gfx_identity();
		vec3set(modelview_camera_pos, 0.0, 0.0, 0.0);
	}
}




void gfx_camera_motion_6dof(gfx_cameraT* cam, float forward, float right, float up, float yaw, float pitch, float roll) {
	
		
	vec3madd(cam->pos, right,	cam->rot.x_axis);
	vec3madd(cam->pos, up,		cam->rot.y_axis);
	vec3madd(cam->pos,-forward,	cam->rot.z_axis);  //negative because -z is the look direction

	gfx_spin_matrix(yaw, pitch, roll, &cam->rot);

}

int trans_debug = 1;
#define debugf  if (trans_debug) printf

void gfx_save_transform(gfx_transformT* s) {
	*s = modelview;
}

void gfx_identity(){
	
	vec4set(modelview.rot.x_axis, 1, 0, 0, 0);	// x axis inialized to 1,0,0
	vec4set(modelview.rot.y_axis, 0, 1, 0, 0); // y axis inialized to 0,1,0
	vec4set(modelview.rot.z_axis, 0, 0, 1, 0); // z axis inialized to 0,0,1
	vec4set(modelview.pos,    0, 0, 0, 1);			//no translation

	matrix_version++;
	
}

void transpose( gfx_mat_3x3* dst, gfx_mat_3x3* src) {

	/* transpose the 3x3 section of the transform */
	
	vec3set( dst->x_axis,  src->x_axis.VX, src->y_axis.VX, src->z_axis.VX);
	vec3set( dst->y_axis,  src->x_axis.VY, src->y_axis.VY, src->z_axis.VY);
	vec3set( dst->z_axis,  src->x_axis.VZ, src->y_axis.VZ, src->z_axis.VZ);
}


//transforms a point by the modelview matrix
//these functions are not particularly fast, and are here for convenience
//sometimes we need to know what a point will be transformed to by the hardware
//consider cacheing the transposed matrix if this is too slow for some reason
void gfx_trans_vec3(vec3* po) {
	gfx_mat_3x3 t;
	vec3 p;
	
	transpose (&t, &modelview.rot);  //transpose the modelview matrix
		
	vec3set(p, vec3dot(t.x_axis, *po), vec3dot(t.y_axis, *po), vec3dot(t.z_axis, *po));
	
	vec3add(p, modelview.pos);
	
	*po = p;
}

//transforms a direction by the modelview matrix (no translation... for normals)
void gfx_trans_dir_vec3(vec3* po) {
	gfx_mat_3x3 t;
	vec3 p;
	
	transpose (&t, &modelview.rot);  //transpose the view matrix
		
	vec3set(p, vec3dot(t.x_axis, *po), vec3dot(t.y_axis, *po), vec3dot(t.z_axis, *po));
		
	*po = p;
}


void gfx_translate(vec3* delta) {

	gfx_mat_3x3 t;
	vec3 p;

	transpose (&t, &modelview.rot);  //transpose the view matrix

	/* the amount we are translating by needs to be projected by the current rotation axes */
	vec3set(p, vec3dot(t.x_axis, *delta), vec3dot(t.y_axis, *delta), vec3dot(t.z_axis, *delta));

	vec3add( modelview.pos, p);

	matrix_version++;
	
}

void gfx_translate3(float x, float y, float z){
	vec3 p;
	vec3set(p, x, y, z);
	gfx_translate(&p);
	
}


#if 0
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
#endif

#if 1
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

#endif


void gfx_rotate_3x3 (gfx_mat_3x3* rot) {

	//float r[16];
	//float m[16];

	gfx_mat_3x3 n;
	gfx_mat_3x3 t;

	/*
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
	*/
	

	transpose(&t, &modelview.rot); 

	vec3set(n.x_axis,  vec3dot(rot->x_axis, t.x_axis) , vec3dot(rot->x_axis, t.y_axis) , vec3dot(rot->x_axis, t.z_axis));
	vec3set(n.y_axis,  vec3dot(rot->y_axis, t.x_axis) , vec3dot(rot->y_axis, t.y_axis) , vec3dot(rot->y_axis, t.z_axis));
	vec3set(n.z_axis,  vec3dot(rot->z_axis, t.x_axis) , vec3dot(rot->z_axis, t.y_axis) , vec3dot(rot->z_axis, t.z_axis));

	modelview.rot = n;
		
	
	matrix_version++;
}




void gfx_load_transform(gfx_transformT* trans) {

	modelview = *trans;

	matrix_version++;
}



void gfx_blend_transform(float a, float b, gfx_transformT* trans) {


	
	vec3scale(modelview.pos, a);
	vec3scale(modelview.rot.x_axis, a);
	vec3scale(modelview.rot.y_axis, a);
	vec3scale(modelview.rot.z_axis, a);

	vec3madd(modelview.pos, b, trans->pos);
	vec3madd(modelview.rot.x_axis, b, trans->rot.x_axis);
	vec3madd(modelview.rot.y_axis, b, trans->rot.y_axis);
	vec3madd(modelview.rot.z_axis, b, trans->rot.z_axis);
	



}


void gfx_rotate_y(float rad) {
	
	gfx_mat_3x3 rot;

	
	vec3set(rot.x_axis, cosf(rad), 0,-sinf(rad));
	vec3set(rot.y_axis, 0, 1, 0);
	vec3set(rot.z_axis, sinf(rad), 0,cosf(rad));

	gfx_rotate_3x3(&rot);
	
	matrix_version++;
}

void gfx_rotate_z(float rad) {
	
	gfx_mat_3x3 rot;

	
	vec3set(rot.x_axis, cosf(rad), sinf(rad),0);
	vec3set(rot.y_axis, -sinf(rad), cosf(rad),0);
	vec3set(rot.z_axis, 0, 0,1);

	gfx_rotate_3x3(&rot);
	
	matrix_version++;
}


void gfx_rotate_x(float rad) {

	gfx_mat_3x3 rot;

	vec3set(rot.x_axis, 1, 0,0);
	vec3set(rot.y_axis, 0, cosf(rad), sinf(rad));
	vec3set(rot.z_axis, 0, -sinf(rad), cosf(rad));

	gfx_rotate_3x3(&rot);
	
	matrix_version++;
}

void gfx_scale3(float x, float y, float z) {

	
	vec3scale(modelview.rot.x_axis, x);
	vec3scale(modelview.rot.y_axis, y);
	vec3scale(modelview.rot.z_axis, z);
		
	
	matrix_version++;
}
