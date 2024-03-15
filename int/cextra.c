


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

tokenT* h_fillvbowrapper(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	float tmp;

	char* vbowrapper = (ex->stack[ex->sp - 2].as.ptr.block + ex->stack[ex->sp - 2].as.ptr.offset);
	typeT* formattype = ex->stack[ex->sp - 2].typeselector->parent;

	zuint16 count = ex->stack[ex->sp - 1].as.n32;

	char* spec = zstr_mk(64);

	typeT* v3 = OFTYPE(OFTYPE(TYPE("Vec3"), ARRAYDYNAMIC), POINTERUSER);
	typeT* vbuffer = OFTYPE(TYPE("VBuffer"), POINTERPOSSESSIVE);

	typeT* v4 = OFTYPE(OFTYPE(TYPE("Vec4"), ARRAYDYNAMIC), POINTERUSER);
	typeT* v2 = OFTYPE(OFTYPE(TYPE("Vec2"), ARRAYDYNAMIC), POINTERUSER);

	int i;
		
	gfx_vertex_bufferT** vb = NULL;

	int array_offsets[8];
	int num_arrays = 0;

	for (i = 0; i < zvec_count(formattype->members);i++) {
		typeT* t = zvec_get_at(formattype->members,i);
		int size=0;

		if (t->ref == v3)
			size = 3;
		else if (t->ref == v2)
			size = 2;
		else if (t->ref == v4)
			size = 4;
		else if (t->ref == vbuffer)
			vb = (void*)(vbowrapper + t->offset);

		if (size) {

			if (i != 0)
				spec = zstrcat(spec, "|");

			spec = zstrprintf(spec, "%s:%d", t->name, size);
			array_offsets[num_arrays++] = t->offset;
		}

	}

	printf("--%s\n", spec);
	if (!vb) {
		ERR("Trying to create VBO in non-VBO object\n");
	}
	
	*vb = gfx_vertex_buffer_mk(count, spec);

	for (i = 0; i < (*vb)->num_attributes; i++) {
		vptrT* pdata = vbowrapper + array_offsets[i];
		pdata->block = (*vb)->data[i];	//todo: use the combined vbo data pointer as block, and use the offsets for here?
		pdata->offset = 0;
	}

	ex->sp--;

	return tnext(t);
}
