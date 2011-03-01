// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


void gx_camera_pos(float x, float y, float z);
void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis);

//rotates 3 vectors around each other
void gx_spin(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward);







void test_lighting_on();
void test_lighting_off();

#if 1

#define BOXY_MINUS	0
#define BOXY_PLUS	1
#define BOXX_MINUS	2
#define BOXX_PLUS	3
#define BOXZ_MINUS	4
#define BOXZ_PLUS	5



typedef struct gx_sector_s
{

	vec3 min;  //max/min  points define an axis aligned box
	vec3 max;

	vec_t meshes;  //meshes to draw in this sector
	struct portal_s* portals;
			
} gx_sector_t;


//a portal can connect two sectors
//for simplicity, a portal is represented as a sphere: If the sphere is visible, the 'portal' is visible,
//and the sector behind it is visible
typedef struct gx_portal_s
{
	vec3 pos;			  //position of portal
	zfloat32 radius;	  //size
	gx_sector_t* target;  //target sector
	struct portal_s* next_portal;
} gx_portal_t;



gx_sector_t* gx_sector_mk(vec3* min, vec3* max );

void gx_sector_outline(gx_sector_t* box);
void gx_portal_draw_test(gx_portal_t* p);

gx_portal_t* gx_sector_add_portal(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target);


#endif