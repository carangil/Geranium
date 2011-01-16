// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

//This is where to put random graphics stuff that doesn't have enough stuff to give it its own file




#include "../ztypes.h"
#include "../vmath.h"

#include "glstuff.h"

#include "../memory/ram.h"
#include "../structures/vector.h"


#include "gx_sys.h"
#include <math.h>"
#include <float.h>

#include "gx_image.h"
#include "gx_drawstyle.h"
#include "gx_buffers.h"
#include "gx_misc.h"



void gx_camera_pos(float x, float y, float z)
{
	glLoadIdentity();
	glTranslatef(-x,-y,-z);

}

void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis)
{
	zfloat32 matr[]={
					     xaxis->vec3x, yaxis->vec3x, -zaxis->vec3x,0,
					     xaxis->vec3y, yaxis->vec3y, -zaxis->vec3y,0,
					     xaxis->vec3z, yaxis->vec3z, -zaxis->vec3z,0,
					     0,0,0,1};
	glLoadIdentity();
	
	glMultMatrixf((float*)&matr);
				
	glTranslatef( -position->vec3x, -position->vec3y, -position->vec3z);
}

//spin crap
void gx_spin(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward)
{

	//roll

	if (roll != 0.0)
	{
		//add a little bit of the right vector to the up vector:
		
		vec3madd(*up, roll, *right);

		//make the new up vector unit-length
		vec3scale(  *up,  1.0/  sqrt( vec3abs_sq( *up ) ) ); 
		
	//cross product to give new right vector
		vec3cross( *right, *forward, *up  );
	}

	//yaw
	if (yaw != 0.0)
	{
		//add some 'right' to 'forward'
		vec3madd( *forward, yaw, *right);

		//make forward unit-length
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake right vector;
		vec3cross( *right, *forward, *up  );
	}

	if (pitch != 0.0)
	{
		//add some 'up' to the forward vector
		vec3madd( *forward, pitch, *up);

		//normalize the new forward vector
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake the up vector
		vec3cross( *up, *right, *forward  );
	}


}


//mesh related stuff

gx_mesh_t*  gx_mesh_def(gx_vbuffer_t* v, gx_drawstyle_t* s, zuint32 drawstart, zuint32 drawend, zbool indexed)
{
	gx_mesh_t* m = NULL;

	m = ram_alloc(sizeof(gx_mesh_t), NULL);  //no destructor since it doesn't contain any other structures it 'owns'
	
	if (m)
	{

		m->data = v;
		m->style = s;
		m->drawstart = drawstart;
		m->drawend = drawend;
		m->indexed = indexed;
	}
	return m;
}


void gx_mesh_draw(gx_mesh_t* mesh_in)
{
	gx_mesh_t* mesh = mesh_in;

	while(mesh)
	{
		gx_drawstyle_activate(mesh->style);
		gx_vbuffer_draw(mesh->data, mesh->drawstart, mesh->drawend, gx_triangles, mesh->indexed);
		mesh = mesh->next;
		
		if (mesh == mesh_in)  
		{
#ifdef DOPRINTF 
			printf(" Warning: breaking mesh cycle\n");
#endif
			//cycle detected!
			break;
		}
	}

}






//sector and portal crap.

gx_sector_t* gx_sector_mk(vec3* min, vec3* max )
{
	gx_sector_t* b = NULL;

	b = ram_alloc(sizeof(*b), NULL);
	if (!b)
		return NULL;
	
	vec3mov( b->min, *min);
	vec3mov( b->max, *max);
	
	vec_mk( &(b->meshes), 6);
	
	
	return b;
}

void gx_portal_draw_test(gx_portal_t* p)

{
	static GLUquadric* quadric = NULL;
	if (!p)
		return;
	


	if (!quadric)
	{
		quadric = gluNewQuadric();
	}
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glPushMatrix();
	glTranslatef( p->pos.vec3x, p->pos.vec3y, p->pos.vec3z);
	gluSphere(quadric, p->radius, 10, 10);
	glPopMatrix();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0);
	glVertex3f(  p->pos.vec3x, p->pos.vec3y, p->pos.vec3z);
	
	glColor3f(1,1,0);
	glVertex3f(  p->pos.vec3x, p->pos.vec3y+.1, p->pos.vec3z);
	
	glColor3f(1,0,1);
	glVertex3f(  p->pos.vec3x+.1, p->pos.vec3y, p->pos.vec3z+.1);
	glEnd();
}

gx_portal_t* gx_sector_add_portal(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target)
{
	//add a point-portal to a sector
	
	gx_portal_t* p = NULL;

	if (!sector) 
		return NULL;

	p = ram_alloc(sizeof(gx_portal_t), NULL);
	
	if (p)
	{
		vec3mov (p->pos, *position);
		p->radius = radius;
		p->target = target;
		p->next_portal = sector->portals; //add existing portal list to the tail of this portal
		sector->portals = p;  //set as head of a sector's portal list
	}
	return p;
}


void gx_sector_outline(gx_sector_t* sector)
{
	
	//BTW this is the lamest way to draw a sector ever

	glColor3f(1,1,1);
	glDisable(GL_BLEND);
	
	gx_set_active_textures(NULL,0);

	if (!sector)
		return;

	
/*
     p3	 		p7

p2			p6


     p1			p5

p0			 p4
*/

#define P0		glVertex3f( sector->min.vec3x, sector->min.vec3y, sector->min.vec3z);
#define P1		glVertex3f( sector->min.vec3x, sector->min.vec3y, sector->max.vec3z);
#define P2		glVertex3f( sector->min.vec3x, sector->max.vec3y, sector->min.vec3z);
#define P3		glVertex3f( sector->min.vec3x, sector->max.vec3y, sector->max.vec3z);			
#define P4		glVertex3f( sector->max.vec3x, sector->min.vec3y, sector->min.vec3z);
#define P5		glVertex3f( sector->max.vec3x, sector->min.vec3y, sector->max.vec3z);
#define P6		glVertex3f( sector->max.vec3x, sector->max.vec3y, sector->min.vec3z);
#define P7		glVertex3f( sector->max.vec3x, sector->max.vec3y, sector->max.vec3z);


	glBegin(GL_LINES);	
		P0 P1 P1 P3 P3 P2 P2 P0
		P4 P5 P5 P7 P7 P6 P6 P4
		P0 P4 P4 P5 P5 P1 P1 P0
		P2 P6 P6 P7 P7 P3 P3 P2
	glEnd();
	
	
}
