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