#include "int.h"
#define XDEBUG 1
#define xprintf(a,...) (XDEBUG?fprintf(logfile, a, __VA_ARGS__),fflush(logfile):0)
FILE* logfile;
 
#ifdef FLOAT 
#include "math.h"
#endif
void breakpoint() {


  	printf("breakpoint here\n");
}


char* lastfile = NULL;
char* lastline = 0;
#define ERR( ...)  do {xprintf(__VA_ARGS__); fprintf(stderr,__VA_ARGS__);fprintf(stderr, "Near %s:%d\n", lastfile, lastline);fflush(stderr);breakpoint();  exit(1);} while(0)
 
//uncommenting below will log a LOT while running.
//#define EXEDEBUG 

/**** Basic Values ****/
//moved to int.h

/* Parse Context */
typedef struct parsectxS{
	zvecT* symbols;	//of type symbolT*
	zuint32	size;	//size of variables in this table
	struct typeS* type;  //if in a procedure, we need to know about its return type and args
						 //if in an immediate context, need to know about the return type 
	struct parsectxS* parent;
	struct symbolS* symfrom;
	int endable;
	exectxT* exec;
	zbool no_global_vars;	//set to true to prevent compiling access to global variables (global functions ok)
	tokenT* t; //where parser left of if calling into a Z program to parse
	char* name;
}parsectxT;

/**** Tokenizer ****/

/* Program is a linked list of tokens*/
/* Macros to make some things easier
 * tremove: removes a token from the list (and returns it as a pointer to be assigned somewhere else or freed.  To remove requires a token is in front of or behind it
 * tnext/tprev: Pointer to the next token
 * insert_after: Inserts a token after another token.  Requires the place of insertion is not the head or tail of the list
 * */
//some macros moved to int.h
#define tremove(ITEM)    zlist_remove_mid(  &(ITEM)->zlistnode)
//next is moved to int.h
//#define tnext(ITEM) ((tokenT*)(ITEM)->zlistnode.next)
#define tprev(ITEM)    ((tokenT*)zlist_prev(ITEM))
#define insert_after(AFTER,NEW)    zlist_insert_node_after(  &(AFTER)->zlistnode,  &(NEW)->zlistnode);

//tokenT->tok values:
//any character symbol itself is just its int value
//PAIR is 2 characters, like [], !=, etc, theya re just combined into a 16-bit value
#define PAIR(B1,B2)	((((unsigned int)(B1&0xff)) <<8) | ((unsigned int)(B2&0xff)))
#define NAME		0x0200
#define NUMBER		0x0300
#define LITERAL 	0x0400
#define ENDFILE		0x0500
#define PASTENDFILE	0x0600
#define STARTFILE	0x0700
#define COND		0x9001
#define STACKARG	0x9002
#define PASSTHRU	0x9003
#define COMPILE		0x9004
#define REDIRECT	0x9005
#define LOADEXEC	0x9006
#define TYPECAST	0x9007
#define PERARG		0x9008
#define SHADERDATA	0x9009


//Token values that are also user-accessible keywords: These are intentionally high enough to not conflict with a pair
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
#define PREFIX		0x8015
#define KLIKE		0x8016	
#define KSHADER		0x8017
#define KOPAQUE		0x8018
#define KIMMEDIATE	0x8019
#define KCODE		0x801a
#define KSTACKED	0x801b
#define KTYPEOF		0x801c
#define KCONSTANT	0x801d
#define KPER		0x801e
#define KALIAS		0x801f
#define KSKIP		0x8020
#define XXKBEGIN		0x8021  /*deleted*/
#define KAND		0x8022
#define KOR			0x8023

char*  keywords[] = {	"var", "type", "end", "primitive", "proc","return", "if", "else", "elseif", "loop", "break", 
						"new", "proto", "trash", "keep", "take", "include", "virtual", "selector", "cpointer", "new0",
						"prefix", "like", "shader","opaque","immediate", "code", "stacked", "typeof", "constant",
						 "per" , "alias", "skip", "XXbegin", "and", "or", NULL};

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
	ram_free(t->sourcefile);

	if (t->val_to_free)
		ram_free(t->val.as.ptr.block);
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
 * Things like 0x10.4E-4 is a 'NUMBER', but will later fail as it isn't a valid form
 */

//the general pattern of the next few functions return the number of characters they accept
//if they return 0, the rule is not accepted
//used to recognize 2-letter combinations like ->, etc
zuint32 findPair(char* patterns, char a, char b){
	for(  ;*patterns;patterns+=2){
		if ( ((*patterns)==a) &&(*(patterns+1)==b))
			return PAIR(a,b) ;
	}
	return 0;
}

//used to either recognize names or numbers. Simple, not regex-fancy or anything
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
//filname is optionsl
void  tokenize(tokenT* insert, char* in, char* filename){
	char* instart = in;
	int c,next;
	tokenT* t=NULL;
	
	int line=1;
	char* fnamecopy = zstrdup(filename);

	while ((c = *in)){
		next = *(in+1);
		zuint32 p;
	

		//find twochar patterns like ->,etc. including comment start/end markers
		//looking for pairs before chars makes sure the matching is 'greedy'
		if ((p = findPair(".&.%.@--++==->/**///[]>=<=!=.-###=\\\\/\\\\/", c, next))){
			if (  p == PAIR('/','/')  ) { //special handling for // comments
				while(*in!= '\n')
					in++;
				in++;//skip
				line++;
				continue;
			}

			if (p == PAIR('/', '*')) { //special handling for /* comments
				char oin = 0;
				while (*in && ( (oin != '*') || (*in != '/'))  ) {
					oin = *in;
					if (*in == '\n')
						line++;
					in++;
				}
				in++;//skip past /
				
				continue;
			}
	
			t = mkToken(PAIR(c, next) , in, 2);
			t->line = line;
			t->sourcefile = ram_addref(fnamecopy);


			zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
			insert = t;
			
			in+=2;
			continue;
		}

		//check for string literals
		int lit = acceptLiteral(in, '"' , '\\' ); //double quote

		if (lit) {
			t = mkToken(LITERAL, in, lit);
			t->line = line;
			t->sourcefile = ram_addref(fnamecopy);
			zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
			insert = t;
			
			while (lit) {
				if (*in == '\n')
					line++;

				lit--;
				in++;
			}


			continue;
		}

		//collapse spaces and tabs together
		int space = acceptPatterns(in, " \t\n\r", "", " \t\n\r");

		if (space) {

			while (space) {
				if (*in == '\n')
					line++;

				space--;
				in++;
			}

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
				".abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",  //start with ._alpha
				"",
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789"); 
			
		if (name && in[name] == '?') {
			name++; //accept names that end with ?
		}

		if (name && in[name] == '=') {
			name++; //accept names that end with =
		}
		
		int digits  = 0;
		
		if (!name){
			digits  = acceptPatterns(in,
					"-.0123456789", //start with digit or decimal point
					"e-E-e+E+",  //- and + only accepted after an e or E
					"0123456789.abcdefABCDEFx"); //continues with digits, deccimal point, hex letters, type suffix letters
			
			//special case: if number starts with '-', but has only 1 character, this isn't a negative number, but just a minus sign
			if ((digits == 1) && (in[0] == '-'))
				digits = 0;
		}
						
		if (digits || name){
			t = mkToken( digits? NUMBER : NAME, in ,   digits|name);
			t->line = line;
			t->sourcefile = ram_addref(fnamecopy);
			in += digits|name;
			zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
			insert = t;
			continue;
		}

		//just some char
		t = mkToken( *in, in, 1);
		t->line = line;
		t->sourcefile = ram_addref(fnamecopy);
		
		zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
		insert = t;
		in++;
	}	
	ram_free(fnamecopy);
}

/**** Data Types ****/

/* type categories */
#define SIMPLE			1
#define POINTERUSER		2
#define STRUCT			3
#define ARRAYSTATIC 	4
#define ARRAYDYNAMIC 	5
#define FUNCTION		6
#define PRIMITIVE		7
#define POINTERPOSSESSIVE 8
#define CPOINTER		9
#define OPAQUE			10
#define VIRTUAL			11
#define LAST_REAL_TYPE	11

//letting virtual types be 'real' when parsing a struct type definition makes the code easier
//OPAQUE is an opaque value with a size.  
//CPOINTER is a machine-sized pointer compatible with C.  Not reference counted, etc. Using Z pointers is preferred, but when a pointer needs to be stored in a C struct, it has to be C-sized
//ARRAYSTATIC have a fixed size.  To be embedded directly in structs, etc
//ARRAYDYNAMIC are heap allocated

//MEMBER is not a type, but is used to mark members of a struct
#define MEMBER	20
//NAMED is not a type, but when passed into findType looks for struct or simple  w/out knowing which it is yet
#define NAMED	21
//PENDING not a type, but is for when a type is mentioned in another declaration but not yet defined.  You can't 'make' or size a PENDING type, but can have pointers to them
#define PENDING  22
//SUBTREE is parsed, executable code tree passed as an arg to an immediate proc
#define SUBTREE 23
#define ALIAS   24
//When a type is a 'LIKE', it is mean to be the same type as one of the function args.  The name is the name of the arg
//A pointer to a LIKE type resolves to be a pointer to whatever the LIKE type arg is.
#define LIKE	25
//A DEREFTYPE means get the type of ref's struct element by name OR, the type pointed to by a pointer.  Usually wraps a 'LIKE' type  so you can refer to the type the pointer points to 
#define DEREFTYPE	26

typedef struct typeS{
	char* name;
	zuint32 size;	
	zuint32 category;	//SIMPLE, POINTER, etc
	zuint32 len; //for definite arrays, 0 for indefinite arrays
	zuint32 offset; //for struct members (byte position)
	struct typeS* ref; //array or pointer types, or function return type
	struct typeS* parent;	//'parent' type, only for members, only in certain situations (currently when finding the 'real' type behind a virtual)
	zvecT* members;  //(typeT*) structs or function parameters
	zvecT* selectors; //(symbolT*)  function selectors
	zbool trashAfterPrimitive; //only for function args (members), only when passing %pointer
	zbool isPer;	//for function areg MEMBERs 
	zbool deref;  //true if the called function is to automatically deref the pointer
	int tid;
	struct symbolS* destructorproc;
	zbool stacked;
	zbool islike;  //type needs to be resolved relative to another item
	zbool genname;  //name is made up by the system
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

	if (category != MEMBER && ((category == LIKE)|| (ref && ref->islike)))
		ty->islike = ZTRUE;


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
	if ((category == POINTERUSER)|| (category == ARRAYDYNAMIC)||(category == POINTERPOSSESSIVE)||(category==CPOINTER) ){
		if (ty->size)
			ERR("Cannot specify size of pointer or dynamic array (it is automatically calculated)\n");
		if ((category == POINTERUSER) || (category == POINTERPOSSESSIVE)) {
		
			if (ref->category == VIRTUAL) {
				ty->size = sizeof(vptrselectorT); //pointer to object AND pointer to selector table
			} else {
				ty->size = sizeof(vptrT);
			}

		}

		if (category == CPOINTER)
			ty->size = sizeof(void*);
		
		if (category == ARRAYDYNAMIC) 
			ty->size = ref->size; //size of 1 element
	}

	if (types == NULL)
		types = zvec_mk(NULL, 100);

	zvec_add(types, ty);

	if (!name && ty->ref) {  //all types need some name
		ty->name = zstrprintf(ty->name, "%s_C%d", ty->ref->name, ty->category);
		ty->genname = ZTRUE;
	}

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
	//	xprintf("%d~", ty->tid);
		if (!skipmembers)
			xprintf("<%dbyte>", (int)ty->size);
		
		switch(ty->category) {
		case  LIKE:
			xprintf("like"); 
			break;
		case  PRIMITIVE:
			xprintf("<PRIMITIVE>");  //continue on as func
		case  FUNCTION:
			xprintf("proc(");
			end=")";
			skipmembers=ZFALSE;
			break;
		
		case ARRAYSTATIC:
		case ARRAYDYNAMIC:
			if (ty->len)
				xprintf("[%d", ty->len);
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
		//	xprintf(".");
			break;
		case VIRTUAL:
			//xprintf("(virt) ");
			break;

		case OPAQUE:
			xprintf("<opaque>");
			break;

		case CPOINTER:
			end = "*";
			break;
	
		case SIMPLE:
			break;

		case SUBTREE:
			xprintf("<subtree>");
			break;

		case DEREFTYPE:
			end = "@";
			break;

		default:
			xprintf(" (U-%x) ", ty->category);
		}
		
		if (ty->name && !ty->genname)
			xprintf("%s", ty->name);
		if (ty->category == MEMBER)
			xprintf(":");
		
		
	
		if (ty->trashAfterPrimitive)
			xprintf("trash");
		
		if (!skipmembers && ty->members){
			zuint32 i;
			for (i=0;i<zvec_count(ty->members);i++){
				printType( zvec_get_at(ty->members,i), ZFALSE, ZTRUE);
				xprintf(";");
			}
	
		}

		if (ty->category == FUNCTION)
			xprintf("->");
				
		if (ty->ref)
			printType(ty->ref, ZFALSE, ZTRUE);
		
		xprintf("%s",end);
	}
	
	if (line)
		xprintf("\n");
	
}

void printTypeNoRedirect(typeT* ty, zbool line, zbool skipmembers) {
	FILE* old = logfile;
	logfile = stdout;
	printType(ty, line, skipmembers);
	logfile = old;
}

typeT* tType, * tPrimitive, * tZ32, * tN32, * tN8, * tBit, * tString, * tReal, * tany, * tImmediate, * tCode, * tExecToken, * tEmptyStack, * tvany, *tSymbol ;
typeT* findType(zuint32 category, typeT* ref, char* name, size_t len);

//given an likename, and a list of arg types a function accepts, and a list of types a caller is using, figure out what type an arg or return value is
// is_caller means it will resolve to a specific type from the point of view of a caller: 
//		if a function takes arg  x:any&, and returns 'like x', and the caller is called a String&, then from the point of view of the caller, the function returns a String&
// if is_caller is false, then its from the point of view of the function.  The function only knows it has an any&, so it can only do any& operations

typeT* resolve_like_type(typeT* liketype, zvecT* proc_likes, zvecT* caller_likes, zbool is_caller) {  

	if (!liketype || !liketype->islike) {
		return liketype;
	}
	if (liketype->islike && liketype->category != LIKE ) {
		//if it is a wrapped like, need to unwrap it, resolve it, and re-wrap it
		return findType(liketype->category, resolve_like_type(liketype->ref, proc_likes, caller_likes, is_caller), liketype->name, 0);
		///return mkType(liketype->category, resolve_like_type(liketype->ref, proc_likes, caller_likes, is_caller), NULL, 0);
	}

	int i;
	typeT* likeType = NULL;
	for (i = 0; i < zvec_count(proc_likes); i++) {
		typeT* arg = zvec_get_at(proc_likes, i);
		if (!strcmp(arg->name, liketype->name)) {
			if (is_caller) 
				likeType = zvec_get_at(caller_likes, i);
			else
				likeType = arg->ref;  //as what the function calls it

				return likeType;
		}

	}
	return NULL;
}

//compare two types, return true if equivalent
//if allow_any is set, than 'any' can match any type. (meaning any& will match all references, [any%] will match all arrays of possiesve pointers, etc)
//todo: Need to restrict 'any' to only real types.  Pointers to virtual types are larger (because of the selector table).  [virtual any]& ?
zbool cmpType(zuint32 category, typeT* ref, char* name, size_t len, typeT* ty, zbool allow_any, zvecT* proc_likes, zvecT* caller_likes){

	//if (!ty)
		//ERR("compare null type\n");
	if (!ty) 
		return ZFALSE;

	if (category == MEMBER)
		ERR("struct members shouldn't be compared\n");

	 //allow any is intended for functions that accept 'any' types

	if (allow_any && ty == tany && category != VIRTUAL) {
		return ZTRUE;  //allow any to match anything
	}
	
	if (allow_any && ty == tvany && category == VIRTUAL) {
		return ZTRUE;  //allow vany to match any virtual type
	}

	if (allow_any && ty->islike) {
		typeT* liketype = resolve_like_type(ty, proc_likes, caller_likes, ZTRUE);		
		return cmpType(category, ref, name, len, liketype, allow_any, NULL, NULL);
	}

	if (ty->category != category)  //early exit for categories that should match
		return ZFALSE;
	
	if ((category == SIMPLE)||(category==STRUCT) || (category==PENDING)){
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

		zuint32 i;
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
	if (ty->ref != ref) {

		if (!allow_any)
			return ZFALSE;

		if (ty->ref && !ref)
			return ZFALSE;

		if (ref && !ty->ref)
			return ZFALSE;

		return cmpType(ref->category, ref->ref, ref->name, ref->len, ty->ref, allow_any, proc_likes, caller_likes);
	}
	
	return ZTRUE;
}

typeT* findTypeMember(typeT* type, char* name, int* pos, int* count);
//finds simple or struct types, OR creates composite types (arrays, pointers of existing types) as needed
typeT* findType(zuint32 category, typeT* ref, char* name, size_t len){

	typeT* ty;
	typeT* found=NULL;
	zuint32 i;

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
			case OPAQUE:
			case ALIAS:
				if (!strcmp(name, ty->name)){
					//found on name

					if (ty->category == ALIAS)
						return ty->ref;

					return ty;
				}
				break; //not it

			case POINTERPOSSESSIVE: 
			case POINTERUSER: 
			case ARRAYSTATIC:
			case ARRAYDYNAMIC:
			case FUNCTION:
			case SUBTREE:
			case CPOINTER:

				if (cmpType(category, ref, NULL, len, ty, ZFALSE, NULL, NULL))
					return ty;

				break;

			case LIKE:
				break; //don't find 'like' types in this list 

			case DEREFTYPE: //for 'like' types

				
				if (name && name[0] && (ref->ref->category == STRUCT)) {
					//printf(" find %s\n", name);
				
						typeT* m = findTypeMember(ref->ref, name, NULL, NULL);
						return m->ref;
				
				}

			

				return findType(ref->ref->category, ref->ref->ref, ref->ref->name, ref->ref->size);

			default:
				ERR("unknown type category %d\n", category);
		}
	}



	if (ref &&((category == ARRAYDYNAMIC)||(category==ARRAYSTATIC) || (category == POINTERUSER)|| (category == POINTERPOSSESSIVE) || (category==SUBTREE) ||(category == CPOINTER) ) ) {

		//If array or pointer, find the type 'underneath' and make it

		//xprintf(" Creating %s type for ", getTypeString(category));
		//printType(ref, ZTRUE, ZFALSE);

		if (ref->category == LIKE) {
			ty = ref;
		}
		else if (ref->islike) {  //if a like type, we always want to wrap it (not go re-find it)
			ty = ref;
		//	ty = mkType(ref->category, ref->ref, ref->name, ref->len);
		}
		else if (ref->category == FUNCTION) {
			ty = findType(FUNCTION, ref, NULL, 0);
		}
		else {
			ty = findType(ref->category, ref->ref, ref->name, ref->len);
		}

		if (ty) {
			return  mkType(category, ty, NULL, len);
		}
	}

	return NULL;
}

typeT* findTypeMember(typeT* type, char* name,  int* pos , int* count){
	zuint32 k;
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
tokenT* hredirectsub(exectxT* ex, tokenT* t);
/*debugging list printer*/
int printList(tokenT* t, tokenT* cur, zuint32 stop_tok, int indentin){

	char* iscur;
	int i;
	int indent=indentin;
	zuint32 count=0;
		
	//if stop_tok is set to a negative number (like -3) then up to 3 tokens will be printed (and their subs)
	
	while(t){
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
		//check if there are redirected subshredirectsub
		if (t->tok == PASSTHRU && t->handler == hredirectsub) {
			tokenT* redirected = t->val.as.token;
			if (redirected && zlist_head(&redirected->subs)) {
				//indent = 
				xprintf("\n");
				for (i = 0; i < indent; i++)
					xprintf("\t");
				xprintf("{redirected");
				printList(zlist_head(&redirected->subs), cur, 0, indent + 1);

			}

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
		xprintf("L%d", t->useslocal);
		
		if (t->handler) {

			//symbolT* sh = findSymbolByHandler(t->handler);
			//if (sh)
				//xprintf("handler %s", sh->name);
			//else
				xprintf(" handler %p ", t->handler);
		}

		xprintf("(%p %x)",t->val.as.ptr.block, t->val.as.ptr.offset);
		
		if (!zlist_head(&t->subs))
			xprintf("}");
	
		if (t->tok == stop_tok)
			break;
		
		if (t->tok == PASTENDFILE)
			break;

		if (!t->zlistnode.next)
			break;

		t = tnext(t);
	}
	return indent;
}

/**** Symbols ****/
typedef struct symbolS{
	char* name;
	char* alias;
	struct symbolS *primsym; //if symbol took its handler from a primitive, this is the one
	void* shaderdata;
	typeT* type;
	instruction handler;
	zbool skipargs;//flow control and some other handler need to skip running args
	tokenT* tokens;
	struct parsectxS* subctx; //procs have their own parsecontext for their local vars
	zuint32 offset;
	int immval; //for primitives
	int selectorArg; //if this is a selector, which arg does the function lookup
	int selectorNum;  //which selector (nth) is this?
	zbool isPrototype;// true if this symbol is just a function prototype
	zbool isImmediate; //function runs whenever it is compiled
	zbool isShader;// when function is called, it only puts it address on the stack, and does not pop off its values
				   //that address will later be used to identify this function running in a different
	int   isSelector;// 1 if symbol is a function selector, 2 is is a data selector, 4 if virtual selector
	zbool isConstant; //1 if symbol is just a constant.  tokens points to the constant handler
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

zbool cmpTypeListToFunc(zvecT* f, zvecT* b, int matchApprox){
	//iterates over a functions list of arguments and compares to a possible list of types
	
	if (zvec_count(f) !=zvec_count(b)){
		//	xprintf(" function has different num of arguments\n");
			return ZFALSE;
		}

		zuint32 i;
		for (i=0;i<zvec_count(f);i++){
			typeT* memberf = zvec_get_at(f,i);
			typeT* memberb = zvec_get_at(b,i);

			//if (!strcmp(memberf->name, "isAny"))
				//printf("isAny");

			memberf = memberf->ref;  //past MEMBER 

			if (memberf->category == SUBTREE) {	//function expects CODE that generates the given type
				memberf = memberf->ref;

				if (memberf == tany) {	//allow code subtrees to be of any type
					if (!memberb || memberb == tEmptyStack) {
						return ZFALSE;
					}
					continue;
				}
			}

			//check for approximations
			if (matchApprox == MATCH_IGNORE_SIGNED) {
				//change both to unsigned to make a match
				if (memberf == tZ32)
					memberf = tN32;
				
				if (memberb == tZ32)
					memberb = tN32;

			}

			//compare types of members
			if (memberf != memberb){

				//if calling with a pointer to a real type, but function expects a pointer to virtual type
				//the use the function
				//TODO: need to add the selector table when doing that
				if (matchApprox == MATCH_VIRTUAL) {
					if (memberf->ref && memberf->ref->category == VIRTUAL && memberb && memberb->ref->category != VIRTUAL && memberb->ref->members) {

						int j;
						zbool found = ZFALSE;
						for (j = 0; j < zvec_count(memberb->ref->members); j++) {
							typeT* ty = zvec_get_at(memberb->ref->members,j);
							if (ty->category == MEMBER) {

								if ((ty->ref == memberf->ref)) {
									if (memberb->category == memberf->category) { //both arg and caller have same kind of pointer (POSS or USER)
										
										found = ZTRUE;
										printf("Matching call by autocast to virtual.  This feature is incomplete and there will be a null selector table.  TODO:  insert a haddselector instruction.");
										break;
									}
								}
							}

						}
						if (!found)
							return ZFALSE;  //not the proper type
						continue; //next arg
						
					}
					
				}

				int allow_any = (matchApprox == MATCH_ALLOW_WILD); 

				if (!cmpType(memberb->category, memberb->ref, memberb->name, memberb->size, memberf, allow_any, f, b )) {
					return ZFALSE;
				}
			}
		}
		return ZTRUE;
}

zbool compatibleType(typeT* a, typeT* b) {
	return cmpType(a->category, a->ref, a->name, a->size, b, ZTRUE, NULL, NULL);
}

symbolT* findSymbolEx(zvecT* table, char* name, zvecT* typelist, int matchApprox){ 
	zuint32 i;
	symbolT* s;
	if (!strcmp(name, ".next") && typelist)
		printf("boo");

	for (i = 0; i < zvec_count(table); i++) {
		s = zvec_get_at(table, i);

		if (   (!strcmp(name, s->name)) || (s->alias && (!strcmp(s->alias,name)))) {
			if (s->type->ref && s->type->ref->category == FUNCTION && typelist) {
				//printf("function pointer case?\n");
				if (cmpTypeListToFunc(s->type->ref->members, typelist, matchApprox)) {
					return s;
				}
			}else if ( s->type->category == FUNCTION && typelist ){
					if (cmpTypeListToFunc(s->type->members, typelist, matchApprox)){
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

symbolT* findSymbol(zvecT* table, char* name, zvecT* typelist) {
	return findSymbolEx(table, name, typelist, 0);

}

zvecT* primitives=NULL;

char* findSymbolByHandler(instruction handler) {
	int i;
	if (!handler)
		return NULL;
	for (i = 0; i < zvec_count(primitives); i++) {
		
		if (handler == zvec_get_x_at(primitives, symbolT*, i)->handler) {
			return zvec_get_x_at(primitives, symbolT*, i)->name;
		}
	}
	return "UnknownSymbol";
}

symbolT* mkSymbol(struct parsectxS* pctx, char* name, typeT* type, instruction handler){
	zvecT* table = NULL;
	 
	if (pctx)
		table = pctx->symbols;
	
	symbolT* sym;
	
	if (!type)
		ERR("no type for symbol\n");
		
	if (type == tPrimitive) {
		if (primitives == NULL)
			primitives = zvec_mk(NULL, 100);
		table = primitives;
	}

	if (type->category == FUNCTION || 
		((type->category == POINTERPOSSESSIVE||type->category==POINTERUSER) &&(type->ref->category == FUNCTION))   )
	{
		//todo search for function of same name, same inputs

		//if ((type->category == POINTERPOSSESSIVE || type->category == POINTERUSER) && (type->ref->category == FUNCTION))
		//	printf("overloading function\n");
	} else if (table &&  findSymbol(table, name, NULL)){
			ERR(" Attempt to redefine %s in same context\n", name);
	}
		
	sym = ram_alloc(sizeof(symbolT), symbol_cleanup);
	sym->name = zstrdup(name);
	sym->type = type;
	sym->handler = handler;
	
	if (pctx) {
		sym->offset = pctx->size;
		pctx->size += type->size; //add context u

		xprintf(" SYMBOL %s at offset %d  , total symbols %d bytes\n", sym->name, sym->offset, pctx->size);
		/*
		if (type->size > 0 && pctx->exec && pctx->exec->vars) {
			int* asize = ram_shadow(pctx->exec->vars);
			if (pctx->size > *asize) {
				ERR(" Cannot add variables to context after execution has began\n");
			}
		}
		*/

	}
	if (table)
		return zvec_add_or_free(table, sym);
	else
		return sym;
}

void printSymbols(zvecT* table , char* label){
	int i;	
	xprintf("\n\nSymbols for %s\n", label);
	for (i=0;i<zvec_count(table);i++){
		symbolT* sym = zvec_get_x_at(table, symbolT*, i);
		xprintf("@+%x\t%s\t", sym->offset, sym->name);
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
tokenT* hnop(exectxT* ex, tokenT* t);

tokenT* evalsubs(exectxT* ex, tokenT* t) {
	if (t->handler == hnop)
		return t;
	exe(ex, tsub(t)); //evaluate all the args
	return t;		  //return the same token (for actual handling)
}
tokenT* noargs(exectxT* ex, tokenT* t) {
	return t;		  //return the same token (for actual handling)
}

int debug_break = 0;
tokenT* hnop(exectxT* ex, tokenT* t) {	//do nothing

	if (t->str && !strcmp(t->str, "debug")) {
		debug_break = 1;
		printf("debug");
	}

	return tnext(t);
}

tokenT* hunimplemented(exectxT* ex, tokenT* t) {	

	ERR("Operation unimplemented on cpu\n");

	return tnext(t);
}


void exe (exectxT* c, struct tokenS* t){
	
	struct tokenS* ts=t;
	char* str = "";
	while(t && ! c->stop){  //until out of instructions, or a return is bubbling up



		instruction handler = t->handler;

		

		if (c->debugstack) {
			str=safestr(t->str);
			xprintf("%x %s  (pre) handler %p\n", t->tok, safestr(t->str), handler);
		}

		if (!handler){
			printList(ts, t, 3, 0);
			ERR("null handler for %c %s\n", t->tok, safestr(t->str));
		}

		if (debug_break)
			printf("debug\n");


		if (t->breakpoint)
			printf("break\n");

		if (!t->skipargs)
			t = evalsubs(c, t);
		
#ifdef EXEDEBUG
		printf(" RUN %s\n", findSymbolByHandler(handler));
#endif
		
		if (handler) 
			t = handler(c, t);
				
		if (c->debugstack) {
			xprintf("exe->  %s",  str);
			xprintf("sp %x:\n", c->sp);
			int i;
			for (i = 0; i <= c->sp+2; i++) {
				if (i == c->sp)
					xprintf("INVALID:");
				xprintf("%d: (%p+%x)/%d", i, c->stack[i].as.ptr.block, c->stack[i].as.ptr.offset, c->stack[i].as.z32);
				if (c->stack[i].typeselector)
					printType(c->stack[i].typeselector, ZTRUE, ZFALSE);

				xprintf("\n");


			}
			xprintf("\n\n");
		}
	}
}

//Individual handlers




tokenT* hconstant (exectxT* ex, tokenT* t) {  //push constant on stack
	ex->stack[(ex->sp)++] = t->val;
	return tnext(t);
}

tokenT* hconstantaddref(exectxT* ex, tokenT* t) {  //push constant on stack (add ref)
	ex->stack[(ex->sp)++] = t->val;

	ram_addref(t->val.as.ptr.block);
	return tnext(t);
}

tokenT* haddselector(exectxT* ex, tokenT* t) {  //adds selectors to item on pointer stack
												//for casting real type to a virtual type
	ex->stack[(ex->sp)-1].typeselector = t->val.as.type;
	return tnext(t);
}

tokenT* hredirectsub(exectxT* ex, tokenT* t) {
	tokenT* redirect = t->val.as.token;
	
	exe(ex, tsub(redirect));
	return tnext(t);
}

void* zlist_tail_for_insert(zlistT* list);

tokenT* hsubst(exectxT* ex, tokenT* t) {  //copy linear list of tokens
	
	tokenT* tcode = mkToken(KCODE, "code", 4);
	tcode->ty = t->ty;

	tokenT* t2 = tsub(t);

	xprintf(" SUBST SOURCE<<\n");
	printList(t2, NULL, 0, 0);
	xprintf(">>\n");
		
	while (t2) {
		tokenT* newtok = NULL;

		if ( (t2->tok == '$') || (t2->tok == '\'') ) {
		//	if (t2->tok == '\'')
			//	boo();

			tokenT* last = zlist_tail(&t2->subs);
			tokenT* first = zlist_head(&t2->subs);
				
			xprintf(" Executing this to get value:<<\n");
			printList(tsub(t2),NULL,0,0);
			xprintf(">>\n");
			
			exe(ex, tsub(t2)); //this code puts an item on the stack; this item is the constant value to be baked into the code
			
			if (t2->tok == '$') {

				if (last->ty->category == POINTERUSER && last->ty!=tType) { 
					ERR(" Cannot bake non-possessive pointers into code!\n"); //except: can bake 'types' into code
				}

				newtok = mkToken(PASSTHRU, NULL, 0);	 //PASSTHRU tokens will not be processed by the compiler; in this case the value is compiled now as a constant
				newtok->ty = last->ty;
				newtok->val = ex->stack[--(ex->sp)];
				newtok->val = ex->stack[--(ex->sp)];
				
				if (last->ty->category == POINTERPOSSESSIVE) {
					//printf(" baking a possessive pointer into code\n");
					newtok->val_to_free = 1;
				}
				
				newtok->handler = hconstant;
				newtok->skipargs = ZTRUE;
			
			}
			else {
								
				if (last->ty && (last->ty->category == POINTERUSER)) {
				
					//string turns into a token
					if (last->ty->ref == tString) {
						char* str = ex->stack[--(ex->sp)].as.ptr.block;
						str += ex->stack[(ex->sp)].as.ptr.offset;
						tokenT* stub = NULL;
						
						tokenize(zlist_tail_for_insert(&tcode->subs), str, "nofile");

					}

					if (last->ty->ref == tCode) {
						//inserting raw list of tokens
						
						--(ex->sp);
						tokenT* from = ex->stack[(ex->sp)].as.ptr.offset + (char*)ex->stack[(ex->sp)].as.ptr.block;
						from = tsub(from);
						while (from) {
							tokenT* copyt = mkToken(from->tok, from->str, 0);
							
							
							if ((from->handler == hconstant)|| (from->handler == hconstantaddref)) {
								copyt->handler = from->handler;
								copyt->val = from->val;
								copyt->tok = PASSTHRU;
								copyt->ty = from->ty;
								if (from->handler == hconstantaddref)
									ram_addref(copyt->val.as.ptr.block);
							} else if (from->handler ) {
								ERR("Unknown copy token case\n");
							}
							//todo:copy values and crap
							zlist_addtail(&tcode->subs, copyt);
							from = tnext(from);

						}
					}
				}

				if (last->ty->category == SUBTREE) {
					//printf(" inserting subtree \n");

					--(ex->sp);
					tokenT* subtree = ex->stack[(ex->sp)].as.ptr.offset + (char*)ex->stack[(ex->sp)].as.ptr.block;
		
					printList(subtree, NULL, 0, 2);
					//printf("%p\n", subtree);
					
					
					typeT* rt = subtree->ty->ref;

					if (subtree->tok == REDIRECT) {
					
						tokenT* redirected = tsub(subtree);
						if ((redirected->tok == '@')&&(redirected->generated)) {
				
							if (tnext(t2)->tok == '&') {
								//
								subtree = redirected;
								rt = tsub(subtree)->ty;
								ram_free(tremove(tnext(t2)));
							}
						}
					}

					newtok= mkToken(PASSTHRU, "callredirect", 0); //PASSTHRU token so this isn't reparsed
					newtok->handler = hredirectsub;  //hredirectsub runs the 
					newtok->skipargs = ZTRUE;
					newtok->val.as.token = ram_addref(subtree);
					newtok->val_to_free = ZTRUE;
					newtok->ty = rt;
					
					//todo: need?
					newtok->trackpossptr = tsub(subtree)->trackpossptr; //need to propagate subtree's pointer tracking
									
					if (subtree->restrict_parse_context) {

						if (tcode->restrict_parse_context && tcode->restrict_parse_context != subtree->restrict_parse_context) {
							ERR("Generating code with mismatched quote contexts... not supported\n");
						}

						tcode->restrict_parse_context = subtree->restrict_parse_context;
	
					}

				}
			
			}


		} else 
			newtok = mkToken(t2->tok, t2->str, strlen(t2->str)); //copy the token

		if (newtok) {
			zlist_addtail(&tcode->subs, newtok);
		
			printList(newtok, NULL, 0, 2);
			
		}
		t2 = tnext(t2);
	}

	xprintf("Subst results:<<\n");
	printList(tcode, NULL, 0, 20);
	xprintf(">>\n");
		
	ex->stack[(ex->sp)].as.ptr.level = 0; //heap object
	ex->stack[(ex->sp)].as.ptr.block = tcode;	
	ex->stack[(ex->sp)++].as.ptr.offset = 0;

	return tnext(t);
}

tokenT* hglobal (exectxT* ex, tokenT* t) {	//push pointer to global variable on stack


	if (!ex->globalvars)
		ERR("No access to global variables in current scope");

 	ex->stack[(ex->sp)].as.ptr.block = ex->globalvars;
	ex->stack[(ex->sp)].as.ptr.level = 0; 
	ex->stack[(ex->sp)++].as.ptr.offset = t->val.as.ptr.offset;
	//xprintf(" Global block %p +%d\n", ex->globalvars  ,   t->val.as.ptr.offset);
	return tnext(t);
}

tokenT* himmvar(exectxT* ex, tokenT* t) {	//push pointer to immediate scope variable on stack (immediate scope is the 'global' scope for immediate blocks

	if (!ex->immediatevars)
		ERR("No access to immediate  variables in current scope");

	ex->stack[(ex->sp)].as.ptr.block = ex->immediatevars;
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

tokenT* hstackread (exectxT* ex, tokenT* t) {	//read a stack variable (really function parameters)
	
	if (!tsub(t)){
		ERR("hstackread needs sub for stack position offset\n");
	}
	
	ex->stack[ex->sp] = ex->stack[ ex->fp + tsub(t)->val.as.z32 ];

	//ex->stack[ex->sp] = ex->stack[ex->fp + t->val.as.z32];
	
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

tokenT* hstackptr(exectxT* ex, tokenT* t) {	//get pointer to nth item on stack (only for structs kept on the stack)
	
 	ex->stack[ex->sp].as.ptr.block = &ex->stack[ex->fp + t->val.as.z32];
	ex->stack[ex->sp].as.ptr.offset = 0;
	ex->stack[ex->sp].as.ptr.level = ex->level + 1; //add level to source (so it must be used or passed but not locally stored)

	(ex->sp)++;

	return tnext(t);
}


//#define DEREF(TYPE,BASE,OFFSET)      (*((TYPE*)(((char*)(BASE))+(OFFSET))))

void* NULLERR() {
	ERR("Null pointer\n");
}

#define CHECK(BBB)  ( BBB?BBB: NULLERR() )

#define DEREF(TYPE,BASE,OFFSET)      (*((TYPE*)(((char*)(CHECK(BASE)))+(OFFSET))))

tokenT* hstoreptr (exectxT* ex, tokenT* t) {  //store a pointer
	
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
		ram_free(DEREF(vptrT, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset).block);
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

tokenT* hstoreptrnp(exectxT* ex, tokenT* t) {

	//store the pointer
	tokenT* nt = hstoreptr(ex, t);
	ex->sp++;  //unpop the value stored

	if (t->val.as.n32 & 16) {	//was storing possessive, so addref the value that was stored
		ram_addref(ex->stack[ex->sp - 1].as.ptr.block);
	}
	else {
		ex->stack[(ex->sp - 1)].as.ptr.level = ex->level + 1;//pointer 'belongs' to deeper stack frames
	}

	return nt;
}

tokenT* hloadptr (exectxT* ex, tokenT* t) { //load a pointer
	
	if (!ex->stack[ex->sp - 1].as.ptr.block) {
		//loading a null pointer from null just results in null instead of crash
		ex->stack[ex->sp - 1].as.ptr.offset = 0;
		ex->stack[ex->sp - 1].typeselector = NULL;
		return tnext(t);
	}
		
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
	
	ex->stack[ex->sp-1].as.ptr.offset += t->val.as.n32;
	
	return tnext(t);
}

tokenT* hselectorptr(exectxT* ex, tokenT* t) { //add constant offset to pointer by selector

	typeT* seltable = ex->stack[ex->sp - 1].typeselector;
	symbolT* selected = zvec_get_at(seltable->selectors, t->val.as.n32); //grab the nth function from the selector table

	/*
	printf(" Data selector %d for virtual type %s is  %d for real type %s\n",
		t->val.as.n32, seltable->ref->name,
		selected->offset, seltable->parent->name);
		*/
	
	ex->stack[ex->sp - 1].as.ptr.offset += selected->offset;

	return tnext(t);
}

//finds the first token of an executable symbol
tokenT* firstTokenHelper(symbolT* s) {
	tokenT* ts = s->tokens;
	if (ts->tok == KPROC) {
		ts = tsub(ts);
		ts = tnext(ts);
	}
	else if (ts->tok == KCODE) {
		ts = tsub(ts);
	}
	return ts;
}

//returns the first executable token for a proc
tokenT* hsymtoken(exectxT* ex, tokenT* t) {
	symbolT* sym = ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset;
	tokenT* ts = NULL;
	if (sym && !sym->primsym) {
		
		ts = firstTokenHelper(sym);
		
	}
	ex->stack[ex->sp - 1].as.ptr.block = ts;
	ex->stack[ex->sp - 1].as.ptr.offset=0;
	return tnext(t);
}

typeT* tvaltype(tokenT* t);
//returns the constant embedded in a token,//NOT  but only if the expected type is correct
tokenT* htokenval(exectxT* ex, tokenT* t) {

	tokenT* ts = ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset;
	typeT* tsty = tvaltype(t);

	//if (tsty == t->ty) {	//if expected type
		ex->stack[ex->sp - 1] = ts->val;
		if (t->ty->category == POINTERPOSSESSIVE)
			ram_addref(t->val.as.ptr.block);
	//}
	//else {
		//memset(&ex->stack[ex->sp - 1], 0, sizeof(ex->stack[ex->sp - 1]));
	//}

	return tnext(t);
}

tokenT* htesttype(exectxT* ex, tokenT* t) { //convert a virtual pointer to real pointer, if its the correct type.  returns NULL if not. frees possessive pointers if NULLing them

	typeT* tsel = ex->stack[ex->sp - 1].typeselector;

	if ( !tsel    //not virtual
		|| tsel->parent != t->val.as.type->ref //type selector table isn't of the correct type
		){

		if (t->val.as.type->category == POINTERPOSSESSIVE) {
			ram_free(ex->stack[ex->sp - 1].as.ptr.block);
		}

		ex->stack[ex->sp - 1].as.ptr.block = NULL;

	}

	ex->stack[ex->sp - 1].typeselector = NULL;

	//todo: check if this is the correct type, and if it isn't, null it.  also might have to addref
	//also, maybe instead of putting it in this instruction, push a typeT*, then later do a test


	return tnext(t);
}

//changes a pointer from one virtual type to another virtual type.  Requires the source selector table has a link to the new selector table 
tokenT* hchselector(exectxT* ex, tokenT* t) {  
	
	typeT* seltable = ex->stack[ex->sp - 1].typeselector;
	symbolT* selected = zvec_get_at(seltable->selectors, t->val.as.n32); //grab the nth item from the selector table

	/*
	printf(" selector %d for virtual  %s is  %d for  virtual %s\n",
		t->val.as.n32, seltable->ref->name,
		selected->offset, seltable->parent->name);*/
	
	ex->stack[(ex->sp) - 1].typeselector = selected->type; //load other selector

 	return tnext(t);
}


tokenT* hindex(exectxT* ex, tokenT* t) {	//index into array
	//don't index NULL arrays (keep null ptr)
	if (!ex->stack[ex->sp - 2].as.ptr.block) {
		ERR("index null array");
		
		return NULL;
	}
	
	ex->stack[ex->sp - 2].as.ptr.offset += ex->stack[ex->sp - 1].as.n32 * t->val.as.n32;
	
	//xprintf(" index using multiplier %d\n", t->val.as.n32);
	
	ex->sp--;
	return tnext(t);
}

tokenT* harrayinfo(exectxT* ex, tokenT* t) {	//edit array

 	void* array = ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset;
	ex->stack[ex->sp - 1].as.ptr.block = 0;

	if (t->val.as.n32 == 0)
		ex->stack[ex->sp - 1].as.n32 = zarray_size(array);
	else if (t->val.as.n32 == 1)
		ex->stack[ex->sp - 1].as.n32 = zarray_count(array);
	else if (t->val.as.n32 == 2) {
		 zarray_use(array, ex->stack[ex->sp - 2].as.n32);
		 ex->sp-=2;
	} else if (t->val.as.n32 == 3) {  //resize array
		int newsize = ex->stack[ex->sp - 2].as.n32;
		ex->stack[ex->sp - 2].as.ptr.block = zarray_resizef(array, t->ty->ref->ref->size, newsize, NULL);

		//todo: if lowering size and there are possessive pointers, need to free them


		ex->sp -= 1;
	}

	return tnext(t);
}


tokenT* hboolshort(exectxT* ex, tokenT* t) {

	//run first instruction
	tokenT* subs = tsub(t);
	instruction handler = subs->handler;

	if (!handler)
		ERR(" Null handler on cond's first arg\n");

	if (!subs->skipargs)
		subs = evalsubs(ex, subs);

	subs = handler(ex, subs);	//pushes true or false on stack

	//AND and  result of false means we skip the second instruction
	if (t->val.as.n32 == 1 && !ex->stack[ex->sp - 1].as.n32)
		return tnext(t);	//leave the false on the stack and continue on

	//OR with a result of true means we skip the second instruction
	if (t->val.as.n32 == 0 && ex->stack[ex->sp - 1].as.n32)
		return tnext(t); //leave the true on the stack and continue on

	ex->sp--; //pop the first result.

		handler = subs->handler;
	if (!subs->skipargs)
		subs = evalsubs(ex, subs);

	subs = handler(ex, subs);	//pushes true or false on stack

	//the result is now whatever just got pushed
	
	return tnext(t); //next one
}

tokenT* hcondblock (exectxT* ex, tokenT* t){	//if first sub is true, execute the rest of the subs list
	
	//run first instruction
	tokenT* subs =tsub(t);
	
	instruction handler =  subs->handler;

	if (!handler)
		ERR(" Null handler on cond's first arg\n");
		
	if (!subs->skipargs)
		subs = evalsubs(ex, subs);

	subs = handler(ex, subs); 
	

	//check result;
	
	ex->sp--;
	
	if ( ex->stack[ex->sp].as.n32){
			//xprintf (" COND is true, execute body\n");
			exe(ex, subs); //continue this  

			if (t->val.as.n32 == 1) {
				//xprintf(" EXIT CHAIN\n");
				return NULL;
			}
	} 
	//xprintf(" COND was false, so call next in chain\n");
	return tnext(t); //next one
}

tokenT* hbreakblock (exectxT* ex, tokenT* t) { 
	ex->stop = STOPBLOCK; //flag to signal block breakage
	return NULL;//stop running this block of instructions
}
tokenT* hbreakcontinueloop (exectxT* ex, tokenT* t) {
	
	if (t->val.as.n32 == 1)
 		ex->stop = RELOOP;
	else
		ex->stop=STOPLOOP; //flag to signal loop breakage

	return NULL;//stop running this block of instructionsST
}

tokenT* hloop (exectxT* ex, tokenT* t) { //run subs continously until STOPLOOP is set
	
	while(!ex->stop){
		exe(ex, tsub(t) );; 
		if (ex->stop == RELOOP)
			ex->stop = 0;
	}
	if(ex->stop== STOPLOOP)
		ex->stop=0;
	
	return tnext(t);	
}

//parses / compiles a list of tokens, if it hasn't been already
//executes the list of tokene

parsectxT* global = NULL;
parsectxT* immediate_parse = NULL;
parsectxT* mkcontext(char* name);
tokenT* parse(parsectxT* pc, tokenT* t);

int debugtimes = 0;

tokenT* h_compile(exectxT* ex, tokenT* t) {
	tokenT* code = ex->stack[ex->sp-1].as.ptr.offset + (char*)ex->stack[ex->sp-1].as.ptr.block;

	xprintf("\nCODE TO COMPILE ---\n");
	printList(code, code, 0, 2);
	xprintf("---\n");

	//need to see if its compiled
	if (!code->val.as.ptr.block) {
		//needs to compile

		if (code->restrict_parse_context) {
			ERR("Compiling code that contains subtree from other context: not allowed\n");
		}

		parsectxT* pc = mkcontext("dynamic");
		symbolT* s = mkSymbol(NULL, "dynamic", code->ty, NULL);
		pc->symfrom = s;
		s->subctx = pc;
		s->tokens = code; // ram_addref(code); NOT addred, because the symbol's tokens now is the only reference to this code block
		
		code->sym = s;  //toek syms are not freed; they are freed by whatever owns the symbol, usually sumbol table.  in this case, since its an anonymous function, the symbol is passed back to the user as a possessive pointer to keep track of
		pc->type = code->ty;
		pc->endable = 1;
		pc->parent = t->val.as.ptr.block;
		parse(pc, tsub(code));
	}
	else {
		ERR("Double-compile?");
	}

	//ex->stack[ex->sp-1].as.ptr.level = 0; //heap object  NOT doing this, since this should only be fed possessive code pointers directly from a subst token
	ex->stack[ex->sp - 1].as.ptr.block = code->sym;  //track sym instead
	ex->stack[ex->sp - 1].as.ptr.offset = 0;

	return tnext(t);
}

tokenT* hload32 (exectxT* ex, tokenT* t) {

	zint32 i = DEREF(zint32, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	//xprintf(" Loaded 32 %d   from +%x\n", i,ex->stack[ex->sp-1].as.ptr.offset );
	ex->stack[ex->sp-1].as.ptr.block=NULL;
	ex->stack[ex->sp-1].as.z32 = i;
	
	return tnext(t);
}


tokenT* hstore32 (exectxT* ex, tokenT* t) {
	
	DEREF(zint32, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = ex->stack[ex->sp-2].as.z32;
	
	ex->sp-=2;
	
	return tnext(t);
}

tokenT* hstore32np(exectxT* ex, tokenT* t) {
	tokenT* next = hstore32(ex, t);
	ex->sp++; //put item back on stack
	return next;
}

tokenT* hloadcptr(exectxT* ex, tokenT* t) {
	void* i = DEREF(void*, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset);
	//xprintf(" Loaded 32 %d   from +%x\n", i,ex->stack[ex->sp-1].as.ptr.offset );
	ex->stack[ex->sp - 1].as.ptr.block = i;
	ex->stack[ex->sp - 1].as.ptr.offset = 0;
	ex->stack[ex->sp - 1].as.ptr.level = 9999; //

	return tnext(t);
}


tokenT* hstorecptr(exectxT* ex, tokenT* t) {
	//store block+offset as a flattened c pointer
	DEREF(void*, ex->stack[ex->sp - 1].as.ptr.block, ex->stack[ex->sp - 1].as.ptr.offset) = ex->stack[ex->sp - 2].as.ptr.block + ex->stack[ex->sp - 2].as.ptr.offset;
	ex->sp -= 2;
	return tnext(t);
}

tokenT* hload8 (exectxT* ex, tokenT* t) {
	
#ifdef EXEDEBUG 
	xprintf(" Load byte at %p+%d\n",  ex->stack[ex->sp-1].as.ptr.block,  ex->stack[ex->sp-1].as.ptr.block);
#endif	
	zbyte i = DEREF(zbyte, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	ex->stack[ex->sp-1].as.ptr.block=NULL;
	ex->stack[ex->sp-1].as.n32 = i;
		
	return tnext(t);
}

tokenT* hstore8 (exectxT* ex, tokenT* t) {
		
#ifdef EXEDEBUG
	xprintf(" Store byte at %p+%d\n",  ex->stack[ex->sp-1].as.ptr.block,  ex->stack[ex->sp-1].as.ptr.offset);
#endif
	DEREF(zbyte, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = (zbyte) ex->stack[ex->sp-2].as.n32;
	ex->sp-=2;
	return tnext(t);
}

tokenT* hloadbytes(exectxT* ex, tokenT* t) {
	void* thing = ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset;
	memcpy(&ex->stack[ex->sp - 1], thing, t->val.as.n32);
	return tnext(t);
}

tokenT* hstorebytes(exectxT* ex, tokenT* t) {

	void* thing = &ex->stack[ex->sp - 2];
	void* destination = ex->stack[ex->sp - 1].as.ptr.block + ex->stack[ex->sp - 1].as.ptr.offset;

	memcpy(destination, thing, t->val.as.n32);
	ex->sp -= 2;
	return tnext(t);
}


tokenT* hprint32 (exectxT* ex, tokenT* t) {
	
	ex->sp--;
	printf("%d", ex->stack[ex->sp].as.z32);
	return tnext(t);
}

#ifdef FLOAT

tokenT* hprintfloat (exectxT* ex, tokenT* t) {
	ex->sp--;
	printf("%.10g", ex->stack[ex->sp].as.f);
	return tnext(t);
}

tokenT* hfload (exectxT* ex, tokenT* t) {
	FLOAT f = DEREF(FLOAT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset);
	ex->stack[ex->sp-1].as.ptr.block=NULL;
	ex->stack[ex->sp-1].as.f = f;
	return tnext(t);
}

tokenT* hfstore (exectxT* ex, tokenT* t) {
	DEREF(FLOAT, ex->stack[ex->sp-1].as.ptr.block, ex->stack[ex->sp-1].as.ptr.offset) = ex->stack[ex->sp-2].as.f;
	ex->sp-=2;
	return tnext(t);
}
#endif

tokenT* hprintchar (exectxT* ex, tokenT* t) {
	ex->sp--;
	printf("%c", ex->stack[ex->sp].as.z32);
	return tnext(t);
}

tokenT* hprintptr (exectxT* ex, tokenT* t) {
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
	ex->stack[ex->sp-2].as.RESULTAS  = ex->stack[ex->sp-2].as.AS  OPERATOR   ex->stack[ex->sp-1].as.AS;	\
	ex->sp--;							\
	return tnext(t);							\
}

tokenT* hadd32(exectxT* ex, tokenT* t) {				
		ex->stack[ex->sp - 2].as.z32 = ex->stack[ex->sp - 2].as.z32  +   ex->stack[ex->sp - 1].as.z32;
		ex->sp--;						
		return tnext(t);			
}

//integer arithmetic
//BINOP(hadd32, z32, z32, + )
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
	ex->stack[ex->sp-1].as.RESULTAS  =  OPERATOR (  ex->stack[ex->sp-1].as.AS );	\
	return tnext(t);							\
}



UNOP(hboolnot, z32, z32, !)
UNOP(hinvert32, z32, z32, ~)
UNOP(hneg32, z32, z32, -)

#define BINOPFUNC(NAME, AS, FUNC)\
tokenT* NAME (exectxT* ex, tokenT* t) {						\
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

tokenT* hreturn (exectxT* ex, tokenT* t){
	
	ex->stop=STOPFUNC;  //returning from function

	if (t->ty) {  

		xprintf("Pushing return type (return from immediate)\n");

		//if returning from an immediate block, put the return type on the stack too
		//if the immediate block is not returning a value, t->ty should have been set to tImmediate anyway
		//because returning from here is expecting to take the type off of the stack
		ex->stack[ex->sp++].as.type = t->ty;
	}
	return NULL; //stop instructions stream (process subs first, as they might be return values or something)
}

tokenT* hgroup (exectxT* ex, tokenT* t){
	//exe(ex, tsub(t)); //evaluate all the args
	
	if ((ex->stop == STOPBLOCK) && t->val.as.n32) {
		ex->stop = 0;
	}
	
	//if (ex->stop)
	//	return NULL;  //stopping

	return tnext(t);
}

tokenT* hcall (exectxT* ex, tokenT* t) {
		
	tokenT* argsubs = tsub(t);
	
	int indirect = 0;

	tokenT* next = tnext(t);
	tokenT* origt = t;

	symbolT* sym = t->sym;

	


	if (t->val.as.n32 == 2) {
		//calling indirectly.  top of stack has pointer to a variable that contains the function pointer

		vptrT destination;
		ex->sp--;
		destination = DEREF(vptrT, ex->stack[ex->sp].as.ptr.block, ex->stack[ex->sp].as.ptr.offset);
		indirect = 1;
		sym = destination.block;
		if (!sym)
			ERR("Null proc pointer\n");
		t = sym->tokens;
	}
	

	


	//xprintf(" CALLING PROC %s\n", t->str);
	//xprintf("pre call SP:%d  FP:%d\n", ex->sp, ex->fp); 
	
	zuint32 oldfp = ex->fp;  //save frame pointer

	if (sym->isPrototype){
			ERR("Function body missing:%s\n", t->str );
	}
	
	symbolT* selected = sym;

	//what happens if an overridden 't' from the stack is a selector?  is that possible?
	
	if (sym->isSelector) {
 		
		typeT* seltable = ex->stack[ex->sp  - zvec_count(t->sym->type->members) + t->sym->selectorArg].typeselector;
		
		//printf(" CALL SELECTOR %d from %s's table for %s\n", t->sym->selectorNum, seltable->parent->name, seltable->ref->name);

		selected = zvec_get_at(seltable->selectors, t->sym->selectorNum); //grab the nth function from the selector table
		
	}

	ex->fp=ex->sp;
		
	if (!selected->subctx) {
		//calling a primitive
		//args were already evaluated above
		if (selected->handler) {
			tokenT emptyt;
			memset(&emptyt, 0, sizeof(emptyt));
			selected->handler(ex, &emptyt);
		
			return next;
		}
	}

	if (selected->isShader) {

		//free things that are marked 'trackpossptr'
		//these should really be freed later, but since they are trackpossptr, they should have at least 1 other refcount.
		//trackpossptr was only so that things can't be invalidated while pointers to them are on the stack.
		//for what we are using shaders for, this isn't a problem yet
		tokenT* tv = argsubs;
		int i;
		int count = zvec_count(sym->type->members);

		for (i = 0; i < count; i++) {
			typeT* m = zvec_get_at(sym->type->members, i);

			float* f = ex->stack[ex->sp - count + i].as.ptr.block;
			if (tv->trackpossptr)
				ram_free(ex->stack[ex->sp - count + i].as.ptr.block);

			tv = tnext(tv);

		}

		ex->stack[ex->sp].as.ptr.block = sym->shaderdata;  //put symbol on stack instead of calling
		ex->stack[ex->sp++].as.ptr.offset = 0;

		return tnext(t);
	}




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

//	if (!indirect)
	//	exe(ex, (tokenT*)tnext(tsub(selected->tokens)));
	//else
	
	exe(ex, (tokenT*)tsub(selected->tokens));
	
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
	
	tokenT* tv = argsubs;
	int addfp = 0;

	if (sym->type->members && argsubs) {
		int i;
		int count = zvec_count(sym->type->members);

		for (i=0;i<  count; i++) {
			typeT* m = zvec_get_at(sym->type->members, i);
			//xprintf(" ARG %d is ", i);
			//printType(m, ZTRUE, ZFALSE);
						
			if (i == 0 && m->ref == tEmptyStack) {
				addfp = 1;  //not a real value; so don't popit
				//don't advance tv, since it points at the 1st real arg
				//tv = tnext(tv);  
				continue;
			}

			
			if ( (tv->trackpossptr)|| resolve_like_type(m->ref,sym->type->members, NULL, ZFALSE)->category == POINTERPOSSESSIVE){
				//if (m->ref->category == LIKE) {
					//printf("free like arg\n");
				//}
				ram_free(ex->stack[ex->fp - count + i].as.ptr.block);
			}

			tv = tnext(tv);
			
		}
		ex->fp += -count + addfp;
	}
	
	if (sym->type->ref) {
			
		ex->stack[ex->fp++] = ex->stack[ex->sp-1];  
		
		if (sym->type->ref->category == POINTERUSER && sym->type->ref != tType){ 
			ERR("Can't return non-possessive pointer!\n"); //exception above for types
		}
		
#ifdef EXEDEBUG
		xprintf(" Function returns ");printType(sym->type->ref,1,1);
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
	return next;
}

valueT* callint(symbolT* f, valueT* stack, int sp);

zbool struct_clean(void* v, typeT* ty){
	
	//xprintf(" Clean for %s\n", (ty)->name);
	//printType(ty, ZTRUE, ZFALSE);
	if (ty->destructorproc) {
	
		valueT val[100];
		memset(&val, 0, sizeof(val));
		val[0].as.ptr.block = v;

		callint(ty->destructorproc, val, 1 );
	}

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
	
	//TODO: allow destructors for alloced structs?
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
	//exe(ex, tsub(t)); //evaluate all the args
	ex->sp--;
#ifdef EXEDEBUG	
	xprintf(" TO TRASH %p\n", ex->stack[ex->sp].as.ptr.block );
#endif
	ram_free(ex->stack[ex->sp].as.ptr.block);
	ex->stack[ex->sp].as.ptr.block=0;
	return tnext(t);
}

tokenT* haddref(exectxT* ex, tokenT* t) {
	//exe(ex, tsub(t)); //evaluate all the args
	
#ifdef EXEDEBUG
	xprintf(" TO KEEP %p\n", ex->stack[ex->sp-1].as.ptr.block );
#endif
	//this also does not pop off the stack, the value stays on
	
	ram_addref(ex->stack[ex->sp-1].as.ptr.block);  //retention is on the BLOCK.  Means you can addref PART of a block... the whole block will be kept and waste memory, but this is OK for now; it will eventually be freed.  
	
	return tnext(t);
}

#define HANDLER(CONTEXT, NAME)	mkSymbol( CONTEXT,  #NAME, tPrimitive, h ## NAME)

void addhandlers(struct parsectxS* pctx) {

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
	HANDLER(pctx, store32np);
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

	HANDLER(pctx, nop);
	HANDLER(pctx, tokenval);
	HANDLER(pctx, unimplemented);

	//things that are generally not directly user-accessible

	//immediate values
	HANDLER(pctx, constant);
	HANDLER(pctx, constantaddref);

	//dynamic code generation
	HANDLER(pctx, subst);
	HANDLER(pctx, _compile);
	
	//flow control
	HANDLER(pctx, group);  //a group of statements
	HANDLER(pctx, condblock);  //conditional execution (IF)
	HANDLER(pctx, breakblock);
	HANDLER(pctx, loop);
	HANDLER(pctx, breakcontinueloop);
	
	//reflection 
	HANDLER(pctx, symtoken); //get symbols for an executable proc
	
	//virtual types
	HANDLER(pctx, addselector);	//turn a regular pointer to virtual pointer (adds a selector table)
	HANDLER(pctx, chselector);  //switches a virtual type to another
	HANDLER(pctx, testtype);	//turn a virtual pointer in to a specific type (NULL if wanted type is not true)
	HANDLER(pctx, selectorptr); //get pointer to data member of virtual item

	HANDLER(pctx, call);
	HANDLER(pctx, return);
	
	HANDLER(pctx, redirectsub);
		
	//load/store
	HANDLER(pctx, loadbytes);
	HANDLER(pctx, storebytes);
	HANDLER(pctx, loadptr);
	HANDLER(pctx, takeptr);
	HANDLER(pctx, storeptr);
	HANDLER(pctx, storeptrnp);
	
	//read value from arg stack
	HANDLER(pctx, stackread);

	//get pointer to item on arg stack (only for stacked structs)
	HANDLER(pctx, stackptr);

	//memory allocation
	HANDLER(pctx, alloc);
	HANDLER(pctx, allocarray);
	HANDLER(pctx, free);	//dec ref count and free if zero
	HANDLER(pctx, addref);	//inc ref count


	//pointer arithmetic to index into arrays to get struct members
	HANDLER(pctx, offsetptr);
	HANDLER(pctx, index);
	HANDLER(pctx, arrayinfo);	//get size of array

	//get pointer to local, immediate, or global variable
	HANDLER(pctx, local);
	HANDLER(pctx, immvar);
	HANDLER(pctx, global);
	
#endif

}

zbool exectx_cleanup(void* v) {
	exectxT* e = v;
	
	ram_free(e->immediatevars);
	ram_free(e->globalvars);
	ram_free(e->vars);
	ram_free(e->stack);
	return ZTRUE;
}

#define MIN_IMMEDIATE_SIZE 64*1024

valueT* callint(symbolT* f, valueT* stack, int sp) {

	exectxT ex;
	memset(&ex, 0, sizeof(ex));
	ex.sp = sp;
	ex.stack = stack;
	ex.fp = sp;
	
	tokenT t;
	memset(&t, 0, sizeof(t));
	t.sym = f;
	ex.globalvars = global->exec->globalvars;
	hcall(&ex, &t);
	if (ex.sp > 0)
		return ex.stack + ex.sp - 1;
	return NULL;
}


tokenT* hDEBUG(exectxT* ex, tokenT* t) {  //push constant on stack
	return hconstant(ex, t);
}

void start(parsectxT* pctx, tokenT* t, valueT* initial){

	if (!pctx->exec) {
		pctx->exec = ram_alloc(sizeof(exectxT), exectx_cleanup);
		pctx->exec->stack = ram_alloc(sizeof(valueT) * 100, NULL);
	
		if (pctx == global) {
			pctx->exec->globalvars = ram_alloc(pctx->size, NULL);
		}
		else if (pctx == immediate_parse) {

			zuint32 immsize = pctx->size < MIN_IMMEDIATE_SIZE ? MIN_IMMEDIATE_SIZE : pctx->size;
			
			pctx->exec->immediatevars = ram_alloc_shadow(immsize, NULL, sizeof(int));
			//store vars size in the shadow allocation.  TODO: consider adding ram_getsize to zmem
			int* asize = ram_shadow(pctx->exec->immediatevars);
			*asize = immsize;

			pctx->exec->in_immediate = pctx;

		} else {//local variable frame for functions
			pctx->exec->vars = ram_alloc(pctx->size, NULL);
		}

		pctx->exec->sp = 0;
	}
	else  {
		//printf("resuming immediate context\n");
		pctx->exec->debugstack = 1;
	}
	//spsave = pctx->exec->sp;
	
	pctx->exec->stop = 0;

	if (pctx == immediate_parse &&  pctx->exec->immediatevars) {
		//check vars space is large enough
		int* asize = ram_shadow(pctx->exec->immediatevars);
		if (pctx->size > *asize) {
			ERR("Out of immediate space\n");
			/*
			printf(" Resizing immediate vars from %d to %d\n", *asize, pctx->size);

			int oldsize = *asize;

			pctx->exec->immediatevars = ram_resize(pctx->exec->immediatevars, pctx->size, NULL);
			asize = ram_shadow(pctx->exec->immediatevars);
			*asize = pctx->size;

			//clear the additional space (in case it has garbage possessive pointers)
			memset(oldsize + (char*)pctx->exec->immediatevars, 0, pctx->size - oldsize );
			*/
			

		}
	}

	
#ifdef EXEDEBUG	
	xprintf(" PCTX %d bytes\n", pctx->size);
#endif	
	

#ifdef EXEDEBUG	
	xprintf("EXE\n");
#endif	
	/*
	for (int i=0;i<32;i++){
			xprintf(",%02x ",  exectx->globalvars[i]  );
			xprintf(",%02x ",  exectx->globalvars[i]  );
			
	}
	xprintf(" \n");*/
	
	int oldfp = 0;
	int oldsp = 0;
	if (initial) {
		//push a value, setup like 1 function call
		//careful, this might not be correct
		oldfp = pctx->exec->fp;
		pctx->exec->stack[pctx->exec->sp++] = *initial;
		pctx->exec->fp = pctx->exec->sp;
	}

	exe(pctx->exec, t);
	
	if (initial) {
		pctx->exec->fp = oldfp;
		pctx->exec->sp = oldsp;
	}

	//pctx->exec->sp = spsave;

	//ram_free(exectx->stack);

	//clean_context_pointers(pctx->symbols, exectx->globalvars);
	//ram_free(exectx->globalvars);
	//ram_free(exectx);
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
		
		//propagate uselocal, count how many children use local
		//propagate uselocal, count how many children use local
		under->useslocal += t->useslocal;
		//under->useslocal |= t->useslocal;
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

		//propagate uselocal, count how many children use local
		//if (under->useslocal)
			//t->useslocal++;
		under->useslocal += t->useslocal;
		//under->useslocal |= t->useslocal;
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

		if (!named && t->tok == KLIKE) {
			tnext(t)->ty = mkType(LIKE, NULL, tnext(t)->str, 0);
			t = tnext(t);
			fold(tprev(t), t);
			next = zlist_next(t);
			named = ZTRUE;
			continue;
		}

		if (!named && t->tok == PASSTHRU) {
			if ((t->handler == hconstant) && (t->ty == tType)) {
				t->handler = hnop;
				t->ty = t->val.as.type;
				next = tnext(t);
				named = ZTRUE;
				continue;

			}
		}


		if (!named && t->tok == NAME){ //simple typename, but only 1 per 'type'

			named=ZTRUE;

			t->ty = findType(NAMED, NULL, t->str, 0);
			if (!t->ty){
					t->ty = mkType(PENDING, NULL, t->str, 0);
			}
			next = zlist_next(t);
			continue;
		}

		if (tprev(t) && tprev(t)->ty && tprev(t)->ty->islike) {
			if (t->tok == '@') {
				xprintf("Making deref for type:\n");
				printType(tprev(t)->ty, 1, 0);

				t->ty = mkType(DEREFTYPE, tprev(t)->ty, NULL, 0);

				next = zlist_next(t);
				fold(tprev(t), t);
				continue;
			}
			if (t->tok == NAME && t->str[0] == '.') {

				t->ty = mkType(DEREFTYPE, tprev(t)->ty, t->str + 1, 0);
				next = zlist_next(t);
				fold(tprev(t), t);

				continue;
			}
		}

		if (t->tok == KCPOINTER) { //pointer type
			t->ty = findType(CPOINTER, tprev(t)->ty, NULL, 0);
			next = zlist_next(t);
			fold(tprev(t), t);
			continue;
		}

		if (t->tok == '&'){ //pointer type
			t->ty = findType( POINTERUSER, tprev(t)->ty, NULL,0);
			next = zlist_next(t);
			fold(tprev(t),t);
			continue;
		}

		if (t->tok == '%'){ //pointer type
			xprintf("Making pointer for type:\n");
			printType(tprev(t)->ty, 1, 0);
			t->ty = findType( POINTERPOSSESSIVE, tprev(t)->ty, NULL,0);
			xprintf("Found pointer type:\n");
			printType(t->ty, 1, 0);

			next = zlist_next(t);
			fold(tprev(t),t);
			continue;
		}

				
		if (t->tok =='(' && !named){ //type list for function parameters & return value
			tokenT* S = t;

			typeT* ty = mkType(FUNCTION, NULL, NULL, 0); 
			
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
	
	for (; t; t = next) {
		char* name = NULL;
		typeT* type = NULL;

		if (t->tok == KEND)
			break;

		if (t->tok == ')') {
			if (parent->category != FUNCTION)
				ERR("Unexpected )\n");
			break;
		}

		if ((t->tok == PAIR('-', '>'))) {
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

		int argStyle = 0; //regular normal arg... execute the code to get the value to pass.  normal evaluation

		if (t->tok == KCODE) {

			argStyle = 1; // the code is the arg

			t = tnext(t);
			//compile function arg to match on expression type (like Z32), but call with the subtree (Code%)
		}

		if (t->tok == KPER) {
			argStyle = 2; //function is passed an array, but the function sees a singular value (for parallel computing/shaders)

			t = tnext(t);
		}

		if (t->tok == NAME && tnext(t)->tok == '&' && reqname){		//if we are naming parameters, and the name is followed by &
			// so like  a&:Real&	
			argStyle = 3; //function accepts pointer, but inside function it is automatically dereferenced
						  //the pointer cannot be written to inside the function
			ram_free(tremove(tnext(t))); //remove &

		}
						
		t = parseVar(t, &name, &type  );
		//printList((tokenT*)t->zlistnode.prev->prev->prev, t, ENDFILE,0);
		
		
	//	if (type && type->islike) {
		//	type->ref = resolve_like_type(type, parent->members, NULL, ZFALSE);
		//}

		
		if ( (reqname && !name) || !type){
 			ERR(" type or name missing for struct member or function arg\n");
		} 

		
		if (type->category > LAST_REAL_TYPE && (!parent || (parent->category != FUNCTION)) ){
			if ((type->category == PENDING) && (type->stacked))
				printf("allowing stacked struct in struct/fcall\n");
			else
			ERR("Name '%s' category %d cannot be in struct/fcall\n",  safestr(type->name),   type->category);
		}

		if (parent && !reqname){ // !reqname means past the arrow ->
			if (parent->ref){
				ERR("Functions can only return 1 value\n");
			} else {
				parent->ref= type ;
			}
		} else	if (parent && parent->members) {

			if (argStyle == 1)
				type = findType(SUBTREE, type, NULL, 0);
							
			typeT* mty = mkType(MEMBER, type, name, offset);
			
			if (argStyle == 2)
				mty->isPer = 1;

			if (argStyle == 3)
				mty->deref = 1;

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

	if (parent) {
		xprintf("Parsed type list for this type:\n");
		printType(parent, 1, 0);
	}

	return t; //closing paren on func parm list OR 'end' in typedef
}

/* Parse a variable definition */
tokenT*  parseVar(tokenT* t,  char** nameOut, typeT** typeOut) {
	char* name=NULL;

	tokenT* S = t;

	if (tnext(t)->tok == ':'){
		t->handler = hnop;
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
	ram_free(pc->exec);
	ram_free(pc->name);
	return ZTRUE;
}

parsectxT* mkcontext(char* name)
{
	parsectxT* c = ram_alloc(sizeof(parsectxT), parsectx_cleanup);
	c->name = zstrdup(name);
	c->symbols = zvec_mk(NULL, 10);
	return c;
}



void checkUsage(tokenT* start, tokenT* end){
	
	tokenT* t = start;
				
	while (t && t != end){
	
		if (t->ty && t->tok != KRETURN && t->tok != KEND){  //if value on this level has a type, its an unused value.  Exception for KEND and KRETURN; that ty is the return type

			xprintf("\n value not used:{{{ \n");

			printList(t, t, -1,1);
			
			printType(t->ty, ZFALSE, ZFALSE);
			xprintf("\n}}}\n");
			
			
				
			if (t->ty->category == POINTERPOSSESSIVE) {
				xprintf("  Abandoning possessive pointer will cause a memory leak\n");
				printList(start, t, 0, 20);
				ERR(" Abandoning possessive pointer will cause a memory leak\n");
			}
			
		}
		t = tnext(t);
		
	}
	
	
} 
//checks that real_type implements all the selectors of vtype
void check_implementation(typeT* vtype, typeT* real_type, typeT* real_type_vmember) {

	if (real_type_vmember->selectors) {

		int a = zvec_count(real_type_vmember->selectors);
		int b = zvec_count(vtype->selectors);
		

		/*
		printf(" Implementation of %s has %d selectors and virtual type %s has %d selectors. \n",
			real_type->name, a, vtype->name, b);
	*/

		if (a != b) {
			ERR(" Implementation of %s has %d selectors but virtual type %s has %d selectors.  Extra selectors were added to virtual type after usage\n",
				real_type->name, a, vtype->name, b);
		}

		return; //already found all the functions for it
	}
	
	real_type_vmember->selectors = zvec_mk(NULL, zvec_count(vtype->selectors)); 
	real_type_vmember->parent = real_type;
	//zvec_disown(real_type_vmember->selectors);

	int n;
	for (n = 0; n < zvec_count(vtype->selectors); n++) {
		symbolT* vselector = zvec_get_at(vtype->selectors, n);
		xprintf(" Need to find member %s implementation of %s ",
			real_type->name,
			vselector->name);
		
		/*
		if (vselector->isSelector == 4) {
			printf(" virtual to virtual selector\n");
		}*/


		if ((vselector->isSelector == 2)  || (vselector->isSelector == 4)) {  //if data selector
			
			//find offset in real type member
			int j;
			for (j = 0; j < zvec_count(real_type->members); j++) {
				typeT* member = zvec_get_at(real_type->members, j);
				if (!strcmp(member->name, vselector->name)) {
					
					
					
					if ((member->ref == vselector->type) || compatibleType(member->ref, vselector->type)) {
						
						//make a symbol for it.
						symbolT* s;

						if (vselector->isSelector == 4) { //virtual selector
							//need to check the real type supports this other type
							check_implementation(vselector->type, real_type, member);
							s = mkSymbol(NULL, member->name, member, NULL); 
						}
						else {

							s = mkSymbol(NULL, member->name, member->ref, NULL);
							s->offset = member->offset;
						}

						zvec_add(real_type_vmember->selectors, s); //add the found function to the real type's virtual field for the virtual type
						break;
					}
					else
					{
						ERR(" Type of %s mismatch in %s and %s\n", vselector->name, real_type->name, vtype->name);
					}

				}
			}
			if (j == zvec_count(real_type->members))
				ERR("Type %s does not contain %s's field %s\n", real_type->name, vtype->name, vselector->name);

			continue;
		}

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

			zvec_add(real_type_vmember->selectors, ram_addref(s)); //add the found function to the real type's virtual field for the virtual type

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

tokenT* quote(parsectxT* pc, tokenT* t, char* endString) {
	while (!t->str || strcmp(t->str, endString)) {

		if ((t->tok == '$') || (t->tok == '\'')) {

			tokenT* dollar = t;
			

			t = tnext(t); //variable name
			if (tnext(t)->tok == KKEEP || tnext(t)->tok == KTAKE) {  //allow possessive pointers to be quoted
				t = tnext(t);
			}
			zlist_insert_node_after(t, mkToken(KEND, "end", 3));
			pc->endable++;
			t = parse(pc, tnext(dollar)); //t is 'end' after this call
			t = tnext(t);   //move past 'end
			ram_free(tremove(tprev(t))); //delete 'end'
			lfold(dollar, t);
			continue; 
		}

		t = tnext(t);

	}
	return t;

}

tokenT*  parse(parsectxT* pc, tokenT* t) {
	
	tokenT* ttop = t;
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
	
	//tokenT* ignoresigned = NULL; //set to 't' when symbol not found, to try to find what if its unsigned

	int matchApprox = MATCH_EXACT;
	tokenT* matchApproxToken = NULL;


	while ( t) {
 		if (t->tok==ENDFILE)
			t->handler = hbreakblock; //don't really do anything

		if (t->tok==ENDFILE || t->tok== PASTENDFILE)
			return t;

		lastfile = t->sourcefile;
		lastline = t->line;
		
		if (t != matchApproxToken) { //if this is a new token, try matching exactly first
			matchApproxToken = t;	//we are on this token
			matchApprox = MATCH_EXACT;	
		}

			
		//printList( tprev(tprprintf(" Token %s ", t->str);ev(tprev(t))), t, PASTENDFILE,0);
		//xprintf(" parse token %s\n", t->str);
		//getc(stdin);
		char* name=NULL;
		char* pname=NULL;
		typeT* type=NULL;
		handler = NULL;
		
		ts = t;

		int dataselector = 0;
		int nopop = 0;
		

		switch (t->tok) {


		case '~':
			t = tnext(t);
			ram_free(tremove(tprev(t)));
			t->breakpoint = ZTRUE;
			continue;

		case KCODE:
			typeT* codetype = NULL;

			t = tnext(t);

			if (t->tok == '(') {
				//this code is TYPED
				t = parseType(t);
				codetype = tprev(t)->ty;
				ram_free(tremove(tprev(t))); //remove the datatype token
			}


			char* endString = ram_addref(t->str);
			t = tnext(t);
			ram_free(tremove(tprev(t))); //remove  start delimiter token
			t = quote(pc, t, endString);
			ram_free(endString);

			t = tnext(t);

			if (codetype) {
				tprev(t)->tok = KEND; //turn the end delimiter token into an END token.  Now the quote can be parsed similar to a function body
				ram_free(tprev(t)->str);
				tprev(t)->str = ram_strdup("");
			}
			else
				ram_free(tremove(tprev(t))); //no untyped code (raw token snippets), delete this 



			lfold(ts, t);


			ts->handler = hsubst;
			ts->skipargs = ZTRUE;
			//ts->val.as.ptr.block = tsub(t);
			//ts->val.as.ptr.offset = 0;

			if (codetype) {
				//code assigned a proc type.  This needs to be compiled into a function
				ts->ty = codetype;
				tokenT* comp = mkToken(COMPILE, "SYScompile", 0);
				zlist_insert_node_after(&ts->zlistnode, comp);
				t = tnext(ts);


			}
			else {
				//untyped code:  just a list of tokens
				ts->ty = findType(POINTERPOSSESSIVE, tCode, NULL, 0);
			}


			continue;

		case COMPILE:

			if (tprev(t)->ty && (tprev(t)->ty->category == FUNCTION)) {
				typeT* proctype = tprev(t)->ty;
				fold(tprev(ts), t);
				t = tnext(t);

				ts->handler = h_compile;
				ts->val.as.ptr.block = ram_addref(pc);
				ts->val_to_free = ZTRUE;
				ts->ty = findType(POINTERPOSSESSIVE, proctype, NULL, 0);

				continue;
			}

			break;

		case KAND:
		case KOR:

			//two booleans
			if (tprev(t)->ty == tBit && tprev(tprev(t))->ty == tBit) {
				fold(tprev(tprev(t)), t);

				t->handler = hboolshort;
				t->ty = tBit;
				t->skipargs = ZTRUE;
				if (t->tok == KAND)
					t->val.as.n32 = 1;
				else
					t->val.as.n32 = 0;

				t = tnext(t);
				continue;
			}

			break;



		case KSKIP:
			t->handler = hbreakcontinueloop;
			t->val.as.n32 = 1;
			t = tnext(t);
			break;

		case KEND:

			if (pc->endable && pc->type == tImmediate) {
				t->handler = hreturn;
				t->ty = tImmediate;
			}


			if ((pc->endable == 1) && pc->type && (pc->type->category == FUNCTION)) {

				if (pc->type->ref && (tprev(t)->tok != KRETURN))
					ERR("End of proc without returning a value\n");

				t->handler = hreturn;


			}


			if (pc->endable) {
				pc->endable--;
				//xprintf(" 'end' block \n");

				return t;
			}

			ERR(" Cannot 'end' in the global context\n");

		case KOPAQUE:


			csize = 0;
			if (tnext(t)->tok == '@' && tnext(tnext(t))->tok == NAME) {
				t = tnext(tnext(t));
				csize = getCSize(t->str);
			}
			t = tnext(t);

			typeT* ot = findType(NAMED, NULL, t->str, 0);
			if (ot)
				ot->size = csize;
			else
				mkType(OPAQUE, NULL, t->str, csize);

			t = tnext(t);
			if (t->tok != ';')
				ERR("Expected ; after opaque\n");
			t = tnext(t);
			lfold(ts, t);
			ts->handler = hnop;
			continue;

		case KALIAS:
			ts = t;
			name = tnext(ts)->str;

			t = parseType(tnext(tnext(t)));

			mkType(ALIAS, tprev(t)->ty, name, 0);

			if (t->tok != ';')
				ERR("Expected ; after opaque\n");
			t = tnext(t);
			lfold(ts, t);
			ts->handler = hnop;

			continue;


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
				typeT* vt = mkType(VIRTUAL, NULL, t->str, 0);
				vt->selectors = zvec_mk(NULL, 4);
				//zvec_disown(vt->selectors);
			}
			else
				ERR(" Expected virtual NAME, then ';'\n");

			t = tnext(tnext(t)); //skip over name and semicolon
			lfold(ts, t);
			ts->handler = hnop;
			continue;

		case KIMMEDIATE:
		case '$':
			ts = t;

			if (!immediate_parse) {
				//create parse context for immediate blocks
				immediate_parse = mkcontext("immediate");
				immediate_parse->type = tImmediate;
			}

			typeT* prev_imm_context_ty = immediate_parse->type;

			//if (prev_imm_context_ty != immediate_parse->type)
				//printf("pushing new immediate context type\n");

			immediate_parse->type = tImmediate;

			if (!pc->exec || !pc->exec->globalvars)
				immediate_parse->no_global_vars = ZTRUE;
			else
				immediate_parse->no_global_vars = ZFALSE; //enable global variables for immediate blocks that are created after program is running?


	//		if (immediate_parse->endable) {
	//			ERR("Nested immediate blocks:  This makes adding immediate variables (growing immediate space) impossible on-the-fly\n");
	//		}






			immediate_parse->endable++;
			t = parse(immediate_parse, tnext(t));
			immediate_parse->type = prev_imm_context_ty;


			t = tnext(t);

			lfold(ts, t);
			tokenT* sub = tsub(ts);
			printList(sub, NULL, 0, 10);

			printSymbols(immediate_parse->symbols, "immediate symbols");
			checkUsage(sub, NULL);//check all values are used up

			//run it

			int oldfp = 0;

			//	exectxT* oldint = immediate_parse->interrupted_parser_context;

			if (immediate_parse->exec) {
				oldfp = immediate_parse->exec->fp;
				immediate_parse->exec->sp = immediate_parse->exec->fp;
			}

			parsectxT* oldparent = immediate_parse->parent;
			immediate_parse->parent = pc;

			tokenT* savet = immediate_parse->t;	//save if there was another one already being processed
			immediate_parse->t = t;	//save our 't', because the code running may change it

			start(immediate_parse, sub, NULL);

			t = immediate_parse->t;  //get what might have been changed

			immediate_parse->t = savet;  //restore the saved t value

			immediate_parse->parent = oldparent;

			int spdone = immediate_parse->exec->sp;
			immediate_parse->exec->sp = immediate_parse->exec->fp;
			immediate_parse->exec->fp = oldfp;

			typeT* rettype = immediate_parse->exec->stack[spdone - 1].as.type;

			if (rettype != tImmediate) {
				ts->handler = hconstant;
				ts->skipargs = ZTRUE;
				ts->ty = rettype;
				ts->val = immediate_parse->exec->stack[spdone - 2];
				
				if (t->tok == SHADERDATA) {
					printf("To set shader data\n");
				
					if (t->sym->isShader == 2)
						ts->val_to_free =ZTRUE;

					t->sym->shaderdata= ts->val.as.ptr.block + ts->val.as.ptr.offset;
					//->shaderdata = ts->val.as.ptr.block;
					
					t = tnext(t);
					ram_free(tremove(tprev(t)));
				}


				//TODO: if returning a String&, why not just return is as a constant?
				if (rettype != tType && (rettype->category == POINTERUSER)) {
					//Don't return user pointers from immediate blocks.
					//Exception is tType, which is a pointer to a type
					//That's ok, because all types are 'owned' by the type system
					//and won't be freed while a program is running
					ERR("Cannot return a non-possessive pointer in immediate block\n");
				}

				if (ts->ty->category == POINTERPOSSESSIVE) {
					ts->val_to_free = ZTRUE;
					ts->handler = hconstantaddref; //need to add ref when putting on the stack
					ts->skipargs = ZTRUE;
				}

				//special case if code is returned: just insert it
				//the above setting of val_to_free will clear out the empty code token

				if (rettype->category == POINTERPOSSESSIVE && rettype->ref == tCode) {
					//if an immediate block returns code, the code is inserted directly into the token list
					//printf(" Insert code here\n");

					tokenT* t2 = ts->val.as.token;
					if (t2->restrict_parse_context && t2->restrict_parse_context != pc) {
						ERR("Inserting code that contains quoted arg subtrees from other contexts; not allowed\n");
					}
					t2 = tsub(t2);
					tokenT* t2next;
					tokenT* prev = ts;  //start inserting after ts
					while (t2) {

						t2next = tnext(t2);
						tremove(t2);
						insert_after(prev, t2); //remove and insert after the previous one
						prev = t2;
						t2 = t2next;

					}
					t = tnext(ts); //continue parsing with the first token inserted
					ram_free(tremove(ts)); //remove the immediate token
					continue;
				}



			}
			else
				ts->handler = hnop;



			continue;

		case KSELECTOR:		//proc selector
		case KPRIMITIVE:	//primitive declaration


			xprintf(" Alias is %s\n", tnext(t)->str);
			name2 = ram_addref(tnext(t)->str);

			ram_free(tremove(tnext(t)));


			if (t->tok == KSELECTOR && tnext(t)->tok == NAME && tnext(t)->str[0] == '.') {
				//printf(" data selector for %s\n", name2);

				dataselector = 1;
			}

			//fall through to var/proc decl
				//variable declaration
		case KVAR:
		case KPROC:		//proc body definition
		case KPROTO:		//proc prototype
			typeT* type = NULL;
			int isImmediate = 0;
			int isShader = 0;
			symbolT* primsym = NULL;
			int immval = 0;

			

			if (ts->tok == KPRIMITIVE) {

				s = findSymbol(primitives, name2, NULL);
				if (s) {
					handler = s->handler;
					primsym = s;
					if (tnext(t)->tok == NUMBER) {
						immval = atoi(tnext(t)->str);
						ram_free(tremove(tnext(t)));
					}

				}
				else
					ERR("No primitive named %s\n", name2);

			}

			if (tnext(t)->tok == KSHADER) {
				ram_free(tremove(tnext(t)));
				isShader = 1;

				if (tnext(t)->tok == '%') {
					ram_free(tremove(tnext(t)));
					isShader = 2;  //return value should be eventually freed when the code is freed
				}


			}

			if ((t->tok == KPROC || t->tok == KPRIMITIVE) && tnext(t)->tok == KIMMEDIATE) {  //proc flagged as immediate

				ram_free(tremove(tnext(t)));
				isImmediate = 1;

				if (tnext(t)->tok == '%') {
					if (t->tok != KPRIMITIVE) {
						ERR("immediate%% is only for primitives\n");
					}
					ram_free(tremove(tnext(t)));
					isImmediate = 2;  //return value should be eventually freed when the code is freed
				}

			}

			//if (t->tok == KPROC && tnext(t)->tok == KSHADER) {  //proc flagged as shader
				//ram_free(tremove(tnext(t)));
		//		isShader = 1;
		//	}


		//	if (pc->type == tImmediate)	//proc is defined inside an immediate context
			//	isImmediate = 1;  

			//name = tnext(t)->str;

			char* tmpstring = NULL;

			if (tnext(t)->tok == ':') {
				ram_free(tremove(tnext(t))); //delete colon


				char* tmpstring = zstrndup(":", 32);
				tmpstring = zstrcat(tmpstring, tnext(t)->str);
				ram_free(tnext(t)->str);
				tnext(t)->str = tmpstring;
			}


			if (tnext(tnext(t))->tok == '=') { //function name that ends in '='
				tnext(t)->str = zstrcat(tnext(t)->str, "=");
				ram_free(tremove(tnext(tnext(t))));

			}

			t = parseVar(tnext(t), &name, &type); //parse variable; name is required

			if (dataselector)
				name++;


			xprintf("proc/var %s   %s is type ", ts->str, name);
			printType(type, 1, 1);

			if (!name) {
				ERR("Expected name and ':'\n");
			}

			if (type->category == PENDING) {
				ERR("Cannot create 'pending' type variable... unknown size\n");
			}

			s = NULL;


			if (ts->tok == KPROC && name) {
				if (type->category != FUNCTION)
					ERR("proc defined as non-proc type\n");

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

			if (!s && !dataselector) {
				s = mkSymbol(pc, name, type, handler);
				s->primsym = primsym;
				s->immval = immval;
			}

			if (s) {
				s->isImmediate = isImmediate;
				s->isShader = isShader;
			}

			if (ts->tok == KPROTO) {
				s->isPrototype = 1;
				s->handler = hcall;	//will eventually be a function call
			}

			if (ts->tok == KSELECTOR && !dataselector) {
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
						if (pt->ref->category != VIRTUAL)
							ERR("Selectors can only operate on virtual types\n");

						s->selectorNum = zvec_count(pt->ref->selectors); //track which selector this is
						zvec_add(pt->ref->selectors, ram_addref(s));
						s->selectorArg = i;
						printType(pt->ref, ZTRUE, ZTRUE);

						break;
					}
				}
				if (i == zvec_count(s->type->members))
					ERR("No such arg %s\n", name2);
			}
			if (dataselector) {
				//char* stmp = zstrdup2(name2, name);
				//s = mkSymbol(pc, stmp, type, handler);
				s = mkSymbol(NULL, name, type, handler);
				//add to the selector table
				if (type->category == VIRTUAL)
					s->isSelector = 4;
				else
					s->isSelector = 2;
				//	s->handler = hdataselect;

				typeT* vt = findType(VIRTUAL, NULL, name2, 0);

				s->selectorNum = zvec_count(vt->selectors); //track which selector this is
				zvec_add(vt->selectors, s);
			}

			if ((ts->tok == KPROC || ts->tok == KVAR) && name)
				s->alias = name2;
			else
				ram_free(name2);

			name2 = NULL;

			if (ts->tok == KPROC) {
				//procedures go into a body of statements
				printf(" parse proc %s ", name);
				printTypeNoRedirect(type, ZTRUE, ZFALSE);
				s->subctx = mkcontext(name);
				s->subctx->symfrom = s;
				s->subctx->parent = pc;

				s->subctx->endable++; //its a subcontext

				if (!isShader)
					s->subctx->type = s->type; //expecting to parse a 'return' real return value if not a shader context


				//	t->sym = s;
				s->handler = hcall;  //need to set handler before parsing, in case of recursion
				t = parse(s->subctx, t);

				checkUsage(ts, t);//check all values are used up

				printf(" parsed proc %s ", name);

				if (!strcmp(name, "destructor")) {
					if (s->type && s->type->members && zvec_count(s->type->members) == 1) {
						typeT* dt = zvec_get_at(s->type->members, 0);
						//dt is the 0th arg of the function. ->ref is the type, which would be a pointer to something.  then ->ref again for the thing
						if (dt && dt->ref && dt->ref->ref && dt->ref->ref->category == STRUCT) {
							dt->ref->ref->destructorproc = s;
						}
					}
				}

				//t should now be 'end' 
			}
			else if (t->tok != ';') {
				ERR(" missing ;\n");
			}

			if (t->tok == KEND) {

			}

			t = tnext(t); //skip past semicolon (or 'end')

			lfold(ts, t);  //everything up to an including semicolon folded
			//ts->tok = 0;  //to break tokennext chains from escaping the proc
			if (!s->isPrototype)
				s->tokens = ram_addref(ts); //symbol has this tokenstream

			if (isShader) {
				//the proc declaration returns the Symbol
				ts->handler = hDEBUG;
				ts->skipargs = ZTRUE;
				ts->val.as.ptr.block = s;
				ts->val.as.ptr.offset = 0;
				ts->ty = tSymbol;
				

				//put in a SHADERDATA token so the immediate block the runs after can know to set it
				tokenT* sd = mkToken(SHADERDATA, "SetShaderData", 0);
				sd->sym = s;
				insert_after(t, sd);
				//t = tprev(t);

			}
			else {
				ts->handler = hnop;	//the proc declaration doesn't do anything 
			}

			continue;

			/*case '?':
				if (tprev(t)->ty) {
					if (tprev(t)->ty->ref && tprev(t)->ty->ref->category == VIRTUAL) {
						printf(" Virtual type %s \n", tprev(t)->ty->ref->name);
						//need the real type

					}
					else {
						printf("Known type of size %d\n", tprev(t)->ty->size);

					}


				}

				t = tnext(t);
				continue;
				*/

		case '#':	//create variable of whaatever type is on the stack, and store 
		case PAIR('#', '#'):

			t = tnext(t); //is variable name

		//variables don't need a handler set
		//if (pc == global)
		//	handler = hglobal;
		//else
			//handler = hlocal;
			s = findSymbol(pc->symbols, t->str, NULL);
			if (s != NULL)
 				ERR(" Redefining %s\n", t->str);
			s = mkSymbol(pc, t->str, tprev(ts)->ty, NULL);

			t->sym = s; //preresolve this symbol

			if (ts->tok == PAIR('#', '#'))
				zlist_insert_node_after(t, mkToken('#=', "#=", 0));
			else {
				tokenT* tt = mkToken('=', "=", 0);
				tt->generated = 1;
				zlist_insert_node_after(t, tt);
			}
			if (t->sym) {
				if (((t->sym->type->category == POINTERUSER) || (t->sym->type->category == POINTERPOSSESSIVE))
					&& t->sym->type->ref->category == FUNCTION) {
					zlist_insert_node_after(t, mkToken('&', "autonoexec", 0));
				}
			}
			fold(ts, t);
			//t = tnext(t);
			continue;
		case KRETURN:

			//todo: check return type
			//allow return no value

			t->handler = hreturn;

			if (pc->type == tImmediate) {

				//printf(" compiling return for immediate\n");
				if (tprev(t)->ty) {
					//	pc->type = tprev(t)->ty;
					t->ty = tprev(t)->ty;
					fold(tprev(t), t);
					t = tnext(t);
				}
				else {
					t->ty = tImmediate; //must return tImmediate type if returning no value (so there is something to pop off the stack)
					t = tnext(t);
				}


				continue;

			}

			if (pc->type && pc->type->ref) {
				xprintf(" RETURN a value\n");
				printType(pc->type->ref, ZTRUE, ZFALSE);

				if (pc->type->ref->islike)
					printf("return like\n");
				typeT* lt = resolve_like_type(pc->type->ref, pc->type->members, NULL, ZFALSE);

				if (tprev(t)->ty != lt) {
					xprintf("Type mismatch expected:\n");
					//printType(pc->type->ref, ZTRUE, ZTRUE);
					printType(lt, ZTRUE, ZTRUE);
					xprintf("attempt to return: \n");
					printType(tprev(t)->ty, ZTRUE, ZTRUE);
					ERR("TYPE MISMATCH %s\n", pc->name);
				}

				fold(tprev(t), t);
			}
			else {
				xprintf(" RETURN no value\n");
			}

			t = tnext(t);
			continue;

		case KBREAK:
			t->handler = hbreakcontinueloop;
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
			ts->skipargs = ZTRUE;

			continue;

		case KELSEIF:

			typeT* ct = tprev(t)->ty;

			if (!ct)
				ERR(" If: no type input\n");

			if ((ct->category != POINTERUSER) && (ct != tBit) && (ct != tZ32)) {
				tokenize(ct, ":Bit", "nofile");
				t = ct;
				continue;
				//ERR(" if needs Bit or Pointer datatype\n");

			}

			fold(tprev(t), t);//take previous node as the condition
			t->handler = hcondblock;
			t->skipargs = ZTRUE;
			t->val.as.n32 = 1;
			return t;

		case KELSE:  //return in the middle
			return t;

		case KIF:

			ct = tprev(t)->ty;

			if (!ct)
				ERR(" If: no type input\n");

			if ((ct->category != POINTERUSER) && (ct != tBit) && (ct != tZ32)) {
				tokenize(tprev(t), ":Bit", "nofile");
				t = tprev(t);
				continue;
				//ERR(" if needs Bit or Pointer datatype\n");

			}

			tokenT* condition = tremove(tprev(t));
			ts = t;

			t = tnext(t);
			pc->endable++;

			t = parse(pc, t); //parse until end, else or elseif

			insert_after(ts, condition); //insert the condition to be the first child
			lfold(ts, t); //move all the 'true' case code into the cond block
			ts->handler = hcondblock;
			ts->skipargs = ZTRUE;

			//SIMPLE CASE:
			//  hcondblock (IF) {		
			//		condition
			//		true statements
			//  }

			
			if (t->tok == KEND) { //simple case, just a condblock
				t->handler = hnop;
				t = tnext(t);
				continue;
			}
			
			//COMPLEX CASE:   
			// in this case, the whole if/elseif/else tree is put in a hgroup
			//  hcondblock will 'break' the group after it runs
			//  hgroup{
			//	
			//		hcondblock:{			IF
			//			condition
			//			true statements
			//		}
			//		hcondblock:{			ELSEIF	(0 or more)
			//			condition
			//			true statements
			//		}
			//								ELSE	(optional)
			//		else statements			
			// 
			//	}							END



			if (t->tok == KELSE || t->tok == KELSEIF) {
				ts->val.as.n32 = 1; //COND will break the group after running true clause
			}
			else
				ERR(" unexpected %s\n", t->str);

			
			

			while (t->tok == KELSEIF) {
				tokenT* elsif = t;  
				t = tnext(t);
				t = parse(pc, t); //t is going to be elseif, else, or end
				lfold(elsif, t); //fold the condition's iftrue code into the elseif tag
			}

			//else case
			if (t->tok == KELSE) {
				t->handler = hnop; //
				t = tnext(t);
				t = parse(pc, t); //continue until 'end'
				
			}

			if (t->tok == KEND) {
				//t->handler = hnop;
				fold(ts, t);
				t->handler = hgroup;
				t = tnext(t);
				continue;
			}
			ERR("expected END\n");
		



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

		case PASSTHRU:
			t = tnext(t);
			continue;
		case NUMBER:

			if (strchr(t->str, '.')) {  //decimal point makes it a float
#ifdef FLOAT
				FLOAT n = (FLOAT)atof(t->str);
				t->val.as.f = n;
				t->ty = tReal;
				t->handler = hconstant;
				t->skipargs = ZTRUE;
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
			t->skipargs = ZTRUE;
			
			
			//xprintf(" set handler for %s to %p\n", t->str, t->handler);
			t = tnext(t);
			continue;
		case LITERAL: //string literal (byte array)
			t->ty = findType(POINTERUSER, tString, NULL, 0); //findType(ARRAYDYNAMIC, tN8, NULL, 0);
			t->handler = hconstant;
			t->skipargs = ZTRUE;
			t->val.as.ptr.block = zstrndup(t->str + 1, strlen(t->str) - 2); //t->str already a zstring
			//unescape slashes
			char* rpos = t->val.as.ptr.block;
			char* wpos = rpos;
			do {
				if (*rpos == '\\')
					rpos++;
				*wpos = *rpos;
				rpos++; wpos++;
			} while (*rpos);
			*wpos = 0;

			t->val.as.ptr.offset = 0;
			t->val_to_free = ZTRUE;
			t = tnext(t);
			continue;

		case KCONSTANT:
			ts = t;
			
			if (tprev(ts)->handler != hconstant)
				ERR(" previous item must be a constant!\n");

			
			t = tnext(t); //name of constant
			
			s = mkSymbol(pc, t->str, tprev(ts)->ty, NULL); //copy the type
			s->tokens = ram_addref(tprev(ts)); //point to the constant handler
			s->isConstant = ZTRUE;
			fold(tprev(ts), t);
			t->handler = hnop;
			t = tnext(t);
			
			continue;

		case KINCLUDE:
			xprintf("lit? %x %x\n", tprev(t)->tok, LITERAL);
			if (tprev(t)->tok == LITERAL) {
				char* fname = tprev(t)->val.as.ptr.block;
				char* strfile = ram_loadstr(fname);
				if (strfile) {

					tokenize(t, strfile, fname);
					ram_free(strfile);
					t = tnext(t);
					ram_free(tremove(tprev(ts)));  //remove 'include'
					ram_free(tremove(ts)); //remove string literal
					continue;
				}
				else {
					ERR("cannot open file %s\n", tprev(t)->val.as.ptr.block);
				}
			}
			break;

	
		case KSTACKED:

			t = tnext(t); //t at name now
			ty = findType(NAMED, NULL, t->str, 0);
			if (!ty) {
				ty = mkType(PENDING, NULL, t->str, 0);
				ty->stacked = ZTRUE;
				//ERR("No type named %s\n", t->str);
			}
			ty->stacked = ZTRUE;
			t = tnext(t);
			
			if (t->tok != ';')
				ERR("expected stacked <typename>;");

			
			fold(ts, t);
			t->handler = hnop;
			t = tnext(t);
			continue;
			
		case ':': //typecast

			//if next token is text, we should attempt a function call instead
			//this lets a function name start with :
			//meaning :Thing  could be a function like:    proc :Thing(a:Z32->Thing)
			//so this lets you create custom functions that act like a typecast
			//could also beused for constructors:   1.0 2.0 3.0:Vec3  
			// proc :Vec3(x:Real;y:Real;z:Real -> Vec3);
			if (tnext(t)->tok == NAME) {
				char* cname = zstrcat(zstrdup(":"), tnext(t)->str);
				tokenT* nt = mkToken(NAME, cname, 0);
				insert_after(tprev(t), nt);
				t->tok = TYPECAST;
				t = tprev(t);
				ram_free(cname);
				
				continue;
			}

			printf(" next isn't name\n");

			
		case TYPECAST: //typecast

			ts = t; //ts is colon

			
			if (   (tnext(t)->tok == PASSTHRU) 
				&& (tnext(t)->handler == hconstant)
				&& (tnext(t)->ty == tType) ) {

				ty = tnext(t)->val.as.type;
				t = tnext(tnext(t));
			}
			else {
				t = parseType(tnext(t));

				//special case cast to virtual pointer type
				ty = tnext(ts)->ty;

			}

			printList(tprev(ts), t, -5, 2);
			typeT* from = tprev(ts)->ty;

			if (
				((from->category == POINTERUSER) || (from->category == POINTERPOSSESSIVE))
				&& (from->ref->category == FUNCTION)
				&& (!strcmp(ty->name , "ExecToken"))
				) 
			{
				ts->handler = hsymtoken;
				ts->ty = tExecToken;
				fold(tprev(ts), ts);
				ram_free(tremove(tnext(ts)));
				continue;
			}



			if (ty
				&& ((ty->category == POINTERUSER) || (ty->category == POINTERPOSSESSIVE))
				&& ty->ref && (ty->ref->category == VIRTUAL)) {

				//check that this type supports this virtual type
				typeT* from = tprev(ts)->ty;

				//virtual to virtual 
				if ((from->category == ty->category) && from->ref) {
					int j;
					if (from->ref->category == VIRTUAL) {
						//from virtual to virtual
						for (j = 0; j < zvec_count(from->ref->selectors); j++) {
							symbolT* sel = zvec_get_at(from->ref->selectors, j);
							if (sel->type == ty->ref) {
								//printf(" Found selector %d\n", sel->selectorNum);
								

								ram_free(tremove(tnext(ts)));
								ts->handler = hchselector;	//changeselector
								ts->val.as.n32 = sel->selectorNum;
								ts->ty = ty;
								fold(tprev(ts), ts);
								printList(tprev(ts), ts, -5, 3);
								break;

							}
						}
						if (j == zvec_count(from->ref->selectors)) {
							ERR("Struct does not contain selector member \n"
							);
						}
						continue;
					}

					//real to virtual
					for (j = 0; j < zvec_count(from->ref->members); j++) {
						typeT* t2 = zvec_get_at(from->ref->members, j);  //look at the type's members (t2 is the memer; t2->ref is the type of the member)
						if (ty->ref && t2->ref && (ty->ref->tid == t2->ref->tid)) { //check the type the member refers to to the virtual type we are casting to
							printf(" Type %s supports virtual %s\n", from->ref->name, t2->name);
							//need to make sure all of ty->ref's selectors are 1)implemented on t2->ref AND are in t2's selector list
							

							check_implementation(ty->ref, from->ref, t2);

							ram_free(tremove(tnext(ts)));
							ts->handler = haddselector;	//push the set of selectors
							ts->val.as.type = t2;				//TODO: put the selectors on a seperate stack
							ts->tyval = tType;
							ts->ty = ty;
							fold(tprev(ts), ts);
							printList(tprev(ts), ts, -5, 3);
							break;

						}
					}
					if (j == zvec_count(from->ref->members)) {
						ERR("Struct does not contain selector member for %s", ty->name );
					}
					continue;

				}
				else
					ERR(" only pointers (of the same pointers can be casted to virtual, & to & and %% to %%\n");


			}

			//virtual to real:
				//virtual to real
			//
			

			if (  (ty->ref)
				  && (tprev(ts)->ty && tprev(ts)->ty->ref && tprev(ts)->ty->ref->category == VIRTUAL)  
				  && (ty->ref != tany)
				) {
				ram_free(tremove(tnext(ts)));
				ts->handler = htesttype;	//check if its the right type
				ts->val.as.type = ty;
				ts->tyval = tType;
				ts->ty = ty;
				fold(tprev(ts), ts);

				continue;
			}

		

			//forced typecast
			printf(" Forced typecast ");
			printTypeNoRedirect(tprev(ts)->ty, ZFALSE, ZFALSE);
			printf(" to ");
			printTypeNoRedirect(ty, ZTRUE, ZFALSE);

			if (tprev(ts)->ty->category != ty->category)
				xprintf(" Typecast changing from category %d to %d\n", tprev(ts)->ty->category, tprev(t)->ty->category);

			tprev(ts)->tyorig = tprev(ts)->ty;
			//tprev(ts)->ty = tprev(t)->ty;
			tprev(ts)->ty = ty;

			ram_free(tremove(tnext(ts)));
			ram_free(tremove(ts));

			printList(tprev(tprev(t)), t, -5, 3);
			continue;
		
		case KTYPEOF:
			t->handler = hconstant;
			t->skipargs = ZTRUE;
			t->val.as.ptr.block = tprev(t)->ty;
			t->val.as.ptr.offset = 0;
			t->ty = tType;
			
			fold(tprev(t), t);
			t->useslocal = 0; //no local
			t = tnext(t);
			continue;

		//resize replaced as a primitive

		case PAIR('[', ']'):
			//array indexing


			if ((tprev(t)->ty != tZ32) && (tprev(t)->ty != tN32)) {
				break; //allow [] to be used in other words
				//ERR("Array index must be Z32 or N32\n");
			}

			

			//todo: check its an integer, and type is an array
			//array accesshload32

			ts = tprev(tprev(t));
			
			//if not a pointer to an array type or , skip processing
			//the below t->str case might find a matching word
			if (!ts->ty->ref || 
				((ts->ty->ref->category != ARRAYDYNAMIC) && (ts->ty->ref->category != ARRAYSTATIC) && (ts->ty->ref != tString))
				)
				break;
			
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

			if ((t->ty->ref->category != STRUCT)  || (t->ty->ref->stacked)) {
				//if t->ty->ref is a STRUCT< then t->ty is a pointer to a struct... we have an array of structs.  Can't load a struct, so don't load it. [] on an array of structs returns a pointer to the nth element
				//If it was an array of pointers to structs, then ty->ref->category is a pointer to pointer to a struct, 

				insert_after(t, mkToken('@', "@", 1)); //read the item ( The next parsed token, might remove this, if it wants to store or manipulate the pointer)
				tnext(t)->generated = 1;
			}

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

			
			
			switch (tnext(t)->tok) {


				//if next token is assignment, remove the '@' token
			case '=':
				
			case PAIR('#', '='):
				t = tnext(t);
				ram_free(tremove(ts));
				continue;

			}
			
			


			//next token is struct member
			if (tnext(t)->str && tnext(t)->str[0] == '.'
				&& tprev(t)->ty
				&& tprev(t)->ty->ref
				&& tprev(t)->ty->ref->category == STRUCT 
				&& (tprev(t)->tok != STACKARG || tprev(t)->ty->ref->stacked) ) {
				
				if (tprev(t)->ty->ref->stacked && tprev(t)->tok == STACKARG)
					tprev(t)->handler = hstackptr; //get the stack pointer

				//access struct member:  
				//remove the @ token, since accessing the struct member is just pointer addition
				t = tnext(t);
				ram_free(tremove(ts));
				continue;
			}

			

			//read function arguments from the stack (Note: function args are READONLY...)
			if (tprev(t)->tok == STACKARG) {

				t->ty = tprev(t)->ty->ref;
				tprev(t)->ty = NULL;

				//t->val = tprev(t)->val; //take the stack position value

				fold(tprev(t), t);
				
				t->handler = hstackread;
				t->tyval = tZ32;
				t->skipargs = ZTRUE;

				
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



			//user pointer to stacked value
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->stacked)) {

				t->handler = hloadbytes;
				t->val.as.n32 = tprev(t)->ty->ref->size;
				t->ty=tprev(t)->ty->ref;
				if (t->val.as.n32 > sizeof(valueT))
					ERR(" Type %s does not fit in a stack slot\n", tprev(t)->ty->ref->name);
				fold(tprev(t), t);
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
			
				//t->ty = tprev(t)->ty->ref;  //instead of putting cpointer on stack, promote to userpointer
				t->ty = findType(POINTERUSER, tprev(t)->ty->ref->ref, NULL, 0);

			
				fold(tprev(t), t);
				t->handler = hloadcptr;
				t = tnext(t);
				continue;
			}


			//other '@' cases that aren't handled where are done via primitive handlers
			break;

		case PAIR('#','='): //store without pop
			nopop = 1;

		case '=': //store
			xprintf("checking = %d\n", debugtimes++);
			


			//store stacked value
			//user pointer to stacked value
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->stacked)
				&& tprev(tprev(t))->ty->stacked
				&& tprev(t)->ty->ref == tprev(tprev(t))->ty  //and same type
				) {

				t->handler = hstorebytes;
				t->val.as.n32 = tprev(tprev(t))->ty->size;

				if (t->val.as.n32 > sizeof(valueT))
					ERR(" Type %s does not fit in a stack slot\n", tprev(t)->ty->ref->name);

				fold(ts, t);
				t = tnext(t);
				continue;
			}

			//try to handle storing ptr to ptr.  Top of stack has a pointer to the pointer var
			
			if (!tprev(t)->ty || ! tprev(t)->ty->ref || ! tprev(t)->ty->ref->ref)
				break;
			
			if (!tprev(tprev(t))->ty || !tprev(tprev(t))->ty->ref)
				break;

			if (tprev(tprev(t))->ty->ref != tprev(t)->ty->ref->ref)
				break;

		//	xprintf(" COMPARE ");
			//printType(tprev(t)->ty->ref->ref, ZFALSE, ZTRUE);
		//	xprintf(" TO ");
		//	printType(tprev(tprev(t))->ty->ref, ZTRUE, ZTRUE);


		//	if (tprev(t)->ty->ref->ref != tprev(tprev(t))->ty->ref) {
			//	printf("type mismatch \n");
				//break;
		//	}


			//handle     @= case.... if '@' a pointer to get a variable, and store to the variable...
			//  pointervar =         //writes a pointer to a pointer variable
			// The pointer variable is represented by a pointer to some kind of pointer
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == POINTERUSER)) {
				if (tprev(tprev(t))->ty && tprev(tprev(t))->ty->category == POINTERUSER) {
					//xprintf("general pointer to pointer store\n");

					if (tprev(tprev(t))->ty->ref->category == VIRTUAL)
						t->val.as.n32 = 8; //save virtual part too

					
					if (nopop) {	
						t->handler = hstoreptrnp;
					} else
						t->handler = hstoreptr;
					
					//TODO check level
					
					if (nopop)
						t->ty = tprev(tprev(t))->ty;

					fold(tprev(tprev(t)), t);
					t = tnext(t);
					continue;
				}
			}

			//stpre a possessive pointer in a possessive pointer variable (which is represented by a user pointer to a possessive pointer)
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == POINTERPOSSESSIVE)) {
				if (tprev(tprev(t))->ty && tprev(tprev(t))->ty->category == POINTERPOSSESSIVE) {

					if (tprev(tprev(t))->ty->ref->category == VIRTUAL)
						t->val.as.n32 = 8; //save virtual part too


					
					if (nopop)
						t->handler = hstoreptrnp;
					else
						t->handler = hstoreptr;


					t->val.as.n32 |= 1; //flag to free the pointer being overwritten.

					if (nopop) {

						//if next is keep, keep it a possessive pointer

  						if (tnext(t)->tok == KKEEP) {
							ram_free(tremove(tnext(t)));
							t->val.as.n32 |= 16; //addref
							t->ty = tprev(tprev(t))->ty;
						}
						else
						{
							//don't addref... demote to regular user pointer
							t->ty = findType(POINTERUSER, tprev(tprev(t))->ty->ref, NULL, 0);

							t->trackpossptr = t; //have the nonpossessive pointer and all their derivatives track this

						}
												
					}
					fold(tprev(tprev(t)), t);

 					t = tnext(t);
					continue;
				}
			}




			//storing a cpointer or user pointer into a pointer
			if (tprev(t)->ty && (tprev(t)->ty->category == POINTERUSER) && (tprev(t)->ty->ref->category == CPOINTER)) { //pointer to Cpointer
				if ((tprev(tprev(t))->ty->category == CPOINTER)|| (tprev(tprev(t))->ty->category == POINTERUSER)) {  //cpointer
					fold(tprev(tprev(t)), t);
					if (nopop)
						ERR("nopop not supported on cpointer\n");
					else
						t->handler = hstorecptr;
					t = tnext(t);

					if (nopop)
						t->ty = tprev(tprev(t))->ty;

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
		
			int isint = (tprev(t)->ty == tZ32) || (tprev(t)->ty == tN32);
			if    (isint && (tprev(tprev(t))->ty == tType))		{//Type Z32{
							
				fold(  tprev(t), t); //put array size as sub
					
				typeT* rt = (void*) tprev(t)->val.as.ptr.block;  //value of item  (todo fix to be like below non-array case)
				if (tprev(t)->handler != hconstant) {
					ERR("alloction requires a constant type\n");

				}
			
				if(rt->category != ARRAYDYNAMIC){
					xprintf(" allocating array for type ");
					printType(rt, 0, 0);
					//ERR("Only dynamic arrays can be allocated by  '[type] count new' \n");
					break;  //see if another work down below can handle it
				}
				
				ram_free(tremove(tprev(t))); //remove array type token
				
				t->ty = findType(POINTERPOSSESSIVE, rt, NULL, 0);//get possessive pointer to
											
				t->handler =   hallocarray;
				if (ts->tok == KNEW)
					t->val.as.z32 = 1; //array starts full
				else
					t->val.as.z32 = 0; //array starts empty
				
				t = tnext(t);
		
				continue;
			}
				
			if (tprev(t)->ty == tType){	//suchas as MyWhateverStrucutre new

				if (tprev(t)->handler != hconstant) {
					ERR("alloction requires a constant type\n");
				}

				
				
				typeT* rt = (void*) tprev(t)->val.as.type;
				if (tprev(t)->handler == hredirectsub) {
					tokenT* redirected = tprev(t)->val.as.token;
					rt = tsub(redirected)->val.as.type;
				}
				
				
				
				if ((rt->category == ARRAYSTATIC)||(rt->category == ARRAYDYNAMIC)){
					printType(rt, ZTRUE, ZTRUE);
					ERR(" Cannot allocate array with '[type] new' ; must use dynamic syntax: [type] count new\n");
				}
				if (rt->category == PENDING)
					ERR("Cannot allocate pending type\n");
				
				t->ty = findType(POINTERPOSSESSIVE, rt, NULL, 0);
				xprintf(" new will return \n");
				printType( rt, 1, 1);
				
				ram_free(tremove(tprev(t))); //remove type token
			//	fold(tprev(t), t);

				t->handler =   halloc;
				t = tnext(t);
				continue;
			}
			
			ERR(" WHAT %x %s?\n", t->tok, t->str);
			
			break;						
			
		}//end switch
		
		
		//if didn't match anything above, continue on
				
		//check local variables
		if (pc->type && pc->type->members){	 //set to function type if inside function
			int count =0;
			int pos=0;
			
			xprintf(" LOOKING IN ");
			printType(pc->type, ZTRUE, ZFALSE);
			typeT* m = findTypeMember( pc->type, t->str, &pos, &count);
			
			if (m){
				int so = -count+pos;
				xprintf("Found %s  stack pos fp+%d, of type   ", m->name, so);
				printType(m->ref,ZTRUE, ZFALSE);
					
				if (m->ref->islike)
					printf("like arg\n");
		
				typeT* lt = resolve_like_type(m->ref, pc->type->members, 0, ZFALSE);

				
				if (m->isPer) {

					printf("'per type':  better be a pointer to array\n");

					lt = lt->ref->ref;
					
					t->ty = lt;
					t->tok = PERARG;
					t->useslocal = 1;

					t->val.as.z32 = so;
					t = tnext(t);
					continue;
				}


				t->tok = STACKARG;


				if (m->deref) {
					//if the var is a deref arg, then load it off the stack


					tokenT* tn = mkToken('@', "@", 1);  //read from the stackarg.  This is loading either null or a pointer, which can either loaded by the next '@', or the load can be suppreseed with a & for testing
					tn->generated = 1;
					insert_after(t, tn);
				}
				
				
				t->ty = findType(POINTERUSER, lt, NULL, 0);
				
				t->useslocal = 1;
			


				tokenT* tn = mkToken('@', "@", 1);  //load the variable
				tn->generated = 1;
				insert_after(t, tn);
				
				
				t->val.as.z32=so;
				t=tnext(t);
				continue;
			}
		}
			
		if (t->str){

			if (ram_numrefs(t) == 0)
				printf("freed token!\n");


			if (ram_numrefs(t->str) == 0)
				printf("freed string!\n");


			int j;
			v = zvec_disown(zvec_mk(NULL,15));
			int fpointer=0;//found symbol is fpointer
			int noexec = 0;//do not execute found item
			
			if (!strcmp(t->str, "[]") && tnext(t)->tok == '=') {
				//try []= as a function name
				t->str = zstrcat(t->str, "="); //append '='
				ram_free(tremove(tnext(t))); //kill '='
			}
		

			if ( tnext(t)->tok =='&') 
			{
				noexec = 1;
			}

			tokenT* pos;
			tokenT* startfold=NULL;
			int emptystack = 0;

			for(j=0;!emptystack;j++){  //keep going until we either find a symbol, or we tried w/ an 'emptystack' as the first symbol

				int k;
				pos = t;
				//xprintf(" DEPTH %d: \n", j);
				zvec_setcount(v, 0);
				for (k=0;k< j ;k++){  //go back j spaces.
					if (pos)
						pos = tprev(pos);
					else {
						zvec_add(v, tEmptyStack);
						emptystack = 1;
					}
				}

				if (!pos)
						break;
				startfold=pos;
				
				for(k=emptystack;k<j;k++){ //if emptystack ==1, skip the 1st arg (since its 'fake')
			
					typeT* tt = pos->ty;

					if ( !pos->ty ){ 
						emptystack = 1;
						tt = tEmptyStack;
						startfold = tnext(startfold);
					}

					if (noexec && tt == tType) {
						//if no exec, instead of a placeholder expression of the arg type, allow using a Type& of that type
						//instead of  1 1 +&   it is ok to say Z32 Z32 +&
						tt = pos->val.as.type;
					}

				//	if ((ignoresigned == t) && (tt == tZ32))
					//	tt = tN32;
											
					zvec_add(v, tt);
					
					pos = tnext(pos);
				}//end k
				//xprintf("\n");
				if (!pos)
					break;
				s=NULL;
			
				if (t->sym) { 
					s = t->sym;
					//printf(" preresolved symbol\n");
					if (pc == immediate_parse)
						local = -1;
					else if (pc == global)
						local = 0;
					else
						local = 1;
					break;
				} else{

					if ((pc != global) && (pc != immediate_parse)) {

						local = 1;
						s = findSymbolEx(pc->symbols, t->str, v, matchApprox);
						//xprintf(" LOCAL SYMBOL %p  %s\n", s, t->str);

						if (s)
							break;

						//took to parent's symbols (siblings) for functions that could be called
						if (pc->parent && pc != immediate_parse && pc->parent != global && pc->parent != immediate_parse) {
							s = findSymbolEx(pc->parent->symbols, t->str, v, matchApprox);

							if (s && s->type->category != FUNCTION) {
								ERR("Can't access parent variable... pass it as a parameter instead\n");

							}

							if (s)
								break;
						}

					}

					//try global immediate parse
					if (immediate_parse) {
						s = findSymbolEx(immediate_parse->symbols, t->str, v, matchApprox);
						if (s) {
							local = -1;
							break;
						}
					}

					//global
					local = 0;
					s = findSymbolEx(global->symbols, t->str, v, matchApprox);
				}
			
				if (s)
 					break;				
			}//end j
			
			if (s){ //found symbol
				ram_free(v);
			
				if (matchApprox != 0) {
					printf(" Found symbol %s  approximation:%d\n", t->str, matchApprox);
				}

				//xprintf(" Found symbol %s  local:%d \n", t->str, local);
				//printType(s->type,0,0);
				//xprintf("\n");

				if (tnext(ts)->tok == TYPECAST) {
					ram_free(tremove(tnext(ts))); //remove typecast token
					ram_free(tremove(tnext(ts))); //remove typename its being cast to
				}
			
			


				if ((s->type->category == POINTERUSER || s->type->category == POINTERPOSSESSIVE) && s->type->ref && s->type->ref->category == FUNCTION)
					fpointer = 1;
							
				if (s->isConstant) {
					//printf("constant\n");
					t->val = s->tokens->val;
					t->handler = s->tokens->handler;
					t->ty = s->tokens->ty;
					t->tyval = s->tokens->ty;
					t = tnext(t);
					v = NULL;
					continue;
				}

			

				if ((s->type->category == FUNCTION || fpointer  )) {
					//even when noexecing, we still need to fold in the parameters, since the type of the parameters 
					//were used to find the function
					fold(startfold, t);

					if (noexec) {
						if (tnext(t)->tok == '&') {
							ram_free(tremove(tnext(t))); //remove it
						}
						else {
							ERR("expected noexec (&)\n");
						}

					}
				}

				//if found a function or function pointer, and noexec (&) hasn't been put in
				
				if (!noexec && (s->type->category == FUNCTION || fpointer)) {

					//check parameters to function being called
					//if any parameters are tracked from a possessive pointer, flag for addref

					tokenT* tv = startfold;
					startfold = NULL;//
					int argnum = 0;
			//	if (emptystack) {
				//		argnum = 1;
					//}
					while (tv) {
						
//decided to use 'shader' as a new class of proc

						int isredir = 0;

						if (s->type->members) {
							typeT* argtype = zvec_get_at(s->type->members, argnum);
							if (argtype && argtype->ref && argtype->ref->category == SUBTREE) {
								//printf(" Wrap %dth arg with redirect\n", argnum);
								tokenT* redirect = mkToken(REDIRECT, "redirect", 0);
								redirect->handler = hconstant;
								redirect->skipargs = ZTRUE;
								
								//instead take the type from the actual arg token
								//because argtype->ref might be 'any' or other 'fake' type

								//redirect->ty = argtype->ref;
								redirect->ty = findType(SUBTREE, tv->ty, NULL, 0);
								isredir = 1; 
								
								/*
								printf("SUBTREE ");
								printTypeNoRedirect(tv->ty, ZFALSE, ZTRUE);
								printf(" ==?== ");
								printTypeNoRedirect(argtype->ref, ZFALSE, ZTRUE);
								*/

								redirect->val.as.token = redirect;  //push self on stack
								//redirect->tyval = tType;// WRONG?
								redirect->restrict_parse_context = pc;
								insert_after(tv, redirect);
								tv = tnext(tv);	//tv is at 'follow'
								fold(tprev(tv), tv); //
								if (redirect->useslocal)
									t->useslocal-= redirect->useslocal; //if code being passed uses locals, thats fine, subtract them out.  Because the code being passed can only run in the same context it came from. (it will have restricted context value set)
								//redirect->useslocal = ZFALSE; //any local variables used in the redirected code aren't actually used in the call to the immediate proc
								printList(redirect, redirect, 0, 1);

							}
						}


						if (tv->trackpossptr && !isredir) {
							//xprintf(" Arg derived from possessive pointer <%s> ", tv->str  );
							//if an arg being passed to a function is a possive pointer (or result of adding/indexing from a possessive pointer), then it needs to be addref'd before being passed to a function
							if (s->handler == hcall || fpointer) {

								if (tv->trackpossptr->handler == hloadptr) {
									tv->trackpossptr->val.as.n32 |= 2;

								}
								else if (tv->trackpossptr->handler == hstoreptrnp) {

									tv->trackpossptr->val.as.n32 |= 16;
								}
								else {
									printf(" trackpossptr unknown handler\n");
								}
							}


							//getc(stdin);
							//	xprintf(" addref %p +%d\n", ex->stack[ex->fp-count+i].as.ptr.block, ex->stack[ex->fp-count+i].as.ptr.offset);
								//ram_addref( ex->stack[ex->fp-count+i].as.ptr.block );
						}


						tv = tnext(tv);
						argnum++;

					}

					if (!fpointer) { //regular function call

						t->ty = resolve_like_type(s->type->ref, s->type->members, v, ZTRUE);
						t->sym = s;
						
						if (s->handler ) {
							t->handler = s->handler;
							if (s->primsym)

								if (t->val.as.z32 || t->val.as.ptr.block)
									printf("this should be zero\n");
								if (s->immval)
									t->val.as.z32 = s->immval;
						}
						
						if (s->isImmediate) {
							//add kimmediate token
							tokenT* timm = mkToken(KIMMEDIATE, "X_immediate", 0);					
							tokenT* ret = mkToken(KRETURN, "X_return", 0);
							tokenT* en = mkToken(KEND, "X_end", 0);

							if (s->isImmediate == 2)
								timm->val_to_free = ZTRUE;

							//insert_after(tprev(ts), timm);
							zlist_insert_node_before(ts, timm);

							insert_after(t, ret);
							insert_after(ret, en);

							t->tok = PASSTHRU; //already parsed the function call

							if (t->useslocal)
								ERR("Subexpression uses local variables that don't exist at compile time:  Cannot move proc call into immediate context.\n");

							t = tprev(ts); //backtrack to the immediate block just created
							v = NULL;
							continue;

						}

						t = tnext(t);
						v = NULL;
						continue;

					}

				}
				v = NULL;
				//by this point, normal function calls have been taken care of.
				//what is left is a noexec 'normal' function, variable fetches, and noexec function pointers
			
				if (s->type->category == FUNCTION) {

					if (noexec) {
						//printf(" noexec regular function\n");
						t->handler = hconstant;
						t-> skipargs = ZTRUE;
						t->val.as.symbol = s;

					//	t->val.as.n32 = 0xffff;
						//t->sym = 
					
						t->ty = findType(POINTERUSER, s->type, NULL, 0);
			
						t = tnext(t);
						continue;
					}

					if (!noexec) {
						t = tnext(t);
						continue;
					}
				}

				tokenT* loaderToken = t; //this is the one to apply the var loading logic to
				if (fpointer && !noexec) {
					loaderToken = mkToken(0, "loader", 0);
					zlist_addtail(&t->subs, loaderToken);
				}

				//for va`riables, (including function pointer variables) put the variable's address on the stack for now 
				//and then follow it with '@' to get it
				//for static arrays, this is not needed

				loaderToken->ty = findType(POINTERUSER, s->type, NULL, 0);  //pointer to the symbol's type
				if (local == 1) {
					loaderToken->handler = hlocal;
					loaderToken->skipargs = ZTRUE;
					t->useslocal = ZTRUE;
				}
				else if (local == -1) {
					loaderToken->handler = himmvar;
					loaderToken->skipargs = ZTRUE;
				}
				else if (pc->no_global_vars)
					ERR("Cannot access global variables at this time");
				else {
					loaderToken->handler = hglobal;
					loaderToken-> skipargs = ZTRUE;
				}

				loaderToken->val.as.ptr.block = 0;
				loaderToken->val.as.ptr.offset = s->offset;

				if (fpointer && !noexec) {
					  t->ty = s->type->ref->ref; //return value of function being called via pointer
					  //todo: check the bwlow behavior, OR forbif pointers to 'like' typed functions OR make function pointers be specific
					//s->type resolve_like_type(s->type->ref->ref, s->type->ref->members, v, ZTRUE);


					t->handler = hcall;
					t->val.as.n32 = 2; //indirect
					//t = tnext(t);

					printList(t, t, 0, 2);
				}else if ((s->type->category!=ARRAYSTATIC)&&(   s->type->category!=STRUCT || s->type->stacked  )) {  
					tokenT* tn = mkToken('@', "@", 1);  //load the variable	
					tn->generated = 1;
					insert_after(t, tn);
				}
							
				//printList(ts, t, ENDFILE, 1);
				t=tnext(t);
				continue;	
			}//end s
			

			







			//check if struct member
			if (t->str && t->str[0]=='.'){
				ram_free(v);
				v = NULL;
				//xprintf(" dot\n");
							
				typeT* ptype = tprev(t)->ty;

				//if we have a pointer to virtual
				if (ptype && (ptype->category == POINTERUSER) && (ptype->ref) && (ptype->ref->category == VIRTUAL)) {

					//printf(" access in virtual type \n");
					zuint32 j;
					int found = 0;
					for (j = 0; j < zvec_count(ptype->ref->selectors); j++) {
						symbolT* s = zvec_get_at(ptype->ref->selectors, j);
						//todo: check it really is a data selector
						if (!strcmp(s->name, t->str + 1)) {
							//get real type selector table
							t->handler =  hselectorptr;

							t->val.as.n32 = s->selectorNum;
							t->ty = findType(POINTERUSER, s->type, NULL, 0);//
							found = 1;
							break;
										

						

						}

					}

					if (found) {

						/* This part is copied from below for struct members */
						t->trackpossptr = tprev(t)->trackpossptr;  //if was tracking from a possessive pointer, still track this pointer is derived from that
						fold(tprev(t), t);

						//for static arrays or substructs (that are embedded (not pointers)) then the pointer addition already made a pointer to the substruct/array.  For other cases (it is a pointer to a struct, integer, etc) then insert a load token.  
						if ((t->ty->ref->category != ARRAYSTATIC) && (t->ty->ref->category != STRUCT)) { //todo maybe also type->stacked
							tokenT* tn = mkToken('@', "@", 1);  //load the variable
							tn->generated = 1;
							insert_after(t, tn);
						}
						/* end copy*/

						t = tnext(t);
						continue;
					}

				}

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
						
						/* This part is copied above for data selectors */
						t->trackpossptr = tprev(t)->trackpossptr;  //if was tracking from a possessive pointer, still track this pointer is derived from that
						fold(tprev(t),t);
						
						//for static arrays or substructs (that are embedded (not pointers)) then the pointer addition already made a pointer to the substruct/array.  For other cases (it is a pointer to a struct, integer, etc) then insert a load token.  
						if (( m->ref->category!=ARRAYSTATIC)&&( m->ref->category!=STRUCT || m->ref->stacked)) {  
							tokenT* tn = mkToken('@', "@", 1);  //load the variable
							insert_after(t, tn);
						}
						/* end copy*/
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
				if (ty) { //if the name of a type.  call parsetype to see if there are any qualifiers on it
					t = parseType(t);
					ty = tprev(t)->ty;
					tn = t;
				}
			}
			
			if (ty){
				ram_free(v);
				v = NULL;

				ts = tprev(tn);
				xprintf("Found type %s\n", ty->name);
				ts->ty = tType;
				ts->val.as.type = ty;
				ts->tyval = tType;
				ts->handler = hconstant;
				ts->skipargs = ZTRUE;
				t=tn;
				continue;
			}
			 
		

			if (matchApprox < MATCH_MAX_APPROX) {
				matchApprox++; //try next match approximation (ignore integer sign, use wildcard types, etc)
				ram_free(v);
				v = NULL;
				continue;
			}

			/*
			if (t!=ignoresigned) {
				ram_free(v);
				v = NULL;

				//printf("Attempt match by conversion to unsigned\n");
				ignoresigned = t;
				continue; //try again
			}
			ignoresigned = NULL;
			*/
			
			//WAS IT A TYPECAST?
			if (tnext(ts)->tok == TYPECAST) {
				ram_free(v);
				v = NULL;

				t = tnext(ts);
				ram_free(tremove(ts));
				continue;
			}


			//check if has '='
			char* eq = strchr(t->str, '=');
			if (eq && eq != t->str && !eq[1]){
				
				*eq = '\0'; //delete '='
				//tokenize(t, t->str);  //retokenize the shortened string (in case now its a keyword or something)

				insert_after(t, mkToken('=', "=", 0));
				ram_free(v);
				matchApprox = MATCH_EXACT; //try without = sign, using exact matching
				continue;
			}



			printf("\n\nERROR Undefined %s\n", t->str);

			if (v) {
				for (int n = 0; n < zvec_count(v); n++) {
					typeT* st = zvec_get_at(v, n);
					printType(st, ZFALSE, ZTRUE);
					printTypeNoRedirect(st, ZFALSE, ZTRUE);

				

					xprintf(" ");
					printf(" ");

				}
			}
			
			
     	 	ERR(" %s  %s:%d\n\n", t->str, t->sourcefile,  t->line);
					
		}//end str
		xprintf("?How to parse %x %c\n", t->tok, t->tok);
		ERR("Unimplemented\n"); 
			
	} //end while
	xprintf(" returning NULL token\n"); 
	return NULL;
}

//cleanup called after calling a C function
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

#include "cextra.c"		//hand-written extensions
#include "gen.c"		//generated extensions from C header files

extern scanmain(int argc, char** args);

int main(int argc, char** args){

	if ((argc > 1) && !strcmp(args[1] , "-scan")) {
		return scanmain(argc - 1, args + 1);
	}

	logfile = fopen("debug.log", "wb");

	if (!logfile)
		logfile = stdout;
	else
		printf("Debug output is to file\n");
		

	tany = mkType(SIMPLE, NULL, "any", 0); //matches any concrete type for any&, any%, [any%}, etc
	tvany = mkType(VIRTUAL, NULL, "vany", 0); //matches any virtual type for vany&, vany%, [vany%}, etc

	tType = findType(POINTERUSER, mkType(PENDING, NULL, "Type", 0), NULL, 0); //datatype about "types"
	tPrimitive = mkType( PRIMITIVE, NULL, "Primitive", 0 ); //allows lookup of C functions by name
	tZ32 = mkType( SIMPLE, NULL, "Z32", sizeof(zint32));
	tN32 = mkType( SIMPLE, NULL, "N32", sizeof(zuint32));
	tN8 = mkType( SIMPLE, NULL, "N8", sizeof(zbyte));
	tBit = mkType( SIMPLE, NULL, "Bit", sizeof(zbyte));
	tString = mkType(SIMPLE, NULL, "String", sizeof(char*));
	tImmediate = mkType(SIMPLE, NULL, "Immediate", sizeof(FLOAT));
	tCode = mkType(SIMPLE, NULL, "Code", 0); 
	tExecToken = mkType(PENDING, NULL, "ExecToken", 0);
	tSymbol = mkType(PENDING, NULL, "Symbol", 0);
	tEmptyStack = mkType(SIMPLE, NULL, "Empty", 0);

#ifdef FLOAT
	tReal = mkType(SIMPLE, NULL, "Real", sizeof(FLOAT));
#endif
	
	if (argc < 2)
		exit(1);

	char* fname = args[1];
	char* x = ram_loadstr(fname);

	global = mkcontext("global");
	
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
	
	tokenize(t, x, fname);
	
	ram_free(x);
	
	//add all the binary and unary operator handlers
	addhandlers(global);

#ifdef SET_EXTENSIONS
	SET_EXTENSIONS();
#endif
		
	parse( global, tnext((tokenT*)zlist_head(tokens)) );	
	checkUsage(zlist_head(tokens), zlist_tail(tokens));

	xprintf("Types:\n");
	zuint32 i;
 	for (i=0;i<zvec_count(types);i++)
		printType(zvec_get_at(types,i),ZTRUE, ZFALSE);
			
	printSymbols(global->symbols, "globals");
 	printList(zlist_head(tokens),NULL,ENDFILE, 0);

	start(global, tnext((tokenT*)zlist_head(tokens)) , NULL  );//run
	 
	clean_context_pointers(global->symbols, global->exec->globalvars);
	if (immediate_parse ) {
		clean_context_pointers(immediate_parse->symbols, immediate_parse->exec->immediatevars);
	}
	
	ram_free(tokens);
	ram_free(global);
	ram_free(immediate_parse);
	ram_free(types);
	ram_free(csizes);
	ram_free(primitives);

	ram_allocs(); //dump memory leak list
	return 0;   
}

