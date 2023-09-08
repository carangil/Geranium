#include "zmem.h"
#include "zarray.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"
#include "zvector.h"




#define xprintf(a,...) fprintf(logfile, a, __VA_ARGS__),fflush(logfile)
FILE* logfile;
int parseDebugFlag = 0;

//make adjustable precision
#define FLOAT float 
#define MATH_FUNC_SUFFIX(FFF)   FFF ## f
 
#ifdef FLOAT 
#include "math.h"
#endif
void boo() {

       	           	printf("breakpoint here\n");
}
 
#define ERR( ...) { fprintf(stderr,__VA_ARGS__);fflush(stderr);boo();  exit(1);}
 
//uncommenting below will log a LOT while running.
//#define EXEDEBUG 

/**** Basic Values ****/

//Pointers
typedef struct vptrS{	
	char* block;	/* char instead of void, for strict aliasing */
	zuint32 offset;
	zuint16	level; 
}vptrT;

typedef struct vptrselectorS {
	vptrT ptr;
	struct typeS* typeselector;
} vptrselectorT;

typedef union valu {	//Generic value (datatype is tracked through other means)
		zint32 z32;
		zuint32 n32;
		vptrT ptr;
#ifdef FLOAT
		FLOAT f;
#endif
		struct typeS* type; //not datatype of valU, but represents a detatype itself (datatypes can be on the stack)  
		
	} valU;
	
typedef struct valueS{
	valU as;
	struct typeS* typeselector;
}valueT;

/**** Execution Context ****/
typedef struct exectxS{
	valueT* stack;  //call/parameter stack
	zuint32	sp;	//number of items on stack.  sp-1 is the top item
	zuint32 fp;
	char* vars; 	//local data space
	char* globalvars; //global data space
	int stop;
	int level;//stackframe level
}exectxT;
#define STOPFUNC 1
#define STOPLOOP 2

typedef struct tokenS* (*instruction) (exectxT*,struct tokenS* ) ;

/* Parse Context */
typedef struct parsectxS{
	zvecT* symbols;	//of type symbolT*
	zuint32	size;	//size of variables in this table
	struct typeS* type;  //if in a procedure, we need to know about its return type and args
	struct parsectxS* parent;
	int endable;
}parsectxT;

/**** Tokenizer ****/

/* Program is a linked list of tokens*/
typedef struct tokenS{
	zlistnodeT zlistnode;
	zuint32 tok;	//A constant defined below, a character, or a pair of characters
	char* str;	//string representation of this token
	struct typeS* ty;	//datatype of this token
	struct typeS* tyorig;	//original datatype of this token (before cast)
	valueT val;	//token's value
	zlistT subs;	//make a tree out of token list
	instruction handler; //function that does what this token represents
	struct symbolS* sym;  //for things like procs that have a bunch of context info
	int line;	//line number from source file
	zbool val_to_free; //if true, free val's ptr block when destroying token
	struct tokenS* trackpossptr;

	struct tokenS** debug_subs;
	struct tokenS** debug_next;
	struct tokenS** debug_prev;

}tokenT;


/* Macros to make some things easier
 * tremove: removes a token from the list (and returns it as a pointer to be assigned somewhere else or freed.  To remove requires a token is in front of or behind it
 * tnext/tprev: Pointer to the next token
 * insert_after: Inserts a token after another token.  Requires the place of insertion is not the head or tail of the list
 * */

#define tremove(ITEM)    zlist_remove_mid(  &(ITEM)->zlistnode)
//#define tnext(ITEM) ((tokenT*)(ITEM)->zlistnode.next)
#define tnext(ITEM)    ((tokenT*)zlist_next(ITEM))
#define tprev(ITEM)    ((tokenT*)zlist_prev(ITEM))
#define insert_after(AFTER,NEW)    zlist_insert_node_after(  &(AFTER)->zlistnode,  &(NEW)->zlistnode);

//tokenT->tok values:
#define PAIR(B1,B2)	((((unsigned int)(B1&0xff)) <<8) | ((unsigned int)(B2&0xff)))
#define NAME		0x200
#define NUMBER		0x300
#define LITERAL 	0x400
#define ENDFILE		0x500
#define PASTENDFILE	0x600
#define STARTFILE	0x700
#define COND		0x9001
#define STACKARG	0x9002

//Token values that are also user-accessible keywords:
#define KWORDS		0x8000
#define KVAR		0x8000
#define KTYPE		0x8001
#define KEND		0x8002
#define KPRIMITIVE	0x8003
#define KPROC		0x8004
#define KRETURN		0x8005
#define KIF			0x8006
#define KELSE		0x8007
#define KELSEIF		0x8008
#define KLOOP		0x8009
#define KBREAK		0x800a
#define KNEW		0x800b
#define KPROTO		0x800c
#define KTRASH		0x800d
#define KKEEP		0x800e
#define KTAKE		0x800f
#define KINCLUDE	0x8010
#define KVIRTUAL	0x8011
#define KSELECTOR	0x8012
#define KCPOINTER	0x8013
#define KNEW0		0x8014
#define KCOUNT		0x8015
#define KSIZE		0x8016	
#define KSETCOUNT   0x8017
#define KCDATA		0x8018

char*  keywords[] = {"var", "type", "end", "primitive", "proc","return", "if", "else", "elseif", "loop", "break", "new", "proto", "trash", "keep", "take", "include", "virtual", "selector", "cpointer", "new0", "count", "size", "setcount","cdata",  NULL};

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
	if (t->val_to_free)
		ram_free(t->val.as.ptr.block);
	//xprintf(" cleaning subs\n");
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

	//tokenT** s that point to the listnode... so in a debugger we can see *debug_next as next as a tokenT
	t->debug_next = &t->zlistnode.next;
	t->debug_prev = &t->zlistnode.prev;
	t->debug_subs = &t->subs.sentinal_head.next;

	return t;
}

//guards against potentially null strings
#define safestr(SSS)  ((SSS) ? (SSS):"<nullstring>")

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

//reads string 'in', and adds token nodes AFTER *insert
zbool tokenize(tokenT* insert, char* in){

	int c,next;
	tokenT* t=NULL;
	
	int line=1;

	while ((c = *in)){
		next = *(in+1);
		zuint32 p;
		if (c == '\n')
			line++;

		//find twochar patterns like ->,etc. including comment start/end markers
		if ((p = findPair("<<>>--++->==/**///[]>=<=!=.-", c, next))){
			if (  p == PAIR('/','/')  ) { //special handling for // comments
				while(*in!= '\n')
					in++;
				line++;
				continue;
			}
	
			t = mkToken( PAIR(c, next) , in, 2);
			zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
			insert = t;
			
			in+=2;
			continue;
		}

		//check for string literals
		int lit = acceptLiteral(in, '"' , '\\' ); //double quote

		if (lit) {
			t = mkToken(LITERAL, in, lit);
			zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
			insert = t;
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
				"'.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",  //start with ._alpha
				"",
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789"); //numbers can be in the name after the first character
		int digits  = 0;
		
		if (!name){
			digits  = acceptPatterns(in,
					"-.0123456789", //start with digit or decimal point
					"e-E-e+E+",  //- and + only accepted after an e or E
					"0123456789abcde.fABCDEFxlLuUfF"); //continues with digits, deccimal point, hex letters, type suffix letters
			
			//special case: if number starts with '-', but has only 1 character, this isn't a negative number, but just a minus sign
			if ((digits == 1) && (in[0] == '-'))
				digits = 0;
		}
						
		if (digits || name){
			t = mkToken( digits? NUMBER : NAME, in ,   digits|name);
			in += digits|name;
			zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
			insert = t;
			continue;
		}

		//just some char
		t = mkToken( *in, in, 1);
		t->line=line;
		zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
		insert = t;
		in++;
	}
	
	

	
}

/**** Data Types ****/

/* Simple type system*/
#define SIMPLE	1
#define POINTERUSER 2
#define STRUCT 	3
#define ARRAYSTATIC 	4
#define ARRAYDYNAMIC 	5
#define FUNCTION 6
#define PRIMITIVE 7
#define POINTERPOSSESSIVE 8
#define CPOINTER 9
#define CDATA 10
#define VIRTUAL 11
#define LAST_REAL_TYPE 11
//letting virtual types be 'real' when parsing a struct type definition makes the code easier
//CPOINTER is an opaque value.  Different CPOINTERS can have different names for some type safety, but they are all treated the same
//CDATA is also an opaque value, but is never manipulated directly, only by pointers (% or &).  For structures allocated by C, but by using the interpreter's memory allocator.  Possessive CDATA pointers particpate in the normal reference tracking... keep, trash, take, auto free on out of scope, etc.
//CDATA is also an opaque value, but is never manipulated directly, only by pointers (% or &).  For structures allocated by C, but by using the interpreter's memory allocator.  Possessive CDATA pointers particpate in the normal reference tracking... keep, trash, take, auto free on out of scope, etc.

//ARRAYSTATIC have a fixed size.  To be embedded directly in structs, etc
//ARRAYDYNAMIC are heap allocated

//MEMBER is not a type, but is used to mark members of a struct
#define MEMBER	20
//NAMED is not a type, but when passed into findType looks for struct or simple  w/out knowing which it is yet
#define NAMED	21
//PENDING not a type, but is for when a type is mentioned in another declaration but not yet defined.  You can't 'make' or size a PENDING type, but can have pointers to them
#define PENDING  22


typedef struct typeS{
	char* name;
	size_t size;		//for structs
	zuint32 category;	//SIMPLE, POINTER, etc
	zuint32 len; //for definite arrays, 0 for indefinite arrays
	zuint32 offset; //for struct members (byte position)
	struct typeS* ref; //array or pointer types, or function return type
	zvecT* members;  //(typeT*) structs or function parameters
	zvecT* selectors; //(symbolT*)  function selectors
	int trashAfterPrimitive; //only for function args, only when passing %pointer
	int tid;
}typeT;

zvecT* types;
int tid=0;

zbool type_cleanup(void* v){
	typeT* ty = v;
	ram_free(ty->name);
	ram_free(ty->members); 
	ram_free(ty->selectors);
	return ZTRUE;
}


//creates a type and returns a pointer do it.  Caller does not need to free the pointer
typeT* mkType(zuint32 category, typeT* ref, char* name, size_t szlen){
	
	
	typeT* ty= ram_alloc(sizeof(typeT), type_cleanup); //todo: destructor
	
	ty->tid = tid++;
	ty->category = category;
	ty->name = zstrdup(name);
	ty->ref = ref;

	if (category == ARRAYSTATIC || category == ARRAYDYNAMIC){	
		ty->len = szlen;
		ty->size = szlen * ref->size;
	}
	else if (category == MEMBER){
		ty->offset = szlen;
	} else {
		ty->size = szlen;
	}

	//pointer or indefinite array
	if ((category == POINTERUSER)|| (category == ARRAYDYNAMIC)||(category == POINTERPOSSESSIVE) ){
		if (ty->size)
			ERR("Cannot specify size of pointer or dynamic array (it is automatically calculated)\n");
		if ((category == POINTERUSER) || (category == POINTERPOSSESSIVE)) {
		
			if (ref->category == VIRTUAL) {
				ty->size = sizeof(vptrselectorT); //pointer to object AND pointer to selector table
			} else {
				ty->size = sizeof(vptrT);
			}

		}
		
		if (category == ARRAYDYNAMIC) 
			ty->size = ref->size; //size of 1 element
	}

	if (types == NULL)
		types = zvec_mk(NULL, 100);

	zvec_add(types, ty);

	return ty;
}
void printSymbols(zvecT* table, char* label);
void printType(typeT* ty, zbool line, zbool skipmembers){
	char* end = "";
	
	if (!ty) {
		xprintf("{nulltype}");
		return;
	}
	
	if (ty->category == VIRTUAL) {
		xprintf("");
	}

	if (ty){
		xprintf("%d~", ty->tid);
		if (!skipmembers)
			xprintf("<size%d>", (int)ty->size);
		
		switch(ty->category) {
		case  PRIMITIVE:
			xprintf("<PRIMITIVE>");  //continue on as func
		case  FUNCTION:
			xprintf("<func>(");
			end=")";
			skipmembers=ZFALSE;
			break;
		
		case ARRAYSTATIC:
		case ARRAYDYNAMIC:
			if (ty->len)
				xprintf("[%d ", ty->len);
			else
				xprintf("[");
			
			
			end="]";
			break;
			
		case POINTERUSER:
			end="&"; 
			break;
			
		case POINTERPOSSESSIVE:
			end="%"; 
			break;
			
		case PENDING:
			xprintf("<pending>");
			break;
			
		case STRUCT:
			if (!skipmembers){
				xprintf("type ");
				end=" end";
			}
			break;
		case MEMBER:
			xprintf(".");
			break;
		case VIRTUAL:
			xprintf("virt. ");
			break;

		case CPOINTER:
			xprintf("C*  ");
			break;
		case CDATA:
			xprintf("CDATA  ");
			break;
		case SIMPLE:
			break;
		default:
			xprintf(" Unknown printType category %d\n", ty->category);
		}
		
		if (ty->name)
			xprintf("%s%c", ty->name, ty->category == MEMBER? ':':' ');
	
		if (ty->trashAfterPrimitive)
			xprintf(" trash  ");

		/*
		if (!skipmembers && (ty->selectors)) {
			printSymbols(ty->selectors, "\n~SELECTORS:\n");
			printf("~");
		}
		*/

		if (!skipmembers && ty->members){
			int i;
			for (i=0;i<zvec_count(ty->members);i++){
				printType( zvec_get_at(ty->members,i), ZFALSE, ZTRUE);
				xprintf("; ");
			}
	
		}

		
		
		if (ty->category == FUNCTION)
			xprintf(" -> ");
				
		if (ty->ref)
			printType(ty->ref, ZFALSE, ZTRUE);
		
		xprintf("%s",end);
	}
	
	if (line)
		xprintf("\n");
	
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

	if ((ty->category == ARRAYSTATIC) &&(ty->len != len)) //array of different sizes
		return ZFALSE;

	//check reference same type  (findType should not be returning equivalent duplicates)

	if ( (category ==  FUNCTION)&&(ref)&& (ref->category==FUNCTION)) {

		//ty shuld contain a FUNCTION type, with ref and members
		//ref should contain a FUNCTION type with the same ref and members

		if (ty->ref != ref->ref){
			//xprintf(" functions return different types\n");
			return ZFALSE;
		}

		if (zvec_count(ty->members) !=zvec_count(ref->members)){
		//	xprintf(" function has different num of arguments\n");
			return ZFALSE;
		}

		int i;
		for (i=0;i<zvec_count(ty->members);i++){
			typeT* memberty = zvec_get_at(ty->members,i);
			typeT* memberref = zvec_get_at(ref->members,i);
			//compare types of members
			if (memberty->ref != memberref->ref){
			//	xprintf("arg %d to function is of different type\n", i);
				return ZFALSE;
			}
			
			//compare names of members too
			if (strcmp(memberty->name, memberref->name)){
				//xprintf("arg %d to function is of different name\n", i);
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
			
			case VIRTUAL:
			case NAMED:  //find structs, simples or pendings by name
			case SIMPLE: //simple types matched by name only
			case STRUCT: 
			case PENDING:
			case CPOINTER:
			case CDATA:
				if (!strcmp(name, ty->name)){
					//found on name
					return ty;
				}
				break; //not it

			case POINTERPOSSESSIVE: 
			case POINTERUSER: 
			case ARRAYSTATIC:
			case ARRAYDYNAMIC:
			case FUNCTION:

				if (cmpType(category, ref, NULL, len, ty))
					return ty;

				break;

			default:
				ERR("unknown type category %d\n", category);
		}
	}

	//did not find.

	if (ref &&((category == ARRAYDYNAMIC)||(category==ARRAYSTATIC) || (category == POINTERUSER)|| (category == POINTERPOSSESSIVE))) {

		//If array or pointer, find the type 'underneath' and make it

		//xprintf(" Creating %s type for ", getTypeString(category));
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
int printList(tokenT* t, tokenT* cur, zuint32 stop_tok, int indentin){

	char* iscur;
	int i;
	int indent=indentin;
	zuint32 count=0;
		
	//if stop_tok is set to a negative number (like -3) then up to 3 tokens will be printed (and their subs)
	
	for ( ;t;  t = zlist_next(t)  ){
		
		
		if (  ( (zint32)stop_tok < 0) && (count == stop_tok))
			break;
		count--;
		
		//print subs first
		if (zlist_head(&t->subs)){ 
			//indent = 
			xprintf("\n");
			for (i=0;i<indent;i++) 
				xprintf("\t");
			xprintf("{");
			printList(  zlist_head(&t->subs), cur, 0, indent+1);
			
		} 
		
		if (t==cur)
			iscur="CUR";
		else 
			iscur="   ";
		
		xprintf("\n");
		for (i=0;i<indent;i++) 
			xprintf("\t");
		
		
		if ( zlist_head(&t->subs)){ 
			xprintf("} ");
		} else {
			xprintf("{");
		}
		
		if ( (t->tok >20) && (t->tok < 0x7f))
			xprintf(" %s %c  ",iscur, t->tok);
		else
			xprintf(" %s%04x", iscur, t->tok);
		
		if (t->str)
			xprintf("= '%s'", t->str);
			
		if (t->ty) 
			printType(t->ty, ZFALSE, ZTRUE);
		
		if (t->tyorig) {
			xprintf(" castfrom ");
			printType(t->tyorig, ZFALSE, ZTRUE);
		}
		
		if (t->trackpossptr){
			xprintf("(Tracking %s ", t->trackpossptr->str);
			printType( t->ty, ZFALSE, ZTRUE);
			xprintf(")");
		}

		
		if (t->handler)
			xprintf(" handler %p ", t->handler);

		xprintf("(%p %x)",t->val.as.ptr.block, t->val.as.ptr.offset);
		
		if (!zlist_head(&t->subs))
			xprintf("}");
	
		if (t->tok == stop_tok)
			break;
		
		if (t->tok == PASTENDFILE)
			break;
	}

	return indent;
}

/**** Symbols ****/

typedef struct symbolS{
	char* name;
	char* alias;
	typeT* type;
	zuint32 offset;
	instruction handler;
	tokenT* tokens;
	struct parsectxS* subctx; //procs have their own parsecontext for their local vars
	int isPrototype;// true if this symbol is just a function prototype
	int isSelector;// true if this symbol is a function selector
	int selectorArg; //if this is a selector, which arg does the function lookup
	int selectorNum;  //which selector (nth) is this?
} symbolT;

zbool symbol_cleanup(void* v){
	symbolT* s = v;
	
	ram_free(s->name);
	ram_free(s->alias);
	ram_free(s->subctx);
	ram_free(s->tokens);
	//types are freed elsewhere
	return ZTRUE;
}

zbool cmpTypeListToFunc(zvecT* f, zvecT* b){
	//iterates ove r a functions list of arguments and compares to a possible list of types
	
	if (zvec_count(f) !=zvec_count(b)){
		//	xprintf(" function has different num of arguments\n");
			return ZFALSE;
		}

		int i;
		for (i=0;i<zvec_count(f);i++){
			typeT* memberf = zvec_get_at(f,i);
			typeT* memberb = zvec_get_at(b,i);

		

			//compare types of members
			if (memberf->ref != memberb){
			//	xprintf("arg %d to function is of different type\n", i);
				return ZFALSE;
			}
		}
		return ZTRUE;
}




symbolT* findSymbol(zvecT* table, char* name, zvecT* typelist){
	
	int i;	
	symbolT* s;
	
	for (i=0;i<zvec_count(table);i++){

		s = zvec_get_at(table, i);
				
		if (   (!strcmp(name, s->name)) || (s->alias && (!strcmp(s->alias,name)))) {
						
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
 	//		ERR(" type with size 0\n");
		}
	
	}
	
	sym = ram_alloc(sizeof(symbolT), symbol_cleanup);
	sym->name = zstrdup(name);
	sym->type = type;
	sym->handler = handler;
	sym->offset = pctx->size;
	
	pctx->size += type->size; //add context u
	
	xprintf(" SYMBOL %s at offset %d  , total symbols %d bytes\n", sym->name, sym->offset, pctx->size);
	
	return zvec_add_or_free(table, sym);
}

void printSymbols(zvecT* table , char* label){
	int i;	
	int j;
	xprintf("\n\nSymbols for %s\n", label);
	for (i=0;i<zvec_count(table);i++){
		symbolT* sym = zvec_get_x_at(table, symbolT*, i);
		xprintf("#%x\t%s\t", sym->offset, sym->name);
		printType(sym->type, ZTRUE,ZFALSE);
				
		
		xprintf("\n");
	}
}

void free_array_of_possessive_pointers(void* v, zuint32 len, zbool virtual);

/**** Execution ****/
zbool struct_clean(void* v, typeT* ty);
void clean_context_pointers( zvecT* table, char* vars){
	
	int i;
	for (i=0;i<zvec_count(table);i++){
		symbolT* sym = zvec_get_x_at(table, symbolT*, i);
		if (sym->type->category == POINTERPOSSESSIVE){
			ram_free( * (void**)  (vars+  sym->offset)  ); //free the possessive pointer
		}
		if (sym->type->category == STRUCT){
			struct_clean(  (void*)  (vars+  sym->offset) , sym->type);  //clean up structs
		}

	
		
		if ((sym->type->category == ARRAYSTATIC)&&(sym->type->ref->category == POINTERPOSSESSIVE)){
			//ERR(" Free static array of possessive pointers\n");
			free_array_of_possessive_pointers( (void*)  (vars+  sym->offset) , sym->type->len, sym->type->ref->ref->category == VIRTUAL   );
			
		}
		
	}	
}


void exe (exectxT* c, struct tokenS* t){
	
	struct tokenS* ts=t;
	
	while(t && ! c->stop){  //until out of instructions, or a return is bubbling up
						
		instruction handler = t->handler;
		
		char* str=  safestr(t->str);
#ifdef EXEDEBUG
		xprintf("%x %s  (pre) handler %p\n",t->tok, str, handler);
#endif
				
		if (!handler){
			
			printList(ts, t, 3, 0);
			ERR("null handler for %c %s\n", t->tok, str);
		}
			
		//getc(stdin);
		t = handler(c,t);
		
#ifdef EXEDEBUG
		xprintf("-> %s",str);
		xprintf("sp %x:\n", c->sp);
		int i;
		for (i=0;i<c->sp;i++){
			xprintf("%d: (%p+%x)/%d", i, c->stack[i].as.ptr.block,c->stack[i].as.ptr.offset, c->stack[i].as.z32);
			if (c->stack[i].typeselector)
				printType(c->stack[i].typeselector, ZFALSE, ZFALSE);
			
			xprintf("\n");
			
			
		}	
		xprintf("\n\n");
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
#define tsub(TTT)  ((tokenT*)zlist_head(&(TTT)->subs))

tokenT* haddselector(exectxT* ex, tokenT* t) {  //adds selectors to item on pointer stack
	exe(ex, tsub(t));
	ex->stack[(ex->sp)-1].typeselector = t->val.as.type;
	return tnext(t);
}


tokenT* hglobal (exectxT* ex, tokenT* t) {	//push pointer to global variable on stack
 	ex->stack[(ex->sp)].as.ptr.block = ex->globalvars;
	ex->stack[(ex->sp)].as.ptr.level = 0; 
	ex->stack[(ex->sp)++].as.ptr.offset = t->val.as.ptr.offset;
	//xprintf(" Global block %p +%d\n", ex->globalvars  ,   t->val.as.ptr.offset);
	return tnext(t);
}

tokenT* hlocal (exectxT* ex, tokenT* t) {	//push pointer to local variable on stack
	ex->stack[(ex->sp)].as.ptr.block = ex->vars;
	ex->stack[(ex->sp)].as.ptr.level = ex->level;//pointer is in this stack frame
	ex->stack[(ex->sp)++].as.ptr.offset = t->val.as.ptr.offset;
	
	//xprintf(" local block %p +%d   level %d\n", ex->vars  ,   t->val.as.ptr.offset, t>val.as.ptr.level);
	return tnext(t);
}

//#define tsub(TTT)  ((tokenT*)((TTT)->subs.head))




tokenT* hstackread (exectxT* ex, tokenT* t) {	//read a stack variable (really function parameters)
	
	if (!tsub(t)){
		ERR("hstackread needs sub for stack position offset\n");
	}
	
	ex->stack[ex->sp] = ex->stack[ ex->fp + tsub(t)->val.as.z32 ];
	
#ifdef EXEDEBUG
	xprintf(" READ STACK POSITION + %d  option %d\n", tsub(t)->val.as.z32, t->val.as.n32);
#endif
	if (t->val.as.n32 == 1)
		ex->stack[ ex->fp + tsub(t)->val.as.z32 ].as.ptr.block = 0;  //zero out the source ( KTAKE on possessive pointer)
	
	if (t->val.as.n32 == 2)
		ex->stack[ex->sp].as.ptr.level = ex->level+1; //add level to source (so it must be used or passed but not locally stored or returned)
	
	(ex->sp)++;
	
	return tnext(t);
}

#define DEREF(TYPE,BASE,OFFSET)      (*((TYPE*)(((char*)(BASE))+(OFFSET))))



tokenT* hstoreptr (exectxT* ex, tokenT* t) {  //store a pointer
	exe(ex, tsub(t) );
#ifdef EXEDEBUG
	xprintf(" DST LEVEL: %d  SRC LEVEL: %d\n", ex->stack[ex->sp-1].as.ptr.level, ex->stack[ex->sp-2].as.ptr.level);
#endif
	if (  ex->stack[ex->sp-1].as.ptr.level < ex->stack[ex->sp-2].as.ptr.level){
		xprintf("err-----------\n");
		printList(tprev(t), t, 0 ,2);
		ERR("Pointer level mismatch: attempt to escape scope\n");
	}
	
	if (t->val.as.n32 & 1) {
		//flag to free the destination pointer if its non-null
#ifdef EXEDEBUG
		xprintf(" storeptr: free block in destination address  %p\n", DEREF(vptrT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset).block);
#endif
		(DEREF(vptrT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset).block);
	}


	vptrselectorT* vps = &DEREF(vptrselectorT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset);
	vptrT* vp = &DEREF(vptrT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset);

	if (t->val.as.n32 & 8) {  //also store selector
		
		

		DEREF(vptrselectorT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset).typeselector = ex->stack[ex->sp - 2].typeselector;

		
		
	}


	
	DEREF(vptrT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = ex->stack[ex->sp-2].as.ptr;
	ex->sp-=2;
	return tnext(t);
}


//tokenT* hcall (exectxT* ex, tokenT* t);
//tokenT* hreturn (exectxT* ex, tokenT* t);
//tokenT* hgroup (exectxT* ex, tokenT* t);

tokenT* hloadptr (exectxT* ex, tokenT* t) { //load a pointer
	
	exe(ex, tsub(t) );
		
	vptrselectorT* vps = &DEREF(vptrselectorT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset);
	vptrT* vp = &DEREF(vptrT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset);

	if (t->val.as.n32 & 8) {  //also load selector  
		ex->stack[ex->sp - 1].typeselector = DEREF(vptrselectorT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset).typeselector;
	}

	ex->stack[ex->sp-1].as.ptr = DEREF(vptrT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	
	
	if ( t->val.as.n32 & 1) {
		//the pointer will be assigned ex->level + 1
		//this makes it so a possessive pointer can be duplicated as a user pointer and passed as an arg to a function
		//if it was allowed to make user pointers available at the same level(this function and not just as an arg to another function)
		//then its possible for the possessive pointer we are copying to be freed
		//and that would leave a dangling user pointer.
		//the reason why we can pass a user pointer to a function without fear of dangling, is because this calling function holds a reference
		//to it, so the pointer is guaranteed to be good until the callee returns, allowing it to be used by the callee as long as it needs
		//and since its a user pointer, the callee can't store it anywhere in the heap, or return it
		//and it can't write it to a lower level pointer variable
		//and since it isn't possessive, it doesn't have to be freed either, so it can't leak
		ex->stack[(ex->sp-1)].as.ptr.level = ex->level+1;//pointer 'belongs' to deeper stack frames
	}
	if (t->val.as.n32 & 2){
		//non-possessive pointer, passed into a function, needs to be addreffed
		ram_addref(ex->stack[ex->sp-1].as.ptr.block);
	}
	
	return tnext(t);
}



tokenT* htakeptr (exectxT* ex, tokenT* t) { //load a pointer, source is made null  (doesn't affect reference count)
	
	exe(ex, tsub(t) );

	if (t->val.as.n32 & 8) {  //also load selector
		ex->stack[ex->sp - 1].typeselector = DEREF(vptrselectorT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset).typeselector;
	}
	 
 	vptrT tmp = DEREF(vptrT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);

	//zero the source pointer
	DEREF(vptrT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset).block = NULL;
	
	ex->stack[ex->sp-1].as.ptr = tmp;
	

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
	//xprintf(" index using multiplier %d\n", t->val.as.n32);
	ex->sp--;
	return tnext(t);
}

tokenT* harrayinfo(exectxT* ex, tokenT* t) {	//index into array
	exe(ex, tsub(t));

 	void* array = ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset;
	ex->stack[ex->sp - 1].as.ptr.block = 0;

	if (t->val.as.n32 == 0)
		ex->stack[ex->sp - 1].as.n32 = zarray_size(array);
	else if (t->val.as.n32 == 1)
		ex->stack[ex->sp - 1].as.n32 = zarray_count(array);
	else if (t->val.as.n32 == 2) {
		 zarray_use(array, ex->stack[ex->sp - 2].as.n32);
		 ex->sp-=2;
	}

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
			//xprintf (" COND is true, execute body\n");
			exe(ex, subs); //continue this  
			//xprintf(" EXIT CHAIN\n");
			return NULL;
	} 
	//xprintf(" COND was false, so call next in chain\n");
	return tnext(t); //next one
		
	
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


typeT *tType, *tPrimitive, *tZ32, *tN32, *tN8, *tBit, *tString, *tReal;



tokenT* hload32 (exectxT* ex, tokenT* t) {
	
	exe(ex, tsub(t));

	zint32 i = DEREF(zint32, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	//xprintf(" Loaded 32 %d   from +%x\n", i,ex->stack[ex->sp-1].as.ptr.offset );
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


tokenT* hloadcptr(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	void* i = DEREF(void*, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset);
	//xprintf(" Loaded 32 %d   from +%x\n", i,ex->stack[ex->sp-1].as.ptr.offset );
	ex->stack[ex->sp - 1].as.ptr.block = i;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}


tokenT* hstorecptr(exectxT* ex, tokenT* t) {

	exe(ex, tsub(t));

	DEREF(void*, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset) = ex->stack[ex->sp - 2].as.ptr.block;

	ex->sp -= 2;

	return tnext(t);
}



tokenT* hload8 (exectxT* ex, tokenT* t) {
	
	exe(ex, tsub(t));
#ifdef EXEDEBUG
	xprintf(" Load byte at %p+%d\n",  ex->stack[ex->sp-1].as.ptr.block,  ex->stack[ex->sp-1].as.ptr.block);
#endif	
	zbyte i = DEREF(zbyte, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	ex->stack[ex->sp-1].as.ptr.block=NULL;
	ex->stack[ex->sp-1].as.n32 = i;
	
	
	return tnext(t);
}


tokenT* hstore8 (exectxT* ex, tokenT* t) {
	
	exe(ex,tsub(t));
		
#ifdef EXEDEBUG
	xprintf(" Store byte at %p+%d\n",  ex->stack[ex->sp-1].as.ptr.block,  ex->stack[ex->sp-1].as.ptr.offset);
#endif
	DEREF(zbyte, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = (zbyte) ex->stack[ex->sp-2].as.n32;
	
	ex->sp-=2;
	
	return tnext(t);
}




tokenT* hprint32 (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->sp--;

	printf("%d", ex->stack[ex->sp].as.z32);

	return tnext(t);
}

#ifdef FLOAT
tokenT* hprintfloat (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->sp--;

	printf("%.10g", ex->stack[ex->sp].as.f);

	return tnext(t);
}


tokenT* hfload (exectxT* ex, tokenT* t) {
	
	exe(ex, tsub(t));
	
	FLOAT f = DEREF(FLOAT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	
	ex->stack[ex->sp-1].as.ptr.block=NULL;
	ex->stack[ex->sp-1].as.f = f;
		
	return tnext(t);
}


tokenT* hfstore (exectxT* ex, tokenT* t) {
	
	exe(ex,tsub(t));
		
	DEREF(FLOAT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = ex->stack[ex->sp-2].as.f;
	
	ex->sp-=2;
	
	return tnext(t);
}



#endif

tokenT* hprintchar (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->sp--;

	printf("%c", ex->stack[ex->sp].as.z32);
	
	return tnext(t);
}

tokenT* hprintptr (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
	ex->sp--;

	printf("{%p+%x lvl%d refs%d ", ex->stack[ex->sp].as.ptr.block,  ex->stack[ex->sp].as.ptr.offset, ex->stack[ex->sp].as.ptr.level   , ram_numrefs( ex->stack[ex->sp].as.ptr.block)  );

	if (ex->stack[ex->sp].typeselector) {
		printType(ex->stack[ex->sp].typeselector, ZFALSE, ZTRUE);
	}
	xprintf("}");

	return tnext(t);
}

tokenT* hreadchar (exectxT* ex, tokenT* t) {
	ex->stack[(ex->sp)++].as.z32 = fgetc(stdin);
	return tnext(t);
}

#define BINOP(NAME, RESULTAS, AS, OPERATOR)\
tokenT* NAME (exectxT* ex, tokenT* t) {						\
	exe(ex, tsub(t));							\
	ex->stack[ex->sp-2].as.RESULTAS  = ex->stack[ex->sp-2].as.AS  OPERATOR   ex->stack[ex->sp-1].as.AS;	\
	ex->sp--;							\
	return tnext(t);							\
}

//integer arithmetic
BINOP(hadd32, z32, z32, + )
BINOP(hsub32, z32, z32, - )
BINOP(hmul32, z32,  z32, * )
BINOP(hdivz32, z32, z32, / )
BINOP(hand32, z32,  z32, & )
BINOP(hor32, z32,   z32, | )
BINOP(hxor32, z32,   z32, ^ )
BINOP(hequal32, z32, z32, == )
BINOP(hnotequal32, z32, z32, != )

//unsigned division
BINOP(hmodu32, n32, n32, % )
BINOP(hdivu32, n32, n32, / )

//signed compares
BINOP(hless32, z32,z32, < )
BINOP(hlesse32, z32,z32, <= )
BINOP(hgreater32, z32,z32, > )
BINOP(hgreatere32, z32,z32, >= )

//unsigned compares
BINOP(hlessu32, n32, n32, < )
BINOP(hlesseu32, n32, n32, <= )
BINOP(hgreateru32, n32, n32, > )
BINOP(hgreatereu32, n32, n32, >= )

#define UNOP(NAME, RESULTAS, AS, OPERATOR)\
tokenT* NAME (exectxT* ex, tokenT* t) {						\
	exe(ex, tsub(t));							\
	ex->stack[ex->sp-1].as.RESULTAS  =  OPERATOR (  ex->stack[ex->sp-1].as.AS );	\
	return tnext(t);							\
}

UNOP(hboolnot, z32, z32, !)
UNOP(hinvert32, z32, z32, ~)
UNOP(hneg32, z32, z32, -)

#define BINOPFUNC(NAME, AS, FUNC)\
tokenT* NAME (exectxT* ex, tokenT* t) {						\
	exe(ex, tsub(t));							\
	ex->stack[ex->sp-2].as.AS  =   FUNC (   ex->stack[ex->sp-2].as.AS,   ex->stack[ex->sp-1].as.AS);	\
	ex->sp--;								\
	return tnext(t);							\
}

//float arithmetic
#ifdef FLOAT

BINOP(hfadd, f,f,  + )
BINOP(hfsub, f,f, - )
BINOP(hfmul, f,f, * )
BINOP(hfdiv, f,f, / )

UNOP(hfneg, f,f, -)

BINOP(hflesse, z32, f, <= )
BINOP(hfless, z32, f, < )
BINOP(hfgreatere, z32, f, >= )
BINOP(hfgreater, z32, f, > )

//functions, but easily called 'like' a unary op
UNOP(hfabs, f,f, MATH_FUNC_SUFFIX(fabs))
UNOP(hfsqrt, f,f, MATH_FUNC_SUFFIX(sqrt))
UNOP(hfsin, f,f, MATH_FUNC_SUFFIX(sin))
UNOP(hfcos, f,f, MATH_FUNC_SUFFIX(cos))
UNOP(hftan, f,f, MATH_FUNC_SUFFIX(tan))

UNOP(hfasin, f,f, MATH_FUNC_SUFFIX(asin))
UNOP(hfacos, f,f, MATH_FUNC_SUFFIX(acos))
UNOP(hfatan, f,f, MATH_FUNC_SUFFIX(atan))

BINOPFUNC(hfpow,  f, MATH_FUNC_SUFFIX(pow))
BINOPFUNC(hfatan2,  f, MATH_FUNC_SUFFIX(atan2))

UNOP(hint2real, f,z32, (FLOAT)  )
UNOP(hreal2int, z32,f, (zint32) )

#endif


#define HANDLER(CONTEXT, NAME)	mkSymbol( CONTEXT,  #NAME, tPrimitive, h ## NAME)

void addhandlers(struct parsectxS* pctx){

	//binop
	HANDLER(pctx, add32);
	HANDLER(pctx, sub32);
	HANDLER(pctx, mul32);
	HANDLER(pctx, divz32);
	HANDLER(pctx, and32);
	HANDLER(pctx, or32);
	HANDLER(pctx, xor32);
	HANDLER(pctx, equal32);
	HANDLER(pctx, notequal32);
	HANDLER(pctx, modu32);
	HANDLER(pctx, divu32);
	HANDLER(pctx, less32);
	HANDLER(pctx, lesse32);
	HANDLER(pctx, greater32);
	HANDLER(pctx, greatere32);
	HANDLER(pctx, lessu32);
	HANDLER(pctx, lesseu32);
	HANDLER(pctx, greateru32);
	HANDLER(pctx, greatereu32);
	
	//unop
	HANDLER(pctx, boolnot);
	HANDLER(pctx, invert32);
	HANDLER(pctx, neg32);

	//special
	HANDLER(pctx, load32);
	HANDLER(pctx, store32);
	HANDLER(pctx, load8);
	HANDLER(pctx, store8);
	HANDLER(pctx, fload);
	HANDLER(pctx, fstore);
	HANDLER(pctx, print32);
	HANDLER(pctx, printchar);
	HANDLER(pctx, printptr);
	HANDLER(pctx, readchar);
	
	//cpointers
	HANDLER(pctx, loadcptr);
	HANDLER(pctx, storecptr);

#ifdef FLOAT
	HANDLER(pctx, printfloat);
	HANDLER(pctx, fadd);
	HANDLER(pctx, fsub);
	HANDLER(pctx, fmul);
	HANDLER(pctx, fdiv);
	HANDLER(pctx, fsqrt);
	HANDLER(pctx, fabs);
	HANDLER(pctx, fpow);	
	HANDLER(pctx, fneg);	
	
	HANDLER(pctx, flesse);
	HANDLER(pctx, fgreatere);
	HANDLER(pctx, fless);
	HANDLER(pctx, fgreater);
	
	HANDLER(pctx, fsin);
	HANDLER(pctx, fcos);
	HANDLER(pctx, ftan);
	HANDLER(pctx, fasin);
	HANDLER(pctx, facos);
	HANDLER(pctx, fatan);
	HANDLER(pctx, fatan2);
	
	HANDLER(pctx, int2real);
	HANDLER(pctx, real2int);
	
#endif
	
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
	//xprintf("group finished, no continue\n");
	return NULL;
}

tokenT* hcall (exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args (all after the head)
		
	
	//xprintf(" CALLING PROC %s\n", t->str);
	
	//xprintf("pre call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	zuint32 oldfp = ex->fp;  //save frame pointer

	if (t->sym->isPrototype){
			ERR("Function body missing:%s\n", t->str );
	}
	
	symbolT* selected = t->sym;

	if (t->sym->isSelector) {
 		xprintf(" CALL SELECTOR\n");
		typeT* seltable = ex->stack[ex->sp  - zvec_count(t->sym->type->members) + t->sym->selectorArg].typeselector;
		selected = zvec_get_at(seltable->selectors, t->sym->selectorNum); //grab the nth function from the selector table
	}

	ex->fp=ex->sp;
		
	/*
	//check if there are any tracked pointers to addref
	tokenT* tv = tsub(t);
	int count = zvec_count(t->sym->type->members);//number of args inside
	int i=0; //number of subs (should be the same)
	while(tv){ 
		if (tv->trackpossptr){
				xprintf(" Arg %d/%d derived from possessive pointer <%s> ", i, count, tv->str  );	
				xprintf(" addref %p +%d\n", ex->stack[ex->fp-count+i].as.ptr.block, ex->stack[ex->fp-count+i].as.ptr.offset);
				ram_addref( ex->stack[ex->fp-count+i].as.ptr.block );
		}
		
		tv = tnext(tv);
		i++;
	}
	*/
	
	//xprintf(" enter call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	void* oldlocal = ex->vars;   //take old local var data
	
#ifdef EXEDEBUG
	xprintf(" PROC %s has context size %d\n",t->str, selected->subctx->size);
#endif
	//printList(t->sym->tokens, NULL, 0, 0);
	ex->level++;	//going up a call frame
	
	ex->vars = ram_alloc(selected->subctx->size, NULL);

	//exe(ex, (tokenT*) t->sym->t->subs.head->next);
	//t->sym->t points to the token describing that function
	//head of that token's sub list is the tokens describing its data type (input parameters, etc)
	//the 'next' of that head is the first statement

	exe(ex, (tokenT*)tnext(tsub(selected->tokens)));
	
	//if (ex->stop==STOPFUNC){
	//	xprintf(" Caught RET\n");
	//}
	
	ex->stop=0; //stop return bubble-up
	
	//xprintf(" exit call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	

	/*
	if (t->sym->type->members)
		ex->fp -=  zvec_count(t->sym->type->members);  //subtract out all passed values
	*/
	
	//free any passed pointers that didn't get taken, or pointers that were addreffed
	
	tokenT* tv = tsub(t);
	
	if (t->sym->type->members) {
		int i;
		int count = zvec_count(t->sym->type->members);
		for (i=0;i<  count; i++) {
			typeT* m = zvec_get_at(t->sym->type->members, i);
			//xprintf(" ARG %d is ", i);
			//printType(m, ZTRUE, ZFALSE);

			if (m->ref->category == POINTERPOSSESSIVE) {
				//xprintf(" freeing poss pointer also has track %p\n", tv->trackpossptr);
				ram_free( ex->stack[ex->fp-count+i].as.ptr.block );
				
			} else if (tv->trackpossptr){
				//xprintf(" free trackpossptr\n");
				ram_free( ex->stack[ex->fp-count+i].as.ptr.block );
			}

			

			tv = tnext(tv);
			
		}
		ex->fp -= count;
	}
	
	
	if (t->sym->type->ref) {
		
				
		ex->stack[ex->fp++] = ex->stack[ex->sp-1];  
		
		if (t->sym->type->ref->category == POINTERUSER){
			ERR("Can't return non-possessive pointer!\n");
		}
		
		
#ifdef EXEDEBUG
		xprintf(" Function returns ");printType(t->sym->type->ref,1,1);
#endif
	} else {
		//xprintf(" Function returns nothing\n");
	}
	
	//print symbol table of call frame
	//free any possessive pointers still in variables
	
	clean_context_pointers(selected->subctx->symbols, ex->vars);
	ram_free(ex->vars);
	
	ex->sp = ex->fp;//put stack back to repositioned fp
	ex->fp=oldfp;  //put old frame pointer back
	ex->vars = oldlocal;//put old vars back
	ex->level--;
#ifdef EXEDEBUG		
	xprintf(" return to  caller complete SP:%d  FP:%d\n", ex->sp, ex->fp); 		
#endif
		
	//todo:handle return value, putting stack back together
	return tnext(t);
}

zbool struct_clean(void* v, typeT* ty){
	
	//xprintf(" Clean for %s\n", (ty)->name);
	//printType(ty, ZTRUE, ZFALSE);

	int i;
	for (i=0; i < zvec_count( (ty)->members); i++){
		typeT* member = zvec_get_at((ty)->members , i);
		//xprintf(" fields: %s  %d n", member->name, member->offset);
		if (member->ref->category == POINTERPOSSESSIVE){
			vptrT* vp = (void*) (member->offset  +  (char*)(v));
			ram_free( vp->block);
		}
		if (member->ref->category == STRUCT){
			struct_clean( member->offset  +  (char*)(v) , member->ref);
		}
		if ((member->ref->category == ARRAYSTATIC)&&(member->ref->ref->category==POINTERPOSSESSIVE)){
		
			//printf(" MEMBER is "); printType(member, ZTRUE,ZTRUE);
			//printf(" REF is "); printType(member->ref, ZTRUE,ZTRUE);
			//printf(" REFREF is "); printType(member->ref->ref, ZTRUE,ZTRUE);
			
			//member->ref is a static array type
			//member->ref->ref is the type of the array element
			
			free_array_of_possessive_pointers( member->offset + (char*)(v), member->ref->len, member->ref->ref->ref->category == VIRTUAL);
			
			
			
			
						
		}
	}
		
	return ZTRUE;
}



zbool struct_destructor(void* v){
	typeT** ty = ram_shadow(v); //get the type
		
	if (!ty)
		xprintf("Runtime issue: No Shadow on block %p\n", v);
	else if (*ty){
		//printf(" Destruct for %s\n", (*ty)->name);
		xprintf(" Destruct for %s\n", (*ty)->name);
		return struct_clean( v, *ty);
	}
	else
		xprintf(" NULL type on struct shadow %p\n", v);
		
	return ZTRUE;
}
zbool ptr_array_destructor(void* va);

tokenT* halloc(exectxT* ex, tokenT* t) {
	
	
	size_t size =   t->ty->ref->size;
#ifdef EXEDEBUG
	xprintf(" ALLOC %d for ", size);
	printType( t->ty->ref, ZTRUE, ZTRUE);
#endif
	ex->stack[ex->sp].as.ptr.level=0;
	ex->stack[ex->sp].as.ptr.offset=0;
	
	if ( t->ty->ref->members){  //structs need destructors optimization TODO: structures that don't have possessive pointer don't actually need destructirs
		ex->stack[ex->sp].as.ptr.block = ram_alloc_shadow( size ,struct_destructor, sizeof(typeT*)  ); //struct needs type pointer
		typeT** typtr = ram_shadow(ex->stack[ex->sp].as.ptr.block); //get the shadow
		if (typtr)
			*typtr = t->ty->ref;
	} else {
		ex->stack[ex->sp].as.ptr.block = ram_alloc(size, NULL  ); //simple types with no members need no destructor
	}
	
	
	(ex->sp)++;
		
	return tnext(t);
}


void free_array_of_possessive_pointers(void* v, zuint32 len, zbool virtual){

	if (!virtual) {
		vptrT* vprs = v;
		int j;
		for (j = 0; j < len; j++)
			ram_free(vprs[j].block);
	}
	else {
		vptrselectorT* vprs = v;
		int j;
		for (j = 0; j < len; j++)
			ram_free(vprs[j].ptr.block);
	}
}


zbool ptr_array_destructor(void* va){
	//If va is a zarray of pointers, it is iterated and freed
	typeT* ty = zarray_get_meta(va);
	
	//xprintf(" Clean for array ");
	//printType(ty, ZTRUE, ZFALSE);
	
	
	if (ty && ty->category != ARRAYDYNAMIC){
		ERR(" attempt to array destruct something that isn't a dynamic array\n");
		
	}
	
	ty = ty->ref;  //get the base type of the array
		
	//xprintf(" THIS ARRAY IS %d out of %d of type (cat %x) \n", zarray_count(va), zarray_size(va), ty->category );
	//printType( ty, ZTRUE,ZTRUE);
	//xprintf("\n");
	
	if (ty->category == POINTERPOSSESSIVE){
		int i;

		if (ty->ref->category == VIRTUAL) {

			vptrselectorT* pv = va;
			//xprintf("To free each pointer.  size of an array element is %d\n", ty->size);
			for (i = 0; i < zarray_size(va); i++) {  //for now whole array, not just using 'count'
				ram_free(pv[i].ptr.block);
			}

		}
		else {

			vptrT* pv = va;
			//xprintf("To free each pointer.  size of an array element is %d\n", ty->size);
			for (i = 0; i < zarray_size(va); i++) {  //for now whole array, not just using 'count'
				ram_free(pv[i].block);
			}

		}
		
		
	} else if (ty->category == STRUCT){
		
		int i;
		char* vc = va;
		//xprintf(" To clean each struct element, size of each is %d\n", ty->size);
		
		
		for (i=0;i<zarray_size(va);i++){  //for now whole array, not just using 'count'
			struct_clean(  (void*) vc, ty); //clean one of these
			vc += ty->size;
		}
		
		
	}
	
	
	return ZTRUE;
}

tokenT* hallocarray(exectxT* ex, tokenT* t) {
	
	
	//xprintf("  exe to alloc \n");
	//printList(tsub(t), t, 0,0);
	
	exe(ex, tsub(t));
	
		
	
	
	//TODO: allow destructors for alloced structs
	size_t size =   t->ty->ref->size;
	
	ex->sp--;
	//size *= ex->stack[ex->sp].as.z32;
#ifdef EXEDEBUG
	xprintf(" ALLOC %d for ", size);
	printType( t->ty->ref, ZTRUE, ZTRUE);

#endif
	
	
	ex->stack[ex->sp] .as.ptr.level=0;
	ex->stack[ex->sp] .as.ptr.offset=0;
	//ex->stack[(ex->sp)++].as.ptr.block = ram_alloc( size ,NULL  );

	int arraycount = ex->stack[ex->sp].as.z32;
	ex->stack[ex->sp].as.ptr.block = zarray_allocf(size, arraycount, ptr_array_destructor, __FILE__, __LINE__ );

	if (t->val.as.z32 == 1) {
		zarray_use(ex->stack[ex->sp].as.ptr.block, arraycount);  //say all the elements are in use
	}

	//xprintf(" Created array %d count  %d size\n", zarray_count(ex->stack[ex->sp].as.ptr.block), zarray_size(ex->stack[ex->sp].as.ptr.block));

	zarray_set_meta( ex->stack[ex->sp].as.ptr.block, t->ty->ref);
	(ex->sp)++;
	//exit(1);
	return tnext(t);
}

tokenT* hfree(exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args
	ex->sp--;
#ifdef EXEDEBUG	
	xprintf(" TO TRASH %p\n", ex->stack[ex->sp].as.ptr.block );
#endif
	ram_free(ex->stack[ex->sp].as.ptr.block);
	ex->stack[ex->sp].as.ptr.block=0;
	return tnext(t);
}

tokenT* haddref(exectxT* ex, tokenT* t) {
	exe(ex, tsub(t)); //evaluate all the args
	
#ifdef EXEDEBUG
	xprintf(" TO KEEP %p\n", ex->stack[ex->sp-1].as.ptr.block );
#endif
	//this also does not pop off the stack, the value stays on
	
	ram_addref(ex->stack[ex->sp-1].as.ptr.block);  //retention is on the BLOCK.  Means you can addref PART of a block... the whole block will be kept and waste memory, but this is OK for now; it will eventually be freed.  
	
	return tnext(t);
}




void start(parsectxT* pctx, tokenT* t){
	exectxT* exectx = ram_alloc(sizeof(exectxT), NULL);
	exectx->stack = ram_alloc( sizeof(valueT)*100, NULL);

	exectx->sp = 0;
#ifdef EXEDEBUG	
	xprintf(" PCTX %d bytes\n", pctx->size);
#endif	
	exectx->globalvars = ram_alloc(pctx->size , NULL);
#ifdef EXEDEBUG	
	xprintf("EXE\n");
#endif	
	/*
	for (int i=0;i<32;i++){
			xprintf(",%02x ",  exectx->globalvars[i]  );
			
	}
	xprintf(" \n");*/
	 
	exe(exectx, t);
	
	ram_free(exectx->stack);

	clean_context_pointers(pctx->symbols, exectx->globalvars);
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
 * Pointers  Z32&  [Z32]&
 * Functions (a:Z32; b:Z32 -> Z32)
 * Arrays [10 Z32]    */
tokenT*  parseType(tokenT* t) {

	char* count=NULL;
	zbool named=ZFALSE;
	tokenT* next=NULL;
	int iscptr = 0;

	if (t->tok == KCPOINTER) {
		t = tnext(t);
		iscptr = 1;
	}
	/*else if (t->tok == KCDATA) {
		t = tnext(t);
		iscptr = 2;
	}*/

	//xprintf("PTstart\n");
	for ( ; t;  t = next ) {

		//xprintf("PT %s\n", t->str);
		
		if (t->tok == '[' && !named){ //array type
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
			int icount = count? atoi(count) : 0;
			if (icount)
				S->ty = findType(ARRAYSTATIC, tprev(t)->ty, NULL, icount);	
			else
				S->ty = findType(ARRAYDYNAMIC, tprev(t)->ty, NULL, 0);	
			
			
			
			next = tnext(t);
			lfold(S,next); //everything after opening bracket to closing bracked is folded under the open
			named = ZTRUE; //array type [Z32], counts as a type name 
			continue;
			
		}

	



		if (!named && t->tok == NAME){ //simple typename, but only 1 per 'type'
					       //xprintf(" type %s\n", t->str);
					       //t->str = zstrcat( t->str, "$type" );
			
			named=ZTRUE;
			t->ty = findType(NAMED, NULL, t->str, 0);
			if (!t->ty){
				if (iscptr == 1)
					t->ty = mkType(CPOINTER, NULL, t->str, 0);
				else if (iscptr == 2)
					t->ty = mkType(CDATA, NULL, t->str, 0);
				else
					t->ty = mkType(PENDING, NULL, t->str, 0);
			}
			next = zlist_next(t);
			continue;
		}
		if (t->tok == '&'){ //pointer type
			t->ty = findType( POINTERUSER, tprev(t)->ty, NULL,0);
			next = zlist_next(t);
			fold(tprev(t),t);
			continue;
		}
		if (t->tok == '%'){ //pointer type
			t->ty = findType( POINTERPOSSESSIVE, tprev(t)->ty, NULL,0);
			next = zlist_next(t);
			fold(tprev(t),t);
			continue;
		}
		
		
		if (t->tok =='(' && !named){ //type list for function parameters & return value
			tokenT* S = t;

			typeT* ty = mkType( PENDING, NULL, NULL, 0); 

			t = parseTypeList(tnext(t), ty);

			if (t->tok !=')'){
				ERR("Expected )\n");
			}
			
			S->ty = findType( FUNCTION, ty, NULL, 0); //either adds this function to the type list, or returns the version already existing
			//xprintf(" finding function type %d %d->\n", ty->tid, S->ty->tid);

			named=ZTRUE;
			next = tnext(t);
			lfold(S,next);
			continue;
		}
		break;
	}
	
	
	//xprintf("PTend\n");


	return t;
}

/*parseTypeList parses both function parameter lists  a:Z32; b:Z32;, etc or type structure definitions, which are intentionally the same syntax */
tokenT*  parseTypeList(tokenT* t, typeT* parent) {
	tokenT* next=NULL;
	if (parent && !parent->members){
		parent->members = zvec_mk(NULL, 10);
		zvec_disown(parent->members);
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

		
		if ( (t->tok == PAIR('-','>'))){
			//is a function
			parent->category = FUNCTION;
			reqname = ZFALSE; //no longer need names (returned values are anonymous)
			next = tnext(t);
			continue;

		}
		
		if ((t->tok == '@') && (tnext(t)->tok == NAME)) {
			offset = getCSize(tnext(t)->str);
			
			t = tnext(tnext(t));
		}
		
		t = parseVar(t, &name, &type  );
		//printList((tokenT*)t->zlistnode.prev->prev->prev, t, ENDFILE,0);
		if ( (reqname && !name) || !type){
 			ERR(" type or name missing for struct member or function arg\n");
		} 
		 
		if (type->category > LAST_REAL_TYPE){
			ERR("Name '%s' category %d cannot be in struct/fcall\n",  safestr(type->name),   type->category);
		}

		if (parent && parent->category == FUNCTION){
			if (parent->ref){
				ERR("Functions can only return 1 value\n");
			} else {
				parent->ref= type ;
			}
		} else	if (parent && parent->members) {
			typeT* mty = mkType(MEMBER, type, name, offset);
			
			if (t->tok == KTRASH) {
				//todo: only applicable to primitive parameter lists, but at this part of the code, we don't know that's what we are doing.
				t = tnext(t);
				ram_free(tremove(tprev(t)));
				mty->trashAfterPrimitive = 1;
			}

			zvec_add(parent->members, mty);
		}

		offset += (zuint32) (type->size);  //TODO:  alignment
	
		if (t->tok == ';'  ){
			next = tnext(t);
			continue; //list item seperator
		}
		
		if ( (t->tok == PAIR('-','>'))){
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
		
		if (t->str && t->tok != KWORDS)  //TODO: check this line, might be wrong... why KWORDS here?
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
		xprintf(" parsetype returned type ");
		printType(tprev(t)->ty, ZTRUE, ZFALSE);
	}*/

	if (name)
		lfold(S,t);
	
	return t;

}



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

void checkUsage(tokenT* start, tokenT* end){
	
	tokenT* t = start;
				
	while (t && t != end){
	
		if (t->ty){
			
			printList(t, t, -1,1);
			
			printType(t->ty, ZFALSE, ZFALSE);
			xprintf(" not used \n");
			
		}
		t = tnext(t);
		
	}
	
	
}
//checks that real_type implements all the selectors of vtype
void check_implementation(typeT* vtype, typeT* real_type, typeT* real_type_vmember) {

	if (real_type_vmember->selectors)
		return; //already found all the functions for it
	
	real_type_vmember->selectors = zvec_mk(NULL, zvec_count(vtype->selectors)); 
	zvec_disown(real_type_vmember->selectors);

	int n;
	for (n = 0; n < zvec_count(vtype->selectors); n++) {
		symbolT* vselector = zvec_get_at(vtype->selectors, n);
		xprintf(" Need to find member %s implementation of %s ",
			real_type->name,
			vselector->name);
		xprintf(" subst arg %d\n", vselector->selectorArg);
		printType(vselector->type, ZTRUE, ZFALSE);
		int arg;
		zvecT* v = zvec_mk(NULL, zvec_count(vselector->type->members));
		zvec_disown(v);
		for (arg = 0; arg < zvec_count(vselector->type->members); arg++) {
			typeT* argmember = zvec_get_at(vselector->type->members, arg); //get the arg'th member
			typeT* argtype = argmember->ref;	//get the type the member (arg) references
			if (arg == vselector->selectorArg) {
				if (argtype->category != POINTERUSER && argtype->category != POINTERPOSSESSIVE)
					ERR("Only can select from pointer type\n");
				typeT* subst = findType(argtype->category, real_type, NULL, 0);
				zvec_add(v, subst);//replace virtual type with the real type
			}
			else
				zvec_add(v, argtype);	//add the regular type
		}

		//selectors are at the global level. find function with that name and has the above arguments (v)... which is the same as the virtual type's selector, but with the argument type switched
		symbolT* s = findSymbol(global->symbols, vselector->name, v);
		ram_free(v);
		if (s) {
			xprintf(" Found symbol %s ", s->name);
			printType(s->type, ZTRUE, ZFALSE);
			if (s->type->ref != vselector->type->ref) {
				printf("Return type mismatch for selector Got:  ");
				printType(s->type->ref, ZTRUE, ZFALSE);
				printf("\n Expected ");
				printType(vselector->type->ref, ZTRUE, ZFALSE);
				ERR("");
			}

			zvec_add(real_type_vmember->selectors, s); //add the found function to the real type's virtual field for the virtual type

		}
		else {
			printf("Type %s does not support selector %s", real_type->name, vselector->name);
			printType(vselector->type, ZTRUE, ZFALSE);
			ERR("");
		}

	}



}

typedef struct {
	char* name;
	int val;
} csizeT;

zvecT* csizes = NULL;

zbool cs_cleanup(void* v) {
	csizeT* cs = v;
	ram_free(cs->name);
	return ZTRUE;
}

void addCSize(char* name, size_t size) {

	if (!csizes)
		csizes = zvec_mk(NULL, 16);

	csizeT* cs = ram_alloc(sizeof(*cs), cs_cleanup);

	cs->name = zstrdup(name);
	cs->val = (int)size;

	zvec_add_or_free(csizes, cs);

}

int getCSize(char* name) {

	int i;
	
	for (i = 0; i < zvec_count(csizes); i++) {
		csizeT* cs = zvec_get_at(csizes, i);
		if (!strcmp(cs->name, name))
			return cs->val;
	}

	ERR("Unknown csize: %s\n", name);
	return 0;

}


tokenT*  parse(parsectxT* pc, tokenT* t) {
	
	
	tokenT* ts=NULL;
	symbolT* s= NULL;
	zvecT* v=NULL;xprintf(" Token %s ", t->str);
	instruction handler = NULL;
	char* name2 = NULL;
	tokenT* tfirst = t;
	
	//xprintf(" START PARSE:\n");
	//printList( t, t, 0,0);
	
	int local=0;//true when a found symbol is from the local context
	int csize = 0;
	
	//xprintf(" PARSE IN SYM %s %p\n", t->sym? t->sym->name : "none", t->sym? t->sym->type : NULL);
	//if (t->sym)
	//	printType(t->sym->type, 1,1);
	
	while ( t) {
		if (t->tok==ENDFILE)
			t->handler = hbreakblock; //don't really do anything

		if (t->tok==ENDFILE || t->tok== PASTENDFILE)
			return t;
			
		//printList( tprev(tprprintf(" Token %s ", t->str);ev(tprev(t))), t, PASTENDFILE,0);
		//xprintf(" parse token %s\n", t->str);
		//getc(stdin);
		char* name=NULL;
		char* pname=NULL;
		typeT* type=NULL;
		handler = NULL;
		
		ts = t;
		if (parseDebugFlag)
			boo();

		switch (t->tok) {

		case '!':
			parseDebugFlag = 1;
			t = tnext(t);
			tremove(tprev(t));
			continue;



		case KEND:

			if (pc->endable) {
				pc->endable--;
				//xprintf(" 'end' block \n");
				return t;
			}

			ERR(" Cannot 'end' in the global context\n");

		case KCDATA:
		case KCPOINTER:
		case KVIRTUAL:

			csize = 0;
			if (tnext(t)->tok == '@' && tnext(tnext(t))->tok == NAME) {
				t = tnext(tnext(t));
				csize = getCSize(t->str);
			}


			t = tnext(t);

			//create a 'virtual' type (AKA an interface)
			//TODO:  proc selector prototypes:   selector arg1 Fun:(arg0:Z32;arg1:MyVirtualType& ->);
			//the above should add 'Fun' proc to the virtual type's list of supported functions
			//When calling a virtual proc, The 'selector' argument the one whose method table is searched
			if ((t->tok == NAME) && (t->str) && (tnext(t)->tok == ';')) {

				if (ts->tok == KVIRTUAL)
					mkType(VIRTUAL, NULL, t->str, 0);
				else if (ts->tok == KCDATA)
					mkType(CDATA, NULL, t->str, 0); //CDATA has  no size AND  possessive/user pointers to Cdata are vptrs like any other
				else
					mkType(CPOINTER, NULL, t->str, sizeof(void*));
			}
			else
				ERR(" Expected virtual/cpointer NAME, then ';'\n");

			t = tnext(tnext(t)); //skip over name and semicolon
			lfold(ts, t);
			ts->handler = hnop;
			continue;


			//




		case KSELECTOR:		//proc selector
		case KPRIMITIVE:	//primitive declaration

			xprintf(" Alias is %s\n", tnext(t)->str);
			name2 = ram_addref(tnext(t)->str);


			typeT* type = NULL;

			ram_free(tremove(tnext(t))); //remove '/'


			//fall through to var/proc decl
		case KVAR:		//variable declaration
		case KPROC:		//proc body definition
		case KPROTO:		//proc prototype


			if (ts->tok == KPRIMITIVE) {

				s = findSymbol(global->symbols, name2, NULL);
				if (s)
					handler = s->handler;
				else
					ERR("No primitive named %s\n", name2);



			} 
			name = tnext(t)->str;

			t = parseVar(tnext(t), &name, &type); //parse variable; name is required

			xprintf("proc/var %s   %s is type ", ts->str, name);
			printType(type, 1, 1);
			if (!name) {
				ERR("Expected name and ':'\n");
			}

			if (type->category == PENDING) {
				ERR("Cannot create 'pending' type variable... unknown size\n");
			}

			s = NULL;


			if (ts->tok == KPROC) {
				xprintf(" Try to find exact symbol\n");
				int i;
				symbolT* ss;

				for (i = 0; i < zvec_count(global->symbols); i++) {
					ss = zvec_get_x_at(global->symbols, symbolT*, i);

					if (!ss->isPrototype)
						continue;

					if (!strcmp(name, ss->name)) {
						//same name

						//need to compare type with proto
						if (ss->type == type) {
							xprintf(" Found type match\n");
							ss->isPrototype = 0;
							s = ss;
							break;
						}

					}
				}

			}

			if (!s)
				s = mkSymbol(pc, name, type, handler);

			if (ts->tok == KPROTO) {
				s->isPrototype = 1;
				s->handler = hcall;	//will eventually be a function call
			}

			if (ts->tok == KSELECTOR) {
				s->isSelector = 1;
				s->handler = hcall;	//will eventually be a function call
				//find which argument is the selector type
				int i;
				for (i = 0; i < zvec_count(s->type->members); i++) {
					typeT* arg = zvec_get_at(s->type->members, i);
					if (!strcmp(arg->name, name2)) {
						xprintf("Selector for function %s is on arg %d %s of type ", s->name, i, arg->name);

						typeT* pt = arg->ref;  //should be a pointer to a virtual type
						if ((pt->category != POINTERUSER) && (pt->category != POINTERPOSSESSIVE)) {
							ERR(" Virtual types can only by passed by pointer\n");
						}

						if (!pt->ref->selectors) {
							pt->ref->selectors = zvec_mk(NULL, 4);
							zvec_disown(pt->ref->selectors);
						}
						s->selectorNum = zvec_count(pt->ref->selectors); //track which selector this is
						zvec_add(pt->ref->selectors, s);
						s->selectorArg = i;
						printType(pt->ref, ZTRUE, ZTRUE);

						break;
					}
				}
				if (i == zvec_count(s->type->members))
					ERR("No such arg %s\n", name2);
			}

			if (ts->tok == KPROC)
				s->alias = name2;
			else
				ram_free(name2);

			name2 = NULL;

			if (ts->tok == KPROC) {
				//procedures go into a body of statements
				s->subctx = mkcontext();
				s->subctx->parent = pc;
				s->subctx->endable++; //its a subcontext
				s->subctx->type = s->type;
				t->sym = s;
				s->handler = hcall;  //need to set handler before parsing, in case of recursion
				t = parse(s->subctx, t);

				checkUsage(ts, t);//check all values are used up
				//t should now be 'end' 
			}
			else if (t->tok != ';') {
				ERR(" missing ;\n");
			}

			t = tnext(t); //skip past semicolon (or 'end')
			lfold(ts, t);  //everything up to an including semicolon folded
			if (!s->isPrototype)
				s->tokens = ram_addref(ts); //symbol has this tokenstream
			//ram_free(tremove(ts)); //remove from the executable token list 
			ts->handler = hnop;
			continue;

		case '#':	//create variable of whaatever type is on the stack, and store 
			t = tnext(t); //is variable name

			if (pc == global)
				handler = hglobal;
			else
				handler = hlocal;

			s = mkSymbol(pc, t->str, tprev(ts)->ty, handler);
			//don't do t=tnext(t). This way t, which contains the name of the var,
			//will be parsed again, and that will make a 'load' token.
			//so insert a token after this so that it makes it a store
			zlist_insert_node_after(t, mkToken('=', "=", 0));
			fold(ts, t);
			continue;
		case KRETURN:

			//todo: check return type
			//allow return no value

			t->handler = hreturn;
			if (pc->type && pc->type->ref) {
				xprintf(" RETURN a value\n");
				printType(pc->type->ref, ZTRUE, ZFALSE);

				if (tprev(t)->ty != pc->type->ref) {
					xprintf("Type mismatch expected:\n");
					printType(pc->type->ref, ZTRUE, ZTRUE);
					xprintf("attempt to return: \n");
					printType(tprev(t)->ty, ZTRUE, ZTRUE);
					ERR("TYPE MISMATCH\n");
				}

				fold(tprev(t), t);
			}
			else {
				xprintf(" RETURN no value\n");
			}

			t = tnext(t);
			continue;

		case KBREAK:
			t->handler = hbreakloop;
			t = tnext(t);
			continue;

		case KLOOP:
			ts = t;
			pc->endable++;
			t = parse(pc, tnext(t));
			t = tnext(t);

			ram_free(tremove(tprev(t)));//remove 'end'

			checkUsage(ts, t);//check all values are used up

			lfold(ts, t);
			ts->handler = hloop;

			continue;

		case KELSEIF:
			fold(tprev(t), t);//take previous node as the condition
			t->handler = hcondblock;
			t = tnext(t);
			continue;

		case KELSE:
			t->handler = hgroup;
			t = tnext(t);
			continue;

		case KIF:
			ts = t;
			ts->handler = hgroup;
			ts->val.as.n32 = 1;  //continue after group

			tokenT* cond;
			insert_after(tprev(t), cond = mkToken(COND, "cond", 4));
			fold(tprev(tprev(t)), tprev(t)); //condition statrment inside 'cond' wrapper
			fold(tprev(t), t);  //put condition node inside if
			cond->handler = hcondblock;
			pc->endable++;
			//xprintf(" START PARSE IF\n");
			t = parse(pc, tnext(t));  //continue parsing until 'end'
			//xprintf(" PARSED TO END\n");
			tokenT* end = t;

			checkUsage(ts, t);//check all values are used up

			t = tnext(t);
			lfold(ts, t);

			//scan
			tokenT* cs = cond;
			tokenT* ls = cs;
			for (ls = cs; ls; ls = tnext(ls)) {

				//xprintf("ls %p	%s\t",ls, ls->str);
				//xprintf("cs %p	%s\n",cs, cs->str);
				if ((ls->tok == KEND) || (ls->tok == KELSEIF) || (ls->tok == KELSE)) {


					if (ls->tok == KEND)
						ls->handler = hnop; //nothing

					lfold(cs, ls);

					cs = ls;
				}

			}


			continue;



		case KTYPE:
			csize = 0;

			if (tnext(t)->tok == '@' && tnext(tnext(t))->tok == NAME) {
				t = tnext(tnext(t));
				csize = getCSize(t->str);


			}


			if (tnext(t)->tok == NAME)
				name = tnext(t)->str;
			else {
				ERR("expected type name\n");
			}

			ts->handler = hnop;

			typeT* ty = findType(NAMED, NULL, name, 0); //find a type by name

			if (ty && ty->category != PENDING) {
				ERR("redefining type %s\n", name);
			}

			if (!ty)
				ty = mkType(PENDING, NULL, name, 0);  //create a pending type (so it can be referenced by pointer, but not directly yet)

			//check if adding any +
		//	while (tnext(tnext(t))->tok == '+') {
			//	printf(" Add virtual functions for %s\n", tnext(tnext(tnext(t)))->str);
				//t = tnext(tnext(t)); //skip over 2 for + and virtualname
				//todo: add function table members for the struct, not counted as part of the struct's normal size
			//}

			t = parseTypeList(tnext(tnext(t)), ty);

			ty->category = STRUCT; //its a real struct now

			if (csize) {
				ty->size = csize;
			}

			if (t->tok == KEND) {
				t = tnext(t);
				lfold(ts, t); //includes 'end' in the fold
				continue;	
			}

			ERR(" Expected 'end' for type\n");

			continue;

		case NUMBER:

			if (strchr(t->str, '.')) {  //decimal point makes it a float
#ifdef FLOAT
				FLOAT n = atof(t->str);
				t->val.as.f = n;
				t->ty = tReal;
				t->handler = hconstant;
				//xprintf(" set handler for %s to %p\n", t->str, t->handler);
				t = tnext(t);
				continue;
#else
				ERR("Floating point not enabled\n");
#endif
			}

			zuint32 n = atoi(t->str);
			t->val.as.z32 = n;
			t->ty = tZ32;
			t->handler = hconstant;
			//xprintf(" set handler for %s to %p\n", t->str, t->handler);
			t = tnext(t);
			continue;
		case LITERAL: //string literal (byte array)
			t->ty = findType(POINTERUSER, tString, NULL, 0); //findType(ARRAYDYNAMIC, tN8, NULL, 0);
			t->handler = hconstant;
			t->val.as.ptr.block = zstrndup(t->str + 1, strlen(t->str) - 2); //t->str already a zstring
			t->val.as.ptr.offset = 0;
			t->val_to_free = ZTRUE;
			t = tnext(t);
			continue;

		case KINCLUDE:
			xprintf("lit? %x %x\n", tprev(t)->tok, LITERAL);
			if (tprev(t)->tok == LITERAL) {
				char* strfile = ram_loadstr(tprev(t)->val.as.ptr.block);
				if (strfile) {

					tokenize(t, strfile);
					ram_free(strfile);
					t = tnext(t);
					ram_free(tremove(tprev(ts)));  //remove 'include'
					ram_free(tremove(ts)); //remove string literal
					continue;
				}
				else {
					ERR("cannot open %s\n", tprev(t)->val.as.ptr.block);
				}
			}
			break;

		case ':': //typecast


			ts = t; //ts is colon

			

			t = parseType(tnext(t));



			printList(tprev(ts), t, -5, 2);
			//special case cast to virtual pointer type
			ty = tnext(ts)->ty;


		



			if (ty
				&& ((ty->category == POINTERUSER) || (ty->category == POINTERPOSSESSIVE))
				&& ty->ref && (ty->ref->category == VIRTUAL)) {

				//check that this type supports this virtual type
				typeT* from = tprev(ts)->ty;
				if ((from->category == ty->category) && from->ref) {

					int j;

					for (j = 0; j < zvec_count(from->ref->members); j++) {
						typeT* t2 = zvec_get_at(from->ref->members, j);  //look at the type's members (t2 is the memer; t2->ref is thr type of the member)
						if (ty->ref && t2->ref && (ty->ref->tid == t2->ref->tid)) { //check the type the member refers to to the virtual type we are casting to
							xprintf(" Type %s supports virtual %s\n", from->ref->name, t2->name);
							//need to make sure all of ty->ref's selectors are 1)implemented on t2->ref AND are in t2's selector list

							check_implementation(ty->ref, from->ref, t2);

							ram_free(tremove(tnext(ts)));
							ts->handler = haddselector;	//push the set of selectors
							ts->val.as.type = t2;				//TODO: put the selectors on a seperate stack
							ts->ty = ty;
							fold(tprev(ts), ts);
							printList(tprev(ts), ts, -5, 3);
							break;

						}
					}
					if (j == zvec_count(from->ref->members)) {
						ERR("Not supported\n");
					}
					continue;

				}
				else
					ERR(" only pointers can be casted to virtual\n");


			}

			tprev(ts)->tyorig = tprev(ts)->ty;
			tprev(ts)->ty = tprev(t)->ty;

			ram_free(tremove(tnext(ts)));
			ram_free(tremove(ts));

			printList(tprev(tprev(t)), t, -5, 3);
			continue;

		case KSIZE:
		case KCOUNT:
		case KSETCOUNT:
			//tprev(ts)->ty should be a pointer to an arrray

			if (
				tprev(ts)->ty &&
				tprev(ts)->ty->category == POINTERUSER &&
				(tprev(ts)->ty->ref->category == ARRAYDYNAMIC  //TODO: add case for String to be treated like [N8]&
				 || tprev(ts)->ty->ref == tString)
			){

				ts->handler = harrayinfo;

				if (ts->tok == KSETCOUNT) {
					ts->val.as.n32 = 2;
					fold(tprev(tprev(ts)), t);
				}
				else {

					if (ts->tok == KSIZE)
						ts->val.as.n32 = 0;
					else if (ts->tok == KCOUNT)
						ts->val.as.n32 = 1;


					fold(tprev(ts), t);
				}

				ts->ty = tZ32;

				t = tnext(t);
				continue;
			}

			break; //maybe 'size' is also a symbol defined otherwise

		

		case PAIR('[', ']'):
			//array indexing


			if ((tprev(t)->ty != tZ32) && (tprev(t)->ty != tN32)) {
				ERR("Array index must be Z32 or N32\n");
			}

			//todo: check its an integer, and type is an array
			//array accesshload32

			ts = tprev(tprev(t));
			fold(ts, t);
			//ts->ty is pointer to array  (sizeof ptr)
			//ts->ty->ref is array of something (sizeof the array)
			//ts->ty->ref->ref is the element type 

			printType(ts->ty, 0, 0);


			t->handler = hindex;

			t->trackpossptr = ts->trackpossptr;  //if was tracking from a possessive pointer, still track this pointer is derived from that

			if (
				((ts->ty->category == POINTERUSER) || (ts->ty->category == POINTERPOSSESSIVE))
				&&
				(ts->ty->ref == tString)
				) {
				t->ty = findType(POINTERUSER, tN8, NULL, 0); //special case: index into a string is treated as index into byte array
				t->val.as.n32 = 1;

			}
			else {

				t->ty = findType(POINTERUSER, ts->ty->ref->ref, NULL, 0);
				t->val.as.n32 = ts->ty->ref->ref->size;

			}

			//xprintf(" array element size %d\n", t->val.as.n32);

			insert_after(t, mkToken('@', "@", 1)); //read the item ( The next parsed token, might remove this, if it wants to store or manipulate the pointer)

			t = tnext(t);
			continue;


		case '@':

			//if next token is &, remove both (cancels to just leave the pointer)
			//varname  puts varname
			if (tnext(t)->tok == '&') {
				t = tnext(t);
				ram_free(tremove(ts));
				ts = t;
				t = tnext(t);
				ram_free(tremove(ts));
				continue;
			}

			//if next token is assignment, remove the '@' token
			switch (tnext(t)->tok) {



			case '=':
				t = tnext(t);
				ram_free(tremove(ts));
				continue;

			}

			if (tnext(t)->str && tnext(t)->str[0] == '.'
				&& tprev(t)->ty
				&& tprev(t)->ty->ref
				&& tprev(t)->ty->ref->category == STRUCT 
				&& tprev(t)->tok != STACKARG
				) {
				
				//access struct member:  
				//remove the @ token, since accessing the struct member is just pointer addition
				t = tnext(t);
				ram_free(tremove(ts));
				continue;
			}

			//read function arguments from the stack (Note: function args are READONLY...)
			if (tprev(t)->tok == STACKARG) {

				t->ty = tprev(t)->ty;
				tprev(t)->ty = NULL;

				fold(tprev(t), t);
				t->handler = hstackread;

				if (t->ty->category == POINTERPOSSESSIVE) {

					if (tnext(t)->tok == KTAKE) {
						t->val.as.n32 = 1;	//flag 1 here means zero out the source pointer (taking) 

						ram_free(tremove(tnext(t)));

					}
					else if (tnext(t)->tok != KKEEP) {

						t->val.as.n32 = 2;	//flag 2 here means make a level+1 pointer; meaning it can be used or passed, but not stored in the current frame or returned

						//return nonpossessive form of same pointer
						t->ty = findType(POINTERUSER, t->ty->ref, NULL, 0);
					}
					else {
						ERR("unhandled possessive case");
					}

				}
				t = tnext(t);
				continue;
			}

			//user pointer to user pointer
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == POINTERUSER)) {
				t->ty = tprev(t)->ty->ref;

				if (tprev(t)->ty->ref->ref->category == VIRTUAL)
					t->val.as.n32 = 8; //load virtual part too



				fold(tprev(t), t);
				t->handler = hloadptr;
				t = tnext(t);
				continue;
			}

			//user pointer to possessive pointer
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == POINTERPOSSESSIVE)) {

				if (tprev(t)->ty->ref->ref->category == VIRTUAL)
					t->val.as.n32 = 8; //load virtual part too


				if ((tnext(t)->tok == KKEEP) || (tnext(t)->tok == KTAKE)) {
					t->ty = tprev(t)->ty->ref; //return possessive pointer (since it is being addreffed or taken)

				}
				else {
					//return nonpossessive form of same pointer
					t->ty = findType(POINTERUSER, tprev(t)->ty->ref->ref, NULL, 0);

					t->trackpossptr = t; //have the nonpossessive pointer and all their derivatives track this
				}

				fold(tprev(t), t);

				if (tnext(t)->tok == KTAKE) {
					ram_free(tremove(tnext(t)));
					t->handler = htakeptr;
				}
				else {
					t->handler = hloadptr;

					if (tnext(t)->tok != KKEEP)
						t->val.as.n32 |= 1; //flag that raises the pointer level so that this can't be stored locally... must be passed to a function
				}
				t = tnext(t);
				continue;

			}


			//user pointer to cpointer
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == CPOINTER)) {
			
				t->ty = tprev(t)->ty->ref;

			
				fold(tprev(t), t);
				t->handler = hloadcptr;
				t = tnext(t);
				continue;
			}


			//other '@' cases that aren't handled where are done via primitive handlers
			break;

		case '=':  //try to handle storing ptr to ptr.  Top of stack has a pointer to the pointer var
			if (parseDebugFlag)
				boo();

			//handle     @= case.... if '@' a pointer to get a variable, and store to the variable...
			//  pointervar =         //writes a pointer to a pointer variable
			// The pointer variable is represented by a pointer to some kind of pointer
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == POINTERUSER)) {
				if (tprev(tprev(t))->ty && tprev(tprev(t))->ty->category == POINTERUSER) {
					//xprintf("general pointer to pointer store\n");

					if (tprev(tprev(t))->ty->ref->category == VIRTUAL)
						t->val.as.n32 = 8; //save virtual part too

					fold(tprev(tprev(t)), t);
					t->handler = hstoreptr;

					//TODO check level

					t = tnext(t);
					continue;
				}
			}

			//stpre a possessive pointer in a possessive pointer variable (which is represented by a user pointer to a possessive pointer)
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == POINTERPOSSESSIVE)) {
				if (tprev(tprev(t))->ty && tprev(tprev(t))->ty->category == POINTERPOSSESSIVE) {

					if (tprev(tprev(t))->ty->ref->category == VIRTUAL)
						t->val.as.n32 = 8; //save virtual part too


					fold(tprev(tprev(t)), t);
					t->handler = hstoreptr;
					t->val.as.n32 |= 1; //flag to free the pointer being overwritten

					t = tnext(t);
					continue;
				}
			}

		
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == CPOINTER)) { //pointer to Cpointer
				if (tprev(tprev(t))->ty->category == CPOINTER) {  //cpointer
					fold(tprev(tprev(t)), t);
					t->handler = hstorecptr;
					t = tnext(t);
					continue;
				}
			}

			break;
			
		case KTRASH: //item destruction
			
			if (tprev(t)->ty->category == POINTERPOSSESSIVE){
				fold(tprev(t), t);
				t->handler = hfree;
				t=tnext(t);
				continue;
			}
			
			break;
		
		case KKEEP: //item addref
			if (tprev(t)->ty->category == POINTERPOSSESSIVE){
				t->ty = tprev(t)->ty; //return the same type (does not pop the stack)
				fold(tprev(t), t);
				t->handler = haddref;
				t=tnext(t);
				continue;
			}
			
			break;
			
		case KNEW0:  //empty array
		case KNEW:  //item or array
		

			if    ((tprev(t)->ty == tZ32) && (tprev(tprev(t))->ty == tType))		{//Type Z32{
						
								
				fold(  tprev(t), t); //put array size as sub
					
				typeT* rt = (void*) tprev(t)->val.as.type;  //value of item
				
			
				
				if(rt->category != ARRAYDYNAMIC){
					xprintf(" allocating array for type ");
					printType(rt, NULL, NULL);
					ERR("Only dynamic arrays can be allocated by  '[type] count new' \n");
				
				}
				
				ram_free(tremove(tprev(t))); //remove array type token
				
				t->ty = findType(POINTERPOSSESSIVE, rt, NULL, 0);//get possessive pointer to
											
				t->handler =   hallocarray;
				if (ts->tok == KNEW)
					t->val.as.z32 = 1; //array starts full
				else
					t->val.as.z32 = 0;
				
				t = tnext(t);
		
				continue;
			}
				
			if (tprev(t)->ty == tType){	//suchas as MyWhateverStrucutre new
				typeT* rt = (void*) tprev(t)->val.as.type;
				
				if ((rt->category == ARRAYSTATIC)||(rt->category == ARRAYDYNAMIC)){
					printType(rt, ZTRUE, ZTRUE);
					ERR(" Cannot allocate array with '[type] new' ; must use dynamic syntax: [type] count new\n");
				}
				
				t->ty = findType(POINTERPOSSESSIVE, rt, NULL, 0);
				xprintf(" new will return \n");
				printType( rt, 1, 1);
				fold(tprev(t), t);
				t->handler =   halloc;
				t = tnext(t);
				continue;
			}
			
			ERR(" WHAT %x %s?\n", t->tok, t->str);
			
			break;						
			
		}//end switch
		
		//if didn't match anything above, continue on
				
		//check local variables
		if (pc->type){	 //set to function type if inside function
			int count =0;
			int pos=0;
			
			xprintf(" LOOKING IN ");
			printType(pc->type, ZTRUE, ZFALSE);
			typeT* m = findTypeMember( pc->type, t->str, &pos, &count);
			
			if (m){
				int so = -count+pos;
				xprintf("Found %s  stack pos fp+%d, of type   ", m->name, so);
				printType(m->ref,ZTRUE, ZFALSE);
				t->ty = m->ref;
				t->tok = STACKARG;
				tokenT* tn = mkToken('@', "@", 1);  //load the variable
				insert_after(t, tn);
				
				t->val.as.z32=so;
				t=tnext(t);
				continue;
			}
		}
			
		if (t->str){
			int j;
			v = zvec_disown(zvec_mk(NULL,15));
			
		
			tokenT* pos;
			tokenT* startfold=NULL;
			for(j=0;;j++){
			

				int k;
				pos = t;
				//xprintf(" DEPTH %d: \n", j);
				zvec_setcount(v, 0);
				for (k=0;k<j;k++){  //go back j spaces
					pos = tprev(pos);
					
					if (!pos)
						break;
				}
				if (!pos)
						break;
				startfold=pos;
				if (parseDebugFlag) {
					xprintf("debug\n");
				}

				for(k=0;k<j;k++){
			
					//xprintf("%d:%s ", k, pos->str);
					//printType(pos->ty, ZFALSE, ZTRUE);
					if ( !pos->ty ){ //don't go past a nulltype
						pos=NULL;
						break;
					}
					zvec_add(v, pos->ty);
					pos = tnext(pos);
				}//end k
				//xprintf("\n");
				if (!pos)
					break;
				s=NULL;
			

				if (pc != global) {

					
					if (!strcmp(t->str, "gg")) {
						xprintf(" parsing subctx\n");
					}

					local = 1;
					s = findSymbol(pc->symbols, t->str, v);
					//xprintf(" LOCAL SYMBOL %p  %s\n", s, t->str);

					if (s)
						break;

					if (pc->parent && pc->parent != global) {
						s = findSymbol(pc->parent->symbols, t->str, v);

						if (s && s->type->category != FUNCTION) {
							ERR("Can't access parent variable\n");

						}

						if (s)
							break;
					}

				}
				
				local=0;
				
				s= findSymbol(global->symbols, t->str, v);
				  
				//xprintf(" GLOBAL SYMBOL %p  %s\n", s, t->str);
				
				if (s)
 					break;
									
				//getc(stdin);
									
			}//end j
			ram_free(v);
			v=NULL;
								
			if (s){
				//xprintf(" Found symbol %s  local:%d \n", t->str, local);
				//printType(s->type,0,0);
				//xprintf("\n");
				
				if (s->type->category == FUNCTION){
					//xprintf(" IS FUNCTION\n");
					fold(startfold, t);
					
					//check parameters to function being called
					//if any parameters are tracked from a possessive pointer, flag for addref
					
					tokenT* tv =startfold;
					
					while(tv){ 
						if (tv->trackpossptr){
							//xprintf(" Arg derived from possessive pointer <%s> ", tv->str  );
							//if an arg being passed to a function is a possive pointer (or result of adding/indexing from a possessive pointer), then it needs to be addref'd before being passed to a function
							if (s->handler == hcall){
								
								if (tv->trackpossptr->handler == hloadptr){
									tv->trackpossptr->val.as.n32 |=2;
									
								}	
							}
							
							//getc(stdin);
							//	xprintf(" addref %p +%d\n", ex->stack[ex->fp-count+i].as.ptr.block, ex->stack[ex->fp-count+i].as.ptr.offset);
								//ram_addref( ex->stack[ex->fp-count+i].as.ptr.block );
						}
						
						tv = tnext(tv);
						
					}
	
					
					
					
					t->ty = s->type->ref;
					t->sym=s;
					if (s->handler){
						//xprintf("using handler %s %p\n", s->name, s->handler);
						t->handler=s->handler;
						
					}
				}
				else{
					
					//for variables, put the variable's address on the stack for now 
					//and then follow it with '@' to get it
					//for static arrays, this is not needed
											
					t->ty = findType(POINTERUSER, s->type, NULL, 0);  //pointer to the symbol's type
					if (local)
						t->handler = hlocal;
					else
						t->handler = hglobal;
					t->val.as.ptr.block = 0;
					t->val.as.ptr.offset= s->offset;
					
					
					
					
					if ((s->type->category!=ARRAYSTATIC)&&(s->type->category!=STRUCT)) {  
						tokenT* tn = mkToken('@', "@", 1);  //load the variable
						
						
						
						insert_after(t, tn);
					}
					//printList(ts, t, ENDFILE, 1);
								
										
				}
				t=tnext(t);
				continue;	
			}//end s
			
			
			
			//check if struct member
			if (t->str && t->str[0]=='.'){
				//xprintf(" dot\n");
							
				typeT* ptype = tprev(t)->ty;
						
				//if we have a pointer to a struct..
				if (ptype && (ptype->category == POINTERUSER)   && (ptype->ref) && (ptype->ref->category==STRUCT)){
				
					//we can create pointer to type member
					//xprintf(" look for member\n");
					
					typeT* m = findTypeMember( ptype->ref, t->str+1, NULL, NULL);
					if (m){
						
					
						//xprintf(" OFFSET %d for %s in %s\n", m->offset,t->str+1, tprev(t)->ty->ref->name );
						
						t->handler = hoffsetptr;
						
						t->val.as.n32 = m->offset;
						t->ty = findType(POINTERUSER, m->ref, NULL,0); //find pointer to the member type
						
						t->trackpossptr = tprev(t)->trackpossptr;  //if was tracking from a possessive pointer, still track this pointer is derived from that
						fold(tprev(t),t);
						
						//for static arrays or substructs (that are embedded (not pointers)) then the pointer addition already made a pointer to the substruct/array.  For other cases (it is a pointer to a struct, integer, etc) then insert a load token.  
						if (( m->ref->category!=ARRAYSTATIC)&&( m->ref->category!=STRUCT)) {  
							tokenT* tn = mkToken('@', "@", 1);  //load the variable
							insert_after(t, tn);
						}
						
						t=tnext(t);
						
						//printList(tprev(tprev(tpis& :[Z32]& :any& printrev(ts))), t, NULL, 3);
						
						
						continue;
					} //end found matching member
					//xprintf(" no found\n");
				}//end has ref type
				
			}
			
			//Check if it is a datatype
			typeT* ty = NULL;
			tokenT* tn=NULL;
			if (t->tok=='['){
				t = parseType(t);
				ty = tprev(t)->ty;
				tn=t;
			} else {
				ty = findType(NAMED, NULL, t->str, 0);
				tn = tnext(t);
			}
			
			if (ty){
				xprintf("Found type %s\n", ty->name);
				ts->ty = tType;
				ts->val.as.type = ty;
				t=tn;
				continue;
			}
			
			t->zlistnode.next=NULL;//end it
			printList(tprev(ts),t,0,1);
			ERR("Undefined symbol:%s\n\n", t->str);
			
			
		}//end str
		xprintf("?How to parse %x %c\n", t->tok, t->tok);
		ERR("Unimplemented\n");
			
	} //end while
	xprintf(" returning NULL token\n"); 
	return NULL;
}

void cleanCCall(exectxT* ex, tokenT* t, void* first) {

 	if (t->sym->type->members) {
		int i;
		int count = zvec_count(t->sym->type->members);
		for (i = 0; i < count; i++) {
			typeT* m = zvec_get_at(t->sym->type->members, i);

			if (m->trashAfterPrimitive) {
				if (i == 0)
					ram_free(first); //first arg is passed seperately because the C return value has already overwritten it
				else
					ram_free(ex->stack[ex->sp - count + i].as.ptr.block);
			}

			

		}
	}
}

#ifdef EXTENSION_H
#include "gen_extension.h"
#endif

#include "zwindow.h";
#include "gfx_gl.h"
#include "cextra.c";
#include "gen.c";




 int_abyss(void* unknown) {

	//What if it is a type?
	typeT* ty = unknown;
	printf(" unknown as typeT:\n");
	printType(ty, ZTRUE, ZFALSE);

	printf("\n");
}


int main(int argc, char** args){
	
	if ((argc > 1) && !strcmp(args[1] , "-scan")) {
		return scanmain(argc - 1, args + 1);
	}

	logfile = fopen("debug.log", "wb");

	abyss = int_abyss;

	if (!logfile)
		logfile = stdout;
	else
		printf("Debug output is to file\n");

	tType = mkType( SIMPLE, NULL, "Type", 0 ); //datatype about "types"
	mkType( SIMPLE, NULL, "any", 0 ); //not really a type, but for plain pointers (any*)
	tPrimitive = mkType( PRIMITIVE, NULL, "Primitive", 0 ); //allows lookup of C functions by name
	tZ32 = mkType( SIMPLE, NULL, "Z32", sizeof(zint32));
	tN32 = mkType( SIMPLE, NULL, "N32", sizeof(zuint32));
	tN8 = mkType( SIMPLE, NULL, "N8", sizeof(zbyte));
	tBit = mkType( SIMPLE, NULL, "Bit", sizeof(zbyte));
	tString = mkType(SIMPLE, NULL, "String", sizeof(char*));
#ifdef FLOAT
	tReal = mkType(SIMPLE, NULL, "Real", sizeof(FLOAT));
#endif
	
	if (argc < 2)
		exit(1);

	char* x = ram_loadstr(args[1]);

	global = mkcontext();
	
	zlistT* tokens = ram_alloc(sizeof(zlistT), (ram_destructor) zlist_cleanup);
	
	tokenT* t = mkToken(STARTFILE, NULL,0);
	zlist_addhead(tokens,&t->zlistnode);
	t->zlistnode.prev=&t->zlistnode;  //'trap' so ->prev->prev is always safe
				
	//something should stop reading at endfile
	tokenT* end = mkToken(ENDFILE, "ENDFILE", 0);
	zlist_addtail(tokens, &end->zlistnode);

	//if program ever advances reads PASTENDFILE, its an error
	//this is a trap so 'next->next->next' always is safe
	end = mkToken(PASTENDFILE, "PASTENDFILE", 0);
	zlist_addtail(tokens, &end->zlistnode);
	
	tokenize(t, x);
	
	ram_free(x);
	
	//add all the binary and unary operator handlers
	addhandlers(global);



#ifdef SET_EXTENSIONS
	SET_EXTENSIONS();
#endif
	
	//delete the STARTFILE token
	
	
	parse( global, tnext((tokenT*)zlist_head(tokens)) );
	

	xprintf("Types:\n");
	int i;
 	for (i=0;i<zvec_count(types);i++)
		printType(zvec_get_at(types,i),ZTRUE, ZFALSE);
			
	printSymbols(global->symbols, "globals");
 	printList(zlist_head(tokens),NULL,ENDFILE, 0);
	//getc(stdin);
	//start(global, (tokenT*)tokens->head->next   );//run
	start(global, tnext((tokenT*)zlist_head(tokens))   );//run
	
	ram_free(tokens);
	ram_free(global);
	ram_free(types);
	ram_free(csizes);
	
	xprintf(" done\n");
	ram_allocs(); //dump memory leak list
	return 0;   
}

