// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/ram.h"
#include "../vmath.h"
#include "../graphics/gx_sys.h"
#include "../graphics/gx_image.h"
#include "../graphics/gx_sprite.h"
#include "../graphics/gx_line.h"
#include "../graphics/gx_buffers.h"
#include "../graphics/gx_light.h"
#include "../graphics/gx_drawstyle.h"
#include "../structures/vector.h"
#include "../graphics/gx_misc.h"
#include "../graphics/gx_mesh.h"
#include "meshtree.h"

//math helper

//random float 0.0 to 1.0
float randf()
{
	return (rand()&255) / 255.0;
}


//random float -1.0 to 1.0
float randfs()
{
	return -1.0 + 2*(rand()&511) / 511.0;
}

//normalize a vector
void vec3norm( vec3* p)
{
	float d = sqrt( vec3abs_sq(*p)  );
	vec3scale( *p, (1/d) );
}

//camera stuff -todo move into library

typedef struct g_camera_s
{
	vec3 camera_pos;
	vec3 camera_forward;
	vec3 camera_right;
	vec3 camera_up;
} g_camera_t;

void g_camera_init(g_camera_t* cam)
{
	if (cam)
	{
		vec3set( cam->camera_pos,		0.0f, 0.0f, 0.0f);
		vec3set( cam->camera_right,		1.0f, 0.0f, 0.0f);
		vec3set( cam->camera_up,		0.0f, 1.0f, 0.0f);
		vec3set( cam->camera_forward,	0.0f, 0.0f, -1.0f);
	}
}


//patchs
#if 1

//patchS  ::todo: move to library if 'useful'
typedef struct patch_edge_s
{
	vec_t indirect_vertices; //pointer to vertices on the edge of this patch
	gx_vbuffer_t* vb;

	struct patch_s* patch0; 
	struct patch_s* patch1; 



} patch_edge_t;



//8 lod levels
#define MAXLEVELS 8

typedef struct patch_s
{
	gx_vbuffer_t* vb;	//holds points for this patch
	
	int numpoints;		//how many points are in this patch  (only for non-rectangulat patchs)
	int maxpoints;
	
	int width;			//w/h only for rectangular patchs
	int height;
	
	int* points;		//vertices within the vbuffer

	int levels;
	int indexstart[MAXLEVELS];
	int indexstop[MAXLEVELS];

	patch_edge_t* edges[4];

	vec3 center;
	float area;
	
	vec3 corner_min;
	vec3 corner_max;

} patch_t;

patch_edge_t* edge_mk(  patch_t* patch)
{

	patch_edge_t* edge = ram_alloc(sizeof(patch_edge_t), NULL);
	edge->patch0 = patch;
	if (patch)
	{
		edge->vb = patch->vb;
	}

	return edge;
}

#define PATCH_EDGE_BOTTOM	0
#define PATCH_EDGE_LEFT		1
#define PATCH_EDGE_RIGHT	2
#define PATCH_EDGE_TOP		3

//NOTE:  PATCH_POINT_AT is not safe!
#define PATCH_POINT_AT(SSSS,XXXX,YYYY)    ((SSSS)->points[   (SSSS)->width*(YYYY) + XXXX ] )

//create 2D patch.  Does not have any points filled out yet
patch_t * patch_mk(gx_vbuffer_t* vb, int width, int height)
{
	patch_t* patch = ram_alloc(sizeof(patch_t), NULL);
	int a;

	if (!patch)
		return;

	patch->numpoints = 0;
	patch->width = width;
	patch->height = height;
	patch->maxpoints = width*height;
	patch->points = ram_alloc(sizeof(zint32) * patch->maxpoints, NULL);
	patch->vb = vb;
//	patch->num_edges = 4;  //maybe support for triangular patches in the future
	
	patch->edges[0] = edge_mk(patch);
	patch->edges[1] = edge_mk(patch);
	patch->edges[2] = edge_mk(patch);
	patch->edges[3] = edge_mk(patch);


	vec_mk( &patch->edges[PATCH_EDGE_TOP]->indirect_vertices, width);
	vec_mk( &patch->edges[PATCH_EDGE_BOTTOM]->indirect_vertices, width);
	for (a=0;a<width;a++)
	{
		vec_add(&(patch->edges[PATCH_EDGE_TOP]->indirect_vertices), & PATCH_POINT_AT(patch, a, 0));
		vec_add(&(patch->edges[PATCH_EDGE_BOTTOM]->indirect_vertices), & PATCH_POINT_AT(patch, a, height-1));
	}
	
	vec_mk( &patch->edges[PATCH_EDGE_LEFT]->indirect_vertices, height);
	vec_mk( &patch->edges[PATCH_EDGE_RIGHT]->indirect_vertices, height);

	for (a=0;a<height;a++)
	{
		vec_add(&patch->edges[PATCH_EDGE_LEFT]->indirect_vertices, & PATCH_POINT_AT(patch, 0, a));
		vec_add(&patch->edges[PATCH_EDGE_RIGHT]->indirect_vertices, & PATCH_POINT_AT(patch, width-1, a));
	}

	return patch;
}

int patch_set_at( patch_t* s, int x, int y,  int vertex)
{
	if (!s)
		return GX_INDEX_INVALID;


	PATCH_POINT_AT(s, x, y) = vertex;

	return vertex;
}

void print_patch_points(patch_t* s)
{
	int a;
	for (a=0;a< s->width * s->height;a++)
	{
		if (a%s->width ==0) printf("\n");
		printf(" %02d", s->points[a]);
	}

}

//this function sets one edge of S to use vertices from T

#define		ASSIGN_INDICES 1
#define		COPY_POSITION  2

int steplevs[] = {1};

//{  64 , 32 ,16 ,8 , 4 , 2 ,1};

void index_patch(patch_t* patch)
{
	int i,j;	
 	int sstep = 1;
	int k;
	for (k=0;k< sizeof(steplevs)/sizeof(steplevs[0]);k++)
	{
		sstep = steplevs[k];

		patch->indexstart[k] = patch->vb->index_count;

		for (i=sstep;i<patch->width;i+=sstep)
		{
			for (j=sstep;j<patch->height;j+=sstep)
			{

				gx_vbuffer_add_index( patch->vb, PATCH_POINT_AT(patch, i-sstep, j-sstep));
				gx_vbuffer_add_index( patch->vb, PATCH_POINT_AT(patch, i,   j-sstep ));
				gx_vbuffer_add_index( patch->vb, PATCH_POINT_AT(patch, i,   j  ));
				gx_vbuffer_add_index( patch->vb, PATCH_POINT_AT(patch, i-sstep, j ));
				
				
				
			}
		}

		patch->indexstop[k] = patch->vb->index_count;
		patch->levels++;  //extra levels of detail

	}

}



//maybe:
void sew_patchs( patch_t* s, int s_edge, patch_t* t, int t_edge, int operation)
{
	int a;
	
	if (s->vb != t->vb)  //can't sew patchs that are in different vbuffers
		return;
	
	if (s->edges[s_edge]->indirect_vertices.count != t->edges[t_edge]->indirect_vertices.count)
		return;  //can't sew patchs that have different arity


	for (a=0;a< s->edges[s_edge]->indirect_vertices.count;a++)
	{
		if (operation & ASSIGN_INDICES)
		{
			*(int*)(s->edges[s_edge]->indirect_vertices.elements[a]) =  *(int*)(t->edges[t_edge]->indirect_vertices.elements[a]);
		}

		if (operation & COPY_POSITION)
		{
			vec3* spos = gx_vbuffer_v(s->vb, *(int*)(s->edges[s_edge]->indirect_vertices.elements[a]));
			vec3* tpos = gx_vbuffer_v(t->vb, *(int*)(t->edges[t_edge]->indirect_vertices.elements[a]));;
			
			vec3mov (*spos, *tpos);
		}

	}

}

//generate asteroid: todo: make a generic 'generate cube' function
//generate asteroid
#endif



//game structures



//game constants
#define STARCOUNT 2000

#define PATCHSIZE (64+1)

void junkpatch (gx_vbuffer_t* vb, patch_t* patch, float xstart, float xstop, float zstart, float zstop, float ylevel)
{
int i,j;
		float xsize = xstop-xstart;
		float zsize = zstop-zstart;

		for (i=0;i<PATCHSIZE;i++)
		{
			for(j=0;j<PATCHSIZE;j++)
			{
				float fi= ((float)i) / (PATCHSIZE-1);
				float fj= ((float)j) / (PATCHSIZE-1);
				float fk =  .05*sin((xstart+ fi*xsize)*10)  +.001*sin((zstart+ fj*zsize)*1000);
				

				//gx_vbuffer_add_tex(vb, 0,  fi    ,fj );
				gx_vbuffer_add_tex(vb, 0,  xstart+ fi*xsize   ,zstart+ fj*zsize );
				
				PATCH_POINT_AT(patch, i, j) = gx_vbuffer_add_vertex(vb, xstart+ fi*xsize, ylevel + fk , zstart+ fj*zsize);

//				vec3print(  *gx_vbuffer_v( vb, PATCH_POINT_AT(patch, i, j)));
//				printf(" s=%f t=%f\n", fi, fj);
			}
		}

		vec3set(patch->center,  (xstop+xstart) /2 , ylevel, (zstop+zstart) /2);
		patch->area = fabs(  (xstop-xstart) * (zstop-zstart)  );

		vec3set(patch->corner_min, xstart, ylevel, zstart);
		vec3set(patch->corner_max, xstop, ylevel, zstop);


}


#if 1
//maybe better sheet support

#define EDGE_LEFT    0
#define EDGE_RIGHT   1
#define EDGE_TOP     2
#define EDGE_BOTTOM  3


typedef struct quadarray_s
{
	gx_vbuffer_t* vb;
	
	zint32 count;  //# of vertices

	zint32 w;  //2d array dimension
	zint32 h;

	struct quadarray_s* children[4];
	struct quadarray_s* parent;
	int self; //which one of my parent children am i?



	vindex startindex;  //1st vertex
	vindex endindex; //

	vec3 center;  //center of the quadarray
	float size;   //area metric
	float dsize;   //distance metric


	//neighbors
	struct quadarray_s* left;
	struct quadarray_s* right;
	struct quadarray_s* up;
	struct quadarray_s* down;



	
	int tag;

} quadarray_t ;

void quadarray_draw(quadarray_t* qa)
{
	
	gx_vbuffer_draw(qa->vb, qa->startindex, qa->endindex,  gx_triangles, ztrue);
	
}


//create the quadarray
quadarray_t* quadarray_mk(int w, int h)
{
	quadarray_t *qa = NULL;
	int a;
	int b;

	qa = ram_alloc(sizeof(*qa), NULL);

	if (qa)
	{
		qa->count = w*h;
		qa->w=w;
		qa->h=h;
		qa->vb = gx_vbuffer_mk(w*h, w*h*3*2, zfalse, ztrue, 0);
		qa->startindex = 0;
 
		qa->vb->vertex_count = w*h;  //say all vertices are filled out

		qa->size = 1;

		//need to index to make triangles
		for (a=0;a<(w-1);a++)
		{
			for (b=0;b<(h-1);b++)
			{
				

				//tri 1
				gx_vbuffer_add_index( qa->vb, ((a+0) + (b+0)*w) );
				gx_vbuffer_add_index( qa->vb, ((a+1) + (b+1)*w) );
				gx_vbuffer_add_index( qa->vb, ((a+1) + (b+0)*w) );

				//tri 2
				gx_vbuffer_add_index( qa->vb, ((a+0) + (b+0)*w) );
				gx_vbuffer_add_index( qa->vb, ((a+0) + (b+1)*w) );
				gx_vbuffer_add_index( qa->vb, ((a+1) + (b+1)*w) );

				qa->endindex+=6;
			}
		}

	}
	return qa;
}

#define qa_vindex(qaaa, qxxx, qyyy)  (((qaaa)->w * (qyyy)) + (qxxx))

void quadarray_norm(quadarray_t* qa);


//order: 0,1
//       2,3

void quadarray_sew(quadarray_t* source, int source_edge, quadarray_t* dest, int dest_edge)
{
	int i;

	vec3* s;
	vec3* sn;
	

	if (dest->w != dest->h)
		return;
	if (dest->w != source->w)
		return;

	for (i=0;i<source->w;i++)
	{

		s = NULL;
		//get
		if (source_edge == EDGE_LEFT)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, 0,i));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, 0,i));
		}
		else if (source_edge == EDGE_RIGHT)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, source->w-1,i));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, source->w-1,i));
		}
		else if (source_edge == EDGE_TOP)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, i,0));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, i,0));
		}
		else if (source_edge == EDGE_BOTTOM)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, i,source->h-1));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, i,source->h-1));
		}


		//put
//		{
//		vec3 u;
//		vec3set(u, 0,.1,0);
//		vec3add(*s, u);
//		}
		

  		if (dest_edge == EDGE_LEFT)
		{
  			*gx_vbuffer_v( dest->vb, qa_vindex( dest, 0,i)) = *s; 
			*gx_vbuffer_n( dest->vb, qa_vindex( dest, 0,i)) = *sn; 
		}
  		else if (dest_edge == EDGE_RIGHT)
		{
			*gx_vbuffer_v( dest->vb, qa_vindex( dest, dest->w-1,i)) = *s;
			*gx_vbuffer_n( dest->vb, qa_vindex( dest, dest->w-1,i)) = *sn;
		}
  		else if (dest_edge == EDGE_TOP)
		{
  			*gx_vbuffer_v( dest->vb, qa_vindex( dest, i,0)) = *s;
			*gx_vbuffer_n( dest->vb, qa_vindex( dest, i,0)) = *sn;
		}
  		else if (dest_edge == EDGE_BOTTOM)
		{
  			*gx_vbuffer_v( dest->vb, qa_vindex( dest, i,dest->h-1)) = *s;
			*gx_vbuffer_n( dest->vb, qa_vindex( dest, i,dest->h-1)) = *sn;
		}
			

	}
}

// 0 1
// 2 3

void quadarray_patchup(quadarray_t* qa)
{
	quadarray_t* neighbor = NULL;

	if (! qa->parent)
		return;
#if 1
	//find neighbor to right of me.
	neighbor = NULL;

	if (qa->self == 0)
		neighbor = qa->parent->children[1];

	else if (qa->self == 2)
		neighbor = qa->parent->children[3];

	else if (qa->parent->right)
	{

		if (qa->self == 3)
			neighbor = qa->parent->right->children[2];
	
		if (qa->self == 1)
			neighbor = qa->parent->right->children[0];
	}
	

	if (neighbor)
	{
		qa->right = neighbor;
		neighbor->left = qa;
		quadarray_sew(neighbor, EDGE_LEFT, qa, EDGE_RIGHT);
	}
#endif

	//find neighbor to left of me
	neighbor = NULL;

	if (qa->self == 1)
		neighbor = qa->parent->children[0];

	else if (qa->self == 3)
		neighbor = qa->parent->children[2];

	else if (qa->parent->left)
	{

		if (qa->self == 0)
			neighbor = qa->parent->left->children[1];
	
		if (qa->self == 2)
			neighbor = qa->parent->left->children[3];
	}
	

	if (neighbor)
	{
		qa->left = neighbor;
		neighbor->right = qa;
		quadarray_sew(neighbor, EDGE_RIGHT, qa, EDGE_LEFT);
	}


	//find neighbor below
	neighbor = NULL;

	if (qa->self == 0)
		neighbor = qa->parent->children[2];

	else if (qa->self == 1)
		neighbor = qa->parent->children[3];

	else if (qa->parent->down)
	{

		if (qa->self == 2)
			neighbor = qa->parent->down->children[0];
	
		if (qa->self == 3)
			neighbor = qa->parent->down->children[1];
	}
	

	if (neighbor)
	{
		qa->down = neighbor;
		neighbor->up = qa;
		quadarray_sew(neighbor, EDGE_TOP, qa, EDGE_BOTTOM);
	}


//find neighbor above
	neighbor = NULL;

	if (qa->self == 2)
		neighbor = qa->parent->children[0];

	else if (qa->self == 3)
		neighbor = qa->parent->children[1];

	else if (qa->parent->up)
	{

		if (qa->self == 0)
			neighbor = qa->parent->up->children[2];
	
		if (qa->self == 1)
			neighbor = qa->parent->up->children[3];
	}
	

	if (neighbor)
	{
		qa->up = neighbor;
		neighbor->down = qa;
		quadarray_sew(neighbor, EDGE_BOTTOM, qa, EDGE_TOP);
	}


}

quadarray_t* quadarray_detail_2x(int self, quadarray_t* source, int a_start, int a_end, int b_start, int b_end)
{
	int a;
	int b;
	quadarray_t* dest;
	int w = (a_end-a_start-1) * 2+1 ;
	int h = (b_end-b_start-1) * 2+1 ;
	int x;
	int y;
	vec3 p;
	int div;

	dest= quadarray_mk(w,h);  //make new quadarray
	dest->self = self;

	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			a = x/2 + a_start;
			b = y/2 + b_start;
		
			vec3mov(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a,b)));
			div = 1;

			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a+1,b)))
					div++;
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a,b+1)))
					div++;
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a+1,b+1)))
					div++;
			}


			vec3scale(p, 1.0/div);


			//add noise
			{
				vec3 u;
				vec3set(u, 0,1,0);

				vec3madd(p, (rand()&0xff)/255.0 * .1* - source->dsize, u );
			}

			*gx_vbuffer_v( dest->vb, qa_vindex( dest, x, y)) = p;


		//	*gx_vbuffer_v( dest->vb, qa_vindex( dest, x, y))
		//	=
		//	*gx_vbuffer_v( source->vb, qa_vindex( source, a,b));

//				printf("copy %d %d  to %d %d\n", a,b,x,y);
		}
	}

	
	dest->size = source->size /4;  //decrease area by 4
	dest->dsize = source->dsize/2;  //decrease lengths by 2
	
	dest->center = *gx_vbuffer_v( dest->vb, qa_vindex( dest, dest->w/2,dest->h/2));
	dest->parent = source;
	

	quadarray_norm(dest);
	
	quadarray_patchup(dest); //find siblings and patch-up connections

	
	gx_vbuffer_update(dest->vb);
	return dest;
}

//split into 4 quadarrays
void quadarray_split(quadarray_t* source)
{
	if (source->children[0])
		return;  //already split

	//otherwise split it:

/*	source->children[0] = quadarray_detail_2x(source, 0, source->w/2+1, 0, source->h/2+1);
	source->children[1] = quadarray_detail_2x(source, source->w/2, source->w , 0, source->h/2+1);
	source->children[2] = quadarray_detail_2x(source, 0, source->w/2+1,        source->h/2,source->h );
	source->children[3] = quadarray_detail_2x(source, source->w/2, source->w , source->h/2, source->h);*/

	

	source->children[0] = quadarray_detail_2x(0, source, 0, source->w/2+1, 0, source->h/2+1);
	source->children[1] = quadarray_detail_2x(1, source, source->w/2, source->w , 0, source->h/2+1);
	

	source->children[2] = quadarray_detail_2x(2, source, 0, source->w/2+1,        source->h/2,source->h );
	source->children[3] = quadarray_detail_2x(3, source, source->w/2, source->w , source->h/2, source->h);	

	


	//child order: 0,1
	//             2,3


}






#endif

void calcnorm(vec3* n, vec3* a, vec3* b, vec3* c)
{
	vec3 ac;
	vec3 bc;
	float d;
	
	vec3mov(ac, *c);
	vec3sub(ac, *a);


	vec3mov(bc, *b);
	vec3sub(bc, *a);

	vec3cross(*n, ac,bc);
	 
	d = vec3abs_sq(*n);
	vec3scale(*n, 1/d);


}

void quadarray_norm(quadarray_t* qa)
{
	int a;
	int b;
	vec3 p;
	vec3 acc;
	int cnt=0;

	

	for (a=0;a< qa->w; a++)
	{
		for(b=0;b<qa->h;b++)
		{
			vec3set(acc,0,0,0);
			cnt=0;
#if 1
			if ((a< (qa->w-1)) && (b< (qa->h-1)))
			{
				cnt++;
				calcnorm(&p,
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a+1,b)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b+1)));
				vec3add(acc, p);
			}

#endif		

#if 1
			if ((a> 0) && (b< qa->w-1))
			{
				cnt++;
				calcnorm(&p,
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b+1)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a-1,b)));
				vec3add(acc, p);
			}
#endif


#if 1
			if ((a> 0) && (b> 0))
			{
				cnt++;
				calcnorm(&p,
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a-1,b)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b-1)));
					
				vec3add(acc, p);
			}
#endif

#if 1
			if ((a< (qa->w-1)) && (b>0 ))
			{
				cnt++;
				calcnorm(&p,
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b-1)),
					gx_vbuffer_v( qa->vb, qa_vindex( qa, a+1,b)));
					
				vec3add(acc, p);
			}
#endif

			if (cnt > 0)
			{
				float d;
				d = vec3abs_sq(acc);
				d = sqrt(d);
				vec3scale(acc,1/d);
				
			
			}

			else
			{
				vec3set(acc,0,1,0);
				printf(" ZERO\n");
				exit(0);
			}

			*gx_vbuffer_n(qa->vb, qa_vindex( qa, a,b)) = acc;

		}

	}

}

int main(int argc, char** argv)
{
	// IO variables
	zchar keypress=0;
	zint32 mouse_x=0;
	zint32 mouse_y=0;
	zbool  mouse_relative=zfalse;

	//game/graphics variables
	g_camera_t	player_camera;
	vec3		camera_inertia;

	int patchlevel=0;

	//need a starfield (we ARE in space)
	gx_vbuffer_t*	starfield = NULL;

	
	vec_t*	 quadarrays;
	
	
	gx_light_t* light = NULL;


	gx_image_t		*spacerock = NULL;  //holds rock texture for asteroids
	gx_drawstyle_t	spacerock_ds;   //drawstyle for the asteroid
	
	
	gx_image_t* hf = NULL;

	//meshtree_t* mtree = NULL;
	
	patch_t*  patches[100];
	
	int numpatches=0;

	gx_vbuffer_t* vb =  NULL;

	quadarray_t* qa = NULL;



	//Initialize graphics
	gx_init(1024, 768 , "Test");
	gx_clear_color(0,0,0,1);
	//gx_mouse_capture(ztrue);  //mouse input will be relative 
	
	//Load assets
	spacerock  = gx_image_load_tga( "spacerock.tga");
	spacerock_ds.textures = &spacerock;
	spacerock_ds.numtextures=1;


	

	//initialize game data
	g_camera_init(&player_camera);
	vec3set(camera_inertia, 0,0,0);

	
	//make light
	{
		vec3 lpos;
		vec3 lcolor;
		vec3 lamb;

		vec3set (lpos, 0,1,0);
		vec3set(lcolor, .5,.3,.3);
		vec3set(lamb, .1,.1,.1);


		light = gx_light_mk( gx_light_directional, &lpos, &lcolor, &lamb );
	}

	//create stars

	starfield = gx_vbuffer_mk(STARCOUNT,0,ztrue, zfalse, 0);		
	{
		int i;  
		for (i=0;i<STARCOUNT;i++)
		{
			vec3 p;
			float s;

			vec3set(p, randf()-.5,randf()-.5,randf()-.5);
			
			//normalize
			
			s=sqrt(vec3abs_sq(p));
			vec3scale( p, ( 1.0/s )  );

			gx_vbuffer_add_color(starfield, .7+.3*randf(),.7+.3*randf(),.7+.3*randf(),1);
			gx_vbuffer_add_vertex(starfield, p.vec3x, p.vec3y, p.vec3z);

		}
	}
	gx_vbuffer_update(starfield);

	//lets create some meshtree stuff
#if 0	
	{
		meshtree_node_t* initial_node = ram_alloc(sizeof( meshtree_node_t), NULL);
		edgetree_node_t* edges[3];
		vindex vertices[3];

		int i;

		mtree = ram_alloc(sizeof (meshtree_t), NULL);
		mtree->vbuffer = gx_vbuffer_mk( 65535,65535,zfalse, zfalse, 1);
		vec_mk(&mtree->nodes, 4);

		
		gx_vbuffer_add_tex( mtree->vbuffer, 0, 0.0,0.0);
		vertices[0] = gx_vbuffer_add_vertex( mtree->vbuffer, 0,-.5, -1);

		gx_vbuffer_add_tex( mtree->vbuffer, 0, 1.0,0.0);
		vertices[1] = gx_vbuffer_add_vertex( mtree->vbuffer, 1,-.5, -1);


		gx_vbuffer_add_tex( mtree->vbuffer, 0, 1.0,1.0);
		vertices[2] = gx_vbuffer_add_vertex( mtree->vbuffer, 1,-.5, -2);
		
		for(i=0;i<3;i++)
		{
			edges[i] = ram_alloc(sizeof (edgetree_node_t), NULL);	
			edges[i]->vertex[0] = vertices[i];
			edges[i]->vertex[1] = vertices[ (i+1)% 3];
			edges[i]->triangles[0] = initial_node;
			edges[i]->vbuffer = mtree->vbuffer;
			
			initial_node->edges[i] = edges[i];
			initial_node->vertex[i] = vertices[i];

		}

		

		vec_add(&mtree->nodes, initial_node);

		

		printf("set\n");
	}
#endif


	//vb = gx_vbuffer_mk(10000000,10000000,zfalse, zfalse, 1);

	
	

//	gx_vbuffer_update(vb);
	
	quadarrays = vec_mk(NULL, 32);
	

	//make quadarray
	//qa = quadarray_mk(65,65);  //make mesh of 64 by 64 quads( 65by65 points)
	qa = quadarray_mk(33,33);
	//qa = quadarray_mk(9,9);
	qa->size = .25; //start at root size
	qa->dsize = .1;

	{
		float d;

		int a,b;
		for (a=0;a < qa->w;a++)
		{
			for (b=0;b<qa->h;b++)
			{
				vec3* vv = gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b));
				vv->named.x = ((a-qa->w/2)/  (float) (qa->w-1)) *sqrt(2) ;
				vv->named.z =  ((b-qa->h/2)/ (float) (qa->h-1)) *sqrt(2) ;
				vv->named.y =  1;
				
				//now normalize
		
				d = vec3abs_sq(*vv);
				d=sqrt(d);
				vec3scale(*vv, 1/d);

				vv->named.z -= 2;
				vv->named.y -= 2;
				//vv->named.y = -1 +(rand() & 0xff) / 255.0 * .1;

					//do normal
				vv = gx_vbuffer_n( qa->vb, qa_vindex( qa, a,b));
				vec3set(*vv, 0,1,0);

			}
		}

		qa->center = *gx_vbuffer_v( qa->vb, qa_vindex( qa, qa->w/2,qa->h/2));

		//qa->mesh = gx_mesh_def( qa->vb, NULL, qa->startindex, qa->endindex, ztrue);
		
		quadarray_norm(qa);
		gx_vbuffer_update(qa->vb);
		

		vec_add(quadarrays, qa);
	}
	

	//create an image
#if 0
	{	
		int start;
		int end;
		gx_vbuffer_t* vv  = NULL;
		int a;
		int b;
		hf = gx_image_mk(128,128,GX_IMAGE_GRAY);
 
		for (a=0;a<128;a++)
		{
			for (b=0;b<128;b++)
			{
				hf->data[b*128 +a] = 127 + 10 * (sin( (float)a/20.0) + cos((float)b/20.0));
			}
		}

		//make heightfield from it
		vv = gx_vbuffer_from_image(NULL, hf, 0, -1, -5, 0, 1, 2, 5, 5, 5, zfalse, 0, &start, &end, zfalse);

		gx_vbuffer_update(vv);
		hmesh = gx_mesh_def(vv, NULL, start, end, ztrue);


	}
#endif

	//The game loop 
	for(;;)  
	{
 		gx_window_event();  //handles any window events (I/O)

		//set up projection matrix for this frame
 		gx_setup_3d( 70.0f,  gx_frame_get_dimensions(NULL,NULL), .01f, 1000.0f);
		
		//Read mouse input
		gx_mouse_pos(&mouse_x, &mouse_y, &mouse_relative);
		if (!mouse_relative)
		{
			//if some some reason we get an absolute mouse position, clear it out
			mouse_x=0;
			mouse_y=0;
		}
	
	

		//read keyboard input
		keypress = gx_getkey();  //reads decoded ascii chars
		
		if (keypress=='m')
		{
			//toggle the mouse capture
			gx_mouse_capture( mouse_relative ^ 1);
		}
		else if (keypress=='~')
		{
			break;
		}
		else if (keypress =='+')
		{
//			patchlevel= (patchlevel +1) % patch->levels;
		}
		else if (keypress=='-')
		{
//			if (patchlevel > 0)
///				patchlevel--;
			
		}else if (keypress == '9')
		{
			//9 will split the 1st quadarray

			quadarray_t* qa = vec_get_at(quadarrays, 0);
			quadarray_t* dest[4];
			if (qa)
			{
				vec_remove_unordered(quadarrays, 0); //remove the one we just took
				
				quadarray_split(qa, dest);

				vec_add(quadarrays, dest[0]);
				vec_add(quadarrays, dest[1]);
				vec_add(quadarrays, dest[2]);
				vec_add(quadarrays, dest[3]);

			}
		}
			


		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;
		
			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);
	
			if (gx_key_state('w')) delta_pos.vec3z+=.001;
			if (gx_key_state('s')) delta_pos.vec3z=-.001;
			if (gx_key_state('a')) delta_pos.vec3x=-.001;
			if (gx_key_state('d')) delta_pos.vec3x=+.001;
			if (gx_key_state('r')) delta_pos.vec3y=+.001;
			if (gx_key_state('f')) delta_pos.vec3y=-.001;

			if (gx_key_state('x')) vec3set(camera_inertia, 0,0,0);

			if (gx_key_state('q')) delta_roll=-.02;
			if (gx_key_state('e')) delta_roll=.02;

			if (gx_key_state('4')) delta_yaw=-.02;
			if (gx_key_state('6')) delta_yaw=.02;

			if (gx_key_state('8')) delta_pitch=-.02;
			if (gx_key_state('2')) delta_pitch=.02;

			//move camera using camera's basis
			vec3madd( camera_inertia, delta_pos.vec3x, player_camera.camera_right);
			vec3madd( camera_inertia, delta_pos.vec3y, player_camera.camera_up);
			vec3madd( camera_inertia, delta_pos.vec3z, player_camera.camera_forward);

			vec3madd( player_camera.camera_pos, .1 , camera_inertia);

			//lets spin camera  (relative to its own coord system)

			delta_pitch += mouse_y*.003;
			delta_yaw += mouse_x*.003;

			gx_spin(ztrue, delta_yaw, delta_pitch, delta_roll,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);

		}

		//clear screen		
		gx_frame_clear(ztrue,ztrue);
		
		//draw starfield
		gx_camera_home();
		gx_camera_pos_rot(NULL,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward); 
		gx_zbuffer(zfalse);
		
		gx_drawstyle_activate(NULL) ;
		gx_vbuffer_draw(starfield, 0, starfield->vertex_count, gx_points, zfalse);

		gx_zbuffer(ztrue);
		//render here

		gx_camera_pos_rot( &player_camera.camera_pos,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);
	
		gx_drawstyle_activate(&spacerock_ds);

//		mt_update(mtree,  &player_camera.camera_pos);
		
		
		//gx_vbuffer_draw( mtree->vbuffer, 0, mtree->vbuffer->index_count, gx_triangles, ztrue);


		gx_set_active_lights(&light, 1);
		{
			gx_drawstyle_t ds;
			ram_clear(&ds, sizeof(ds));
		
			vec3set( ds.specular_color, 1,1, 1);
			ds.specular_exponent = 10;
//			ds.blending = gx_blend_alpha;		
//			ds.alpha = .5;
			gx_drawstyle_activate(&ds);
		}
		
		
		
		

		{ 
			int i;
		//	printf(" %d patches ", vec_count(quadarrays));

			for (i=0;i<vec_count(quadarrays);i++)
			{
				qa = vec_get_at(quadarrays, i);
				if (qa->tag == 0)
					quadarray_draw(qa);
				else
				{
					qa->tag = 0; //clear tag
					vec_remove_unordered(quadarrays, i);
					i--; //repeat this position 
					//printf(" removed patch\n");
				
				}

			}
			printf("  %d patches\n", vec_count(quadarrays));
		}

		gx_set_active_lights(NULL, 0);

		gx_frame_show();  //show the frame



		//evaluate quadarrays
		{
			int i;
			int ocount = 0;

			ocount = vec_count(quadarrays);

			for(i=0;(i<vec_count(quadarrays)) && (i < ocount);i++)
			{
				vec3 p;
				float d;
				quadarray_t* qa = vec_get_at(quadarrays, i);

				//find its distance from camera
				vec3mov(p, player_camera.camera_pos);
				vec3sub(p, qa->center);
				d = vec3abs_sq(p);
				d = sqrt(d);
				
//				printf(" distance: %f  size: %f\n", d, qa->size / d );

		
				if (qa->tag)
					continue;

				if ( (qa->size/(d*d)) > .02)
				{
					quadarray_t* dest[4];
					//remove the current qa
					
					quadarray_split(qa);
					if (qa->children[0])
					{
						qa->tag = 1; //tag it
						
						vec_add(quadarrays, qa->children[0]);
						vec_add(quadarrays, qa->children[1]);
						vec_add(quadarrays, qa->children[2]);
						vec_add(quadarrays, qa->children[3]);
					}
				}
				else if ( qa->parent )
				{
					vec3mov(p, player_camera.camera_pos);
					vec3sub(p, qa->parent->center);
					d = vec3abs_sq(p);
					d = sqrt(d);
					
					if ((qa->parent->size/(d*d)) < (.02) )
					{  //combine threshhold
					
						vec_add(quadarrays, qa->parent); //put parent back in


						//tag all siblings
						qa->parent->children[0]->tag = 1;
						qa->parent->children[1]->tag = 1;
						qa->parent->children[2]->tag = 1;
						qa->parent->children[3]->tag = 1;
					}
				}
		
			}

		}

	}

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
