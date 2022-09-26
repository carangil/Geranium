#include "zmem.h"
#include "zarray.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"
#include "zvector.h"

#define ERR( ...) { fprintf(stderr,__VA_ARGS__);  exit(1);}
//#define EXEDEBUG

/**** Basic Values ****/

//Pointers
typedef struct vptrS{	
	char* block;
	zuint32 offset;
}vptrT;

typedef union valu {	//Generic value (datatype is tracked through other means)
		zint32 z32;
		zint32 n32;
		vptrT ptr;
	} valU;
	
typedef struct valueS{
	valU as;
}valueT;


/**** Execution Context ****/
typedef struct exectxS{
	valueT* stack;
	zuint32	sp;
	zuint32 fp;
	char* vars; 
	char* globalvars; 
	int stop;
}exectxT;
#define STOPFUNC 1
#define STOPLOOP 2

typedef struct tokenS* (*instruction) (exectxT*,struct tokenS* ) ;

/* Parse Context */
typedef struct parsectxS{
	zvecT* symbols;
	zuint32	size;	//size of variables in this table
	struct typeS* type;  //if in a procedure, we need to know about its return type and args
	int endable;
}parsectxT;



/**** Tokenizer ****/

/* Program is a linked list of tokens*/
typedef struct tokenS{
	zlistnodeT zlistnode;
	zuint32 tok;	//A constant defined below, a character, or a pair of characters
	char* str;	//string representation of this token
	struct typeS* ty;	//type of this token
	valueT val;
	zlistT subs;	//make a tree out of token list
	instruction handler; 
	struct symbolS* sym;  //for things like procs that have a bunch of context info
}tokenT;


/* Macros to make some things easier
 * tremove: removes a token from the list (and returns it as a pointer to be assigned somewhere else or freed.  To remove requires a token is in front of or behind it
 * tnext/tprev: Pointer to the next token
 * insert_after: Inserts a token after another token.  Requires the place of insertion is not the head or tail of the list
 * */

#define tremove(ITEM)    zlist_remove_mid(  &(ITEM)->zlistnode)
#define tnext(ITEM) ((tokenT*)(ITEM)->zlistnode.next)
#define tprev(ITEM)    ((ITEM)?((tokenT*)(ITEM)->zlistnode.prev):NULL)
#define insert_after(AFTER,NEW)    zlist_insert_node_after(  &(AFTER)->zlistnode,  &(NEW)->zlistnode);



//tokenT->tok values:
#define PAIR(B1,B2)	((((unsigned int)(B1&0xff)) <<8) | ((unsigned int)(B2&0xff)))
#define NAME		0x200
#define NUMBER		0x300
#define LITERAL 	0x400
#define ENDFILE		0x500
#define PASTENDFILE	0x600
#define STARTFILE	0x700
#define COND		0x800a

//Token values that are also user-accessible keywords:
#define KWORDS		0x8000
#define KVAR		0x8000
#define KTYPE		0x8001
#define KEND		0x8002
#define KPRIMITIVE	0x8003
#define KPROC		0x8004
#define KRETURN		0x8005
#define KIF		0x8006
#define KELSE		0x8007
#define KELSEIF		0x8008
#define KLOOP		0x8009
#define KBREAK		0x800a

char*  keywords[] = {"var", "type", "end", "primitive", "proc","return", "if", "else", "elseif", "loop", "break", NULL};

zuint32 findKeyword(char* c){
	if (c)
		for (int i=0;keywords[i];i++) 
			if ( !strcmp(keywords[i], c))
				return KWORDS+i;
	return 0;
}

zbool token_cleanup(void* v){
	tokenT* t = v;
	ram_free(t->str);
	zlist_cleanup(&t->subs);
	return ZTRUE;
}

tokenT* mkToken(zuint32 tok, char* str, zuint32 len){
	tokenT* t = ram_alloc( sizeof(tokenT) , token_cleanup );
	t->tok = tok;
	if (str && len)
		t->str = zstrndup(str, len);
	else if (str)
		t->str = zstrndup(str, ZSTRING_ALL);
	if ((t->tok == NAME) && t->str) {
		zuint32 b  = findKeyword(t->str);
		if (b)	//replace NAME tokens that match a keyword with a keyword token
			t->tok = b;
	}
	return t;
}


char* safestr(char* s){
	return s ? s:""; 
}

/* TOKENIZER*/
/* This first part parses an input string and returns a linked list of tokens
 * Very little is verified, things are broadly classified as pairs (such as ->, --, etc),
 * as single character tokens (+,-, etc), names (alpha_numeric_123), numbers (1.0E-4, 0x100, etc)
 * literals  "boo", whitespace, etc.
 * As whether a particiar token such as '-' is really on operator or part of a number it's preceding, that's figured later
 * Things like 0x10.4E-4 is a 'NUMBER', but will later fail as it isn't a valid form
 */

//used to recognize 2-letter combinations like ->, etc
zuint32 findPair(char* patterns, char a, char b){
	for(  ;*patterns;patterns+=2){
		if ( ((*patterns)==a) &&(*(patterns+1)==b))
			return PAIR(a,b) ;
	}
	return 0;
}

//used to either recognize names or numbers. Primitive, not regex-fancy or anything
int acceptPatterns(char* s, char* startChars, char* continuePairs,  char* continueChars){
	int i=0;
	char* st =s;
	zuint32 p=0;
	
	if (strchr(startChars, *(s++))){	//if input string 's' begins with any of the start chars
		i++; //advance to next char
		for(;*s;i++,s++){  //continue on
			if (p=findPair(continuePairs,*s,*(s+1))){ //accept any pairs of characters
				s++,i++; //loop does s++, i++ automatically; we need +=2, so increment here as well (to jump over whole pair)
				continue;
			}
			if (strchr(continueChars, (*s))){ //accept any of the continue characters
				continue;
			}
			break;
		}
	}
	
	return i; //return number of characters the pattern accepted
}

//scans through string literals between two 'start' characters... commonly 'start' is ' or "
int acceptLiteral(char* in, char start, char escape){

	if (*(in++) != start)
		return 0;

	int i=1;

	while(*in){
		if (*in==escape){
			in+=2;
			i+=2;
			continue;
		}
		if (*in==start){
			return i+1;
		}
		in++;
		i++;
	}
	return 0;
}

zlistT* tokenize(zlistT* list, char* in){

	int c,next;
	tokenT* t=NULL;
	char small[3];
	int i;

	if (!list) {
		list = ram_alloc(sizeof(zlistT), (ram_destructor) zlist_cleanup);
		t = mkToken(STARTFILE, NULL,0);
		zlist_addhead(list,&t->zlistnode);
		t->zlistnode.prev=&t->zlistnode;  //'trap' so ->prev->prev is always safe
	}

	while (c = *in){
		next = *(in+1);
		zuint32 p;

		//find twochar patterns like ->,etc. including comment start/end markers
		if (p=findPair("<<>>--++->==||&&/**///[]", c, next)){
			if (  p == PAIR('/','/')  ) { //special handling for // comments
				while(*in!= '\n')
					in++;
				continue;
			}
	
			t = mkToken( PAIR(c, next) , in, 2);
			zlist_addtail(list, &t->zlistnode);
			in+=2;
			continue;
		}

		//check for string literals
		int lit = acceptLiteral(in, '\'' , '\\');  //single quote
		if (!lit)
			lit = acceptLiteral(in, '"' , '\\' ); //double quote

		if (lit) {
			t = mkToken(LITERAL, in, lit);
			zlist_addtail(list, &t->zlistnode);
			in+=lit;
			continue;
		}

		//collapse spaces and tabs together
		int space = acceptPatterns(in, " \t\n\r", "", " \t\n\r");

		if (space) {
			in+= space;
			//below will turn whitespace into a token.  Currently, whitespace is ignored, so just skip the token
#if 0			
			t = mkToken(' ');
			t->str=zstrdup(" ");
			zlist_addtail(list, &t->zlistnode);
#endif
			continue;
		}
		
		//names can start with a dot(struct member reference)
		int name = acceptPatterns(in, 
				".abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",
				"",
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
		int digits  = 0;
		
		if (!name){
			digits  = acceptPatterns(in,
					"-.0123456789", //start with digit or decimal point
					"e-E-e+E+",  //- and + only accepted after an e or E
					"0123456789abcde.fABCDEFxlLuUfF"); //ontinues with figits, deccimal point, hex letters, type suffix letters
			
			//special case: if number starts with '-', but has only 1 character, this isn't a negative number, but just a minus sign
			if ((digits == 1) && (in[0] == '-'))
				digits = 0;
		}
		
		if (digits || name){
			t = mkToken( digits? NUMBER : NAME, in ,   digits|name);
			in += digits|name;
			zlist_addtail(list, &t->zlistnode);
			continue;
		}

		//just some char
		t = mkToken( *in, in, 1);
		zlist_addtail(list, &t->zlistnode);
		in++;
	}
	
	//something should stop reading at endfile
	tokenT* end = mkToken(ENDFILE, "ENDFILE", 0);
	zlist_addtail(list, &end->zlistnode);

	//if program ever advances reads PASTENDFILE, its an error
	//this is a trap so 'next->next->next' always is safe
	end = mkToken(PASTENDFILE, "PASTENDFILE", 0);
	zlist_addtail(list, &end->zlistnode);
	//end->zlistnode.next = (zlistnodeT*) end;

	return list;
}

/**** Data Types ****/

/* Simple type system*/
#define SIMPLE	1
#define POINTER 2
#define STRUCT 	3
#define ARRAY 	4
#define FUNCTION 5
#define PRIMITIVE 6
#define LAST_REAL_TYPE 7

//MEMBER is not a type, but is used to mark members of a struct
#define MEMBER	10
//NAMED is not a type, but when passed into findType looks for struct or simple  w/out knowing which it is yet
#define NAMED	11
//PENDING not a type, but is for when a type is mentioned in another declaration but not yet defined
#define PENDING 12

typedef struct typeS{
	char* name;
	size_t size;		//for structs
	zuint32 category;	//SIMPLE, POINTER, etc
	zuint32 len; //for definite arrays, 0 for indefinite arrays
	zuint32 offset; //for struct members
	struct typeS* ref; //array or ointer types, or function return type
	zvecT* members;  //structs or function parameters
	int tid;
	zbool pending;
}typeT;

zvecT* types;
int tid=0;

zbool type_cleanup(void* v){
	typeT* ty = v;
	ram_free(ty->name);
	ram_free(ty->members); //free the container (the contents are already disowned and freed elsewhere)
	return ZTRUE;
}

typeT* mkType(zuint32 category, typeT* ref, char* name, size_t szlen){
	typeT* ty= ram_alloc(sizeof(typeT), type_cleanup); //todo: destructor
	ty->tid = tid++;
	ty->category = category;
	ty->name = zstrdup(name);
	ty->ref = ref;

	if (category == ARRAY){
		ty->len = szlen;
		ty->size = szlen * ref->size;
	}
	else if (category == MEMBER){
		ty->offset = szlen;
	} else {
		ty->size = szlen;
	}

	//pointer or indefinite array
	if ((category == POINTER)|| (category == ARRAY) && (szlen == 0)){
		if (ty->size)
			ERR("Cannot specify size of pointer or indefinite array (it is automatically calculated)\n");
		ty->size = sizeof (vptrT);
	}

	if (types == NULL)
		types = zvec_mk(NULL, 100);

	zvec_add(types, ty);

	return ty;
}

void printType(typeT* ty, zbool line, zbool skipmembers){
	char* end = "";
	
	if (!ty)
		printf("{nulltype}");
	
	if (ty){
		printf("#%d#", ty->tid);
		if (!skipmembers)
			printf("<size%d>", ty->size);
		
		switch(ty->category) {
		case  PRIMITIVE:
			printf("<PRIMITIVE>");  //continue on as func
		case  FUNCTION:
			printf("<func>(");
			end=")";
			skipmembers=ZFALSE;
			break;
		
		case ARRAY:
			printf("[%d ", ty->len);
			end="]";
			break;
			
		case POINTER:
			end="*"; //fallthru
			break;
			
		case PENDING:
			printf("<pending>");
			break;
			
		case STRUCT:
			if (!skipmembers){
				printf("type ");
				end=" end";
			}
		case MEMBER:
			printf("  ");
		case SIMPLE:
			break;
		default:
			printf(" Unknown printType category %d\n", ty->category);
		}
		
		
		if (ty->name)
			printf("%s%c", ty->name, ty->category == MEMBER? ':':' ');
		
		if (!skipmembers && ty->members){
			int i;
			for (i=0;i<zvec_count(ty->members);i++){
				printType( zvec_get_at(ty->members,i), ZFALSE, ZTRUE);
				printf("; ");
			}
			
		}
		
		if (ty->category == FUNCTION)
			printf(" >> ");
				
		if (ty->ref)
			printType(ty->ref, ZFALSE, ZTRUE);
		
		printf("%s",end);
	}
	
	if (line)
		printf("\n");
	
}

//compare two types, return true if the same
zbool cmpType(zuint32 category, typeT* ref, char* name, size_t len, typeT* ty){

	if (!ty)
		ERR("compare null type\n");

	if (category == MEMBER)
		ERR("struct members shouldn't be compared\n");

	if (ty->category != category)  //early exit for categories that should match
		return ZFALSE;

	if ((category == SIMPLE)||(category==STRUCT)){
		//check named types
		if (strcmp(ty->name, name))
			return ZFALSE;
	}

	if ((ty->category == ARRAY) &&(ty->len != len)) //array of different sizes
		return ZFALSE;

	//check reference same type  (findType should not be returning equivalent duplicates)

	if ( (category ==  FUNCTION)&&(ref)&& (ref->category==FUNCTION)) {

		//ty shuld contain a FUNCTION type, with ref and members
		//ref should contain a FUNCTION type with the same ref and members

		if (ty->ref != ref->ref){
			//printf(" functions return different types\n");
			return ZFALSE;
		}

		if (zvec_count(ty->members) !=zvec_count(ref->members)){
		//	printf(" function has different num of arguments\n");
			return ZFALSE;
		}

		int i;
		for (i=0;i<zvec_count(ty->members);i++){
			typeT* memberty = zvec_get_at(ty->members,i);
			typeT* memberref = zvec_get_at(ref->members,i);
			//compare types of members
			if (memberty->ref != memberref->ref){
			//	printf("arg %d to function is of different type\n", i);
				return ZFALSE;
			}
		}
		return ZTRUE; //function type is the same
	}

	//check array or pointer ref value
	if (ty->ref != ref)
		return ZFALSE;

	return ZTRUE;
}

//finds simple or struct types, OR creates composite types (arrays, pointers of existing types) as needed
typeT* findType(zuint32 category, typeT* ref, char* name, size_t len){

	typeT* ty;
	typeT* found=NULL;
	int i;

	for (i=0; i< zvec_count(types);i++){
		ty = zvec_get_at(types, i);

		if (ty->category == MEMBER)
			continue;
		
		if (category != NAMED) {
			//if category is named, we match whether its a struct or int (caller doesn't know which it is yet)

			if (ty->category != category)
				continue;
		}

		switch (category){
			
			case NAMED:  //find structs, simples or pendings by name
			case SIMPLE: //simple types matched by name only
			case STRUCT: 
			case PENDING:
				if (!strcmp(name, ty->name)){
					//found on name
					return ty;
				}
				break; //not it

			case POINTER: 
			case ARRAY:
			case FUNCTION:

				if (cmpType(category, ref, NULL, len, ty))
					return ty;

				break;

			default:
				ERR("unknown type category %d\n", category);
		}
	}

	//did not find.

	if (ref &&((category == ARRAY) || (category == POINTER))) {

		//If array or pointer, find the type 'underneath' and make it

		//printf(" Creating %s type for ", getTypeString(category));
		//printType(ref, ZTRUE, ZFALSE);

		ty = findType(ref->category, ref->ref, ref->name, ref->len);
		if (ty){
			return mkType( category, ty, NULL, len);
		}

	}

	return NULL;
}

typeT* findTypeMember(typeT* type, char* name,  int* pos , int* count){
	int k;
	for (k=0;k<   zvec_count(type->members) ;k++){
				
		typeT* ty= zvec_get_x_at( type->members, typeT*, k);
			if (!strcmp(ty->name, name) ){
				if (pos)
					*pos=k;
				
				if (count)
					*count = zvec_count(type->members);
				return ty;
			}
	}
	return NULL;
}

/*debugging list printer*/
void printList(tokenT* t, tokenT* cur, zuint32 stop_tok, int indent){

	char* iscur;
	int i;
	
	for ( ;t;  t = zlist_next(t)  ){

		if (t==cur)
			iscur="CUR";
		else 
			iscur="   ";
		
		printf("\n");
		for (i=0;i<indent;i++) 
			putc('\t', stdout);
		
		if ( (t->tok >20) && (t->tok < 0x7f))
			printf("{%s %c  ",iscur, t->tok);
		else
			printf("{%s%04x", iscur, t->tok);
		
		
		if (t->str)
			printf("= '%s'", t->str);
		
		
		if (t->ty) 
			//printf("#%d# ",t->ty->tid);
			printType(t->ty, ZFALSE, ZTRUE);

		if (t->handler)
			printf(" handler %p ", t->handler);

		int i;

		if ( zlist_head(&t->subs)){ 
			printList(  zlist_head(&t->subs), cur, stop_tok, indent+1);    
		}
		printf("}");

		if (t->tok == stop_tok)
			break;
		
		if (t->tok == PASTENDFILE)
			break;
	}

}

/**** Symbols ****/

typedef struct symbolS{
	char* name;
	typeT* type;
	zuint32 offset;
	instruction handler;
	tokenT* tokens;
	struct parsectxS* subctx; //procs have their own parsecontext for their local vars
} symbolT;


zbool symbol_cleanup(void* v){
	symbolT* s = v;
	
	ram_free(s->name);
	ram_free(s->subctx);
	ram_free(s->tokens);
	//types are freed elsewhere
	return ZTRUE;
}

zbool cmpTypeListToFunc(zvecT* f, zvecT* b){
	//iterates over a functions list of arguments and compares to a possible list of types
	
	if (zvec_count(f) !=zvec_count(b)){
		//	printf(" function has different num of arguments\n");
			return ZFALSE;
		}

		int i;
		for (i=0;i<zvec_count(f);i++){
			typeT* memberf = zvec_get_at(f,i);
			typeT* memberb = zvec_get_at(b,i);
			//compare types of members
			if (memberf->ref != memberb){
			//	printf("arg %d to function is of different type\n", i);
				return ZFALSE;
			}
		}
		return ZTRUE;
}




symbolT* findSymbol(zvecT* table, char* name, zvecT* typelist){
	
	int i;	
	symbolT* s;
	
	for (i=0;i<zvec_count(table);i++){
		if (!strcmp(name, (s = zvec_get_x_at(table, symbolT*, i))->name)){
						
			if (s->type->category == FUNCTION){
					if (cmpTypeListToFunc(s->type->members, typelist)){
						return s;
					}
			} else {
				//not a function, so just return if we are looking for an obj/var
				if (!typelist || (zvec_count(typelist)==0))
					return s;
			}
										
		}
	}
	return NULL;
}




symbolT* mkSymbol(struct parsectxS* pctx, char* name, typeT* type, instruction handler){
	zvecT* table = pctx->symbols;
	
	symbolT* sym;
		
	if (type->category == FUNCTION){
		//search for function of same name, same inputs
	} else if (findSymbol(table, name, NULL)){
			ERR(" Attempt to redefine %s in same context\n", name);
	}	
	
	if ( (type->category != FUNCTION) && (type->category!= PRIMITIVE)){
		
		if (type->size == 0){
// 			ERR(" type with size 0\n");
		}
	
	}
	
	sym = ram_alloc(sizeof(symbolT), symbol_cleanup);
	sym->name = zstrdup(name);
	sym->type = type;
	sym->handler = handler;
	sym->offset = pctx->size;
	
	pctx->size += type->size; //add context u
	
	printf(" SYMBOL %s at offset %d  , total symbols %d bytes\n", sym->name, sym->offset, pctx->size);
	
	return zvec_add_or_free(table, sym);
}

void printSymbols(zvecT* table , char* label){
	int i;	
	int j;
	printf("\n\nSymbols for %s\n", label);
	for (i=0;i<zvec_count(table);i++){
		symbolT* sym = zvec_get_x_at(table, symbolT*, i);
		printf("#%x\t%s\t", sym->offset, sym->name);
		printType(sym->type, ZTRUE,ZFALSE);
		
		
		
		for (j=0;j<sym->type->size;j++){
			printf("%02x ", * (j+(unsigned char*)(sym+1)));
			if (j>=31)
				{printf("..."); break;}
		}
		printf("\n");
	}
}


/**** Execution ****/

void exe (exectxT* c, struct tokenS* t){
	zuint32 i;
	
	while(t && ! c->stop){  //until out of instructions, or a return is bubbling up
						
		instruction handler = t->handler;
		
		char* str=  safestr(t->str);
#ifdef EXEDEBUG
		printf("%x %s  (pre) handler %p\n",t->tok, safestr(t->str), handler);
#endif
				
		if (!handler)
			ERR("null handler for %c %s\n", t->tok, str);
			
		//getc(stdin);
		t = handler(c,t);
		
#ifdef EXEDEBUG
		printf(">> %s\n",str);
		printf("sp %x|", c->sp);
				
		for (i=0;i<c->sp;i++){
			printf("(%p+%x)/%d ", c->stack[i].as.ptr.block,c->stack[i].as.ptr.offset, c->stack[i].as.z32);
			
		}	
		printf("\n\n");
#endif
	}
}

tokenT* hnop(exectxT* ex, tokenT* t) {	//do nothing
	return tnext(t);
}

tokenT* hconstant (exectxT* ex, tokenT* t) {  //push constant on stack
	ex->stack[(ex->sp)++] = t->val;
	return tnext(t);
}

tokenT* hglobal (exectxT* ex, tokenT* t) {	//push pointer to global variable on stack
 	ex->stack[(ex->sp)].as.ptr.block = ex->globalvars;
	ex->stack[(ex->sp)++].as.ptr.offset = t->val.as.ptr.offset;
	//printf(" Global block %p +%d\n", ex->globalvars  ,   t->val.as.ptr.offset);
	return tnext(t);
}

tokenT* hlocal (exectxT* ex, tokenT* t) {	//push pointer to local variable on stack
	ex->stack[(ex->sp)].as.ptr.block = ex->vars;
	ex->stack[(ex->sp)++].as.ptr.offset = t->val.as.ptr.offset;
	//printf(" local block %p +%d\n", ex->vars  ,   t->val.as.ptr.offset);
	return tnext(t);
}

tokenT* hstackread (exectxT* ex, tokenT* t) {	//read a stack variable (really function parameters)
	//grab from stack, relative to fp
	ex->stack[(ex->sp)++] = ex->stack[ ex->fp + t->val.as.z32 ];
	return tnext(t);
}

#define DEREF(TYPE,BASE,OFFSET)      *((TYPE*)(((char*)(BASE))+(OFFSET)))

#define tsub(TTT)  ((tokenT*)((TTT)->subs.head))

tokenT* hstoreptr (exectxT* ex, tokenT* t) {  //store a pointer
	exe(ex, tsub(t) );
	DEREF(vptrT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = ex->stack[ex->sp-2].as.ptr;
	ex->sp-=2;
	return tnext(t);
}

//tokenT* hcall (exectxT* ex, tokenT* t);
//tokenT* hreturn (exectxT* ex, tokenT* t);
//tokenT* hgroup (exectxT* ex, tokenT* t);

tokenT* hloadptr (exectxT* ex, tokenT* t) { //load a pointer
	
	exe(ex, tsub(t) );
		
	ex->stack[ex->sp-1].as.ptr = DEREF(vptrT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	
	return tnext(t);
}

tokenT* hoffsetptr (exectxT* ex, tokenT* t) { //add constant offset to pointer
	
	exe(ex, tsub(t) );

	ex->stack[ex->sp-1].as.ptr.offset += t->val.as.n32;
	
	return tnext(t);
}

tokenT* hindex(exectxT* ex, tokenT* t) {	//index into array
	exe(ex, tsub(t) );
	
	ex->stack[ex->sp-2].as.ptr.offset +=    ex->stack[ex->sp-1].as.n32   * t->val.as.n32;
	//printf(" index using multiplier %d\n", t->val.as.n32);
	ex->sp--;
	return tnext(t);
}

tokenT* hcondblock (exectxT* ex, tokenT* t){	//if first sub is true, execute the rest of the subs list
	
	//run first instruction
	tokenT* subs =tsub(t);
	
	instruction handler =  subs->handler;
	
	if (!handler)
		ERR(" Null handler on cond's first arg\n");
	
	subs = handler(ex, subs); 
	
	//check result;
	
	ex->sp--;
	
	if ( ex->stack[ex->sp].as.n32){
			//printf (" COND is true, execute body\n");
			exe(ex, subs); //continue this  
			//printf(" EXIT CHAIN\n");
			return NULL;
	} 
	//printf(" COND was false, so call next in chain\n");
	return tnext(t); //next one
		
	exe(ex, tsub(t) ); //evaluate all the args
}

tokenT* hbreakblock (exectxT* ex, tokenT* t) { 
	return NULL;//stop running this block of instructions
}
tokenT* hbreakloop (exectxT* ex, tokenT* t) {
	ex->stop=STOPLOOP; //flag to signal loop breakage
	return NULL;//stop running this block of instructions
}

tokenT* hloop (exectxT* ex, tokenT* t) { //run subs continously until STOPLOOP is set
	
	while(!ex->stop){
		exe(ex, tsub(t) );;
	}
	if(ex->stop== STOPLOOP)
		ex->stop=0;
	
	return tnext(t);
	
}

tokenT* hadd32 (exectxT* ex, tokenT* t) {
	
	exe(ex, tsub(t));
	
	ex->stack[ex->sp-2].as.z32 += ex->stack[ex->sp-1].as.z32;
	ex->sp--;
	
	//printf(" add\n");
	
	return tnext(t);
}



tokenT* hload32 (exectxT* ex, tokenT* t) {
	
	exe(ex, tsub(t));

	zint32 i = DEREF(zint32, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	//printf(" Loaded 32 %d   from +%x\n", i,ex->stack[ex->sp-1].as.ptr.offset );
	ex->stack[ex->sp-1].as.ptr.block=NULL;
	ex->stack[ex->sp-1].as.z32 = i;
	
	
	return tnext(t);
}


tokenT* hstore32 (exectxT* ex, tokenT* t) {
	
	exe(ex,tsub(t));
		
	
	
	DEREF(zint32, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = ex->stack[ex->sp-2].as.z32;
	
	ex->sp-=2;
	
	return tnext(t);
}




tokenT* hreturn (exectxT* ex, tokenT* t){
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->stop=STOPFUNC;  //returning from function
	return NULL; //stop instructions stream (process subs first, as they might be return values or something)
}

tokenT* hgroup (exectxT* ex, tokenT* t){
	exe(ex, tsub(t)); //evaluate all the args
	
	if (t->val.as.z32)
		return tnext(t); //go on to the next step
	//printf("group finished, no continue\n");
	return NULL;
}


tokenT* hcall (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
		
	
	//printf(" CALLING PROC %s\n", t->str);
	
	//printf("pre call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	zuint32 oldfp = ex->fp;  //save frame pointer
	
	ex->fp=ex->sp;
		
	//printf(" enter call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	void* oldlocal = ex->vars;   //take old local var data
	
	
	//printf(" PROC has context size %d\n", t->sym->subctx->size);
	//printList(t->sym->tokens, NULL, 0, 0);
	
	
	ex->vars =  ram_alloc(t->sym->subctx->size , NULL);
	//exe(ex, (tokenT*) t->sym->t->subs.head->next);
	//t->sym->t points to the token describing that function
	//head of that token's sub list is the tokens describing its data type (input parameters, etc)
	//the 'next' of that head is the first statement
	
	exe(ex, (tokenT*)  tnext( tsub(t->sym->tokens))  );
	
	//if (ex->stop==STOPFUNC){
	//	printf(" Caught RET\n");
	//}
	
	ex->stop=0; //stop return bubble-up
	
	//printf(" exit call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	
	//todo:  manipulate frame pointer to get rid of passed values
	
	if (t->sym->type->members)
		ex->fp -=  zvec_count(t->sym->type->members);  //subtract out all passed values
	
	
	if (t->sym->type->ref) {
		ex->stack[ex->fp++] = ex->stack[ex->sp-1];  //copy last value to the frame pointer  todo: make this conditional
		//printf(" Function returns ");printType(t->sym->type->ref,1,1);
	} else {
		//printf(" Function returns nothing\n");
	}

	ram_free(ex->vars);
	
	ex->sp = ex->fp;//put stack back to repositioned fp
	ex->fp=oldfp;  //put old frame pointer back
	ex->vars = oldlocal;//put old vars back
	
		
	//printf(" return to  caller complete SP:%d  FP:%d\n", ex->sp, ex->fp); 		
		
	//todo:handle return value, putting stack back together
	return tnext(t);
}

tokenT* hprinti (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->sp--;
	printf("%d", ex->stack[ex->sp].as.z32);
	return tnext(t);
}
tokenT* hprintnl (exectxT* ex, tokenT* t) {
	printf("\n");
	return tnext(t);
}
tokenT* hprintchar (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->sp--;
	printf("%c", ex->stack[ex->sp].as.z32);
	return tnext(t);
}

void start(parsectxT* pctx, tokenT* t){
	exectxT* exectx = ram_alloc(sizeof(exectxT), NULL);
	exectx->stack = ram_alloc( sizeof(valueT)*100, NULL);
	exectx->sp = 0;
	printf(" PCTX %d bytes\n", pctx->size);
	exectx->globalvars = ram_alloc(pctx->size , NULL);
	
	printf("EXE\n");
	/*
	for (int i=0;i<32;i++){
			printf(",%02x ",  exectx->globalvars[i]  );
			
	}
	printf(" \n");*/
	
	exe(exectx, t);
	
	ram_free(exectx->stack);
	ram_free(exectx->globalvars);
	ram_free(exectx);
}



/* Parser */

/**** Parser ****/
/* After tokenizing, the parser scans through the tokens.  Sublists of tokens are removed and 'folded' under other tokens to create a tree structure representing the program.  Tokens can be assigned a handler, which, currently, executes that step of the program.  In the future, the handlers might be swapped out for functions that compile to bytecode or machine code. */




tokenT*  parseVar(tokenT*,  char** nameOut, typeT** typeOut) ;
tokenT*  parseTypeList(tokenT*, typeT* parent);

void fold(tokenT* start, tokenT* under){
		//tokens from start to (but not including under) will be removed from the list and appended to 'under'
		//fold of  1   (START)2 3   (UNDER)+    will become   1 (2 3)+

	tokenT* next;
	tokenT* prev = tprev(start);
	
	for (tokenT* t = start; t!= under; t = next) {
		
		next = zlist_next(t);
		tremove(t);
		zlist_addtail(&under->subs, &(t->zlistnode));
				
	}
}

void lfold(tokenT* under, tokenT* end){
		//tokens from under->next to end (and not including end) are removed from the tree and made subs of under
		//fold of  1     (under)x	1	2	(end)3 		will become     1   x(1 2)  3
	tokenT* next;
		
	for (tokenT* t = zlist_next(under) ; t!=end; t = next) {
		
		next = zlist_next(t);
		tremove( t  );
		zlist_addtail(&under->subs, &(t->zlistnode));
	
	}
}

/* Parses a datatype such as:
 * Simple types:  Z32, etc
 * Pointers  Z32*  [Z32]*
 * Functions (a:Z32; b:Z32 >> Z32)
 * Arrays [10 Z32]    */
tokenT*  parseType(tokenT* t) {

	char* count=NULL;
	zbool named=ZFALSE;
	tokenT* next=NULL;

	for ( ; t;  t = next ) {

		if (t->tok == '['){ //array type
			tokenT* S = t;
			
			if (tnext(t)->tok == NUMBER){
				t=tnext(t);
				count = t->str;
			}

			t  = parseType( tnext(t)); //parse the type
			
			if (t->tok != ']'){
				ERR(" missing ]\n");
			}
			
			//attach array type to opening bracket
			S->ty = findType(ARRAY, tprev(t)->ty, NULL, count? atoi(count):0);
			
			next = tnext(t);
			lfold(S,next); //everything after opening bracket to closing bracked is folded under the open
			continue;
			
		}

		if (!named && t->tok == NAME){ //simple typename, but only 1 per 'type'
					       //printf(" type %s\n", t->str);
					       //t->str = zstrcat( t->str, "$type" );
			named=ZTRUE;
			t->ty = findType(NAMED, NULL, t->str, 0);
			if (!t->ty){
				t->ty = mkType(PENDING, NULL, t->str, 0);

			}
			next = zlist_next(t);
			continue;
		}
		if (t->tok == '*'){ //pointer type
			t->ty = findType( POINTER, tprev(t)->ty, NULL,0);
			next = zlist_next(t);
			fold(tprev(t),t);
			continue;
		}
		
		if (t->tok =='('){ //type list for function parameters & return value
			tokenT* S = t;

			typeT* ty = mkType( PENDING, NULL, NULL, 0); 

			t = parseTypeList(tnext(t), ty);

			if (t->tok !=')'){
				ERR("Expected )\n");
			}
					
			S->ty = findType( FUNCTION, ty, NULL, 0); //either adds this function to the type list, or returns the version already existing

			named=ZTRUE;
			next = tnext(t);
			lfold(S,next);
			continue;
		}
		break;
	}
	return t;
}

/*parseTypeList parses both function parameter lists  a:Z32; b:Z32;, etc or type structure definitions, which are intentionally the same syntax */
tokenT*  parseTypeList(tokenT* t, typeT* parent) {
	tokenT* next=NULL;
	if (parent && !parent->members){
		parent->members = zvec_mk(NULL, 10);
		zvec_disown(parent->members);  //don't free insides when freeing vector
	}
	size_t offset=0;

	zbool reqname=ZTRUE; //parameters must be named

	for(;t;t=next){
		char* name = NULL;
		typeT* type = NULL;
		
		if (t->tok == KEND)
			break;
		
		if (t->tok == ')')
			break;  

		
		if ( (t->tok == PAIR('>','>'))){
			//is a function
			parent->category = FUNCTION;
			reqname = ZFALSE; //no longer need names (returned values are anonymous)
			next = tnext(t);
			continue;

		}
		
		
		t = parseVar(t, &name, &type  );
		//printList((tokenT*)t->zlistnode.prev->prev->prev, t, ENDFILE,0);
		if ( (reqname && !name) || !type){
			ERR(" type or name missing for struct member or function arg\n");
		}

		if (type->category > LAST_REAL_TYPE){
			ERR("Name '%s' category %d cannot be in struct\n",  safestr(name),   type->category);
		}

		if (parent && parent->category == FUNCTION){
			if (parent->ref){
				ERR("Functions can only return 1 value\n");
			} else {
				parent->ref= type ;
			}
		} else	if (parent && parent->members) {
			zvec_add(parent->members, mkType(MEMBER, type, name, offset));
		}

		offset += (zuint32) (type->size);  //TODO:  alignment
	
		if (t->tok == ';'  ){
			next = tnext(t);
			continue; //list item seperator
		}
		
		if ( (t->tok == PAIR('>','>'))){
			next = t;  // >> handler at top of loop will handle it
			continue;
		}

		break;

	}

	//reached end of a type list, guess should group them up
	if (parent && parent->category != FUNCTION)
		parent->size = offset;//total size of object (TODO:alignment)


	return t; //closing paren on func parm list OR 'end' in typedef
}

/* Parse a variable definition */
tokenT*  parseVar(tokenT* t,  char** nameOut, typeT** typeOut) {
	char* name=NULL;

	tokenT* S = t;
	
	if (tnext(t)->tok == ':'){
		
		if (t->str && t->tok != KWORDS)
			name = t->str;	
		else 
			ERR(" Token %x not allowed here (var/parm name)\n", t->tok);
				
		t=tnext(tnext(t));
	} 
		
	if (nameOut)
		*nameOut = name;
																					
	t = parseType(t);

	if (typeOut)
		*typeOut = tprev(t)->ty;  //get the type that was last parsed

	
	/*if (tprev(t)->ty){
		printf(" parsetype returned type ");
		printType(tprev(t)->ty, ZTRUE, ZFALSE);
	}*/

	if (name)
		lfold(S,t);
	
	return t;

}

typeT *tPrimitive, *tZ32, *tN32, *tN8;

zbool parsectx_cleanup(void* v){
	parsectxT* pc = v;
	ram_free(pc->symbols);
	return ZTRUE;
}

parsectxT* mkcontext()
{
	parsectxT* c = ram_alloc(sizeof(parsectxT), parsectx_cleanup);
	c->symbols = zvec_mk(NULL, 10);
	return c;
}

parsectxT* global = NULL;



tokenT*  parse(parsectxT* pc, tokenT* t) {
	
	tokenT* ts=NULL;
	symbolT* s= NULL;
	zvecT* v=NULL;
	instruction handler = NULL;

	
	
	int local=0;//true when a found symbol is from the local context
	
	//printf(" PARSE IN SYM %s %p\n", t->sym? t->sym->name : "none", t->sym? t->sym->type : NULL);
	//if (t->sym)
	//	printType(t->sym->type, 1,1);
	
	while ( t) {
		if (t->tok==ENDFILE)
			t->handler = hbreakblock; //don't really do anything

		if (t->tok==ENDFILE || t->tok== PASTENDFILE)
			return t;
			
		//printList( tprev(tprev(tprev(t))), t, PASTENDFILE,0);
		//printf(" parse token %s\n", t->str);
		//getc(stdin);
		char* name=NULL;
		char* pname=NULL;
		typeT* type=NULL;
		handler = NULL;
		
		ts = t;
			
		switch (t->tok){
		
			
			
		case KEND:
			
			if (pc->endable){
				pc->endable--;
				//printf(" 'end' block \n");
				return t;
			}
			
			ERR(" Cannot 'end' in the global context\n");
		
			
			
		case KPRIMITIVE:	//primitive declaration
			
			
			pname = tnext(t)->str;
			
			s = findSymbol(global->symbols, pname, NULL);
			if (s)
				handler = s->handler;
			else
				ERR("No primitive named %s\n", pname);
				
			t = tnext(t);
			//fall through to var decl
		case KVAR:		//variable declaration
		case KPROC:
			name=NULL;
			typeT* type=NULL;
			
			t = parseVar( tnext(t), &name, &type); //parse variable; name is required
			//printf( " KPROC ");
			//printType(type, 1,1);
			if (!name){
				ERR("Expected name and ':'\n");
			}
			
			s=mkSymbol( pc, name, type, handler);
			
			if (ts->tok == KPROC){
				//procedures go into a body of statements
				s->subctx = mkcontext();
				s->subctx->endable++; //its a subcontext
				s->subctx->type = s->type;
				t->sym=s;
				s->handler = hcall;  //need to set handler before parsing, in case of recursion
				t = parse(s->subctx, t);
			} else if (t->tok != ';') {
				ERR(" missing ;\n");
			}
			
			t=tnext(t); //skip past semicolon
			lfold(ts, t);  //everything up to an including semicolon folded
			s->tokens = ram_addref(ts); //symbol has this tokenstream
			ram_free(tremove(ts)); //remove from the executable token list 
			ts->handler=hnop;
			continue;
		case KRETURN:
			
			//todo: check return type
			//allow return no value
						
			t->handler = hreturn;
			fold(tprev(t),t);
			t=tnext(t);
						
			continue;
	
		case KBREAK:
			t->handler=hbreakloop;
			t=tnext(t);
			continue;
		case KLOOP:
			ts=t;
			pc->endable++;
			t = parse(pc, tnext(t));
			t = tnext(t);
			//todo: delete 
			ram_free(tremove(tprev(t)));//remove 'end'
			lfold(ts,t);
			ts->handler=hloop;
			
			continue;
		case KELSEIF:
			
			//take previous node as the condition
			fold (tprev(t), t);
			t->handler = hcondblock;
			t=tnext(t);
			continue;
			
		case KELSE:
			t->handler = hgroup;
			t=tnext(t);
			continue;
			
		case KIF:
			ts=t;
			ts->handler = hgroup;
			ts->val.as.n32=1;  //continue after group
			
			tokenT* cond;
			insert_after(tprev(t),  cond=mkToken(COND,"cond",4));
			fold (tprev(tprev(t)), tprev(t)); //condition statrment inside 'cond' wrapper
			fold (tprev(t), t);  //put condition node inside if
			cond->handler = hcondblock;
			pc->endable++;
			//printf(" START PARSE IF\n");
			t = parse(pc, tnext(t));  //continue parsing until 'end'
			//printf(" PARSED TO END\n");
			tokenT* end = t;
			
			
		
			t = tnext(t);
		
			lfold(ts, t); 

			//scan
			tokenT* cs=cond;
			tokenT* ls=cs;
			for (ls = cs;ls;ls=tnext(ls)){
				
				//printf("ls %p	%s\t",ls, ls->str);
				//printf("cs %p	%s\n",cs, cs->str);
				if( (ls->tok==KEND)||(ls->tok==KELSEIF)||(ls->tok==KELSE)){
					
					lfold(cs,ls);
					
					cs = ls;  
				}
				
			}

			
			continue;
			
			
					
		case KTYPE:
			if (tnext(t)->tok == NAME) 
				name = tnext(t)->str;
			else {
				ERR("expected type name\n");

			}
			
			ts->handler=hnop;

			typeT* ty = findType(NAMED, NULL, name, 0); //find a type by name

			if (ty && ty->category != PENDING){
				ERR("redefining type %s\n", name);
			}

			if (ty)
				ty->category = STRUCT; //if it was pending, its a real struct now
			else
				ty = mkType( STRUCT, NULL, name, 0);  //create a struct type

			t = parseTypeList(tnext(tnext(t)), ty);

			if(t->tok == KEND) {
				//printf(" Defined type %s as %s\n", name, tprev(t)->str);
				t=tnext(t);
				lfold(ts,t); //includes 'end' in the fold
				//tremove(ts);
								
				continue;
			}

			ERR( " Expected 'end' for type\n");

			continue;
		
		case NUMBER:
			
			//for now, assume integers
			
			zuint32 n = atoi(t->str);
			t->val.as.z32=n;
			t->ty = tZ32;
			t->handler = hconstant;
			//printf(" set handler for %s to %p\n", t->str, t->handler);
			t=tnext(t);
			continue;
		
		
		case LITERAL: //string literal (byte array)
			t->ty = findType(ARRAY, tN8, NULL, 0);
			t=tnext(t);
			continue;
				
		case PAIR('[',']'):
			//todo: check its an integer, and type is an array
			//array accesshload32
						
			ts = tprev(tprev(t));
			fold(ts, t);
			//ts->ty is pointer to array  (sizeof ptr)
			//ts->ty->ref is array of something (sizeof the array)
			//ts->ty->ref->ref is the element type 
			
			//printType( ts->ty->ref->ref, 0,0);
			
			t->ty = findType(POINTER, ts->ty->ref->ref, NULL,0);
			
			t->handler = hindex;
			t->val.as.n32 = ts->ty->ref->ref->size;
			//printf(" array element size %d\n", t->val.as.n32);
			
			if (tnext(t)->tok=='&')
				ram_free(tremove(tnext(t))) ;
			else
				insert_after(t, mkToken('@', "@", 1) );
			
			t=tnext(t);
			continue;
		
		
			
		case ':': //typecast
			
			//for now this is dumb
						
			
			ts=t; //ts is colon
			t = parseType(tnext(t));
						
			lfold(ts, t);
			fold( tprev(ts), ts);
			
			ts->ty = tsub(ts)->ty;
			tsub(ts)->handler=hnop;
			ts->handler = hgroup; //don't need to do anything for casts at the moment
			
			continue;
				
			
		case '@':  //try to handle loading ptr to ptr.  Top of stack has a pointer to the pointer var
			
			//if 'getting' a variable that is going to be assigned to, or have a sturct member taken, defer the 'dereference'
			if (	(tnext(t)->tok=='=') ||
				(tnext(t)->str && tnext(t)->str[0]=='.')
			){
				t=tnext(t);
				ram_free(tremove(ts));
				continue;
			}
			
			
			
			if ( tprev(t)->ty && (tprev(t)->ty->category == POINTER) && (tprev(t)->ty->ref->category == POINTER)){
				//printf("general pointer to pointer load\n");
				t->ty = tprev(t)->ty->ref;
				fold(tprev(t),t);
				t->handler = hloadptr;
				
				t=tnext(t);
				continue;	
								
			}
			//other '@' cases that aren't handled will drop down later
			break;
				
		case '=':  //try to handle loading ptr to ptr.  Top of stack has a pointer to the pointer var
		
			//handle     @= case.... if '@' a pointer to get a variable, and store to the variable...
			//  pointervar =         //writes a pointer to a pointer variable
			// The pointer variable is represented by a pointer to some kind of pointer
			if ( tprev(t)->ty && (tprev(t)->ty->category == POINTER) && (tprev(t)->ty->ref->category == POINTER)){
				if (tprev(tprev(t))->ty && tprev(tprev(t))->ty->category == POINTER){
					//printf("general pointer to pointer store\n");
					
					fold(tprev(tprev(t)),t);
					t->handler = hstoreptr;
					

					t=tnext(t);
					continue;	
				}
			}
			
			
									
			
		}//end switch
		
		//if didn't match anything above, continue on
		
		
		if (t->str){
			int j;
			v = zvec_disown(zvec_mk(NULL,15));
			
			tokenT* pos;
			tokenT* startfold;
			for(j=0;;j++){
				int k;
				pos = t;
				//printf(" DEPTH %d: \n", j);
				zvec_setcount(v, 0);
				for (k=0;k<j;k++){  //go back j spaces
					pos = tprev(pos);
					
					if (!pos)
						break;
				}
				if (!pos)
						break;
				startfold=pos;
				for(k=0;k<j;k++){
			
					//printf("%d:%s ", k, pos->str);
					//printType(pos->ty, ZFALSE, ZTRUE);
					if ( !pos->ty ){ //don't go past a nulltype
						pos=NULL;
						break;
					}
					zvec_add(v, pos->ty);
					pos = tnext(pos);
				}//end k
				//printf("\n");
				if (!pos)
					break;
				s=NULL;
				if (pc != global){
					local=1;
					s= findSymbol(pc->symbols, t->str, v);
				}
				
				if (s)
					break;
					
				local=0;
				s= findSymbol(global->symbols, t->str, v);
				
				if (s)
 					break;
									
				//getc(stdin);
									
			}//end j
			ram_free(v);
			v=NULL;
								
			if (s){
				//printf(" Found symbol %s  local:%d \n", t->str, local);
				//printType(s->type,0,0);
				//printf("\n");
				
				if (s->type->category == FUNCTION){
					//printf(" IS FUNCTION\n");
					fold(startfold, t);
					t->ty = s->type->ref;
					t->sym=s;
					if (s->handler){
						//printf("using handler %s %p\n", s->name, s->handler);
						t->handler=s->handler;
						
					}
				}
				else{
					//printf(" IS VARIABLE\n");
											
					t->ty = findType(POINTER, s->type, NULL, 0);  //pointer to the symbol's type
					if (local)
						t->handler = hlocal;
					else
						t->handler = hglobal;
					t->val.as.ptr.block = 0;
					t->val.as.ptr.offset= s->offset;
					
					if (tnext(t)->tok=='&')   {
						ram_free(tremove(tnext(t)));
					} else if (s->type->category!=ARRAY) {  //else if (tnext(t)){
						tokenT* tn = mkToken('@', "@", 1);  //load the variable
						insert_after(t, tn);
					}
					
															
										
				}
				t=tnext(t);
				continue;	
			}//end s
			
			//check stack variables
					
			
			if (pc->type){	 //set to function type if inside function
				int count =0;
				int pos=0;
				
				
				typeT* m = findTypeMember( pc->type, t->str, &pos, &count);
				
				if (m){
					int so = -count+pos;
					//printf("Found %s  stack pos fp+%d, of type   ", m->name, so);
					//printType(m->ref,ZTRUE, ZFALSE);
					t->ty = m->ref;
					t->handler = hstackread;
					t->val.as.z32=so;
					t=tnext(t);
					continue;
				}
				
			}
			//printf(" check if struct member\n");
			
			//check if struct member
			if (t->str && t->str[0]=='.'){
				//printf(" dot\n");
				if (tprev(t)->ty && (tprev(t)->ty->category == POINTER )&& (tprev(t)->ty->ref)){
				
					//printf(" look for member\n");
					
					typeT* m = findTypeMember( tprev(t)->ty->ref, t->str+1, NULL, NULL);
					if (m){
						
					
						//printf(" OFFSET %d for %s in %s\n", m->offset,t->str+1, tprev(t)->ty->ref->name );
						
						t->handler = hoffsetptr;
						t->val.as.n32 = m->offset;
						t->ty = findType(POINTER, m->ref, NULL,0); //find pointer to the member type
						
						fold(tprev(t),t);
						
						
						if (tnext(t)->tok=='&')   {
							ram_free(tremove(tnext(t)));
						}  else if (  m->ref->category != ARRAY   ){ //don't insert a load if struct member is an array
							tokenT* tn = mkToken('@', "@", 1);  //load the variable
							insert_after(t, tn);
						}
						
						
						t=tnext(t);
						
						
						
						
						continue;
					} //end found matching member
					//printf(" no found\n");
				}//end has ref type
				
			}
			
			printList(t,NULL,0,0);
			ERR("Undefined symbol:%s\n\n", t->str);
			
			
		}//end str
		printf("?How to parse %x %c\n", t->tok, t->tok);
		ERR("Unimplemented\n");
			
	} //end while
	//printf(" returning NULL token\n");
	return NULL;
}



int main(int argc, char** args){

	
	mkType( SIMPLE, NULL, "any", 0 ); //not really a type, but for plain pointers (any*)
	tPrimitive = mkType( PRIMITIVE, NULL, "Primitive", 0 ); //allows lookup of C functions by name
	tZ32 = mkType( SIMPLE, NULL, "Z32", sizeof(zint32));
	tN32 = mkType( SIMPLE, NULL, "N32", sizeof(zuint32));
	tN8 = mkType( SIMPLE, NULL, "N8", sizeof(zbyte));
	
	
	if (argc < 2)
		exit(1);

	char* x = ram_loadstr(args[1]);

	global = mkcontext();

	//add in primitive C function pointers
	
		
	zlistT* tokens = tokenize(NULL, x);

	ram_free(x);
	
	
	mkSymbol(global, "add32", tPrimitive, hadd32);
	mkSymbol(global, "load32", tPrimitive, hload32);
	mkSymbol(global, "store32", tPrimitive, hstore32);
	
	mkSymbol(global, "storeptr", tPrimitive, hstoreptr);
	mkSymbol(global, "loadptr", tPrimitive, hloadptr);
	
	mkSymbol(global, "print32", tPrimitive, hprinti);
	mkSymbol(global, "printchar", tPrimitive, hprintchar);
	mkSymbol(global, "printnewline", tPrimitive, hprintnl);	
	
	parse( global, (tokenT*) tokens->head->next );

	printf("Types:\n");
	int i;
 	for (i=0;i<zvec_count(types);i++)
		printType(zvec_get_at(types,i),ZTRUE, ZFALSE);
			
	printSymbols(global->symbols, "globals");
	printList((tokenT*) tokens->head,NULL,ENDFILE, 0);
	//getc(stdin);
	start(global, (tokenT*)tokens->head->next);//run
	
	ram_free(tokens);
	ram_free(global);
	ram_free(types);
	
	
	printf(" done\n");
	ram_allocs(); //dump memory leak list
	return 0;   
}



