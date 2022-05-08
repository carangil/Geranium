
//build cubes from quadpatches

#define GEN_CUBE_TOP	0
#define GEN_CUBE_BOTTOM	1
#define GEN_CUBE_FRONT	2
#define GEN_CUBE_BACK	3
#define GEN_CUBE_LEFT	4
#define GEN_CUBE_RIGHT	5

// 2018INSIDE reorders vertices so culling order is correct
//from viewing inside the cube

#define GEN_INSIDE	1
#define GEN_SPHERE	2


zbool gen_qpcube(gx_quadpatch_sys_t* qpsys,
				 vec3 * origin,
				 int tessel,
				 float radius,
				  gx_drawstyle_t* ds[6],
				  gx_quadpatch_t* qpo[6],
					gx_quadpatch_detailer_f detailer[6],
				 int flags,
				 float size, float dsize
 				);

void gen_random_detail(gx_quadpatch_t* dest, gx_quadpatch_t* source, 
	int a_start, int a_end, int b_start, int b_end);

//generates a vbuffer with a multicolor test unit sphere at 0,0,0
//just for testing
gx_vbuffer_t* gensphere(int quality);
