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
//stack size is in values
#define STACK_SIZE 1024

typedef struct ptrS{
	union {
		void* block;
		char* bytes;
		struct typeS* type;
		struct wordS* word;
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
	//put type info here for interfaces
}valueT;


//tokens

typedef struct tokenS{

    zlistnodeT zlistnode;
    char* str;  //actual token string
    unsigned int tok; //which tokene
  //  struct typeS* type; //what type is this

    char * sourcefile;
    int line;
    //char * comment;

    //valueT val;

    //zlistT subs;

}tokenT;

tokenT* int_insert_tokenf(tokenT* A, tokenT* B, zbool before);

#define tinsert_after(AFTER,NEW)    int_insert_tokenf(  AFTER, NEW, ZFALSE)

#define tnext(ITEM)		((tokenT*)zlist_next(ITEM))
#define tprev(ITEM)    ((tokenT*)zlist_prev(ITEM))



//types/symbols etc

/*
 * to
 *
 *
 */

//categories
//enums for typeT

typedef enum {
    NAMED=0,    //searching for a type by its name, creating a pending type if not existing
    NAMEDEXISTING, //searching for a type by its name, but only if it already exists
    OPAQUE,
	PENDING, //type details aren't defined yet.  But you could have pointers to them, etc
	PROC,
	SIMPLE, //ints, etc
	REFERENCE,
	FRAME,	//struct or proc frame
	VARIABLE, //not a pointer, but refers to the variable itself
	LOADED,  //refers to a value loaded from a variable.
	LIKE
}categoryE;

struct exectxS;

typedef struct wordS{
    //char* key;
    char* name;
    struct typeS* type; 


//	valueT data;
	//broken out seperate instead of a union so that I fault on a null pointer instead of accidently reading garbage
	struct typeS* target_type;// if word represents a type, this points to it
    struct parsectxS* target_pctx;// if word represents a proc or struct, these are the variables/fields
//	struct tokenS* target_tokens; //if word represents a proc, this is the code

    //special handler during parse phase
    tokenT* (*parse) (struct exectxS* exe, struct parsectxS* pctx, struct wordS* word, tokenT* t, int argc);

	int opcode; //what opcode to use to call this function (can be a primitive like 'add', could call a c function, etc...)
	valueT val;
	struct typeS* val_type;
	char* comment;
} wordT;



typedef struct typeS{
    char* key;
    char* name;
    struct typeS* ref;
	//    zstringmapT* membermap; //members are args to functions, parts of a struct etc   name is key, value is typeT*
	int argc;//number of args

	struct typeS** argtypes; //if has args (PROCs)

	struct wordS* word; //word that represents this type

	struct parsectxS* pctx;//if the type is a frame, it has a context

	char** opt_argnames;  //holds name of args for a proc, optional

    categoryE category;
    int size; 
} typeT;

//means it has an

//
#define INST_FREE_VALUE 1

//#define INST_ NEW VALUE  2

typedef struct instructionS{
	int opcode;
	valueT val;
	typeT* val_type;
	struct instructionS** args;
	typeT* result_type;
	int    flags;
	char* comment;
}instructionT;

#define MAXRUNNERS		2
#define SWITCHRUNNER	0

struct runnerS;

typedef struct parsectxS{
	struct parsectxS* parent; //where to get things not found here
    zstringmapT* dictionary; //wordT*
    zstringmapT* types;      //typeT*
	int id;//for debugging
	zvecT* codestack;
	struct runnerS *runners[MAXRUNNERS];
}parsectxT;

#define ERROR_PARSE		1
#define ERROR_RUNTIME	2

typedef struct exectxS{
	valueT*		stack;

	//todo typeinfo for selectors?
	int sp;
	int bp;//bp=-1 is last arg, bp=-2 is one before that...

	char* error_string;
	int error_code;

}exectxT;

#define iferr(EXECTX)  if((EXECTX)->error_code)
#define ifok(EXECTX)  if(!(EXECTX)->error_code)

//instruction enums

#define op_MAX 255

typedef enum {
    op_nop=0,
	op_constant,
	op_print32,
	op_add32,
	op_stop,

	//get pointer to 'static' struct OR get value of variable
	op_globalvar,
	op_localvar,
	op_subvar,
	op_reference,


	op_load,
	op_loadaddref,
	op_take,
	op_store,

	op_follow,

	op_trash,
	op_alloc,
	op_allocarray,
	op_call,
	op_callp,
	op_handler,	//call a handler func (instead of being builtin opcode)
	op_argpick,
	op_return,
	op_returnval,
	op_loop,
	op_breakcontinue,
	op_cond,
	//op_condblock,	// condblock(  ( bool ...) (bool ...) (bool ...) ( 'true' ...  ) ),
	op_typeof,	//static compile-time
	op_dynamic_typeof,
	op_change_selector,
	op_FIRST_EXT  //first instruction that isn't part of this enum
}opcodeE;

//runtime

void int_run_str(char* src, char* filename);


#endif
