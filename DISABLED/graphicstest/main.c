// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

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

#include "gx_mesh.h"

#include "gx_misc.h"
#include "gen.h"


#if 1


#endif

#define VEC3PASS(v)   ((v).vec3x),((v).vec3y),((v).vec3z)



//return last occurance of character
char* lastchr(char* s, char c){
	char* p=NULL;
	while (*s) {
		if ((*s)==c)
			p=s;
		s++;
	}
	return p;
}


gx_sector_t* find_sector( zvec_t* sectors, char* name) {

	int i;
	vec3 p;
	vec3set (p,0,0,0);

	gx_sector_t* s;

	for (i=0;i<zvec_count(sectors);i++) {
		s = zvec_get_at(sectors, i);
		if (!strcmp(s->name, name))
			return s;
	}
	printf(" %s not found, creating\n", name);


	s=gx_sector_mk(name, &p, &p);
	zvec_add(sectors, s);
	return s;


}

typedef struct pending_portal_s{

	gx_sector_t* room1;
	gx_sector_t* room2;
	vec3	     center;
	float	     radius;
} pending_portal_t;

void process_obj(zvec_t* sectors, zvec_t* pending_portals, gx_mesh_t* obj,vec3* center, float radius) {
	char *p;
	char *name;
	char *name1;
	char *name2;
	gx_sector_t* s;

	pending_portal_t* np=NULL;


	if (!obj->name) {
		return;
	}
	printf(" CHECK %s\n", obj->name);


	name = ram_strdup(obj->name);

	
	p = lastchr(name, '_');
	if (p)
		*p=0; //end


//	printf("%s^^^%s\n", name,p+1);

	if (!strncmp(name, "sportal.",8)){
	
		name1 = name+8;
		name2 = strchr(name1, '.');
		if (name2) {
			*name2=0;
			name2++;
		}

		printf(" {%s} and {%s}\n", name1, name2);

		np = ram_alloc(sizeof(pending_portal_t), NULL);
		np->room1 = find_sector(sectors, name1);
		np->room2 = find_sector(sectors, name2);
		np->radius = radius;
		np->center = *center;

		//add to pending portal list
		zvec_add(pending_portals, np);


	} else {  //just a room
		s = find_sector(sectors, name);
		s->mesh = obj;
		s->center = *center;
		//printf(" Create room %s with center %f %f %f\n", s->name, VEC3PASS(s->center));		
	}

	ram_free(name);
	return;

}	


zvec_t* sectors_from_obj(gx_mesh_t* objs) {

	zvec_t* sectors = zvec_mk(NULL,10);
	zvec_t* pending_portals = zvec_mk(NULL,10);
	int i;

	char* o_name=NULL;

	gx_mesh_t* obj=objs;
	gx_mesh_t* startobj=NULL;
	gx_mesh_t* prev = NULL;

	vec3 average;
	vec3 p;
	int  naverage;
	float radius=0;

	while(obj) {

		int i;
		
		if (obj->name) {
			if ( (!o_name) || strcmp(o_name, obj->name)) {
				//if next mesh has a name, and the name is different
				if (naverage >0) {
					vec3scale(average, 1.0/naverage);
					vec3sub (p, average);
					radius = sqrt( vec3abs_sq(p));
					printf(" Object %s has %d points average %f %f %f  radius %f\n", o_name,naverage, VEC3PASS(average), radius);
					
					process_obj(sectors,pending_portals, startobj, &average, radius);
//					printf(" done process obj\n");
					if (prev)
						prev->next = NULL;
				}
				naverage = 0;
				vec3set(average, 0,0,0);
				printf("start object %s\n", obj->name);
				o_name = obj->name;
				startobj = obj;
			} else {
				printf("continue object %s\n", obj->name);
			}
		}

		printf(" load %d to %d from buffer %p\n", obj->drawstart, obj->drawend, obj->data);
		for(i=obj->drawstart;i<obj->drawend;i++) {
			int j;
			gx_vbuffer_get_i(j, obj->data, i);
			gx_vbuffer_get_v(p, obj->data, j);
		//	vec3print(p);printf("\n");	
			vec3add(average,p);

		}
		naverage += (obj->drawend-obj->drawstart);
		prev = obj;
		obj=obj->next;
	}

	//last object
	if (naverage >0) {
		vec3scale(average, 1.0/naverage);
		printf(" Object %s has %d points average %f %f %f\n", o_name,naverage, VEC3PASS(average));
		process_obj(sectors, pending_portals, startobj, &average, radius);
	}
	
	for (i=0;i<zvec_count(pending_portals);i++) {
		pending_portal_t* pp = zvec_get_at(pending_portals, i);

		printf(" Pending portal: %s %s %f\n",pp->room1->name,pp->room2->name, pp->radius); 
		gx_sector_add_portal_sphere( pp->room1, &pp->center, pp->radius, pp->room2, NULL);
		gx_sector_add_portal_sphere( pp->room2, &pp->center, pp->radius, pp->room1, NULL);

	}	
	//exit(0);

	return sectors;
}



int main(int argc, char** argv) {


	int run_tesselator=1;
	gx_image_t * tex=NULL;
	gx_camera_t	player_camera;
	gx_camera_init(&player_camera);
	zvec_t* materials;
	zvec_t* sectors=NULL;
	gx_sector_t *camera_sector=NULL;
	gx_mesh_t* testobj=NULL;

	gx_init(800, 600 , "Test", GX_OPTION_NO_SHADER);
	//gx_init(800, 600 , "Test", 0);

	//testobj = gx_mesh_load_obj(NULL, "../shared/untitled.obj", NULL);
	//testobj = gx_mesh_load_obj(NULL, "/home/alarm/Downloads/blendermodels/test.obj", NULL);




//	#define FOLDER 			"/home/alarm/Downloads/blendermodels/chicken/"
//	#define FOLDERTEX FOLDER 	"textures/"
//	#define OBJ    			"chickenV2.obj"
//	#define MTL			"chickenV2.mtl"


//	#define FOLDER 			"/home/alarm/Downloads/blendermodels/humans/free3d-nude/"
//	#define FOLDERTEX FOLDER 	""
//	#define OBJ    			"WhipperNude" ".obj"
//	#define MTL			"WhipperNude-tga" ".mtl"

//	#define FOLDER 			"/home/alarm/Downloads/blendermodels/humans/soldier_DM/"
//	#define FOLDERTEX FOLDER 	""
//	#define OBJ    			"Soldier_final" ".obj"
//	#define MTL			"soldier_final" ".mtl"


	
	#define FOLDER 			"../shared/"
	#define FOLDERTEX FOLDER 	""
	#define OBJ    			"testmap" ".obj"
	#define MTL			"testmap" ".mtl"



	materials = gx_drawstyle_load_mtl(NULL, FOLDER MTL, FOLDERTEX);

	testobj = gx_mesh_load_obj(NULL, FOLDER OBJ, materials);
	
	ram_free(materials);//the mesh will keep any materials still used alive via reference counts



	sectors = sectors_from_obj(testobj);
	camera_sector = find_sector(sectors, "start");

	//tex = gx_image_load_tga("../shared/label.tga");
	tex = gx_image_load_tga("../shared/rock.tga");

	gx_drawstyle_t* teststyle = gx_drawstyle_mk("teststyle", tex);
	gx_drawstyle_t* teststylenotex = gx_drawstyle_mk("notexture", NULL);

	gx_environment_t* testenv = gx_environment_mk();
	//gx_environment_t* nolights = gx_environment_mk();
	
	//gx_shadergroup_t* sg = gx_shader_source("@../graphics/shader.v", "@../graphics/shader.f");

	gx_shadergroup_t* sg = NULL;  //should force use of default shader

	gx_light_t* li;
	{
		vec3 p,c,ca;
		vec3set(p, -10,10, 10);
		vec3set(c, 1, .8, .9);
		vec3set(ca, .1, .1, .3);
		li = gx_light_mk(gx_light_point, &p, &c, &ca  );
		gx_light_set_attenuation( li, ZTRUE, 10, 10, 1);


		zvec_add_or_free(&testenv->lights, li);


//		vec3set(c, 1,1,.4);
////		vec3set(p, 10,-10, -10);
//		li = gx_light_mk(gx_light_point, &p, &c, &ca  );
//		zvec_add_or_free(&testenv->lights, li);

	}


	gx_clear_color(0,0,0,1);


	gx_setup_3d( 90.0, 4.0/3.0, .1, 100);



	//create gx_quadpatch

	#define PLANET_SIZE 3


	gx_quadpatch_sys_t qpsys;

	gx_quadpatch_sys_init(&qpsys);



	vec3 origin;
	//vec3set (origin, 0,-3,-3);
	vec3set (origin, 0, 0,0);


	gx_quadpatch_detailer_f detailers[] = {
		gen_random_detail,gen_random_detail,gen_random_detail,
		gen_random_detail,gen_random_detail,gen_random_detail};
		
	
	gen_qpcube(&qpsys,
				&origin,
				17,
				2.0,
				NULL,
				NULL,
				detailers,
				/*GEN_SPHERE | GEN_INSIDE*/ GEN_INSIDE,
				30, 0.07
  			);


	
	gx_mouse_capture(ZTRUE);
	//The game loop 
	for(;;)  
	{


		gx_window_event();  //handles any window events (I/O)


		{
			//move the light position around

			//	vec3set(zvec_elements_as(gx_light_t*, &testenv->lights)[0]->position, cos(ang*DEGREE * .1), 0, 0);

		}

		//camera control
		{
			zfloat32 delta_yaw		= 0.0;
			zfloat32 delta_pitch	= 0.0;
			zfloat32 delta_roll		= 0.0;

			vec3	 delta_pos;
			vec3set	 (delta_pos, 0,0,0);

			int mouse_x, mouse_y;
			zbool	mouse_relative;

			//speed of motion
#define SSS .05
			if (gx_key_state('w')) delta_pos.vec3z+=SSS;
			if (gx_key_state('s')) delta_pos.vec3z=-SSS;
			if (gx_key_state('a')) delta_pos.vec3x=-SSS;
			if (gx_key_state('d')) delta_pos.vec3x=+SSS;
			if (gx_key_state('r')) delta_pos.vec3y=+SSS;
			if (gx_key_state('f')) delta_pos.vec3y=-SSS;

			char x = gx_getkey();

			if (x=='m') gx_mouse_capture(ZFALSE);
			if (x=='M') gx_mouse_capture(ZTRUE);
			if (x=='t') run_tesselator^=1;

			if (x=='l') gx_wireframe(ZTRUE);
			if (x=='L') gx_wireframe(ZFALSE);

			if (x=='~') break;

			if (gx_key_state('q')) delta_roll=-SSS;
			if (gx_key_state('e')) delta_roll=SSS;

			if (gx_key_state('z')) delta_yaw=-SSS;
			if (gx_key_state('x')) delta_yaw=SSS;

			if (gx_key_state('g')) delta_pitch=-.02;
			if (gx_key_state('b')) delta_pitch=.02;

	{
			vec3 world_delta_pos;
			vec3 world_delta_pos_norm;
			vec3set(world_delta_pos, 0,0,0);

			//move camera using camera's basis
			vec3madd(world_delta_pos, delta_pos.vec3x, player_camera.rot.x_axis);
			vec3madd(world_delta_pos, delta_pos.vec3y, player_camera.rot.y_axis);
			vec3madd(world_delta_pos, delta_pos.vec3z, player_camera.rot.z_axis);
			printf("\t\t\t world deltapos %f %f %f\n", VEC3PASS(world_delta_pos));

			vec3mov(world_delta_pos_norm, world_delta_pos);
			vec3normalize(&world_delta_pos_norm);
			zbool block = ZFALSE;
			gx_mesh_t* m = camera_sector->mesh;
			while (m) {	

				block |= gx_vbuffer_collide(m->data, m->drawstart, m->drawend,  &player_camera.pos, &world_delta_pos_norm);
					

				m=m->next;
			}

			if (block) {
				printf(" HIT\n");
			}else {
				vec3add(player_camera.pos, world_delta_pos);
			}


		}







			//lets spin camera  (relative to its own coord system)
			gx_mouse_pos(&mouse_x, &mouse_y, &mouse_relative);

			if (!mouse_relative)
			{
				//if some some reason we get an absolute mouse position, clear it out
				mouse_x=0;
				mouse_y=0;
			}

			delta_pitch += mouse_y*.003;
			delta_yaw += mouse_x*.003;

			if (fabsf(delta_pitch) < .3 && fabsf(delta_yaw) < .3)  //large mouse movements are probably just entering/leaving window
				gx_spin(ZTRUE, delta_yaw, delta_pitch, delta_roll, &player_camera.rot);

		}	





		GX_TRACE


		{ 
			vec3 p;
			vec3set(p, .5,.5,.5);
			vec3mov(li->position, player_camera.pos); //set light to camera pos	
			vec3add(li->position, p);
		}

		gx_camera_pos_rot( &player_camera.pos,&player_camera.rot.x_axis, &player_camera.rot.y_axis, &player_camera.rot.z_axis);


		gx_env_evaluate_lights(testenv);
	
		GX_TRACE

		//clear screen		
		gx_frame_clear(ZTRUE, ZTRUE);

		//teststyle->specular_exponent=40.0;
		//vec3set(teststyle->specular_color, 1, 1, 1);
		vec3set(teststyle->specular_color, 0, 0, 0);

		teststyle->shadergroup =sg;
		teststylenotex->shadergroup =sg;


		GX_TRACE
		gx_set_environment(testenv);
		GX_TRACE
		gx_drawstyle_activate(teststyle);
		GX_TRACE


	/*
		if (run_tesselator)
			gx_quadpatch_sys_eval(&qpsys, &player_camera);
*/
	
		//gx_quadpatch_sys_draw(&qpsys, &player_camera);
	
	
//		gx_set_environment(nolights);
//		gx_drawstyle_activate(teststylenotex);

		{
			gx_sector_t* ncs = gx_traverse_sectors(&player_camera, camera_sector );
			if (ncs)
				camera_sector = ncs;
		}


	

		{
				int i;
				gx_immediate(gx_lines);
				for (i=0;i<zvec_count(&testenv->lights);i++) {
					float green[4] = { 0,1,0,1};
					vec3 v;
					vec3 p;
					
					gx_light_t* li = zvec_get_at(&testenv->lights, i);
					
					
					vec3set(p,.1,.1,.1);
					vec3mov(v, li->position);
					gx_point(&v, NULL, green, 0,0);
					vec3add(v, p);
					gx_point(&v, NULL, green, 0,0);
				}
				gx_end();
		}
		
		gx_set_environment(testenv);
		gx_drawstyle_activate(teststyle);

		//gx_mesh_draw(testobj);


		
/*
		gx_sector_outline(camera_sector,ZTRUE);
		{
			int a=0;
			int b=0;
			gx_sector_t* t =zvec_get_at(&sectors, (a+1)*10+b);
			gx_sector_outline(t,ZTRUE);
		}*/
#if 0
		gx_sector_outline(camera_sector,ZTRUE);
	
		{
				gx_sector_t * s;
				int i;
				for (i=0;i<zvec_count(&sectors);i++) {
					gx_sector_t * s = zvec_elements_as(gx_sector_t*, &sectors)[i];
					
					gx_sector_outline(s,ZTRUE);
					
				}
		}
#endif


		gx_frame_show(); 
		
		GX_TRACE

	}
	gx_disable();

	printf("allocations left: %d\n", ram_allocs());
	return 0;
} 
