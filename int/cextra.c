#include "zwindow.h"
#include "gfx_gl.h"
#include "glsl.h"
#include "int.h"


void store16(int val, zuint16* zp) {
	*zp = val;
}

zint32 load16(zuint16* z) {
	return *z;
}





tokenT* h_vec3add(exectxT* ex, tokenT* t) {


	
	vec3add(    
		*(vec3*)&(ex->stack[ex->sp - 2]),
		*(vec3*)&(ex->stack[ex->sp - 1])
	);

	ex->sp -= 1;

	return tnext(t);
}


tokenT* h_vec3sub(exectxT* ex, tokenT* t) {

	
	vec3sub(
		*(vec3*)&(ex->stack[ex->sp - 2]),
		*(vec3*)&(ex->stack[ex->sp - 1])
	);

	ex->sp -= 1;

	return tnext(t);
}


tokenT* h_vec3cross(exectxT* ex, tokenT* t) {

	
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

typedef struct glsl_call_wrapperS {
	gx_shadergroupT* sg;
	zbool indirect[64];  //one for each input, true means value on Z stack is a pointer.  false means the value is on the Z stack
} glsl_call_wrapperT;

zbool freecw(void* v) {
	glsl_call_wrapperT* cw = v;
	ram_free(cw->sg);
	return ZTRUE;
}

tokenT* h_glslprocbody(exectxT* ex, tokenT* t) {

	typeT* v3 = (OFTYPE(TYPE("Vec3"), ARRAYDYNAMIC));
	typeT* v4 = (OFTYPE(TYPE("Vec4"), ARRAYDYNAMIC));
	typeT* v2 = (OFTYPE(TYPE("Vec2"), ARRAYDYNAMIC));
	typeT* v1 = (OFTYPE(TYPE("Vec1"), ARRAYDYNAMIC));

	typeT* m33 = (OFTYPE(TYPE("Matrix33"), ARRAYDYNAMIC));
	typeT* m44 = (OFTYPE(TYPE("Matrix44"), ARRAYDYNAMIC));

	
	
	char* vsource = (ex->stack[ex->sp - 3].as.ptr.addr.bytes + ex->stack[ex->sp - 3].as.ptr.offset);
	char* fsource = (ex->stack[ex->sp - 2].as.ptr.addr.bytes + ex->stack[ex->sp - 2].as.ptr.offset);
	symbolT* sym =  (ex->stack[ex->sp - 1].as.ptr.addr.bytes + ex->stack[ex->sp - 1].as.ptr.offset);

	glsl_call_wrapperT* cw = ram_alloc(sizeof(glsl_call_wrapperT), freecw);
	gx_shadergroupT* sg = gx_shader_source(vsource, fsource);
	cw->sg = sg;


	

		//typeT* ftype = ex->in_immediate->parent->type;
	typeT* ftype = sym->type;
	if (ftype && ftype->category == FUNCTION) {
		int i = 0;
		typeT* arg = NULL;
		while (arg = type_member(ftype, i)) {
			int sitype = 0;

			typeT* rt;

			if (arg->ref->category == POINTERUSER) {
				cw->indirect[i] = ZTRUE;
				rt = arg->ref->ref;
			}
			else {
				rt = arg->ref; 
			}
			
			//array types: for now, count is 1 because 
			if (rt == v1)
				sitype = GFX_FLOAT | GFX_ARRAY;
			else if (rt == v2)
				sitype = GFX_FLOAT2 | GFX_ARRAY;
			else if (rt == v3)
				sitype = GFX_FLOAT3 | GFX_ARRAY;
			else if (rt == v4)
				sitype = GFX_FLOAT4 | GFX_ARRAY;
			else if (rt == TYPE("Real"))
				sitype = GFX_FLOAT;
			else if (rt == TYPE("Vec2"))
				sitype = GFX_FLOAT2;
			else if (rt == TYPE("Vec3"))
				sitype = GFX_FLOAT3;
			else if (rt == TYPE("Vec4"))
				sitype = GFX_FLOAT4;
			else if (rt == (TYPE("Matrix33") ))
				sitype = GFX_MAT33;
			else if (rt == (TYPE("Matrix44") ))
				sitype = GFX_MAT44;
			else if (rt == (TYPE("Transform") ))
				sitype = GFX_MAT44;
			else if (rt == (TYPE("Texture") ))
				sitype = GFX_TEXTURE;
			else
				printf("Unknown input type to shader:%s\n", arg->ref->name);

			if (arg->isPer) {
				gfx_shader_add_input(sg, arg->name, sitype, ZFALSE); //is attribute
				printf("attr ");
			} else {
				gfx_shader_add_input(sg, arg->name, sitype, ZTRUE); //is uniform
				printf("unfm ");
			}

			if (cw->indirect[i])
				printf(" indirect \n");

			printTypeNoRedirect(arg, ZTRUE, ZFALSE);
			i++;
		}

	}

	

	ex->sp-=2;
	
	ex->stack[ex->sp - 1].as.ptr.addr.bytes = cw;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;
	return tnext(t);
}



//compiles a shader, if necessary, and sets all the attributes and uniforms
//there is a lot of potential for optimization here
//especially:
//	a) gfx_set_input could skip values that aren't 'dirty', if multiple draws with the same data and shader are made in a row
//  b) uniforms can be moved in to a UBO and updated in a batch
//graphics_pipelineT*  last_prep_pipe = NULL;
glsl_call_wrapperT* last_prep_pipe = NULL;

tokenT* h_prepshader(exectxT* ex, tokenT* t) {

	glsl_call_wrapperT* cw = (void*)(ex->stack[ex->sp - 1].as.ptr.addr.bytes);
	
	gx_shadergroupT* sg = cw->sg;
		
	last_prep_pipe = cw;

	int argcount = zvec_count(sg->shader_inputs);
	int i;
	zuint64 mask = 0; 
	zuint64 bit = 1;
	//check for missing values
		//time to populate all the values

	for (i = 0; i < zvec_count(sg->shader_inputs); i++) {
 		void* data = ex->stack[ex->sp - 1 - argcount + i].as.ptr.addr.bytes + ex->stack[ex->sp - 1 - argcount + i].as.ptr.offset;
		if (data || ! cw->indirect[i] )
			mask |= bit;					//set if the pointer to data is non-null OR we are using stack data (!indirect)
		bit <<= 1;
	}

	
	gx_shader_variantT* variant = gx_shader_variant2(sg, mask);
		
	gx_use_shader(variant);

	//time to populate all the values
	for (i = 0; i < zvec_count(sg->shader_inputs); i++) {
		gfx_shader_inputT* si = zvec_get_at(sg->shader_inputs, i);
		void* data = ex->stack[ex->sp - 1 - argcount + i].as.ptr.addr.bytes  + ex->stack[ex->sp - 1 - argcount + i].as.ptr.offset;

		if (!data)
			continue; //skip
		
		if (si->uniform && !cw->indirect[i])	//item on stack
			gfx_set_input(si, (void*)(ex->stack + ex->sp - 1 - argcount + i));
		else
			gfx_set_input(si, data);	//pointer to item is on stack (for vbo attributes, large uniforms, optional uniforms)

		

	/*	
		if (si->uniform && (gfx_sizeof(si->type) <= 16) &&  ! (si->type&GFX_ARRAY) && si->type !=GFX_TEXTURE  )  //stack value directly, for small uniforms
			gfx_set_input(si, (void*)(ex->stack + ex->sp - 1 - argcount + i));
		else
			gfx_set_input(si, data);	//pointer for large uniforms (matrix, array) or vbos, textures
			*/

	}
	
	//all inputs are set; ready to draw

	
	ex->sp -= zvec_count(sg->shader_inputs);

	//put prepared shader object on stack
	ex->stack[ex->sp-1 ].as.ptr.addr.bytes = cw;
	ex->stack[ex->sp-1 ].as.ptr.offset = 0;
	

	return tnext(t);
}


tokenT* h_execdraw(exectxT* ex, tokenT* t) {

	
	//this doesn't actually use the sg on the stack, it just takes it off that stack.
	//an easy way to enforce that execdraw follows setting up a shader
	//gx_shadergroupT* sg = (void*)(ex->stack[ex->sp - 4].as.ptr.addr.bytes + ex->stack[ex->sp - 4].as.ptr.offset);
	
	glsl_call_wrapperT* cw = (void*)(ex->stack[ex->sp - 4].as.ptr.addr.bytes);

	gx_shadergroupT* sg = cw->sg;

	
	zuint32 prim = ex->stack[ex->sp - 3].as.n32;
	zuint32 start = ex->stack[ex->sp - 2].as.n32;
	zuint32 stop = ex->stack[ex->sp - 1].as.n32;

	if (cw != last_prep_pipe) {
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


	//this doesn't actually use the sg on the stack, it just takes it off that stack.
	//an easy way to enforce that execdraw follows setting up a shader
	if (!ex->in_immediate) {
		ERR("Cannot 'here' unless running in an immediate context\n");
	}

	tokenT* here = ex->in_immediate->t;

	ex->stack[ex->sp].as.ptr.addr.bytes = here;
	ex->stack[ex->sp].as.ptr.offset = 0;

	ex->sp++;
			
	return tnext(t);
}


tokenT* followRedirect(tokenT* there){
	
	while (there && there->handler == hredirectsub) {
		if (tsub(there))
			ERR("unexpected child\n");
		there = there->val.as.ptr.addr.token;
		there = tsub(there);
	}
	return there;
	
}

tokenT* h_firsttoken(exectxT* ex, tokenT* t) {

	tokenT* there = ex->stack[ex->sp - 1].as.ptr.addr.bytes;// +ex->stack[ex->sp - 1].as.ptr.offset;
	
	//if 'there' is a redirect, we need to get to the real list of args

	there = followRedirect(there);

	if (there)
		there = tsub(there); //get first sub (ignoring this might be a redirect)

	
	ex->stack[ex->sp - 1].as.ptr.addr.bytes = there;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}


//not sure if this one is needed.  it was to address a bug that I think is now otherwise solved.
tokenT* h_hasfirsttoken(exectxT* ex, tokenT* t) {

	tokenT* there = ex->stack[ex->sp - 1].as.ptr.addr.bytes;// +ex->stack[ex->sp - 1].as.ptr.offset;

	//ignore that 'there' might be a redirect.
	//if we follow it, 'next' won't work
	// .text, .prim, .type, .valtype etc will all have to do the redirect instead

	there = followRedirect(there);

	if (there)
		there = tsub(there); //get first sub


	if (there) {
		ex->stack[ex->sp - 1].as.n32=1;
	}
	else {
		ex->stack[ex->sp - 1].as.n32 = 0;
	}

	return tnext(t);
}


//returns the string representation of a token
tokenT* h_tokenstring(exectxT* ex, tokenT* t) {
	
	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.addr.bytes;
	if (!ts)
		ERR("no token\n");

	ts = followRedirect(ts);
	
	ex->stack[ex->sp - 1].as.ptr.addr.bytes = ts->str;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}






typeT* tvaltype(tokenT* t){
	

	t = followRedirect(t);

	if (t->handler == hconstant || t->handler == hconstantaddref){
		//if this was a constant instruction, the constant is val, so is the same type as the runtime value
		return t->ty;
	} 
		
	return t->tyval;
}



//returns the type embedded in a token (NOT the token's runtime type, but of ty->val)
tokenT* h_tokenvaltype(exectxT* ex, tokenT* t) {
	
	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.addr.bytes + ex->stack[ex->sp - 1].as.ptr.offset;

	ts = followRedirect(ts);

	ex->stack[ex->sp - 1].as.ptr.addr.bytes = tvaltype(ts);
	ex->stack[ex->sp - 1].as.ptr.offset = 0;
	
	return tnext(t);
}

//returns the type the token returns at runtime
tokenT* h_tokenevaltype(exectxT* ex, tokenT* t) {

	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.addr.bytes + ex->stack[ex->sp - 1].as.ptr.offset;


	ts = followRedirect(ts);

	ex->stack[ex->sp - 1].as.ptr.addr.bytes = ts->ty;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}


//returns the token's symbol 
tokenT* h_tokensymbol(exectxT* ex, tokenT* t) {

	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.addr.bytes + ex->stack[ex->sp - 1].as.ptr.offset;
	

	ts = followRedirect(ts);
	
	
	if (ts)
		ex->stack[ex->sp - 1].as.ptr.addr.bytes = ts->sym;
	else
		ex->stack[ex->sp - 1].as.ptr.addr.bytes = NULL;

	return tnext(t);
}

tokenT* h_symboltype(exectxT* ex, tokenT* t) {
	symbolT* s = ex->stack[ex->sp - 1].as.ptr.addr.bytes + ex->stack[ex->sp - 1].as.ptr.offset;
	
	
	if (s)
		ex->stack[ex->sp - 1].as.ptr.addr.bytes = s->type;
	else
		ex->stack[ex->sp - 1].as.ptr.addr.bytes = NULL;

	return tnext(t);
}



//returns the next token  like token=next(token)
tokenT* h_tokennext(exectxT* ex, tokenT* t) {

	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.addr.bytes;
	
	if (ts)
		ts = tnext(ts);
	
	ex->stack[ex->sp - 1].as.ptr.addr.bytes = ts;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}

tokenT* h_tokenclip(exectxT* ex, tokenT* t) {

	if (!ex->in_immediate) {
		ERR("Cannot 'here' unless running in an immediate context\n");
	}

	tokenT* here = ex->in_immediate->t;
	//clip takes from 'here' to passed in position (inclusive) and returns it as a Code%
	//That 
	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.addr.bytes;

	if (!ts) 
		ERR("cannot clip to null\n");

	tokenT* tcode = mkToken(KCODE, "Hcode", 5);
	tcode->ty = findType(POINTERPOSSESSIVE, tCode, NULL, 0);
	insert_after(ts, tcode);
	fold(here, tcode);

	ex->in_immediate->t = tnext(tcode);

	tremove(tcode);

	ex->stack[ex->sp - 1].as.ptr.addr.bytes = tcode;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}


tokenT* h_tokeninsert(exectxT* ex, tokenT* t) {

	tokenT* additional = ex->stack[ex->sp - 1].as.ptr.addr.bytes;
	ex->sp -= 1;

	if (!additional)	//nothing to add, just return what was already there	
		return tnext(t);

	tokenT* here = ex->in_immediate->t;
	
	//take tokens from additional and add to before 'here'
	//the first token here becomes the new 'here'

	tokenT* ts = NULL; 
	tokenT* first = tsub(additional);
	while (ts = tsub(additional)) {
		tremove(ts); //remove from the additional
		zlist_insert_node_before(here, ts);
	}
	ram_free(additional);
	ex->in_immediate->t = first;
	return tnext(t);
}


tokenT* h_codecat(exectxT* ex, tokenT* t) {


	tokenT* first = (tokenT*)ex->stack[ex->sp - 2].as.ptr.addr.bytes;
	tokenT* additional = (tokenT*) ex->stack[ex->sp-1].as.ptr.addr.bytes;
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


//if token is to execute a primitive, return the name of the primitive 
tokenT* h_tokenprim(exectxT* ex, tokenT* t) {
	char* name = "Unknown";

	tokenT* ts = (tokenT*) ex->stack[ex->sp - 1].as.ptr.addr.bytes;
	

	ts = followRedirect(ts);

	if (ts && ts->sym && ts->sym->primsym) {
		name = ts->sym->primsym->name;
	}
	else if (ts)
		name = findSymbolByHandler(ts->handler);
	
	
	ex->stack[ex->sp - 1].as.ptr.addr.bytes = name;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}


