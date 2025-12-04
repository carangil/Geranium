#ifndef INT_H
#define INT_H

#include "int.h"
#include "zmem.h"
#include "zlist.h"
#include "zstring.h"
#include "zstringmap.h"
#include "zarray.h"

//default float precision
#define FLOAT float

// Interpreter

//tokens

typedef struct tokenS{

    zlistnodeT zlistnode;
    char* str;  //actual token string
    unsigned int tok; //which token
    struct typeS* type; //what type is this
    char * sourcefile;
    int line;
    char * comment;
   // zlistT subs;

}tokenT;

tokenT* int_insert_tokenf(tokenT* A, tokenT* B, zbool before);

#define tinsert_after(AFTER,NEW)    int_insert_tokenf(  AFTER, NEW, ZFALSE)
//#define tinsert_before(PREV,NEW)    int_insert_tokenf(  PREV, NEW, ZTRUE)
#define tremove(ITEM)    zlist_remove_mid(  &(ITEM)->zlistnode)
#define tnext(ITEM)		((tokenT*)zlist_next(ITEM))
#define tprev(ITEM)    ((tokenT*)zlist_prev(ITEM))


//types/symbols etc

/*
 * to
 *
 *
 */

//categories



typedef enum {
    NAMED=0,    //searching for a type by its name
    OPAQUE,
	PENDING, //type details aren't defined yet.  But you could have pointers to them, etc
	LITERALTOKEN, //the type is the value.  for putting keywords in word args

		//
    //callable things
//    PRIMITIVE,
      WORD,

    //variable-like things
//    VARIABLE,
//    PARAMETER,
//    MEMBER,

    //kinds of values
    SIMPLE, //ints, etc
    REFERENCE,
	STRUCT
	
}categoryE;



typedef struct wordS{
    char* key;
    char* name;
    struct parsectxS* pctx;
    struct typeS* type;
    tokenT* (*parse) (struct parsectxS* pctx, struct wordS* word, tokenT* t, int argc);
} wordT;

typedef struct typeS{
    char* key;
    char* name;
    struct typeS* ref;
    zstringmapT* members; //members are args to functions, parts of a struct etc   name is key, value is typeT*
	int argc;//number of args
    categoryE category;
    int size; 
} typeT;

//generic primitive
typedef struct parserS{
    char* name;

    //func to run at parse time
    tokenT* (*parse) (struct parsectxS* pctx, tokenT* t, void* machine);

    //void (*exec) (parsectxT* pctx, tokenT* t, void* machine);

}primitiveT;

typedef struct {
	primitiveT prim;


}Cprimitive;	//a C primitive runs a C function at runtime

typedef struct ptrS{
	union {
		void* block;
		char* bytes;
		typeT* type;
		wordT* word;
	} address;
	int offset;
}ptrT;

typedef struct valueS{
	union {
		FLOAT f;
		zint32 z32;
		zuint32 u32;
		ptrT ptr;
	} as;
}valueT;
		
typedef struct parsectxS{
    zstringmapT* dictionary; //wordT*
    zstringmapT* types;      //typeT*
    zstringmapT* primitives; //primitiveT*

	typeT**		typestack;
	valueT*		valuestack;
	int sp; 

}parsectxT;



//runtime
/*
union ptrtype {
	char* bytes;
    typeT* type;
    wordT* word;
    tokenT* token;
};

typedef struct vptrS {
	union ptrtype addr;
	zuint32 offset;
}vptrT;
*/

//functions

void int_run_str(char* src, char* filename);


#endif
