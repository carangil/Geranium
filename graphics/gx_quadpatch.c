#include <stdio.h>
#include <math.h>


#include "ztypes.h"
#include "zmem.h"
#include "zmath.h"
#include "zvector.h"
#include "zlist.h"
#include "zstring.h"
#include "gx_sys.h"
#include "gx_buffers.h"
#include "gx_image.h"
#include "gx_drawstyle.h"
#include "gx_trans.h"
#include "gx_light.h"

#include "gx_quadpatch.h"

//todo: remove these globals
int total=0;
int drawn=0;

//qp cleanup
zbool gx_gp_delete(void* v)
{
	int i;
	int j;
	//rcurse_count++;
	gx_quadpatch_t* qp = (gx_quadpatch_t*) v;
    printf("Delete!\n");
	//scribble out adjacent references to this patch
	for (i=0;i<4;i++)
	{
		gx_quadpatch_t* qpadj = qp->adjacent[i];
		if (qpadj)
		{
			for (j=0;j<4;j++)
			{
				if (qpadj->adjacent[j] == qp)
				{
					qpadj->adjacent[j] = NULL;
//					printf(" deleted adjacent backreference %d\n", j);
				}
			}
		}
		
		if (qp->parent)
		{	

			for (j=0;j<4;j++)
			{
				if (qp->parent->children[j]==qp) {
					qp->parent->children[j] = NULL;
//					printf(" deleted Downreference %d\n", j);
				}
			}


		}

	}


	//free children
	ram_free(qp->children[0]);
	ram_free(qp->children[1]);
	ram_free(qp->children[2]);
	ram_free(qp->children[3]);


	ram_free(qp->vb); //free the vbuffer

	total--;


	//rcurse_count--;

	return ZTRUE; //free ourself
}


//create the gx_quadpatch


gx_quadpatch_t* gx_quadpatch_mk(int w, int h, gx_quadpatch_detailer_f  detailer_func)
{
	gx_quadpatch_t *qp = NULL;
	int a;
	int b;

	qp = ram_alloc(sizeof(*qp), gx_gp_delete);
	total++;

	if (qp)
	{
		
		qp->w=w;
		qp->h=h;
#ifdef QUADARRAY_SKIRTS
		qp->vb = gx_vbuffer_mk(w*h  +2*w + 2*h, w*h*3*2 + w*3*2 +h*3*2, GX_VBUFFER_NORMAL | GX_VBUFFER_TEXCOORD);
#else
		qp->vb = gx_vbuffer_mk(w*h, w*h*3*2,  GX_VBUFFER_NORMAL | GX_VBUFFER_TEXCOORD);
#endif
		qp->startindex = 0;
 
		//qp->vb->vertex_count = w*h;  //say all vertices are filled out

		qp->generation = 1;
		qp->size = 1;

		qp->detailer = detailer_func;

#if 1
		//need to index to make triangles
		for (a=0;a<(w-1);a++)
		{
			for (b=0;b<(h-1);b++)
			{	

				//tri 1
				gx_vbuffer_index( qp->vb, qp_vindex(qp,a,b));
				gx_vbuffer_index( qp->vb, qp_vindex(qp,a+1,b) );
				gx_vbuffer_index( qp->vb, qp_vindex(qp,a+1,b+1) );

				//tri 2
				gx_vbuffer_index( qp->vb, qp_vindex(qp,a,b) );		
				gx_vbuffer_index( qp->vb, qp_vindex(qp,a+1,b+1) );
				gx_vbuffer_index( qp->vb, qp_vindex(qp,a,b+1) );

				qp->endindex+=6;
			}
		}

		//now add skirts
		
		qp->endindex_noskirt = qp->endindex;

#ifdef QUADARRAY_SKIRTS


		for (b=0;b< (h-1); b++)
		{
			//left skirt
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,0,b)    );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,0,b) );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,0,b+1) );

			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,0,b)    );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,0,b+1) );
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,0,b+1)    );

			qp->endindex+=6;


			//right skirt
		
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_RIGHT,b)    );
			
			gx_vbuffer_index( qp->vb, qp_vindex(qp,w-1,b+1) );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,w-1,b) );

			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_RIGHT,b)    );
			
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_RIGHT,b+1)    );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,w-1,b+1) );

			qp->endindex+=6;


	}


	for (b=0;b<(w-1);b++)
	{


			//top skirt
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_TOP,b)    );
			
			gx_vbuffer_index( qp->vb, qp_vindex(qp,b+1 ,0) );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,b , 0) );

			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_TOP,b)    );
			
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_TOP,b+1)    );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,b+1 , 0) );

			qp->endindex+=6;


			//bottom skirt
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_BOTTOM,b)    );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,b , h-1) );
			gx_vbuffer_index( qp->vb, qp_vindex(qp,b+1 ,h-1) );
			


			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_BOTTOM,b+1)    );
			gx_vbuffer_index( qp->vb, qp_skirtindex(qp,GX_EDGE_BOTTOM,b)    );
			

			gx_vbuffer_index( qp->vb, qp_vindex(qp,b+1 , h-1) );

			qp->endindex+=6;



		}

#endif

#endif

		
	}
	return qp;
}

void gx_quadpatch_draw(gx_quadpatch_t* qp)
{

	
	if (!qp)
		return;

	drawn ++;


#ifdef QUADARRAY_SKIRTS
	{
		int i;
		int ok=1;
		for (i=0;i<4;i++)
		{
			if ((!qp->adjacent[i] ) || !(qp->adjacent[i]->onlevel))
				ok=0;
		}

		if (ok)
		{
		//	nonskirted++;
			qp->useskirt=0;
		}
		else
		{
		//	skirted++;
			qp->useskirt=1;
		}

	}



	if (qp->useskirt )

		gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex,  gx_triangles, ZTRUE);
	//	gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex,  gx_lines, ZTRUE);
	
		
	else 
#endif
		gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex_noskirt,  gx_triangles, ZTRUE);
		//gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex_noskirt,  gx_lines, ZTRUE);
}




/* high level qp system */


zbool gx_quadpatch_sys_init(gx_quadpatch_sys_t* qps)
{
	memset(qps, 0, sizeof(*qps));
	if (!zvec_mk(&qps->root_patches, 4))
		return ZFALSE;

	if (!zvec_mk(&qps->active_patches, 4))
		return ZFALSE;

	zvec_disown(&qps->active_patches); //don't own pointers to things

	qps->splitsize=1.0;

	return ZTRUE;
}


zbool gx_quadpatch_sys_cleanup(gx_quadpatch_sys_t* qps)
{

	zvec_cleanup(&qps->root_patches);
	zvec_cleanup(&qps->active_patches);


	return ZTRUE;
}





zbool gx_quadpatch_sys_add(gx_quadpatch_sys_t* qps, gx_quadpatch_t* qp)
{

	if (!qps || !qp )
		return ZFALSE;

	
#ifdef QUADARRAY_SKIRTS
	
	gx_quadpatch_skirt(qp);
	//make sure skirt vertices are calculated
	
#endif
	
	gx_vbuffer_update(qp->vb); 
	
	zvec_add(&qps->root_patches, qp);
	zvec_add(&qps->active_patches, qp);

	return ZTRUE;
}




// a quadpatch's children are laid out in this order:

// 2 3         (y=1)
// 0 1         (y=0)


//edge child arrays
int edge_left_child[] = {0,2};
int edge_right_child[] = {1,3};
int edge_bottom_child[] = {0,1};
int edge_top_child[] = {2,3};

int* edge_child[] = { edge_left_child, edge_right_child, edge_bottom_child, edge_top_child};

char* edge_names[] = {"LEFT", "RIGHT", "BOTTOM", "TOP"};

int edge_opposite[] = { GX_EDGE_RIGHT, GX_EDGE_LEFT, GX_EDGE_TOP, GX_EDGE_BOTTOM};

//finds adjacent patch, even if it has a different parent
gx_quadpatch_t* find_neighbor(gx_quadpatch_t* q, int edge, int* nedge, int* rev)
{
	int x;
	int y;
	int child_num;
//	int edge_child_n;
	*rev=0;
	

	if (!q->parent)
		return NULL; //can't

	if (q->adjacent[edge]) 
	{
		//printf("already done\n");
		//this would happen if patches were alreasy sewn together
		return q->adjacent[edge];
	}



	
	// decompose my 'child number' into x and y
	x = q->self & 1;
	y = q->self >> 1;


	
	//find coords of neigbor

	if (edge == GX_EDGE_LEFT)
		x--;

	if (edge == GX_EDGE_RIGHT)
		x++;

	if (edge == GX_EDGE_BOTTOM)
		y--;

	if (edge == GX_EDGE_TOP)
		y++;

	if (((x==0 || x==1)) && ((y==0||y==1)))
	{ 
		//neighbor has same parent (within a group of 4)
	
		//lookup opposite edge
		
		*nedge = edge_opposite[edge];
		

		//build child number from x and y
		child_num = (y << 1) | x ;
		//return NULL;

	//	printf( " child %d is also edge child %d of edge %s  == ", q->self, edge_child_n, edge_names[edge]);

	//	printf(" %s of child %d is   %s %d\n", edge_names[edge], q->self, edge_names[*nedge], child_num);

		return q->parent->children[child_num];
	}
	//return NULL;
	//need to look at parent's neighbors
	if (!q->parent->adjacent[edge]) 
		return NULL;

//	printf("  case: child number: %d, edge %s, connected edge %s\n",
//		q->self,
//		edge_names[edge],
//		edge_names[q->parent->adjacent_edge[edge]]
//		);
	
		{	
			int edge_target =   q->parent->adjacent_edge[edge];
			int edge_child_idx;
			if (edge_child[edge][0] == q->self)
				edge_child_idx =0;
			else if (edge_child[edge][1] == q->self)
				edge_child_idx = 1;
			else
			{
				printf(" illegal case: child number: %d, edge %s, connected edge %s\n",
					q->self,
					edge_names[edge],
					edge_names[q->parent->adjacent_edge[edge]]
				);
				return NULL;
			}

			if(q->parent->rev[edge])
				edge_child_idx = 1- edge_child_idx;   //reverse it

			*nedge =edge_target;
			*rev = q->parent->rev[edge];
			child_num = edge_child[edge_target][edge_child_idx];
			return q->parent->adjacent[edge]->children[child_num];


		}

	printf(" unhandled case: child number: %d, edge %s, connected edge %s\n",
		q->self,
		edge_names[edge],
		edge_names[q->parent->adjacent_edge[edge]]
		);

	return NULL;
}

//add skirts to quadpatches

#ifdef QUADARRAY_SKIRTS
void gx_quadpatch_skirt( gx_quadpatch_t* qp)
{
	int i;
	
	float skirtd = qp->dsize*2 ;

	if (qp->skirtflip)
		skirtd= -skirtd;
	
	//qp->useskirt = ZTRUE;
	
	for (i=0;i<qp->h;i++)
	{
		//left
		vec3mov( *gxi_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_LEFT, i)), *gxi_vbuffer_n(qp->vb, qp_vindex(qp, 0, i)));
		vec3mov( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_LEFT, i)), *gxi_vbuffer_v(qp->vb, qp_vindex(qp, 0, i)));
		vec3sub(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_LEFT, i)), qp->origin);
		vec3scale( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_LEFT, i)), 1-skirtd);
		vec3add(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_LEFT, i)), qp->origin);

		gxi_vbuffer_s(qp->vb,0, qp_skirtindex(qp, GX_EDGE_LEFT, i)) = gxi_vbuffer_s(qp->vb,0, qp_vindex(qp, 0, i) );
		gxi_vbuffer_t(qp->vb,0, qp_skirtindex(qp, GX_EDGE_LEFT, i)) = gxi_vbuffer_t(qp->vb,0, qp_vindex(qp, 0, i) );
				
		
	
		//right
		vec3mov( *gxi_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), *gxi_vbuffer_n(qp->vb, qp_vindex(qp, qp->w-1, i)));
		vec3mov( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), *gxi_vbuffer_v(qp->vb, qp_vindex(qp, qp->w-1, i)));
		vec3sub(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), qp->origin);
		vec3scale( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), 1-skirtd);
		vec3add(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), qp->origin);
		
		gxi_vbuffer_s(qp->vb,0, qp_skirtindex(qp, GX_EDGE_RIGHT, i)) = gxi_vbuffer_s(qp->vb,0, qp_vindex(qp, qp->w-1, i) );
		gxi_vbuffer_t(qp->vb,0, qp_skirtindex(qp, GX_EDGE_RIGHT, i)) = gxi_vbuffer_t(qp->vb,0, qp_vindex(qp, qp->w-1, i) );
		
		
		
	}

	//Note: edge top/ bottom might be reversed here, but it doesn't matter
	//as long as each edge is generated once
	for(i=0;i<qp->w;i++)
	{
		//top
		vec3mov( *gxi_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), *gxi_vbuffer_n(qp->vb, qp_vindex(qp,  i,0)));
		vec3mov( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), *gxi_vbuffer_v(qp->vb, qp_vindex(qp,  i,0)));
		vec3sub(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), qp->origin);
		vec3scale( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), 1-skirtd);
		vec3add(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), qp->origin);

		gxi_vbuffer_s(qp->vb,0, qp_skirtindex(qp, GX_EDGE_TOP, i)) = gxi_vbuffer_s(qp->vb,0, qp_vindex(qp, i, 0) );
		gxi_vbuffer_t(qp->vb,0, qp_skirtindex(qp, GX_EDGE_TOP, i)) = gxi_vbuffer_t(qp->vb,0, qp_vindex(qp, i, 0) );
		
		
		
		
		//bottom
		vec3mov( *gxi_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), *gxi_vbuffer_n(qp->vb, qp_vindex(qp,  i,qp->h-1)));
		vec3mov( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), *gxi_vbuffer_v(qp->vb, qp_vindex(qp,  i,qp->h-1)));
		vec3sub(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), qp->origin);
		vec3scale( *gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), 1-skirtd);
		vec3add(*gxi_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), qp->origin);

		
		gxi_vbuffer_s(qp->vb,0, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)) = gxi_vbuffer_s(qp->vb,0, qp_vindex(qp,  i,qp->h-1) );
		gxi_vbuffer_t(qp->vb,0, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)) = gxi_vbuffer_t(qp->vb,0, qp_vindex(qp,  i,qp->h-1) );
		
		
	}



}
#endif





#define QUADARRAY_TAG_NONE        0

#define QUADARRAY_TAG_REMOVE	    1
//#define QUADARRAY_TAG_DRAW_ONLY     2
#define QUADARRAY_TAG_DELETE	    3

#define ENABLE_DELETE


//scale up a 1/4 of a patch 2x each direction into a full-size patch
gx_quadpatch_t* gx_quadpatch_detail_2x(int self, struct gx_quadpatch_s* source, int a_start, int a_end, int b_start, int b_end)
{
	int a;
	int b;
	gx_quadpatch_t* dest;
	int w = (a_end-a_start-1) * 2+1 ;
	int h = (b_end-b_start-1) * 2+1 ;
	int x;
	int y;
	vec3 p;
	int div;
	float fdiv;


	dest =  gx_quadpatch_mk(source->w,source->h, source->detailer);
    
    
    //dest->vb->vertex_count = w*h;
	dest->self=self;
	dest->generation = source->generation + 1;	
	dest->parent = source;

//	srandf(  source->center.named.x + source->center.named.y  + source->center.named.z);

	dest->origin = source->origin;
	dest->size = source->size /4;  //decrease area by 4
	dest->dsize = source->dsize/2;  //decrease lengths by 2
	dest->skirtflip = source->skirtflip;
	
	//detailer must fill out vertices, texture coordinates, normals
	//detailer may call gx_quadpatch_norm to automatic normal calculation
	source->detailer(dest, source, a_start, a_end, b_start, b_end);
	

//	dest->center = *gxi_vbuffer_v( dest->vb, qp_vindex( dest, dest->w/2,dest->h/2));
	dest->parent = source;
	

	
	//for each child, find the 4 neighbors and link together
	
#if 1	
	if (1) {
		int edge;
		int nedge;
		int rev;

		for (edge=0;edge<4;edge++)
		{
			gx_quadpatch_t* n = find_neighbor( dest, edge, &nedge,&rev);
			gx_quadpatch_sew(n, nedge, dest, edge,rev, ZTRUE);
		}


	}
#endif

	
	//if we are using skirts, them generate them
#ifdef QUADARRAY_SKIRTS
	gx_quadpatch_skirt(dest);
#endif
	
	
	return dest;
}


void calcnorm(vec3* n, vec3* a, vec3* b, vec3* c)
{
	vec3 ab;
	vec3 ac;
	float d;
	
	vec3mov(ab, *b);
	vec3sub(ab, *a);


	vec3mov(ac, *c);
	vec3sub(ac, *a);

	vec3cross(*n, ab,ac);
	 
	d = vec3abs_sq(*n);
	vec3scale(*n, 1/d);


}

//calculate normals from the already given geometry

void gx_quadpatch_norm(gx_quadpatch_t* qp)
{
	int a;
	int b;
	vec3 p;
	vec3 acc;
	int cnt=0;

	vec3 posavg;
	int  pos_cnt=0;
	
	vec3 avg;
	int  avg_cnt=0;
	vec3set(avg, 0,0,0);
	posavg = avg;

	for (a=0;a< qp->w; a++)
	{
		for(b=0;b<qp->h;b++)
		{
			
			vec3add(posavg, *gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b)));
			pos_cnt++;
			
			
			vec3set(acc,0,0,0);
			cnt=0;
			
			
			
#if 1
			if ((a< (qp->w-1)) && (b< (qp->h-1)))
			{
				cnt++;
				calcnorm(&p,
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a+1,b)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b+1)));
				vec3add(acc, p);
			}

#endif		

#if 1
			if ((a> 0) && (b< qp->w-1))
			{
				cnt++;
				calcnorm(&p,
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b+1)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a-1,b)));
				vec3add(acc, p);
			}
#endif


#if 1
			if ((a> 0) && (b> 0))
			{
				cnt++;
				calcnorm(&p,
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a-1,b)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b-1)));
					
				vec3add(acc, p);
			}
#endif

#if 1
			if ((a< (qp->w-1)) && (b>0 ))
			{
				cnt++;
				calcnorm(&p,
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a,b-1)),
					gxi_vbuffer_v( qp->vb, qp_vindex( qp, a+1,b)));
					
				vec3add(acc, p);
			}
#endif

			if (cnt > 0)
			{
				float d;
				d = vec3abs_sq(acc);
				d = sqrt(d);
				vec3scale(acc,1/d);
				
				vec3add(avg, acc);
				avg_cnt++;
				
			}

			else
			{
				vec3set(acc,0,1,0);
				printf(" ZERO\n");
				exit(0);
			}

			*gxi_vbuffer_n(qp->vb, qp_vindex( qp, a,b)) = acc;

		}

	}

	{
		float d;
		d = vec3abs_sq(avg);
		
		
		
		d = sqrt(d);
		vec3scale(avg,1.0/avg_cnt);
		vec3mov(qp->avgnorm, avg);
	}
	
	vec3scale(posavg, 1.0/pos_cnt);
	qp->center = posavg;
	qp->setcenter = ZTRUE;

}





//this assigns two patches as neighbors
//optionally 'sews' the patches together, by copying source's edge 
//geometry over dest's edge geometry.  fixes cracks when generating
//random geometry
void gx_quadpatch_sew(gx_quadpatch_t* source, int source_edge, gx_quadpatch_t* dest, int dest_edge, int rev, int repair_seam)
{
	int i;
	int is;
	int imax;

	vec3* s;
	vec3* sn;

	if (!source || !dest)
		return;


	if (dest->h != dest->h)
		return;
	if (dest->w != source->w)
		return;

	source->adjacent[source_edge] = dest;
	source->adjacent_edge[source_edge] = dest_edge;
	source->rev[source_edge] = rev;

	dest->adjacent[dest_edge] = source;
	dest->adjacent_edge[dest_edge] = source_edge;
	dest->rev[dest_edge] = rev;
	
	if (!repair_seam)
		return;
	
	if ((source_edge == GX_EDGE_LEFT) || (source_edge==GX_EDGE_RIGHT))	
		imax = source->h;
	else
		imax = source->w;


	for (i=0;i< imax; i++)
	{

		s = NULL;

		if (rev)
			is = source->w -1-i;
		else 
			is = i;


		//get
		if (source_edge == GX_EDGE_LEFT)
		{
			s = gxi_vbuffer_v( source->vb, qp_vindex( source, 0,is));
			sn= gxi_vbuffer_n( source->vb, qp_vindex( source, 0,is));
		}
		else if (source_edge == GX_EDGE_RIGHT)
		{
			s = gxi_vbuffer_v( source->vb, qp_vindex( source, source->w-1,is));
			sn= gxi_vbuffer_n( source->vb, qp_vindex( source, source->w-1,is));
		}
		else if (source_edge == GX_EDGE_BOTTOM)
		{
			s = gxi_vbuffer_v( source->vb, qp_vindex( source, is,0));
			sn= gxi_vbuffer_n( source->vb, qp_vindex( source, is,0));
		}
		else if (source_edge == GX_EDGE_TOP)
		{
			s = gxi_vbuffer_v( source->vb, qp_vindex( source, is,source->h-1));
			sn= gxi_vbuffer_n( source->vb, qp_vindex( source, is,source->h-1));
		}


		//put
  		if (dest_edge == GX_EDGE_LEFT)
		{
  			*gxi_vbuffer_v( dest->vb, qp_vindex( dest, 0,i)) = *s; 
			*gxi_vbuffer_n( dest->vb, qp_vindex( dest, 0,i)) = *sn; 
		}
  		else if (dest_edge == GX_EDGE_RIGHT)
		{
			*gxi_vbuffer_v( dest->vb, qp_vindex( dest, dest->w-1,i)) = *s;
			*gxi_vbuffer_n( dest->vb, qp_vindex( dest, dest->w-1,i)) = *sn;
		}
  		else if (dest_edge == GX_EDGE_BOTTOM)
		{
  			*gxi_vbuffer_v( dest->vb, qp_vindex( dest, i,0)) = *s;
			*gxi_vbuffer_n( dest->vb, qp_vindex( dest, i,0)) = *sn;
		}
  		else if (dest_edge == GX_EDGE_TOP )
		{
  			*gxi_vbuffer_v( dest->vb, qp_vindex( dest, i,dest->h-1)) = *s;
			*gxi_vbuffer_n( dest->vb, qp_vindex( dest, i,dest->h-1)) = *sn;
		}

	}
}




/* make sure a quadpatch has children */
zbool gx_quadpatch_split(gx_quadpatch_t* source)
{
	zbool ok = ZTRUE;

	zuint32 i;
	zuint32 c;

	if (!source->detailer)
		return ZFALSE;

	c=0;
	for (i=0;i<4;i++)
	{
		if (source->children[i])
			c++;

		if (source->children[i] && source->children[i]->tag == 3)
		{
			printf(" tag is delete; this is a bad idea\n");
//			exit(0);
			return ZFALSE;
		}


	}


	if (c==4) //all children split
		return ZTRUE;

	if (c!= 0){
		//partial split
		printf("BAD");
		return ZFALSE;

	}



	//otherwise split it:

	source->children[0] = gx_quadpatch_detail_2x(0, source, 0, source->w/2+1, 0, source->h/2+1);
	source->children[1] = gx_quadpatch_detail_2x(1, source, source->w/2, source->w , 0, source->h/2+1);
	source->children[2] = gx_quadpatch_detail_2x(2, source, 0, source->w/2+1,        source->h/2,source->h );
	source->children[3] = gx_quadpatch_detail_2x(3, source, source->w/2, source->w , source->h/2, source->h);


//	gx_vbuffer_update(source->children[0]->vb);
//	gx_vbuffer_update(source->children[1]->vb);
//	gx_vbuffer_update(source->children[2]->vb);
//	gx_vbuffer_update(source->children[3]->vb);




	//update vbuffers
	for(i=0;i<4;i++)
	{
		if (source->children[i])
			gx_vbuffer_update(source->children[i]->vb);
		else
			return ok= ZFALSE;
	}



	return ok;	

}


#if 1
void gx_quadpatch_sys_eval(gx_quadpatch_sys_t* qps,  gx_camera_t* camera)
{


		int i;
		zvec_t* quadpatches = &qps->active_patches;
		gx_quadpatch_t* qp;
		vec3 p;
		float d;


		for (i=0;i< zvec_count(quadpatches);i++)
		{
			qp = zvec_get_at(quadpatches, i);

			//if was flagged to remove, remove it
			if (qp->tag == QUADARRAY_TAG_REMOVE || qp->tag == QUADARRAY_TAG_DELETE ) 	
			{

				zvec_remove_unordered(quadpatches, i);
				i--; //repeat this position 

				if (qp->tag == QUADARRAY_TAG_DELETE)
					ram_free(qp);
			
				else
					qp->tag = QUADARRAY_TAG_NONE;

				continue;
			}
		
			//check for potential split/recombine

			if (qp->tag != QUADARRAY_TAG_NONE)
					continue;  //don't process if it has any flags set

			vec3mov(p, camera->pos);
			vec3sub(p, qp->center);
			d = vec3abs_sq(p);
			d = sqrt(d);


			//split a tag
			if (   ((qp->size)/(d*d)) > qps->splitsize )
				{
					gx_quadpatch_t* dest[4];

					//try to split.  if successful, do the swap 	
					if (gx_quadpatch_split(qp))
					{
						qp->tag = QUADARRAY_TAG_REMOVE;  //remove this one (the parent)
						qp->onlevel = 0; //will no longer be active
				
						//add the children to the active list
						zvec_add(quadpatches, qp->children[0]);
						zvec_add(quadpatches, qp->children[1]);
						zvec_add(quadpatches, qp->children[2]);
						zvec_add(quadpatches, qp->children[3]);

						
						/*
						//clear any tags on the children
						qp->children[0]->tag = QUADARRAY_TAG_NONE;
						qp->children[1]->tag = QUADARRAY_TAG_NONE;
						qp->children[2]->tag = QUADARRAY_TAG_NONE;
						qp->children[3]->tag = QUADARRAY_TAG_NONE;
						*/

						qp->children[0]->onlevel = 1;
						qp->children[1]->onlevel = 1;
						qp->children[2]->onlevel = 1;
						qp->children[3]->onlevel = 1;

					}
				}

				//combine patches
				//only if parent exists, parent has to tags.
				//and all siblings exist
				//and all siblings are currently drawn
				else if ( qp->parent && ( qp->parent->tag == QUADARRAY_TAG_NONE)
					&& qp->parent->children[0] && qp->parent->children[0]->onlevel
					&& qp->parent->children[1] && qp->parent->children[1]->onlevel
					&& qp->parent->children[2] && qp->parent->children[2]->onlevel
					&& qp->parent->children[3] && qp->parent->children[3]->onlevel
					)
				{
					vec3mov(p, camera->pos);
					vec3sub(p, qp->parent->center);
					d = vec3abs_sq(p);
					d = sqrt(d);
					
					if (((qp->parent->size)/(d*d)) < qps->splitsize)
					{  //combine threshold (if parent woulnd't be split now, then it should be combined)
					
						//will no longer be on level
						qp->parent->children[0]->onlevel = 0;
						qp->parent->children[1]->onlevel = 0;
						qp->parent->children[2]->onlevel = 0;
						qp->parent->children[3]->onlevel = 0;


						qp->parent->children[0]->tag = QUADARRAY_TAG_REMOVE; 
						qp->parent->children[1]->tag = QUADARRAY_TAG_REMOVE;
						qp->parent->children[2]->tag = QUADARRAY_TAG_REMOVE;
						qp->parent->children[3]->tag = QUADARRAY_TAG_REMOVE;



#ifdef ENABLE_DELETE
						qp->parent->children[0]->tag = QUADARRAY_TAG_DELETE; 
						qp->parent->children[1]->tag = QUADARRAY_TAG_DELETE;
						qp->parent->children[2]->tag = QUADARRAY_TAG_DELETE;
						qp->parent->children[3]->tag = QUADARRAY_TAG_DELETE;
					
	
						//throw away children :(
						qp->parent->children[0] = NULL;
						qp->parent->children[1] = NULL;
						qp->parent->children[2] = NULL;
						qp->parent->children[3] = NULL;
#endif


						zvec_add(quadpatches, qp->parent); //put parent back in
						qp->parent->onlevel = 1; //parent is on level
						qp->parent->tag = QUADARRAY_TAG_NONE;

					}
				}

			//zzz

		}
}
#endif

void gx_quadpatch_sys_draw(gx_quadpatch_sys_t* qps, gx_camera_t* camera)
{
		int cull=0;
		int i;
		zvec_t* quadpatches = &qps->active_patches;
		gx_quadpatch_t* qp;

		//printf("%d active patches in %p\n", vec_count(quadpatches), qps);

		for (i=0;i< zvec_count(quadpatches);i++)
		{
			int skipdraw=0;
			qp = zvec_get_at(quadpatches, i);

#if 0
			{
				vec3 d;
				vec3mov (d, qp->center);
				vec3sub (d, camera->pos);
				vec3normalize(&d);

				if (vec3dot(d, camera->rot.z_axis) < 0)
				{
					//outside of view cone
					cull++;
					skipdraw=1;
				}
				else
				if ( vec3dot(qp->avgnorm, camera->rot.z_axis) > .8)  //faces away from camera
				{
					cull++;
					skipdraw =1;
				}



			}
			#endif
			
			//draw
			if (qp->tag == QUADARRAY_TAG_NONE)
			{
				if (!skipdraw)
						gx_quadpatch_draw(qp);
				
			}
			
		}
}

