// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


void gx_camera_pos(float x, float y, float z);


//rotates 3 vectors around each other
//void gx_spin(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward);
void gx_spin(zbool is_camera, zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward);

//prototype camera structures
typedef struct gx_camera_s
{
	vec3 camera_pos;
	vec3 camera_forward;
	vec3 camera_right;
	vec3 camera_up;
} gx_camera_t;

void gx_camera_init(gx_camera_t* cam);




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

	vec3 min;  //max/min  points define an axis aligned box for motion within
	vec3 max;

	vec3 pmin;  // defines min/max for portal usage (no border)
	vec3 pmax;


	vec_t meshes;  //meshes to draw in this sector
	struct portal_s* portals;
			
	int lastframe; //last frame number processed (to prevent cycles)
	int rdepth; //number of hops from camera
	zbool visiting;
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





	int lastpeeked; //frame number last time that portal was peeked through

	//experimental:  make the portal a true polygon
	vec3 points[4];

	//transient values used to determine visibility:
	vec3 points_screen[4];
	zbool points_screen_ok;
	struct gx_portal_s* vis_chain_prev;

} gx_portal_t;



gx_sector_t* gx_sector_mk(vec3* min, vec3* max );

void gx_sector_outline(gx_sector_t* box, zbool show_portals);

void gx_sector_draw(gx_sector_t* sect);

void gx_portal_draw_test(gx_portal_t* p);

//gx_portal_t* gx_sector_add_portal(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target, vec3* normal);

gx_portal_t* gx_sector_add_portal_quad(gx_sector_t* sector, gx_sector_t* target, vec3* normal, vec3** points);

zbool gx_point_in_box( vec3* min, vec3* point, vec3* max, float border);


#endif





void gx_test_sphere(vec3* pos, float radius);


//SHEETS  
typedef struct gx_sheet_edge_s
{
	vec_t indirect_vertices; //pointer to vertices on the edge of this sheet
} gx_sheet_edge_t;

typedef struct gx_sheet_s
{
	gx_vbuffer_t* vb;  //holds points for this sheet
	int numpoints;	  //how many points are in this sheet  (only for non-rectangulat sheets)
	int maxpoints;
	
	int width;			//w/h only for rectangulat sheets
	int height;
	
	int* points;	  //vertices within the vbuffer

	int num_edges;	  //3 or 4 edges (sheets can be triangular or rectangular)
	gx_sheet_edge_t	edges[4]; 

} gx_sheet_t;



#define GX_SHEET_EDGE_TOP		0
#define GX_SHEET_EDGE_BOTTOM	1
#define GX_SHEET_EDGE_LEFT		2
#define GX_SHEET_EDGE_RIGHT		3

gx_sheet_t * gx_sheet_quad_mk(gx_vbuffer_t* vb, int width, int height);
int gx_sheet_set_at( gx_sheet_t* s, int x, int y,  int vertex);
void gx_sheet_show_buffer(gx_sheet_t* s);
void gx_sew_sheets( gx_sheet_t* s, int s_edge, gx_sheet_t* t, int t_edge, int operation);

#define		GX_ASSIGN_INDICES 1
#define		GX_COPY_POSITION  2
void gx_sheet_index(gx_sheet_t* s);

