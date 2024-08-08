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



typedef struct vptrS {
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

	//debugging:
	struct symbolT* symbol;
	struct tokenS* token;


} valU;

typedef struct valueS {
	valU as;
	struct typeS* typeselector;
}valueT;

/**** Execution Context ****/
typedef struct exectxS {
	valueT* stack;  //call/parameter stack
	zuint32	sp;	//number of items on stack.  sp-1 is the top item
	zuint32 fp;
	char* vars; 	//local data space
	char* globalvars; //global data space
	char* immediatevars; //global data space for immediate blocks
	int stop;
	int level;//stackframe level
	int debugstack;
	struct parsectxS* in_immediate; //if executing in an immediate context, this is that context.  NULL othersize
	}exectxT;
#define STOPFUNC 1
#define STOPLOOP 2
#define STOPBLOCK 2

//tokens for interpreter
//custom handlers should restrict to using a few macros

//get next instruction
#define tnext(ITEM)		((tokenT*)zlist_next(ITEM))

//get instructions for args
#define tsub(TTT)		((tokenT*)zlist_head(&(TTT)->subs))

#define ttok(TTT)		((TTT)->tok)
#define tstr(TTT)		((TTT)->str)
#define ttype(TTT)		((TTT)->ty)


//executre
void exe(exectxT* c, struct tokenS* t);


typedef struct tokenS* (*instruction) (exectxT*, struct tokenS*);



typedef struct tokenS {
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
	int useslocal; // INSTEAD OF TRUE/FALSE, THIS IS A COUNT. if true this code (or its subtrees) refers to local variables (as opposed to global or immediate space)
	int generated;
	struct parsectxS* restrict_parse_context; //for SUBTREEs... only can be included in the same context they were created

	struct tokenS* trackpossptr;
	struct tokenS** debug_subs;
	struct tokenS** debug_next;
	struct tokenS** debug_prev;

}tokenT;
