// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

typedef struct gfx_mat_3x3_s {
	vec3 x_axis;  /*RIGHT vector */
	vec3 y_axis;  /*UP vector*/
	vec3 z_axis;  /* Forward Vector (-z) */
} gfx_mat_3x3;


//rotates 3 vectors around each other
void gfx_spin_matrix(zbool is_camera, zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gfx_mat_3x3* rot);


typedef struct gfx_transform_s {
	vec3 pos;			//translation
	gfx_mat_3x3 rot;		//contains rotation and possible scaling
} gfx_transformT;

typedef	gfx_transformT gfx_cameraT;

	/* Note: camera rotation matrices are handled a little different than object matrices. 
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

void gfx_camera_init(gfx_cameraT* cam);
void gfx_trans_init(gfx_transformT* cam);

void gfx_load_transform(gfx_mat_3x3* m, vec3* p);
void gfx_translate(vec3* delta) ;
void gfx_translate3(float x, float y, float z);
void gfx_rotate(gfx_mat_3x3* rot); 
void gfx_identity();
void gfx_rotate_x(float rad);
void gfx_rotate_y(float rad);
void gfx_rotate_z(float rad);
void gfx_scale3(float x,float y, float z);

void gxi_refresh_matrix( /*gfx_shader_t* shader*/);

void gfx_trans_vec3(vec3* po); //transform a point by the current modelview matrix (3x3 + translation)
void gfx_trans_dir_vec3(vec3* pd); //transform a direction by the current modeview matrix (3x3 only)


void gfx_projection3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
void gfx_projection2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);

#define DEGREE (3.14159/180)
