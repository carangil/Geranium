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


void test_mem();

void test_vectors();

//void graphics_main();
//void game_main();


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
		gx_text_size( 12, 12, 0);
		gx_text_draw(font, 320, 240,angle,"Testing some text");


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

/// need to make a stringmap
typedef struct stringmap_entry_ts
{
	char* key;
	void* item;
	zbool cleanitem;
} stringmap_entry_t;

typedef vec_t stringmap_t;

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
int next_word(char** word_start, char** word_end)
{
	//eat leading white space
	
	if (*word_end)
			*word_start = *word_end;

	while ( zspace(**word_start))
		(*word_start) ++;

	*word_end = *word_start;
	while( **word_end && !zspace(**word_end))
		(*word_end) ++;

		
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

void load_level(zchar* filename)
{

	FILE* f;
	zchar* line;
	int ret =0;
	char* word_start;
	char* word_end;

	f = fopen(filename, "rb");
	if (!f)
		return;  //can't load

	while (ret != -1)
	{
		ret = read_line(f, &line);

		if (line)
		{
			int len=1;
			printf(" LINE {%s}\n", line);
			word_start = line;
			word_end = NULL;

			
			len = next_word(&word_start, &word_end);
			if (!wordcmp(word_start, len, "$floor"))
			{
				printf("begin floorheight\n");


			}

			ram_free(line);
		}

	}


	fclose(f);
}


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

void addwall(gx_vbuffer_t* vb, vec3* a, vec3* b, vec3* c, vec3* d)
{
	int p0,p1,p2,p3;

	gx_vbuffer_add_tex(vb, 0, 0,1);
	p0 = gx_vbuffer_add_vertex(vb, a->vec3x, a->vec3y, a->vec3z);

	gx_vbuffer_add_tex(vb, 0, 1,1);
	p1 = gx_vbuffer_add_vertex(vb,  b->vec3x, b->vec3y, b->vec3z);

	gx_vbuffer_add_tex(vb, 0, 1,0);
	p2 = gx_vbuffer_add_vertex(vb,  c->vec3x, c->vec3y, c->vec3z);

	gx_vbuffer_add_tex(vb, 0, 0,0);
	p3 = gx_vbuffer_add_vertex(vb,  d->vec3x, d->vec3y, d->vec3z);

	gx_vbuffer_add_index(vb, p0);
	gx_vbuffer_add_index(vb, p2);
	gx_vbuffer_add_index(vb, p1);

	gx_vbuffer_add_index(vb, p0);
	gx_vbuffer_add_index(vb, p3);
	gx_vbuffer_add_index(vb, p2);
}


gx_sector_t* sector_cube_mk(vec3* in_min, vec3* in_max, sect_parms_t* parms)
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

	textures = vec_mk(NULL, 6);
	textures->own_elements = zfalse;  /* only referening, not 'owning' */

	for (i=0;i<6;i++)
	{
		if (vec_find_idx(textures, parms->walls[i]) == -1 )
		{
			vec_add(textures, parms->walls[i]);
		}
	}

	printf(" %d unique textures for this sector\n", vec_count(textures));
	vb = gx_vbuffer_mk(100,100,zfalse, zfalse, 1);

	s = gx_sector_mk(in_min, in_max);


	//for each texture style
	for (i=0;i< vec_count(textures);i++)
	{

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
					addwall(vb, wall_points[j][0], wall_points[j][1], wall_points[j][2], wall_points[j][3]);

				if(parms->floor_gap[j])
				{
					vec3mov(t1, *wall_points[j][0]);
					vec3mov(t2, *wall_points[j][1]);
					t1.vec3y = parms->floor_gap_y[j];
					t2.vec3y = t1.vec3y;
					addwall(vb, &t1, &t2, wall_points[j][2], wall_points[j][3]);
				}
				if(parms->ceiling_gap[j])
				{
					vec3mov(t1, *wall_points[j][2]);
					vec3mov(t2, *wall_points[j][3]);
					t1.vec3y = parms->ceiling_gap_y[j];
					t2.vec3y = t1.vec3y;
					addwall(vb, wall_points[j][0], wall_points[j][1], &t1, &t2);
				}
					

			}

		}

		
		end = gx_vbuffer_current_index(vb);
		
		if (start == end)
			continue;


		mesh = gx_mesh_def(vb, parms->walls[i], start, end, ztrue);

		vec_add(&(s->meshes), mesh);

	}
	

	ram_free(textures);
	
	gx_vbuffer_update(vb);
	ram_free(vb);  //we don't need it anymore (meshes will refcount it alive)


	return s;

}


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

	sect_parms.walls[LEFT] = brick2style;
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
		gx_sector_outline(sect);

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

void main(int argc, char** argv)
{

	test_mem();
	test_vectors();
	struct_test();

	//simple_graphics_test();

	stringmap_test();

	growstr_test();

//	load_level("level.txt");

	sector_test();
	

	printf("allocations left: %d\n", ram_allocs());
	return 0;
}




