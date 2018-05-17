


typedef struct gx_sector_s
{
	vec3 min;  //max/min  points define an axis aligned box for motion within
	vec3 max;
	vec3 center;
	
	
	//vec3 pmin;  // defines min/max for portal usage (no border)
	//vec3 pmax;

	//vec_t meshes;  //meshes to draw in this sector
	struct portal_s* portals;
			
	//int lastframe; //last frame number processed (to prevent cycles)
	//int rdepth; //number of hops from camera
	//zbool visiting;
} gx_sector_t;


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

zbool gx_traverse_sectors(gx_camera_t* cam, gx_sector_t* sector ) ;

gx_sector_t* gx_sector_mk(vec3* min, vec3* max );

void gx_sector_outline(gx_sector_t* box, zbool show_portals);
void gx_sector_draw(gx_sector_t* sect);
void gx_portal_draw_test(gx_portal_t* p, zbool active);


gx_portal_t* gx_sector_add_portal_quad(gx_sector_t* sector, gx_sector_t* target, vec3* normal, vec3** points);

gx_portal_t* gx_sector_add_portal_sphere(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target, vec3* normal);

///
//need some quick immediate mode functions
void gx_immediate(gx_prim_e); //return a vbuffer for like immediate mode
void gx_done();


void gx_color(float *);
void gx_texcoord0(float s, float t);
void gx_normal(vec3* v);
void gx_vertex(vec3* v);




