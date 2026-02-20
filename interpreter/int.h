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
		struct valueT* value;
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
	FRAME,	//struct or proc frame
	REFERENCE,
	STEWARD,  //owns the pointer to an object
	ARRAY,	//pointer to array
	//INDEX, //element of an array
	VARIABLE, //not a pointer, but refers to the variable itself
	ARG,
	LIKE,
	DEREFERENCE// usually deref just strips off the pointer wrapper, but this is for 'like' types where we have to delay doing that.  This is dereference the like type when resolving
}categoryE;

struct exectxS;

typedef struct wordS{
    //char* key;
    char* name;
    struct typeS* type; 


//	valueT data;
	//broken out seperate instead of a union so that I fault on a null pointer instead of accidently reading garbage

    struct parsectxS* target_pctx;// if word represents a proc or struct, these are the variables/fields
//	struct tokenSf* target_tokens; //if word represents a proc, this is the code

    //special handler during parse phase
    tokenT* (*parse) (struct exectxS* exe, struct parsectxS* pctx, struct wordS* word, tokenT* t, int argc);

	int opcode; //what opcode to use to call this function (can be a primitive like 'add', could call a c function, etc...)
	valueT val;
	struct typeS* val_type;
	char* comment;
	struct wordS * aliases; //find all the aliases for freeing
	int offset;
	zbool autoload; //if true, word will compile an '@' after it, unless & is used or there is more pointer manipulation
} wordT;



typedef struct typeS{
    char* key;
    char* name;
    struct typeS* ref;
	//    zstringmapT* membermap; //members are args to functions, parts of a struct etc   name is key, value is typeT*
	int argc;//number of args for procs.  argn for 'like' types
	//int argleave; //number of input args that are left behind on stack

	struct typeS** argtypes; //if has args (PROCs)

	struct wordS* word; //word that represents this type

//	struct parsectxS* pctx;//if the type is a frame, it has a context

	char** opt_argnames;  //holds name of args for a proc, optional
	int		arena_index; //which arena (0 is global heap)

    categoryE category;
    int size; 
	zbool is_wild; //true if has any wildcards
} typeT;

//means it has an

//f
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
	zvecT* codestack;
	zvecT* cleanlist;  //list of pointers that need to be freed
	struct runnerS *runners[MAXRUNNERS];
	int id;//for debugging
	int size;
}parsectxT;

#define ERROR_PARSE		1
#define ERROR_RUNTIME	2

typedef struct exectxS{
	valueT*	stack;
	valueT*	globals;
	valueT*	locals;

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

//opcodes are

typedef enum {
    op_nop=0,
	op_constant,
	op_print32,
	op_printptr,
	op_printstr,
	op_add32,
	op_stop,
	op_block,  //runs the instructions inside
	op_if,
	//get pointer to 'static' struct OR get value of variable
	op_globalvar,
	op_localvar,
	op_subvar,
	op_arrayindex,
//	op_reference,
	op_load,
	op_loadaddref,
	op_take,
	op_trash,
	op_store,
	op_trashstore,
	op_dim,


//	op_alloc,
//	op_allocarray,
	op_call,
//	op_callp,
//	op_handler,	//call a handler func (instead of being builtin opcode)
	op_argpick,
	op_argtake,
	op_argaddref,
	op_return,
	op_returnval,
	op_loop,
	op_break,
	op_continue,
	op_cond,

	//op_condblock,	// condblock(  ( bool ...) (bool ...) (bool ...) ( 'true' ...  ) ),
	//op_typeof,	//static compile-time
	//op_dynamic_typeof,
	//op_change_selector,


	//these ones are implementation specific:
	op_switch_jump,
	op_switch_jumpfalse,


	op_FIRST_EXT,  //first instruction that isn't part of this enum

}opcodeE;

//runtime

void int_run_str(char* src, char* filename);


#endif
