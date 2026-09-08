#include "int.h"
#include "string.h"
#include "stdarg.h"
#include "zmem.h"
#include "zarray.h"

//disable some stuff

#undef tracef
#define tracef(...)

#define iftrace(...)
//#define iftrace(A) A

#undef debugf
#define debugf(...)

//quick hack for fast error messages
int lastline = 0;
char* lastfile ="nofile";

tokenT* tnext(tokenT*item){

	tokenT* next = ((tokenT*)zlist_next(item));
	if (next){
		lastline = next->line;
		lastfile = next->sourcefile;
	}

	return next;
}

void myExit(int a, char* cfile, int cline){

		printf("EXITING  %s:%d   %s:%d\n", lastfile, lastline, cfile, cline);
		exit(a);
}
#define exit(CODE) myExit(CODE, __FILE__, __LINE__)

//error reporting
void set_error(errorT* err, int code, char* string){

	if (!err){
		fprintf(stderr, "set_error %d: %s",code, string);
		ram_free(string);
		return;
	}

	if (err->error_string){
		fprintf(stderr, "Warning existing error %s already set\n", err->error_string);
		ram_free(err->error_string);
	}


	err->error_code = code;
	err->error_string = string;
	err->file = NULL;
	err->line = 0;
}

//adds
void errtok(errorT* err, tokenT* t){

	if(t){
		err->file = t->sourcefile;
		err->line = t->line;
	} else {
		err->file=NULL;
		err->line=0;
	}
}

int copyerr(errorT* dst, errorT* src){

	set_error(dst, src->error_code, ram_addref(src->error_string));
	dst->line = src->line;
	dst->file = src->file;

	return dst->error_code;
}

#define ERRTOK(CONTEXT,TOKEN)  errtok(&((CONTEXT)->err), TOKEN)
#define ERRUP(UPCONTEXT,DOWNCONTEXT)  copyerr( &((UPCONTEXT)->err), &((DOWNCONTEXT)->err))

int printerr(errorT* err){

	if (err->error_code){
		fprintf(stderr, "%d  %s:%d %s\n",err->error_code, err->file?err->file:"", err->line, err->error_string);
	}

	return err->error_code;
}

void reseterr(errorT* err){
	ram_free(err->error_string);
	err->error_string = NULL;
	err->error_code=0;
}

#define PRINTERR(CONTEXT)    printerr(  &( (CONTEXT)->err))
#define RESETERR(CONTEXT)    reseterr(  &( (CONTEXT)->err))

char* echo(int line, char* file, char* s){
	fprintf(stderr, " %s:%d   %s:%d SETERROR %s\n", file, line, lastfile, lastline,       s);
	//exit(1);
	return s;
}

//#define seterrorf(CONTEXT, CODE, ...)  set_error(&(CONTEXT->err), CODE, echo(zstrprintf( (CONTEXT)->err.error_string, __VA_ARGS__))   )

#define PERROR(CONTEXT, ...) set_error(&(CONTEXT->err), ERROR_PARSE, echo(__LINE__, __FILE__, zstrprintf( NULL, __VA_ARGS__)))




// for testing    if(ERR(pctx))

#define ERR(CONTEXT)	((CONTEXT)->err.error_code)
#define OK(CONTEXT)		(0==(CONTEXT)->err.error_code)


/* TOKENIZER*/
/* This first part parses an input string and returns a linked list of tokens
 * Very little is verified, things are broadly classified as pairs (such as ->, --, etc),
 * as single character tokens (+,-, etc), names (alpha_numeric_123), numbers (1.0E-4, 0x100, etc)
 * literals  "boo", whitespace, etc.
 * Things like 0x10.4E-4 is a 'NUMBER', but will later fail as it isn't a valid form
 */

//the general pattern of the next few functions return the number of characters they accept
//if they return 0, the rule is not accepted


//tokenT->tok values:
//any character (1 byte) symbol itself is just its int value
//PAIR is 2 characters, like [], !=, etc, theya re just combined into a 16-bit value
#define TOKEN_PAIR(B1,B2)   ((((unsigned int)(B1&0xff)) <<8) | ((unsigned int)(B2&0xff)))
#define TOKEN_STARTFILE	    0x0100
#define TOKEN_NAME		    0x0200
#define TOKEN_NUMBER        0x0300
#define TOKEN_LITERAL       0x0400

zbool token_cleanup(void* v){
	tokenT* t = v;
	ram_free(t->str);
	ram_free(t->sourcefile);
	//zlist_cleanup(&t->subs);
	return ZTRUE;
}

tokenT* token_mk(zuint32 tok, char* str, zuint32 len){
	tokenT* t = ram_alloc( sizeof(tokenT) , token_cleanup );
	t->tok = tok;

	//zlist_init(&t->subs);
	//printf(" TOKEN %d   %.*s\n", tok,  str? len: 5, str?str:"nostr");
	if (str && len)
		t->str = zstrndup(str, len);
	else if (str)
		t->str = zstrndup(str, ZSTRING_ALL);
	return t;

}

void int_insert_tokenf(tokenT* A, tokenT* B, zbool before) {

	if (before)
		zlist_insert_node_before(&A->zlistnode, &B->zlistnode);
	else
		zlist_insert_node_after(&A->zlistnode, &B->zlistnode);

}

//used to recognize 2-letter combinations like ->, etc
zuint32 find_pair(char* patterns, char a, char b){

	for(  ;*patterns && *(patterns+1) ; patterns+=3){
		if ( ((*patterns)==a) &&(*(patterns+1)==b))
			return TOKEN_PAIR(a,b) ;

		if (!*(patterns+2))
			break;
	}
	return 0;
}

//used to either recognize names or numbers. Simple, not regex-fancy or anything
//String must start with one of startChars, and then continues on with any number of continuePairs or continueChars
int accept_patterns(char* s, char* startChars, char* continuePairs,  char* continueChars){
	int i=0;
	zuint32 p=0;

	if (strchr(startChars, *(s++))){	//if input string 's' begins with any of the start chars
		i++; //advance to next char
		for(;*s;i++,s++){  //continue on
			if ( (p=find_pair(continuePairs,*s,*(s+1)))){ //accept any pairs of characters
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
//filename is optional

void  tokenize(tokenT* insert, char* in, char* filename, int line){

	char* instart = in;
	int c,next;
	tokenT* t=NULL;

    if (line ==0)
        line=1;

	char* fnamecopy = zstrdup(filename);

	while ((c = *in)){
		next = *(in+1);
		zuint32 p;

		//find twochar patterns like ->,etc. including comment start/end markers
		//looking for pairs before chars makes sure the matching is 'greedy'
		if ((p = find_pair("+= '[ $$ :: $[ =[ .* .& .@ .$ -- ++ == -> >- /* */ // [] >= <= != .- <> \\\\ /\\ \\/" , c, next))){
			if (  p == TOKEN_PAIR('/','/')  ) { //special handling for // comments
				while(*in!= '\n')
					in++;
				in++;//skip
				line++;
				continue;
			}

			if (p == TOKEN_PAIR('/', '*')) { //special handling for /* comments
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

			t = token_mk(TOKEN_PAIR(c, next) , in, 2);
			t->line = line;
			t->sourcefile = ram_addref(fnamecopy);

			tinsert_after(insert,t);
			insert = t;
			in+=2;
			continue;
		}

		//check for string literals
		int lit = acceptLiteral(in, '"' , '\\' ); //double quote

		if (lit) {
			t = token_mk(TOKEN_LITERAL, in, lit);
			t->line = line;
			t->sourcefile = ram_addref(fnamecopy);
			tinsert_after(insert,t);
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
		int space = accept_patterns(in, " \t\n\r", "", " \t\n\r");

		if (space) {
			while (space) {
				if (*in == '\n')
					line++;
				space--;
				in++;
			}

			//below will turn whitespace into a token.  Currently, whitespace is ignored, so just skip the token
#if 0
			t = token_mk(' ');
			t->str=zstrdup(" ");
			zlist_addtail(list, &t->zlistnode);
#endif
			continue;
		}

		//names can start with alpha _ or dot(struct member reference)
		int name = accept_patterns(in,
				"_.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",  //start with ._alpha
				"",
				"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
/*
		if (name && in[name] == '?') {
			name++; //accept names that end with ?
		}

		if (name && in[name] == '=') {
			name++; //accept names that end with =
		}
*/

		int digits  = 0;

		if (!name || (*in == '.') ){
			digits  = accept_patterns(in,
					"-0123456789", //start with digit or decimal point
					"e- E- e+ E+",  //- and + only accepted after an e or E
					"0123456789.eE"); //continues with digits, decimal point, hex letters, type suffix letters

			//special case: if number starts with '-', but has only 1 character, this isn't a negative number, but just a minus sign
			if ((digits == 1) && (in[0] == '-'))
				digits = 0;

			//special case: numbers should not end in '.'
			if ( in[digits-1] == '.' ){
				digits--;
			}

            //special case: Hex starts with 0x

            if ( (*in) == '0' && *(in+1) == 'x'){
				int hexdigits = 2 + accept_patterns(in+2, "0123456789abcdefABCDEF", "", "0123456789abcdefABCDEF");
                if (hexdigits > digits)
                        digits= hexdigits;
            }
		}

		if (name> digits)
			digits=0; //accept name if it is longer

		if (digits || name){

			if (!strncmp(in, "sys_ENDFILE", 11))
				break;

			t = token_mk( digits? TOKEN_NUMBER : TOKEN_NAME, in ,   digits|name);
			t->line = line;
			t->sourcefile = ram_addref(fnamecopy);
			in += digits|name;
			tinsert_after(insert,t);
			insert = t;
			continue;
		}

		//just some char
		t = token_mk( *in, in, 1);
		t->line = line;
		t->sourcefile = ram_addref(fnamecopy);

		tinsert_after(insert,t);
		insert = t;


		in++;
	}
	ram_free(fnamecopy);
}


//execution context
zbool clean_exe(void* v){
	exectxT* exe = v;
	ram_free(exe->err.error_string);
	ram_free(exe->stack);
	return ZTRUE;
}

exectxT* exectx_mk(){
	exectxT* exe = ram_alloc(sizeof(exectxT), clean_exe);
	exe->stack = ram_alloc(sizeof (valueT) * STACK_SIZE, NULL);
	return exe;
}



//Dictionary
zbool word_clean(void* v){
	wordT* w = v;
	ram_free(w->name);
	ram_free(w->type);
	ram_free(w->target_pctx);

	ram_free(w->comment);
	ram_free(w->aliases);
	ram_free(w->ffi_caller);


	return ZTRUE;
}


tokenT* parse_default(parsectxT* pctx, wordT* w, tokenT* t, int argc);

//creates a word.  If owntype no reference is added to type, so that freeing the word frees the type.

wordT* word_mk(parsectxT* pctx, char* name, typeT* type){

	wordT* w = ram_alloc(sizeof(wordT), word_clean);
	
	w->parse = parse_default;

	w->name = zstrdup(name); //note: null string will give a valid empty string non-null result

	w->type = ram_addref(type);
	
	if (pctx){
		w->in_pctx = pctx;
		//add to the dictionary
		zvecT* p =  zstringmap_get(pctx->dictionary, name);
		if (!p){
				p = zvec_mk(NULL, 10);	//make new vector
				zstringmap_put(pctx->dictionary, name, p);
		}

		//before adding, make sure there isn't another word with the same name (0 args) or same proc type
		for (int i=0;i<zvec_count(p); i++){

			wordT* cw = zvec_get_at(p, i);

			//if both same type (proc) then also error
			//todo:might have more  conflicts, not known yet
			if ((cw->type->category == PROC) && !strcmp(cw->type->key, w->type->key)){ //todo use cmp_type with 'no wildcards/subst'

				if(!cw->target_pctx && ! cw->ffi_caller){
					tracef(" Taking previous prototype as the word to use %s %s\n", w->name, cw->type->key);
					ram_free(w);
					return cw;
				}


				PERROR(pctx, "Creating second word with same args in same pctx\n");
				ram_free(w);
				return NULL;

			}

			if (cw->type->argc == 0 && w->type->argc ==0){
				PERROR(pctx, "Creating second word with no args in same pctx\n");
				ram_free(w);
				return NULL;
			}

		}


		zvec_add(p, w);

		if (type->category == VARIABLE){


			//todo: deal with alignment
			w->offset = pctx->size;
			tracef(" made word %s  %s  %d\n", name, type->key, type->ref->size);
			pctx->size += type->ref->size;

			if (type->ref->category == STEWARD){  //todo finish this
				tracef(" Adding CLEAN for word %s at offset %d\n", w->name, w->offset);
				//getc(stdin);
				//zvec_add(pctx->cleanlist, w);
			}

			//exit(1);
		}


	}else{
		PERROR(pctx, "no pctx\n");
		return NULL;

	}

	return w;
}


wordT* word_alias_mk(char* name, typeT* newtype, wordT* original){
	wordT* alias = ram_alloc(sizeof(wordT), word_clean);


	alias->target_pctx = ram_addref( original->target_pctx);
	alias->type = ram_addref(newtype);
	alias->name = zstrprintf(NULL, "{%s %s as %s}\n", name, original->type->key, newtype->key);
	alias->val = original->val;
	alias->val_type = original->val_type;
	alias->offset = original->offset;
	alias->parse = original->parse;
	alias->opcode = original->opcode;
	alias->autoload = original->autoload;
	alias->aliases = original->aliases; //take original's chain of aliases
	original->aliases = alias; //head of original's alias chain

	return alias;
}



void dump_dictionary(parsectxT* pctx){
	char*	name=NULL;
	zvecT* 	words = NULL;
	zstringmap_cursorT	cursor=0;
	int i;
void* p =0;
	while (zstringmap_nextkey(pctx->dictionary, &name, &words, &cursor)) {
		printf("%s:\n", name);
		for (i=0;i<zvec_count(words); i++){
			wordT* word = zvec_get_at(words,i);
			printf("\t%s\t%s\n", word->name, word->type?word->type->key:"notype");
		}
	}
}

//data types

void type_print(typeT* t){
	if (!t)
		return;

	switch (t->category){
		case PENDING:
			printf("pending ");
			break;
	}
	printf("%s ", t->key);
	
	if (t->opt_argnames) {
		for (int i=0;i<t->argc;i++){
				printf(":%s ", t->opt_argnames[i]);
		}
	}

	if (t->word) {
		printf("{by word %s}", t->word->name);
		/*if (t->word->target_pctx){
			printf(" subdictionary:\n");
			dump_dictionary(t->word->target_pctx);
		}
		*/
	}
		//printf("\n");
}

void dump_types(parsectxT* pctx){
	char*	name=NULL;
	typeT* 	type = NULL;
	zstringmap_cursorT	cursor=0;
	printf(" types:\n");
	while (zstringmap_nextkey(pctx->types, &name, &type, &cursor)) {

		type_print(type);
		printf("\n");

	}
}

//most basic types
typeT *tZ32, *tType, *tReal, *tType, *tFloat, *tBit, *tWord, *tString, *tByte, *tStringByte, *tSize32, *tLong32, *tDouble32, *tZL16,  *tUL16;

//some special types
typeT* tAny; //matches any type

/*
			=:( any:val   val typeof storable: loc);  //stores 'val' into 'loc' if 'loc' is a variable that stores type val

 */



//Matching flags for comparing types or words
//when finding words, allow going into parent contexts
#define MATCH_RECURSE_PCTX	0x0001



//allow LIKE type matching
#define MATCH_ALLOW_LIKE	0x0002




typeT* type_find(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args);

int rcount = 0;

typeT* resolve_like_type_for_caller(parsectxT* pctx, typeT* expected, typeT* proctype, typeT** stacktypes){

	if (rcount > 5){
		printf(" excessively recursive debugme\n");
		getc(stdin);
	}


	if (!expected->is_wild){
		return expected;
	}

	if (expected->category == LIKE){
		int argn = expected->argc;	//expected is 'like' the argnth term

		//the proc's nth type
		typeT* proc_n = proctype->argtypes[ expected->argc]; //what type the proc expects for args

		//the stack's nth type
		typeT* passed_n = stacktypes[expected->argc];



		tracef(" %s is %dth, is %s\n", expected->key, expected->argc, passed_n->key);

		//getc(stdin);
		return passed_n;

	}

	tracef(" case of %s\n", expected->key);
	//not a like, but is some other wild
	if (expected->ref){

		tracef(" rcount++\n");
		rcount++;
		typeT* inner = resolve_like_type_for_caller(pctx, expected->ref, proctype, stacktypes);
		rcount--;
		tracef(" rcount--\n");
		tracef(" got inner type %s for %s  want category %d\n", inner->key, expected->key, expected->category);



		typeT* newexpected = type_find(pctx, NULL, inner, expected->category,0 , NULL);
		tracef("  inner type %s  re-wrapped as %s\n", inner->key, newexpected->key);

		return newexpected;


	}


	errorf("BAD LIKETYPE CASE\n");
	exit(1);


	return expected;
}



//Compare two types to see if they are compatible/equivalent

zbool type_cmp(parsectxT* pctx, typeT* expected, typeT* given, int flags, typeT* proctype, typeT** stackargs){

	if (!expected && !given)
		return ZTRUE; //both having no type is the same

	if (!expected || !given) //if one is still not present, they are not the same
		return ZFALSE;




	//at this point both nulls are non-null

	if (expected->is_wild ){

		if (!(flags&MATCH_ALLOW_LIKE)){
			return ZFALSE;
		}

		if (proctype && stackargs){
			typeT* resolved = resolve_like_type_for_caller(pctx, expected, proctype, stackargs);
			debugf(" %s resolved to %s\n", expected->key, resolved->key);
			expected = resolved;
		} else {
			errorf("Can't use like type here\n");
			exit(1);
		}

	}



	if (expected == given)
		return ZTRUE;

	if (expected->category == PROC && given->category == PROC){
		//printf("Need to check types %s %p vs %s %p\n", expected->key, expected, given->key, given);

		//for now just check the string key
		if (!strcmp(expected->key, given->key))
			return ZTRUE;


	}

	if (expected->ref && given->ref && expected->category == given->category){
		return type_cmp(pctx, expected->ref, given->ref, flags, proctype, stackargs);
	}


	//special cases




	if ((expected == tSize32) && (given == tZ32)) {
		//Size32 type allows passing a Z32 to a C function that wants a size_t
		//This rule considers it identical to Z32 for all other uses
		return ZTRUE;
	}

	if ((expected == tLong32) && (given ==tZ32)) {
		//Size32 type allows passing a Z32 to a C function that wants a long, whatever size it is on that system
		//This rule considers it identical to Z32 for all other uses
		return ZTRUE;
	}

	if ((expected == tDouble32) && (given ==tReal)) {
		//for C functions that need double.
		return ZTRUE;
	}

	if ((expected == tZL16) && (given ==tZ32)) {
		//for C functions that need shorts.
		return ZTRUE;
	}

	if ((expected == tUL16) && (given ==tZ32)) {
		//for C functions that need shorts.
		return ZTRUE;
	}

	//iterated calls:
	//The procedure is really called with an array, but the function is defined as taking an iterated array.
	//what that does in this case, outside the function, we match arrays
	//later, when inside the function, taking a value will be an element of the array.
	if (expected->category == ITERATED) {
		return type_cmp(pctx, expected->ref, given, flags, proctype, stackargs);
	}



	if (expected==tAny && (flags&MATCH_ALLOW_LIKE)) //tAny matching anything
		return ZTRUE;

	return ZFALSE;

}

zbool type_equal( typeT* expected, typeT* given){
	//stricter than type_cmp
	if (expected == given)
		return ZTRUE;

	if ((expected->category == given->category) && (expected->ref) && (given->ref)){
		return type_equal(expected->ref, given->ref);
	}
	
	//go by string name 'key'
	if (!strcmp(expected->key, given->key))
		return ZTRUE;

	return ZFALSE;

}

void indent(int n){
		for (int i=0;i<n;i++)
			printf("\t");
}

//instruction set
char** instruction_names = NULL;
void instruction_print(instructionT* inst, int level, zbool recurse){


	if (!inst)  {
		printf("(null code)\n");
		return;
	}

	//print args
	if (recurse && inst->args){
		printf("\n");
		indent(level); printf("{\n");
		for (int i=0; i<zarray_count(inst->args); i++){
			instruction_print(inst->args[i], level+1, recurse);
		}
		indent(level); printf("}\n");
	}



	indent(level);
	char* name = NULL;
	if (inst->opcode < op_MAX)
		name = instruction_names[inst->opcode];

	if (name)
		printf("- %s ", name);
	else
		printf("unnamed(%d) ", inst->opcode);


	if (inst->val_type) {
		if( inst->val_type == tType)
			printf( " %s:(%s) ", inst->val.as.ptr.address.type ?inst->val.as.ptr.address.type->key:"nil", inst->val_type->key);
		else if( inst->val_type == tWord)
			printf( " %s:(%s) ", inst->val.as.ptr.address.word->name, inst->val_type->key);
		else
			printf( " %x:(%s) ", inst->val.as.z32, inst->val_type->key);
	} else {
		printf( "(noimm)%d", inst->val.as.z32);
	}

	if (inst->result_type)
		printf("->%s", inst->result_type->key);

	if (inst->comment)
		printf("  //%s", inst->comment);




	printf("\n");


}

void instructions_print(instructionT** insts, int level, zbool recurse){

	if (!insts)
		return ;

	for (int i=0; i < zarray_count(insts); i++){
		instructionT* inst = insts[i];

		if (!inst)
			printf("(null code)\n");
		else
			instruction_print(inst, level, recurse);
	}

}

void codestack_print(zvecT* cs,  int start, zbool recurse){

	if (!cs)
		return ;
	printf(" --- TREE ---\n");
	for (int i=start; i < zvec_count(cs); i++){
		instructionT* inst = zvec_get_at(cs,i);

		if (!inst)
			printf("(null code)\n");
		else
			instruction_print(inst, 1, recurse);


	}
	printf(" --- END  ---\n");

}
//pops a list of instructions off the stack
instructionT* pop_arg(parsectxT* pctx){
	return zvec_remove_last(pctx->codestack);
}

instructionT** pop_args(parsectxT* pctx, int n){
	if (n==0)
		return NULL;

	//pop right to left
	//instructionT** a = zarray_alloc(instructionT*, n);
	instructionT** a = zarray_allocd(instructionT*, n, zarray_destruct_pointers);
	for (int i=n-1; i >=0;i--){
		a[i] = pop_arg(pctx);
	}

	zarray_use(a,n);

	return a;
}

/*
typeT** inst_result_types(instructionT** a){

	typeT** types = zarray_alloc(typeT*, zarray_count(a) );
	for (int i=0;i<zarray_count(a);i++){
		types[i] = a[i]->result_type;
	}
	zarray_use(types, zarray_count(a));
	return types;
}
*/




//for an array of instructionT*, return an array of the types of the constants they varry
typeT** inst_const_types(instructionT** a){

	if (!a)
		return NULL;

	typeT** types = zarray_allocd(typeT*, zarray_count(a), zarray_destruct_pointers );
	for (int i=0;i<zarray_count(a);i++){
		if (a[i]->result_type == tType){
			types[i] = ram_addref(a[i]->val.as.ptr.address.type);
			if (types[i] == NULL){
				errorf(" Invalid type in list\n");
				exit(1);
			}
		}

	}
	zarray_use(types, zarray_count(a));
	return types;
}

int codestack_depth(parsectxT* pctx){
	return zvec_count(pctx->codestack);
}

instructionT** assembly_args_mk(int n, ...){


	va_list args;
	instructionT** a = zarray_allocd( instructionT*, n, zarray_destruct_pointers);
	tracef("%p  %d  args ... \n\n\n",a, n);

	if (n){

		va_start(args, n);
		for (int i=0;i<n;i++){
			a[i] = va_arg(args, instructionT*);	//get next arg
		}
		va_end(args);
		zarray_use(a,n);
	}


	return a;
}

zbool clean_inst(void* v){

	instructionT* inst = v;
	//printf(" FREE INST ");	instruction_print(inst, 1, ZFALSE);

	int i;
	/*
	if(inst->args){
		for (i=0;i<zarray_count(inst->args); i++){
			ram_free(inst->args[i]);
		}
		ram_free(inst->args);
	}*/
	ram_free(inst->args);
	ram_free(inst->comment);

	if (inst->flags & INST_FREE_VALUE){
		tracef(" FREE INST VAL  %p on inst %p\n", inst->val.as.ptr.address.bytes, inst);

		ram_free(inst->val.as.ptr.address.bytes);
	}


	return ZTRUE;
}

//returns an array item representing one instruction.
int push_assembly_line=0;
instructionT* push_assembly(parsectxT* pctx, int opcode, int flags, valueT* pval, typeT* val_type,  instructionT** args , typeT* ret){

	if (!pctx->codestack){
		pctx->codestack = zvec_mk(NULL, 8);
	}

	instructionT* inst = ram_alloc(sizeof(instructionT), clean_inst); push_assembly_line = __LINE__;


	inst->opcode=opcode;
	inst->result_type =ret;
	inst->flags = flags;

	if (pval){
		inst->val= *pval;
		inst->val_type = val_type;
	}


	///printf(" args have %d\n", args?zarray_count(args):-9999);
	inst->args = args;

	//push it onto the codestack
	zvec_add(pctx->codestack, inst);

	return inst;
}

instructionT* push_subtree(parsectxT* pctx, instructionT* inst){

	if (!pctx->codestack){
		pctx->codestack = zvec_mk(NULL, 8);
	}
	zvec_add(pctx->codestack, inst);
	return inst;
}


//t is the token trying to be matched
//tprev(t) is the last arg to it, if there are any


#define DEBUG_MATCH 0
//tests of a word matches the current typestack of pctx

zbool test_word(parsectxT* pctx, typeT** stacktypes, wordT* word, int flags, int argc){


	if (argc != word->type->argc){ 
		debugf("Skipping mismatch arg count %d %d\n", argc, word->type->argc);
		return ZFALSE;
	}

	if (DEBUG_MATCH){
		printf("test %s %s: to ", word->name, word->type->key);
		for (int i=0;i<argc;i++){
			printf("%s ,", stacktypes[i]? stacktypes[i]->key:"novalue ");
		}
		printf(".\n");
	}


	//argless words	
	if (argc==0 && word->type->argc==0){
		tracef("Word %s has no members, name matches, returning it\n", word->name);
		return ZTRUE;
	}

	//try to match args	
	void* csr=NULL;
	for (int i=0;i<argc;i++){

		typeT* passed_type = stacktypes[i];

		if (!passed_type){
				//errorf(" No result type! on arg %d/%d of %s %s\n", i, argc, word->name , word->type->key);
				//I don't think this is an error, this is just testing a possible match and we don't have a value on the stack
				return ZFALSE;
		}


		typeT* argtype = word->type->argtypes[i];
		tracef("Compare sp%d  %s s %s\n", i, argtype->key,  passed_type->key);



		if (!type_cmp( pctx, argtype, passed_type, flags, word->type, stacktypes)){
			tracef("flags %x arg %d    %p %s vs %p %s \n", flags, i, argtype, argtype->key, passed_type, passed_type->key);
		//	printf("bas compare\n");
			return ZFALSE;
		}

	} //argc

	return ZTRUE; //didn't not match, so I guess it did.
}

//runner interface that could be replaced with a different implementation
typedef struct runnerS{
	void (*execute) (exectxT* exe, struct runnerS* execfunc, int start);
}runnerI;

//Simple switch-statement based runner
//this takes the codestack tree and creates a flat bytecode from it


typedef struct switchops{
	int opcode;		//opcode
	typeT* immtype; //type of the immediate value
	valueT imm;	    //any immediate value


	instructionT* source;
}switchopT;

typedef struct switchrunnerS{
	runnerI runner;
	switchopT* prog;
	parsectxT* pctx; //todo: remove this, copy the necessary data into here
	//int local_size;

	int toexit;
	int tostep;

} switchrunnerT;
runnerI* compile_for_switch(exectxT* exe, parsectxT* pctx);


switchopT* switch_asm(switchopT* prog , int opcode, typeT* immtype, valueT* pimm,  instructionT* sourceinst){
	switchopT op = {0};
	op.opcode = opcode;
	if (pimm){
		op.imm = *pimm;
	}
	op.immtype = immtype;
	op.source=sourceinst;

	prog = zarray_more(prog, 1, NULL);
	prog = zarray_append(prog, op);
	return prog;
}

switchopT* switch_asmi(switchopT* prog , int opcode, int i,  instructionT* sourceinst){
	valueT v = {0};
	v.as.z32 = i;
	return switch_asm(prog, opcode, tZ32, &v, sourceinst);

}


void* alloc_frame(parsectxT* pctx);

switchopT* compile_switch_subtree(exectxT* exe, switchrunnerT* sw, switchopT* prog, instructionT* inst){

	zbool skipargs = ZFALSE;

	if (inst->opcode == op_constant  /*|| inst->opcode == op_nop*/ ){
		skipargs = ZTRUE;
	}

	int argc = inst->args? zarray_count(inst->args) : 0 ;

	if (inst->opcode == op_if){
		//if/else chains alternate args as condition, code, condition, code, etc

		int previous_cond_jump = 0;
		int* fixes = zarray_alloc(int, zarray_count(inst->args));


		for(int i=0;i< zarray_count(inst->args);i+=2){
			//compile condition

			prog = compile_switch_subtree(exe, sw, prog, inst->args[i]);

			//compile jump skipping if false
			previous_cond_jump = zarray_count(prog);  //next instruction needs to be fixed
			prog = switch_asmi(prog, op_switch_jumpfalse, -1, inst); //jumpfalse


			//compile code for true condition
			prog = compile_switch_subtree(exe, sw, prog, inst->args[i+1]);

			//jump to end of block
			zarray_append(fixes,  zarray_count(prog)); //next instruction needs to be fixed
			prog = switch_asmi(prog, op_switch_jump, -2, inst);

			prog[previous_cond_jump].imm.as.z32 = zarray_count(prog); //make the cond jump over true code

		}

		int end = zarray_count(prog);

		for (int i=0; i<zarray_count(fixes);i++){
			prog[fixes[i]].imm.as.z32 = end;
		}
		ram_free(fixes);

		return prog;
	}


	if (inst->opcode == op_loop){

		/*  goto startloop
		 *  goto end
		 *  goto step
		 *  startloop:
		 *  body
		 *
		 *  step:
		 *  step
		 *
		 *  end:
		 * 	endloop
		 *
		 *
		 */

		prog = switch_asmi(prog, op_switch_jump, zarray_count(prog)+3, inst); //skip over these statements

		int oldstep = sw->tostep;
		int oldexit = sw->toexit;

		sw->tostep = zarray_count(prog);
		prog = switch_asmi(prog, op_switch_jump, -1 , inst);

		sw->toexit = zarray_count(prog);
		prog = switch_asmi(prog, op_switch_jump, -2 , inst);

		//first arg is loop body
		//second arg is loop step (if present)
		int looptop = zarray_count(prog);

		prog = compile_switch_subtree(exe, sw, prog, inst->args[0]);



		int stepstart = zarray_count(prog);
		if (zarray_count(inst->args) == 2){
			prog = compile_switch_subtree(exe, sw, prog, inst->args[1]);
		}

		prog = switch_asmi(prog, op_switch_jump, looptop, inst); //jump to top of loop

		//fix up some values
		prog[sw->tostep].imm.as.z32 = stepstart;
		prog[sw->toexit].imm.as.z32 = zarray_count(prog);

		//restore those values (in case of nested loop)
		sw->tostep = oldstep;
		sw->toexit = oldexit;




		return prog;
	}

	if (inst->opcode == op_break){
		prog = switch_asmi(prog, op_switch_jump, sw->toexit, inst);
		return prog;
	}

	if (inst->opcode == op_continue){
		prog = switch_asmi(prog, op_switch_jump, sw->tostep, inst);
		return prog;
	}


	if (inst->args && !skipargs){

		for (int i=0;i<zarray_count(inst->args);i++){
			prog = compile_switch_subtree(exe, sw, prog, inst->args[i]);
		}
	}

	if (inst->opcode == op_block) { //blocks in the tree just organize code; that code has already been assembed
		return prog;
	}

	if (inst->opcode== op_dim){ //allocate an array, given  pointer to array var and count
		valueT v={0};

		v.as.ptr.address.type=inst->args[0]->result_type->ref->ref;
	//	printf(" compiling DIM for %s\b", v.as.ptr.address.type->key);
	//	getc(stdin);
		prog = switch_asm(prog,inst->opcode, tType, &v, inst);
		return prog;
	}


	if (inst->opcode== op_indexplusstore ){
				//put size in
		int elemsize = inst->args[1]->result_type->size;
		prog = switch_asmi(prog,op_switch_indexplus,  elemsize, inst);
		prog = switch_asmi(prog,op_store,  elemsize, inst);
		return prog;
	}

	if (inst->opcode== op_arrayindex  || inst->opcode== op_carrayindex){
		//put size in
		prog = switch_asmi(prog,inst->opcode,  inst->result_type->ref->size, inst);
		return prog;
	}

	/*
	if (inst->opcode == op_arrayinfo && inst->val.as.z32 == 2 && inst->args[0]->result_type->ref->size){
		//want the size of the array element.  this is a constant
		//if the size is zero, then this becomes a dynamic call
		prog = switch_asmi(prog, op_constant,inst->args[0]->result_type->ref->size , inst);
		return prog;
	}
	*/

	if ((inst->opcode==op_subvar)||(inst->opcode==op_localvar)){
		wordT* var = inst->val.as.ptr.address.word;
		prog = switch_asmi(prog, inst->opcode,var->offset, inst);
		return prog;
	}

	if ((inst->opcode==op_load)||(inst->opcode==op_loadaddref)||(inst->opcode==op_take)){
		tracef("compile special load %d bytes from %s\n", inst->result_type->size,inst->result_type->key );
		//printf(" XXX  %s  %d\n",inst->args[0]->result_type->ref )
		prog = switch_asmi(prog, inst->opcode, inst->args[0]->result_type->ref->size, inst);  //use pointer type to find size
		return prog;
	}

	if (inst->opcode==op_store || inst->opcode == op_trashstore){
		tracef("compile store %d bytes\n", inst->args[1]->result_type->ref->size);
		prog = switch_asmi(prog, inst->opcode, inst->args[1]->result_type->ref->size, inst);
		return prog;
	}


	if (inst->opcode == op_sectionvar){

		if(inst->val.as.ptr.address.word->in_pctx->frame == NULL){
			//need to allocate
			inst->val.as.ptr.address.word->in_pctx->frame = alloc_frame(inst->val.as.ptr.address.word->in_pctx);
		}

		valueT v={0};

		debugf("sectionVAR %s frame %p at %d\n", inst->val.as.ptr.address.word->name, inst->val.as.ptr.address.word->in_pctx->frame, inst->val.as.ptr.address.word->offset );

		v.as.ptr.offset = inst->val.as.ptr.address.word->offset;
		v.as.ptr.address.block = inst->val.as.ptr.address.word->in_pctx->frame;

		prog = switch_asm(prog, op_constant,  inst->result_type  , &v, inst);
		return prog;
	}


	if (inst->val_type  == tWord){
		//need to compile the func being pointed to

		if (inst->val_type == tWord){

			wordT* word = inst->val.as.ptr.address.word;


			if (word->target_pctx){
				tracef("To compile switch:%s:%s\n", word->name, word->type->key);
				compile_for_switch(exe, word->target_pctx);
			} else if (word->opcode == op_call){
				errorf("op_call function that has no body\n");
				exit(1);
				return NULL;
			}else {
				tracef("skip compile switch:%s:%s\n", word->name, word->type->key);
			}

		} else{
			errorf("bad op_call \n");
			exit(1);
		}

	}

	prog = switch_asm(prog,inst->opcode, inst->val_type, &inst->val, inst);

	return prog;
}


void print_switch_listing(switchopT* prog){

	for (int i=0;i<zarray_count(prog);i++){

		char* name = instruction_names[ prog[i].opcode < op_MAX? prog[i].opcode:0];

		if (name)
			printf("%d:%s\t", i, name);
		else
			printf("op_%d\t", prog[i].opcode);

		if (prog[i].immtype){

			if (prog[i].immtype == tType && prog[i].imm.as.ptr.address.type)
				printf("%s", prog[i].imm.as.ptr.address.type->key);

			else if (prog[i].immtype == tWord && prog[i].imm.as.ptr.address.word )
				printf("(%s %s)", prog[i].imm.as.ptr.address.word->name, prog[i].imm.as.ptr.address.word->type->key);

			else  if (prog[i].immtype == tString)
				printf(" '%s'", prog[i].imm.as.ptr.address.bytes);
			else if (prog[i].immtype ->category != SIMPLE)
				printf("%s %p+%d",prog[i].immtype->key, prog[i].imm.as.ptr.address.block,prog[i].imm.as.ptr.offset );
			else
			printf(":%s", prog[i].immtype->key);

		}

		printf("\n");

	}

}

//void exec_func(exectxT* exe, switchrunnerT* sw



void clean_by_list(void* v, parsectxT* pctx){
	tracef(" clean by list for %s\n", pctx->comment);


	char* bytes = v;

	char*	name=NULL;
	zvecT* 	words = NULL;
	zstringmap_cursorT	cursor=0;
	int i;

	while (zstringmap_nextkey(pctx->dictionary, &name, &words, &cursor)) {
		for (i=0;i<zvec_count(words); i++){
			wordT* word = zvec_get_at(words,i);
			if (word->type->category  == VARIABLE){

				//free possessive pointers held by this struct
				if (word->type->ref->category == STEWARD){
					tracef("TO CLEAN$ \t%s\t%s\n", word->name, word->type?word->type->key:"notype");
					ptrT* p = (void*) (bytes + word->offset);
					ram_free(p->address.bytes);
				}

				if (word->type->ref->category == FRAME){
					//substruct
					tracef("TO CLEAN subvars \t%s\t%s\n", word->name, word->type?word->type->key:"notype");
					tracef(" starts at offset %d\n", word->offset);

					clean_by_list(bytes + word->offset, word->type->ref->word->target_pctx);
				}


			}
		}
	}




}




//called by ram_free
zbool clean_struct(void* v){
	tracef(" clean struct %p\n", v);
	//getc(stdin);

	parsectxT** pcs = ram_shadow(v);
	parsectxT* pc = *pcs;
	clean_by_list(v, pc);

	return ZTRUE;
}

zbool clean_array(void* v){
	typeT* t = zarray_get_meta(v);  //should be [innertype]

	//t=t->ref;  //innertype

	tracef(" CLEAN ARRAY %p %s\n", v, t? t->key: "none");

	if (t->ref->category == FRAME){
		//iterate on all spaces, cleaning
		for (int i=0;i<zarray_count(v);i++){
			tracef(" item %d  size %d\n", i, t->ref->size);
			void* item = ((char*)v) + i* t->ref->size;
			clean_by_list(item, t->ref->word->target_pctx);
		}
	} else if  (t->ref->category == STEWARD){

		if (t->ref->size == sizeof(ptrT)){
			for (int i=0;i<zarray_count(v);i++){
			//	printf("regular case for %d\n", t->ref->size);
				ptrT* item = ((ptrT*)v) + i;
				ram_free(item->address.block);

			}

		}

		if (t->ref->size == sizeof(void*)) {
				//printf("handled case for %d\n", t->ref->size);
				void **items = (void**) v;
				for (int i=0;i<zarray_count(v);i++){
			//		printf("regular case for %d\n", t->ref->size);
					ram_free(items[i]);
				}
		}
	}

	return ZTRUE;
}

void do_ffi_call(exectxT* exe, wordT* word){

	ffi_arg retu;
	ffi_sarg rets;
	void* retp;

	void* pargs[20];
	if (word->type->argc > 20){
		errorf(" 20 parameters not enough?\n");
		exit(1);
	}

	int i;
	ffi_type** types = word->ffi_caller->types;
	int argc = word->type->argc;
	for (i=0; i< argc;i++){

		if (word->type->argtypes[i]->category == PROC){

			//set exe context to here
			exe->stack[exe->sp - argc + i ].as.ptr.address.word->ffi_caller->exe = ram_addref(exe);

			pargs[i]=  &( exe->stack[exe->sp - argc + i ].as.ptr.address.word->ffi_caller->ffi_closure_code);

		} else if (word->type->argtypes[i] == tSize32) {
			//need to sign-extend because size_t is unsigned
			//printf(" SIGN EXTEND %zu " ,exe->stack[exe->sp - argc + i ].as.size );
			exe->stack[exe->sp - argc + i ].as.size = exe->stack[exe->sp - argc + i ].as.z32;
			//printf(" SIGN EXTENDED %zu \n" ,exe->stack[exe->sp - argc + i ].as.size );
			pargs[i] = &(exe->stack[exe->sp - argc + i ].as);

		} else if (types[i] == &ffi_type_pointer){
			//pointers are special, because the interpreter keeps a base pointer + offset
			//C expects a single pointer.  So it's folded fold into a single C pointer
			exe->stack[exe->sp - argc +i].as.ptr.address.bytes += exe->stack[exe->sp - argc +i].as.ptr.offset;

			//pars is POINTER to the value being passed, which is the folded pointer on the stack
			pargs[i] = &( exe->stack[exe->sp - argc + i ].as.ptr.address.bytes);

		//	printf("\n-%d----- %s\n", i, exe->stack[exe->sp - argc + i ].as.ptr.address.bytes);


		} else {
			//floats, ints, chars, anything else is just in this 'as' union, ready to go

			pargs[i] = &(exe->stack[exe->sp - argc + i ].as);
		}
	}

	exe->sp-= argc;

		//now do the call

	//printf(" to call %s %p\n", word->name, word->ffi_caller->funcptr);

	if (word->ffi_caller->rettype == &ffi_type_void){

		ffi_call(&word->ffi_caller->cif, word->ffi_caller->funcptr , NULL, &pargs[0]);

	} else if (word->ffi_caller->rettype == &ffi_type_pointer){
		void* rv = NULL;
		ffi_call(&word->ffi_caller->cif, word->ffi_caller->funcptr , &rv, &pargs[0]);
		exe->stack[exe->sp].as.ptr.address.block = rv;
		exe->stack[exe->sp].as.ptr.offset = 0;
		exe->sp++;
	} else if (word->ffi_caller->rettype->size <= sizeof(ffi_sarg)){
		ffi_arg rv;
		ffi_call(&word->ffi_caller->cif, word->ffi_caller->funcptr , &rv, &pargs[0]);
		exe->stack[exe->sp].as.z32 = (int)rv;
		exe->sp++;
	} else {
		errorf("unhandled ffi return case %p \n", word->ffi_caller->rettype);
	}

	//getc(stdin);
}


#define TRACE_STACK 0


void do_call(exectxT* exe, wordT* word){

	typeT* proctype = word->type;
	if (proctype->category == REFERENCE)
		proctype = proctype->ref;

	int argc = proctype->argc;

	//printf(" to exec %s %s with %d args\n", word->name, word->type->key, argc);
	switchrunnerT* callee = (switchrunnerT*) word->target_pctx->runners[SWITCHRUNNER];
//	print_switch_listing(callee->prog);

	int prebp = exe->bp;	//bp-1 is last arg. bp-2 is one before that.  bp is first empty val on stack
	exe->bp = exe->sp;

	callee->runner.execute(exe, &callee->runner, 0);

	//free any possessive pointers passed that didn't get taken
	if (proctype->argtypes){
		for (int i=0;i< zarray_count(proctype->argtypes);i++){

			if (proctype->argtypes[i]->category == STEWARD){
				tracef(" NEED TO FREE %.8s %s\n",  exe->stack[ exe->bp-argc+i].as.ptr.address.bytes  ,proctype->argtypes[i]->key);
				ram_free( exe->stack[ exe->bp-argc+i].as.ptr.address.bytes);
			}

		}
	}


	if (proctype->ref){ //returns value
		tracef(" returns value\n");
		exe->stack[exe->bp-argc] = exe->stack[exe->sp-1];  //last value on stack is copied to bp position
		exe->sp = exe->bp-argc+1;
	} else {
		exe->sp = exe->bp-argc; //CHECK THIS
	}

	exe->bp = prebp; //restore stack bp

}

void* alloc_frame(parsectxT* pctx){

	void* frame = ram_alloc_shadow(pctx->size, clean_struct, sizeof(parsectxT*));
	parsectxT** topctx = ram_shadow(frame);
	if (topctx)
		*topctx = pctx;

	if (pctx->size > 0)
		tracef(" Allocating %d for frame %d %p %p\n", pctx->size, pctx->id, *topctx, pctx);


	return frame;
}

void run_switch(exectxT* exe, runnerI* r, int start){

	switchrunnerT* sw = (switchrunnerT*) r;
	switchopT* pc = sw->prog + start;
	void* old_locals = exe->locals;

	exe->locals = alloc_frame(sw->pctx);


	for(;;){
		ptrT* var;
		ptrT* arrayvar;

		if (TRACE_STACK){
			printf("sp:%d|bp:%d|", exe->sp, exe->bp);

			for (int i=0;i<exe->sp;i++){
				printf("%d:%p+%d|", i, (exe->stack[i].as.ptr.address.bytes)  ,  exe->stack[i].as.ptr.offset );
			}
			printf("%d:<>\n", exe->sp);

		}


		switch (pc->opcode) {
			case op_nop: pc++; continue;

			case op_constant:  exe->stack[exe->sp++] = pc->imm;  pc++; continue;
			case op_constantaddref:  exe->stack[exe->sp] = pc->imm; ram_addref( exe->stack[exe->sp++].as.ptr.address.block);  pc++; continue;

			case op_print32:   printf("%d", exe->stack[--exe->sp].as.z32); pc++; continue;

			case op_printptr:   printf("{%p+%x}", exe->stack[exe->sp-1].as.ptr.address.bytes, exe->stack[exe->sp-1].as.ptr.offset); pc++; exe->sp--; continue;

			case op_printstr:
				char* s = exe->stack[exe->sp-1].as.ptr.address.bytes;
				printf("%s", s?s:"{nullstr}"); pc++; exe->sp--;continue;

			case op_printchar:   putc( exe->stack[exe->sp-1].as.z32 , stdout); pc++;exe->sp--;continue;

			case op_getchar:  exe->stack[exe->sp].as.z32=getc(stdin) ; pc++;exe->sp++;continue;

			//add in all the basic expected arithmetic
			//case op_add32:     exe->stack[exe->sp-2].as.u32 = exe->stack[exe->sp-2].as.u32 + exe->stack[exe->sp-1].as.u32; exe->sp--; pc++; continue;
			#define BINOP(NAME, AS, SYMBOL) case op_ ##NAME:     exe->stack[exe->sp-2].as.AS = exe->stack[exe->sp-2].as.AS SYMBOL exe->stack[exe->sp-1].as.AS; exe->sp--; pc++; continue;

			BINOP(add32, z32, +)
			BINOP(sub32, z32, -)
			BINOP(mul32, z32, *)
			BINOP(div32, z32, /)
			BINOP(mod32, z32, %)
			BINOP(equal32, z32, ==)
			BINOP(less32, z32, <)
			BINOP(greater32, z32, >)

			#define UNOP(NAME, AS, OPFUNC, ASARG) case op_ ## NAME:     exe->stack[exe->sp-1].as.AS = OPFUNC(exe->stack[exe->sp-1].as.ASARG);  pc++; continue;

			UNOP(neg32, z32, -, z32)
			UNOP(bnot, z32, !, z32)

			UNOP(ptrvalid, z32 , NULL != , ptr.address.block  )


			BINOP(fadd, f, +)
			BINOP(fsub, f, -)
			BINOP(fmul, f, *)
			BINOP(fdiv, f, /)


			case op_ptrequal:

				void* p1 = exe->stack[exe->sp-2].as.ptr.address.bytes + exe->stack[exe->sp-2].as.ptr.offset;

				void* p2 = exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-2].as.ptr.offset;


				exe->stack[exe->sp-2].as.z32 = (p1==p2);
				exe->sp--;


				pc++;
				continue;


			//variables
			case op_argpick:   exe->stack[exe->sp++] = exe->stack[exe->bp+pc->imm.as.z32];  pc++; continue;

			case op_argaddref:   exe->stack[exe->sp] = exe->stack[exe->bp+pc->imm.as.z32];
			ram_addref( exe->stack[exe->sp++].as.ptr.address.block);
			pc++; continue;



			case op_argtake:   exe->stack[exe->sp++] = exe->stack[exe->bp+pc->imm.as.z32];
			exe->stack[exe->bp+pc->imm.as.z32].as.ptr.address.block = NULL;
			exe->stack[exe->bp+pc->imm.as.z32].as.ptr.offset = 0;
			pc++; continue;



			case op_take:  //takes a pointer from memory, leaving the old one as zero


				if (pc->imm.as.z32 == sizeof(ptrT)){

					var = (void*)  exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset;
					exe->stack[exe->sp-1].as.ptr =*var;
					var->address.bytes = NULL;
				} else { //just a void*
					printf(" taking void*\n");
					void** v = (void*) ( exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);
					exe->stack[exe->sp-1].as.ptr.address.block  = *v;
					exe->stack[exe->sp-1].as.ptr.offset=0;
					*v=NULL;
				}

				pc++;
				continue;

			case op_dup: //duplicate value on stack
				exe->stack[exe->sp] = exe->stack[exe->sp-1];
				exe->sp++;
				pc++;
				continue;

			case op_drop: //duplicate value on stack
				exe->sp--;
				pc++;
				continue;

			case op_dups:
				exe->stack[exe->sp] = exe->stack[exe->sp-1];
				ram_addref(  exe->stack[exe->sp].as.ptr.address.block);
				exe->sp++;
				pc++;
				continue;

			case op_over: //duplicate next-to-last value on stack
				exe->stack[exe->sp] = exe->stack[exe->sp-2];
				exe->sp++;
				pc++;
				continue;

			case op_loadaddref:  //takes a pointer from memory, increasing refcount

				if (pc->imm.as.z32 == sizeof(ptrT)){
					var = (void*) exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset;
					exe->stack[exe->sp-1].as.ptr =*var;
				} else {
					void** v = (void*) ( exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);
					exe->stack[exe->sp-1].as.ptr.address.block  = *v;
					exe->stack[exe->sp-1].as.ptr.offset=0;
				}
				ram_addref(exe->stack[exe->sp-1].as.ptr.address.block);

				pc++;
				continue;

			case op_trash:  //frees top pointer on stack
				exe->sp--;

				ram_free(exe->stack[exe->sp].as.ptr.address.block);
				exe->stack[exe->sp].as.ptr.address.block = NULL;
				exe->stack[exe->sp].as.ptr.offset = 0;



				pc++;
				continue;



			case op_trashstore:  //stores a pointer in memory.  if there is already a value there, it is freed

				if (pc->imm.as.z32 == sizeof(ptrT)){
					var = (void*)  exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset;

					if (var->address.bytes)
						ram_free(var->address.bytes);

					*var = exe->stack[exe->sp-2].as.ptr;
				} else if (pc->imm.as.z32 == sizeof(void*)){

					void** v = (void*) ( exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);
					if (*v)
						ram_free(*v);
					*v = exe->stack[exe->sp-2].as.ptr.address.bytes;
				}


				exe->sp-=2;

				pc++;
				continue;


			case op_load: //TODO check this and also make sure the imm value is set (its not, fix it on switch compiler)

				if (!exe->stack[exe->sp-1].as.ptr.address.bytes){
					printf(" NULL\n");
					exit(1);
				}

				tracef(" load from %p:%d (%p)\n", exe->stack[exe->sp-1].as.ptr.address.bytes,exe->stack[exe->sp-1].as.ptr.offset, exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);

				void* src = exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset;

				//clear before copy into
				memset(  &exe->stack[exe->sp-1], 0, sizeof(valueT));
				memcpy( &exe->stack[exe->sp-1] ,
						src,
						pc->imm.as.z32);



				pc++;
				continue;


			case op_store:

				if (!exe->stack[exe->sp-1].as.ptr.address.bytes){
					printf(" NULL\n");
					exit(1);
				}

				tracef(" store to %p:%d (%p)\n", exe->stack[exe->sp-1].as.ptr.address.bytes,exe->stack[exe->sp-1].as.ptr.offset, exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);

				memcpy( (exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset) ,
						&exe->stack[exe->sp-2] ,
						pc->imm.as.z32);

				exe->sp-=2;
				pc++;
				continue;

			case op_switch_jumpfalse:
				exe->sp--;
				if (exe->stack[exe->sp].as.z32){
					pc++;
					continue;
				}
				//fallthrough

			case op_switch_jump:
				pc = sw->prog + pc->imm.as.z32;
				continue;

			case op_localvar:
				exe->stack[exe->sp].as.ptr.address.block = exe->locals;
				exe->stack[exe->sp].as.ptr.offset = pc->imm.as.z32;
				tracef(" ADDRESS FROM local+%d   %p\n", pc->imm.as.z32, exe->locals);

				exe->sp++;
				pc++;
				continue;


			case op_subvar:
				exe->stack[exe->sp-1].as.ptr.offset += pc->imm.as.z32;
				pc++;
				continue;

			case op_arraycow:

				exe->stack[ exe->sp-1].as.ptr.address.block = zarray_cow( exe->stack[exe->sp-1].as.ptr.address.block, pc->imm.as.z32  ) ;
				pc++;
				continue;

			case op_arrayinfo:

				if (!exe->stack[exe->sp-1].as.ptr.address.block) {
					exe->stack[exe->sp-1].as.z32 =0;
					pc++;
					continue;
				}

				if (pc->imm.as.z32 == 1){
					exe->stack[exe->sp-1].as.z32 = zarray_size( exe->stack[exe->sp-1].as.ptr.address.block);
				} else if (pc->imm.as.z32 == 2){
					typeT* at = zarray_get_meta( exe->stack[exe->sp-1].as.ptr.address.block);
					//printf(" ARRAY IS TYPE %s\n", at->ref->key);
					exe->stack[exe->sp-1].as.z32 = at->ref->size;
				}
				else {
					exe->stack[exe->sp-1].as.z32 = zarray_count( exe->stack[exe->sp-1].as.ptr.address.block);
				}

				//exe->stack[ exe->sp-1].as.ptr.address.block = zarray_cow( exe->stack[exe->sp-1].as.ptr.address.block, pc->imm.as.z32  ) ;
				pc++;
				continue;

			case op_arraysetcount:
				//sp-2 is array
				//sp-1 is the new count
				//TODO: free pointers past count so they don't leak_viewer.  do the same for op_dim.
				zarray_use( exe->stack[ exe->sp-2].as.ptr.address.block, exe->stack[exe->sp-1].as.z32);

				exe->sp-=2;
				pc++;
				continue;


			case op_arrayindex:
				//sp-2 is array
				//sp-1 is the index
				tracef(" --offset is %d , size is %d\n", exe->stack[exe->sp-1].as.z32, pc->imm.as.z32);
				//getc(stdin);

				if (exe->stack[exe->sp-1].as.z32 >= zarray_count(exe->stack[ exe->sp-2].as.ptr.address.block)){
					errorf("Exceed array bounds\n");

				}

				exe->stack[ exe->sp-2].as.ptr.offset += (exe->stack[exe->sp-1].as.z32 * pc->imm.as.z32);
				exe->sp--;
				pc++;
				continue;

			case op_carrayindex:
				//sp-2 is array
				//sp-1 is the index
				tracef(" --offset is %d , size is %d\n", exe->stack[exe->sp-1].as.z32, pc->imm.as.z32);

				//NO bounds checking... this is a C array and that is impossible here

				exe->stack[ exe->sp-2].as.ptr.offset += (exe->stack[exe->sp-1].as.z32 * pc->imm.as.z32);
				exe->sp--;
				pc++;
				continue;


			case op_dim:

				arrayvar = (void*) exe->stack[exe->sp-3].as.ptr.address.bytes +  exe->stack[exe->sp-3].as.ptr.offset;

				int count = exe->stack[exe->sp-2].as.z32;
				int cap = exe->stack[exe->sp-1].as.z32;

				typeT* arraytype = pc->imm.as.ptr.address.type; // [type]$
				tracef(" DIMing array %s for elem size %d  \n", arraytype->key, arraytype->ref->size );

				if (arrayvar->address.block){
					//array already exists... resize
					if (cap >= 0){
						tracef("%d resize\n", cap);
						arrayvar->address.block = zarray_resizef(arrayvar->address.block, arraytype->ref->size, cap, NULL);
						tracef("resized\n");
					}

					if (count >= 0) {
						tracef("%d count\n", cap);
						zarray_use( arrayvar->address.block, count);
					}

				} else{

					arrayvar->address.block = zarray_alloc_size(arraytype->ref->size, cap, clean_array);
					zarray_use( arrayvar->address.block, count);
					arrayvar->offset = 0;
					tracef(" ALLOCATED %d out of %d with elemsize %d  %s\n", count, cap, arraytype->ref->size, arraytype->key);
				}
			//	zarray_use(arrayvar->address.block, count);
				zarray_set_meta( arrayvar->address.block, arraytype );

				exe->sp-=3;
				pc++;
				continue;

			case op_adim:	//allocates array (or object)
							//put in stack

				cap = exe->stack[exe->sp-1].as.z32;
				count = 0;

				arraytype = pc->imm.as.ptr.address.type;

				tracef(" ADIMing array %s for elem size %d  \n", arraytype->key, arraytype->ref->size );

				arrayvar= &(exe->stack[exe->sp-1].as.ptr);
				arrayvar->address.block = zarray_alloc_size(arraytype->ref->size, cap, clean_array);
				arrayvar->offset = 0;
				tracef(" ALLOCATED %d  with elemsize %d  %s\n", count, arraytype->ref->size, arraytype->key);

				zarray_use(arrayvar->address.block, count);
				zarray_set_meta( arrayvar->address.block, arraytype );

				pc++;
				continue;
/*
			case op_sidevar:
					//if stack is     PTR VALUE,  this pushes PTR + constant offset
					exe->stack[exe->sp].as.ptr.address.block = exe->stack[exe->sp-2].as.ptr.address.block;
					exe->stack[exe->sp].as.ptr.offset = exe->stack[exe->sp-2].as.ptr.offset + pc->imm.as.z32;
					pc++;
					exe->sp++;
			continue;
*/

			case op_switch_indexplus:
			{
				void* array = exe->stack[exe->sp-2].as.ptr.address.block;
				int count = zarray_count(array);

				printf("change array %p ", array);
				array = zarray_moref(array, pc->imm.as.z32, 1, NULL);
				printf("to %p\n", array);

				exe->stack[exe->sp-2].as.ptr.address.block = array;

				zarray_use(array, count+1);
				exe->stack[ exe->sp].as.ptr.address.bytes = array;
				exe->stack[ exe->sp].as.ptr.offset = count * pc->imm.as.z32;
				exe->sp++;
				printf("INDEXPLUS elemsize:%d %d->%d\n", pc->imm.as.z32, count, zarray_count(array));
				pc++;
				continue;
			}


			case op_fictional_call:
				if (pc->immtype != tWord){
					errorf(" expected word pointer\n");
					exit(1);
				}
				wordT* word = pc->imm.as.ptr.address.word;

				errorf( "Cannot call %s directly, it is fictional.\n", word->name);
				exit(1);
				continue;

			case op_call:
				if (pc->immtype != tWord){
					errorf(" expected word pointer\n");
					exit(1);
				}
				word = pc->imm.as.ptr.address.word;

				do_call(exe, word);

				pc++;
				continue;

			case op_stop:
			case op_return:
			case op_returnval:
				ram_free (exe->locals);
				exe->locals = old_locals;
				return;

			case op_sys:

				if (pc->immtype != tWord){
					errorf(" expected word pointer\n");
					exit(1);
				}
				word = pc->imm.as.ptr.address.word;

				if (!word->ffi_caller){
					errorf("Need ffi_caller pointer\n");
					exit(1);
				}

				do_ffi_call(exe, word);

				pc++;
				continue;




			default:
				errorf(" unhandled %d %s\n", pc->opcode, instruction_names[pc->opcode]?instruction_names[pc->opcode]:"noname");
				exit(1);
				return;

		}

	}

}

zbool cleansw(void* v){
	switchrunnerT* sw = v;
	debugf(" free prog\n");
	ram_free(sw->prog);
	return ZTRUE;
}


runnerI* compile_for_switch(exectxT* exe, parsectxT* pctx){

	if (pctx->runners[SWITCHRUNNER])
		return pctx->runners[SWITCHRUNNER];


	switchrunnerT* sw = ram_alloc(sizeof(switchrunnerT), cleansw);
	sw->runner.execute = run_switch;

	pctx->runners[SWITCHRUNNER] = &sw->runner;

	switchopT* prog = zarray_alloc( switchopT, 1);

	for (int i=0;i<zvec_count(pctx->codestack);i++){

		instructionT* subtree = zvec_get_at(pctx->codestack, i);


		prog = compile_switch_subtree(exe, sw, prog, subtree);

	}

	switchopT op = {0};
	op.opcode = op_stop;

	prog = zarray_more(prog, 1, NULL);
	prog = zarray_append(prog, op);

	//print_switch_listing(prog);
	printf(" compiled %p\n", prog);
	sw->prog = prog;

	//sw->local_size = pctx->size;
	sw->pctx = pctx;

	print_switch_listing(prog);

	return &sw->runner;
}



//end of simple runner


//find words
wordT* match_word(parsectxT* pctx, parsectxT* searchpctx,  char* name, int inflags, int* rarg, parsectxT** foundpctx){

	int argc = 0;
	int maxarg = zvec_count(pctx->codestack); //most args possible is whole stack
	int flags=0;
	wordT* word = NULL;


	if (pctx->next_match){
		word = pctx->next_match;
		pctx->next_match = NULL;
		if (rarg){
			*rarg = word->type->argc;
			printf(" returning %s with %d args\n", word->name, *rarg);
		}
		return word;
	}


	typeT** argtypes = zarray_alloc(typeT*, maxarg);  //place to keep arg types



	for (int phase=0; phase < 2; phase++){

		if (DEBUG_MATCH){
			printf(" DEBUG MATCH FOR %s phase %d\n", name, phase);
		}


		//todo: if not all flags are set, some searches will be done twice
		//for now I don't care
		if (phase == 0) flags = inflags & (MATCH_RECURSE_PCTX);	//exact match, going up contexts if allowed
		if (phase == 1) flags = inflags & (MATCH_RECURSE_PCTX | MATCH_ALLOW_LIKE); //allow like-types


		if (phase > 0){
			tracef("failed to match exactly... trying phase %d on %s   %x\n", phase, name, flags);
		}

		//get all the words with this name


		//see if any of these words with this name match the args

		for (argc=0;argc <= maxarg; argc++){

			tracef("%d/%d\n", argc, maxarg);

			for (int i=0;i<argc;i++){
				int s = zvec_count(pctx->codestack)-argc+i;
				instructionT* arginst = zvec_get_at(pctx->codestack, s);
				argtypes[i] = arginst->result_type;
				tracef(" %s\n", arginst->result_type? arginst->result_type->key: "--");
				if (!arginst->result_type)
					maxarg=argc;  //make sure we are last one
			}
			zarray_use(argtypes, argc);

			//check all contexts up to the parent
			for (parsectxT* sc = searchpctx; sc; sc=  (flags&MATCH_RECURSE_PCTX)? sc->parent : NULL) {
				//printf(" pctx:%p  sc:%p  sc->parent:%p\n", pctx, sc, sc->parent);
				//get words in this frame with the name
				zvecT* words = zstringmap_get(sc->dictionary, name); //get

				debugf(" try to match %s with %d args in word frame %p \n", name, argc, sc);

				for (int i=0;i< zvec_count(words); i++){

					word = zvec_get_at(words, i);

					//test if the word from sc matches the stack in pctx
					if (test_word(pctx, argtypes, word, flags, argc)){
						if (rarg)
							*rarg = word->type->argc;

						//find return value for word if its a like-type
						if(word->type && word->type->ref && word->type->ref->is_wild){
								tracef(" return type wild %s\n", word->type->ref->key);

								typeT* rettype = resolve_like_type_for_caller(pctx, word->type->ref, word->type, argtypes);
								tracef(" rettype is %s\n", rettype->key);
								typeT* pt = type_find(pctx, NULL, rettype, PROC, 0, argtypes);
								tracef(" new type for word is %s\n", pt->key);


								wordT* new_word = word_alias_mk(word->name, pt, word);
								return new_word;

						}

						ram_free(argtypes);

						if (foundpctx)
							*foundpctx = sc;

						return word; //got one
					}


				}//for words

			}	 //for frame
		} //for argc
	} //for phase
	ram_free(argtypes);
	return NULL;

}

tokenT*  parse(parsectxT* pctx, tokenT* t, char** stop_tokens);

tokenT* parse_number(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	valueT val = {0};

	if (strchr(t->str, '.')){
		val.as.f = atof(t->str);
		push_assembly(pctx, op_constant, 0, &val, tReal, NULL, tReal);
	} else if (strchr(t->str, 'x') == (t->str +1 ) ){
		val.as.u32 = strtol(t->str, NULL, 16);
		push_assembly(pctx, op_constant, 0, &val, tZ32, NULL, tZ32);
	} else {
		val.as.u32 = atoi(t->str);
		push_assembly(pctx, op_constant, 0, &val, tZ32, NULL, tZ32);
	}

	return t;
}

wordT* proc_opcode_mk(parsectxT* pctx,  int opcode, char* name,  typeT* proctype);
wordT* proc_opcode_mk2(parsectxT* pctx,  int opcode, char* name,  char* typestr);
typeT* type_proc_mk(typeT* ret, int n, ...);

tokenT* parse_constant(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	instructionT* inst = pop_arg(pctx);
	t=tnext(t);
	char* name = t->str;

	if (!inst->val_type){


		return NULL;
	}

	if (inst->val_type->category == STEWARD){
		errorf("Constant must not be steward\n");
		return NULL;
	}

	wordT* word = proc_opcode_mk(pctx, op_constant, name, type_proc_mk(inst->val_type,0));
	word->val = inst->val;
	word->val_type = inst->val_type;

	inst->opcode = op_nop;
	inst->result_type = NULL;
	push_subtree(pctx, inst);

	return t;
}

zstringmapT* c_objects=NULL;
zstringmapT* c_constants=NULL;

typedef struct{
	int val32;  //todo van use valueT for more constants if we really need to
} c_constantT;


tokenT* parse_c_constant(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	zbool is_c_constant = ZFALSE;

	instructionT* inst = pop_arg(pctx);
	instructionT* typeinst = pop_arg(pctx);

	t=tnext(t);
	char* name = t->str;

	char* objname = inst->val.as.ptr.address.bytes;
	c_constantT* cc = zstringmap_get(c_constants, objname);

	if (!cc){
		printf(" c constant %s not found\n", objname);
		exit(1);
	}

	typeT* type = typeinst->val.as.ptr.address.type;

	if (type != tZ32){
			errorf(" type %s not supported for sys constant\n", type?type->key:"notype");
			exit(1);
	}

	wordT* word = proc_opcode_mk(pctx, op_constant, name, type_proc_mk(type,0));


	word->val.as.z32 = cc->val32;
	word->val_type = type;

	ram_free(typeinst);
	ram_free(inst);

	return t;
}



tokenT* parse_include(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	instructionT* arg = pop_arg(pctx);

	char* x = ram_loadstr(arg->val.as.ptr.address.bytes);

	tokenize(t, x, arg->val.as.ptr.address.bytes,0);

	ram_free(x);


	ram_free(arg);

	return t;
}


#define OF(PCTX, CAT, REF) type_find(PCTX, NULL, REF, CAT, 0, NULL)

tokenT* parse_string_literal(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	valueT val = {0};
	val.as.ptr.address.bytes = zstrndup(t->str+1, strlen(t->str)-2);

	if (!strcmp(tnext(t)->str , "$")){		//the program will add a ref

		push_assembly(pctx, op_constantaddref, INST_FREE_VALUE, &val, OF(pctx, STEWARD, tString  ) , NULL, OF(pctx, STEWARD, tString ));
		t=tnext(t);
	} else {

		push_assembly(pctx, op_constant, INST_FREE_VALUE, &val, tString, NULL, tString);
	}



	return t;
}





void dereference_assembly(parsectxT* pctx, char* loadername){

	//loadername is usually '@' but can be $ or $$

	if (!loadername) //no loader
		return;

	int argc=0;
	wordT* wloader = match_word(pctx, pctx, loadername, MATCH_RECURSE_PCTX| MATCH_ALLOW_LIKE , &argc, NULL);  //find a loader for it

	if (wloader){
		tracef(" Found loader OPCODE IS %d\n", wloader->opcode);

		instructionT** insts = pop_args(pctx, argc); //get the instruction
		push_assembly(pctx, wloader->opcode,0, NULL, NULL, insts, wloader->type->ref);


	}else {
		errorf(" %s undefined for given type", loadername);
		exit(1);
	}

}




tokenT* autoload(parsectxT* pctx, wordT* w, tokenT* t){
	//Depending on the type it's a reference to, determine if you load it or not

	instructionT* r = pop_arg(pctx);

	char* loadername = "@";  //default loader word
	zbool keep_steward = ZFALSE;


	tracef("- autoload on %s (%s next) for %s\n", w->name,tnext(t)? tnext(t)->str:"EOF", w->type->key);



	if (tnext(t) && !strcmp(tnext(t)->str, "&")){

		loadername = NULL; //do not load

		if (r->opcode == op_argpick && r->result_type->category != REFERENCE){

			//if using & on an arg that is't a reference doesn't make sense.  the higher-level parser will see the & next and do something with it
			push_subtree(pctx, r); //put it back
			return t;
		}

		t=tnext(t);		//skip over token

	} else if (tnext(t) && !strcmp(tnext(t)->str, "=")){

		loadername = NULL; //do not load


	}else if (tnext(t) && !strcmp(tnext(t)->str, "$")){

		loadername = "$";
		t=tnext(t);		//skip over token
		keep_steward=ZTRUE;

	} else if (tnext(t) && !strcmp(tnext(t)->str, "$$")){

		loadername = "$$";
		t=tnext(t);		//skip over token
		keep_steward=ZTRUE;
	}


	if (r->opcode == op_argpick){
		tracef(" p loadername %s\n", loadername);

		if (loadername &&!strcmp(loadername, "$")){
			r->opcode = op_argaddref;
			loadername = NULL;
		}

		if (loadername&&!strcmp(loadername, "$$")){
			r->opcode = op_argtake;
			loadername = NULL;
		}

		if (loadername&&!strcmp(loadername, "@") && r->result_type->category ==STEWARD){
			r->result_type = r->result_type->ref;
			loadername = NULL;
		}


		if (r->result_type->category == ARRAY)
			loadername=NULL; //leave it as an array

		tracef(" p2 loadername %s\n", loadername);
	}

	//


	typeT* target_type = NULL;


	if (r->result_type&& r->result_type->category == SUBTREE)
		target_type = NULL;
	else if (r->result_type)
		target_type = r->result_type->ref;




	if (!target_type) {
		tracef("no target type\n");
		push_subtree(pctx, r); //put it back
		return t;
	}
	tracef("result type %s    target type %s\n",r->result_type->key, target_type->key);

	if (target_type->category == STEWARD && !keep_steward && loadername){
		//if a variable name of a STEWARD pointer is used without $ or $$ or &, it goes on the stack as a regular pointer
		//r->result type is something$& (target is something$), so lets create something&
		r->result_type = type_find(pctx,NULL,target_type->ref, REFERENCE,0, NULL);
	}
	tracef(" tt %s \n", w->type->key);

	if (w->type->category == VARIABLE) {
		//word that put item on stack is a variable

		if (w->type->ref->category == FRAME)	//if variable is a struct, keep it reference
			loadername = NULL;

	}

	if ((w->type->category == ARG) && (w->type->ref->category == REFERENCE)   && (w->type->ref->ref->category == FRAME  )) {
		//if word is an arg that contains a reference to a frame, leave it a reference
		loadername = NULL;
	}

	if ((w->type->category == ARG) && (w->type->ref->category == REFERENCE)   && (w->type->ref->ref->category == PENDING  )) {
		//if word is an arg that contains a reference to a PENDING (an incomplete struct type), keep it as a reference
		loadername = NULL;
	}


	if (w->type->category == PROC){
		// if word is a proc (that had autoload flag set)
		if (w->type->ref->category == REFERENCE){  //and the proc returns a pointer
			if (w->type->ref->ref->category == FRAME) // if the reference returned is to a frame, keep it a reference
				loadername = NULL;
		}

	}
	tracef("loadername %s\n", loadername);
	push_subtree(pctx, r); //put it back
	dereference_assembly(pctx, loadername);	//dereference with the appropriate loader


	return t;
}

tokenT* parse_cast(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	instructionT* arg = pop_arg(pctx);
	arg->result_type = w->val.as.ptr.address.type;  //change the type
	push_subtree(pctx, arg); //put it back
	return t;
}

tokenT* parse_get_subtree(parsectxT* pctx, wordT* w, tokenT* t, int argc){

			instructionT* inst = pop_arg(pctx);
			typeT* inst_type = OF( pctx, STEWARD, OF(pctx, SUBTREE, inst->result_type));
			valueT v = {0};
			v.as.ptr.address.subtree = inst;
			push_assembly( pctx, op_constantaddref, INST_FREE_VALUE, &v, inst_type, NULL, inst_type);
			return t;
}

tokenT* parse_default(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	tracef("default parse for %s %s  (%d args)\n", t->str, w->type->key, w->type->argc);

	instructionT** args = pop_args(pctx, argc);

	//if word is a PROC, assemble it using the word's opcode and value



	if (w->type->category == PROC) {


		instructionT* inst = push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, args, w->type->ref);
		inst->comment = ram_addref(w->comment);

/*
		if (tnext(t) && !strcmp(tnext(t)->str,">-")){
			t=tnext(t);


			instructionT* inst = pop_arg(pctx);

			typeT* inst_type = OF( pctx, STEWARD, OF(pctx, SUBTREE, inst->result_type));
			valueT v = {0};
			v.as.ptr.address.subtree = inst;

			push_assembly( pctx, op_constantaddref, INST_FREE_VALUE, &v, inst_type, NULL, inst_type);
			return t;
			//			printf(" & after function call\n");
			getc(stdin);
		}
*/

		if (w->autoload){
			t = autoload(pctx, w, t);
		}

		return t;
	}

	//if its not a proc, then its a variable of some kind
	//note: thislooks just like a function but with no args
	//not sure if it will stay that way, butis that way for now, so maybe it will merge with the above

	if (w->type->category == ARG){
		tracef("%s arg  at bp %d\n", w->name, w->val.as.z32);

		push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, w->type->ref );

		if (w->autoload) //enforce autoload rules
			t = autoload(pctx, w, t);

		return t;
	}

	//a reference to proc (not a variable that contains a function pointer)
	//but a proc directly dscribed as ( types -> rettyoe)& proc name

	if (w->type->category == REFERENCE && w->type->ref->category == PROC){

		tracef( "a %s\n", w->type->ref->key);
		push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, w->type->ref);
//exit(1);
		return t;
	}

	if (w->type->category == VARIABLE){


		if (w->opcode == op_subvar){
			errorf("Can't use that here %s %s\n", w->name, w->type->key);
			exit(1);
		}

		if (w->opcode == op_localvar){
			if (w->restrict_pctx != pctx){
				errorf(" Cannot use here\n");
				exit(1);
			}
		}

		push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, OF(pctx, REFERENCE, w->type->ref));

		if (w->autoload){

			t = autoload(pctx, w, t);

		}


		return t;
	}

	errorf("Don't know what to do\n");
	exit(1);
}


parsectxT* parsectx_mk(parsectxT* parent, char* name);

typeT* type_mk(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args );


tokenT* parse_array(parsectxT* pctx, wordT* w, tokenT* t, int argc){

		int depth = codestack_depth(pctx);
		char* stops[] = {"]",  NULL};
		t = tnext(t);


		t = parse(pctx, t, stops);

		//should be a datatype on the stack now

		depth = codestack_depth(pctx) - depth;

		if (depth == 1 ){
			instructionT* typearg = pop_arg(pctx);

			if (typearg->result_type == tType){


				typeT* array_type = type_find(pctx, NULL, typearg->val.as.ptr.address.type, ARRAY, 0, NULL);
				valueT v = {0};
				v.as.ptr.address.type = array_type;

				push_assembly(pctx, op_constant, 0, &v, tType, assembly_args_mk(1, typearg) , tType);

				return t;
			}
		}

		PERROR(pctx, "expected type\n");

		return t;
}

tokenT* parse_array_values(parsectxT* pctx, wordT* w, tokenT* t, int argc){


	int depth = codestack_depth(pctx);
	char* stops[] = {"]",  NULL};
	valueT v = {0};
	t = tnext(t);
	t = parse(pctx, t, stops);

	depth = codestack_depth(pctx) - depth;

	if (depth == 0){
		errorf("Need at least one to make array\n");
	}

	instructionT** args = pop_args(pctx, depth);
	//instructionT** appends = zarray_alloc(instructionT*, depth);


	v.as.z32 = depth;

	push_assembly(pctx, op_constant, 0, &v, tZ32, NULL, tZ32);
	instructionT** icount = pop_args(pctx, 1);
	v.as.ptr.address.type = OF(pctx,ARRAY,args[0]->result_type);
	v.as.ptr.offset = 0;


	push_assembly(pctx, op_adim, 0, &v, tType, icount , OF(pctx, STEWARD, OF(pctx,ARRAY,args[0]->result_type)) );

	typeT* type = args[0]->result_type;

	for(int i=0;i<zarray_count(args);i++){

		instructionT* inst_array = pop_arg(pctx);
		if (!type_equal(type, args[i]->result_type)){
			errorf("Type mismatch in array initializer: expected %s, got %s\n", type->key, args[i]->result_type->key);
			exit(1);

		}
		v.as.z32=type->size;
		push_assembly(pctx, op_indexplusstore, 0, &v, tZ32, assembly_args_mk(2, inst_array, args[i]),  OF(pctx, STEWARD, OF(pctx,ARRAY,type))   );
		args[i] = NULL;
	}

	ram_free(args);

/*
	if (!strcmp(tnext(t)->str, "$")){
		//skip over $
		t = tnext(t);
	} else {
		errorf("Dynamically allocated array requires steward.  '[...]$");
		exit(1);
	}
*/


	return t;
}


tokenT* parse_fill_frame(parsectxT* pctx, wordT* w, tokenT* t, int argc){


	int depth = codestack_depth(pctx);
	char* stops[] = {"]", ":",  NULL};
	valueT v = {0};
	t = tnext(t); //skip 'fill'

	instructionT* obj = pop_arg(pctx);
	push_subtree(pctx, obj);

	typeT* ty = obj->result_type;

	if (ty ->category == REFERENCE && ty->ref->category == FRAME){
		ty = ty->ref;
	} else {
		errorf(" can only fill by struct reference\n");
		exit(1);
	}


	while(1){

		depth = codestack_depth(pctx);
		t = parse(pctx, t, stops);
		if (!strcmp(t->str, "]"))
			break;

		if (!strcmp(t->str, ":")){


			if (codestack_depth(pctx) - depth != 1){
				errorf(" when filling, : should appear with one 1 stack element\n");
				exit(1);
			}


			printf(" TO STORE IN %s\n", tnext(t)->str);

			wordT* m =  match_word(pctx, ty->word->target_pctx, tnext(t)->str, 0, NULL, NULL);

			if (m && m->type->category == VARIABLE){

				valueT v = {0};
				v.as.ptr.address.word = m;

				//copy offset pointer
				push_assembly(pctx, op_over, 0, NULL, NULL, NULL, obj->result_type);
				//add offset to the obj pointer
				push_assembly(pctx, op_subvar, 0, &v, tWord, pop_args(pctx, 1) , OF(pctx, REFERENCE, m->type->ref));
				dereference_assembly(pctx, "=");





				instructionT* assign = pop_arg(pctx);
				instructionT* obj = pop_arg(pctx);
				push_assembly(pctx, op_block, 0, NULL, NULL, assembly_args_mk(2, obj, assign), obj->result_type);


				printf(" Found %p\n", m);


			} else {
				errorf(" %s not found in %s\n", tnext(t)->str, ty->key);
				exit(1);
			}

			t=tnext(t); //skip colon
			t=tnext(t); //skip name

		}



	}

	if (codestack_depth(pctx) != depth){
			printf(" Should be same size %d %d \n", codestack_depth(pctx), depth);
			exit(1);
	}


	push_assembly(pctx, op_drop, 0, NULL, NULL, pop_args(pctx, 1), NULL); //cleanup stack value

//	exit(1);

	return t;
}



tokenT* parse_type_list(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	t = tnext(t); //skip over "("
	int depth = codestack_depth(pctx);  //get codestack depth

/*
 *  (typename:varname typename:varname)
 *  (typename:varname | typename:varname -> returntype)
 * varnames optional.
 * */
	char* stops[]={ ":", "->", ")" , "like", /*"|" ,*/  NULL};

	zvecT* argnames = zvec_mk(NULL, 8);
//	zvecT* argtypes = zvec_mk(NULL, 8);
	instructionT** args = NULL;
	instructionT* ret_arg = NULL;

	zvec_disown(argnames);
	//zvec_disown(argtypes);

	int expect_ret=0;
	typeT* ret = NULL;
	//int leave = 0;

	while(t->tok != ')'){

		t = parse(pctx, t, stops);	//parse until a stop

		if (!t)
			break;

		if ((t->tok == ':') ) {
			t=tnext(t);
			tracef("arg name %s\n", t->str);
			zvec_add(argnames, t->str);

			int count = codestack_depth(pctx) - depth;
			if (count != zvec_count(argnames)){
				errorf("Format must be  type:name type:name ...\n");
				exit(1);
			}

			t=tnext(t);
		}else if (t->tok == TOKEN_PAIR('-','>')){
			tracef(" next type is return\n");
			expect_ret = 1;
			t=tnext(t);
		}/* else if (!strcmp(t->str, "|")){
				leave = codestack_depth(pctx)-depth;
				t=tnext(t);
		}*/else if (!strcmp(t->str, "like")){
				valueT v = {0};
				t=tnext(t);
				int is_like =-1;
				//find type as one of the previous args
				for(int i=0;i<zvec_count(argnames);i++){
					if (!strcmp(zvec_get_at(argnames, i), t->str)){
						is_like = i;
						break;
					}
				}

				if (is_like != -1) {
					v.as.ptr.address.type = type_mk(NULL, t->str, NULL, LIKE, 0, NULL);
					v.as.ptr.address.type->argc = is_like;
					push_assembly(pctx, op_constant, INST_FREE_VALUE, &v, tType, NULL, tType);
				} else {
						errorf("like no named arg %s\n", t->str);
						exit(1);
				}
				t=tnext(t);
		}

	}

	if(ERR(pctx))
		return NULL;


	if (expect_ret){

		ret_arg = pop_arg(pctx);
		if (ret_arg->result_type != tType || ret_arg->val_type != tType || ret_arg->val.as.ptr.address.type == NULL){
			PERROR(pctx,  "missing or bad type name in arglist\n");
		}
		ret = ret_arg->val.as.ptr.address.type;


		tracef(" return value: ");
		type_print(ret);
	}
	tracef("\n");

	//now process unnamed args
	int count = codestack_depth(pctx) - depth;
	if (count>0){
		args = pop_args(pctx, count);
	}

	if ((zvec_count(argnames) > 0) && (count != zvec_count(argnames))){
		errorf("Either all parameters must be named, or none of them\n");
		exit(1);
	}

	tracef("parsed proto:\n");
	for (int i=0;i<count;i++){

		if (zvec_count(argnames)> i){
			tracef("  %d %s ", i, zvec_get_x_at(argnames,char*, i));
		}

		iftrace (type_print( args[i]->val.as.ptr.address.type));
		tracef("\n");

	}

	//create the proctype with named args
	//terminate argnames and argtypes will null
	zvec_add(argnames, NULL);

	//create the type

	typeT* ptype = type_mk(NULL, NULL, ret, PROC, 0, inst_const_types(args));

	ptype->opt_argnames = zvec_detach(argnames,NULL);
	//ptype->argleave = leave;

	valueT v = {0};
	v.as.ptr.address.type= ptype;

	push_assembly(pctx, op_constant, INST_FREE_VALUE, &v, tType, args, tType);

	ram_free(argnames);
	ram_free(ret_arg);  //discard return address arg (if exist)

	return t;

}
/*
tokenT* parse_condblock(parsectxT* pctx, wordT* w, tokenT* t, int argc){
	t=tnext(t);
	char* stops []= { "end",  NULL};
	printf(" START CONDBLOCK\n");
	typestack_push(pctx, CondBlock); 
	t= parse(pctx, t, stops);
	typestack_pop(pctx, 1);  //get rid of the condblock
	printf(" STOP CONDBLOCK\n");
	return t;
}
*/
/*
tokenT* parse_cond(parsectxT* pctx, wordT* w, tokenT* t, int argc){
	
	t=tnext(t);
	char* stops [] ={ "end",  NULL};

	printf(" START COND\n");

	t= parse(pctx, t, stops);

	printf(" END COND\n");
	return t;
}
*/

wordT* proc_mk(parsectxT* pctx, char* name, typeT* proctype);

/*
tokenT* parse_idxplusstore(parsectxT* pctx, wordT* w, tokenT* t, int argc){
	//Stack is  [Any]$  val

	instructionT* val_arg = pop_arg(pctx); //get the value

	instructionT* array_arg = pop_arg(pctx); //get the array


	push_subtree( pctx, array_arg); //put the array arg back
	push_subtree( pctx, val_arg); //put val arg back


	valueT v = {0};
	v.as.z32 = val_arg->result_type->size;

	push_assembly(pctx, op_indexplus, 0, &v, tZ32, NULL, array_arg->result_type);
	dereference_assembly(pctx, "="); //perform a store

	//	dereference_assembly(
	//push_assembly(pctx, op_constant, 0, &val, tReal, NULL, tReal);

}
*/


#if 1


tokenT* parse_var(parsectxT* pctx, wordT* wi, tokenT* t, int argc){

	/*
	 * 	Z32 var x  		//create variable x
	 *  20 Z32 .var x& =
	 *  20 'a=	//create variable from value on stack, and store it
	 */


	instructionT* arg0 = pop_arg(pctx);

	zbool store = ZFALSE;	//store the value in variable
	zbool copy = ZFALSE;	//store the value in variable, and keep a copy on stack


	if (!strcmp(t->str, ":")) {
		store=ZTRUE;
	}

	if (!strcmp(t->str, "::")) {
		store = ZTRUE;
		copy = ZTRUE;
	}


	typeT* innertype = NULL; //type of value the variable contains

	if ( arg0->val_type == tType && !store){
		innertype = arg0->val.as.ptr.address.type; //this is a datatype, like a Z32 or whatever
	} else if (store) {
		innertype = arg0->result_type;
	} else {
			errorf("unhandled parse_var case type=%s  token=%s \n", innertype->key, t->str);
	}

	char* varname = tnext(t)->str;


	if (!store)	{//if not keeping the var, advance, so we don't reparse the name
					//if we ARE keeping the var, not advancing will make the variable name be parsed again, which uses the variable
		t=tnext(t);
	}


	wordT* w = NULL;


	if(!innertype){
		PERROR(pctx,  "Type constant or example value not found\n");
		ERRTOK(pctx, t);
		return NULL;
	}

	if (innertype->category == REFERENCE){
		PERROR(pctx,  "Variables cannot hold references\n");
		ERRTOK(pctx, t);
		return NULL ;
	}

	if (innertype->category == ARRAY){
		PERROR(pctx,  "Variables cannot hold arrays (must use steward)\n");
		ERRTOK(pctx, t);
		return NULL;
	}

	if (innertype == tString){
		PERROR(pctx,  "Variables cannot hold string (must use steward)\n");
		ERRTOK(pctx, t);
		return NULL;
	}

	typeT* vartype = OF(pctx, VARIABLE, innertype);

	w = word_mk(pctx, varname, vartype); //word for the variable
	printf(" %d CREATING VAR %s of %s  @%d %d\n", argc,varname,  vartype->key , w->offset, vartype->size );


	if (w) {
		w->val.as.ptr.address.word =w;
		w->val_type = tWord;

		if (pctx->is_section){
			w->opcode = op_sectionvar;
		} else if (pctx->is_frame){
			w->opcode = op_subvar;
		} else if( pctx->parent){
			w->opcode = op_localvar;
			w->restrict_pctx = pctx; //can only be addressed relative to this context
		} else {
			printf("what\n");
			exit(1);
		}

		if (innertype->category != FRAME) //for all variables except 'static' structs, load them
			w->autoload = ZTRUE;

		// printf(" set opcode.  w is %s\n", w->val.as.ptr.address.word->name);


		if (store){

			if(copy){

					push_subtree(pctx, arg0); //push value

					if (innertype->category == STEWARD)
						push_assembly( pctx, op_dups, 0, NULL, NULL, NULL, innertype);  //push  duplicate, adding ref
					else
						push_assembly( pctx, op_dup, 0, NULL, NULL, NULL, innertype);  //push  duplicate


			}
			else
				push_subtree(pctx, arg0); //push value

			arg0 = NULL;

			//put the variable on
			push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, OF(pctx, REFERENCE, w->type->ref));
			t=tnext(t);
			dereference_assembly(pctx, "="); //do a store


			if (copy){
				//if we did a copy, pop the store instruction off, and modify it's result type to be the value type, instead of nothing.
				//that way it accounts for the dup, and leaves the value on the stack ready to be used

				instructionT* istore = pop_arg(pctx); //grab the store
				if (istore->result_type == NULL){
					istore->result_type = innertype;
				} else {
					errorf("Unexpected return value from storing var: %s\n", istore->result_type->key);
					exit(1);
				}
				push_subtree(pctx, istore);
			}



		}

		if (tnext(t)){
			printf(" NEXT IS %s\n", tnext(t)->str);
			if (!strcmp(tnext(t)->str, "&")){
				t=tnext(t);
				push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, OF(pctx, REFERENCE, w->type->ref));
			}
		}


	}
	ram_free(arg0);



	return t;
}


//make a procedure type
typeT* type_proc_mk(typeT* ret, int n, ...){

	va_list args;
	typeT** types = NULL;

	if (n){
		types = zarray_allocd( typeT*, n, zarray_destruct_pointers);
		//types = zarray_alloc( typeT*, n);
		va_start(args, n);
		for (int i=0;i<n;i++){
			types[i] = va_arg(args, typeT*);	//get next arg
			ram_addref(types[i]);
		}
		va_end(args);
		zarray_use(types, n);
	}

	return type_mk(NULL, NULL, ret,PROC,0,types);

}

void interpreter_callback( ffi_cif* cif, void* ret, void** args, void* vword){
	wordT* word = vword;

	int argc = zarray_count(word->type->ref->argtypes);

	debugf(" in interpreter_callback for %s %s \n", word->name, word->type->key  );

	exectxT* exe = word->ffi_caller->exe;


	for (int i=0;i<argc;i++){

		//push the thing on the stack
		//clear
		exe->stack[exe->sp].as.ptr.offset = 0;

		if (cif->arg_types[i] == &ffi_type_pointer){
			char** ptr = args[i];
			exe->stack[exe->sp].as.ptr.address.bytes = *ptr;
			tracef("XXX push %p\n", *ptr);
		} else if (cif->arg_types[i] == &ffi_type_sint){

			exe->stack[exe->sp].as.ptr.address.bytes = NULL;
			int * s = args[i];
			exe->stack[exe->sp].as.z32 = *s;
			tracef("XXX push %d\n", *s);
		} else {
				errorf(" unhandled callback arg case %s\n", word->type->argtypes[i]->key);
		}

		exe->sp++;
		tracef("sp at %d \n", exe->sp);
	}

	//call the interpreted function
	do_call( exe, word);

	if (cif->rtype == &ffi_type_sint){
		int* r = ret;
		*r = exe->stack[exe->sp-1].as.z32;
		exe->sp--;

	} else if (cif->rtype == &ffi_type_pointer){

		*(void**)ret = exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset;

		exe->sp--;

	} else if (cif->rtype != &ffi_type_void){
		errorf(" unknown callback return type\n");
		exit(1);
	}


}

/* FFI stuff */




void int_add_c_val32(char* name, int val){

	if (!c_constants){
		c_constants = zstringmap_mk(64);
	}
	c_constantT* cc = ram_alloc(sizeof(c_constantT), NULL);

	cc->val32 = val;

	printf("CONST %s\t%d\n", name, val);

	zstringmap_put(c_constants, name, cc);
}

void int_add_c_object(char* name, void* proc){

	if (!c_objects){
		c_objects = zstringmap_mk(64);
		zstringmap_disown(c_objects);
	}

	zstringmap_put(c_objects, name, proc);
	printf(" Store %s %p\n", name, proc);


}

ffi_type* type_map_to_c(typeT* type){

	if (!type)
		return &ffi_type_void;

	tracef(" map %s to C\n", type->key);

	if (type == tZ32 || type == tBit)
		return &ffi_type_sint;


	if (type == tByte)
		return &ffi_type_uint8;


	if (type == tReal)
		return &ffi_type_float;

	if (type == tDouble32)
		return &ffi_type_double;

	if (type == tString)
		return &ffi_type_pointer;

	if (type == tSize32 || type == tLong32){ //TODO: windows might have 32-bit long?

		if (sizeof(size_t) == 8)
			return &ffi_type_sint64;
		else{
			errorf(" non-64 bit size\n");
			exit(1);

		}
	}

	if (type == tZL16){
		return &ffi_type_sint16;
	}

	if (type == tUL16){
		return &ffi_type_uint16;
	}

	if (type->category == REFERENCE || type->category == CPOINTER || type->category == ARRAY|| type->category == STEWARD)
		return &ffi_type_pointer;

	if (type->category == SUBTREE || type->category == STEWARD && (type->ref->category == REFERENCE || type->ref == tString  ) ){
		//reference counted pointer or string
		return &ffi_type_pointer;
	}

	if (type == tType)
		return &ffi_type_pointer;


	if (type == tWord)
		return &ffi_type_pointer;

	if (type->category == PROC){
		return &ffi_type_pointer;
	}

	errorf(" unhandled type translation: %s\n", type->key);
	exit(1);
	return NULL;
}


zbool clean_caller(void* v){
	callerT* caller = v;
	ram_free(caller->name);
	ram_free(caller->exe);
	return ZTRUE;
}

void set_ffi_types(callerT* caller,typeT* ret, typeT** args, int argc){

	for (int i=0;i<argc;i++){
		caller->types[i] = type_map_to_c( args[i]);
	}

	caller->rettype = type_map_to_c(ret);


}

callerT* caller_mk(char* name, void* fptr, typeT* ret,  typeT** args){

	int argc = 0;
	if (args){
		argc = zarray_count(args);
	}

	callerT* caller = ram_alloc(sizeof(callerT)+ sizeof(ffi_type*) * (argc +1), clean_caller);

	caller->name = name?zstrdup(name):"noname";

	set_ffi_types(caller, ret, args, argc);

	caller->funcptr = fptr;

	if (ffi_prep_cif(&caller->cif, FFI_DEFAULT_ABI, argc, caller->rettype, caller->types) == FFI_OK){
		printf("OK\n");
		return caller;
	}

	ram_free(caller->name);
	ram_free(caller);
	return NULL;
}


callerT* callback_mk(char* name, typeT* ret,  typeT** args, wordT* w){

	int argc = 0;
	if (args){
		printf(" callback with %d args\n", argc);
		argc = zarray_count(args);
	}

	callerT* caller = ram_alloc(sizeof(callerT)+ sizeof(ffi_type*) * (argc +1), clean_caller);

	caller->name = name?zstrdup(name):"noname";

	set_ffi_types(caller, ret, args, argc);


	if (ffi_prep_cif(&caller->cif, FFI_DEFAULT_ABI, argc, caller->rettype, caller->types) == FFI_OK){

		printf("closer cif ok\n");


		caller->ffi_closure = ffi_closure_alloc( sizeof( ffi_closure), &(caller->ffi_closure_code));

		printf(" got %p %p \n", caller->ffi_closure, caller->ffi_closure_code);

		if (ffi_prep_closure_loc(caller->ffi_closure, &(caller->cif), interpreter_callback, w, caller->ffi_closure_code) == FFI_OK){


			return caller;
		}
	}
	printf(" closure error\n");
	ram_free(caller->name);
	//free ffi stuff too
	exit(1);
	return NULL;
}


callerT* caller_lookup(char* soname, char* name, typeT* ret,  typeT** args){
	void* func = NULL;

	void* handle = RTLD_DEFAULT;

	dlerror(); //clear errors

	if (!strcmp("_LIBC", soname)){
		soname=NULL;	//will use RTLD_DEFAULT for libc, since its already loaded
	} else if (!strcmp("_INTERNAL", soname)){
			func = zstringmap_get(c_objects, name);
			soname = NULL;
	}

	if (soname){
		printf(" To find in %s  .so\n", soname);

		handle= dlopen(soname, RTLD_LAZY);

		printf("handle:%p\n", handle);
		char* err = dlerror();
		if (err){
			fprintf(stderr, "Error loading %s : %s : %s\n", soname, name, err);
			return NULL;
		}
	}

	if (!func){

		func = dlsym(handle, name);
		char* err = dlerror();
		if (err){
			fprintf(stderr, "Error loading %s : %s : %s\n", soname, name, err);
		}

	}

	printf(" found %s func:%p\n", name,  func);

	if (func)
		return caller_mk(name, func, ret, args);


	return NULL;
}


/* End FFI */



tokenT* parse_proc(parsectxT* pctx, wordT* wi, tokenT* t, int argc){

	int tolerate=0;
	int proto=0;
	parsectxT* install_pctx = pctx;  //by default this item is in THIS context

	instructionT* libname = NULL;//for external functions
	instructionT* symname = NULL;
	instructionT* cproc = NULL;

	if (wi->type->argc == 3){  //library name and function name (for dynamic loading)

		symname = pop_arg(pctx);
		libname = pop_arg(pctx);

	}

	if (wi->type->argc == 2){  //explicitly passed c function pointer)

		cproc = pop_arg(pctx);

	}

	instructionT* arg0 = pop_arg(pctx);


	if (!strcmp(t->str, "expose" )){

		t=tnext(t);

		if (!strcmp(t->str, "proc" ) && pctx->frametype && pctx->parent) {
			printf(" put in parent namespace");
			install_pctx = pctx->parent; //install in the parent context
		} else {
			errorf(" cannot use expose here\n");
			exit(1);

		}

	}
	if (!strcmp(t->str, "proto")){
		tracef(" creating prototype\n");
		proto = 1;
	}

	t = tnext(t);

	if (!strcmp(t->str, "optional")){
			tolerate=1;
			t=tnext(t);
	}

	if (!strcmp(t->str, "^")){ //for 'sys' expose outside section
			tolerate=1;
			t=tnext(t);
			if (pctx->parent){
				install_pctx = pctx->parent;
			}
	}

	char* varname = t->str;

	wordT* w = NULL;
	typeT* vartype = NULL;

	if ( arg0->val_type == tType){

		vartype = arg0->val.as.ptr.address.type;  //this is a datatype, like a proc



		tracef(" %d CREATING PROC %s of %s\n", argc,varname,  vartype->key  );
		w = word_mk(install_pctx, varname, vartype); //word for the variable



	} else {

		PERROR(pctx, "Proc type constant not found\n");
	}

	ram_free(arg0);
	arg0 = NULL;

	if(proto){
		w->opcode=op_call;
		w->val.as.ptr.address.word =w;
		w->val_type = tWord;
		return t;
	}


	if (w && symname && symname->val_type== tString ){
		//external library function

		if (w->opcode == op_call){
			errorf("Cannot 'sys' a function that already exists (even if its just a prototype\n");
			exit(1);
		}
		w->opcode = op_sys;

		w->ffi_caller = caller_lookup(libname->val.as.ptr.address.bytes, symname->val.as.ptr.address.bytes, w->type->ref, w->type->argtypes );

		w->val.as.ptr.address.block = w;
		w->val_type = tWord;

		if (!w->ffi_caller && !tolerate){
			PERROR(pctx, zstrprintf(NULL, "Could not find %s:%s\n",
				libname->val.as.ptr.address.bytes, symname->val.as.ptr.address.bytes));
		}

		//if the function returns a special C compatibility type, map it to a normal type so things work normally

		if (w->type->ref == tLong32) {
			ram_free(w->type->ref);
			w->type->ref = ram_addref(tZ32);
		}

		if (w->type->ref == tZL16) {
			ram_free(w->type->ref);
			w->type->ref = ram_addref(tZ32);
		}

		if (w->type->ref == tUL16) {
			ram_free(w->type->ref);
			w->type->ref = ram_addref(tZ32);
		}



		ram_free(symname);
		ram_free(libname);


		return t;
	}

	if (cproc){
		//an explitly given c pointer
		printf(" todo: defer this link until compile time\n");
		exit(1);


	}

	if (w){
		t = tnext(t); //skip over name
		//create parse context
		parsectxT* innerpctx = parsectx_mk(pctx, zstrprintf(NULL, "proc %s %s", w->name, w->type->key));
		w->target_pctx = innerpctx;

		w->val.as.ptr.address.word =w;
		w->val_type = tWord;

		printf (" %s XXX: %s\n",w->name, vartype->key);
		if (vartype->category == REFERENCE && vartype->ref->category == PROC){
			w->opcode = op_constant;
			//this makes it so C can call the function
			w->ffi_caller = callback_mk(w->name, vartype->ref->ref, vartype->ref->argtypes, w);
			printf("XXX Made ffi closure OK caller struct %s %p %p\n", w->name, w->ffi_caller, w->ffi_caller->ffi_closure_code);
		} else {
			w->opcode = op_call;
		}

		typeT* proctype = w->type;
		if (proctype->category == REFERENCE){
			printf("Changing %s to %s\n", proctype->key, proctype->ref->key);
			//getc(stdin);
			proctype = proctype->ref;
		}

		//create the variable arg names
		for (int i=0;i<proctype->argc;i++){
			tracef("adding arg %s",proctype->opt_argnames[i] );
			typeT* atype = proctype->argtypes[i];

			if (atype->category == ITERATED && atype->ref && atype->ref->category == ARRAY){
				//if function input is iterated, inside the function treat the arg as an element of the array
				atype = atype->ref->ref; //get the element inside
				w->opcode = op_fictional_call;

			}

			wordT* argvar = word_mk(innerpctx, proctype->opt_argnames[i], OF(pctx, ARG, atype));
			argvar->val.as.z32 = -proctype->argc + i; //offset from bp
			argvar->val_type = tZ32;
			argvar->opcode = op_argpick;
			argvar->autoload = ZTRUE;
			argvar->restrict_pctx = innerpctx;
			//getc(stdin);
		}

		//create the 'return' keyword for this context, using the return value
		if (proctype->ref && proctype->ref->category == REFERENCE){
			errorf(" Cannot return a reference\n");
			exit(1);
		}


		if (proctype->ref)
			proc_opcode_mk(innerpctx, op_returnval, "return", type_proc_mk(NULL, 1, proctype->ref));
		else //no return value
			proc_opcode_mk(innerpctx, op_return, "return", type_proc_mk(NULL, 0));

		char* stops[]={"end", NULL};

		printf(" parse inner pctx %p for word %p\n", innerpctx, w);


		t= parse(innerpctx, t, stops);

		printf(" Parsed proc %s %s  %p\n", w->name, w->type->key, t);

		ERRUP(pctx,  innerpctx);

		if (OK(pctx)){
			//codestack_print(innerpctx->codestack,0,ZTRUE);
		}


	}


	return t;
}






tokenT* parse_frame(parsectxT* pctx, wordT* w, tokenT* t, int argc){
	zbool issection = ZFALSE;

	if (!strcmp(t->str, "section")){
		issection = ZTRUE;
	}

	t=tnext(t); //skip 'type' string

	//should be the name now
	char* name = t->str;

	debugf(" parsing frame for %s\n", name);

	t=tnext(t);

	//create incomplete data type

	typeT* type = type_mk(pctx, name, NULL, PENDING, 0, NULL);


	//create parse context
	parsectxT* innerpctx = parsectx_mk(pctx, zstrprintf(NULL, "frame %s ", type->name));
	innerpctx->frametype = type;
	innerpctx->is_frame = ZTRUE;
	innerpctx->is_section = issection;

	type->word->target_pctx = innerpctx;

	char* stops[]={"end", NULL};

	printf(" parse inner pctx %p for word %p\n", innerpctx, type->word);
	//getc(stdin);

	t= parse(innerpctx, t, stops);

	printf("Parsed frame:\n");
	codestack_print(innerpctx->codestack,0,ZTRUE);
	printf("--\n");


	type->category = FRAME; //it's done
	type->size = innerpctx->size;
/*
	if (issection){
		if (type->size != 0 ){

			type->word->target_pctx->frame = alloc_frame(innerpctx);
			printf("sectionVAR allocating shared data for type  %s %d bytes %p \n", type->word->name, innerpctx->size, type->word->target_pctx->frame);

		}

	}
*/

	debugf("done at %s\n", t->str);
	return t;
	
}
#endif

tokenT* parse_type_name(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	if (w->val_type == tType && w->val.as.ptr.address.type){

			valueT v = {0};
			v.as.ptr.address.type = w->val.as.ptr.address.type; //push the type that the word represents

			printf("pushing %s\n",  w->val.as.ptr.address.type->key );
		
			push_assembly(pctx, op_constant, 0, &v, tType, NULL, tType);
			//typestack_push_value(pctx, tType, &v);  //and that value we just pushed, is a type
			//t->type = tType;
			return t;
	}
	printf(" t is %s\n", t->str);
	errorf("Unknown type\n");
	errorf("%s\n", w->val_type->key);
	exit(1);
}

typeT* type_find_related(parsectxT* pctx, typeT* type, char* selection){



}


tokenT* parse_related_type(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	//typestack_pop(pctx, argc);
	instructionT* arg0 = pop_arg(pctx);

	typeT* type = arg0->val.as.ptr.address.type;

	int instflags=0;

	if (type){		//if the type is a constant, get it

		//type -> other
		if (!strcmp(t->str,".null")){
			push_assembly(pctx, op_constant, 0, NULL, NULL, assembly_args_mk(1, arg0) ,  type );
			return t;
		}

		//type -> type

		if (!strcmp(t->str, "&"))
			type = type_find(pctx, NULL, type, REFERENCE, 0, NULL);
		if (!strcmp(t->str, "*"))
			type = type_find(pctx, NULL, type, CPOINTER, 0, NULL);
		if (!strcmp(t->str, "$")){
			if (type->category != CPOINTER &&  type->category != ARRAY && type != tString && type != tAny && type->category!= SUBTREE ){
				//seterrorf(exe, ERROR_PARSE, "steward ($) only applies to array or string");
				PERROR(pctx, "steward ($) only applies to array, string or cpointer");
				return NULL;
			}
			type = type_find(pctx, NULL, type, STEWARD, 0, NULL);
		}
		if (!strcmp(t->str, "@")){
			if (type->is_wild){
				type = type_mk(NULL, NULL,type, DEREFERENCE,0, NULL);
				instflags = INST_FREE_VALUE; //the type created above is not attached to a pctx, so free it when done
			} else {
				type = type->ref;
			}
		}

		if (!strcmp(t->str, ".Instruction")){
			type = type_find(pctx, NULL, type, SUBTREE, 0, NULL);
		}

		if (!strcmp(t->str, "~")) //currently only makes sense to array
			type = type_find(pctx, NULL, type, ITERATED, 0, NULL);


	} else {
			// printf(" This requires a constant\n");
			exit(1);
	}

	valueT v = {0};
	v.as.ptr.address.type = type;
	//arg is the 'arglist' but op_constnat does not run its args.
	//the arglist is just to keep track of where the constant came from
	push_assembly(pctx, op_constant, instflags, &v, tType, assembly_args_mk(1, arg0) , tType);

	return t;
}

//parse_flow
tokenT* parse_flow(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	valueT vtrue = {0};
	vtrue.as.z32 = 1;

	if(!strcmp(t->str, "if")){

		char* ifstops[]={ "else", "end", "elseif", NULL};
		char* elsestops[]={ "end", NULL};

		int parts = 1; //the condition

		//first condition on stack

		instructionT* cond = NULL;

		do {
				cond = NULL;

				int depth = codestack_depth(pctx);
				t = tnext(t);  //skip over if/elseif
				t = parse(pctx, t, ifstops); //parse until end/if/elseif

				//true side instructions are on codestack

				if (!strcmp(t->str, "elseif")){
					//if got elseif pop the condition out
					cond = pop_arg(pctx);

				} else if (!strcmp(t->str, "else")){
					//else if just  elseif with constant true
					push_assembly(pctx,op_constant,0,&vtrue,tZ32, NULL, NULL);
					cond = pop_arg(pctx);
				}

				instructionT** trues = pop_args(pctx, codestack_depth(pctx) - depth); //code plus condition

				push_assembly(pctx, op_block, 0, NULL , NULL, trues, NULL);
				parts++;

				if (cond){
					push_subtree(pctx, cond);
					parts++;
				}

		} while(cond);

		instructionT** a = pop_args(pctx, parts);
		push_assembly(pctx, op_if,0,NULL, NULL, a, NULL);

		codestack_print(pctx->codestack,0, ZTRUE);

		return t;

	} else if(!strcmp(t->str, "loop")){

		instructionT** body = NULL;
		instructionT** step = NULL;

		char* loopstops[]= { "end", "step", NULL};
		char* stepstops[]= { "end",  NULL};
		int depth = codestack_depth(pctx);
		t=tnext(t);
		t = parse(pctx,t, loopstops);
		body = pop_args(pctx, codestack_depth(pctx) - depth);

		push_assembly(pctx, op_block, 0,NULL, NULL, body,NULL);


		if (!strcmp(t->str, "step")){
			depth = codestack_depth(pctx);
			t = tnext(t);
			t = parse(pctx,t, loopstops);
			step = pop_args(pctx, codestack_depth(pctx) - depth);
			push_assembly(pctx, op_block, 0, NULL, NULL, step, NULL);
		}

		instructionT** parts = NULL;

		if (step)
			parts = pop_args(pctx, 2);
		else
			parts = pop_args(pctx, 1);


		push_assembly(pctx, op_loop, 0,NULL, NULL, parts ,NULL);

		return t;
	}

	return NULL;
}

//parse until either end of input or any stop_tokens
//topparse
tokenT* parse(parsectxT* pctx, tokenT* t, char** stop_tokens){
	//skip past this stub
	if (t->tok == TOKEN_STARTFILE){
		t=tnext(t);
	}

	while(t){
		//print type stack
		/*
		printf(" Code Stack:\n");
		codestack_print(pctx->codestack,0, ZTRUE);
		printf("--\n");
		*/

		if (stop_tokens){
			char** s = stop_tokens;
			while(*s){
				if (!strcmp(*s, t->str)){
					return t;
				}
				s++;
			}
		}

		switch (t->tok){
			case TOKEN_STARTFILE:
				t=tnext(t);
				continue;

			case TOKEN_NUMBER:
				t = parse_number(pctx, NULL, t, 0);
				t=tnext(t);
				continue;

			case TOKEN_LITERAL:
				t = parse_string_literal(pctx, NULL, t, 0);
				t=tnext(t);
				continue;
		}

		// //try to match words through this and all parent contexts

		int argc=0;
		parsectxT* foundpctx= NULL;
		tracef(" LOOKING FOR '%s'\n", t->str);
		wordT* w = match_word(pctx, pctx,  t->str, MATCH_RECURSE_PCTX| MATCH_ALLOW_LIKE, &argc, &foundpctx);

		if (!w && t->str[0] == '.' && codestack_depth(pctx) >= 1  ){
			//if no match, and there is something on the stack, and it starts with a dotting
			//it might be a frame member

			zbool is_section = ZFALSE;
			zbool have_obj_pointer = ZFALSE;
			instructionT* inst_obj= pop_arg(pctx);

			typeT* ty = inst_obj->result_type; //type the obj on stack is

			if (ty->category == REFERENCE && ty->ref->category == FRAME) { //reference to a frame
				have_obj_pointer = ZTRUE;
				ty = ty->ref;
			} else  if (ty== tType && inst_obj->val_type == tType){
				ty = inst_obj->val.as.ptr.address.type; //or if it's a type itself
				tracef(" on %s  %p %d\n", ty->key, ty->word->target_pctx, ty->word->target_pctx->is_section);
				if (ty->word && ty->word->target_pctx && ty->word->target_pctx->is_section){
					is_section = ZTRUE;
					tracef("is section\n");
				}
			} else{
				ty= NULL;
				errorf(" UNHANDLED '.' case\n");
			}

			if(ty){
				//looking in the pointers' type's pctx, or the explicitly given type's pctx
				tracef(" looking for %s in %s (from %s) \n", t->str+1,ty->key, inst_obj->result_type->key);
				wordT* m = NULL;

				if(ty && ty->word ){
					m = match_word(pctx, ty->word->target_pctx, t->str+1,  MATCH_ALLOW_LIKE, &argc, &foundpctx);
				}

				if (m){
					tracef(" found %s  %s\n", m->name, m->type->key);
				}

				//allowed combinations:

				if (m && m->type&& ( m->type->category == PROC  || m->type==tType || m->opcode == op_constant || m->opcode == op_constantaddref) ){
					//don't need the actual 'type' on the stack
					if (!have_obj_pointer) {
						tracef(" omitting pointer\n");
						w = m;
					}
					ram_free(inst_obj);
					inst_obj = NULL;
				}

				if(m && m->type->category == VARIABLE){

					if (is_section){
						printf(" Need to get variable from inside singleton section\n");
						w=m;
						ram_free(inst_obj);
						inst_obj = NULL;
					} else if (have_obj_pointer){
						tracef("need to put obj pointer back on\n");
						//push_subtree(pctx, inst_obj);


						valueT v = {0};
						v.as.ptr.address.word = m;
						//add offset to the obj pointer
						push_assembly(pctx, op_subvar, 0, &v, tWord, assembly_args_mk(1, inst_obj), OF(pctx, REFERENCE, m->type->ref));
						inst_obj = NULL;


						t=autoload(pctx, m, t); //apply rules for &, =, etc
						t=tnext(t);
						continue;



					}
				}
			}
			if (inst_obj)
				push_subtree(pctx, inst_obj);
			inst_obj = NULL;
		}


		if (w) {
			tracef("found word %s %s   %s\n", w->name, w->type?w->type->key:"notype" , t->str  );

			if (w->restrict_pctx){
				if (pctx != w->restrict_pctx){
					errorf("Cannot access %s outside its original context\n", w->name);
					exit(1);
				}
			}

			if (!w->parse){
				errorf("No parse function for %s\n", w->name);
			} else {
				t = w->parse(pctx, w, t, argc);
			}

			if (ERR(pctx)){
				PRINTERR(pctx);
				return NULL;
			}

			if (!t){
					PERROR(pctx, "unexpected end of input\n");
					return NULL;
			}

			t=tnext(t);
			continue;


		}
		//did not match a word


		char* msg=zstrdup("");
		for (int i=0;i<zvec_count(pctx->codestack);i++){
			instructionT* inst = zvec_get_at(pctx->codestack, i);
			if (inst->result_type) {
				msg =  zstrprintf(msg, " %s;", inst->result_type->key );
			} else
				msg = zstrprintf(msg, ".");
		}

		PERROR(pctx, "UNDEFINED %s  from stack: %s\n", t->str, msg);
		ERRTOK(pctx, t);
		ram_free(msg);

		return NULL;

	}

	return NULL;
}
//types

zbool typecleanup(void* v){

		typeT* type = v;


		//debugf(" Freeing type %s\n", type->key);
		ram_free(type->ref);
		ram_free(type->key);
		ram_free(type->name);
		ram_free(type->argtypes);
		ram_free(type->opt_argnames);

		return ZTRUE;
}

char* type_key(char* name, typeT* ref, categoryE category, int size, typeT** args){
	char* key = NULL;

	if (category == LIKE){
		return zstrprintf(NULL, "like{%s}", name); //
	}

	//'function' types.. These don't have a name
	if (category == PROC){
		key = zstrdup("(");
		if (args) {

			for (int i=0; i<zarray_count(args);i++){
				key = zstrprintf(key, "%s%s", i>0?",":"", args[i]->key);
			}
		}

		tracef(" ref is %p\n", ref);
		if (ref)
			key = zstrprintf(key, "->%s)", ref->key);
		else
			key = zstrprintf(key, ")");

		return key;
		
	}

	//other types based on other types (do not have a direct name)
	if (ref){

		switch (category){

			case REFERENCE:
				return zstrprintf(NULL, "%s &", ref->key);

			case CPOINTER:
				return zstrprintf(NULL, "%s *", ref->key);

			case STEWARD:
				return zstrprintf(NULL, "%s $", ref->key);

			case DEREFERENCE:
				return zstrprintf(NULL, "%s @", ref->key);

			case VARIABLE:
				return zstrprintf(NULL, "%s var", ref->key);

			case ARG:
				return zstrprintf(NULL, "%s arg", ref->key);

			case ARRAY:
				return zstrprintf(NULL, " [%s]", ref->key);

			case SUBTREE:
				return zstrprintf(NULL, "%s .Instruction", ref->key);

		}

		//other cases not handled
		return zstrprintf(NULL, "{%s C%d.%d}", ref->key, category, size);
	}

	//types that just have a name
	if (name)
		return zstrdup(name);

	//these ones shouldn't happen:
	return zstrprintf(NULL, "bad/%d/%d", category, size);

}

int type_mk_line=0;


wordT* proc_parser_mk(parsectxT* pctx, char* name, typeT* proctype, void* parsefunc);

//create a type.  takes ownership of 'args' if passed
typeT* type_mk(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args ){
									
	printf(" CREATING TYPE %s\n", name);
	typeT* type = ram_alloc(sizeof(typeT), typecleanup); type_mk_line = __LINE__;

	type->name = zstrdup(name);

	type->ref = ram_addref(ref);
	if (ref)
		type->is_wild = type->ref->is_wild;	//anything based on wild type is wild

	type->category = category;

	type->size = size;

	if (type->category == LIKE)
		type->is_wild = ZTRUE;	//like types are wild

	if (type->category == ARRAY || type->category ==REFERENCE )
		type->size = sizeof(ptrT);

	if (type->category == STEWARD){
		type->size = ref->size;
	}

	if (type->category == CPOINTER || type->category==SUBTREE)
		type->size = sizeof(void*);

	type->key = type_key(name, ref, category, size, args);

	if (args){
		type->argc = zarray_count(args);
		type->argtypes = args;
	}
	
	if (pctx){
		typeT* oldtype =zstringmap_get(pctx->types, type->key);
		printf(" Looking for %s, got %s\n", type->key, oldtype?oldtype->key:"no");

		if (oldtype && oldtype->category == PENDING && category == PENDING){
			ram_free(type);
			type = oldtype;
			type->size = 0;  //reset size
		} else if (oldtype  ){
			errorf("!!!/There is already a type entry for %s\n", type->key);
			exit(1);
		}

		else{
			debugf(" putting type in %p -> pctx %d\n",type, pctx->id);
			zstringmap_put(pctx->types, type->key, type);
			//store it as a word
			if (name && strlen(name) > 0){
				debugf("Making word %s to push type\n", name);
				wordT* w = proc_parser_mk(pctx, name,  type_proc_mk( tType, 0), parse_type_name);//create a proc word, that runs parse_type_name at parse time
				type->word =w; //the type needs a word to find it
				w->val.as.ptr.address.type = type;
				w->val_type = tType; //is a type
			}
		}
	} else {tracef("creating type with no pctx\n");
	}

	return type;
}



typeT* type_find(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args){


	if (category == DEREFERENCE){

		if(ref && !ref->is_wild && ref->ref){
			//category is dereference
			//ref is the thing to be dereferenced
			//ref->ref is the dereference of it
			return ref->ref;
		} else {
			errorf(" Can't find dereference of wild or null  (for wild, need to resolve like type instead)\n");
			exit(1);
		}
	}

	//lookup type
	char* key = type_key(name, ref, category, size, args);

	typeT* type = zstringmap_get(pctx->types, key);
	ram_free(key);
	key = NULL;

	if (type) {
		debugf("Found existing type %s %p\n", type->key, type);
		//todo : free args?
		if (category != NAMED  && category != NAMEDEXISTING && category != type->category){
			errorf("Type is not expected category %d\n", category);
		}

		ram_free(args);
		return type;
	}


	if (category == NAMED){
		//wanted a type by name (like a struct), but the type is not yet
		//defined.
		return type_mk(pctx, name, NULL, PENDING, 0, NULL);

	}

	if (ref){
		return type_mk(pctx, NULL, ref, category, size, args);
	}

	//did not find anything

	return NULL;

}



//parse context
zbool cleanparsectx(void* v){
	parsectxT* pctx = v;
	tracef("cleaning pctx %d  frame %p\n", pctx->id, pctx->frame);
	ram_free(pctx->frame);

	ram_free(pctx->dictionary);
	ram_free(pctx->types);
	ram_free(pctx->codestack);

	for (int i=0;i<MAXRUNNERS;i++)
		ram_free(pctx->runners[i]);

	RESETERR(pctx); //clear any error messages

	ram_free(pctx->comment);


	return ZTRUE;
}

parsectxT* parsectx_mk(parsectxT* parent, char* name){
	static int debugid=0;

	parsectxT* pctx	= ram_alloc(sizeof(parsectxT), cleanparsectx);
	pctx->dictionary = zstringmap_mk(1024);
	pctx->types = zstringmap_mk(64);
	pctx->parent = parent;
	pctx->id = ++debugid;
	//pctx->cleanlist =zvec_mk(NULL, 8);
	//zvec_disown(pctx->cleanlist);
	pctx->comment = name? name: zstrdup("noname");

	debugf("creating pctx %d\n", debugid);
	return pctx;
}


//Makes a procedure word in a dictionary
wordT* proc_mk(parsectxT* pctx, char* name, typeT* proctype){

		if (!proctype)
			proctype = type_proc_mk(NULL, 0);  //if no type specified, pick a proc with no args and no return

		wordT* procword= word_mk(pctx, name, proctype);
		ram_free(proctype); //word above has a reference to it

		return procword;
}

//Makes a procedure word that runs an opcode. Most primitives are these.
wordT* proc_opcode_mk(parsectxT* pctx,  int opcode, char* name, typeT* proctype){
		wordT* w = proc_mk(pctx, name, proctype);
		if(ERR(pctx)) {
			return NULL;
		}
		w->opcode = opcode;
		w->comment = zstrdup(name);
		return w;
}

typeT* type_from_strf(parsectxT* pctx, char* str, int line);
#define type_from_str(PC, STR) type_from_strf(PC, STR, __LINE__)


wordT* proc_opcode_mk2(parsectxT* pctx,  int opcode, char* name, char* typestr){
	printf(" TRYING %s\n", typestr);
	typeT* proctype = type_from_str(pctx, typestr);
	wordT* w = proc_mk(pctx, name, proctype);
	if(ERR(pctx)) {
		return NULL;
	}
	w->opcode = opcode;
	w->comment = zstrdup(name);
	return w;
}

//Makes a procedure word that runs a C function in the parser.  The parser is made of these.
wordT* proc_parser_mk(parsectxT* pctx, char* name, typeT* proctype, void* parsefunc){
		wordT* w = proc_mk(pctx, name, proctype);
		if(ERR(pctx)) {
			return NULL;
		}
		w->parse = parsefunc;
		return w;
}

//Takes a string and produces the datatype it represents as a constant.  This is much easier than chaining together multiple lines of C building a data type from the actual structs.
//The caller of this function is responsible to free this typeT struct, or store it somewhere that will free it
typeT* type_from_strf(parsectxT* pctx, char* str, int line){
	//takes a string and makes a type
	zlistT tokens;
    zlist_init(&tokens);

    tokenT* first = token_mk(TOKEN_STARTFILE, NULL, 0);
    zlist_addhead(&tokens, &(first->zlistnode));

    tokenize(first, str, "type_from_str", line);

    parse(pctx, zlist_head(&tokens), NULL);

	//codestack_print(pctx->codestack, ZTRUE);
	instructionT* it = pop_arg(pctx);


	if (it->result_type != tType){
		errorf("is not a type: %s'n", str);
		exit(1);
	}
	typeT* ty = it->val.as.ptr.address.type;

	ram_addref(ty); //caller is responsible for freeing this pointer
	//two cases:  A. it owns the pointer, and it will free when it is freed. so we need to add a refcount
	//			  B. it does not own the pointer, so it won't free it.  But the caller will assume it will free it, so we need to add a refcount


	/*
	if (it->flags & INST_FREE_VALUE) {
		//the instruction OWNS the type and will free it. so blank it out here
		//the caller will be responsible to free it
		it->val.as.ptr.address.type = NULL;
	} else {
		//the instruction does NOT own the type.  Add a refcount to it
		//because the caller will also be responsible to free it
		ram_addref(ty);
	}
*/
	zlist_cleanup(&tokens);
	ram_free(it);
	return ty ;

}

//adds an instruction to the name table.  This just makes debugging easier
#define ADD_INST(NAME)	instruction_names[ op_ ## NAME ] = zstrdup(#NAME);


void leak_viewer(void* item, char* file, int line){

		if (line == type_mk_line){	//exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offsetaaabthis is
			printf("\t\tLeaked type: %s\n", ((typeT*)item)->key);
		}
		if (line == push_assembly_line){
			instructionT* inst = item;
	//		printf("\t\tLeaked instruction : %s\n",  inst->result_type->key  );
		}

}

int test_me(int a, int b ,  char* (*callback)(int a, int b)        ){
	printf("testme %d %d \n", a, b);
	printf("sending 111 222 to callback \n");
	char* cbr = callback(111,222);
	printf(" got %s\n", cbr?cbr:"none");

	return a*b;
}

short test_signed_short(short a, unsigned short b){
		printf("signed a is %d   unsigned b is %d \n", a, b);

		return -31222;
}

unsigned short test_unsigned_short(short a, unsigned short b){
		printf("signed a is %d   unsigned b is %d \n", a, b);

		return 65000;
}

//reflection

//type a instruction returns (on the runtime stack)
typeT* instruction_result_type( instructionT* inst){
	if (!inst)
		return NULL;
	return inst->result_type;
}

//type a instruction constant carries
typeT* instruction_val_typeof( instructionT* inst){
	if (!inst)
		return NULL;
	return inst->val_type;

}


int type_sizeof( typeT* t){
	if (!t)
		return 0;
	return t->size;
}

void* word_cfunc(wordT* w){
	if (w && w->ffi_caller)
		return w->ffi_caller->funcptr;

	return NULL;

}

instructionT** word_instructions(wordT* w){



	if (!w || !w->target_pctx || !w->target_pctx->codestack)
		return NULL;


	//TODO: it's annoying this is a zvec and not a zarray, but I don't want to change all that now

	instructionT** code = zarray_alloc(instructionT*,  zvec_count(w->target_pctx->codestack));

	for (int i=0;i< zvec_count(w->target_pctx->codestack);i++){
		code[i] = (zvec_get_at(w->target_pctx->codestack, i));
	}

	zarray_use(code, zvec_count(w->target_pctx->codestack));
	return code;

}



//instruction constant as a integer
int instruction_val_z32(instructionT* inst){
	if(!inst)
		return 0;

	if (inst->val_type == tZ32)
		return inst->val.as.z32;

	return 0;
}

//return an instuction's val, if it is a pointer
void* instruction_val_pointer(instructionT* inst){
	if (!inst)
		return NULL;

	if (!inst->val_type)
		return NULL;

	if (type_map_to_c (inst->val_type) == &ffi_type_pointer)
		return inst->val.as.ptr.address.block;

	return NULL;
}


void* instruction_val_checked_pointer(instructionT* inst, typeT* expected){
	if (!inst)
		return NULL;

	if (!inst->val_type)
		return NULL;

	if (type_map_to_c (inst->val_type) == &ffi_type_pointer) {
		
		if (type_equal( expected, inst->val_type)){
			return inst->val.as.ptr.address.block;
		}

	}

	return NULL;
}




int instruction_opcode(instructionT* inst){
	if (!inst)
		return 0;
	return inst->opcode;
}

char* instruction_opname(instructionT* inst){
	if (!inst)
		return "null";

	char* name = "invalid";

	if (inst->opcode < op_MAX){
		name = instruction_names[inst->opcode];

		if (!name){
			name = zstrprintf(NULL, "op_%d", inst->opcode);
			instruction_names[inst->opcode] = name;

		}

	}

	return name;

}


instructionT** instruction_args(instructionT* inst){
	//printf("iargs from %p\n", inst);
	if (!inst)
		return 0;
	return inst->args;
}

char* word_name(wordT* w){
	if (!w)
		return 0;

	return
		w->name;
}



typeT* word_type (wordT* w){
	if (!w)
		return 0;

	return
		w->type;;
}

char* type_keystr (typeT* t){
	if (!t)
		return NULL;

	return t->key;
}

//return type of a function type OR what an array or pointer type is made of
typeT* type_ref(typeT* t){
	if (!t)
		return NULL;

	return t->ref;
}



typeT** type_proc_args(typeT* t){
	if (!t)
		return NULL;

	printf(" returning %d argtypes\n", zarray_size(t->argtypes));

	return ram_addref(t->argtypes);
}

char** type_proc_argnames(typeT* t){
	if (!t || !t->opt_argnames)
		return NULL;

	//todo: argnames is not a zarray, but a detatched C array from zvector.
	//      I could make zvector use zarray for its pointer array so it can be compatible

	//should be same length, if eixsting
	char** argnames = zarray_alloc( char*, zarray_size(t->argtypes));

	memcpy(argnames, t->opt_argnames, sizeof(char*) * zarray_size(t->argtypes));

	printf(" returning %d argnames\n", zarray_size(argnames));

	zarray_use(argnames, zarray_size(argnames));

	return argnames;
}

//tokenize and parse (evnetually... run)
void int_run_str(char* src, char* filename){
	instruction_names = zarray_allocd( char*, op_MAX, zarray_destruct_pointers );
	zarray_use(instruction_names, op_MAX);
	ADD_INST(constant);
	ADD_INST(load);
	ADD_INST(take);
	ADD_INST(loadaddref);
	ADD_INST(subvar);
	ADD_INST(store);
	ADD_INST(trashstore);
	ADD_INST(print32);
	ADD_INST(printptr);
	ADD_INST(add32);
	ADD_INST(call);
	ADD_INST(return);
	ADD_INST(returnval);
	ADD_INST(argpick);
	ADD_INST(argtake);
	ADD_INST(argaddref);
	ADD_INST(if);
	ADD_INST(loop);
	ADD_INST(block);
	ADD_INST(break);
	ADD_INST(continue);
	ADD_INST(switch_jump);
	ADD_INST(switch_jumpfalse);
	ADD_INST(arrayindex);
	ADD_INST(sys);
	ADD_INST(sectionvar);
	ADD_INST(mul32);
	ADD_INST(sub32);
	ADD_INST(over)
	ADD_INST(dup)
	ADD_INST(dups)
	ADD_INST(drop)
	ADD_INST(adim);
	ADD_INST(switch_indexplus);
	ADD_INST(indexplusstore);

	//C constants



	//C functions
	int_add_c_object("test_me" , test_me);
	int_add_c_object("test_signed_short", test_signed_short);
	int_add_c_object("test_unsigned_short", test_unsigned_short);


	int_add_c_object("instruction_opcode", instruction_opcode);
	int_add_c_object("instruction_opname", instruction_opname);

	int_add_c_object("instruction_args", instruction_args);
	int_add_c_object("instruction_result_type", instruction_result_type);
	int_add_c_object("type_keystr", type_keystr);
	int_add_c_object("instruction_val_pointer", instruction_val_pointer);
	int_add_c_object("instruction_val_z32", instruction_val_z32);
	int_add_c_object("instruction_val_typeof", instruction_val_typeof);
	int_add_c_object("type_sizeof", type_sizeof);

	int_add_c_object("type_proc_args", type_proc_args);
	int_add_c_object("type_proc_argnames", type_proc_argnames);
	int_add_c_object("type_ref", type_ref);

	int_add_c_object("word_cfunc", word_cfunc);
	int_add_c_object("word_type", word_type);
	int_add_c_object("word_name", word_name);
	int_add_c_object("word_instructions", word_instructions);

	//add the string functions I already have to this
	int_add_c_object("zstrndup" , zstrndup);
	int_add_c_object("zstrdup2" , zstrdup2);
	int_add_c_object("zstrcatsub" , zstrcatsub);
	int_add_c_object("zstrcmp" , zstrcmp);


	//debug stuff
	int_add_c_object("ram_numrefs" , ram_numrefs);

	parsectxT* pctx= parsectx_mk(NULL, zstrdup("global"));
	pctx->is_section = ZTRUE; //global is just the top-level section


	//create basic types
	tType = type_mk(pctx, "Type", NULL, OPAQUE, sizeof(typeT*), NULL);
	tType->word->val_type = tType;

	tZ32  = type_mk(pctx, "Z32",  NULL, SIMPLE, sizeof(zint32), NULL);
	tReal = type_mk(pctx, "Real", NULL, SIMPLE, sizeof(FLOAT),  NULL);
	tBit = type_mk(pctx, "Bit", NULL, SIMPLE, sizeof(int),  NULL);
	tWord = type_mk(pctx, "Word", NULL, OPAQUE, sizeof(wordT*),  NULL);
	tAny =  type_mk(pctx, "Any", NULL, OPAQUE, 0, NULL);  //'Any' does not have a size (but any& does)

	tString  = type_mk(pctx, "String",  NULL, SIMPLE, sizeof(char*), NULL);
	tByte  = type_mk(pctx, "Byte",  NULL, SIMPLE, 1, NULL);
	tStringByte  = type_mk(pctx, "StringByte",  NULL, SIMPLE, 1, NULL);

	//These types are internaly, 32-bit, and really only exist on the stack
	//Size32 and Long32 can be used interchangable with zint32
	//Double32 can be used interchangable with Real.
	//They exist for callng C functions that expect or return 64-bit values.  They are converted to/from 64-bit when calling or returning from C
	//These types should only be used in C sys definitions.
	tSize32 = type_mk(pctx, "Size32",  NULL, SIMPLE, sizeof(tZ32), NULL);
	tLong32 = type_mk(pctx, "Long32",  NULL, SIMPLE, sizeof(tZ32), NULL);
	tDouble32 = type_mk(pctx, "Double32",  NULL, SIMPLE, sizeof(tReal), NULL);
	//tZ16: store and save to 16-bit memory locations, or pass/return from functions.  But will be Z32 on the stack.
	tZL16 = type_mk(pctx, "ZL16", NULL, SIMPLE, 2, NULL);
	tUL16 = type_mk(pctx, "UL16", NULL, SIMPLE, 2, NULL);

	//create parser words that work in type constants
	proc_parser_mk(pctx, "(", NULL, parse_type_list); //handle argument lists for functions
	proc_parser_mk(pctx, "&", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into its reference type
	proc_parser_mk(pctx, "$", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into a steward
	proc_parser_mk(pctx, "@", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into its dereferenced type
	proc_parser_mk(pctx, "*", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into a c pointer
	proc_parser_mk(pctx, ".null", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into 0-value.  Z32.null is zero, String.null is null pointer of string type, etc
	proc_parser_mk(pctx, ".Instruction", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into an instruction that produces this type(  Z32.Instruction is an instruction that returns a Z32)
	proc_parser_mk(pctx, "~", type_proc_mk(tType, 1, tType), parse_related_type); //makes an array type into an iterated

	proc_parser_mk(pctx, "[", NULL, parse_array); //create array type [Z32] for example
	proc_parser_mk(pctx, "$[", NULL, parse_array_values); //create an array with values  '[10 20]

	proc_parser_mk(pctx, "=[", type_from_str(pctx,"(Any&)") , parse_fill_frame );

	//create control structures
	proc_parser_mk(pctx, "if", type_proc_mk(NULL, 1, tZ32), parse_flow); //parse flow structures
	proc_parser_mk(pctx, "if", type_proc_mk(NULL, 1, tBit), parse_flow); //parse flow structures
	proc_parser_mk(pctx, "loop", NULL, parse_flow); //parse flow structures

	//define parser words for variables, types and procs
	proc_parser_mk(pctx, "var",  type_proc_mk(NULL, 1, tType), parse_var); //makes a variable in current context
	proc_parser_mk(pctx, ":",  type_proc_mk(NULL, 1, tAny), parse_var); //makes a variable in current context of the same type as top of stack, and stores the value
	proc_parser_mk(pctx, "::",  type_proc_mk(NULL, 1, tAny), parse_var); //makes a variable in current context of the same type as top of stack, and stores copy or addref of the value

	proc_parser_mk(pctx, "constant",  type_proc_mk(NULL, 1, tAny), parse_constant); //makes a const

	proc_parser_mk(pctx, "type", type_proc_mk(NULL, 0), parse_frame);
	proc_parser_mk(pctx, "section", type_proc_mk(NULL, 0), parse_frame);
	proc_parser_mk(pctx, "proc", type_proc_mk(NULL, 1, tType), parse_proc);
	proc_parser_mk(pctx, "proto", type_proc_mk(NULL, 1, tType), parse_proc);
	proc_parser_mk(pctx, "expose", type_proc_mk(NULL, 1, tType), parse_proc);

	proc_parser_mk(pctx, "sys", type_proc_mk(NULL, 3, tType , tString, tString), parse_proc);//import ffi
	proc_parser_mk(pctx, "sys", type_proc_mk(NULL, 2, tType , tString), parse_c_constant);
//	proc_parser_mk(pctx, "sys", type_proc_mk(NULL, 2, tAny , tType), parse_c_constant);

	proc_parser_mk(pctx, "sys", type_proc_mk(NULL, 2, tType , OF(pctx, CPOINTER, tAny)), parse_proc);

	proc_parser_mk(pctx, ">-",  type_from_str(pctx,"(Any->Any.Instruction)") , parse_get_subtree);



	//add some casts
	proc_opcode_mk2(pctx, op_nop, ".Bit", "(Z32->Bit)");
	proc_opcode_mk2(pctx, op_nop, ".Z32", "(Bit->Z32)");







	proc_parser_mk(pctx, "include", type_proc_mk(NULL, 1, tString), parse_include);

	//create arrays
	//dim requires a variable exists to populate the array
	proc_opcode_mk2(pctx, op_dim, "dim", "([Any]$&:array Z32:len Z32:capacity   )");



	wordT* arrayindex = proc_opcode_mk2(pctx, op_arrayindex, "[]", "( [Any]:a Z32:idx -> like a@&)");
	arrayindex->autoload = ZTRUE;


	wordT* carrayindex = proc_opcode_mk2(pctx, op_carrayindex, "[]", "( Any*:a Z32:idx -> like a)");
	carrayindex->autoload = ZTRUE;



	proc_opcode_mk2(pctx, op_indexplusstore, "++", "([Any]$:a  like a@@:b-> like a)");

	wordT* arraycow = proc_opcode_mk2(pctx, op_arraycow, ".Byte", "( String$:a -> [Byte]$)");
	arraycow->val.as.z32 = 1;

	wordT* arraycow2 = proc_opcode_mk2(pctx, op_arraycow, ".String", "( [Byte]$:a -> String$)");
	arraycow2->val.as.z32 = 1;

	proc_opcode_mk2(pctx, op_nop, ".String", "( [Byte]:a -> String)");  //use a byte array as a string, for printing and stuff
	proc_opcode_mk2(pctx, op_arrayinfo, ".count", "([Any]:a -> Z32)");

	wordT* arraysize = proc_opcode_mk2(pctx, op_arrayinfo, ".size", "([Any]:a -> Z32)");
	arraysize->val.as.z32=1;

	arraysize = proc_opcode_mk2(pctx, op_arrayinfo, ".size1", "([Any]:a -> Z32)");
	arraysize->val.as.z32=2;

	//set count of array
	proc_opcode_mk2(pctx, op_arraysetcount, ".setcount", "([Any]:a  Z32:count)");



	//reading a byte from a string returns a StringByte& instead of a Byte&.  StringByte is intentionally is missing a write word
	arrayindex = proc_opcode_mk2(pctx, op_arrayindex, "[]", "( String:a Z32:idx -> StringByte&)");
	arrayindex->autoload = ZTRUE;
	proc_opcode_mk2(pctx, op_load, "@",  "(StringByte& -> Z32)" );


	proc_opcode_mk2(pctx, op_load, "@",  "(Byte& -> Z32)" );
	proc_opcode_mk2(pctx, op_store, "=",  "(Z32:value  Byte&:dst)" );



	proc_opcode_mk2(pctx, op_load, "@",  "(Real& -> Real)" );
	proc_opcode_mk2(pctx, op_store, "=",  "(Real:value  Real&:dst)" );


	proc_opcode_mk2(pctx, op_break, "break", "()");
	proc_opcode_mk2(pctx, op_continue, "continue", "()");

	proc_opcode_mk2(pctx, op_load, "@",  "(ZL16& -> Z32)" );
	proc_opcode_mk2(pctx, op_store, "=",  "(Z32:value  ZL16&:dst)" );

	//create primitives. minimal at the moment
	//these use the above specified parser

	proc_opcode_mk2(pctx, op_printchar, "printchar", "(Z32)");
	proc_opcode_mk2(pctx, op_getchar, "getchar", "(->Z32)");

	proc_opcode_mk2(pctx, op_print32, "print", "(Z32)");

	proc_opcode_mk2(pctx, op_add32, "+", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(pctx, op_sub32, "-", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(pctx, op_mul32, "*", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(pctx, op_div32, "/", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(pctx, op_mod32, "mod", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(pctx, op_less32, "<", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(pctx, op_greater32, ">", "(Z32:a Z32:b -> Bit)");
	proc_opcode_mk2(pctx, op_equal32, "==", "(Z32:a Z32:b -> Bit)");

	proc_opcode_mk2(pctx, op_ptrequal, "==", "(Type:a Type:b -> Bit)");

	proc_opcode_mk2(pctx, op_neg32, ".-", "(Z32:a  -> Z32)");
	proc_opcode_mk2(pctx, op_bnot, "not", "(Bit:a  -> Bit)");
	proc_opcode_mk2(pctx, op_bnot, "not", "(Z32:a  -> Bit)");
	proc_opcode_mk2(pctx, op_ptrvalid, "?", "(String:s -> Bit  )");
	proc_opcode_mk2(pctx, op_ptrvalid, "?", "(Type:s -> Bit  )");
	proc_opcode_mk2(pctx, op_ptrvalid, "?", "(Any*:s -> Bit  )");
	proc_opcode_mk2(pctx, op_ptrvalid, "?", "(Any&:s -> Bit  )");
	proc_opcode_mk2(pctx, op_ptrvalid, "?", "([Any]:s -> Bit  )");
	proc_opcode_mk2(pctx, op_ptrvalid, "?", "([Any]$:s -> Bit  )");

	proc_opcode_mk2(pctx, op_printptr, "printptr", "(Any)");
	proc_opcode_mk2(pctx, op_load, "@",  "(Z32&:a  -> like a@ )" );
	proc_opcode_mk2(pctx, op_store, "=",  "(Z32:value  like value&:dst)" );

	//store/load cpointers
	proc_opcode_mk2(pctx, op_store, "=",  "(Any*:value  like value&:dst)" );
	proc_opcode_mk2(pctx, op_load, "@",  "(Any*&:a  -> like a@ )" );


	//array specific
	proc_opcode_mk2(pctx, op_load, "@",  "([Any]&:a  -> like a@ )" );


	//array specific steward rules
	//proc_opcode_mk2(pctx, op_loadaddref, "$",  "([Any]$&:a  -> like a@ )" );
	//proc_opcode_mk2(pctx, op_take, "$$",  "([Any]$&:a  -> like a@ )" );
	//proc_opcode_mk2(pctx, op_trash, "trash",  "([Any]$:a)" );
	//proc_opcode_mk2(pctx, op_trashstore, "=",  "([Any]$:value  like value&:dst)" );

	proc_opcode_mk2(pctx, op_load, "@",  "(String&:a  -> like a@ )" );
	proc_opcode_mk2(pctx, op_load, "@",  "(Type&:a  -> like a@ )" );


	//switch to generic steward rules
	proc_opcode_mk2(pctx, op_loadaddref, "$",  "(Any$&:a  -> like a@ )" );
	proc_opcode_mk2(pctx, op_take, "$$",  "(Any$&:a  -> like a@ )" );
	proc_opcode_mk2(pctx, op_trash, "trash",  "(Any$:a)" );
	proc_opcode_mk2(pctx, op_trashstore, "=",  "(Any$:value  like value&:dst)" );


	proc_opcode_mk2(pctx, op_load, "@",  "(Any*:a -> like a@)" );



	//the following, when applied to a C pointer, fetches the pointer from the variable AND clears the variable.
	//usage is for 'closing' or 'freeing' something in C, and making sure you null out the pointer
	//  someFileHandle$$ fclose
	// instead of having to fo      someFileHandle fclose   null someFileHandle=
	proc_opcode_mk2(pctx, op_take, "$$",  "(Any*&:a  -> like a@ )" );

		proc_opcode_mk2(pctx, op_load, "@",  "(Any.Instruction&:a -> like a@)" );
	proc_opcode_mk2(pctx, op_printstr, "print", "(String)");

	//proc_opcode_mk2(pctx, op_trashstore, "=",  "(String$:value  like value&:dst)" );
	//proc_opcode_mk2(pctx, op_load, "@",  "(String&:a  -> like a@ )" );
	//proc_opcode_mk2(pctx, op_loadaddref, "$",  "(String$&:a  -> like a@ )" );
	//proc_opcode_mk2(pctx, op_take, "$$",  "(String$&:a  -> like a@ )" );
	//proc_opcode_mk2(pctx, op_trash, "trash",  "(String$:a)" );
	//proc_opcode_mk2(pctx, 3232, "=", "(Z32 Z32&)");
	//proc_opcode_mk2(pctx, 888, "@",  "(Z32&  -> Z32 )" );


	//float
	proc_opcode_mk2(pctx, op_fadd, "+", "(Real:a Real:b->Real)");
	proc_opcode_mk2(pctx, op_fsub, "-", "(Real:a Real:b->Real)");
	proc_opcode_mk2(pctx, op_fmul, "*", "(Real:a Real:b->Real)");
	proc_opcode_mk2(pctx, op_fdiv, "/", "(Real:a Real:b->Real)");
	proc_opcode_mk2(pctx, op_fpow, "^", "(Real:a Real:b->Real)");


	//turn an array or reference into a cpointer

	proc_opcode_mk2(pctx, op_nop, ".cptr", "(Any&:a->like a@*)");
	proc_opcode_mk2(pctx, op_nop, ".cptr", "([Any]:a->like a@*)");


    zlistT tokens;
    zlist_init(&tokens);

    tokenT* first = token_mk(TOKEN_STARTFILE, NULL, 0);
    zlist_addhead(&tokens, &(first->zlistnode));

    tokenize(first, src, filename, 1);




    parse(pctx, zlist_head(&tokens), NULL);

	codestack_print(pctx->codestack,0, ZTRUE);

	PRINTERR(pctx); //print error, if any

	//dump_types(pctx);
	//dump_dictionary(pctx);

	exectxT* exe = exectx_mk();

	if(OK(pctx)){
		runnerI* runme = compile_for_switch(exe, pctx);
		//getc(stdin);

		debugf(" to Alloc %d global space\n", pctx->size);
		runme->execute(exe, runme, 0);

		//debugf(" free globals\n");
	//	ram_free(exe->globals);


	//	ram_free(runme); //runner is part of the pctx, and is freed there
	}

	RESETERR(pctx);

    zlist_cleanup(&tokens);
	ram_free(pctx);
	ram_free(exe);
	ram_free(instruction_names);
	ram_free(c_objects);
	ram_free(c_constants);
	c_objects=NULL;
	abyss = leak_viewer;

}
