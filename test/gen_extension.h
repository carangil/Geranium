int plus5(int a);
tokenT* hc_plus5 (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-1].as.z32 =
		plus5(
			ex->stack[ex->sp-1].as.z32);
	ex->sp+= (-1+1);

	return tnext(t); 
}
char* hello();
tokenT* hc_hello (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-0].as.ptr.block= hello(
			);	ex->stack[ex->sp-0].as.ptr.level=0; 
	ex->stack[ex->sp-0].as.ptr.offset=0; 

	ex->sp+= (-0+1);

	return tnext(t); 
}
int ha();
tokenT* hc_ha (exectxT* ex, tokenT* t) {	
	exe(ex, tsub(t));			
	ex->stack[ex->sp-0].as.z32 =
		ha(
			);
	ex->sp+= (-0+1);

	return tnext(t); 
}
#define SET_EXTENSIONS set_handlers
void set_handlers(){
	mkSymbol(global, "C_plus5", tPrimitive, hc_plus5);
	mkSymbol(global, "C_hello", tPrimitive, hc_hello);
	mkSymbol(global, "C_ha", tPrimitive, hc_ha);
}
