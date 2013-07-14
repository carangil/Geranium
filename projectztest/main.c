// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2013 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include <stdio.h>
#include <math.h>
#include "../ztypes.h"
#include "../memory/ram.h"
#include "../structures/vector.c"


#include "../vmath.h"
#include "../graphics/gx_sys.h"
#include "../graphics/gx_image.h"
#include "../graphics/gx_sprite.h"
#include "../graphics/gx_line.h"
#include "../graphics/gx_buffers.h"
#include "../graphics/gx_drawstyle.h"
#include "../graphics/gx_mesh.h"
#include "../graphics/gx_misc.h"
#include "../graphics/gx_light.h"



float fmin(float a, float b)
{
	if (a<b)
		return a;
	return b;

}

float fmax(float a, float b)
{
	if (a<b)
		return b;
	return a;

}

void test_mem();

void test_vectors();


//try out some iterators

typedef struct z_iterator_s
{
	void* data;
	zbool hasMore;
	void* (*next)( struct z_iterator_s* iter);
} z_iterator_t;


//test iterator:

typedef struct test_iterator_s
{
	z_iterator_t base_iter;
	zuint32 number;
	zchar	tstring[10];
} test_iterator_t;

void* test_iterator_next(z_iterator_t* z_iter)
{
	
	test_iterator_t* iter = z_iter;
	
	if (iter->number == 10)
	{
		iter->base_iter.hasMore = zfalse;
	}

	
	sprintf(iter->tstring, "%d", iter->number++);
	
	return iter->tstring;
}

z_iterator_t* test_iterator_mk()
{
	test_iterator_t* z = ram_alloc(sizeof(test_iterator_t), NULL);
	z->number = 0;
	z->base_iter.hasMore = ztrue;
	z->base_iter.next = test_iterator_next;
	z->base_iter.data = NULL;
	return z;
}


void struct_test()
{
	//test some structures

	
	vec_t* str_vector = vec_mk(NULL, 2);

	vec_add(str_vector, ram_strdup("a"));
	vec_add(str_vector, ram_strdup("b"));
	vec_add(str_vector, ram_strdup("c"));
	vec_add(str_vector, ram_strdup("d"));

	ram_free(str_vector);


	{
		z_iterator_t* some_iter = test_iterator_mk();
		
		while(some_iter->hasMore)
		{
			char* x = some_iter->next(some_iter);
			

			printf("got %s \n ", x);
		}

		ram_free(some_iter);

	}


}

void simple_graphics_test()
{
	gx_image_t* font;
	gx_sprite_t* pxsprite;
	gx_image_t* pximage;

	float angle=0;

	if (GX_OK != gx_init( 640, 480 , "Test Graphics Window"))
	{
		printf("Init Fail\n");
		return;
	}

	
	font = gx_image_load_tga("font8.tga");
	gx_image_set_scaler(font, GX_IMAGE_SCALER_SMOOTH);


	pximage = gx_image_load_tga("pxtest.tga");
	pxsprite = gx_sprite_mk(pximage, 0,0, pximage->width,pximage->height, 16,16);
	gx_image_set_scaler(pximage, GX_IMAGE_SCALER_BLOCKY);

	ram_free(pximage);  //can kill image, because the sprite refadded it


	while(1)
	{
		int width;
		int height;

		angle += .1;
		gx_clear_color(.1,0,0,0);
		gx_frame_clear(ztrue,ztrue);

//		gx_frame_get_dimensions(&width, &height);
//		gx_setup_2d(0,0,width, height);

		//set up for integer pixel coordinates
		gx_setup_2d_pixels(&width, &height);

		gx_text_color(1,1,1,1);
		//gx_text_size( .5*cos(angle/100),.5*sin(angle/100), 0 );
		gx_text_size( 16, 16, 0);
		gx_text_draw(font, 320, 24	,0,"Testing some text");


		{
			char x[] = {176,177,178,219,0};
			gx_text_draw(font, 320, 280, 0, x);
		}
		{
			char x[] = {217,217,219,176,0};
			gx_text_draw(font, 320, 280-16, 0, x);
		}
		{
			char x[] = {179,196,196,219,14,15,15,0};
			gx_text_draw(font, 320, 280-32, 0, x);
		}
		

		gx_line_color(1,1,1,.5);
		
		gx_line(10,10,100,100);
		gx_line_color(1,1,1,1);
		gx_line(100,100,200,200);

		gx_line_color(0,1,1,1);
		gx_line(0, 0, 0, height-1);
		gx_line(0, 0,width-1,0);
		gx_line(width-1,0, width-1, height-1);
		gx_line(0,height-1, width-1, height-1);

		gx_line_finish();

		gx_sprite_draw(pxsprite, 100,100,zfalse);
		gx_sprite_draw(pxsprite, 550,400,zfalse);

		gx_frame_show();
		gx_window_event();
		if(gx_key_state('x'))
			break;
	}

	ram_free(font);
	ram_free(pxsprite);
	
	gx_disable();

}

#define FSWAP(fff,ggg,ttt)  (ttt=ggg,ggg=fff,fff=ttt)


zbool vec_project_3d(vec3* input, gx_camera_t* cam, vec3* output);

zbool quad_intersect(vec3 p1[4], vec3 p2[4], zbool second)
{

	int i;
	int ni;
	int j;

	float ftmp;

	float amin;
	float amax;
	float bmin;
	float bmax;
	
	

	float dx;
	float dy;

	vec3 norm;

	//printf("intersecting test\n");

	for (i=0;i<4;i++)
	{
		ni = (i+1) % 4;
	
		
		//take one line

		dx = p1[i].named.x - p1[ni].named.x;
		dy = p1[i].named.y - p1[ni].named.y;

		//find a vector perpendicular to that line
		vec3set(norm, -dy, dx, 0);  

		//now project all the lines from each polygon against that line
		//if the span of the two polygons don't touch, then we found a seperating axis
		//and the polygons dont touch




		//proj 1st point of each polygon
		amax = amin = vec3dot(norm, p1[0]);
		bmax = bmin = vec3dot(norm, p2[0]);


		for (j=0; j<4; j++)
		{
			float proj;

		
			//project 1st polygon

			proj =  vec3dot(norm, p1[j]);
			if (proj<amin  )
				amin = proj;

			if (proj>amax  )
				amax = proj;


			//project 2nd polygon

			proj =  vec3dot(norm, p2[j]);
			if (proj<bmin )
				bmin = proj;

			if (proj>bmax)
				bmax = proj;


		}

		//now see if polygons touch
			
		//order so a starts first
		if (amin > bmin)
		{

			FSWAP(amin,bmin,ftmp);
			FSWAP(amax,bmax,ftmp);

		}
	//	printf( "reorder  a  %f to %f    b  %f  to %f\n", amin, amax, bmin, bmax);

		if (amax < bmin)
		{
			//no intersection!
			return zfalse;
		}
	}

	if (!second)
		return quad_intersect(p2, p1, ztrue);

	return ztrue;

}


void intersect_quad_test()
{
	gx_image_t* font;
	gx_sprite_t* pxsprite;
	gx_image_t* pximage;

	float angle=0;

	if (GX_OK != gx_init( 640, 480 , "Test Graphics Window"))
	{
		printf("Init Fail\n");
		return;
	}

	
	font = gx_image_load_tga("font8.tga");
	gx_image_set_scaler(font, GX_IMAGE_SCALER_SMOOTH);


	pximage = gx_image_load_tga("pxtest.tga");
	pxsprite = gx_sprite_mk(pximage, 0,0, pximage->width,pximage->height, 16,16);
	gx_image_set_scaler(pximage, GX_IMAGE_SCALER_BLOCKY);

	ram_free(pximage);  //can kill image, because the sprite refadded it


	while(1)
	{
		int width;
		int height;

		gx_clear_color(.1,0,0,0);
		gx_frame_clear(ztrue,ztrue);

		//set up for integer pixel coordinates
		gx_setup_2d_pixels(&width, &height);


		//need to fool around with this
		//zbool quad_intersect(vec3 p1[4], vec3 p2[4], zbool first)


		{
			int i;
			vec3 p1[4];
			vec3 p2[4];

			int mx, my;
			gx_mouse_pos(&mx, &my, NULL);

			mx-=200;
			my=height-my-200;

			vec3set(p1[0], 10,10,0);
			vec3set(p1[1], 20,60,0);
			vec3set(p1[2], 100,50,0);
			vec3set(p1[3], 102,20,0);

			vec3set(p2[0], mx-16,my+6,0);
			vec3set(p2[1], mx-26,my+50,0);
			vec3set(p2[2], mx-90,my+40,0);
			vec3set(p2[3], mx-82,my+20,0);



			for (i=0;i<4;i++)
				gx_line(p1[i].named.x, p1[i].named.y, p1[(i+1)%4].named.x, p1[(i+1)%4].named.y );

			for (i=0;i<4;i++)
				gx_line(p2[i].named.x, p2[i].named.y, p2[(i+1)%4].named.x, p2[(i+1)%4].named.y );


			gx_line_finish();

			if (quad_intersect(p1,p2, zfalse))
				gx_text_draw(font, 320, 50	,0,"yes Intersect");
			else
				gx_text_draw(font, 320, 50	,0,"no  Intersect");



		}

		gx_text_color(1,1,1,1);


		gx_text_size( 16, 16, 0);
		gx_text_draw(font, 320, 24	,0,"Intersect poly test");


		gx_frame_show();
		gx_window_event();
		if(gx_key_state('x'))
			break;
	}

	ram_free(font);
	ram_free(pxsprite);
	
	gx_disable();

}






/// need to make a stringmap
typedef struct stringmap_entry_ts
{
	char* key;
	void* item;
	zbool cleanitem;
} stringmap_entry_t;

typedef vec_t stringmap_t;
#define stringmap_mk vec_mk


stringmap_entry_t* stringmap_find_entry(stringmap_t* vec, char* key, int* index)
{
	int i;
	stringmap_entry_t* e;
	if (!vec)
		return NULL;
	if (!key)
			return NULL;

	for (i=0;i<vec_count(vec);i++)
	{
		e = vec_get_at(vec, i);
		if (!e)
			continue;
		if (!e->key)
			continue;

		if (!strcmp(e->key, key))
		{
			if (index)
				*index = i;  //return index if wanted
			return e;
		}
	}
	return NULL;
}

zbool _stringmap_cleanup(void* x)
{
	stringmap_entry_t* e = (stringmap_entry_t*) x;
	printf(" cleaning stringmap entry\n");
	ram_free(e->key);
	if (e->cleanitem)
	{
		ram_free(e->item);
		printf(" cleaning item too\n");
	}

	return ztrue;
}

stringmap_entry_t* stringmap_add_entry(stringmap_t* vec, char* key, void* item, zbool cleanitem)
{
	//todo: search for existing entry and replace it
	stringmap_entry_t* e = ram_alloc(sizeof(stringmap_entry_t), _stringmap_cleanup);
	if (!e)
		return NULL;
	e->key = ram_strdup(key);
	e->item = item;
	e->cleanitem = cleanitem;


	vec_add(vec, e);
	return e;

}
 

gx_image_t* get_texture(stringmap_t* textures, char* name)
{
	stringmap_entry_t* e;
	gx_image_t* img;

	e = stringmap_find_entry(textures, name, NULL);
	if (e)
	{
		printf(" found cached image %s\n", name);
		return e->item;
	}
	else
	{
		printf(" loading image %s\n", name);
		img = gx_image_load_tga(name);
		if (img)
		{
			printf( " cached image %s\n", name);
			stringmap_add_entry(textures, name, img, ztrue);

		}

		return img;
	}
	

}

//
typedef struct
{
	char* str;
	zsize size;
} zgrowstr_t;

#define zgrowstr_size(ZZSTR,ZZSIZE)   (((ZZSTR)->size>=(ZZSIZE))? ztrue : zgrowstr_size_func(ZZSTR,ZZSIZE)) 
//#define zgrowstr_size zgrowstr_size_func

zbool zgrowstr_size_func(zgrowstr_t* s, zsize size)
{
	zchar* new_str;
	zsize new_size;
	if (!s)
		return zfalse;

	if (s->size >=size)
		return ztrue;  /* already large enough */

	if (size > (s->size * 2))
		new_size = size;
	else
		new_size = s->size *2;  /*otherwise double */


	//more than double, just realloc
	new_str = ram_resize(s->str, new_size);
	if (new_str)
	{
			s->size = size;
			s->str = new_str;
			return ztrue;
	}
	
	return zfalse;

}

void zgrowstr_init(zgrowstr_t* s, zsize size)
{
	if (!s)
		return;

	s->size = size;
	s->str = ram_alloc(size, NULL);
}

zchar* zgrowstr_detach(zgrowstr_t* s)
{
	zchar* str = s->str;
	s->str = NULL;
	return str;
}

void zgrowstr_cleanup(zgrowstr_t* s)
{
	if (s && s->str)
		ram_free(s->str);
}



void growstr_test()
{
	zgrowstr_t gs;
	int i;

	zgrowstr_init(&gs, 5);
	for (i=0;i<100;i++)
	{
		zgrowstr_size(&gs,i+1);
		gs.str[i] = 'x';
	}
	zgrowstr_size(&gs,101);
	gs.str[100] = 0;
	printf(" %s\n", gs.str);

	zgrowstr_cleanup(&gs);
}




int read_line(FILE* f, zchar** line_out)
{
	zgrowstr_t line;
	int pos=0;
	int c;
	int ret=1;

	zgrowstr_init(&line,100);

	while(1)
	{
		c = fgetc(f);
		if (feof(f))
		{
			ret = -1;
			break;
		}

		if (c=='\r')
			continue;

		if (c=='\n')
			break;
		
		if (!zgrowstr_size(&line, pos+2))
			break;


		line.str[pos++]=c;
	}
	line.str[pos]=0;
	
	*line_out = zgrowstr_detach(&line);
	return ret;
}

int zspace(char c)
{
	if (c=='\t')
		return 1;
	if (c==' ')
		return 1;

	return 0;

}


//reads one word at a time, skipping white space
//can also nullterminate the word, but it modifies the string
int next_word(char** word_start, char** word_end, zbool nullterm)
{
	//eat leading white space
	
	if (*word_end)
			*word_start = *word_end;

	while ( zspace(**word_start))
		(*word_start) ++;

	*word_end = *word_start;
	while( **word_end && !zspace(**word_end))
		(*word_end) ++;

	if (nullterm)
	{
		if (**word_end)   //if not already null, 
		{
			int len = (*word_end) - (*word_start);

			**word_end = 0;  //null terminate
			(*word_end)++;    //inc to next char

			return len;
			
		}

	}
			
	return (*word_end) - (*word_start);
}


int wordcmp(char* word_start, int len, char* cmp)
{
	int ret;

	ret = strncmp(word_start, cmp, len);
	
	if (!ret)
	{
		if (cmp[len])
			ret = 1;
	}

	return ret;
}

/*
char* wordcpy(char* word_start, int len)
{
	char* x = ram_alloc( len + 1, NULL);
	
	memcpy(x, word_start, len);
	x[len] = 0;
	
	return x;
}
*/


#define FLOOR 0
#define CEILING 1
#define LEFT 2
#define RIGHT 3
#define BACK 4
#define FRONT 5

typedef struct sect_parms_s
{
	
	//gx_image_t* walls[6];
	
	gx_drawstyle_t* walls[6];

	zbool floor_gap[6]; //true of LEFT, RIGHT, FRONT, or BACK neighbor has mismatched floor height
	float floor_gap_y[6];

	zbool ceiling_gap[6]; //true of LEFT, RIGHT, FRONT, or BACK neighbor has mismatched ceiling height
	float ceiling_gap_y[6];

} sect_parms_t;

void addwall(gx_vbuffer_t* vb, vec3* a, vec3* b, vec3* c, vec3* d, vec3* norm)
{
	int p0,p1,p2,p3;

	gx_vbuffer_add_tex(vb, 0, 0,1);
	gx_vbuffer_add_normal(vb, norm->vec3x, norm->vec3y, norm->vec3z);
	p0 = gx_vbuffer_add_vertex(vb, a->vec3x, a->vec3y, a->vec3z);

	gx_vbuffer_add_tex(vb, 0, 1,1);
	gx_vbuffer_add_normal(vb, norm->vec3x, norm->vec3y, norm->vec3z);
	p1 = gx_vbuffer_add_vertex(vb,  b->vec3x, b->vec3y, b->vec3z);

	gx_vbuffer_add_tex(vb, 0, 1,0);
	gx_vbuffer_add_normal(vb, norm->vec3x, norm->vec3y, norm->vec3z);
	p2 = gx_vbuffer_add_vertex(vb,  c->vec3x, c->vec3y, c->vec3z);

	gx_vbuffer_add_tex(vb, 0, 0,0);
	gx_vbuffer_add_normal(vb, norm->vec3x, norm->vec3y, norm->vec3z);
	p3 = gx_vbuffer_add_vertex(vb,  d->vec3x, d->vec3y, d->vec3z);

	gx_vbuffer_add_index(vb, p0);
	gx_vbuffer_add_index(vb, p2);
	gx_vbuffer_add_index(vb, p1);

	gx_vbuffer_add_index(vb, p0);
	gx_vbuffer_add_index(vb, p3);
	gx_vbuffer_add_index(vb, p2);
}


gx_sector_t* sector_cube_mk(vec3* in_min, vec3* in_max, sect_parms_t* parms, float border)
{
	gx_sector_t* s;
	gx_vbuffer_t* vb;
	vec_t* textures;
	gx_mesh_t* mesh;
	int start;
	int end;
	int i;
	int j;
	int  p0,p1,p2,p3;
	vec3   a,b,c,d,e,f,g,h;
	vec3  norm[6];
	vec3 inner_min;
	vec3 inner_max;

	vec3*  wall_points[6][4] =
	{
		{ &a, &b, &c, &d}, //floor
		{ &h, &g, &f, &e}, //ceiling
		{ &h, &e, &a, &d}, //left
		{ &f, &g, &c, &b}, //right
		{ &e, &f, &b, &a}, //back
		{ &g, &h, &d, &c}  //front
	};

	vec3 t1,t2;
	vec3 min;
	vec3 max;

	vec3set(norm[FLOOR],	0,1,0);
	vec3set(norm[CEILING],	0,-1,0);
	vec3set(norm[LEFT],		1,0,0);
	vec3set(norm[RIGHT],	-1,0,0);
	vec3set(norm[FRONT],	0,0,-1);
	vec3set(norm[BACK],		0,0,1);
	



	textures = vec_mk(NULL, 6);
	textures->own_elements = zfalse;  /* only referening, not 'owning' */

	for (i=0;i<6;i++)
	{
		if (parms->walls[i] && vec_find_idx(textures, parms->walls[i]) == -1 )
		{
			vec_add(textures, parms->walls[i]);
		}
	}

	printf(" %d unique textures for this sector\n", vec_count(textures));
	vb = gx_vbuffer_mk(100,100,zfalse, ztrue, 1);


	//create sector smaller than intended
	vec3set(inner_min, in_min->named.x + border, in_min->named.y + border, in_min->named.z + border); 
	vec3set(inner_max, in_max->named.x - border, in_max->named.y - border, in_max->named.z - border); 

	s = gx_sector_mk(&inner_min, &inner_max);

	s->pmin = *in_min;
	s->pmax = *in_max;


	//for each texture style
	for (i=0;i< vec_count(textures);i++)
	{
		zbool solidwall;

		start = gx_vbuffer_current_index(vb);

		vec3mov(min, *in_min);
		vec3mov(max, *in_max);

		vec3set(a, min.vec3x, min.vec3y, min.vec3z);
		vec3set(b, max.vec3x, min.vec3y, min.vec3z);
		vec3set(c, max.vec3x, min.vec3y, max.vec3z);
		vec3set(d, min.vec3x, min.vec3y, max.vec3z);
		vec3set(e, min.vec3x, max.vec3y, min.vec3z);
		vec3set(f, max.vec3x, max.vec3y, min.vec3z);
		vec3set(g, max.vec3x, max.vec3y, max.vec3z);
		vec3set(h, min.vec3x, max.vec3y, max.vec3z);


		for(j=0;j<6;j++)
		{
			
			if (parms->walls[j] == vec_get_at(textures,i) )
			{

			

				if (! parms->floor_gap[j] && ! parms->ceiling_gap[j])	
				{
					addwall(vb, wall_points[j][0], wall_points[j][1], wall_points[j][2], wall_points[j][3], &norm[j]);
				}

				if(parms->floor_gap[j])
				{
					vec3mov(t1, *wall_points[j][0]);
					vec3mov(t2, *wall_points[j][1]);
					t1.vec3y = parms->floor_gap_y[j];
					t2.vec3y = t1.vec3y;
					addwall(vb, &t1, &t2, wall_points[j][2], wall_points[j][3], &norm[j]);
				}
				if(parms->ceiling_gap[j])
				{
					vec3mov(t1, *wall_points[j][2]);
					vec3mov(t2, *wall_points[j][3]);
					t1.vec3y = parms->ceiling_gap_y[j];
					t2.vec3y = t1.vec3y;
					addwall(vb, wall_points[j][0], wall_points[j][1], &t1, &t2, &norm[j]);
				}
					

			}

		}

		//extended

		for(j=0;j<6;j++)
		{
			solidwall = zfalse;

			if (parms->walls[j] )
			{

				//set the border

				if (! parms->floor_gap[j] && ! parms->ceiling_gap[j])	
				{
					
					solidwall = ztrue;
				}
			}

			if (!solidwall)
			{
			
				if (j==FLOOR)
					s->min.named.y = in_min->named.y;
				if (j==LEFT)
					s->min.named.x = in_min->named.x;
				if (j==BACK)
					s->min.named.z = in_min->named.z;

				if (j==CEILING)
					s->max.named.y = in_max->named.y;
				if (j==RIGHT)
					s->max.named.x = in_max->named.x;
				if (j==FRONT)
					s->max.named.z = in_max->named.z;

			}
		}
		
		end = gx_vbuffer_current_index(vb);
		
		if (start == end)
			continue;


		mesh = gx_mesh_def(vb, vec_get_at(textures,i), start, end, ztrue);

		vec_add(&(s->meshes), mesh);

	}
	

	ram_free(textures);
	
	gx_vbuffer_update(vb);
	ram_free(vb);  //we don't need it anymore (meshes will refcount it alive)


	return s;

}


#define LEVEL_TILE_FLOOR	1
#define LEVEL_TILE_WALL		2


int get_line_char(vec_t* lines, int line, int col)
{
	if (line < vec_count(lines))
	{
		char* li = vec_get_at(lines, line);
		if (col < strlen(li))   //todo: strlen is ugly here
			return li[col];
	}
	return 0;
}

//return is a vector of sectors
vec_t*  load_level(stringmap_t* textures, zchar* filename)
{

	FILE* f;
	zchar* line;
	int ret =0;
	char* word_start;
	char* word_end;
	char chy=0;
	int i;
	
	int mapwidth=0;

	vec_t* sectors = NULL;
	
	gx_drawstyle_t* tilestyles[256];
	int				tiletypes[256];


	memset(tilestyles, 0, sizeof(tilestyles));
	memset(tiletypes, 0, sizeof(tiletypes));

	sectors = vec_mk(NULL, 2);


	f = fopen(filename, "rb");
	if (!f)
		return NULL;  //can't load

	while (ret != -1)
	{
		ret = read_line(f, &line);

		if (line && line[0])
		{
			int len=1;
			printf(" LINE {%s}\n", line);
			word_start = line;
			word_end = NULL;
			
			len = next_word(&word_start, &word_end,0);
			if (!wordcmp(word_start, len, "$tile"))
			{
				unsigned char tilename;

				char* filename;

				len = next_word(&word_start, &word_end, 1);
				if (len != 1)
				{
					printf(" %s is not a valid tile name\n", word_start);
					return NULL;
				}
				
				tilename = word_start[0];

		
				//get tile type
				len = next_word(&word_start, &word_end, 1);
				if (!strcmp(word_start, "floor"))
				{
					tiletypes[tilename] = LEVEL_TILE_FLOOR;
				}
				else if (!strcmp(word_start, "wall"))
					tiletypes[tilename] = LEVEL_TILE_WALL;


				//get texture name
				len = next_word(&word_start, &word_end, 1);

				tilestyles[tilename] = gx_drawstyle_mk(  get_texture( textures, word_start));
				vec3set( tilestyles[tilename]->specular_color , 0,1,0);
				tilestyles[tilename]->specular_exponent = 100.0;
			
			}
			else if (!wordcmp(word_start, len, "$map"))
			{
				//parse out a map
				vec_t maplines;
				vec_t floorheights;
				

				vec_t* mlplane = NULL;

				char* mapline = NULL;
				int i;
				int j;
				
				float floor_y=0;
				
				vec_mk(&maplines, 2);
				vec_mk(&floorheights, 2);

				mlplane = &maplines;  //start reading in maplines

				while (ret != -1)
				{
					ret = read_line(f, &mapline);
					if (&mapline)
					{
						word_start = mapline;
						word_end = NULL;
						len = next_word(&word_start, &word_end, 0);
						if (!wordcmp(word_start, len, "$endmap"))
						{
							ram_free(mapline);
							break;
						}
						if (!wordcmp(word_start, len, "$floorheight"))
						{
							ram_free(mapline);

							mlplane = &floorheights;

							continue;
						}




						printf("%s\\\n", mapline);
						vec_add(mlplane, mapline);
						mapline = NULL;

					}

				}

				//process map here

				for (i=0;i<vec_count(&maplines);i++)
				{
					char* mline = vec_get_at(&maplines, i);
					
					if (i==0) 
						mapwidth =strlen(mline);

					else if (mapwidth != strlen(mline))
					{
						printf(" inconsistent map width!\n");
						return NULL;
					}

					for (j=0;j<mapwidth;j++)
					{
						unsigned char tilename = mline[j];

						if (tiletypes[tilename] ==  LEVEL_TILE_FLOOR)
						{
							sect_parms_t sect_parms;
							vec3 min;
							vec3 max;
							gx_sector_t* sect = NULL;

							printf(".");

							//floor areas are actual sectors

							memset( &sect_parms, 0, sizeof(sect_parms));

							
							//setup the floor to walk on

							sect_parms.walls[FLOOR] = tilestyles[tilename];


							//set ceiling to the same
							sect_parms.walls[CEILING] = tilestyles[tilename ];						
							//is there a floorheight value
							
							chy = get_line_char(&floorheights, i, j);
							if (chy)
								floor_y = (chy-'0')/10.0;
							

							
							//set sides x- and x+
							if (j>0)
							{
								tilename = mline[j-1];
								if ( tiletypes[tilename] == LEVEL_TILE_WALL)
									sect_parms.walls[LEFT] = tilestyles[tilename ];

								else {
									float nfy = floor_y;

									chy = get_line_char(&floorheights, i, j-1);
									if (chy)
										nfy = (chy-'0')/10.0;

									if (nfy > floor_y)
									{
										sect_parms.walls[LEFT] = tilestyles[tilename ];
										sect_parms.floor_gap[LEFT] = 1;
										sect_parms.floor_gap_y[LEFT] = nfy;
									}
										

								}

							}

						
							if (j>0)
							{
								tilename = mline[j+1];
								if ( tiletypes[tilename] == LEVEL_TILE_WALL)
									sect_parms.walls[RIGHT] = tilestyles[tilename ];
								else {
									float nfy = floor_y;

									chy = get_line_char(&floorheights, i, j+1);
									if (chy==' ')
									{
										floor_y = 0;
										sect_parms.walls[FLOOR] = NULL;
									}
									else
									{
										if (chy)
											nfy = (chy-'0')/10.0;

										if (nfy > floor_y)
										{
											sect_parms.walls[RIGHT] = tilestyles[tilename ];
											sect_parms.floor_gap[RIGHT] = 1;
											sect_parms.floor_gap_y[RIGHT] = nfy;
										}
									}
										

								}
							}

							//set sidex z- and z+
							if (i>0)
							{
								char* ll = vec_get_at(&maplines, i-1);
								tilename = ll[j];
								if ( tiletypes[tilename] == LEVEL_TILE_WALL)
									sect_parms.walls[BACK] = tilestyles[tilename ];
								else {
									float nfy = floor_y;

									chy = get_line_char(&floorheights, i-1, j);
									if (chy)
										nfy = (chy-'0')/10.0;

									if (nfy > floor_y)
									{
										sect_parms.walls[BACK] = tilestyles[tilename ];
										sect_parms.floor_gap[BACK] = 1;
										sect_parms.floor_gap_y[BACK] = nfy;
									}
										

								}
							}

							if (i < (vec_count(&maplines)-1))
							{
								char* ll = vec_get_at(&maplines, i+1);
								tilename = ll[j];
								if ( tiletypes[tilename] == LEVEL_TILE_WALL)
									sect_parms.walls[FRONT] = tilestyles[tilename ];

								else {
									float nfy = floor_y;

									chy = get_line_char(&floorheights, i+1, j);
									if (chy)
										nfy = (chy-'0')/10.0;

									if (nfy > floor_y)
									{
										sect_parms.walls[FRONT] = tilestyles[tilename ];
										sect_parms.floor_gap[FRONT] = 1;
										sect_parms.floor_gap_y[FRONT] = nfy;
									}
										

								}
							}

							
							vec3set(min, j, floor_y, i);
							vec3set(max, j+1, 1, i+1);


							#define SECTBORDER 0.2

#if 0
	//no walls
								sect_parms.walls[FRONT] = NULL;
								sect_parms.walls[BACK] = NULL;
								sect_parms.walls[LEFT] = NULL;
								sect_parms.walls[RIGHT] = NULL;

#endif

							sect = sector_cube_mk( &min, &max, &sect_parms, SECTBORDER);

							vec_add(sectors, sect);
							sect = NULL;

						}
						else
						{
							vec_add(sectors, NULL);  //put in an empty one
							printf("#");
						}


					}
					printf("\n");

				}



				//cleanup map text lines

				vec_cleanup(&maplines);
				vec_cleanup(&floorheights);

			}
			else
				printf(" unknown directive\n", word_start);

		}

		ram_free(line);
	}

	fclose(f);

//#define PSIZE 
//#define PSIZE 1
	#define PSIZE (sqrt(2)/3)

	//connect neighboring sectors together
#define SECT_INDEX(mapwidth, x,z)   ((mapwidth)*(z)+(x))
	for (i=0;i<vec_count(sectors);i++)
	{
		vec3 center;

		int x = i % mapwidth;
		int z = i / mapwidth;
		gx_sector_t* sector;
		gx_sector_t* other;

		if (!x)
			printf(">\n");

		if (vec_get_at(sectors, i))
			printf("_");
		else
			printf("#");


		sector = vec_get_at( sectors,  SECT_INDEX(mapwidth,x,z) );
		if (!sector)
			continue;

		vec3mov(center, sector->min);
		vec3add(center, sector->max);
		vec3scale(center, .5);

		if ( x > 0)
		{	
			vec3 p;
			vec3 n;


			other = vec_get_at(sectors, SECT_INDEX(mapwidth,x-1, z));

			if (other)
			{
				vec3 a;
				vec3 b;
				vec3 c;
				vec3 d;
				vec3* pts[4];
				pts[0] = &a;
				pts[1] = &b;
				pts[2] = &c;
				pts[3] = &d;
				vec3set(a, sector->pmin.named.x, sector->pmin.named.y, sector->pmin.named.z);
				vec3set(b, sector->pmin.named.x, sector->pmax.named.y, sector->pmin.named.z);
				vec3set(c, sector->pmin.named.x, sector->pmax.named.y, sector->pmax.named.z);
				vec3set(d, sector->pmin.named.x, sector->pmin.named.y, sector->pmax.named.z);

				
//float y;
		////		y = ( fmax( sector->min.named.y, other->min.named.y)
				//	 +fmin( sector->max.named.y, other->max.named.y) ) /2 ;
				

			
//				vec3set(p, sector->min.named.x,  y , center.named.z);
				vec3set(n, 1, 0, 0);
					

				//gx_sector_add_portal(sector, &p, PSIZE, other, &n);

				//gx_portal_t* gx_sector_add_portal_quad(gx_sector_t* sector, gx_sector_t* target, vec3* normal, vec3** points)
				


				gx_sector_add_portal_quad(sector, other, &n, pts);
			

				vec3scale(n, -1);				
			

				gx_sector_add_portal_quad(other, sector, &n, pts);

				

				//gx_sector_add_portal(other, &p, PSIZE , sector, &n);
			}
			
		}
		
		if (z > 0)
		{	
			vec3 p;
			vec3 n;


			other = vec_get_at(sectors, SECT_INDEX(mapwidth,x, z-1));

			if (other)
			{
			//	float y;

			//	y = ( fmax( sector->min.named.y, other->min.named.y)
			//		 +fmin( sector->max.named.y, other->max.named.y) ) /2 ;
				
				vec3 a;
				vec3 b;
				vec3 c;
				vec3 d;
				vec3* pts[4];
				pts[0] = &a;
				pts[1] = &b;
				pts[2] = &c;
				pts[3] = &d;
				vec3set(a, sector->pmin.named.x, sector->pmin.named.y, sector->pmin.named.z);
				vec3set(b, sector->pmin.named.x, sector->pmax.named.y, sector->pmin.named.z);
				vec3set(c, sector->pmax.named.x, sector->pmax.named.y, sector->pmin.named.z);
				vec3set(d, sector->pmax.named.x, sector->pmin.named.y, sector->pmin.named.z);


			
				//vec3set(p, center.named.x,  y , sector->min.named.z);

				vec3set(n, 0, 0, 1);
					

			//	gx_sector_add_portal(sector, &p,  PSIZE, other, &n);
			
				gx_sector_add_portal_quad(sector, other, &n, pts);
				vec3scale(n, -1);				
				gx_sector_add_portal_quad(other, sector, &n, pts);
				//gx_sector_add_portal(other, &p,  PSIZE , sector, &n);
			}
			
		}



	}



	//delete the tilestyles (if they got used, they will be kept alive by refcount
	for (i=0;i<256;i++)
	{
		ram_free(tilestyles[i]);

	}

	return sectors;
}





#if 0

void sector_test()
{
	stringmap_t* textures;
	gx_image_t* font;
	gx_vbuffer_t* vbuffer;

	gx_sector_t* sect;
	vec_t* sectors;
	sect_parms_t sect_parms;

	gx_drawstyle_t* brickstyle;
	gx_drawstyle_t* brick2style;
	gx_drawstyle_t* rockstyle;


	if (GX_OK != gx_init(640,480, "Sector test"))
	{
		printf("Cannot init graphics\n");
		return;
	}

	textures = vec_mk(NULL, 10);

	//setup any assets
	font = get_texture(textures, "font.tga");
	
	vbuffer = gx_vbuffer_mk(1000,1000,zfalse, zfalse, 1);

	gx_vbuffer_update(vbuffer);

	//create styles	
	brickstyle = gx_drawstyle_mk(get_texture(textures, "brick.tga"));
	brick2style = gx_drawstyle_mk(get_texture(textures, "brick2.tga"));
	rockstyle = gx_drawstyle_mk(get_texture(textures, "rock.tga"));

	//create a simple cube sector
	ram_clear(&sect_parms, sizeof(sect_parms));

	sect_parms.walls[FLOOR] = brickstyle;

	//sect_parms.walls[LEFT] = brick2style;
	sect_parms.walls[RIGHT] = brickstyle;
	sect_parms.walls[CEILING] = rockstyle;
	sect_parms.walls[FRONT] = brickstyle;
	sect_parms.walls[BACK] = brick2style;

	sect_parms.floor_gap[LEFT] = ztrue;
	sect_parms.floor_gap_y[LEFT] = -.3;
	sect_parms.ceiling_gap[LEFT] = ztrue;
	sect_parms.ceiling_gap_y[LEFT] = .5;

	sect_parms.floor_gap[RIGHT] = ztrue;
	sect_parms.floor_gap_y[RIGHT] = -.4;
	sect_parms.ceiling_gap[RIGHT] = ztrue;
	sect_parms.ceiling_gap_y[RIGHT] = .6;

	sect_parms.floor_gap[BACK] = ztrue;
	sect_parms.floor_gap_y[BACK] = -.5;
	sect_parms.ceiling_gap[BACK] = ztrue;
	sect_parms.ceiling_gap_y[BACK] = .7;

	sect_parms.floor_gap[FRONT] = ztrue;
	sect_parms.floor_gap_y[FRONT] = -.6;
	sect_parms.ceiling_gap[FRONT] = ztrue;
	sect_parms.ceiling_gap_y[FRONT] = .8;


	{
		vec3 min;
		vec3 max;
		vec3set( min, -1, -1, -2);
		vec3set(max, 1, 1, -1);

		sect = sector_cube_mk(&min, &max, &sect_parms);

	}

	//main loop
	while(1)
	{
		gx_setup_3d(90.0, gx_frame_get_dimensions(NULL,NULL), .1, 100);
		
		gx_frame_clear(ztrue, ztrue);
		
		gx_setup_3d(90, 4.0/3.0, .1, 1000);
		
		gx_camera_home();

		{
			vec3 p;
			vec3set(p, .1,.1,0);
			gx_move3d(&p);
		}


		gx_set_active_textures(NULL, 0);
		gx_sector_outline(sect, ztrue);

		gx_sector_draw(sect);
		

		//draw in 2d
		gx_text_size(16,16, 0);
		gx_setup_2d_pixels();
		gx_text_draw(font, 0,0, 0,"Sector Test");


		gx_frame_show();
		gx_window_event();
		if (gx_key_state('x'))
			break;
	}

	
	ram_free(vbuffer);
	ram_free(textures);
	ram_free(brickstyle);
	ram_free(brick2style);
	ram_free(rockstyle);
	ram_free(sect);

	gx_disable();

}
#endif 
void stringmap_test()
{
	stringmap_t stringmap;
	stringmap_entry_t* e;
	int idx;

	vec_mk(&stringmap, 1);
	stringmap_add_entry(&stringmap, "test", "itemfortest", zfalse);
	stringmap_add_entry(&stringmap, "test2", ram_strdup("itemfortest2"), ztrue);
	
	e = stringmap_find_entry(&stringmap, "test", &idx);
	if (e) 
		printf (" found %s at %d: %s\n", e->key, idx, e->item);


	e = stringmap_find_entry(&stringmap, "test2", &idx);
	if (e) 
		printf (" found %s at %d: %s\n", e->key, idx, e->item);

	e = stringmap_find_entry(&stringmap, "test3", &idx);
	if (e) 
		printf (" found %s at %d: %s\n", e->key, idx, e->item);

	vec_cleanup(&stringmap);

}

int framecount =0;

float fovy = 0;
float aspect = 1;



int recurse_draw_sectors_old(gx_sector_t* sector, vec3* camera_pos, vec3* camera_look)
{

	gx_portal_t* portal;
	int cnt=0;
	zbool visible = zfalse;

	if (sector->lastframe == framecount)
		return 0 ;

	cnt = 1;

	gx_sector_draw(sector);

//	gx_sector_outline(sector, ztrue);

	sector->lastframe = framecount;

	portal = sector->portals;

	while(portal)
	{
		vec3 p;
		float dist;
		float dot;

		visible = zfalse;
		
		//frustum test (cone actually)
		
		vec3mov(p, portal->pos);
		vec3sub(p, *camera_pos);
		dist = vec3abs_sq(p);
		vec3scale(p, (1/sqrt(dist)));


		dot = vec3dot(p, *camera_look);
		//printf("%f\n", dot);
		if (dot > cos(  .5 * aspect*  fovy   /180*3.14159    +atan(portal->radius/dist)    )) //inside frustum 'cone'
		//if (dot > cos( aspect*fovy/180.0*3.141*.5  +atan(portal->radius/dist) ) )
		{
			//gx_portal_draw_test(portal);

			//now check orientation

			if (vec3dot(portal->normal, p) < 0)
				visible = ztrue;		
		
		}

		//also just count as visible if we are inside the portal sphere
		if ( sqrt(dist) < portal->radius)
			visible = ztrue;


		if (visible)
		{
			portal->lastpeeked = framecount;
			//gx_portal_draw_test(portal);
			cnt += recurse_draw_sectors_old(portal->target, camera_pos, camera_look);
		}
		

		portal = portal->next_portal;
	}

	return cnt;
}


/*



Todo: create portal polygons: quads

to see if a portal is visible, test the screen-space quads against each other
if any of the two portal's lines intersect in screen space, then those portals could see through each other


*/

#define MAXPORTALRECURSE 64

int recurse_draw_sectors_ppoly(gx_sector_t* sector, gx_camera_t* cam, float angle, gx_portal_t* last_portal_in, int deep)
{

	gx_portal_t* portal;
	int cnt=0;
	zbool visible = zfalse;
	zbool reoutline=zfalse;
	gx_portal_t* last_portal = last_portal_in;

//	if (sector->lastframe == framecount)
//		return 0 ;

	if (deep > MAXPORTALRECURSE)
		return 0;

	if (sector->visiting)
		return 0;

	sector->visiting = ztrue;

	if (sector->lastframe != framecount)
	{
		
		gx_sector_draw(sector);
		//reoutline=1;
		cnt = 1;
	}

	sector->lastframe = framecount;

	
	//for all portals that lead out of this sector
	for (portal = sector->portals;portal;portal = portal->next_portal)
	{
		vec3 p;
		float dist;
		float dot;
		float portalangle;
		float coneangle;

		//zbool screen_ok = ztrue;

	//	vec3 p_screen[4];
		int i;

	//	if (portal->lastpeeked == framecount)  //if already looked through here this frame, don't re-look
	//		continue;

		
		

		visible = zfalse;
		
		//frustum test (cone actually), testing with view cone
		
		vec3mov(p, portal->pos);
		vec3sub(p, cam->camera_pos);
		dist = vec3abs_sq(p);
		dist = sqrt(dist);
		vec3scale(p, (1/dist));


		dot = vec3dot(p, cam->camera_forward);
		//printf("%f\n", dot);

		portalangle = atan(portal->radius/dist) ;

		coneangle = angle + portalangle;
		if (coneangle > 3.14/2) coneangle = 3.14/2;

		if (dot > cos( coneangle      )) //inside frustum 'cone'
		//if (dot > cos( aspect*fovy/180.0*3.141*.5  +atan(portal->radius/dist) ) )
		{
		

			//now check orientation

		//	if (vec3dot(portal->normal, p) < 0)
				visible = ztrue;		
		
		}

		
#if 1
		//project portal points into screen space
		
		portal->points_screen_ok = ztrue;

		for (i=0; i<4;i++)
		{


			//screen_ok = screen_ok && vec_project_3d(& portal->points[i], cam, &p_screen[i]);

			portal->points_screen_ok = portal->points_screen_ok && vec_project_3d(& portal->points[i], cam, &portal->points_screen[i]);
			
			
		}


		//only do check if all points are 'ok' (all are in front of screen)
#if 0
		if (portal->points_screen_ok && last_portal && last_portal->points_screen_ok)
		{
			zbool touch = quad_intersect(last_portal->points_screen, portal->points_screen, zfalse);

			if (!touch)
			{
				visible = zfalse;  //not touching, so false

			//	gx_portal_draw_test(last_portal);
			//	gx_portal_inactive_draw_test(portal);
			}


		}
#endif

#if 1
		
		portal->vis_chain_prev = last_portal;

		//go back and test portal against whole chain
		if (portal->points_screen_ok )
		{
			while (last_portal)
			{
				zbool touch = quad_intersect(last_portal->points_screen, portal->points_screen, zfalse);

				if (!touch)
				{
					visible = zfalse;  //not touching, so false

				//	gx_portal_draw_test(last_portal);
				//	gx_portal_inactive_draw_test(portal);
					break;
				}
				

				last_portal = last_portal->vis_chain_prev;
			}

			last_portal = last_portal_in; //reset when checking again later


		}
#endif 



#endif


	
		//also just count as visible if we are inside the portal sphere
		if ( dist < portal->radius)
		{
			portal->points_screen_ok = zfalse; //don't use portal corners if being included by distance
			visible = ztrue;

		}


		if (visible)
		{
		//	gx_portal_draw_test(portal);
			//gx_portal_draw_test(portal);
			portal->lastpeeked =framecount;
			//gx_portal_draw_test( portal);

		//	if (portalangle> angle)
		//		portalangle = angle;




			if (portal->points_screen_ok)
				cnt += recurse_draw_sectors_ppoly(portal->target, cam, angle, portal, deep+1);
			else
				cnt += recurse_draw_sectors_ppoly(portal->target, cam, angle, NULL,deep+1);
		}
		
			
		

		
	}

	if (reoutline)
	{
		gx_zbuffer(zfalse);
		gx_sector_outline(sector, ztrue);
		gx_zbuffer(ztrue);
	}
	
	sector->visiting = zfalse;

	return cnt;
}

//returns a point projected from 3d to 2d
//return true if point is in front of camera
zbool vec_project_3d(vec3* input, gx_camera_t* cam, vec3* output)
{
	vec3 p;
	vec3mov (p, *input);
	vec3sub (p, cam->camera_pos);

	output->named.x = vec3dot(p, cam->camera_right);
	output->named.y = vec3dot(p, cam->camera_up);
	output->named.z = vec3dot(p, cam->camera_forward);

	if (output->named.z > .1)
	{
		float ztop = 1 / tan(.5*fovy/180.0*3.141);

		output->named.x = (1/aspect)*output->named.x  / (output->named.z /ztop  ); 
		output->named.y = output->named.y  / (output->named.z / ztop );
		return ztrue;
	}

	return zfalse;
}


void level_test(vec_t* sectors, vec_t* textures)
{
	gx_sector_t* sect;
	
	gx_camera_t cam;
	vec3     camera_inertia;
	zbool player_walking = zfalse;
	zbool player_falling = ztrue;
	float player_height=.4;

	float jumpdeltapitch=0; //do backflip
	float jumpdeltaroll=0;
	vec3 player_pos_future;
	int seccnt=0;
	char keypress;
	
	
	gx_image_t* font = get_texture(textures, "font8.tga");

	gx_light_t*	light = NULL;

	int i;

	gx_sector_t* camera_sector = NULL; //which sector is the camera in?

	gx_camera_init(&cam);
	vec3set(camera_inertia, 0,0,0);
	cam.camera_pos.named.x = 1.5;
	cam.camera_pos.named.z = 1.5;
	cam.camera_pos.named.y = .6;

	{
		vec3 pos;
		vec3 color;
		vec3 ambient;
		vec3set(pos, 1,.5,1);
		vec3set(color, 1, 1, 1);
		vec3set(ambient, .1,.1,.1);

		light = gx_light_mk(gx_light_point, &pos, &color, &ambient);
	}

	light->attenuated = 1;
	light->unityrange = 3;


	for (i=0;i<vec_count(sectors);i++)
	{
		sect = vec_get_at(sectors, i);
		if (!sect) 
			continue;


		if (sect && !camera_sector)
			camera_sector = sect;

		seccnt++;

	}

//main loop
	while(1)
	{

		int secdraw=0;
		

		vec3mov (light->position, cam.camera_pos);
		//vec3madd(light->position, -.1, cam.camera_forward);

		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;
		
		

			vec3	 delta_pos;
			zbool mouse_relative=0;
			int mouse_x=0;
			int mouse_y=0;

			vec3set	 (delta_pos, 0,0,0);

			

			gx_mouse_pos(&mouse_x, &mouse_y, &mouse_relative);

			if (!mouse_relative)
				{
				//if some some reason we get an absolute mouse position, clear it out
				mouse_x=0;
				mouse_y=0;
			}

#define GRAVITY

#ifdef GRAVITY

			if (player_walking)
			{
				camera_inertia.named.x=0;
				camera_inertia.named.z=0;
			}

			camera_inertia.named.y -=.005;
			
#else
			vec3set(camera_inertia, 0,0,0);  //reset every frame

#endif

				//read keyboard input
			keypress = gx_getkey();  //reads decoded ascii chars
		
			if (keypress=='m')
			{
				//toggle the mouse capture
				gx_mouse_capture( mouse_relative ^ 1);
			}


#define WALKSPEED .05

if (player_walking)
{

			if (gx_key_state('w')) delta_pos.vec3z+= WALKSPEED*2;
			if (gx_key_state('s')) delta_pos.vec3z=- WALKSPEED*2;
			if (gx_key_state('a')) delta_pos.vec3x=- WALKSPEED*2;
			if (gx_key_state('d')) delta_pos.vec3x=+ WALKSPEED*2;
			
			if (gx_key_state(' ')) {
				camera_inertia.vec3y=+ 1.5*WALKSPEED;
				vec3scale(delta_pos, 1.5); //run or strafe faster when doing a jump

				if (gx_key_state('q'))
					jumpdeltaroll= -.07;

				

				if (gx_key_state('e'))
					jumpdeltaroll= +.07;



			}

			if (gx_key_state('r')) delta_pos.vec3y=+ WALKSPEED;
			if (gx_key_state('f')) delta_pos.vec3y=- WALKSPEED;


			

}


			if (player_falling)
			{
				delta_roll += jumpdeltaroll;
				delta_pitch += jumpdeltapitch;

				
			}


			if (gx_key_state('x')) vec3set(camera_inertia, 0,0,0);

			if (gx_key_state('q')) delta_roll-=.02;
			if (gx_key_state('e')) delta_roll+=.02;

			if (gx_key_state('4')) delta_yaw-=.02;
			if (gx_key_state('6')) delta_yaw+=.02;

			if (gx_key_state('8')) delta_pitch-=.02;
			if (gx_key_state('2')) delta_pitch+=.02;

			
		printf(" \t\t\tdelta roll %f\n", delta_roll);

			vec3scale(delta_pos, .5);

			//move camera using camera's basis
			vec3madd( camera_inertia, delta_pos.vec3x, cam.camera_right);
			vec3madd( camera_inertia, delta_pos.vec3y, cam.camera_up);
			vec3madd( camera_inertia, delta_pos.vec3z, cam.camera_forward);

			vec3madd( cam.camera_pos, 1 , camera_inertia);


			

			//lets spin camera  (relative to its own coord system)

			delta_pitch += mouse_y*.003;
			delta_yaw += mouse_x*.003;

		//	if (player_walking)
			{
				//float f;

			//	delta_roll += cam.camera_right.named.y * .1;
				/*vec3 yup;
				vec3set(yup, 0,1,0);
				vec3scale(cam.camera_up, 0.95);
				vec3madd(cam.camera_up,  0.05, yup);
				*/


			}

			gx_spin(ztrue, delta_yaw, delta_pitch, delta_roll,&cam.camera_right, &cam.camera_up, &cam.camera_forward);

		}

		
		
		
		
 		if (! gx_point_in_box(&camera_sector->min, &cam.camera_pos, &camera_sector->max, 0))
		{
			int j = 0;
			gx_portal_t* p;
			
			

			zbool norestrict = zfalse;

			vec3 player_feet;

			vec3mov (player_feet, cam.camera_pos);
			player_feet.named.y -= player_height;

			//where the player will be really soon
			vec3mov (player_pos_future, cam.camera_pos);
			vec3madd(player_pos_future, 10, camera_inertia);
			
		

			p = camera_sector->portals;
			while(p)
			{
				//see if we have crossed into another sector;
				if (gx_point_in_box( &p->target->min, &cam.camera_pos, &p->target->max,0))
				{
					camera_sector = p->target;
					break;

				}

				//see if feet have crossed into another sector;
				if (gx_point_in_box( &p->target->min, &player_feet, &p->target->max,0))
				{
					camera_sector = p->target;
					break;

				}

				//see if we will cross into another sector. if we will, allow this 'illegal' motion
				if (gx_point_in_box( &p->target->min, &player_pos_future, &p->target->max,0))
				{
					norestrict=ztrue;

				}

			


				p=p->next_portal;
			}

			

//restrict;


			if (!norestrict) {	
				int j;
				for (j=0;j<3;j++)
				{
				

					
					cam.camera_pos.array[j] = fmin( cam.camera_pos.array[j],  camera_sector->max.array[j]);

					//if (j==1) continue;
					cam.camera_pos.array[j] = fmax( cam.camera_pos.array[j] ,  camera_sector->min.array[j]);
				}
			}
			

		}
		


		if(1){
			// restrict falling to player's height

			if (cam.camera_pos.named.y < camera_sector->min.named.y + player_height)
			{
				player_falling = zfalse;
				player_walking = ztrue;
				cam.camera_pos.named.y = camera_sector->min.named.y + player_height;
				jumpdeltaroll=0;
				jumpdeltapitch=0;
				printf("JUMPDELTAROLL ZERO\n");
			}

			if (cam.camera_pos.named.y > .1+ camera_sector->min.named.y + player_height)
			{
				player_falling = ztrue;
				player_walking = zfalse;
				
				
			}


			if (!player_falling)
				camera_inertia.named.y=0; //stop falling

		}
		
		printf(" fall:%d  walk:%d\n", player_falling, player_walking);

		
		//gx_setup_3d(90, gx_frame_get_dimensions(NULL,NULL), .1, 1000);
		
		gx_clear_color(0,0,.1,0);
	
		gx_frame_clear(ztrue, ztrue);

		gx_setup_3d(fovy = 80.0, aspect=gx_frame_get_dimensions(NULL,NULL), .1, 1000);
		
		gx_camera_home();

	/*	{
			vec3 p;
			vec3set(p, -8, -3,-10);`
			gx_move3d(&p);
		}
		*/
		gx_camera_pos_rot(&cam.camera_pos,&cam.camera_right, &cam.camera_up, &cam.camera_forward); 


		gx_set_active_lights(&light, 1);

		
	
		++framecount;
//		gx_sector_draw(camera_sector);
		secdraw =  recurse_draw_sectors_ppoly(camera_sector, &cam,   .5 * aspect*  fovy   /180*3.14159, NULL ,0  );
	//	secdraw =  recurse_draw_sectors_old(camera_sector, &cam.camera_pos, &cam.camera_forward,  .5 * aspect*  fovy   /180*3.14159);
		printf(" %d sectors drawm\n" , secdraw );

		//printf(" %d sectors drawm\n" , recurse_draw_sectors_old(camera_sector, &cam.camera_pos, &cam.camera_forward));
//		gx_sector_outline(camera_sector,ztrue);	
		

//	gx_portal_inactive_draw_test(portal);

		//draw all inactive portals
		
#if 0
		gx_zbuffer(zfalse);
		seccnt=0;

		for (i=0;i<vec_count(sectors);i++)
		{

			gx_portal_t* p;
			sect = vec_get_at(sectors, i);
			if (!sect)
				continue;

			seccnt++;
	
			p = sect->portals;
			while(p)
			{
				if(p->lastpeeked != framecount) 
				{
					if (p->target->lastframe == framecount)
						gx_portal_inactive_draw_test(p);
				}

				p = p->next_portal;
			}
	
			
		}
		gx_zbuffer(ztrue);
#endif
		//draw in 2d
		gx_text_size(16,16, 0);
		gx_setup_2d_pixels(NULL,NULL);
		gx_set_active_lights(NULL, 0);

		{
			char buf[100];
			sprintf(buf," %d / %d  (%3.2f %%) sectors drawm\n" , secdraw, seccnt, secdraw *100. / seccnt );
			gx_text_draw(font, 0,0, 0, buf);
		}

		//do a simple projectiontest
		if (0) {
			float ix;
			float iy;
			float iz;
			gx_portal_t* p_test;

			gx_portal_t* port;

			gx_setup_2d(-1,1, 1, -1);
			gx_text_size(.05, .05,0);
			

			

			for (i=0;i<vec_count(sectors);i++)
			{
				sect = vec_get_at(sectors, i);

				if (sect)
				{
					for (port = sect->portals;port;port = port->next_portal)
					{
						

						vec3 p;
						int i;
					
						if (vec_project_3d(&port->pos, &cam, &p))
						{
							gx_text_draw(font, p.named.x, p.named.y, 0, "X");
						}

						for (i=0;i<4;i++)
						{
							char ss[] = {0,0};
							ss[0] = '0'+i;
							if (vec_project_3d(&port->points[i], &cam, &p))
							{
								gx_text_draw(font, p.named.x, p.named.y, 0, ss);
							}

						}


					}
					
					

				}


			}



		}



		gx_frame_show();
		gx_window_event();
		if (gx_key_state('x'))
			break;
	}

	ram_free(light); //free the light

}


void main(int argc, char** argv)
{

	test_mem();
	test_vectors();
	struct_test();


	//simple_graphics_test();
	//intersect_quad_test();

	stringmap_test();

	growstr_test();
	

	
	



	if(1){
		stringmap_t textures;
		vec_t* sectors = NULL;
		


		if (GX_OK != gx_init( 640, 480 , "Test Graphics Window"))
		{
			printf("Init Fail\n");
			return;
		}


		stringmap_mk(&textures, 4);

		sectors = load_level(&textures, "level.txt");

		printf(" %d sectors\n ", vec_count(sectors));

		level_test(sectors, &textures);

		vec_cleanup(&textures);
		ram_free(sectors);
	}



//	sector_test();
	
	
	printf("allocations left: %d\n", ram_allocs());
	gx_disable();
	return 0;
}




