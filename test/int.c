#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"
#include "zvector.h"

/* Program is a linked list of tokens*/

typedef struct tokenS{
    zlistnodeT zlistnode;
    zuint32 tok;
    char* str;
    struct typeS* ty;
}tokenT;

tokenT* mkToken(zuint32 tok, char* str, zuint32 len){
    tokenT* t = ram_alloc( sizeof(tokenT) , NULL );
    t->tok = tok;
    if (str)
	t->str = zstrndup(str, len);
    return t;
}

char* safestr(char* s){
   return s ? s:""; 
}

void printList(tokenT* t, tokenT* cur, zuint32 stop_tok){
   
    char* iscur;
    
    for ( ;t;  t = zlist_next(t)  ){
	
	if (t==cur)
		iscur="CUR";
	else 
		iscur ="";

	if ( (t->tok >20) && (t->tok < 0x7f))

		printf("{%s %c %s}->",iscur, t->tok , safestr(t->str));
	else
		printf("{%s %x %s}->",iscur, t->tok , safestr(t->str));


	if (t->tok == stop_tok)
		break;
    }
    printf("{end}\n\n");
        
}


/* This first part parses an input string and returns a linked list of tokens
 * Very little is verified, things are broadly classified as pairs (such as ->, --, etc),
 * as single character tokens (+,-, etc), names (alpha_numeric_123), numbers (1.0E-4, 0x100, etc)
 * literals  "boo", whitespace, etc.
 * As whether a particiar token such as '-' is really on operator or part of a number it's preceding, that's figured later
 * Things like 0x10.4E-4 is a 'NUMBER', but will later fail as it isn't a valid form
 */

//used to recognize 2-letter combinations like ->, etc
char* findPair(char* patterns, char a, char b){
    for(  ;*patterns;patterns+=2){
	if ( ((*patterns)==a) &&(*(patterns+1)==b))
	    return patterns;
    }
    return NULL;
}

//used to either recognize names or numbers
int acceptPatterns(char* s, char* startChars, char* continuePairs,  char* continueChars){
    int i=0;
    char* st =s;
    char* p=0;
    

    if (strchr(startChars, *(s++))){
	i++;
	
	for(;*s;i++,s++){
	
	    	if (p=findPair(continuePairs,*s,*(s+1))){
		    s++,i++;
		    continue;
		}

		if (strchr(continueChars, (*s))){
		    continue;
		}
	
		break;
	}
			
    }
    // printf(" took %d bytes %.*s  \n",i, i, st);
    return i;
}

//scans through string literals
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

#define PAIR		0x100
#define NAME		0x200
#define NUMBER		0x300
#define LITERAL 	0x400
#define ENDFILE		0x500
#define PASTENDFILE	0x600
#define STARTFILE	0x700
#define KEYWORD		0x8000


zlistT* tokenize(zlistT* list, char* in){
    
    int c,next;
	tokenT* t=NULL;
    
    int i;
    
    if (!list) {
	list = ram_alloc(sizeof(zlistT), NULL);
	t = mkToken(STARTFILE, NULL,0);
	zlist_addhead(list,&t->zlistnode);
	t->zlistnode.prev=&t->zlistnode;  //'trap' so ->prev->prev is always safe
    }
     	
    while (c = *in){
	next = *(in+1);
	char*p;
	
	//find twochar patterns like ->,etc. including comment start/end markers
	if (p=findPair("<<>>--++->==||&&+=-=/=*=&=|=^=/**///", c, next)){
	  
	   
	   if (!strncmp(p, "//",2)) { //special handling for // comments
	       while(*in!= '\n')
		   in++;
	       continue;
	   }
	   
	    t = mkToken(PAIR, p, 2);
	    	   
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
	
	//fold spaces and tabs together
	int space = acceptPatterns(in, " \t\n\r", "", " \t\n\r");
	
	if (space) {
	    in+= space;
	//ignore whitespace
	#if 0
	    t = mkToken(' ');
	    t->str=zstrdup(" ");
	    zlist_addtail(list, &t->zlistnode);
	#endif
	    continue;
	}
	
	int digits  = acceptPatterns(in,
			".0123456789", //start with digit or decimal point
			"e-E-e+E+",  //- and + only accepted after an e or E
			"0123456789abcde.fABCDEFxlLuUfF"); //ontinues with figits, deccimal point, hex letters, type suffix letters
		
	int name = acceptPatterns(in, 
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",
			"",
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
	
//	printf(" %d accepted as space %.*s\n", space, space, in);
//	printf(" %d accepted as digits %.*s\n", digits, digits, in);
//	printf(" %d accepted as name %.*s\n", name, name, in);
		
	if (digits && name  ){
		printf(" ambiguous name or number %s\n", in);
		exit(1);
	}
	
	if (digits || name){
	    t = mkToken( digits? NUMBER : NAME, in ,   digits|name);
	    in += digits|name;
	     zlist_addtail(list, &t->zlistnode);
	     continue;
	}
	
	//just some char
	t = mkToken( *in, NULL, 0);
	zlist_addtail(list, &t->zlistnode);
	in++;
		
    }
    //something should stop reading at endfile
    tokenT* end = mkToken(ENDFILE, NULL, 0);
    end->str = zstrdup("ENDFILE");
    zlist_addtail(list, &end->zlistnode);

    //if program ever advances reads PASTENDFILE, its an error
    //this is a trap so 'next->next->next' always is safe
    end = mkToken(PASTENDFILE, NULL, 0);
    zlist_addtail(list, &end->zlistnode);
    end->str = zstrdup("PASTENDFILE");
    end->zlistnode.next = (zlistnodeT*) end;

    return list;
}


#define ERR( ...) { fprintf(stderr,__VA_ARGS__); exit(1);}

/* Simple type system*/
#define SIMPLE	0
#define POINTER 1
#define STRUCT 	2
#define ARRAY 	3
#define FUNCTION 4
#define LAST_REAL_TYPE 4

//MEMBER is not a type, but is used to mark members of a struct
#define MEMBER	5
//NAMED is not a type, but when passed into findType looks for struct or simple type
#define NAMED	6
//PEDNING not a type, but is for when a type is mentioned in another declaration but not yet defined
#define PENDING 7


char* typeString[] = {"simple", "pointer", "struct","array","function","-member-","-named-", "-pending-", "-tempfunc-" };
char* getTypeString(zuint32 a){
    if (a < sizeof(typeString)/sizeof(typeString[0]))
	return typeString[a];
    
    return "invalid";
}

typedef struct typeS{
    char* name;
    size_t size;
    zuint32 category;
    zuint32 len; //for definite arrays
    zuint32 offset; //for struct members
    struct typeS* ref; //array or ointer types, or function return type
    zvecT* members;  //structs or function parameters
    int tid;
    zbool pending;
}typeT;

zvecT* types;
int tid=0;

typeT* mkType(zuint32 category, typeT* ref, char* name, size_t szlen){
    typeT* ty= ram_alloc(sizeof(typeT), NULL); //todo: destructor
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
	ty->size = sizeof (void*);
    }
    
    if (types == NULL)
	types = zvec_mk(NULL, 100);
  
	zvec_add(types, ty);
    
    return ty;
}

void printType(typeT* ty, zbool line, zbool skipmembers){
	int i;
	if (ty){
	   
	    if (ty->category == FUNCTION)
		skipmembers=ZFALSE;
	    
	    if (ty->category == ARRAY)
		printf("{#%d\t%d:%s\t%zu[%d]\t%s", ty->tid, ty->category,  getTypeString(ty->category), ty->size, ty->len, safestr(ty->name));
	    else if (ty->category == MEMBER)
		printf("{#%d\t%d:%s\t+%d\t%s", ty->tid, ty->category,  getTypeString(ty->category), ty->offset,  safestr(ty->name));
	    else
		printf("{#%d\t%d:%s\t%zu\t%s", ty->tid, ty->category,  getTypeString(ty->category), ty->size,  safestr(ty->name));
	    
	    if (ty->ref)
		printType(ty->ref, ZFALSE, ZTRUE);
	    
	    if (ty->members && ! skipmembers ){
		printf("(\n");
		
	    
		for (i=0;i<zvec_count(ty->members); i++) {
		    
		    printType(zvec_get_at(ty->members, i), ZTRUE,   ZTRUE   );
		}
    
		printf(")");
	    }
	    printf("}");
	    
	    
		
	} else {
	    printf("{nulltype}");
	}

	if (line)
	    printf("\n");
}
	      
	      //todo: find/compare function types
	      
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
		printf(" functions return different types\n");
		return ZFALSE;
	}
	
	if (zvec_count(ty->members) !=zvec_count(ref->members)){
		printf(" function has different num of arguments\n");
		return ZFALSE;
	}
	
	int i;
	for (i=0;i<zvec_count(ty->members);i++){
	    typeT* memberty = zvec_get_at(ty->members,i);
	    typeT* memberref = zvec_get_at(ref->members,i);
	    //compare types of members
	    if (memberty->ref != memberref->ref){
		printf("arg %d to function is of different type\n", i);
	    }
	}
	return ZTRUE; //function type is the same
    }
    
      //check array ref value
     if (ty->ref != ref)
	return ZFALSE;
    
    
    return ZTRUE;
}
	      
typeT* findType(zuint32 category, typeT* ref, char* name, size_t len){
    
    typeT* ty;
    typeT* found=NULL;
    int i;
    
    for (i=0; i< zvec_count(types);i++){
	ty = zvec_get_at(types, i);
	
	if (category != NAMED) {
	    //if category is named, we match whether its a struct or int (caller doesn't know which it is yet)
	    
	    if (ty->category != category)
		continue;
	}
	
	switch (category){
	    
	    case SIMPLE: //simple types matched by name only
	    case STRUCT:
	    case NAMED:
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
    
	printf(" Creating %s type for ", getTypeString(category));
	printType(ref, ZTRUE, ZFALSE);
	
	ty = findType(ref->category, ref->ref, ref->name, ref->len);
	if (ty){
	    return mkType( category, ty, NULL, len);
	}
    
    }
    
    ERR("could not find or create appropriate type\n");
    return NULL;
    
    
    
}


/* Parse out stuff */

#define tnext(ITEM) ((tokenT*)(ITEM)->zlistnode.next)
#define tprev(ITEM)    ((ITEM)?((tokenT*)(ITEM)->zlistnode.prev):NULL)


tokenT*  parseVar(tokenT*, zbool name_required, char** nameOut, typeT** typeOut) ;
tokenT*  parseTypeList(tokenT*, typeT* parent);



tokenT*  parseType(tokenT* t) {
    
	char* count=NULL;

	printf(" Enter parseType\n");

	zbool named=ZFALSE;

    for (; t;  t = (tokenT*)( t->zlistnode.next)) {

	printList(tprev(tprev(t)),(void*)t,ENDFILE);


	if (t->tok == '['){ //array type

		if (tnext(t)->tok == NUMBER){
			t=tnext(t);
			count = t->str;
		}
				
		t  = parseType( tnext(t)); //parse the type
		if (t->tok != ']'){
			printf(" missing ]\n");
		}

		t->str = zstrcat( zstrdup("array ("), tprev(t)->str );
		if (count){
			t->str = zstrcat( t->str, " x ");
			t->str = zstrcat( t->str,  count );
		}

		t->str = zstrcat( t->str, ")");

		printf(" t->str is %s\n", t->str);

		t->ty = findType( ARRAY, tprev(t)->ty, NULL,atoi(count) );
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
		continue;
	}
	if (t->tok == '*'){ //pointer type
		t->ty = findType( POINTER, tprev(t)->ty, NULL,0);
		printf(" mk pointer to %s\n", tprev(t)->str);
		t->str = zstrcat( zstrdup("{pointer to("), tprev(t)->str);
		t->str = zstrcat( t->str,")}");
		printf(" made %s\n", t->str );
		continue;
	}
	if (t->tok =='('){ //type list for function parameters & return value
		printf(" start parseTypeList\n");
		
		typeT* ty = mkType( PENDING, NULL, NULL, 0); 
		
		t = parseTypeList(tnext(t), ty);
		
		printf(" type list contains: \n");
		printType(ty, ZTRUE,ZFALSE);
		
		printf(" parseTypeList returned at\n");
		//printList(tprev(tprev(t)), t, ENDFILE);

		if (t->tok !=')'){
			ERR("Expected )\n");
		}
		t->str = tprev(t)->str;  //typelist is done, so the closing ')' takes all the data
		
		
		//t->ty = findType( ty;          //closing paren takes the 'type' of everything inside
		t->ty = findType( FUNCTION, ty, NULL, 0); //either adds this function to the type list, or returns the version already existing
		    
		named=ZTRUE;

		continue;
		
	}

	printf(" last was %s\n", tprev(t)->str);
	break;
    }

	return t;
}

tokenT*  parseTypeList(tokenT* t, typeT* parent) {
    		
		if (parent && !parent->members){
			parent->members = zvec_mk(NULL, 10);
		}
		size_t offset=0;
		
		zbool reqname=ZTRUE; //parameters must be named
		
		for(;t;t=tnext(t)){

			char* name = NULL;
			typeT* type = NULL;
			
			//special case:  a typelist might be in a list that ends with 'end', and it should not be processed as a varname
			if ( (t->tok == NAME) && !strcmp( t->str, "end"))
			    break;
			
			
			t = parseVar(t, reqname, &name, &type  );
			
			if ( (reqname && !name) || !type){
				ERR(" type or name missing for struct member or function arg\n");
			}
			
			if (type->category > LAST_REAL_TYPE){
				ERR("Type %s cannot be in struct\n", getTypeString(type->category));
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
			
			

			if (t->tok == ';'){
			    continue; //list item seperator
			}
			
			if ( (t->tok == PAIR) && !strcmp(t->str, ">>")){
				//is a function
				parent->category = FUNCTION;
				reqname = ZFALSE; //no longer need names (returned values are anonymous)
				
				
				continue;

			}
			break;

		}

		//reached end of a type list, guess should group them up

			
		printf(" endtype list at \n");
		printList(tprev(t), t, ENDFILE);

		return t; //closing paren on func parm list OR 'end' in typedef
}

tokenT*  parseVar(tokenT* t, zbool name_required,  char** nameOut, typeT** typeOut) {
    char* name=NULL;


	if (t->tok == NAME && tnext(t)->tok == ':'){
		name = t->str;
		t=tnext(tnext(t));
		printf("Name is %s and type follows\n", name );
	}
	if (name==NULL && name_required){
		ERR("var name required\n");
	}
	if (nameOut)
	    *nameOut = name;
	
	t = parseType(t);
	
	if (typeOut)
	    *typeOut = tprev(t)->ty;  //get the type that was last parsed
	
	if (tprev(t)->ty){
		printf(" parsetype returned type ");
		printType(tprev(t)->ty, ZTRUE, ZFALSE);
	}
	    
	
	printf("parsetype returned at\n");
	
	printList(tprev(tprev(t)), t, ENDFILE);
	if(name){
		
		tprev(t)->str = zstrcat(tprev(t)->str, " named ");
		tprev(t)->str = zstrcat(tprev(t)->str, name);

	}
	return t;

}

tokenT*  parse(tokenT* t) {
    
    
    for ( ; t;  t = (tokenT*)( t->zlistnode.next)) {

	if (t->tok==ENDFILE || t->tok== PASTENDFILE)
	    break;
		
	if (t->tok == NAME){

	    if (!strcmp(t->str, "var")){

	    	t = parseVar( tnext(t), ZTRUE, NULL, NULL); //parse variable; name is required

		printf(" Type returned was %s\n", tprev(t)->str);
		if (t->tok != ';')
		    printf(" missing ;\n");
		
		continue;
	    }

           if (!strcmp (t->str, "type")){
	   	char* name = NULL;
		if (tnext(t)->tok == NAME) 
			name = tnext(t)->str;
		else {
			ERR("expected type name\n");
			
		}
	    
		typeT* ty = findType(NAMED, NULL, name, 0); //find a type by name
		
		if (ty && ty->category != PENDING){
			ERR("redefining type %s\n", name);
		}
		
		if (ty)
		    	ty->category = STRUCT; //if it was pending, its a real struct now
		else
		    ty = mkType( STRUCT, NULL, name, 0);  //create a struct type
		
		t = parseTypeList(tnext(tnext(t)), ty);
		
		if(t->tok == NAME && !strcmp(t->str, "end")) {
			printf(" Defined type %s as %s\n", name, tprev(t)->str);
			continue;
		}
		
	
		
		ERR( " Expected 'end' for type\n");
		
	   }

	}
    }

}



 
int func(int a, int b){
    printf(" two %d %d\n", a, b);
    return a+b;
}

int   (*getFunc(int a)) (int a, int b)      {
    printf(" getFunc called %d %d n", a );
    return func;
}

int (*(*getGetFunc(void))(int a))(int a, int b){
	
	return getFunc;
   }



int main(int argc, char** args){
    
	//char* t = "int x; void main(int c, char x){printf(\"boo\"";
	  
 //   int (*f)(int,int) = getGetFunc()(5);
   // f(1,2);
    
    mkType( SIMPLE, NULL, "Z32", sizeof(int));
    mkType( SIMPLE, NULL, "Z16", sizeof(short));
    mkType( SIMPLE, NULL, "Z8", sizeof(char));
    mkType( SIMPLE, NULL, "N32", sizeof(unsigned int));
    mkType( SIMPLE, NULL, "N16", sizeof(unsigned short));
    mkType( SIMPLE, NULL, "N8", sizeof(unsigned char));
    mkType( SIMPLE, NULL, "R32", sizeof(float));
    mkType( SIMPLE, NULL, "any", 0 ); //not really a type.. but to support any*   (aka void*)
    
    
    if (argc < 2)
	exit(1);
    
	char* x = ram_loadstr(args[1]);
	
	zlistT* tokens = tokenize(NULL, x);
    
	printList((tokenT*) tokens->head,NULL,ENDFILE);
    
	parse( (tokenT*) tokens->head->next );
    
	int i;
	for (i=0;i<zvec_count(types);i++)
	    printType(zvec_get_at(types,i),ZTRUE, ZFALSE);
    
	return 0;   
}

