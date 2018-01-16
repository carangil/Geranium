#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/ram.h"
#include "../vmath/vmath.h"
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

#include "../graphics/gx_quadpatch.h"

int skirted=0;
int nonskirted=0;
int drawn=0;
int total=0;

static int rcurse_count=0;

zbool gx_gp_delete(void* v)
{
	int i;
	int j;
	rcurse_count++;
	gx_quadpatch_t* qp = (gx_quadpatch_t*) v;


	//printf("rc %d\n ", rcurse_count);

	//scribbe out adjacent references
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


	rcurse_count--;

	return ztrue; //free ourself
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
		qp->vb = gx_vbuffer_mk(w*h  +2*w + 2*h, w*h*3*2 + w*3*2 +h*3*2, zfalse, ztrue, 1);
#else
		qp->vb = gx_vbuffer_mk(w*h, w*h*3*2, zfalse, ztrue, 1);
#endif
		qp->startindex = 0;
 
		qp->vb->vertex_count = w*h;  //say all vertices are filled out

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
				gx_vbuffer_add_index( qp->vb, qp_vindex(qp,a,b));
				gx_vbuffer_add_index( qp->vb, qp_vindex(qp,a+1,b) );
				gx_vbuffer_add_index( qp->vb, qp_vindex(qp,a+1,b+1) );

				//tri 2
				gx_vbuffer_add_index( qp->vb, qp_vindex(qp,a,b) );		
				gx_vbuffer_add_index( qp->vb, qp_vindex(qp,a+1,b+1) );
				gx_vbuffer_add_index( qp->vb, qp_vindex(qp,a,b+1) );

				qp->endindex+=6;
			}
		}

		//now add skirts
		
		qp->endindex_noskirt = qp->endindex;

#ifdef QUADARRAY_SKIRTS


		for (b=0;b< (h-1); b++)
		{
			//left skirt
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,0,b)    );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,0,b) );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,0,b+1) );

			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,0,b)    );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,0,b+1) );
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,0,b+1)    );

			qp->endindex+=6;


			//right skirt
		
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_RIGHT,b)    );
			
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,w-1,b+1) );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,w-1,b) );

			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_RIGHT,b)    );
			
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_RIGHT,b+1)    );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,w-1,b+1) );

			qp->endindex+=6;


	}


	for (b=0;b<(w-1);b++)
	{


			//top skirt
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_TOP,b)    );
			
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,b+1 ,0) );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,b , 0) );

			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_TOP,b)    );
			
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_TOP,b+1)    );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,b+1 , 0) );

			qp->endindex+=6;


			//bottom skirt
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_BOTTOM,b)    );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,b , h-1) );
			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,b+1 ,h-1) );
			


			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_BOTTOM,b+1)    );
			gx_vbuffer_add_index( qp->vb, qp_skirtindex(qp,GX_EDGE_BOTTOM,b)    );
			

			gx_vbuffer_add_index( qp->vb, qp_vindex(qp,b+1 , h-1) );

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

//dynamically determine if use skirt
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
			nonskirted++;
			qp->useskirt=0;
		}
		else
		{
			skirted++;
			qp->useskirt=1;
		}

	}



	if (qp->useskirt )

		gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex,  gx_triangles, ztrue);
	else 
#endif
		gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex_noskirt,  gx_triangles, ztrue);

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
		printf("already done\n");
		return q->adjacent[edge];
	}



	
	// decompose my 'child number' into x and y
	x = q->self & 1;
	y = q->self >> 1;

//	if (edge == GX_EDGE_LEFT || edge == GX_EDGE_RIGHT)
//			edge_child_n = y;
//	else
//			edge_child_n = x;

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

	return NULL;
}


void gx_quadpatch_sew(gx_quadpatch_t* source, int source_edge, gx_quadpatch_t* dest, int dest_edge, int rev)
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
			s = _gx_vbuffer_v( source->vb, qp_vindex( source, 0,is));
			sn= _gx_vbuffer_n( source->vb, qp_vindex( source, 0,is));
		}
		else if (source_edge == GX_EDGE_RIGHT)
		{
			s = _gx_vbuffer_v( source->vb, qp_vindex( source, source->w-1,is));
			sn= _gx_vbuffer_n( source->vb, qp_vindex( source, source->w-1,is));
		}
		else if (source_edge == GX_EDGE_BOTTOM)
		{
			s = _gx_vbuffer_v( source->vb, qp_vindex( source, is,0));
			sn= _gx_vbuffer_n( source->vb, qp_vindex( source, is,0));
		}
		else if (source_edge == GX_EDGE_TOP)
		{
			s = _gx_vbuffer_v( source->vb, qp_vindex( source, is,source->h-1));
			sn= _gx_vbuffer_n( source->vb, qp_vindex( source, is,source->h-1));
		}


		//put
  		if (dest_edge == GX_EDGE_LEFT)
		{
  			*_gx_vbuffer_v( dest->vb, qp_vindex( dest, 0,i)) = *s; 
			*_gx_vbuffer_n( dest->vb, qp_vindex( dest, 0,i)) = *sn; 
		}
  		else if (dest_edge == GX_EDGE_RIGHT)
		{
			*_gx_vbuffer_v( dest->vb, qp_vindex( dest, dest->w-1,i)) = *s;
			*_gx_vbuffer_n( dest->vb, qp_vindex( dest, dest->w-1,i)) = *sn;
		}
  		else if (dest_edge == GX_EDGE_BOTTOM)
		{
  			*_gx_vbuffer_v( dest->vb, qp_vindex( dest, i,0)) = *s;
			*_gx_vbuffer_n( dest->vb, qp_vindex( dest, i,0)) = *sn;
		}
  		else if (dest_edge == GX_EDGE_TOP )
		{
  			*_gx_vbuffer_v( dest->vb, qp_vindex( dest, i,dest->h-1)) = *s;
			*_gx_vbuffer_n( dest->vb, qp_vindex( dest, i,dest->h-1)) = *sn;
		}

	}
}


#ifdef QUADARRAY_SKIRTS
void gx_quadpatch_skirt( gx_quadpatch_t* qp)
{
	int i;
	//pos for outside , neg for inside	
	float skirtd =- qp->dsize*2 ;

	qp->useskirt = ztrue;
	for (i=0;i<qp->h;i++)
	{
		//left
		vec3mov( *_gx_vbuffer_n(qp->vb, qp_skirtindex(qp, 0, i)), *_gx_vbuffer_n(qp->vb, qp_vindex(qp, 0, i)));
		vec3mov( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, 0, i)), *_gx_vbuffer_v(qp->vb, qp_vindex(qp, 0, i)));
		vec3sub(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, 0, i)), qp->origin);
		vec3scale( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, 0, i)), 1-skirtd);
		vec3add(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, 0, i)), qp->origin);


		//right
		vec3mov( *_gx_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), *_gx_vbuffer_n(qp->vb, qp_vindex(qp, qp->w-1, i)));
		vec3mov( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), *_gx_vbuffer_v(qp->vb, qp_vindex(qp, qp->w-1, i)));
		vec3sub(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), qp->origin);
		vec3scale( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), 1-skirtd);
		vec3add(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_RIGHT, i)), qp->origin);
	}

	for(i=0;i<qp->w;i++)
	{
		//top
		vec3mov( *_gx_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), *_gx_vbuffer_n(qp->vb, qp_vindex(qp,  i,0)));
		vec3mov( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), *_gx_vbuffer_v(qp->vb, qp_vindex(qp,  i,0)));
		vec3sub(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), qp->origin);
		vec3scale( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), 1-skirtd);
		vec3add(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_TOP, i)), qp->origin);

		//bottom
		vec3mov( *_gx_vbuffer_n(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), *_gx_vbuffer_n(qp->vb, qp_vindex(qp,  i,qp->h-1)));
		vec3mov( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), *_gx_vbuffer_v(qp->vb, qp_vindex(qp,  i,qp->h-1)));
		vec3sub(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), qp->origin);
		vec3scale( *_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), 1-skirtd);
		vec3add(*_gx_vbuffer_v(qp->vb, qp_skirtindex(qp, GX_EDGE_BOTTOM, i)), qp->origin);

	}



}
#endif




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
	dest->self=self;
	dest->generation = source->generation + 1;	
	dest->parent = source;

//	srandf(  source->center.named.x + source->center.named.y  + source->center.named.z);

	dest->origin = source->origin;

	source->detailer(dest, source, a_start, a_end, b_start, b_end);
	

#if 0
	for (y=0;y<h;y++)
	{
		for (x=0;x<w;x++)
		{
			a = x/2 + a_start;
			b = y/2 + b_start;

			
		
			vec3mov(p, *_gx_vbuffer_v( source->vb, qp_vindex( source, a,b)));
			div = 1;
			fdiv = sqrt(vec3abs_sq(*_gx_vbuffer_v( source->vb, qp_vindex( source, a,b))));

			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				vec3add(p, *_gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b)))
				div++;
				fdiv += sqrt(vec3abs_sq(*_gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b))));
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				vec3add(p, *_gx_vbuffer_v( source->vb, qp_vindex( source, a,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*_gx_vbuffer_v( source->vb, qp_vindex( source, a,b+1))));
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				vec3add(p, *_gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b+1)))
				div++;
				fdiv += sqrt(vec3abs_sq(*_gx_vbuffer_v( source->vb, qp_vindex( source, a+1,b+1))));
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


		



			*_gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y)) = p;


			//texcoord: don't interpolate coordinates
			//_gx_vbuffer_s(dest->vb, qp_vindex( dest, x, y),0) = ((float) x) / (w-1);
			//_gx_vbuffer_t(dest->vb, qp_vindex( dest, x, y),0) = ((float) y) / (w-1);


		//	*_gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y))
		//	=
		//	*_gx_vbuffer_v( source->vb, qp_vindex( source, a,b));

//				printf("copy %d %d  to %d %d\n", a,b,x,y);
		}
	}

	gx_quadpatch_norm(dest);
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
			p = _gx_vbuffer_v( dest->vb, qp_vindex( dest, x, y));
		//	n = _gx_vbuffer_n( dest->vb, qp_vindex( dest, x, y));

			vec3sub(*p, source->origin); 
			{	float r = randf();
				vec3scale(*p,  1.0  -  r*   source->dsize );
			}
			vec3add(*p, source->origin);


			//d = sqrt(vec3abs_sq(

			vec3norm( p); //round out

//			displace in direction of normal
		//	vec3madd(*p, -randf()  *  source->dsize, *n );

			//displace straight up and down
		//	vec3scale(*p, .9);
	//		vec3scale(*p,  1.0  - (randf()*source->dsize)  );
			
		}

	}

	#endif
	
	dest->size = source->size /4;  //decrease area by 4
	dest->dsize = source->dsize/2;  //decrease lengths by 2
	
	dest->center = *_gx_vbuffer_v( dest->vb, qp_vindex( dest, dest->w/2,dest->h/2));
	dest->parent = source;
	

	gx_quadpatch_norm(dest);
	
	
	if (1) {
		int edge;
		int nedge;
		int rev;

		for (edge=0;edge<4;edge++)
		{
			gx_quadpatch_t* n = find_neighbor( dest, edge, &nedge,&rev);
			gx_quadpatch_sew(n, nedge, dest, edge,rev);
		}


	}

	

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

void gx_quadpatch_norm(gx_quadpatch_t* qp)
{
	int a;
	int b;
	vec3 p;
	vec3 acc;
	int cnt=0;

	vec3 avg;
	int  avg_cnt=0;
	vec3set(avg, 0,0,0);
	

	for (a=0;a< qp->w; a++)
	{
		for(b=0;b<qp->h;b++)
		{
			vec3set(acc,0,0,0);
			cnt=0;
#if 1
			if ((a< (qp->w-1)) && (b< (qp->h-1)))
			{
				cnt++;
				calcnorm(&p,
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a+1,b)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b+1)));
				vec3add(acc, p);
			}

#endif		

#if 1
			if ((a> 0) && (b< qp->w-1))
			{
				cnt++;
				calcnorm(&p,
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b+1)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a-1,b)));
				vec3add(acc, p);
			}
#endif


#if 1
			if ((a> 0) && (b> 0))
			{
				cnt++;
				calcnorm(&p,
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a-1,b)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b-1)));
					
				vec3add(acc, p);
			}
#endif

#if 1
			if ((a< (qp->w-1)) && (b>0 ))
			{
				cnt++;
				calcnorm(&p,
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a,b-1)),
					_gx_vbuffer_v( qp->vb, qp_vindex( qp, a+1,b)));
					
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

			*_gx_vbuffer_n(qp->vb, qp_vindex( qp, a,b)) = acc;

		}

	}

	{
		float d;
		d = vec3abs_sq(avg);
		
		
		
		d = sqrt(d);
		vec3scale(avg,1.0/avg_cnt);
		vec3mov(qp->avgnorm, avg);
	}

}



/* make sure a quadpatch has children */

zbool gx_quadpatch_split(gx_quadpatch_t* source)
{
	zbool ok = ztrue;

	zuint32 i;
	zuint32 c;

	if (!source->detailer)
		return zfalse;

	c=0;
	for (i=0;i<4;i++)
	{
		if (source->children[i])
			c++;

		if (source->children[i] && source->children[i]->tag == 3)
		{
			printf(" tag is delete; this is a bad idea\n");
//			exit(0);
			return zfalse;
		}


	}


	if (c==4) //all children split
		return ztrue;

	if (c!= 0){
		//partial split
		printf("BAD");
		return zfalse;

	}



	//otherwise split it:

	source->children[0] = gx_quadpatch_detail_2x(0, source, 0, source->w/2+1, 0, source->h/2+1);
	source->children[1] = gx_quadpatch_detail_2x(1, source, source->w/2, source->w , 0, source->h/2+1);
	source->children[2] = gx_quadpatch_detail_2x(2, source, 0, source->w/2+1,        source->h/2,source->h );
	source->children[3] = gx_quadpatch_detail_2x(3, source, source->w/2, source->w , source->h/2, source->h);


	gx_vbuffer_update(source->children[0]->vb);
	gx_vbuffer_update(source->children[1]->vb);
	gx_vbuffer_update(source->children[2]->vb);
	gx_vbuffer_update(source->children[3]->vb);




	//update vbuffers
	for(i=0;i<4;i++)
	{
		if (source->children[i])
			gx_vbuffer_update(source->children[i]->vb);
		else
			return ok= zfalse;
	}



	return ok;	

}



//quadpatch system: a way to manage/draw a tree of quadpatches

#define QUADARRAY_TAG_NONE        0

#define QUADARRAY_TAG_REMOVE	    1
//#define QUADARRAY_TAG_DRAW_ONLY     2
#define QUADARRAY_TAG_DELETE	    3

#define ENABLE_DELETE


zbool gx_quadpatch_sys_init(gx_quadpatch_sys_t* qps)
{
	memset(qps, 0, sizeof(qps));
	if (!vec_mk(&qps->root_patches, 4))
		return zfalse;

	if (!vec_mk(&qps->active_patches, 4))
		return zfalse;

	vec_disown(&qps->active_patches); //don't own pointers to things

	qps->splitsize=1.0;

	return ztrue;
}


zbool gx_quadpatch_sys_cleanup(gx_quadpatch_sys_t* qps)
{

	vec_cleanup(&qps->root_patches);
	vec_cleanup(&qps->active_patches);


	return ztrue;
}





zbool gx_quadpatch_sys_add(gx_quadpatch_sys_t* qps, gx_quadpatch_t* qp)
{

	if (!qps || !qp )
		return zfalse;

	//add to root and active

	vec_add(&qps->root_patches, qp);
	vec_add(&qps->active_patches, qp);

	return ztrue;
}

void gx_quadpatch_sys_eval(gx_quadpatch_sys_t* qps,  gx_camera_t* camera)
{


		int i;
		vec_t* quadpatches = &qps->active_patches;
		gx_quadpatch_t* qp;
		vec3 p;
		float d;


		for (i=0;i< vec_count(quadpatches);i++)
		{
			qp = vec_get_at(quadpatches, i);

			//if was flagged to remove, remove it
			if (qp->tag == QUADARRAY_TAG_REMOVE || qp->tag == QUADARRAY_TAG_DELETE ) 	
			{

				vec_remove_unordered(quadpatches, i);
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

			vec3mov(p, camera->camera_pos);
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
						vec_add(quadpatches, qp->children[0]);
						vec_add(quadpatches, qp->children[1]);
						vec_add(quadpatches, qp->children[2]);
						vec_add(quadpatches, qp->children[3]);

						
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
					vec3mov(p, camera->camera_pos);
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


						vec_add(quadpatches, qp->parent); //put parent back in
						qp->parent->onlevel = 1; //parent is on level
						qp->parent->tag = QUADARRAY_TAG_NONE;

					}
				}

			//zzz

		}
}

void gx_quadpatch_sys_draw(gx_quadpatch_sys_t* qps, gx_camera_t* camera)
{
		int cull=0;
		int i;
		vec_t* quadpatches = &qps->active_patches;
		gx_quadpatch_t* qp;

		//printf("%d active patches in %p\n", vec_count(quadpatches), qps);

		for (i=0;i< vec_count(quadpatches);i++)
		{
			int skipdraw=0;
			qp = vec_get_at(quadpatches, i);

			{
				vec3 d;
				vec3mov (d, qp->center);
				vec3sub (d, camera->camera_pos);
				vec3normalize(&d);

				if (vec3dot(d, camera->camera_forward) < 0)
				{
					//outside of view cone
					cull++;
					skipdraw=1;
				}
				else
				if ( vec3dot(qp->avgnorm, camera->camera_forward) > .8)  //faces away from camera
				{
					cull++;
					skipdraw =1;
				}



			}
			
			//draw
			if (qp->tag == QUADARRAY_TAG_NONE)
			{
				if (!skipdraw)
						gx_quadpatch_draw(qp);
				
			}
			
		}
}
