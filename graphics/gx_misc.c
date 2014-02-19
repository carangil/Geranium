// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

//This is where to put random graphics stuff that doesn't have enough stuff to give it its own file




#include "../ztypes.h"
#include "../vmath/vmath.h"

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
#include "gx_mesh.h"
#include "gx_light.h"

#include <stdio.h>




zbool gx_point_in_box( vec3* min, vec3* point, vec3* max, float border)
{
	int j;

	for (j=0;j<3;j++)
	{

		if (point->array[j]- border < min->array[j])
			return zfalse;


		if (point->array[j] + border > max->array[j])
			return zfalse;

	}

	return ztrue;

}






/*void gx_camera_pos(float x, float y, float z)
{
	glLoadIdentity();
	glTranslatef(-x,-y,-z);

}
*/




//spin crap
//FLIP switches the order
// TRUE for camera matrices, FALSE for object matrices


void gx_spin(zbool flip, zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward)
{

	//roll

	if (roll != 0.0)
	{
		//add a little bit of the right vector to the up vector:
		
		vec3madd(*up, roll, *right);

		//make the new up vector unit-length
		vec3scale(  *up,  1.0/  sqrt( vec3abs_sq( *up ) ) ); 
		
	//cross product to give new right vector

		if (flip)
		{
			vec3cross( *right, *forward, *up  );
		}
		else
		{
			vec3cross( *right, *up, *forward  );
		}
		
	}

	//yaw
	if (yaw != 0.0)
	{
		//add some 'right' to 'forward'
		vec3madd( *forward, yaw, *right);

		//make forward unit-length
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake right vector;
		if (flip)
		{
			vec3cross( *right, *forward, *up  );
		}
		else
		{
			vec3cross( *right, *up, *forward  );
		}
	}

	if (pitch != 0.0)
	{
		//add some 'up' to the forward vector
		vec3madd( *forward, pitch, *up);

		//normalize the new forward vector
		vec3scale(  *forward,  1.0/  sqrt( vec3abs_sq( *forward ) ) ); 

		//remake the up vector
		if (flip)
		{
			vec3cross( *up, *right, *forward  );
		}
		else 
		{
			vec3cross( *up, *forward, *right  );
		}
	}


}



void gx_camera_init(gx_camera_t* cam)
{
	if (cam)
	{
		vec3set( cam->camera_pos,		0.0f, 0.0f, 0.0f);
		vec3set( cam->camera_right,		1.0f, 0.0f, 0.0f);
		vec3set( cam->camera_up,		0.0f, 1.0f, 0.0f);
		vec3set( cam->camera_forward,	0.0f, 0.0f, -1.0f);
	}
}


void test_lighting_on()
{
	float gray[]={.9,.9,.9,1};
	float white[]={1,1,1,1};
	float black[]={0,0,0,1};
	float red[]={1,0,0,1};
	float dim[]={.05,.05,.05,1};
	float light_position[]={1,1,0,0};

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  black);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  100.0);


//	glLightfv(GL_LIGHT0, GL_AMBIENT, dim);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	

//	glLightfv(GL_LIGHT0, GL_POSITION, light_position);



}


//portals and sectors

zbool sector_free(void* x)
{

	gx_sector_t* s = x;

	vec_cleanup(& s->meshes);

	ram_destructor_tail(s->portals);

	return ztrue;

}

gx_sector_t* gx_sector_mk(vec3* min, vec3* max )
{
	gx_sector_t* b = NULL;

	b = ram_alloc(sizeof(*b), sector_free);
	if (!b)
		return NULL;
	
	vec3mov( b->min, *min);
	vec3mov( b->max, *max);
	

	
	
	vec_mk( &(b->meshes), 6);
		
	return b;
}

void gx_sector_draw(gx_sector_t* sect)
{
	int i;

	if (!sect)
		return;

	for (i=0;i<vec_count(&sect->meshes);i++)
	{

		gx_mesh_draw( vec_get_at(&sect->meshes, i));

	}


}
void gx_portal_inactive_draw_test(gx_portal_t* p)

{
	//static GLUquadric* quadric = NULL;
	vec3 nn;

	if (!p)
		return;
	

/*
	if (!quadric)
	{
		quadric = gluNewQuadric();
	}
	*/
	
	gx_set_active_textures(NULL,0);
	//gx_set_active_lights(NULL, 0);

	gx_light_tmp_off();

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
		glColor3f(1,1,1);
		glVertex3fv(&p->pos);
		glVertex3fv(&nn);


		//connect portal points
		glColor3f(0,1,0);
		glVertex3fv(& p->points[0]);
		glVertex3fv(& p->pos);

		glVertex3fv(& p->points[1]);
		glVertex3fv(& p->pos);

		glVertex3fv(& p->points[2]);
		glVertex3fv(& p->pos);

		glVertex3fv(& p->points[3]);
		glVertex3fv(& p->pos);

		//go in square
		glColor3f(0,1,1);
		glVertex3fv(& p->points[0]);
		glVertex3fv(& p->points[1]);

		glVertex3fv(& p->points[1]);
		glVertex3fv(& p->points[2]);

		glVertex3fv(& p->points[2]);
		glVertex3fv(& p->points[3]);

		glVertex3fv(& p->points[3]);
		glVertex3fv(& p->points[0]);



	glEnd();


	gx_light_restore();
}




void gx_portal_draw_test(gx_portal_t* p)

{
	//static GLUquadric* quadric = NULL;
	vec3 nn;

	if (!p)
		return;
	
gx_light_tmp_off();
/*
	if (!quadric)
	{
		quadric = gluNewQuadric();
	}
	*/
	
	gx_set_active_textures(NULL,0);
//	gx_set_active_lights(NULL, 0);


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

	gx_light_restore();
}






zbool portal_delete(void* x)
{
	gx_portal_t* p = x;

	ram_destructor_tail(p->next_portal);

	return ztrue;
}
#if 0
gx_portal_t* gx_sector_add_portal(gx_sector_t* sector, vec3* position, zfloat32 radius, gx_sector_t* target, vec3* normal)
{
	//add a point-portal to a sector
	
	gx_portal_t* p = NULL;

	if (!sector) 
		return NULL;

	p = ram_alloc(sizeof(gx_portal_t), portal_delete);
	
	if (p)
	{
		vec3mov (p->pos, *position);
		vec3mov (p->normal, *normal);
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
	

	if (!sector)
		return;


	//BTW this is the lamest way to draw a sector ever

//	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_BLEND);
	
	gx_set_active_textures(NULL,0);
	//gx_set_active_lights(NULL,0);

	gx_light_tmp_off();

	
	
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
		
			glColor4f(1,1,1,1);
	glBegin(GL_LINES);	
		P0 P1 P1 P3 P3 P2 P2 P0
		P4 P5 P5 P7 P7 P6 P6 P4
		P0 P4 P4 P5 P5 P1 P1 P0
		P2 P6 P6 P7 P7 P3 P3 P2
	glEnd();
#endif

		if (show_portals) {

			gx_portal_t* p = sector->portals;
			
			while(p)
			{
				gx_portal_draw_test(p);
				p = p->next_portal;

			}

		}

//glEnable(GL_DEPTH_TEST);
	gx_light_restore();
}


//Sheets
//NOTE:  SHEET_POINT_AT is not safe!
#define SHEET_POINT_AT(SSSS,XXXX,YYYY)    ((SSSS)->points[   (SSSS)->width*(YYYY) + XXXX ] )

//create 2D rectanular sheet.  Does not have any points filled out yet
gx_sheet_t * gx_sheet_quad_mk(gx_vbuffer_t* vb, int width, int height)
{
	gx_sheet_t* sheet = ram_alloc(sizeof(gx_sheet_t), NULL);
	int a;

	if (!sheet)
		return NULL;

	sheet->numpoints = 0;
	sheet->width = width;
	sheet->height = height;
	sheet->maxpoints = width*height;
	sheet->points = ram_alloc(sizeof(zint32) * sheet->maxpoints, NULL);
	sheet->vb = vb;
	sheet->num_edges = 4;
	
	
	vec_mk( &sheet->edges[GX_SHEET_EDGE_TOP].indirect_vertices, width);
	vec_mk( &sheet->edges[GX_SHEET_EDGE_BOTTOM].indirect_vertices, width);
	for (a=0;a<width;a++)
	{
		vec_add(&(sheet->edges[GX_SHEET_EDGE_TOP].indirect_vertices), & SHEET_POINT_AT(sheet, a, 0));
		vec_add(&(sheet->edges[GX_SHEET_EDGE_BOTTOM].indirect_vertices), & SHEET_POINT_AT(sheet, a, height-1));
	}
	
	vec_mk( &sheet->edges[GX_SHEET_EDGE_LEFT].indirect_vertices, height);
	vec_mk( &sheet->edges[GX_SHEET_EDGE_RIGHT].indirect_vertices, height);

	for (a=0;a<height;a++)
	{
		vec_add(&sheet->edges[GX_SHEET_EDGE_LEFT].indirect_vertices, & SHEET_POINT_AT(sheet, 0, a));
		vec_add(&sheet->edges[GX_SHEET_EDGE_RIGHT].indirect_vertices, & SHEET_POINT_AT(sheet, width-1, a));
	}

	return sheet;
}

int gx_sheet_set_at( gx_sheet_t* s, int x, int y,  int vertex)
{
	if (!s)
		return GX_INDEX_INVALID;


	SHEET_POINT_AT(s, x, y) = vertex;

	return vertex;
}

void gx_sheet_show_buffer(gx_sheet_t* s)
{
	int a;
	for (a=0;a< s->width * s->height;a++)
	{
		if (a%s->width ==0) printf("\n");
		printf(" %02d", s->points[a]);
	}

}


//#define		COPY_TEXCOORD  4
//this function sets one edge of S to use vertices from T
//assign indices will indicate they use the exact same vertex (in the same vbuffer).  texcoords and normals are shared
//copy position will indicate they are separate vertices (can have different texcoords and normals), but the position is copied

void gx_sew_sheets( gx_sheet_t* s, int s_edge, gx_sheet_t* t, int t_edge, int operation)
{
	int a;
	
	if (s->vb != t->vb)  //can't sew sheets that are in different vbuffers
		return;
	
	if (s->edges[s_edge].indirect_vertices.count != t->edges[t_edge].indirect_vertices.count)
		return;  //can't sew sheets that have different arity


	for (a=0;a< s->edges[s_edge].indirect_vertices.count;a++)
	{
		if (operation & GX_ASSIGN_INDICES)
		{
			*(int*)(s->edges[s_edge].indirect_vertices.elements[a]) =  *(int*)(t->edges[t_edge].indirect_vertices.elements[a]);
		}

		if (operation & GX_COPY_POSITION)
		{
			vec3* spos = gx_vbuffer_v(s->vb, *(int*)(s->edges[s_edge].indirect_vertices.elements[a]));
			vec3* tpos = gx_vbuffer_v(t->vb, *(int*)(t->edges[t_edge].indirect_vertices.elements[a]));;
			
			vec3mov (*spos, *tpos);
		}

	}

}


//place all the indices needed for this sheet into the specified vbuffer
//after this step, the sheet can be discarded, but the renderable geometry will remain
void gx_sheet_index(gx_sheet_t* s)
{
	int i;
	int j;
	for (i=0;i<s->width;i++)
	{
		for (j=0;j<s->height;j++)
		{

			gx_vbuffer_add_index(s->vb, SHEET_POINT_AT(s, i-1, j-1));
			gx_vbuffer_add_index(s->vb, SHEET_POINT_AT(s, i,   j-1));
			gx_vbuffer_add_index(s->vb, SHEET_POINT_AT(s, i,   j  ));
			gx_vbuffer_add_index(s->vb, SHEET_POINT_AT(s, i-1 , j ));
		}
	}
}





void gx_test_sphere(vec3* pos, float radius)
{

	static GLUquadric* quadric = NULL;
	


	if (!quadric)
	{
		quadric = gluNewQuadric();
	}

	
	gx_set_active_textures(NULL,0);


	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glPushMatrix();
	glTranslatef( pos->vec3x, pos->vec3y, pos->vec3z);
	gluSphere(quadric, radius, 10, 10);
	glPopMatrix();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//	gluDeleteQuadric(quadric);

}
