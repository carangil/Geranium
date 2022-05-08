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

		gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex,  gx_lines, ZTRUE);
	else 
#endif
		gx_vbuffer_draw(qp->vb, qp->startindex, qp->endindex_noskirt,  gx_lines, ZTRUE);

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

	//add to root and active

	zvec_add(&qps->root_patches, qp);
	zvec_add(&qps->active_patches, qp);

	return ZTRUE;
}


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

	source->detailer(dest, source, a_start, a_end, b_start, b_end);
	

	
	dest->size = source->size /4;  //decrease area by 4
	dest->dsize = source->dsize/2;  //decrease lengths by 2
	
	dest->center = *gxi_vbuffer_v( dest->vb, qp_vindex( dest, dest->w/2,dest->h/2));
	dest->parent = source;
	

//	gx_quadpatch_norm(dest);
	
#if 0	
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
#endif

	

#ifdef QUADARRAY_SKIRTS
	gx_quadpatch_skirt(dest);
#endif
	return dest;
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

