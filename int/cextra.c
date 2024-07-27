
#include "int.h"
#include "gfx_gl.h"

void store16(int val, zuint16* zp) {
	*zp = val;
}

zint32 load16(zuint16* z) {
	return *z;
}





tokenT* h_vec3add(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	
	vec3add(    
		*(vec3*)&(ex->stack[ex->sp - 2]),
		*(vec3*)&(ex->stack[ex->sp - 1])
	);

	ex->sp -= 1;

	return tnext(t);
}


tokenT* h_vec3sub(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	vec3sub(
		*(vec3*)&(ex->stack[ex->sp - 2]),
		*(vec3*)&(ex->stack[ex->sp - 1])
	);

	ex->sp -= 1;

	return tnext(t);
}


tokenT* h_vec3cross(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));
	
	vec3 tmp;

	vec3cross(tmp,
		*(vec3*)&(ex->stack[ex->sp - 2]),
		*(vec3*)&(ex->stack[ex->sp - 1])
	);
	
	*(vec3*)&(ex->stack[ex->sp - 2]) = tmp;

	ex->sp -= 1;

	return tnext(t);
}

tokenT* h_vec3dot(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	float tmp;

	tmp = vec3dot(
		*(vec3*)&(ex->stack[ex->sp - 2]),
		*(vec3*)&(ex->stack[ex->sp - 1])
	);

	ex->stack[ex->sp - 2].as.f = tmp;
	
	ex->sp -= 1;

	return tnext(t);
}

//type reflection

typeT* type_member(typeT* t, zuint32 i)
{
	if (!t)
		return NULL;
	if (!t->members)
		return NULL;
	if (i >= zvec_count(t->members))
		return NULL;

	return zvec_get_at(t->members, i);
}

zbool ptrequal(void* a, void* b) {
	return a == b;
}

#define OFTYPE(TTT,CCC) findType(CCC,TTT,NULL,0)
#define TYPE(NNN) findType(NAMED, NULL, NNN,0)

#include "gfx_gl.h"



typedef struct {
	char* vsource;
	char* fsource;
	zvecT* shader_inputs; //shader_inputT 
	gfx_shaderT* shader;
} graphics_pipelineT;

zbool pipline_free(void* v) {
	graphics_pipelineT* p = v;
	ram_free(p->vsource);
	ram_free(p->fsource);
	ram_free(p->shader_inputs);
	ram_free(p->shader);
	return ZTRUE;
}

tokenT* h_glslprocbody(exectxT* ex, tokenT* t) {

	typeT* v3 = OFTYPE(OFTYPE(TYPE("Vec3"), ARRAYDYNAMIC), POINTERUSER);
	typeT* v4 = OFTYPE(OFTYPE(TYPE("Vec4"), ARRAYDYNAMIC), POINTERUSER);
	typeT* v2 = OFTYPE(OFTYPE(TYPE("Vec2"), ARRAYDYNAMIC), POINTERUSER);
	typeT* v1 = OFTYPE(OFTYPE(TYPE("Vec1"), ARRAYDYNAMIC), POINTERUSER);

	typeT* m33 = OFTYPE(OFTYPE(TYPE("Matrix33"), ARRAYDYNAMIC), POINTERUSER);
	typeT* m44 = OFTYPE(OFTYPE(TYPE("Matrix44"), ARRAYDYNAMIC), POINTERUSER);


	exe(ex, tsub(t));

	char* vsource = (ex->stack[ex->sp - 2].as.ptr.block + ex->stack[ex->sp - 2].as.ptr.offset);
	char* fsource = (ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset);


	graphics_pipelineT* gp = ram_alloc(sizeof(graphics_pipelineT), pipline_free);



	gfx_shader_inputT* si = NULL;

	gp->vsource = zstrdup(vsource);
	gp->fsource = zstrdup(fsource);	
	gp->shader_inputs = zvec_mk(NULL, 8);

	if (ex->in_immediate && ex->in_immediate->parent) {

			typeT* ftype = ex->in_immediate->parent->type;
		if (ftype && ftype->category == FUNCTION) {
			int i = 0;
			typeT* arg = NULL;
			while (arg = type_member(ftype, i)) {
				printf(" arg %d \n", i);

				si = ram_alloc(sizeof(*si), NULL);
				int argtype = 0;
				
				//array types: for now, count is 1 because 
				if (arg->ref == v1)
					si->type = GFX_FLOAT | GFX_ARRAY;
				else if (arg->ref == v2)
					si->type = GFX_FLOAT2 | GFX_ARRAY;
				else if (arg->ref == v3)
					si->type = GFX_FLOAT3 | GFX_ARRAY;
				else if (arg->ref == v4)
					si->type = GFX_FLOAT4 | GFX_ARRAY;
				else if (arg->ref == TYPE("Real"))
					si->type = GFX_FLOAT;
				else if (arg->ref == TYPE("Vec2"))
					si->type = GFX_FLOAT2;
				else if (arg->ref == TYPE("Vec3"))
					si->type = GFX_FLOAT3;
				else if (arg->ref == TYPE("Vec4"))
					si->type = GFX_FLOAT4;
				else if (arg->ref == OFTYPE(TYPE("Matrix33"), POINTERUSER))
					si->type = GFX_MAT33;
				else if (arg->ref == OFTYPE(TYPE("Matrix44"), POINTERUSER))
					si->type = GFX_MAT44;
				else if (arg->ref == OFTYPE(TYPE("Texture"), POINTERUSER))
					si->type = GFX_TEXTURE;


				if (arg->isPer) {
						printf("attr ");
					}		else {
					printf("unfm ");
					si->count = 1;	//for now arrays will be single for
					si->uniform = ZTRUE;
				}

				si->name = arg->name;  //borrow name (arg belongs to the function this is being compiled for.)

				zvec_add(gp->shader_inputs, si);

				printTypeNoRedirect(arg, ZTRUE, ZFALSE);
				i++;
			}
			

		}

	}

	
	ex->sp--;
	
	ex->stack[ex->sp - 1].as.ptr.block = gp;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;
	return tnext(t);
}



//compiles a shader, if necessary, and sets all the attributes and uniforms
//there is a lot of potential for optimization here
//especially:
//	a) gfx_set_input could skip values that aren't 'dirty', if multiple draws with the same data and shader are made in a row
//  b) uniforms can be moved in to a UBO and updated in a batch
graphics_pipelineT*  last_prep_pipe = NULL;

tokenT* h_prepshader(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	graphics_pipelineT* sg = (void*)(ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset);
	last_prep_pipe = sg;

	int argcount = zvec_count(sg->shader_inputs);
	int i;
	int mask = 0; //specifies which args are null..  need to scan the stack and set, for now require all args present
	
	if (!sg->shader) {
		//need to compie the shader
		sg->shader = gx_compile_shader(sg->vsource, sg->fsource, sg->shader_inputs, mask);
	}
	gx_use_shader(sg->shader);


	//time to populate all the values
	for (i = 0; i < zvec_count(sg->shader_inputs); i++) {
		gfx_shader_inputT* si = zvec_get_at(sg->shader_inputs, i);
		void* data = ex->stack[ex->sp - 1 - argcount + i].as.ptr.block  + ex->stack[ex->sp - 1 - argcount + i].as.ptr.offset;

		if (si->uniform && (gfx_sizeof(si->type) <= 16) &&  ! (si->type&GFX_ARRAY) && si->type !=GFX_TEXTURE  )  //stack value directly, for small uniforms
			gfx_set_input(si, (void*)(ex->stack + ex->sp - 1 - argcount + i));
		else
			gfx_set_input(si, data);	//pointer for large uniforms (matrix, array) or vbos, textures

	}

	//all inputs are set; ready to draw

	return tnext(t);
}



tokenT* h_execdraw(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	//this doesn't actually use the sg on the stack, it just takes it off that stack.
	//an easy way to enforce that execdraw follows setting up a shader
	graphics_pipelineT* sg = (void*)(ex->stack[ex->sp - 4].as.ptr.block + ex->stack[ex->sp - 4].as.ptr.offset);

	
	zuint32 prim = ex->stack[ex->sp - 3].as.n32;
	zuint32 start = ex->stack[ex->sp - 2].as.n32;
	zuint32 stop = ex->stack[ex->sp - 1].as.n32;

	if (sg != last_prep_pipe) {
		printf(" invalid draw\n");
	}
	else {
		gfx_draw(prim, ZFALSE, start, stop);
	}
	
	ex->sp -= 4;

	return tnext(t);
}


//when run in an immediate context, returns the token that follows the immediate block
tokenT* h_heretoken(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	//this doesn't actually use the sg on the stack, it just takes it off that stack.
	//an easy way to enforce that execdraw follows setting up a shader
	if (!ex->in_immediate) {
		ERR("Cannot 'here' unless running in an immediate context\n");
	}

	tokenT* here = ex->in_immediate->t;

	ex->stack[ex->sp].as.ptr.block = here;
	ex->stack[ex->sp].as.ptr.offset = 0;

	ex->sp++;
			
	return tnext(t);
}

//returns the string representation of a token
tokenT* h_tokenstring(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	//this doesn't actually use the sg on the stack, it just takes it off that stack.
	//an easy way to enforce that execdraw follows setting up a shader
	
	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.block;

	ex->stack[ex->sp - 1].as.ptr.block = ts->str;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}

//returns the next token  like token=next(token)
tokenT* h_tokennext(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

		
	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.block;
	
	if (ts)
		ts = tnext(ts);

	ex->stack[ex->sp - 1].as.ptr.block = ts;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}

tokenT* h_tokenclip(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	if (!ex->in_immediate) {
		ERR("Cannot 'here' unless running in an immediate context\n");
	}

	tokenT* here = ex->in_immediate->t;
	//clip takes from 'here' to passed in position (inclusive) and returns it as a Code%
	//That 
	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.block;

	if (!ts) 
		ERR("cannot clip to null\n");

	tokenT* tcode = mkToken(KCODE, "Hcode", 5);
	tcode->ty = findType(POINTERPOSSESSIVE, tCode, NULL, 0);
	insert_after(ts, tcode);
	fold(here, tcode);

	ex->in_immediate->t = tnext(tcode);

	tremove(tcode);

	ex->stack[ex->sp - 1].as.ptr.block = tcode;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}




tokenT* h_codecat(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	

	tokenT* first = ex->stack[ex->sp - 2].as.ptr.block;
	tokenT* additional = ex->stack[ex->sp-1].as.ptr.block;
	ex->sp-=2;
	
	if (!additional)	//nothing to add, just return what was already there	
		return tnext(t);
	
	
	if (!first){  //no first one, so just return the additional one.  If that's null, whatever, 
		ERR("Can't append to Null code pointer\n");
	}
	
	
	tokenT* ts;
	while (ts = tsub(additional)){
		tremove(ts); //remove from the additional
		zlist_addtail(&first->subs, ts);
	}
	ram_free(additional);
	
	return tnext(t);
}
