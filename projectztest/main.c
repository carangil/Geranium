// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#include "..\ztypes.h"
#include "..\vmath.h"
#include "..\memory\ram.h"
#include "..\structures\vector.h"
#include <stdio.h>
#include "..\graphics\gx_sys.h"
#include "..\graphics\gx_image.h"
#include "..\graphics\gx_sprite.h"
#include "..\graphics\gx_buffers.h"


typedef struct test_s
{
	char* string;
	struct test_s* next;
} test_t;



void destruct_test(test_t* t)
{
	test_t* p;

	while(t)
	{
		p = t->next;
		ram_free(t->string);
		ram_shallow_free(t);
		t = p;
	}

}

test_t* mk_test(char* z , test_t* next)
{
	test_t* t = ram_alloc( sizeof(test_t), destruct_test);
	t->next = next;
	t->string = z;

	return t;
}

typedef struct inner_s
{
	char* a;
	char* b;
} inner_t;

void inner_free(inner_t* x)
{
	ram_free(x->a);
	ram_free(x->b);

	ram_shallow_free(x);
}

typedef struct outer_s
{
	char* string;
	inner_t* inner;
} outer_t;

void outer_free(outer_t* z)
{
	ram_free(z->inner);
	ram_free(z->string);
	ram_shallow_free(z);
}




void test_mem()
{

test_t* t1=0;

	char* z = NULL;


	t1 = mk_test( ram_strdup("This "), mk_test(ram_strdup("test"), NULL)  );
	

	z  = ram_alloc(100, NULL);

	printf("hello %p %d\n", z, ram_allocs());

	ram_free(z);



	printf("%s %s\n", t1->string, t1->next->string);

	ram_free(t1);

	printf("%d\n", ram_allocs());
	

	{
		outer_t* test = ram_alloc(sizeof(outer_t), outer_free);

		test->inner = ram_alloc(sizeof(inner_t), inner_free);

		test->inner->a = ram_strdup("a string\n");
		test->inner->b = ram_strdup("other string\n");
		test->string = ram_strdup("third string\n");

		printf(" %s %s %s\n", test->inner->a, test->inner->b, test->string);

		ram_free(test);

		printf("%d\n", ram_allocs());

	}


}

void test_vectors()
{
	vec_t  svec;

	vec_t* vec = NULL;
	
	vec = vec_mk(NULL, 2);

	vec_add(vec, ram_strdup("string0\n"));
	vec_add(vec, ram_strdup("string1\n"));
	vec_add(vec, ram_strdup("string2\n"));
	vec_add(vec, ram_strdup("string3\n"));
	vec_add(vec, ram_strdup("string4\n"));

	vec_print(vec);

	//printf(" %s \n", vec_get_at(vec, 7));
	//printf(" %s \n", vec_get_at(vec, 4));
	//printf(" %s \n", vec_get_at(vec, 1));
	//printf(" %s \n", vec_get_at(vec, 0));



	ram_free(vec_remove_ordered(vec, 2));

	vec_print(vec);


	ram_free(vec_get_at(vec, 1)); // free an item in the vector
	//replace item with a new item
	vec_set_at(vec, 1, ram_strdup("THIS IS CRAP"));

	vec_print(vec);

	ram_free(vec);

	printf("allocations left: %d\n", ram_allocs());


	//now try a statically allocated svec structure

	if (!vec_mk(&svec, 2))
	{
		printf("Could not allocate!\n");
	}
	
	vec_add(&svec, ram_strdup("S0"));
	vec_add(&svec, ram_strdup("S1"));
	vec_add(&svec, ram_strdup("S2"));
	vec_add(&svec, ram_strdup("S3"));
	vec_add(&svec, ram_strdup("S4"));

	vec_print(&svec);

	vec_cleanup(&svec);

	printf("allocations left: %d\n", ram_allocs());
}


void test_graphics()
{

	zfloat32 cx=0;
	zfloat32 cy=.5;
	zfloat32 cz=1;


	vec3 camera_pos;
	vec3 camera_up;
	vec3 camera_right;
	vec3 camera_forward;



	zfloat32 cxs=0;
	zfloat32 cys=0;
	zfloat32 czs=0;

	zfloat32 xxx=0;
	zfloat32 yyy=0;

	gx_vbuffer_t * vbuf = NULL;
	gx_image_t *image1 = NULL;
	gx_image_t *image2 = NULL;
	gx_sprite_t* sprite = NULL;

	gx_image_t* heightmap = NULL;

	gx_vbuffer_t* heightmesh = NULL;

	printf("Init graphics\n");
	gx_init(640, 480 , "Test Graphics Window");

	gx_clear_color(.1,.1,.6,1);
	gx_frame_clear(ztrue,ztrue);
	gx_frame_show();

	image1 = gx_image_load_tga( "rgbatarga.tga");
	image2 = gx_image_load_tga( "tex2.tga");
	
	
	//gx_image_enable(image1);  //don't need to enable because first use will

	vec3set(camera_pos,		0,	.5,	1);
	vec3set(camera_right,	1,	0,	0);
	vec3set(camera_up,		0,	1,	0);
	vec3set(camera_forward,	0,	0,	-1);

	sprite = gx_sprite_mk(image1,100,100, image1->width, image1->height, .1, .1);

	vbuf = gx_vbuffer_mk(100, 12, ztrue, 1);
	
	{
		zuint32 v0=GX_INDEX_INVALID;
		zuint32 v1=GX_INDEX_INVALID;
		zuint32 v2=GX_INDEX_INVALID;
		zuint32 v3=GX_INDEX_INVALID;
		zuint32 v4=GX_INDEX_INVALID;


		gx_vbuffer_add_tex(vbuf, 0, 0,0);
		gx_vbuffer_add_color(vbuf, 1, 1, 1, 1);
		v0=gx_vbuffer_add_vertex(vbuf, .1,.1,0);

		gx_vbuffer_add_tex(vbuf,0, 0,1);
		gx_vbuffer_add_color(vbuf, 0, 1, 0, 1);
		v1=gx_vbuffer_add_vertex(vbuf, .2, .1,0);

		gx_vbuffer_add_tex(vbuf,0, 1,1);
		gx_vbuffer_add_color(vbuf, 0, 0, 1, 1);
		v2=gx_vbuffer_add_vertex(vbuf, 0,.2,0);

		gx_vbuffer_add_tex(vbuf,0, 1,1);
		gx_vbuffer_add_color(vbuf, 0, 0, 1, 1);
		v3=gx_vbuffer_add_vertex(vbuf, .3,.2,0);


		gx_vbuffer_add_tex(vbuf,0, 1,1);
		gx_vbuffer_add_color(vbuf, 0, 0, 1, 1);
		v4=gx_vbuffer_add_vertex(vbuf, .15,.3,0);

		/*
        4

    2       3

      0   1
*/



		gx_vbuffer_add_index(vbuf, v0);
		gx_vbuffer_add_index(vbuf, v4);

		gx_vbuffer_add_index(vbuf, v4);
		gx_vbuffer_add_index(vbuf, v1);

		gx_vbuffer_add_index(vbuf, v1);
		gx_vbuffer_add_index(vbuf, v2);

		gx_vbuffer_add_index(vbuf, v2);
		gx_vbuffer_add_index(vbuf, v3);


		gx_vbuffer_add_index(vbuf, v3);
		gx_vbuffer_add_index(vbuf, v0);

	}


	gx_vbuffer_update(vbuf);  //make sure latest data is ready

	heightmap = gx_image_load_tga("heightmap.tga");

	heightmesh = gx_mesh_from_image(heightmap, 0.0,0.0,0.0,   //offset
		0,1,2,        //axis swizzle
		3.0,.1,3.0,  //scaling
		ztrue, 2);

	gx_vbuffer_update(heightmesh);

	gx_mouse_capture(ztrue); //capture the mouse for relative motion

	while( gx_window_event() == GX_LOOP_NOTHING)
	{
		int i;
		zint32 mx, my;


		gx_mouse_pos(&mx, &my);

		printf(" mouse position %d %d\n", mx, my);

		gx_frame_clear(ztrue,ztrue);

		//gx_setup_2d(-1,1,1,-1);

		


		//gx_vbuffer_draw(vbuf,2,4, gx_lines, ztrue);

	
		
		//gx_sprite_draw(sprite, xxx,yyy);

		xxx+=.01;
		yyy+=.03;

		if (xxx>1) xxx=-1;
		if (yyy>1) yyy=-1;
	
		gx_setup_3d( 70.0, gx_get_image_dimensions(NULL,NULL), .1, 1000);


		{
			char c = gx_getkey();
			zfloat32 yaw	= 0.0;
			zfloat32 pitch	= 0.0;
			zfloat32 roll	= 0.0;

			switch(c)
			{
			case 'Q':
				exit(0);


			case'w':
				czs+=.01; break;

			case's':
				czs-=.01; break;

			case'a':
				cxs-=.01; break;
			case'd':
				cxs+=.01; break;

			case'r':
				cys+=.01; break;

			case'f':
				cys-=.01; break;

			case'8':
				pitch = .05;
				break;

			case'2':
				pitch = -.05;
				break;


			case'6':
				yaw = .05;
				break;

			case'4':
				yaw = -.05;
				break;

			case'q':
				roll = .05;
				break;

			case'e':
				roll = -.05;
				break;


			}


			pitch += my*.001;
			yaw += mx*.001;


			//cx+=cxs;
			//cy+=cys;
			//cz+=czs;

			vec3madd(camera_pos, cxs, camera_right);
			vec3madd(camera_pos, cys, camera_up);
			vec3madd(camera_pos, czs, camera_forward);


			//try some spin crap
			gx_spin(yaw, pitch, roll,&camera_right, &camera_up, &camera_forward);


		}
		
	//	vec3set(camera_pos, cx, cy, cz);

		
		gx_camera_pos_rot( &camera_pos, &camera_right, &camera_up, &camera_forward);

		//gx_camera_pos(cx,cy,cz);
		//gx_camera_pos_rot( position, vec3* xaxis, vec3* yaxis, vec3* zaxis);
		
		//_gx_set_active_textures( NULL , 0);
		{
			gx_image_t * txlist[2];
			txlist[0]=image1;
			txlist[1]=image2;
			gx_set_active_textures( txlist, 2);
		}

		gx_vbuffer_draw(heightmesh,0,heightmesh->index_count, gx_triangles, ztrue);

		gx_frame_show();
	}

}

int main(int argc, char** argv)
{

	//test_mem();
	//test_vectors();
	test_graphics();
#if 0
	vec3 a;
	vec3 b;
	
	vec3set(a, 1.1, 2.22, 3.333);
	printf(" %f %f %f \n", a.vec3p[0], a.vec3p[1], a.vec3p[2]);
	vec3mov(b,a);
	printf(" %f %f %f \n", b.vec3p[0], b.vec3p[1], b.vec3p[2]);	
	b.vec3y= 22.222;
	printf(" %f %f %f \n", b.vec3p[0], b.vec3p[1], b.vec3p[2]);	


	printf("allocations left: %d\n", ram_allocs());
#endif 
	return 0;
}
