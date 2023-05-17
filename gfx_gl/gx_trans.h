// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

typedef struct gfx_mat_3x3_s {
	vec3 x_axis;  /*RIGHT vector */
	vec3 y_axis;  /*UP vector*/
	vec3 z_axis;  /* Forward Vector (-z) */
} gfx_mat_3x3;


//camera relative to itself
void gfx_spin_matrix(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gfx_mat_3x3* rot);

typedef struct gfx_transform_s {
	vec3 pos;			//translation
	gfx_mat_3x3 rot;		//contains rotation and possible scaling
} gfx_transformT;

typedef	gfx_transformT gfx_cameraT;


void gfx_camera_init(gfx_cameraT* cam);
void gfx_trans_init(gfx_transformT* cam);
void gfx_camera_motion_6dof(gfx_cameraT* cam, float forward, float right, float up, float yaw, float pitch, float roll);
void gfx_camera_view(gfx_cameraT* cam);


void gfx_load_transform(gfx_transformT* trans);


void gfx_save_transform(gfx_transformT* s);

void gfx_blend_transform(float a, float b, gfx_transformT* trans);

void gfx_translate(vec3* delta) ;
void gfx_translate3(float x, float y, float z);
void gfx_rotate(gfx_mat_3x3* rot); 
void gfx_identity();
void gfx_rotate_x(float rad);
void gfx_rotate_y(float rad);
void gfx_rotate_z(float rad);
void gfx_rotate_3x3(gfx_mat_3x3* rot);
void gfx_scale3(float x,float y, float z);

void gxi_refresh_matrix( struct gx_shader_variant_s* shader);

void gfx_trans_vec3(vec3* po); //transform a point by the current modelview matrix (3x3 + translation)
void gfx_trans_dir_vec3(vec3* pd); //transform a direction by the current modeview matrix (3x3 only)


void gfx_projection3d(zfloat32 fovy, zfloat32 aspect, zfloat32 neardist, zfloat32 fardist);
void gfx_projection2d(zfloat32 left, zfloat32 right, zfloat32 top, zfloat32 bottom);

#define DEGREE (3.14159/180)
