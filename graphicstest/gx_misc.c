// projectZ - This file is frt of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

//This is where to put random graphics stuff that doesn't have enough stuff to give it its own file

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

#include "glheaders.h"



#include "gx_misc.h"


#include <stdio.h>

//immediate drawing mode
//for tests, debugging, etc
static gx_prim_e imm_prim;
static gx_vbuffer_t* imm_vb=NULL;


void gx_immediate(gx_prim_e prim)
{
	if (!imm_vb)
		imm_vb = gx_vbuffer_mk(400, 0, GX_VBUFFER_COLOR | GX_VBUFFER_NORMAL | GX_VBUFFER_TEXCOORD);
	
	
	imm_prim = prim;
}


void gx_point(vec3* vec, vec3* norm, float* c, float s, float t){
	
	if (imm_vb->vertex_count == imm_vb->vertex_capacity) {
		gx_vbuffer_update(imm_vb);
		gx_vbuffer_draw(imm_vb, 0, imm_vb->vertex_count,imm_prim, ZFALSE);
		imm_vb->vertex_count = 0;
	}
	
	gx_vbuffer_tex2(imm_vb,0,s,t);
	if (norm)
		gx_vbuffer_normal(imm_vb,*norm);
	
	if (c)
		gx_vbuffer_color4(imm_vb,c[0],c[1],c[2],c[3]);
	else
		gx_vbuffer_color4(imm_vb,1,1,1,1);
	
	gx_vbuffer_vertex(imm_vb, *vec);
	
}

void gx_end(){
	gx_vbuffer_update(imm_vb);
	gx_vbuffer_draw(imm_vb, 0, imm_vb->vertex_count,imm_prim, ZFALSE);
	imm_prim = 0;
	imm_vb->vertex_count = 0;
}









//portals and sectors

zbool sector_free(void* x)
{

	gx_sector_t* s = x;

//	vec_cleanup(& s->meshes);

	//ram_destructor_tail(s->portals);
	ram_free(s->portals);

	return ZTRUE;

}

gx_sector_t* gx_sector_mk(vec3* min, vec3* max )
{
	gx_sector_t* b = NULL;
	vec3 center;
	
	b = ram_alloc(sizeof(*b), sector_free);
	if (!b)
		return NULL;
	
	vec3mov( b->min, *min);
	vec3mov( b->max, *max);
	
	vec3mov(center, *min);
	vec3add(center, *max);
	vec3scale(center, .5);
	vec3mov(b->center, center);
	
	
	//vec_mk( &(b->meshes), 6);
		
	return b;
}

void gx_sector_draw(gx_sector_t* sect)
{
	int i;

	if (!sect)
		return;

//	for (i=0;i<vec_count(&sect->meshes);i++)
//	{

//		gx_mesh_draw( vec_get_at(&sect->meshes, i));

//	}


}


zbool gx_traverse_sectors_prim(gx_camera_t* cam, gx_sector_t* sector, gx_portal_t* peer ) {
	
	//todo:
	float fovy = 90.0;
	float aspect = 1;
	vec3 p;
	gx_portal_t* portal;
	
	gx_sector_outline(sector, ZFALSE);
	
	portal = sector->portals;
	zbool visible;
	zbool inside;
	
	while(portal)
	{
		float dot;
		float backface;
		float dist;
		visible=ZFALSE;
		inside = ZFALSE;
		float pa;
		
		//cone test
		vec3mov(p, portal->pos);
		vec3sub(p, cam->pos);
		
		//p is vector from camera to portal
		
		dist = sqrt(vec3abs_sq(p));
		vec3scale(p, (1/dist));
		//p is now normalized
		
		//check which way portal is facing
		backface = vec3dot(cam->rot.z_axis,portal->normal); 
		
		if (backface > 0){
			portal = portal->next_portal;
			continue;  
		}
						   
		dot = vec3dot(p, cam->rot.z_axis);
		//dot is cosine of angle between p and view direction
		
		pa =  .5 * fovy / 180*3.142;  //angle of code enclosing frustum
		pa += asin(portal->radius / dist); //add angular distance of sphere to cone
		
		if (dist <= portal->radius) {
		//	printf("inside\n");
			visible = ZTRUE;
			inside = ZTRUE;
		}
		else if (dot > cos( pa ) ) {
				
				visible = ZTRUE;
		}
	//	visible = ZTRUE;//remove
		
		
		if (visible && ! inside && peer ) {  
			float dist2;
			vec3 q;
			
	
			
			//p already has from camera to portal being tested
			//dist already has the distance
			
			//need to calculate the other one
			//cone test
			//peer is the portal the camera is peering through
			vec3mov(q, peer->pos);
			vec3sub(q, cam->pos);
		
			//q is vector from camera to peering portal
		
			dist2 = sqrt(vec3abs_sq(q));
			vec3scale(q, (1/dist2));
		
			//pa is the sum of the half of the angular size of both portals added together
			pa = asin(portal->radius / dist) + asin(peer->radius / dist2);
			
			// now find angle between these two portals
			dot = vec3dot( p, q);
		
		//	printf("ang1: %f ang2: %f  angbet: %f dot:%f ", 2*asin(portal->radius / dist), 2*asin(peer->radius / dist2), acos(dot), dot);
			
			
			if (acos(dot) > pa ) {
				
					visible = ZFALSE;
			}
			
		//	pringx_sector_add_portal_sphere(s,&pos, sqrt(2)/2, t, NULL);tf(" vis:%d\n", visible);
			
					gx_portal_draw_test(peer, ZTRUE);
			gx_portal_draw_test(portal, visible);
		}
			
			
		
	//	gx_portal_draw_test(portal, visible);
		
		if (visible && portal->target) {
				if (inside){
					gx_traverse_sectors_prim(cam, portal->target, NULL); 
				} else if (peer) {
					gx_traverse_sectors_prim(cam, portal->target, peer);
				} else {
					gx_traverse_sectors_prim(cam, portal->target, portal);
				}
			
		}
		
		
		
		portal = portal->next_portal;
		
	}
	
	
	return ZFALSE;
	
}


zbool gx_traverse_sectors(gx_camera_t* cam, gx_sector_t* sector ) {
	return gx_traverse_sectors_prim(cam, sector, NULL);
}


void gx_portal_draw_test(gx_portal_t* p, zbool active)

{
	//static GLUquadric* quadric = NULL;
	vec3 nn;

	if (!p)
		return;

	vec3 pos;
	vec3 pos2;
	float a;
	float b;
	float r = p->radius;
	float color[] = { 1,0,0,1};
	float green[] = { 0,1,0,1};
	if (active) 
		color[1] = 1;
	
	if (!p->isquad) {
		
			vec3mov (pos, p->pos);
			
			gx_immediate(gx_points);
			
			gx_point(&pos,NULL ,&green, 0,0);
			
			for (a=-3.14;a<3.14;a+=.5){
				for(b=-3.14/2;b<3.14/2;b+=.5) {
					vec3set(pos2, r*sin(a)*cos(b), r*sin(a)*sin(b), r*cos(a));
					vec3add(pos2, pos);
				
					gx_point(&pos2, NULL,&color, 0,0);
					
				}
			}
			
			gx_end();
		
			return;
	}
	
	
	
/*
	if (!quadric)
	{
		quadric = gluNewQuadric();
	}
	*/

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	
	/*glPushMatrix();
	glTranslatef( p->pos.vec3x, p->pos.vec3y, p->pos.vec3z);
	glColor3f(1,1,0);
	gluSphere(quadric, p->radius, 16, 16);
	glPopMatrix();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	*/

/*
	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0);
	glVertex3f(  p->pos.vec3x, p->pos.vec3y, p->pos.vec3z);
	
	glColor3f(1,1,0);
	glVertex3f(  p->pos.vec3x, p->pos.vec3y+.1, p->pos.vec3z);
	
	glColor3f(1,0,1);
	glVertex3f(  p->pos.vec3x+.1, p->pos.vec3y, p->pos.vec3z+.1);
	glEnd();
	*/

	vec3mov(nn, p->pos);
	vec3madd(nn, p->radius*.3, p->normal);
	
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3fv(&p->pos);
		glVertex3fv(&nn);

#if 0
		//connect portal points
		glColor3f(1,0,0);
		glVertex3fv(& p->points[0]);
		glVertex3fv(& p->pos);

		glVertex3fv(& p->points[1]);
		glVertex3fv(& p->pos);

		glVertex3fv(& p->points[2]);
		glVertex3fv(& p->pos);

		glVertex3fv(& p->points[3]);
		glVertex3fv(& p->pos);
#endif

		//go in square
		glColor3f(0,0,1);
		glVertex3fv(& p->points[0]);
		glVertex3fv(& p->points[1]);

		glVertex3fv(& p->points[1]);
		glVertex3fv(& p->points[2]);

		glVertex3fv(& p->points[2]);
		glVertex3fv(& p->points[3]);

		glVertex3fv(& p->points[3]);
		glVertex3fv(& p->points[0]);



	glEnd();


}






zbool portal_delete(void* x)
{
	gx_portal_t* p = x;

	//ram_destructor_tail(p->next_portal);
	ram_free(p->next_portal);

	return ZTRUE;
}


//try spherical portals
#if 1
gx_portal_t* gx_sector_add_portal_sphere(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target, vec3* normal)
{
	//add a point-portal to a sector
	
	gx_portal_t* p = NULL;

	if (!sector) 
		return NULL;

	p = ram_alloc(sizeof(gx_portal_t), portal_delete);
	
	if (p)
	{
		vec3mov (p->pos, *position);
		if (normal) {
			vec3mov (p->normal, *normal);
		} else {
			//calculate the normal 
			vec3 n;
			vec3mov(n, sector->center);
			vec3sub(n, p->pos);
			vec3normalize(&n);
			vec3mov(p->normal, n);
		}
		p->radius = radius;
		p->target = target;
		p->next_portal = sector->portals; //add existing portal list to the tail of this portal
		
		sector->portals = p;  //set as head of a sector's portal list
	}
	return p;
}
#endif

gx_portal_t* gx_sector_add_portal_quad(gx_sector_t* sector, gx_sector_t* target, vec3* normal, vec3* points[4])
{
	//add a point-portal to a sector
	
	
	int i;

	gx_portal_t* p = NULL;
	float d;
	float max_d=0;

	if (!sector) 
		return NULL;

	p = ram_alloc(sizeof(gx_portal_t), portal_delete);
	
	if (p)
	{
		//vec3mov (p->pos, *position);
 		vec3mov(p->normal, *normal);

		//find center
		vec3mov( p->pos,  *points[0]);
		vec3add( p->pos,  *points[1]);
		vec3add( p->pos,  *points[2]);
		vec3add( p->pos,  *points[3]);
		vec3scale(p->pos, .25);

		//now find farthest portal
		for (i=0;i<4;i++)
		{
			vec3 q;
			vec3mov (q, *points[i]);  

			vec3mov (p->points[i], q); //copy point into portal
			vec3sub (q, p->pos);
			d = vec3abs_sq(q);
			if (d > max_d)
				max_d = d;
		}

		p->radius = sqrt(d);


	//	p->radius = radius;
		p->target = target;
		p->next_portal = sector->portals; //add existing portal list to the tail of this portal

		
		
		sector->portals = p;  //set as head of a sector's portal list
	}
	return p;
}


void gx_sector_outline(gx_sector_t* sector, zbool show_portals)
{

	/* This doesn't work:  immediate mode uses the builtin glPopMatrix that doesn't exist anymore!  */
	

	if (!sector)
		return;
	

	//BTW this is the lamest way to draw a sector ever

//	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
	
	//gx_set_active_textures(NULL,0);
	//gx_set_active_lights(NULL,0);


	//glUseProgram(0);
	//	gx_light_tmp_off();
	
/*
     p3	 		p7

p2			p6


     p1			p5

p0			 p4
*/


#define glVertex3f(x,y,z)  { vec3 p; vec3set(p,x,y,z); gx_point(&p,NULL,NULL,0,0);}


#define P0		glVertex3f( sector->min.vec3x, sector->min.vec3y, sector->min.vec3z);
#define P1		glVertex3f( sector->min.vec3x, sector->min.vec3y, sector->max.vec3z);
#define P2		glVertex3f( sector->min.vec3x, sector->max.vec3y, sector->min.vec3z);
#define P3		glVertex3f( sector->min.vec3x, sector->max.vec3y, sector->max.vec3z);			
#define P4		glVertex3f( sector->max.vec3x, sector->min.vec3y, sector->min.vec3z);
#define P5		glVertex3f( sector->max.vec3x, sector->min.vec3y, sector->max.vec3z);
#define P6		glVertex3f( sector->max.vec3x, sector->max.vec3y, sector->min.vec3z);
#define P7		glVertex3f( sector->max.vec3x, sector->max.vec3y, sector->max.vec3z);
#if 0
glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1,1);

		glColor4f(1,.5,.2, .5);
		glBegin(GL_QUADS);

		P0 P1 P5 P4
		P0 P1 P3 P2
		P3 P7 P6 P2
		P6 P7 P5 P4
		P3 P7 P5 P1
		P2 P6 P4 P0


		glEnd();
	
#endif 
#if 1
		
			
				
	
	gx_immediate(gx_lines);
		P0 P1 P1 P3 P3 P2 P2 P0
		P4 P5 P5 P7 P7 P6 P6 P4
		P0 P4 P4 P5 P5 P1 P1 P0
		P2 P6 P6 P7 P7 P3 P3 P2	
	gx_end();
#endif

		if (show_portals) {

			gx_portal_t* p = sector->portals;
			
			while(p)
			{
				gx_portal_draw_test(p,ZFALSE);
				p = p->next_portal;

			}

		}

//glEnable(GL_DEPTH_TEST);
	gx_light_restore();
}



