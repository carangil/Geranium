// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.




typedef struct gx_mat_3x3_s {
	vec3 x_axis;  /*RIGHT vector */
	vec3 y_axis;  /*UP vector*/
	vec3 z_axis;  /* Forward Vector (-z) */
} gx_mat_3x3;

//
//rotates 3 vectors around each other
void gx_spin(zbool is_camera, zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gx_mat_3x3* rot);
//
typedef struct gx_camera_s {
	vec3		pos;
	gx_mat_3x3	rot;

	/* Note: camera rotation matrices are different than object matrices. 
	 *
	 *  A nonrotated object has an identity matrix of:
	 *  1 0 0
	 *  0 1 0
	 *  0 0 1
	 *
	 *  A camera is defined by 3 vectors, the right vector (x_axis), up vector (y_axis), and the forward vector (-zaxis).
	 *  Note the negative.  Opengl defines the camera is looking towards -Z.  So an unrotated camera is initialized as:
	 *  1 0 0
	 *  0 1 0
	 *  0 0 -1
	 *
	 *  I thought about having an unrotated camera also be a proper identity matrix (all positive 1's), but 
	 *  then that means the camera is defined by the vector the oposite direction it is looking. 
	 *  It is the 'butt' vector.  I want to use the 'look' direction  vector, not the fart direction.
	 *  operations that act on rotation matrices, ask for a boolean call 'is_camera' that takes the flipped 'z' 
	 *  sign into account.
	 */


} gx_camera_t;


void gx_camera_init(gx_camera_t* cam);


void gx_load_transform(gx_mat_3x3* m, vec3* p);
void gx_translate(vec3* delta) ;
void gx_translate3(float x, float y, float z);
void gx_rotate(gx_mat_3x3* rot); 
void gx_identity();
void gx_getmatrix();
void gx_rotate_x(float rad);
void gx_rotate_y(float rad);
void gx_rotate_z(float rad);
void gx_scale3(float x,float y, float z);

void gxi_refresh_matrix(gx_shaderset_t* shader);
void gx_trans_vec3(vec3* po);

#define GX_TRANSFORM_INTERNAL_TO_GL	0  /* We calculate the modeview matrix, but send it to gl for fixed functionality */
#define GX_TRANSFORM_GL			1  /* use opengl's modelview matrix calculations */

extern int transmode;


#define DEGREE (3.14159/180)
