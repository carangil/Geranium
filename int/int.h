#pragma once
#include "zmem.h"
#include "zarray.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"
#include "zvector.h"


//enable float and make adjustable precision   (if suffix is f, then sin cos before cosf, sinf, the single precision version
#define FLOAT zfloat32
#define MATH_FUNC_SUFFIX(FFF)   FFF ## f


union ptrtype {
	char* bytes;
	struct tokenS* token;
	struct symbolS* symbol;
	struct typeS* type;
	struct valueS* stackval;
	struct parsectxS* parsectx;
};

typedef struct vptrS {
	union ptrtype addr;

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
	
	//struct symbolT* symbol;


} valU;

typedef struct valueS {
	valU as;
	struct typeS* typeselector;  //selector table for the current type's view
	
}valueT;


//the error is text to display to the user, such as compile errors.
#define ZERROR_GENERIC	0x01000000
//stop until we have a frame that handles errors
#define ZERROR_STOP		0x02000000	

//quit the application immediately
#define ZERROR_QUIT		0x04000000

//other flags might be like:
#define ZERROR_MATH (0x1000 | ZERROR_STOP)
#define ZERROR_NULL (0x2000 | ZERROR_STOP)
#define ZERROR_IO	(0x4000)
#define ZERROR_BRK	(0x8000 | ZERROR_STOP)

typedef struct {
	char* message;
	char* file;
	int line;
	int code;
} errorT;


#define ERR(...)									fsetError(NULL, zstrprintf(NULL, __VA_ARGS__), NULL, ZERROR_QUIT)
#define setError(pobj, ptoken, MSG)					fsetError(&(pobj)->error, zstrdup(MSG), ptoken, ZERROR_GENERIC)
#define setErrorf(pobj, ptoken, ...)				fsetError(&(pobj)->error, zstrprintf(NULL, __VA_ARGS__), ptoken, ZERROR_GENERIC)
#define setErrorFlags(pobj, FLAGS, ptoken, MSG)		fsetError(&(pobj)->error, zstrdup(MSG), ptoken, FLAGS)
#define setErrorFlagsf(pobj,FLAGS, ptoken, ...)		fsetError(&(pobj)->error, zstrprintf(NULL, __VA_ARGS__), ptoken, FLAGS)
#define getError(pobj)								fgetError(&(pobj)->error)
#define moveError(DST,SRC)							fmoveError( &(DST)->error, &(SRC)->error)
void fsetError(errorT* error, char* message, struct tokenS* t, int flags);

/**** Execution Context ****/
typedef struct exectxS {
	valueT* stack;  //call/parameter stack
	zuint32	sp;	//number of items on stack.  sp-1 is the top item
	zuint32 fp;
	char* vars; 	//local data space
	char* globalvars; //global data space
	char* immediatevars; //global data space for immediate blocks
	errorT error;
	int stop;
	int level;//stackframe level
	int debugstack;
	struct parsectxS* in_immediate; //if executing in an immediate context, this is that context.  NULL othersize
	}exectxT;
#define STOPFUNC 1
#define STOPLOOP 2
#define STOPBLOCK 4
#define RELOOP 8
#define STOPERROR  16

//tokens for interpreter
//custom handlers should restrict to using a few macros

//get next instruction
#define tnext(ITEM)		((tokenT*)zlist_next(ITEM))

//get instructions for args
#define tsub(TTT)		((tokenT*)zlist_head(&(TTT)->subs))

#define ttok(TTT)		((TTT)->tok)
#define tstr(TTT)		((TTT)->str)
#define ttype(TTT)		((TTT)->ty)


//executor
void exe(exectxT* c, struct tokenS* t);



typedef struct tokenS* (*instruction) (exectxT*, struct tokenS*);

#define SPLIT_SELECTOR_POINTER 1

typedef struct tokenS {
	zlistnodeT zlistnode;
	zuint32 tok;	//A constant defined below, a character, or a pair of characters
	char* str;	//string representation of this token

	
	int ty_flags;
	struct typeS* ty;	//datatype of this token
	struct typeS* tyorig;	//original datatype of this token (before cast)
	
	struct typeS* traced_like_type;

	valueT val;	//token's constant value
	struct typeS* tyval; //type of the above constant
	zlistT subs;	//make a tree out of token list
	instruction handler;    //function that does what this token represents
//	instruction arghandler; //function that evaluates the args
	zbool skipargs;
	struct symbolS* sym;  //for things like procs that have a bunch of context info
	
	char* sourcefile; //what file
	int line;

	zbool val_to_free; //if true, free val's ptr block when destroying token
	int useslocal; // INSTEAD OF TRUE/FALSE, THIS IS A COUNT. if true this code (or its subtrees) refers to local variables (as opposed to global or immediate space)
	int generated;
	struct parsectxS* restrict_parse_context; //for SUBTREEs... only can be included in the same context they were created

	struct tokenS* trackpossptr;
	struct tokenS** debug_subs;
	struct tokenS** debug_next;
	struct tokenS** debug_prev;
	zbool breakpoint;

}tokenT;



//proc search matchApprox values

#define MATCH_EXACT			0
#define MATCH_IGNORE_SIGNED	1
#define MATCH_VIRTUAL		2
#define MATCH_LIKE  		3
#define MATCH_ALLOW_WILD	4
#define MATCH_MAX_APPROX	4

// exact:  only the exact same types
// ignore signed:  N32 and Z32 considered the same
// match virtual:  can pass a real pointer as virtual (adds a typeselector
// match like:  'like' types are resolved
// allow wild:  any&, any% will match things