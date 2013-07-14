// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define FADE_PATCHES
#define ALPHA_SPEED .03


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
#include "../structures/vector.h"
#include "../graphics/gx_drawstyle.h"

#include "../graphics/gx_misc.h"
#include "../graphics/gx_mesh.h"
#include "meshtree.h"

//math helper

//random float 0.0 to 1.0


int srandf(float x)
{
	int seed = x * RAND_MAX;
	int seed2 = x* 17*5;
	seed += seed2;
	seed = seed % RAND_MAX;
	srand(seed);
}

float randf()
{
	
	return  rand() /  (float)RAND_MAX;
	
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





//game structures



//game constants
#define STARCOUNT 2000

#define PATCHSIZE (64+1)




#if 1
//maybe better sheet support

#define EDGE_LEFT    0
#define EDGE_RIGHT   1
#define EDGE_TTT     2
#define EDGE_BBB  3




typedef struct quadarray_s
{
	gx_vbuffer_t* vb;
	
	zint32 count;  //# of vertices

	zint32 w;  //2d array dimension
	zint32 h;

	struct quadarray_s* children[4];
	struct quadarray_s* parent;
	int self; //which one of my parent children am i?

	zbool useskirt;

	vindex startindex;  //1st vertex
	vindex endindex; //
	vindex endindex_noskirt;
	//zbool dirtyindex; //must rebuild indexbuffer before drawing

	vec3 origin; //center of planet

	vec3 center;  //center of the quadarray
	float size;   //area metric
	float dsize;   //distance metric

	vec3 avgnorm; //average normal

	//neighbors
//	struct quadarray_s* left;
//	struct quadarray_s* right;
//	struct quadarray_s* up;
//	struct quadarray_s* down;
	struct quadarray_s* adjacent[4];      //the 4 adjacent neighbors at the same detail level
	int adjacent_edge[4]; //the neighbor's edge I am touching
	int rev[4];  //joined to something in reverse order

	int tag;
	
	int onlevel; //true if this is the level being drawn

	int generation; // +1 each time split

	float alpha; //for fadeout


	//for 
	int use_offset;
	vec3  offset; // for high precision crap
	float scale;
	
	int boo;  //todo remove
} quadarray_t ;
#define qa_vindex(qaaa, qxxx, qyyy)  (((qaaa)->w * (qyyy)) + (qxxx))


// 0 1
// 2 3

//edge child arrays
int edge_left_child[] = {0,2};
int edge_right_child[] = {1,3};
int EDGE_TTT_child[] = {0,1};
int EDGE_BBB_child[] = {2,3};

int* edge_child[] = { edge_left_child, edge_right_child, EDGE_TTT_child, EDGE_BBB_child};

char* edge_names[] = {"LEFT", "RIGHT", "BOTTOM", "TOP"};

int edge_opposite[] = { EDGE_RIGHT, EDGE_LEFT, EDGE_BBB, EDGE_TTT};


quadarray_t* find_neighbor(quadarray_t* q, int edge, int* nedge, int* rev)
{
	int x;
	int y;
	int child_num;
	int edge_child_n = 0;
	*rev=0;
	

	if (!q->parent)
		return NULL; //can't

	if (q->adjacent[edge]) 
	{
		printf("already done\n");
		return q->adjacent[edge];
	}
	
	// decompose my 'child number' into x and y
	x = q->self & 1;
	y = q->self >> 1;

	if (edge == EDGE_LEFT || edge == EDGE_RIGHT)
			edge_child_n = y;
	else
			edge_child_n = x;

	//find coords of neigbor

	if (edge == EDGE_LEFT)
		x--;

	if (edge == EDGE_RIGHT)
		x++;

	if (edge == EDGE_TTT)
		y--;

	if (edge == EDGE_BBB)
		y++;

	if (((x==0 || x==1)) && ((y==0||y==1)))
	{ 
		//neighbor has same parent (within a group of 4)
	

		*nedge = edge_opposite[edge];
		

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

	

#if 0
	//todo: fill out all the tables

	if (x==2) {  //too far to the right
		
		x=0;
		if (!q->parent->adjacent[EDGE_RIGHT])
			return NULL;

		*nedge = q->parent->adjacent_edge[EDGE_RIGHT];

		
		child_num = (y << 1) | x ;

		return q->parent->adjacent[EDGE_RIGHT]->children[child_num];

		
	}

#endif

	return NULL;
}

//void quadarray_reindex(quadarray_t* qa);

void quadarray_draw(quadarray_t* qa)
{
	//
	//if (qa->use_offset)
//	{
	//	gx_move3d(&qa->offset);
	//}
	vec3 off;


	int i;


	if (qa->boo)
		return;

//	int f=0;

	vec3set (off, 0, .01, .01);
/*
	for(i=0;i<4;i++)
		if (! qa->adjacent[i] || 
			(qa->adjacent[i] && 
			!qa->adjacent[i]->onlevel &&
			qa->adjacent[i]->parent &&
			qa->adjacent[i]->parent->onlevel))
				f=1;
*/

	//if (f)
	//	gx_move3d(&off);

	//	if (f && qa->parent)
	//		gx_vbuffer_draw(qa->parent->vb, qa->parent->startindex, qa->parent->endindex,  gx_triangles, ztrue);
		

//	if (qa->dirtyindex)
//		quadarray_reindex(qa);

	if (qa->useskirt)

		gx_vbuffer_draw(qa->vb, qa->startindex, qa->endindex,  gx_triangles, ztrue);
	else 
		gx_vbuffer_draw(qa->vb, qa->startindex, qa->endindex_noskirt,  gx_triangles, ztrue);
	//if(f)
	//	gx_home();

	//if (qa->use_offset)
//	{
	//	gx_home();
//	}

}



int add_pentagon(gx_vbuffer_t* vb, zuint32 v0,zuint32 v1,zuint32 v2,zuint32 v3,zuint32 v4)
//int add_triangle5(gx_vbuffer_t* vb, zuint32 v0,zuint32 v1,zuint32 v2, zuint32 v3,zuint32 v4  )
{
	gx_vbuffer_add_index(vb, v0);
	gx_vbuffer_add_index(vb, v1);
	gx_vbuffer_add_index(vb, v2);

	gx_vbuffer_add_index(vb, v0);
	gx_vbuffer_add_index(vb, v2);
	gx_vbuffer_add_index(vb, v4);

	gx_vbuffer_add_index(vb, v2);
	gx_vbuffer_add_index(vb, v3);
	gx_vbuffer_add_index(vb, v4);

	return 9;
}

int edge_lowdetail(quadarray_t* qa, int edge)
{

if (
		/* if item has neighbor, neighbor is not active but neighbor's parent is */
		(
			qa->adjacent[edge] && 
			!qa->adjacent[edge]->onlevel && 
			qa->adjacent[edge]->parent &&
			qa->adjacent[edge]->parent->onlevel)
		

		||

		/*or don't have  neighbor, but parent's neighbor is on level */
		(
			! qa->adjacent[edge] && 
			qa->parent && 
			qa->parent->adjacent[edge] &&
			qa->parent->adjacent[edge]->onlevel)
	)

		return 1;

	return 0;

}

#if 0
void quadarray_reindex(quadarray_t* qa)
{
	//all 4 sides low detail
	int top_lowdetail=0;
	int bottom_lowdetail=0;
	int left_lowdetail=0;
	int right_lowdetail=0;
	
	int a;
	int b;
	int h = qa->h;
	int w = qa->w;




	
	if (edge_lowdetail(qa, EDGE_LEFT))
		left_lowdetail = 1;

	if (edge_lowdetail(qa, EDGE_RIGHT))
		right_lowdetail = 1;

	if (edge_lowdetail(qa, EDGE_BBB))
		top_lowdetail = 1;

	if (edge_lowdetail(qa, EDGE_TTT))
		bottom_lowdetail = 1;



	qa->endindex = qa->startindex; //restart
	gx_vbuffer_clear(qa->vb, ztrue, zfalse);


	//fill, with triangles
	//normally a from 0 to w-1  , and b from, 0 to h-1
	// if left_lowdetail, etc are set, move, start, end inside from edge
	for (a=left_lowdetail;a<(qa->w-1-right_lowdetail);a++)
	{
		for (b=bottom_lowdetail;b<(qa->h-1-top_lowdetail);b++)
		{
	
			//tri 1
			gx_vbuffer_add_index( qa->vb, ((a+0) + (b+0)*qa->w) );				
			gx_vbuffer_add_index( qa->vb, ((a+1) + (b+0)*qa->w) );
			gx_vbuffer_add_index( qa->vb, ((a+1) + (b+1)*qa->w) );

			//tri 2
			gx_vbuffer_add_index( qa->vb, ((a+0) + (b+0)*qa->w) );		
			gx_vbuffer_add_index( qa->vb, ((a+1) + (b+1)*qa->w) );
			gx_vbuffer_add_index( qa->vb, ((a+0) + (b+1)*qa->w) );

			qa->endindex+=6;
		}
	}


	//low do all 4 edges

	#if 1
		//left edge 
	if (left_lowdetail)
	{
		a = 0;
		for (b=0;b<(h-2);b+=2)
		{		 
	
			if (b==h-3 && top_lowdetail)
					continue;

			if (b==0 && bottom_lowdetail)
					continue;

			qa->endindex+= add_pentagon(qa->vb,
										qa_vindex(qa, a+0, b+0), 
										qa_vindex(qa, a+1 , b+0), 
										qa_vindex(qa, a+1, b+1),
										qa_vindex(qa, a+1, b+2),
										qa_vindex(qa, a+0 , b+2));
					
		}
	}
#endif
#if 1
		//right edge
		if (right_lowdetail)
		{



			a = w-2;
			for (b=0;b<(h-2);b+=2)
			{		 


			if (b==h-3 && top_lowdetail)
					continue;

			if (b==0 && bottom_lowdetail)
					continue;

				qa->endindex+= add_pentagon(qa->vb,
											qa_vindex(qa, a+0, b+0) , 
											qa_vindex(qa, a+1, b+0), 
											qa_vindex(qa, a+1, b+2) ,
											qa_vindex(qa, a+0, b+2),
											qa_vindex(qa, a+0, b+1));
						
			}
		}
#endif
#if 1
		//top edge 
		if (top_lowdetail)
		{
			b = h-2;
			for (a=0;a<(w-2);a+=2)
			{		 
		
			if (a== 0  &&  left_lowdetail)
					continue;

			if (a== w-3  &&  right_lowdetail)
					continue;

				qa->endindex+= add_pentagon(qa->vb,
											qa_vindex(qa, a+0 , b+1), 
											qa_vindex(qa, a+0 , b+0), 
											qa_vindex(qa, a+1 , b+0),
											qa_vindex(qa, a+2 , b+0),
											qa_vindex(qa, a+2 , b+1));
						
			}
		}
#endif
#if 1
		if (bottom_lowdetail)
		{
			//bottom edge 
			b = 0;
			for (a=0;a<(w-2);a+=2)
			{		 

					if (a== 0  &&  left_lowdetail)
					continue;

				if (a== w-3  &&  right_lowdetail)
					continue;
		

				qa->endindex+= add_pentagon(qa->vb,
											qa_vindex(qa, a+2 , b+0),
											qa_vindex(qa, a+2 , b+1),
											qa_vindex(qa, a+1 , b+1),
											qa_vindex(qa, a+0 , b+1),
											qa_vindex(qa, a+0 , b+0) 
											
											
											
											
											);
						
			}
		}
#endif

	gx_vbuffer_update_indices(qa->vb);
//	qa->dirtyindex = 0;

	return;
}
#endif

#define QUADARRAY_SKIRTS

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
#ifdef QUADARRAY_SKIRTS
		qa->vb = gx_vbuffer_mk(w*h  +2*w + 2*h, w*h*3*2 + w*3*2 +h*3*2, zfalse, ztrue, 1);
#else
		qa->vb = gx_vbuffer_mk(w*h, w*h*3*2, zfalse, ztrue, 0);
#endif
		qa->startindex = 0;
 
		qa->vb->vertex_count = w*h;  //say all vertices are filled out

		qa->generation = 1;
		qa->size = 1;

		//reindex the quadarray
	//	quadarray_reindex(qa);
	//	qa->dirtyindex = 1;

#if 1
		//need to index to make triangles
		for (a=0;a<(w-1);a++)
		{
			for (b=0;b<(h-1);b++)
			{	

				//tri 1
				gx_vbuffer_add_index( qa->vb, qa_vindex(qa,a,b));
				gx_vbuffer_add_index( qa->vb, qa_vindex(qa,a+1,b) );
				gx_vbuffer_add_index( qa->vb, qa_vindex(qa,a+1,b+1) );

				//tri 2
				gx_vbuffer_add_index( qa->vb, qa_vindex(qa,a,b) );		
				gx_vbuffer_add_index( qa->vb, qa_vindex(qa,a+1,b+1) );
				gx_vbuffer_add_index( qa->vb, qa_vindex(qa,a,b+1) );

				qa->endindex+=6;
			}
		}

		//now add skirts

#ifdef QUADARRAY_SKIRTS
		

#define qa_skirtindex(qa, s, i)    (((qa)->w * (qa)->h) + ((s)*(qa)->w) + (i))

		qa->endindex_noskirt = qa->endindex;

		for (b=0;b< (h-1); b++)
		{
			//left skirt
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,0,b)    );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,0,b) );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,0,b+1) );

			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,0,b)    );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,0,b+1) );
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,0,b+1)    );

			qa->endindex+=6;


			//right skirt
		
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_RIGHT,b)    );
			
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,h-1,b+1) );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,h-1,b) );

			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_RIGHT,b)    );
			
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_RIGHT,b+1)    );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,h-1,b+1) );

			qa->endindex+=6;


			//top skirt
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_BBB,b)    );
			
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,b+1 ,0) );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,b , 0) );

			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_BBB,b)    );
			
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_BBB,b+1)    );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,b+1 , 0) );

			qa->endindex+=6;


			//bottom skirt
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_TTT,b)    );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,b , h-1) );
			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,b+1 ,h-1) );
			


			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_TTT,b+1)    );
			gx_vbuffer_add_index( qa->vb, qa_skirtindex(qa,EDGE_TTT,b)    );
			

			gx_vbuffer_add_index( qa->vb, qa_vindex(qa,b+1 , h-1) );

			qa->endindex+=6;


		}

#endif

#endif

		
	}
	return qa;
}


void quadarray_skirt( quadarray_t* qa)
{
	int i;
	
	vec3 p;
	float skirtd = qa->dsize*2 ;

	qa->useskirt = ztrue;
	for (i=0;i<qa->h;i++)
	{
		//left
		vec3mov( *gx_vbuffer_n(qa->vb, qa_skirtindex(qa, 0, i)), *gx_vbuffer_n(qa->vb, qa_vindex(qa, 0, i)));
		vec3mov( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, 0, i)), *gx_vbuffer_v(qa->vb, qa_vindex(qa, 0, i)));
		vec3sub(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, 0, i)), qa->origin);
		vec3scale( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, 0, i)), 1-skirtd);
		vec3add(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, 0, i)), qa->origin);


		//right
		vec3mov( *gx_vbuffer_n(qa->vb, qa_skirtindex(qa, EDGE_RIGHT, i)), *gx_vbuffer_n(qa->vb, qa_vindex(qa, qa->w-1, i)));
		vec3mov( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_RIGHT, i)), *gx_vbuffer_v(qa->vb, qa_vindex(qa, qa->w-1, i)));
		vec3sub(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_RIGHT, i)), qa->origin);
		vec3scale( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_RIGHT, i)), 1-skirtd);
		vec3add(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_RIGHT, i)), qa->origin);

		//top
		vec3mov( *gx_vbuffer_n(qa->vb, qa_skirtindex(qa, EDGE_BBB, i)), *gx_vbuffer_n(qa->vb, qa_vindex(qa,  i,0)));
		vec3mov( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_BBB, i)), *gx_vbuffer_v(qa->vb, qa_vindex(qa,  i,0)));
		vec3sub(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_BBB, i)), qa->origin);
		vec3scale( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_BBB, i)), 1-skirtd);
		vec3add(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_BBB, i)), qa->origin);

		//bottom
		vec3mov( *gx_vbuffer_n(qa->vb, qa_skirtindex(qa, EDGE_TTT, i)), *gx_vbuffer_n(qa->vb, qa_vindex(qa,  i,qa->h-1)));
		vec3mov( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_TTT, i)), *gx_vbuffer_v(qa->vb, qa_vindex(qa,  i,qa->h-1)));
		vec3sub(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_TTT, i)), qa->origin);
		vec3scale( *gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_TTT, i)), 1-skirtd);
		vec3add(*gx_vbuffer_v(qa->vb, qa_skirtindex(qa, EDGE_TTT, i)), qa->origin);

	}



}


void quadarray_norm(quadarray_t* qa);


//order: 0,1
//       2,3

#define QUADARRAY_TAG_NONE        0
#define QUADARRAY_TAG_REMOVE	    1
#define QUADARRAY_TAG_DRAW_ONLY     2


void quadarray_sew(quadarray_t* source, int source_edge, quadarray_t* dest, int dest_edge, int rev)
{
	int i;
	int is;

	vec3* s;
	vec3* sn;
	
	if (!source || !dest)
		return;


	if (dest->w != dest->h)
		return;
	if (dest->w != source->w)
		return;

	source->adjacent[source_edge] = dest;
	source->adjacent_edge[source_edge] = dest_edge;
	source->rev[source_edge] = rev;

	dest->adjacent[dest_edge] = source;
	dest->adjacent_edge[dest_edge] = source_edge;
	dest->rev[dest_edge] = rev;
		


	for (i=0;i<source->w;i++)
	{

		s = NULL;

		if (rev)
			is = source->w -1-i;
		else 
			is = i;


		//get
		if (source_edge == EDGE_LEFT)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, 0,is));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, 0,is));
		}
		else if (source_edge == EDGE_RIGHT)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, source->w-1,is));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, source->w-1,is));
		}
		else if (source_edge == EDGE_TTT)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, is,0));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, is,0));
		}
		else if (source_edge == EDGE_BBB)
		{
			s = gx_vbuffer_v( source->vb, qa_vindex( source, is,source->h-1));
			sn= gx_vbuffer_n( source->vb, qa_vindex( source, is,source->h-1));
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
  		else if (dest_edge == EDGE_TTT)
		{
  			*gx_vbuffer_v( dest->vb, qa_vindex( dest, i,0)) = *s;
			*gx_vbuffer_n( dest->vb, qa_vindex( dest, i,0)) = *sn;
		}
  		else if (dest_edge == EDGE_BBB )
		{
  			*gx_vbuffer_v( dest->vb, qa_vindex( dest, i,dest->h-1)) = *s;
			*gx_vbuffer_n( dest->vb, qa_vindex( dest, i,dest->h-1)) = *sn;
		}
			

	}
}

// 0 1
// 2 3







#if 0
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

	else if (qa->parent->adjacent[EDGE_RIGHT])
	{
		
		if (qa->self == 3)
			neighbor = qa->parent->adjacent[EDGE_RIGHT]->children[2];
	
		if (qa->self == 1)
			neighbor = qa->parent->adjacent[EDGE_RIGHT]->children[0];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_RIGHT] = neighbor;
		neighbor->adjacent[EDGE_LEFT] = qa;
		quadarray_sew(neighbor, EDGE_LEFT, qa, EDGE_RIGHT);
	}
#endif

	//find neighbor to left of me
	neighbor = NULL;

	if (qa->self == 1)
		neighbor = qa->parent->children[0];

	else if (qa->self == 3)
		neighbor = qa->parent->children[2];

	else if (qa->parent->adjacent[EDGE_LEFT])
	{

		if (qa->self == 0)
			neighbor = qa->parent->adjacent[EDGE_LEFT]->children[1];
	
		if (qa->self == 2)
			neighbor = qa->parent->adjacent[EDGE_LEFT]->children[3];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_LEFT] = neighbor;
		neighbor->adjacent[EDGE_RIGHT] = qa;
		quadarray_sew(neighbor, EDGE_RIGHT, qa, EDGE_LEFT);
	}


	//find neighbor below
	neighbor = NULL;

	if (qa->self == 0)
		neighbor = qa->parent->children[2];

	else if (qa->self == 1)
		neighbor = qa->parent->children[3];

	else if (qa->parent->adjacent[EDGE_BBB])
	{

		if (qa->self == 2)
			neighbor = qa->parent->adjacent[EDGE_BBB]->children[0];
	
		if (qa->self == 3)
			neighbor = qa->parent->adjacent[EDGE_BBB]->children[1];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_BBB] = neighbor;
		neighbor->adjacent[EDGE_TTT] = qa;
		quadarray_sew(neighbor, EDGE_TTT, qa, EDGE_BBB);
	}


//find neighbor above
	neighbor = NULL;

	if (qa->self == 2)
		neighbor = qa->parent->children[0];

	else if (qa->self == 3)
		neighbor = qa->parent->children[1];

	else if (qa->parent->adjacent[EDGE_TTT])
	{

		if (qa->self == 0)
			neighbor = qa->parent->adjacent[EDGE_TTT]->children[2];
	
		if (qa->self == 1)
			neighbor = qa->parent->adjacent[EDGE_TTT]->children[3];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_TTT] = neighbor;
		neighbor->adjacent[EDGE_BBB] = qa;
		quadarray_sew(neighbor, EDGE_BBB, qa, EDGE_TTT);
	}


}
#endif



#if 0
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

	else if (qa->parent->adjacent[EDGE_RIGHT])
	{
		
		if (qa->self == 3)
			neighbor = qa->parent->adjacent[EDGE_RIGHT]->children[2];
	
		if (qa->self == 1)
			neighbor = qa->parent->adjacent[EDGE_RIGHT]->children[0];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_RIGHT] = neighbor;
		neighbor->adjacent[EDGE_LEFT] = qa;
		quadarray_sew(neighbor, EDGE_LEFT, qa, EDGE_RIGHT);
	}
#endif

	//find neighbor to left of me
	neighbor = NULL;

	if (qa->self == 1)
		neighbor = qa->parent->children[0];

	else if (qa->self == 3)
		neighbor = qa->parent->children[2];

	else if (qa->parent->adjacent[EDGE_LEFT])
	{

		if (qa->self == 0)
			neighbor = qa->parent->adjacent[EDGE_LEFT]->children[1];
	
		if (qa->self == 2)
			neighbor = qa->parent->adjacent[EDGE_LEFT]->children[3];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_LEFT] = neighbor;
		neighbor->adjacent[EDGE_RIGHT] = qa;
		quadarray_sew(neighbor, EDGE_RIGHT, qa, EDGE_LEFT);
	}


	//find neighbor below
	neighbor = NULL;

	if (qa->self == 0)
		neighbor = qa->parent->children[2];

	else if (qa->self == 1)
		neighbor = qa->parent->children[3];

	else if (qa->parent->adjacent[EDGE_BBB])
	{

		if (qa->self == 2)
			neighbor = qa->parent->adjacent[EDGE_BBB]->children[0];
	
		if (qa->self == 3)
			neighbor = qa->parent->adjacent[EDGE_BBB]->children[1];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_BBB] = neighbor;
		neighbor->adjacent[EDGE_TTT] = qa;
		quadarray_sew(neighbor, EDGE_TTT, qa, EDGE_BBB);
	}


//find neighbor above
	neighbor = NULL;

	if (qa->self == 2)
		neighbor = qa->parent->children[0];

	else if (qa->self == 3)
		neighbor = qa->parent->children[1];

	else if (qa->parent->adjacent[EDGE_TTT])
	{

		if (qa->self == 0)
			neighbor = qa->parent->adjacent[EDGE_TTT]->children[2];
	
		if (qa->self == 1)
			neighbor = qa->parent->adjacent[EDGE_TTT]->children[3];
	}
	

	if (neighbor)
	{
		qa->adjacent[EDGE_TTT] = neighbor;
		neighbor->adjacent[EDGE_BBB] = qa;
		quadarray_sew(neighbor, EDGE_BBB, qa, EDGE_TTT);
	}
}
#endif


#if 1

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
	float fdiv;

	srandf(  source->center.named.x + source->center.named.y  + source->center.named.z);

	dest= quadarray_mk(w,h);  //make new quadarray
	dest->self = self;
	dest->origin = source->origin;

	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			a = x/2 + a_start;
			b = y/2 + b_start;

			
		
			vec3mov(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a,b)));
			div = 1;
			fdiv = sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qa_vindex( source, a,b))));

			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a+1,b)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qa_vindex( source, a+1,b))));
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qa_vindex( source, a,b+1))));
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				vec3add(p, *gx_vbuffer_v( source->vb, qa_vindex( source, a+1,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*gx_vbuffer_v( source->vb, qa_vindex( source, a+1,b+1))));
			}


			vec3scale(p, 1.0/div);  //unit scale
		//	fdiv /= div;  //average length
		//	printf("div %d fdiv  %f\n", div, fdiv);
			//vec3scale(p, 1.0/fdiv); //round out

/*
			//add noise
			{
				vec3 u;
				vec3set(u, randf(),randf(),randf());

				vec3madd(p, ( .2+ ((rand()&0xff))/256.0) *- .1*  source->dsize, u );
			}*/


		



			*gx_vbuffer_v( dest->vb, qa_vindex( dest, x, y)) = p;


			//texcoord: don't interpolate coordinates
			gx_vbuffer_s(dest->vb, qa_vindex( dest, x, y),0) = ((float) x) / (w-1);
			gx_vbuffer_t(dest->vb, qa_vindex( dest, x, y),0) = ((float) y) / (w-1);


		//	*gx_vbuffer_v( dest->vb, qa_vindex( dest, x, y))
		//	=
		//	*gx_vbuffer_v( source->vb, qa_vindex( source, a,b));

//				printf("copy %d %d  to %d %d\n", a,b,x,y);
		}
	}

	quadarray_norm(dest);
	//add noise
	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			vec3* p;
			vec3* n;

		//	if (   ((x&1)|(y&1)) == 0)
		//		continue; //skip 



		//	float d;

		//	float r= randf();
		//	vec3 u;
		//	vec3set(u, randf(),randf(),randf());
			p = gx_vbuffer_v( dest->vb, qa_vindex( dest, x, y));
		//	n = gx_vbuffer_n( dest->vb, qa_vindex( dest, x, y));

			vec3sub(*p, source->origin); 
			{	float r = randf();
				vec3scale(*p,  1.0  -  r*   source->dsize );
			}
			vec3add(*p, source->origin);


			//d = sqrt(vec3abs_sq(

			//vec3norm( p); //round out

//			displace in direction of normal
		//	vec3madd(*p, -randf()  *  source->dsize, *n );

			//displace straight up and down
		//	vec3scale(*p, .9);
	//		vec3scale(*p,  1.0  - (randf()*source->dsize)  );
			
		}

	}

	
	dest->size = source->size /4;  //decrease area by 4
	dest->dsize = source->dsize/2;  //decrease lengths by 2
	//dest->dsize = source->dsize/2.5; 
	
	dest->center = *gx_vbuffer_v( dest->vb, qa_vindex( dest, dest->w/2,dest->h/2));
	dest->parent = source;
	

	quadarray_norm(dest);
	
	//quadarray_patchup(dest); //find siblings and patch-up connections
	
	if (1) {
		int edge;
		int nedge;
		int rev;

		for (edge=0;edge<4;edge++)
		{
			quadarray_t* n = find_neighbor( dest, edge, &nedge,&rev);
			quadarray_sew(n, nedge, dest, edge,rev);
		}


	}

	
	//gx_vbuffer_update(dest->vb);

#ifdef QUADARRAY_SKIRTS
	quadarray_skirt(dest);
#endif

	return dest;
}


#endif

#if 0
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

	vec3 offset;
	float scale=1;
	
	vec3set(offset, 0,0,0);

	dest= quadarray_mk(w,h);  //make new quadarray
	dest->self = self;

	//check for scaling crap
	{
		float d;
		
		vec3mov(p, *gx_vbuffer_v( source->vb, qa_vindex( source, 0,0)));
		vec3sub(p, *gx_vbuffer_v( source->vb, qa_vindex( source, 1,1)));

		d = vec3abs_sq(p);
		d = sqrt(d);

#if 0
		if (source->use_offset)
		{
			vec3mov( dest->offset, source->offset);
			dest->scale = source->scale;
			source->use_offset = 1;
		}
		else
		{
			dest->scale = 1;
			vec3set(dest->offset, 0,0,0);
		}
		

		if (d < .005)
		{
			//scale = 10;  
			//vec3mov(offset, *gx_vbuffer_v( source->vb, qa_vindex( source, 0,0)));
			//scale = 1.001;
			//offset.named.y += .001;

			offset =  *gx_vbuffer_v( source->vb, qa_vindex( source, 0,0));
			//vec3set(offset, 1, 1, 1);
	
		
			dest->use_offset = 1;

			vec3add(dest->offset, offset);

			dest->scale *= scale;
		}
#endif
		


	}



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

			vec3sub(p, offset);
			vec3scale(p, scale);

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

//	vec3scale(dest->center, dest->scale);
//	vec3add(dest->center, dest->offset);
	//ve

	dest->parent = source;
	

	quadarray_norm(dest);
	
//	quadarray_patchup(dest); //find siblings and patch-up connections

	
	gx_vbuffer_update(dest->vb);
	return dest;
}
#endif

/*
void dirty_edges(quadarray_t* qa)
{
	int i;
	if (!qa) 
		return;

	qa->dirtyindex=1;
	for (i=0;i<4;i++)
		if (qa->adjacent[i])
			qa->adjacent[i]->dirtyindex=1;
}
*/

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

	source->children[0]->generation = source->generation+1;
	source->children[1]->generation = source->generation+1;
	source->children[2]->generation = source->generation+1;
	source->children[3]->generation = source->generation+1;



	//connect siblings together
	//0 1
	//2 3

//	quadarray_sew( source->children[0], EDGE_RIGHT, source->children[1], EDGE_LEFT);
//	quadarray_sew( source->children[2], EDGE_RIGHT, source->children[3], EDGE_LEFT);

//	quadarray_sew(source->children[1], EDGE_BBB, source->children[3], EDGE_TTT);
//	quadarray_sew(source->children[0], EDGE_BBB, source->children[2], EDGE_TTT);

	//have to solve neighbors
	
	


	//update vbuffers
	gx_vbuffer_update(source->children[0]->vb);
	gx_vbuffer_update(source->children[1]->vb);
	gx_vbuffer_update(source->children[2]->vb);
	gx_vbuffer_update(source->children[3]->vb);




	//printf(" Generation %d \n",  source->generation+1);
	//child order: 0,1
	//             2,3


}






#endif
#if 0
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
#endif
#if 1
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
#endif


void quadarray_norm(quadarray_t* qa)
{
	int a;
	int b;
	vec3 p;
	vec3 acc;
	int cnt=0;

	vec3 avg;
	int  avg_cnt=0;
	vec3set(avg, 0,0,0);
	

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
				
				vec3add(avg, acc);
				avg_cnt++;
				
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

	{
		float d;
		d = vec3abs_sq(avg);
		
		
		
		d = sqrt(d);
		vec3scale(avg,1.0/avg_cnt);
		vec3mov(qa->avgnorm, avg);
	}

}
quadarray_t* closest = NULL;

int main(int argc, char** argv)
{
	// IO variables
	zchar keypress=0;
	zint32 mouse_x=0;
	zint32 mouse_y=0;
	zbool  mouse_relative=zfalse;
	int i;
	int j;
	int k;
	//game/graphics variables
	g_camera_t	player_camera;
	vec3		camera_inertia;

	int patchlevel=0;

	//need a starfield (we ARE in space)
	gx_vbuffer_t*	starfield = NULL;

	quadarray_t* rootqa[6];

	gx_drawstyle_t ds_qa;
	
	vec_t*	 quadarrays;

	

	vec_t*	 quadarrays_fadeout;  //quadarrays fading out
	vec_t*	 quadarrays_fadein;  //quadarrays fading out
	


	quadarray_t*	 atmosqa[6];
	quadarray_t*	 oceanqa[6];
	
	gx_light_t* light = NULL;


	gx_image_t		*spacerock = NULL;  //holds rock texture for asteroids
	gx_drawstyle_t	spacerock_ds;   //drawstyle for the asteroid
	
	
	gx_image_t* hf = NULL;


	gx_image_t* atmosphere = NULL;
	gx_sprite_t* atmos_s = NULL;

	//meshtree_t* mtree = NULL;
	
//	patch_t*  patches[100];
	
	int numpatches=0;

	gx_vbuffer_t* vb =  NULL;

	quadarray_t* qa = NULL;

	vec3 skycolor;
	vec3 fogcolor;

	vec3set(skycolor, .7,.2,.1);
	vec3set(fogcolor, .6,.6,.6);



	//Initialize graphics
	gx_init(800, 600 , "Tri Mesh Quad Tree");
	//gx_clear_color(0,0,0,1);

	gx_clear_color(.7,.7,.7,1);
	//gx_mouse_capture(ztrue);  //mouse input will be relative 
	
	//Load assets
	spacerock  = gx_image_load_tga( "rocktile.tga");
//	spacerock_ds.textures = &spacerock;
//	spacerock_ds.numtextures=1;
	vec_mk(&spacerock_ds.textures,1);
	vec_add(&spacerock_ds.textures, spacerock);


//	atmosphere  = gx_image_load_tga( "atmosphere.tga");

//	atmos_s = gx_sprite_mk(atmosphere, 0, 0, 256,256,3 ,3);

	//initialize game data
	g_camera_init(&player_camera);
	vec3set(camera_inertia, 0,0,0);

	player_camera.camera_pos.named.z +=2;
	player_camera.camera_pos.named.y +=1;
	//make light
	{
		vec3 lpos;
		vec3 lcolor;
		vec3 lamb;

		vec3set (lpos, sqrt(3)/3,sqrt(3)/3,sqrt(3)/3);
		vec3set(lcolor, 0.7,0.7,0.7);
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
	
	
	//low detail fading out
	quadarrays_fadeout = vec_mk(NULL, 32);
	
	//low detail fading in
	quadarrays_fadein = vec_mk(NULL, 32);


	

	//create a stupud perect sphere for atmosphere

	for (i=0;i<6;i++)
	{

		qa = quadarray_mk(33,33);

				
#define CUBE_TOP 0
#define CUBE_BOTTOM 1
#define CUBE_FRONT 2
#define CUBE_BACK 3
#define CUBE_LEFT 4
#define CUBE_RIGHT 5

		{
			float d;

			int a,b;
			for (a=0;a < qa->w;a++)
			{
				for (b=0;b<qa->h;b++)
				{
					vec3* vv = gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b));

					float fa;
					float fb;

					fa = ((a-qa->w/2)/  (float) (qa->w-1)) *2 ;
					fb  =  ((b-qa->h/2)/ (float) (qa->h-1)) *2 ;

					
					
					gx_vbuffer_s(qa->vb, qa_vindex( qa, a,b),0) = fa /2+1.0;
					gx_vbuffer_t(qa->vb, qa_vindex( qa, a,b),0) = fb /2+1.0;

										
					switch (i)
					{
					case CUBE_TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case CUBE_BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case CUBE_FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case CUBE_BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case CUBE_LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case CUBE_RIGHT:
						vec3set(*vv, 1, fb, -fa);
						break;

					}


					d = vec3abs_sq(*vv);
					d=sqrt(d);
					vec3scale(*vv, 1/d);

				

					vec3scale(*vv, 1.1);

					

					//vec3add(*vv, origin);

			
					//do normal
					vv = gx_vbuffer_n( qa->vb, qa_vindex( qa, a,b));
					vec3set(*vv, 0,1,0);

				}
			}

			qa->center = *gx_vbuffer_v( qa->vb, qa_vindex( qa, qa->w/2,qa->h/2));
			
					
			quadarray_norm(qa);
		
			atmosqa[i] = qa;
			gx_vbuffer_update(qa->vb);
			
		}
	}

//create a stupud perect sphere for ocean

	for (i=0;i<6;i++)
	{

		qa = quadarray_mk(33,33);

				
#define CUBE_TOP 0
#define CUBE_BOTTOM 1
#define CUBE_FRONT 2
#define CUBE_BACK 3
#define CUBE_LEFT 4
#define CUBE_RIGHT 5

		{
			float d;

			int a,b;
			for (a=0;a < qa->w;a++)
			{
				for (b=0;b<qa->h;b++)
				{
					vec3* vv = gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b));

					float fa;
					float fb;

					fa = ((a-qa->w/2)/  (float) (qa->w-1)) *2 ;
					fb  =  ((b-qa->h/2)/ (float) (qa->h-1)) *2 ;

					
					
					gx_vbuffer_s(qa->vb, qa_vindex( qa, a,b),0) = fa /2+1.0;
					gx_vbuffer_t(qa->vb, qa_vindex( qa, a,b),0) = fb /2+1.0;

										
					switch (i)
					{
					case CUBE_TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case CUBE_BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case CUBE_FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case CUBE_BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case CUBE_LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case CUBE_RIGHT:
						vec3set(*vv, 1, fb, -fa);
						break;

					}


					d = vec3abs_sq(*vv);
					d=sqrt(d);
					vec3scale(*vv, 1/d);

				

					vec3scale(*vv, .97);

					

					//vec3add(*vv, origin);

			
					//do normal
					vv = gx_vbuffer_n( qa->vb, qa_vindex( qa, a,b));
					vec3set(*vv, 0,1,0);

				}
			}

			qa->center = *gx_vbuffer_v( qa->vb, qa_vindex( qa, qa->w/2,qa->h/2));
			
					
			quadarray_norm(qa);
		
			oceanqa[i] = qa;
			gx_vbuffer_update(qa->vb);
			
		}
	}


	//generate planet terrain

	for (k=0;k<1;k++) {

		vec3 origin;

		//vec3set(origin, 10*k,0,0);

		vec3set(origin, 0,0,0);

	//all faces
	for (i=0;i<6;i++)
	{

		//make quadarray
		//qa = quadarray_mk(257,257); 
		//qa = quadarray_mk(129,129);
		qa = quadarray_mk(65,65);  //make mesh of 64 by 64 quads( 65by65 points)
		//qa = quadarray_mk(33,33);
		//qa = quadarray_mk(17,17);
	//	qa = quadarray_mk(9,9);
		//qa = quadarray_mk(3,3);
		
		
		//qa->size = .3; //start at root size
		//qa->dsize = .1;

		qa->onlevel = 1;

		qa->size=  5.0 ;
		//qa->dsize = .015  ;
		qa->dsize = .004  ;

		qa->origin = origin;

//		qa->size =  qa->size / ((qa->w-1) * (qa->h-1));
//		qa->dsize =  qa->dsize / (qa->w );
		
#define CUBE_TOP 0
#define CUBE_BOTTOM 1
#define CUBE_FRONT 2
#define CUBE_BACK 3
#define CUBE_LEFT 4
#define CUBE_RIGHT 5
		{
			float d;

			int a,b;
			for (a=0;a < qa->w;a++)
			{
				for (b=0;b<qa->h;b++)
				{
					vec3* vv = gx_vbuffer_v( qa->vb, qa_vindex( qa, a,b));

					float fa;
					float fb;

					fa = ((a-qa->w/2)/  (float) (qa->w-1)) *2 ;
					fb  =  ((b-qa->h/2)/ (float) (qa->h-1)) *2 ;

			
					
					gx_vbuffer_s(qa->vb, qa_vindex( qa, a,b),0) = fa /2+1.0;
					gx_vbuffer_t(qa->vb, qa_vindex( qa, a,b),0) = fb /2+1.0;

					srandf(fa+fb+a+b);
					
					switch (i)
					{
					case CUBE_TOP: 
						vec3set(*vv, fa, 1, -fb);
						break;

					case CUBE_BOTTOM:
						vec3set(*vv, fa, -1, fb);
						break;


					case CUBE_FRONT:
						vec3set(*vv, fa, fb, 1);
						break;

					case CUBE_BACK:
						vec3set(*vv, -fa, fb, -1);
						break;

						
					case CUBE_LEFT:
						vec3set(*vv, -1, fb, fa);
						break;

					case CUBE_RIGHT:
						vec3set(*vv, 1, fb, -fa);
						break;

					}

					//now normalize
			
					{
						vec3 vvf;

//						vec3set(vvf, ((int) (5*vv->named.x)) /5.0,  ((int) (7*vv->named.y)) /7.0,  ((int) (3*vv->named.z)) /3.0   );
//						d = vec3abs_sq(vvf); 

					}

					d = vec3abs_sq(*vv);
					d=sqrt(d);
					vec3scale(*vv, 1/d);

					//make a little rough
					if (k!=1)
						vec3scale(*vv, 1 - randf() * qa->dsize);

			//		vv->named.z -= 2;
				//	vv->named.x += 60*k ;


					if (k==1)
					{
							vec3scale(*vv, 1.1);

					}

					vec3add(*vv, origin);


					

				
					
				//	vv->named.y -= (rand() & 0xff) / 255.0 * .05;

					
					//SCALE HUGE
					//vec3scale(*vv, 5000 );

						//do normal
					vv = gx_vbuffer_n( qa->vb, qa_vindex( qa, a,b));
					vec3set(*vv, 0,1,0);

				}
			}

			qa->center = *gx_vbuffer_v( qa->vb, qa_vindex( qa, qa->w/2,qa->h/2));
			

			//qa->mesh = gx_mesh_def( qa->vb, NULL, qa->startindex, qa->endindex, ztrue);
			
			quadarray_norm(qa);
			//gx_vbuffer_update(qa->vb);
			
			
		
			vec_add(quadarrays, qa);
			rootqa[i] = qa;

			
			
		}
	}
	
	
	//quadarray_sew(rootqa[3], EDGE_LEFT, rootqa[4], EDGE_RIGHT);

//	quadarray_sew(rootqa[CUBE_BACK], EDGE_LEFT, rootqa[CUBE_LEFT], EDGE_RIGHT);

	//loop	
	quadarray_sew(rootqa[CUBE_FRONT], EDGE_LEFT, rootqa[CUBE_LEFT], EDGE_RIGHT,0);
	quadarray_sew(rootqa[CUBE_FRONT], EDGE_RIGHT, rootqa[CUBE_RIGHT], EDGE_LEFT,0);
	quadarray_sew(rootqa[CUBE_BACK], EDGE_LEFT, rootqa[CUBE_RIGHT], EDGE_RIGHT,0);
	quadarray_sew(rootqa[CUBE_BACK], EDGE_RIGHT, rootqa[CUBE_LEFT], EDGE_LEFT,0);


	//top
	quadarray_sew(rootqa[CUBE_TOP], EDGE_TTT, rootqa[CUBE_FRONT], EDGE_BBB,0);
	quadarray_sew(rootqa[CUBE_TOP], EDGE_LEFT, rootqa[CUBE_LEFT], EDGE_BBB,1);
	quadarray_sew(rootqa[CUBE_TOP], EDGE_RIGHT, rootqa[CUBE_RIGHT], EDGE_BBB,0);
	quadarray_sew(rootqa[CUBE_TOP], EDGE_BBB, rootqa[CUBE_BACK], EDGE_BBB,1);


	//bottom
	quadarray_sew(rootqa[CUBE_BOTTOM], EDGE_BBB, rootqa[CUBE_FRONT], EDGE_TTT,0);
	quadarray_sew(rootqa[CUBE_BOTTOM], EDGE_LEFT, rootqa[CUBE_LEFT], EDGE_TTT,0);
	quadarray_sew(rootqa[CUBE_BOTTOM], EDGE_RIGHT, rootqa[CUBE_RIGHT], EDGE_TTT,1);
	quadarray_sew(rootqa[CUBE_BOTTOM], EDGE_TTT, rootqa[CUBE_BACK], EDGE_TTT,1);



//	rootqa[3]->adjacent[EDGE_LEFT] = rootqa[4];
//	rootqa[4]->adjacent[EDGE_RIGHT] = rootqa[3];

	//update
	for (i=0;i<6;i++)
	{
		quadarray_skirt(rootqa[i]);
		gx_vbuffer_update(rootqa[i]->vb);

	}



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
 		gx_setup_3d( 80.0f,  gx_frame_get_dimensions(NULL,NULL),0.000001f, 10.0f);
		
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
			
		}else if (keypress == 'h')
		{
			vec3scale(camera_inertia, .5);
		}
			


		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;
		
			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);
	
			//speed of motion
#define SSS .001
			if (gx_key_state('w')) delta_pos.vec3z+=SSS;
			if (gx_key_state('s')) delta_pos.vec3z=-SSS;
			if (gx_key_state('a')) delta_pos.vec3x=-SSS;
			if (gx_key_state('d')) delta_pos.vec3x=+SSS;
			if (gx_key_state('r')) delta_pos.vec3y=+SSS;
			if (gx_key_state('f')) delta_pos.vec3y=-SSS;


			

			if (gx_key_state('x')) {
				vec3set(camera_inertia, 0,0,0);
				delta_roll=0;
				delta_yaw=0;
				delta_pitch=0;
			}

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


		if (1){
			vec3 pos;

			float dist;
			//vec3 fogc;
		//	vec3 skyc;
		//	vec3set(fogc, .7,.5,.5);
		//	vec3set(skyc, .5,.3,.3);

			vec3mov(pos, player_camera.camera_pos);
			//subtract planet center( currently just 0,0,0)
			
			dist = vec3abs_sq(pos);
			dist = sqrt(dist);

			gx_light_fog(NULL, 0, 0);
			gx_clear_color(0,0,0,0);


			if (dist < 1.2)
			{
				float d2;
				if (dist > 1.1)
				{
					d2 = 1-fabs(dist - 1.1) / (1.2-1.1);
					d2 +=.1;
					if (d2 > 1)
							d2=1;
					printf( "atmos dist %f\n", d2);
					gx_clear_color(d2*skycolor.array[0], d2*skycolor.array[1], d2*skycolor.array[2], 0);
				}
				else
				{
					gx_clear_color(skycolor.array[0], skycolor.array[1], skycolor.array[2], 0);
				}

				

				
				
				//gx_clear_color(1,0,0,0);
				{
					float t = (dist -1) / (1.1-1);
					if (t<0) t=0;

					gx_light_fog(&skycolor,0,  t * 1 + (1-t)*.5   );
				}
			}


			//gx_light_fog(&fogc, 0, .3);
/*
			if (dist > 1.6)
			{
				gx_clear_color(0,0,0,0);
				gx_light_fog(NULL,0,0);
			}
			else if (dist < 1.2)
			{
				gx_clear_color(skyc.array[0], skyc.array[1], skyc.array[2],0);
				gx_light_fog(&fogc, 0,.3);
			}
			else
			{
				dist = dist - 1.2;
				dist = 1-fabs(dist *10);
				gx_clear_color(dist*skyc.array[0], dist*skyc.array[1], dist*skyc.array[2],0);
				gx_light_fog(&fogc, 0, 1.6-dist);
			}
*/

		}
		//clear screen		
		gx_frame_clear(ztrue,ztrue);
		
		//draw starfield
		gx_camera_home();
		gx_camera_pos_rot(NULL,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward); 
		gx_zbuffer(zfalse);
		
		
		gx_set_active_lights(NULL,0);
		gx_drawstyle_activate(NULL) ;
		gx_vbuffer_draw(starfield, 0, starfield->vertex_count, gx_points, zfalse);

		gx_zbuffer(ztrue);
		//render here

		gx_camera_pos_rot( &player_camera.camera_pos,&player_camera.camera_right, &player_camera.camera_up, &player_camera.camera_forward);
	
		//gx_drawstyle_activate(&spacerock_ds);

//		mt_update(mtree,  &player_camera.camera_pos);
		
		
		//gx_vbuffer_draw( mtree->vbuffer, 0, mtree->vbuffer->index_count, gx_triangles, ztrue);

	/*	{	
			vec3 spos;
			//vec3set(spos, 0,0,0);
		//	vec3mov(spos, player_camera.camera_forward);
			vec3scale(spos, -.5);
			gx_sprite_draw_3d( atmos_s, &spos, &player_camera.camera_up, &player_camera.camera_right, ztrue, ztrue);

		}
		*/

		


		gx_set_active_lights(&light, 1);
		
	//	gx_set_active_lights(NULL,0);
		
		ram_clear(&ds_qa, sizeof(ds_qa));
		
		vec3set( ds_qa.specular_color, 1,1, 1);
		ds_qa.specular_exponent = 10;
		ds_qa.blending = gx_blend_nothing;
		ds_qa.alpha = .5;

		memcpy(&ds_qa.textures, &spacerock_ds.textures, sizeof(spacerock_ds.textures));
	//	ds_qa.textures = spacerock_ds.textures;
	//	ds_qa.numtextures = 1;

		gx_drawstyle_activate(&ds_qa);
		
				
		//printf(" %d opaque patches \n", vec_count(quadarrays));
		//draw opaque patches
		{
			int cull=0;
			int i;

			for (i=0;i< vec_count(quadarrays);i++)
			{
				int skipdraw=0;
				qa = vec_get_at(quadarrays, i);

				{
					vec3 d;
					vec3mov (d, qa->center);
					vec3sub (d, player_camera.camera_pos);
					vec3norm(&d);

					if (vec3dot(d, player_camera.camera_forward) < 0)
					{
						//outside of view cone
						cull++;
						skipdraw=1;
					}
					else
					if ( vec3dot(qa->avgnorm, player_camera.camera_forward) > .8)  //faces away from camera
					{
						cull++;
						skipdraw =1;
					}



				}
				
				//draw
				if (qa->tag == QUADARRAY_TAG_NONE || qa->tag == QUADARRAY_TAG_DRAW_ONLY)
				{
					if (!skipdraw)
							quadarray_draw(qa);
					
				}
				else if (qa->tag == QUADARRAY_TAG_REMOVE) //remove from opaque draw list
				{
					qa->tag = QUADARRAY_TAG_NONE; //clear tag
					vec_remove_unordered(quadarrays, i);
					i--; //repeat this position 
				}
			}
		}

		ds_qa.use_constant_alpha = 1;

#ifdef FADE_PATCHES
		//draw fadeout patches (low detail fading out into high detail below it)
		{ 
			int i;

			for (i=0;i<vec_count(quadarrays_fadeout);i++)
			{
				qa = vec_get_at(quadarrays_fadeout, i);
				if (qa->alpha > 0)
				{
					ds_qa.blending = gx_blend_alpha;		
					ds_qa.alpha = qa->alpha;
					gx_drawstyle_activate(&ds_qa);

					quadarray_draw(qa);
					qa->alpha -= ALPHA_SPEED;
				}
				else
				{
					vec_remove_unordered(quadarrays_fadeout, i);
					i--; //repeat this position
					
					//untag the children
					qa->children[0]->tag=0;
					qa->children[1]->tag=0;
					qa->children[2]->tag=0;
					qa->children[3]->tag=0;
								
				}

			}
			//printf("  %d patches fadeout\n", vec_count(quadarrays_fadeout));
		}


		//draw fadein patches
#if 1
		{ 
			int i;
		//	printf(" %d patches ", vec_count(quadarrays));

			for (i=0;i<vec_count(quadarrays_fadein);i++)
			{
//				exit(1);
				qa = vec_get_at(quadarrays_fadein, i);
				
				{
					ds_qa.blending = gx_blend_alpha;		
					ds_qa.alpha = qa->alpha;
					gx_drawstyle_activate(&ds_qa);

					quadarray_draw(qa);
		 			qa->alpha +=ALPHA_SPEED;
			//		printf("%f\n", qa->alpha);
				}
				if (qa->alpha > 1)
				{
					vec_remove_unordered(quadarrays_fadein, i);
					i--; //repeat this position 
					//add to running patch
					vec_add(quadarrays, qa);
					qa->tag=0;
					//tag children for removal

					qa->children[0]->tag=1;
					qa->children[1]->tag=1;
					qa->children[2]->tag=1;
					qa->children[3]->tag=1;

				}

			}
			//printf("  %d patches fadein\n", vec_count(quadarrays_fadein));
		}
#endif
#endif

		//gx_set_active_lights(&light, 1);


if (0) {

			//draw ocean
			float d;
			float fc[] = {0,0,0,0};
			gx_light_t ll;

			gx_light_t* lll;

			gx_drawstyle_t ds;
			ram_clear(&ds, sizeof(ds));
			ram_clear(&ll, sizeof(ll));
			//vec3mov(ds.specular_color, skycolor);
			//ds.specular_exponent = 0;

			ll.light_type = gx_light_point;
			vec3mov(ll.color, skycolor);
			//vec3scale(ll.color, .6);
			
			vec3mov(ll.position, player_camera.camera_pos);
			//vec3scale(ll.position, -1);
			
			//vec3scale (ll.ambient, .25);
			
			//lll = &ll;
			//gx_set_active_lights(&lll, 1);


			
			ds.use_constant_alpha = ztrue;
			ds.blending = 0; 
			gx_drawstyle_activate(&ds);

			


			d = vec3abs_sq(player_camera.camera_pos);
			d = sqrtf(d);
			//gx_light_fog( fc, 0, d+2);
			gx_light_fog( NULL,0,0);

			
			for (i=0;i<6;i++)
			{
				quadarray_draw(oceanqa[i]);

			}
		}




		gx_set_active_lights(NULL, 0);

		{

			//draw atmos
			float d;
			float fc[] = {0,0,0,0};
			gx_light_t ll;

			gx_light_t* lll;

			gx_drawstyle_t ds;
			ram_clear(&ds, sizeof(ds));
			ram_clear(&ll, sizeof(ll));
			//vec3mov(ds.specular_color, skycolor);
			//ds.specular_exponent = 0;

			ll.light_type = gx_light_point;
			vec3mov(ll.color, skycolor);
			//vec3scale(ll.color, .6);
			
			vec3mov(ll.position, player_camera.camera_pos);
			//vec3scale(ll.position, -1);
			
			//vec3scale (ll.ambient, .25);
			
			lll = &ll;
			gx_set_active_lights(&lll, 1);


			
			ds.use_constant_alpha = ztrue;
			ds.blending = gx_blend_add; 
			gx_drawstyle_activate(&ds);

			


			d = vec3abs_sq(player_camera.camera_pos);
			d = sqrtf(d);
			//gx_light_fog( fc, 0, d+2);
			gx_light_fog( NULL,0,0);

			
			for (i=0;i<6;i++)
			{
				quadarray_draw(atmosqa[i]);

			}
		}


		gx_light_fog( NULL,0,0);


		



		gx_frame_show();  //show the frame

#ifdef FADE_PATCHES
		//remove anything that was tagged for it
	 	{
			int i;
			for (i=0;i<vec_count(quadarrays);i++)
			{
				quadarray_t* qa = vec_get_at(quadarrays, i);
				if (qa->tag==1)
				{
					vec_remove_unordered(quadarrays, i);
					i--;
				}

			}

		}
#endif

		//evaluate quadarrays
		{
			int i;
			int ocount = 0;
			int splitcount=0;
			int splitlimit=1000;

			
			
			float closest_d=0;//quick:find the closest one

			if (closest)
				closest->boo = 0;

			closest = NULL;

			for (i=0;i<vec_count(quadarrays);i++)
			{
				vec3 p;
				float d;
				quadarray_t* qa = vec_get_at(quadarrays, i);

				vec3mov(p, qa->center);
				//vec3norm(&p);
				vec3sub(p, player_camera.camera_pos);
				
				if (!closest ||  (d=vec3abs_sq(p)) < closest_d)
				{
					closest_d = d;
					closest = qa;

				}

			}
			
			if (closest)
			{
			
				vec3 p;
				float dplayer;
				float d;
				int i;
				int j;

				vec3 p_closest;
				float closest_point_d=10000000;


				//closest->boo=1;  //flag to draw differently

				//ok now check all these points against the camera to make sure we don't go into the planet
				//(slow)
				dplayer = vec3abs_sq(player_camera.camera_pos);  // planet center is at 0,0, so ok
				dplayer = sqrt(dplayer);

				vec3set(p_closest, 0,0,0);

				//find point on quadarray closest to player
				for (i=0;i<qa->w;i++)
				{
					for (j=0;j<qa->h;j++)
					{

						vec3mov(p, *gx_vbuffer_v( qa->vb, qa_vindex( qa, i,j)));

						vec3sub(p, player_camera.camera_pos);

						d = vec3abs_sq(p);

						if (d< closest_point_d)
						{
							closest_point_d = d;
							vec3mov(p_closest, *gx_vbuffer_v( qa->vb, qa_vindex( qa, i,j)));
						}

					}
				}

				//now if the closest point to us on the surface is farther away from the planet center than us, then we are inside the planet, and thats bad
				d = vec3abs_sq(p_closest);
				d = sqrt(d)  ; // + 0.00002f;

				if (dplayer < d)
				{
					vec3norm(& (player_camera.camera_pos));
					vec3scale(player_camera.camera_pos, d);
					vec3scale(camera_inertia, .5);
					//vec3set(camera_inertia,0,0,0);
				}
						



			}
				


			ocount = vec_count(quadarrays);
			

			for(i=0;(i<vec_count(quadarrays)) && (i<ocount) ;i++)
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

		
				if (qa->tag != QUADARRAY_TAG_NONE)
					continue;  //don't process unless it's a fully active quadarray


				if (   ((((qa->size)/(d*d)) > 1.0) &&(splitcount < splitlimit)))
				{
					quadarray_t* dest[4];


				
			
			

					//remove the current qa
					splitcount++;
					
					quadarray_split(qa);
					if (qa->children[0])
					{
						qa->tag = QUADARRAY_TAG_REMOVE;  //remove parent
						qa->onlevel = 0; //will no longer be active
				

						vec_add(quadarrays, qa->children[0]);
						vec_add(quadarrays, qa->children[1]);
						vec_add(quadarrays, qa->children[2]);
						vec_add(quadarrays, qa->children[3]);

#ifndef FADE_PATCHES
						

						//add in the children
						qa->children[0]->tag = QUADARRAY_TAG_NONE;
						qa->children[1]->tag = QUADARRAY_TAG_NONE;
						qa->children[2]->tag = QUADARRAY_TAG_NONE;
						qa->children[3]->tag = QUADARRAY_TAG_NONE;

						qa->children[0]->onlevel = 1;
						qa->children[1]->onlevel = 1;
						qa->children[2]->onlevel = 1;
						qa->children[3]->onlevel = 1;


#endif

					/*	dirty_edges(qa);
						{int i;
							for (i=0;i<4;i++)
							{
								dirty_edges(qa->children[i]);
							}
						}
						*/



#ifdef FADE_PATCHES
						//tag children to draw, but not to process (until the fadeout is done)
						qa->children[0]->tag = 2;
						qa->children[1]->tag = 2;
						qa->children[2]->tag = 2;
						qa->children[3]->tag = 2;

						qa->children[0]->onlevel = 1;
						qa->children[1]->onlevel = 1;
						qa->children[2]->onlevel = 1;
						qa->children[3]->onlevel = 1;


						//add to fadeout list
						vec_add(quadarrays_fadeout, qa);
				
						qa->alpha = 1.0; 
#endif

						
						

					}
				}
				//combine patches
				else if ( qa->parent && ( qa->parent->tag == QUADARRAY_TAG_NONE)
					&& qa->parent->children[0]->onlevel
					&& qa->parent->children[1]->onlevel
					&& qa->parent->children[2]->onlevel
					&& qa->parent->children[3]->onlevel
					)
				{
					vec3mov(p, player_camera.camera_pos);
					vec3sub(p, qa->parent->center);
					d = vec3abs_sq(p);
					d = sqrt(d);
					
					if (((qa->parent->size)/(d*d)) < 1.0)
					{  //combine threshhold
					
					//	vec_add(quadarrays, qa->parent); //put parent back in
	

						//remove these
						//qa->parent->children[0]->tag = 1; //2 is draw but don't remove, dont process
						//qa->parent->children[1]->tag = 1;
						//qa->parent->children[2]->tag = 1;
						//qa->parent->children[3]->tag = 1;

						//will no longer be on level
						qa->parent->children[0]->onlevel = 0;
						qa->parent->children[1]->onlevel = 0;
						qa->parent->children[2]->onlevel = 0;
						qa->parent->children[3]->onlevel = 0;

#ifndef FADE_PATCHES
						qa->parent->children[0]->tag = QUADARRAY_TAG_REMOVE; 
						qa->parent->children[1]->tag = QUADARRAY_TAG_REMOVE;
						qa->parent->children[2]->tag = QUADARRAY_TAG_REMOVE;
						qa->parent->children[3]->tag = QUADARRAY_TAG_REMOVE;

						vec_add(quadarrays, qa->parent); //put parent back in
						qa->parent->onlevel = 1;
						qa->parent->tag = QUADARRAY_TAG_NONE;

					/*	dirty_edges(qa);
						{int i;
							for (i=0;i<4;i++)
							{
								dirty_edges(qa->children[i]);
							}
						}
						*/

					
#endif

#ifdef FADE_PATCHES
						qa->parent->alpha = 0;
						qa->parent->tag = QUADARRAY_TAG_DRAW_ONLY;
						qa->parent->onlevel=1;

						vec_add(quadarrays_fadein, qa->parent); //put parent back in

						//tag all siblings
						qa->parent->children[0]->tag = QUADARRAY_TAG_DRAW_ONLY; //2 is draw but don't remove, dont process
						qa->parent->children[1]->tag = QUADARRAY_TAG_DRAW_ONLY;
						qa->parent->children[2]->tag = QUADARRAY_TAG_DRAW_ONLY;
						qa->parent->children[3]->tag = QUADARRAY_TAG_DRAW_ONLY;

						//also mark as dirty
						/*
						dirty_edges(qa->parent);
						dirty_edges(qa->parent->children[0]);
						dirty_edges(qa->parent->children[1]);
						dirty_edges(qa->parent->children[2]);
						dirty_edges(qa->parent->children[3]);
						*/

#endif

					}
				}
		
			}
			//printf(" SPLIT %d\n", splitcount);
		}

	}

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
