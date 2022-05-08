#define QUADARRAY_SKIRTS

//#define QUADARRAY_MIPMAP





struct gx_quadpatch_s;

#ifdef QUADARRAY_MIPMAP
typedef struct gx_quadpatch_mip_s
{
	int w;
	int h;
	vindex startindex;
	vindex endindex;
	struct gx_quadpatch_mip_s * lower;
} gx_quadpatch_mip_t;

void gx_quadarray_enable_mipmap( struct gx_quadpatch_s* qp);

#endif


typedef void (*gx_quadpatch_detailer_f)(struct  gx_quadpatch_s* dest, struct gx_quadpatch_s* source, int a_start, int a_end, int b_start, int b_end);

struct gx_quadpatch_s
{
	gx_vbuffer_t* vb;

	
	vindex startindex;  //1st vertex
	vindex endindex; //last vertex
	vindex endindex_noskirt;

	#ifdef QUADARRAY_MIPMAP
	gx_quadpatch_mip_t* mips;  //
	#endif

	zint32 w;  //2d array dimension
	zint32 h;
	struct gx_quadpatch_s* children[4];
	struct gx_quadpatch_s* parent;
	int self; //which one of my parent children am i?


#ifdef QUADARRAY_SKIRTS
	zbool useskirt;
	zbool skirtflip;
#endif

    //todo: move this
	vec3 origin; //center of planet
	vec3 center;  //center of the gx_quadpatch
	
	vec3 avgnorm; //average normal
    //end move
	
	
	float size;   //area metric
	float dsize;   //distance metric

	
	struct gx_quadpatch_s* adjacent[4];      //the 4 adjacent neighbors at the same detail level
	int adjacent_edge[4]; 					//the neighbor's edge I am touching
	int rev[4];  							//true if joined to neighbor in reverse direction

	//detailer function
	gx_quadpatch_detailer_f detailer; 
	int tag;
	int onlevel; //true if this is the level being drawn
	int generation; // +1 each time split
	int deleted;
	
	zbool setcenter;
};

typedef struct gx_quadpatch_s gx_quadpatch_t ;


//given a quadpatch, and x, y, a point index is returned
#define qp_vindex(qaaa, qxxx, qyyy)  (((qaaa)->w * (qyyy)) + (qxxx))

/* quad patches are arranged like this: (example 5x5)

 20 21 22 23 24     ^
 15                 |
 10                 | 
 5  6  7            |   b  direction
 0  1  2  3  4      |
					|
  a direction------>  

EDGES:
		TOP

LEFT			RIGHT

		BOTTOM

*/


//only works for square
//#define qp_skirtindex(qp, s, i)    (((qp)->w * (qp)->h) + ((s)*(qp)->h) + (i))

//non-square skirts

#define qp_corepoints(qp)   ((qp)->w * (qp)->h) 
#define qhm(s)  ((s)>=2 ? 2 : s)
#define qwm(s)  (((s)>=2) ? (s)-2 : 0)
#define qp_skirtindex(qp, s, i)    (  qp_corepoints(qp)   + (qhm(s))*(qp)->h    + qwm(s)*(qp)->w  + (i))


// Create a quadpatch of w my h points
gx_quadpatch_t* gx_quadpatch_mk(int w, int h, gx_quadpatch_detailer_f  detailer_func);

//draw a quadpatch
void gx_quadpatch_draw(gx_quadpatch_t* qp);

//join two edges of a quadpatch together
//when a quadpatch is split, the children will also stay connected
void gx_quadpatch_sew(gx_quadpatch_t* source, int source_edge, gx_quadpatch_t* dest, int dest_edge, int rev, int repair_seam);


#define GX_EDGE_LEFT    0
#define GX_EDGE_RIGHT   1
#define GX_EDGE_BOTTOM  2
#define GX_EDGE_TOP     3

//split a patch in to 4 children
zbool gx_quadpatch_split(gx_quadpatch_t* source);

void gx_quadpatch_norm(gx_quadpatch_t* qp);

extern int skirted;
extern int nonskirted;
extern int drawn;
extern int total;
 

typedef struct gx_quadpatch_sys_s
{
	zvec_t root_patches;  //these are the least detailed patches

	zvec_t active_patches; //the patches currently drawn

	zfloat32 splitsize;

} gx_quadpatch_sys_t;


void gx_quadpatch_sys_eval(gx_quadpatch_sys_t* qps, gx_camera_t* camera);
void gx_quadpatch_sys_draw(gx_quadpatch_sys_t* qps, gx_camera_t* camera);

zbool gx_quadpatch_sys_init(gx_quadpatch_sys_t* qps);
zbool gx_quadpatch_sys_add(gx_quadpatch_sys_t* qps, gx_quadpatch_t* qp);
zbool gx_quadpatch_sys_cleanup(gx_quadpatch_sys_t* qps);
