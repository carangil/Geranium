#ifndef INT_H
#define INT_H


#include "zmem.h"
#include "zlist.h"
#include "zstring.h"
#include "zstringmap.h"
#include "zarray.h"

#include <ffi.h>
#include <dlfcn.h>

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
		struct instructionS* subtree;
	} address;
	int offset;
}ptrT;

typedef struct valueS{
	union {
		ptrT ptr;
		FLOAT f;
		zint32 z32;
		zuint32 u32;
		zsize size;
	} as;
	//put type info here for interfaces
}valueT;


void int_add_c_object(char* name, void* proc);
void int_add_c_val32(char* name, int val);

#define INT_STRUCT_SIZE(TYPE) int_add_c_val32( "_size_" #TYPE , sizeof(TYPE))
#define INT_STRUCT_MEMBER(TYPE,MEMBER) int_add_c_val32( "_offset_" #TYPE "_" #MEMBER , offsetof(TYPE,MEMBER) )


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

void int_insert_tokenf(tokenT* A, tokenT* B, zbool before);

#define tinsert_after(AFTER,NEW)    int_insert_tokenf(  AFTER, NEW, ZFALSE)

//#define tnext(ITEM)		((tokenT*)zlist_next(ITEM))

tokenT* tnext(tokenT* item);

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
	PENDING, //type details aren't defined yet.  But you could have pointers to them, etc
    OPAQUE,
	SIMPLE, //ints, etc
	PROC,
	FRAME,	//struct or proc frame
	REFERENCE,
	STEWARD,  //owns the pointer to an object
	ARRAY,	//pointer to array
	CPOINTER, //for C interop
	VARIABLE, //not a pointer, but refers to the variable itself, ref is the contents of the variable
	ARG,
	LIKE,
	SUBTREE,
	DEREFERENCE,// usually deref just strips off the pointer wrapper, but this is for 'like' types where we have to delay doing that.  This is dereference the like type when resolving
	ITERATED //for a function arg that is passed as an array, but each invocation of the function sees the value inside the array (such as attributes on shaders)
}categoryE;

struct exectxS;

typedef struct wordS{
    //char* key;
    char* name;
    struct typeS* type; 


//	valueT data;
	//broken out seperate instead of a union so that I fault on a null pointer instead of accidently reading garbage

    struct parsectxS* target_pctx;//$ if word represents a proc or struct, these are the variables/fields /
	struct parsectxS* restrict_pctx;// if word can only be used in a particiualr context
	struct parsectxS* in_pctx; //what pctx this word is in

    //special handler during parse phase
    tokenT* (*parse) (struct parsectxS* pctx, struct wordS* word, tokenT* t, int argc);

	int opcode; //what opcode to use to call this function (can be a primitive like 'add', could call a c function, etc...)
	valueT val;
	struct typeS* val_type;
	char* comment;
	struct wordS * aliases; //find all the aliases for freeing
	struct callerS* ffi_caller; //for FFI (c functions)
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

	char** opt_argnames;  //holds name of args for a proc, optional
	int		arena_index; //which arena (0 is global heap)

    categoryE category;
    int size; 
	zbool is_wild; //true if has any wildcards
} typeT;

//'val' contains a pointer that must be freed when instriction is freed:
#define INST_FREE_VALUE 1



typedef struct instructionS{
	int opcode;
	valueT val;
	typeT* val_type;
	struct instructionS** args;
	typeT* result_type;
	int    flags;
	char* comment;
}instructionT;


#define ERROR_PARSE		0x100
#define ERROR_RUNTIME	0x200

typedef struct errorS {
	char*	error_string;
	int		error_code;
	char*	file;
	int		line;
} errorT;

//a runner is an implemenation of the language.
// SWITCHRUNNER is a switch-case bytecode interpreter

#define MAXRUNNERS		2
#define SWITCHRUNNER	0

struct runnerS;

typedef struct parsectxS{
	struct parsectxS* parent; //where to get things not found here
    zstringmapT* dictionary; //wordT*
    zstringmapT* types;      //typeT*
	zvecT* codestack;
	wordT* next_match;
	errorT err; //parse_errors
	char* comment;  //debugging name for this
//	zvecT* cleanlist;  //list of pointers that need to be freed
	void* frame;  //pointer to the frame that this parsectx is bound to. (if it s a single thing like a section.  )
	typeT* frametype;
	struct runnerS *runners[MAXRUNNERS];
	int id;//for debugging
	int size;

	zbool is_frame; //if a struct or section
	zbool is_section;

}parsectxT;


typedef struct exectxS{
	valueT*	stack;
	valueT*	locals;

	//todo typeinfo for selectors?
	int sp;
	int bp;//bp=-1 is last arg, bp=-2 is one before that...

	errorT err; //runtime errors

//	char* error_string;
//	int error_code;

}exectxT;




//instruction enums

#define op_MAX 255

//opcodes are

typedef enum {
	//abstract instructions that may or may not map to a single bytecode instruction
    op_nop=0,
	op_constant,	//load any constant value into a stack slot
	op_constantaddref,	//load any constant value into a stack slot, increasing its reference count
	op_print32,		//prints integer on stdout
	op_printptr,	//points pointer (for debugging mostly)
	op_printstr,	//prints a null-terminated string
	op_getchar,		//read 1 character from stdin
	op_printchar,	//print 1 character to stdout

	//integer arithmetic:
	op_add32,
	op_sub32,
	op_mul32,
	op_div32,
	op_mod32,


	op_and32,
	op_or32,
	op_xor32,



	op_ptrvalid,  //returns true if pointer is non-null
	op_ptrequal,  //returns true if pointers equal

	op_equal32,		//bnot -> not equal
	op_less32,		//bnot -> greater or equal
	op_greater32,	//bnot -> less or equal

	//unary:
	op_neg32,	//negative integer  (-3 -> 3)
	op_inv32,	//invert bits  ...FFFE to ...0001
	op_bnot,	//true<->false

	//get pointer to variable, struct member, or array element
	op_localvar,
	op_subvar,	//get fields from a
	op_sectionvar,
	op_arrayindex,
	op_carrayindex,
	op_arraycow, //makes a copy of array if it has more than 1 reference
	op_arrayinfo, //get size or count
	op_arraysetcount,
	op_adim,
	op_arrayindexplus,


	//load
	op_load,
	op_loadaddref,
	op_take,

	//store
	op_store,
	op_trashstore,

	//allocate
	op_dim,
	op_trash, // (drop with free)

	op_argpick,
	op_argtake,
	op_argaddref,

	//control
	op_stop,
	op_block,  //runs the instructions inside
	op_if,
	op_return,
	op_returnval,
	op_call,
	op_sys,
	op_loop,
	op_break,
	op_continue,

	op_dup, //duplicate value
	op_dups, //duplicate and add reference

	op_over,
	op_drop,

	//floating point
	op_fadd,
	op_fsub,
	op_fmul,
	op_fdiv,
	op_fpow,

	op_fictional_call, //don't really call the function.  This instruction is not ever implemented in the interpreter and produces a runtime error.  But exists to allow the concept of 'calling' something that another part of the compiler will resolve into something.

	op_LASTCORE,	//all instructions before this are part of the AST

	//these ones are implementation specific:
	op_switch_jump,
	op_switch_jumpfalse,
	//op_switch_overindexplus,
	op_FIRST_EXT,  //first instruction that isn't part of this enum (user-defined)

}opcodeE;

//runtime

//FFI


typedef struct callerS{
	char* name;

	void* dlhandle;//if ldopen'ed
	void* funcptr; //c function to call

	void* ffi_closure;
	void* ffi_closure_code; //for c to call back into interpreter
	exectxT* exe;//context to run in (set at time callback is passed as an arg

	int argc;
	ffi_cif cif;
	ffi_type* rettype;
	ffi_type* types[0];
}callerT;



void int_run_str(char* src, char* filename);


#endif
