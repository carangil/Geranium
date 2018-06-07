


typedef struct gx_sector_s
{
	vec3 min;  //max/min  points define an axis aligned box for motion within
	vec3 max;
	vec3 center;
	
	
	//vec3 pmin;  // defines min/max for portal usage (no border)
	//vec3 pmax;

	gx_mesh_t* mesh;
	//vec_t meshes;  //meshes to draw in this sector
	struct portal_s* portals;
	
	void (*outline)(struct gx_sector_s*, zbool);

	char* name;

	zbool visiting;//true during tree traversal.. breaks cycles
} gx_sector_t;



#define SECTOR_CUBE_Z	4
#define SECTOR_CUBE_Y	2
#define SECTOR_CUBE_X 	1


//points 0-7, broken out as  components
// - zero, and left, down, forward
// + are bitpositions for right, up, and backward
/*
#define POINT_LEFT_BOTTOM_FRONT		(0)
#define POINT_RIGHT_BOTTOM_FRONT	(SECTOR_CUBE_X)
#define POINT_LEFT_TOP_FRONT		(SECTOR_CUBE_Y)
#define POINT_RIGHT_TOP_FRONT		(SECTOR_CUBE_X|SECTOR_CUBE_Y)
#define POINT_LEFT_BOTTOM_BACK		(SECTOR_CUBE_Z)
#define POINT_RIGHT_BOTTOM_BACK		(SECTOR_CUBE_Z|SECTOR_CUBE_X)
#define POINT_LEFT_TOP_BACK			(SECTOR_CUBE_Z| SECTOR_CUBE_Y)
#define POINT_RIGHT_TOP_BACK		(SECTOR_CUBE_Z|SECTOR_CUBE_X|SECTOR_CUBE_Y)
*/



typedef struct cubesector_s{
		gx_sector_t sector;
		vec3 points[8];
} gx_cube_sector_t;

void gx_cube_points(vec3* points, vec3* min, vec3* max);
gx_cube_sector_t* gx_cube_sector_mk(vec3* points);



//a portal can connect two sectors
//for simplicity, a portal is represented as a sphere: If the sphere is visible, the 'portal' is visible,
//and the sector behind it is visible
typedef struct gx_portal_s
{
	vec3 pos;			  //position of portal
	zfloat32 radius;	  //size
	gx_sector_t* target;  //target sector

	vec3 normal;  //look direction of the portal (maybe have circular portals?)

	struct gx_portal_s* next_portal;


	//int lastpeeked; //frame number last time that portal was peeked through

	//experimental:  make the portal a true polygon
	zbool isquad; //true if this is a quad portal
	vec3 points[4];

	//transient values used to determine visibility:
	//vec3 points_screen[4];
	//zbool points_screen_ok;
	//struct gx_portal_s* vis_chain_prev;

} gx_portal_t;

gx_sector_t* gx_traverse_sectors(gx_camera_t* cam, gx_sector_t* sector ) ;

gx_sector_t* gx_sector_mk(char* name, vec3* min, vec3* max );

void gx_sector_outline(gx_sector_t* box, zbool show_portals);
void gx_sector_draw(gx_sector_t* sect);
void gx_portal_draw_test(gx_portal_t* p, zbool active);


gx_portal_t* gx_sector_add_portal_quad(gx_sector_t* sector, gx_sector_t* target, vec3* normal, vec3** points);

gx_portal_t* gx_sector_add_portal_sphere(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target, vec3* normal);

///
//need some quick immediate mode functions
void gx_immediate(gx_prim_e); //return a vbuffer for like immediate mode
void gx_end();


void gx_point(vec3* vec, vec3* norm, float c[4], float s, float t);


zbool gx_vbuffer_collide(gx_vbuffer_t* vb, zuint32 start, zuint32 end,vec3* pos, vec3* dir ) ;

