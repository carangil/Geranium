// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


//TODO: move to vectormath, to better support using vectors in z
#define Structmap_namedv3 Vec3:x=x:Real;y=y:Real;z=z:Real;


//3x3 matrix is padded out so it can be inside an opengl 4x4 matrix
typedef struct gfx_mat_3x3_ps {
	vec4 x_axis;  /*RIGHT vector */
	vec4 y_axis;  /*UP vector*/
	vec4 z_axis;  /* Forward Vector (-z) */
} gfx_mat_3x3;

//Zdef struct gfx_mat_3x3 Matrix33:x_axis=XAxis:Vec3;y_axis=YAxis:Vec3;z_axis=ZAxis:Vec3;

//camera relative to itself
void gfx_spin_matrix(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, gfx_mat_3x3* rot);

typedef struct gfx_transform_s {
	gfx_mat_3x3 rot;	//contains rotation and possible scaling
	vec4 pos;			//translation
} gfx_transformT;

typedef union  {
	gfx_transformT transform;
	zfloat32 array[16];
} gfx_mat_4x4 ;


typedef	gfx_transformT gfx_cameraT;
//Zdef struct gfx_transformT Transform:rot=Rotation:Matrix33;pos=Position:Vec3;
//Zdef type gfx_cameraT Transform



void gfx_camera_init(gfx_cameraT* cam);
void gfx_trans_init(gfx_transformT* cam);
void gfx_camera_motion_6dof(gfx_cameraT* cam, float forward, float right, float up, float yaw, float pitch, float roll);
void gfx_camera_view(gfx_cameraT* cam);


void gfx_load_transform(gfx_transformT* trans);


void gfx_save_transform(gfx_transformT* s);

void gfx_blend_transform(float a, float b, gfx_transformT* trans);

void gfx_translate(vec3* delta) ;
void gfx_translate3(float x, float y, float z);

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
