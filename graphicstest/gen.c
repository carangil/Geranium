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
#include "gen.h"

/* adds random bumpy detail to quadpatch based on dsize value */

void gen_random_detail(gx_quadpatch_t* dest, gx_quadpatch_t* source, 
	int a_start, int a_end, int b_start, int b_end)
{
	int a;
	int b;
	int w = (a_end-a_start-1) * 2+1 ;
	int h = (b_end-b_start-1) * 2+1 ;
	int x;
	int y;
	vec3 p;
	vec3 q;
	vec3 r;
	int div;

	float s,t;
	float tmp;

	printf(" GENERATION %d\n", source->generation);

	for (y=0;y<h;y++) {
		for (x=0;x<w;x++){

			a = x/2 + a_start;
			b = y/2 + b_start;


			gx_vbuffer_get_v(p, source->vb, qp_vindex( source, a,b));
			
			div = 1;

			
			gx_vbuffer_get_s(s,source->vb, 0,qp_vindex( source, a, b));
			gx_vbuffer_get_t(t,source->vb, 0,qp_vindex( source, a, b));


			if ((x & 1) && ( (a+1)< a_end) ) //if odd x
			{
				
				gx_vbuffer_get_v(q, source->vb, qp_vindex( source, a+1,b));
				vec3add(p,q);
			
				div++;
	
				gx_vbuffer_get_s(tmp, source->vb,0, qp_vindex( source, a+1, b));
				s+=tmp;
				
				gx_vbuffer_get_t(tmp, source->vb,0, qp_vindex( source, a+1, b));
				t+=tmp;
			}

			if ((y & 1) && ( (b+1)< b_end) ) //if odd y
			{
				gx_vbuffer_get_v(q, source->vb, qp_vindex( source, a,b+1));
				vec3add(p,q);
				
					div++;
				
				gx_vbuffer_get_s(tmp, source->vb,0, qp_vindex( source, a, b+1));
				s+=tmp;
				
				gx_vbuffer_get_t(tmp, source->vb,0, qp_vindex( source, a, b+1));
				t+=tmp;
			}

			if ((y & 1) && ( (b+1)< b_end)  && (x & 1) && ( (a+1)< a_end)) //if odd x and odd y
			{
				gx_vbuffer_get_v(q, source->vb, qp_vindex( source, a+1,b+1));
				vec3add(p,q);
				
				div++;
				
				
				gx_vbuffer_get_s(tmp, source->vb,0, qp_vindex( source, a+1, b+1));
				s+=tmp;
				
				gx_vbuffer_get_t(tmp, source->vb,0, qp_vindex( source, a+1, b+1));
				t+=tmp;
			}


			vec3scale(p, 1.0/div);  //find average

			vec3set(r, randfs(), randfs(), randfs());
			vec3scale(r, dest->dsize);
			vec3add(p,r);
	
			gx_vbuffer_tex2(dest->vb,0, s/div, t/div);
			gx_vbuffer_vertex(dest->vb, p);
			
		}
	}

	gx_quadpatch_norm(dest);
	
}




zbool gen_qpcube(gx_quadpatch_sys_t* qpsys,
				 vec3 * origin,
				 int tessel,
				 float radius,
				 gx_drawstyle_t* ds[6],
				 gx_quadpatch_t* qpo[6],
				 gx_quadpatch_detailer_f detailer[6],
				 int flags,
				 float size, float dsize
 				)
{
	int i;
	int a,b;


	gx_quadpatch_t* qp;
	float d;
	gx_quadpatch_t* qps[6];
	
	if(!qpo)
		qpo = qps;
	
	float flip=1;
	
	if (flags & GEN_INSIDE) {
			flip = -1;
			//flip some things when inside
	}
	
	for (i=0;i<6;i++)
	{
		
		qpo[i] = qp =  gx_quadpatch_mk(tessel,tessel, detailer[i]);

		if (flags&GEN_INSIDE)
			qp->skirtflip=ZTRUE;
		
		for (b=0;b<tessel;b++)
		{
			for (a=0; a<tessel;a++)
			{

				vec3 v;

				float fa;
				float fb;

				fa = ((a-qp->w/2)/  (float) (qp->w-1)) *2 ;
				fb  =  ((b-qp->h/2)/ (float) (qp->h-1)) *2 ;

				gx_vbuffer_tex2(qp->vb, 0, a / (float)(tessel-1), b / (float)(tessel-1) );

				switch (i)
				{
					case GEN_CUBE_TOP: 
						vec3set(v, fa, 1, -fb * flip);
						break;

					case GEN_CUBE_BOTTOM:
						vec3set(v, fa, -1, fb * flip);
						break;

					case GEN_CUBE_FRONT:
						vec3set(v, fa, fb, 1*flip);
						break;

					case GEN_CUBE_BACK:
						vec3set(v, -fa, fb, -1*flip);
						break;

					case GEN_CUBE_LEFT:
						vec3set(v, -1, fb, fa*flip);
						break;

					case GEN_CUBE_RIGHT:
						vec3set(v, 1, fb, -fa*flip);
						break;

				}

				//now normalize


				if (flags & GEN_SPHERE) {
					d = vec3abs_sq(v);
					d=sqrt(d);
					vec3scale(v, radius/d);
				}
				else {
					vec3scale(v, radius);
				}

				qp->dsize=dsize;

				vec3add(v, *origin);

				gx_vbuffer_vertex(qp->vb, v);
				
			}
		}
	
		qp->size = size;
	
		qp->origin=*origin;
		gx_quadpatch_norm(qp);
		
	}
		
	//sew all together
	//somehow both inside and outside came out the same, i don't know how
	//i thought for sure this was going to be confusing 
			
	//loop	
	gx_quadpatch_sew(qpo[GEN_CUBE_FRONT], GX_EDGE_LEFT, qpo[GEN_CUBE_LEFT], GX_EDGE_RIGHT,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_FRONT], GX_EDGE_RIGHT, qpo[GEN_CUBE_RIGHT], GX_EDGE_LEFT,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_BACK], GX_EDGE_LEFT, qpo[GEN_CUBE_RIGHT], GX_EDGE_RIGHT,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_BACK], GX_EDGE_RIGHT, qpo[GEN_CUBE_LEFT], GX_EDGE_LEFT,0,ZTRUE);


	//top
	gx_quadpatch_sew(qpo[GEN_CUBE_TOP], GX_EDGE_BOTTOM, qpo[GEN_CUBE_FRONT], GX_EDGE_TOP,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_TOP], GX_EDGE_LEFT, qpo[GEN_CUBE_LEFT], GX_EDGE_TOP,1,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_TOP], GX_EDGE_RIGHT, qpo[GEN_CUBE_RIGHT], GX_EDGE_TOP,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_TOP], GX_EDGE_TOP, qpo[GEN_CUBE_BACK], GX_EDGE_TOP,1,ZTRUE);


	//bottom
	gx_quadpatch_sew(qpo[GEN_CUBE_BOTTOM], GX_EDGE_TOP, qpo[GEN_CUBE_FRONT], GX_EDGE_BOTTOM,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_BOTTOM], GX_EDGE_LEFT, qpo[GEN_CUBE_LEFT], GX_EDGE_BOTTOM,0,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_BOTTOM], GX_EDGE_RIGHT, qpo[GEN_CUBE_RIGHT], GX_EDGE_BOTTOM,1,ZTRUE);
	gx_quadpatch_sew(qpo[GEN_CUBE_BOTTOM], GX_EDGE_BOTTOM, qpo[GEN_CUBE_BACK], GX_EDGE_BOTTOM,1,ZTRUE);

	
	//generate normals and add to qpsystem
	
	for (i=0;i<6;i++)
	{
		gx_quadpatch_sys_add(qpsys, qpo[i]);
	}
	
	return ZTRUE;  
}




/* test sphere just for debugging */

gx_vbuffer_t* gensphere(int quality){


	float s,t;
	float ds =2*PI/quality;
	float dt = PI/quality;


	gx_vbuffer_t* sbuf = gx_vbuffer_mk((1+quality)*(1+quality)*4, 0, GX_VBUFFER_NORMAL |GX_VBUFFER_COLOR  | GX_VBUFFER_TEXCOORD  );


	for (s= -PI/2 ; (s+.5*ds)< PI/2; s+=ds) {
		for (t= 0 ; (t+.5*dt)<2*PI; t+=dt) {


			vec3 p;
			vec3 pds;
			vec3 pdsdt;
			vec3 pdt;

			vec3set(p, 	cos(s) * cos(t) , sin(t)*cos(s) , sin(s));
			vec3set(pds,	cos(s+ds) * cos(t) , sin(t)*cos(s+ds) , sin(s+ds));
			vec3set(pdsdt,	cos(s+ds) * cos(t+dt) , sin(t+dt)*cos(s+ds) , sin(s+ds));
			vec3set(pdt,	cos(s) * cos(t+dt) , sin(t+dt)*cos(s) , sin(s));

			gx_vbuffer_color4(sbuf, 1, 0,0,1);
			gx_vbuffer_tex2(sbuf, 0,  (s)/PI +.5, (t)/2/PI);
			gx_vbuffer_normal(sbuf, p);
			gx_vbuffer_vertex(sbuf,p);

			gx_vbuffer_color4(sbuf, 0, 1,0,1);
			gx_vbuffer_tex2(sbuf, 0, (s)/PI +.5,(t+dt)/2/PI);
			gx_vbuffer_normal(sbuf,pdt);
			gx_vbuffer_vertex(sbuf,pdt);

			gx_vbuffer_color4(sbuf, 0, 0,1, 1);
			gx_vbuffer_tex2(sbuf, 0, (s+ds)/PI +.5, (t+dt)/2/PI);
			gx_vbuffer_normal(sbuf,pdsdt);
			gx_vbuffer_vertex(sbuf,pdsdt);

			gx_vbuffer_color4(sbuf, 1, 1,1, 1);
			gx_vbuffer_tex2(sbuf, 0, (s+ds)/PI +.5, (t)/2/PI);
			gx_vbuffer_normal(sbuf,pds);
			gx_vbuffer_vertex(sbuf,pds);


		}

	}

	gx_vbuffer_update(sbuf);

	return sbuf;
}



