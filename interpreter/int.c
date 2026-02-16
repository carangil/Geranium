#include "int.h"
#include "string.h"
#include "stdarg.h"
#include "zmem.h"
#include "zarray.h"

//error reporting
void set_error(exectxT* exectx, int code, char* string){
	if (!exectx){
		fprintf(stderr, "Error %d: %s",code, string);
		ram_free(string);

	}
	exectx->error_code = code;
	exectx->error_string = string;
}


char* echo(char* s){
	fprintf(stderr, "SETERROR %s\n",s);
	exit(1);
	return s;
}

#define seterrorf(EXECTX, CODE, ...)  set_error(EXECTX, CODE, echo(zstrprintf( (EXECTX)->error_string, __VA_ARGS__)))


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
	printf(" TOKEN %d   %.*s\n", tok,  str? len: 5, str?str:"nostr");
	if (str && len)
		t->str = zstrndup(str, len);
	else if (str)
		t->str = zstrndup(str, ZSTRING_ALL);
	return t;

}

tokenT* int_insert_tokenf(tokenT* A, tokenT* B, zbool before) {

	if (before)
		return zlist_insert_node_before(&A->zlistnode, &B->zlistnode);
	else
		zlist_insert_node_after(&A->zlistnode, &B->zlistnode);

}

//used to recognize 2-letter combinations like ->, etc
zuint32 find_pair(char* patterns, char a, char b){
	for(  ;*patterns;patterns+=2){
		if ( ((*patterns)==a) &&(*(patterns+1)==b))
			return TOKEN_PAIR(a,b) ;
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
		if ((p = find_pair("$$.&.%.@--++==->/**///[]>=<=!=.-###=\\\\/\\\\/", c, next))){
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

		if (!name || (*in == '.') ){
			digits  = accept_patterns(in,
					"-.0123456789", //start with digit or decimal point
					"e-E-e+E+",  //- and + only accepted after an e or E
					"0123456789.eE"); //continues with digits, decimal point, hex letters, type suffix letters

			//special case: if number starts with '-', but has only 1 character, this isn't a negative number, but just a minus sign
			if ((digits == 1) && (in[0] == '-'))
				digits = 0;

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

		zlist_insert_node_after(&insert->zlistnode,&t->zlistnode);
		insert = t;
		in++;
	}
	ram_free(fnamecopy);
}


//execution context
zbool clean_exe(void* v){
	exectxT* exe = v;
	ram_free(exe->error_string);
	ram_free(exe->stack);
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
	return ZTRUE;
}

//creates a word.  If owntype no reference is added to type, so that freeing the word frees the type.

wordT* word_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* type){

	wordT* w = ram_alloc(sizeof(wordT), word_clean);
	
	w->name = zstrdup(name); //note: null string will give a valid empty string non-null result

	w->type = ram_addref(type);
	
	if (pctx){
		//add to the dictionary
		zvecT* p =  zstringmap_get(pctx->dictionary, name);
		if (!p){
				p = zvec_mk(NULL, 10);	//make new vector
				zstringmap_put(pctx->dictionary, name, p);
		}

		//before adding, make sure there isn't another word with the same name (0 args) or same proc type
		for (int i=0;i<zvec_count(p); i++){

			wordT* cw = zvec_get_at(p, i);
			if (cw->type->argc == 0 && w->type->argc ==0){
				seterrorf(exe, ERROR_PARSE, "Creating second word with no args in same pctx\n");
				ram_free(w);
				return NULL;
			}
			//if both same type (proc) then also error
			//todo:might have more  conflicts, not known yet
			if (cw->type == w->type){ //todo use cmp_type with 'no wildcards/subst'
				seterrorf(exe, ERROR_PARSE, "Creating second word with same type\n");
				ram_free(w);
				return NULL;
			}

		}

		zvec_add(p, w);

		if (type->category == VARIABLE){
			//todo: deal with alignment
			w->offset = pctx->size;
			printf(" made word %s  %s  %d\n", name, type->key, type->ref->size);
			pctx->size += type->ref->size;
			//exit(1);
		}


	}else{
		seterrorf(exe, ERROR_PARSE,  "creating word with no name\n");
		return NULL;

	}

	return w;
}


wordT* word_alias_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* newtype, wordT* original){
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
	void*	cursor=NULL;
	int i;

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
	void*	cursor=NULL;
	printf(" types:\n");
	while (zstringmap_nextkey(pctx->types, &name, &type, &cursor)) {

		type_print(type);
		printf("\n");

	}
}

//most basic types
typeT *tZ32, *tType, *tReal, *tType, *tFloat, *tBit, *tWord;


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


typeT* resolve_like_type_for_caller(parsectxT* pctx, typeT* expected, typeT* proctype, typeT** stacktypes, int flags){

	if (!expected->is_wild){
		return expected;
	}

	if (expected->category == LIKE){
		int argn = expected->argc;	//expected is 'like' the argnth term

		//the proc's nth type
		typeT* proc_n = proctype->argtypes[ expected->argc]; //what type the proc expects for args

		//the stack's nth type
		typeT* passed_n = stacktypes[expected->argc];



		printf(" %s is %dth, is %s\n", expected->key, expected->argc, passed_n->key);

		//getc(stdin);
		return passed_n;

	}

	printf(" case of %s\n", expected->key);
	//not a like, but is some other wild
	if (expected->ref){
		typeT* inner = resolve_like_type_for_caller(pctx, expected->ref, proctype, stacktypes, flags);
		printf(" got inner type %s for %s  want category %d\n", inner->key, expected->key, expected->category);

		typeT* newexpected = type_find(pctx, NULL, inner, expected->category,0 , NULL);
		printf("  inner type %s  re-wrapped as %s\n", inner->key, newexpected->key);

		return newexpected;


	}

	errorf("BAD LIKETYPE CASE\n");
	exit(1);


	return expected;
}



//Compare two types to see if they are compatible/equivalent

zbool type_cmp(parsectxT* pctx, typeT* expected, typeT* given, int flags, typeT* proctype, typeT** stackargs){


	if (expected->is_wild ){

		if (!(flags&MATCH_ALLOW_LIKE)){
			return ZFALSE;
		}

		if (proctype && stackargs){
			typeT* resolved = resolve_like_type_for_caller(pctx, expected, proctype, stackargs, flags);
			printf(" %s resolved to %s\n", expected->key, resolved->key);
			expected = resolved;
		} else {
			printf("Can't use like type here\n");
			exit(1);
		}

	}



	if (expected == given)
		return ZTRUE;

	if (expected->ref && given->ref && expected->category == given->category){
		return type_cmp(pctx, expected->ref, given->ref, flags, proctype, stackargs);
	}

	if (expected==tAny && (flags&MATCH_ALLOW_LIKE)) //tAny matching anything
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



	indent(level);
	char* name = NULL;
	if (inst->opcode < op_MAX)
		name = instruction_names[inst->opcode];

	if (name)
		printf("%s ", name);
	else
		printf("unnamed(%d) ", inst->opcode);


	if (inst->val_type) {
		if( inst->val_type == tType)
			printf( " %s:(%s) ", inst->val.as.ptr.address.type->key, inst->val_type->key);
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

	if (recurse && inst->args){
		indent(level); printf("{\n");
		for (int i=0; i<zarray_count(inst->args); i++){
			instruction_print(inst->args[i], level+1, recurse);
		}
		indent(level); printf("} ");
	}


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
			instruction_print(inst, 0, recurse);
	}

}

void codestack_print(zvecT* cs,  int start, zbool recurse){

	if (!cs)
		return ;

	for (int i=start; i < zvec_count(cs); i++){
		instructionT* inst = zvec_get_at(cs,i);

		if (!inst)
			printf("(null code)\n");
		else
			instruction_print(inst, 0, recurse);


	}


}
//pops a list of instructions off the stack
instructionT* pop_arg(parsectxT* pctx){
	return zvec_remove_last(pctx->codestack);
}

instructionT** pop_args(parsectxT* pctx, int n){
	if (n==0)
		return NULL;


	//pop right to left
	instructionT** a = zarray_alloc(instructionT*, n);
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

	typeT** types = zarray_alloc(typeT*, zarray_count(a) );
	for (int i=0;i<zarray_count(a);i++){
		if (a[i]->result_type == tType){
			types[i] = a[i]->val.as.ptr.address.type;
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
	instructionT** a = zarray_alloc( instructionT*, n);
	printf("%p  %d  args ... \n\n\n",a, n);

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
	if(inst->args){
		for (i=0;i<zarray_count(inst->args); i++){
			ram_free(inst->args[i]);
		}
		ram_free(inst->args);
	}
	ram_free(inst->comment);

	if (inst->flags & INST_FREE_VALUE){
		printf(" FREE INST VAL  %p on inst %p\n", inst->val.as.ptr.address.bytes, inst);

		ram_free(inst->val.as.ptr.address.bytes);
	}

	return ZTRUE;
}

//returns an array item representing one instruction.
instructionT* push_assembly(parsectxT* pctx, int opcode, int flags, valueT* pval, typeT* val_type,  instructionT** args , typeT* ret){

	if (!pctx->codestack){
		pctx->codestack = zvec_mk(NULL, 8);
	}

	instructionT* inst = ram_alloc(sizeof(instructionT), clean_inst);


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


int DEBUG_MATCH=1;
//tests of a word matches the current typestack of pctx

zbool test_word(parsectxT* pctx, typeT** stacktypes, wordT* word, int flags, int argc){


	if (argc != word->type->argc){ 
		//debugf("Skipping mismatch arg count %d %d\n", argc, word->type->argc);
		return ZFALSE;
	}

	if (DEBUG_MATCH){
		debugf("test %s %s: to ", word->name, word->type->key);
		for (int i=0;i<argc;i++){
			printf("%s ", stacktypes[i]? stacktypes[i]->key:"novalue ");
		}
		printf(".\n");
	}


	//argless words	
	if (argc==0 && word->type->argc==0){
	//	debugf("Word %s has no members, name matches, returning it\n", word->name);
		return ZTRUE;
	}

	//try to match args	
	void* csr=NULL;
	for (int i=0;i<argc;i++){

		typeT* passed_type = stacktypes[i];

		if (!passed_type){
				errorf(" No result type!\n");
				return ZFALSE;
		}


		typeT* argtype = word->type->argtypes[i];
	//	debugf("Compare sp%d  %s s %s\n", i, argtype->key,  arginst->result_type->key);



		if (!type_cmp( pctx, argtype, passed_type, flags, word->type, stacktypes)){
			printf("flags %x arg %d    %p %s vs %p %s \n", flags, i, argtype, argtype->key, passed_type, passed_type->key);
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

switchopT* compile_switch_subtree(exectxT* exe, switchrunnerT* sw, switchopT* prog, instructionT* inst){

	zbool skipargs = ZFALSE;

	if (inst->opcode == op_constant || inst->opcode == op_nop){
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

	if (inst->opcode == op_block) {
		return prog;
	}

	if (inst->opcode== op_dim){
		valueT v={0};
		v.as.ptr.address.type=inst->args[0]->result_type->ref;
		printf(" compiling DIM for %s\b", v.as.ptr.address.type->key);
		getc(stdin);
		prog = switch_asm(prog,inst->opcode, tType, &v, inst);
		return prog;
	}

	if (inst->opcode== op_arrayindex){
		//put size in
		prog = switch_asmi(prog,inst->opcode,  inst->result_type->ref->size, inst);
		return prog;
	}

	if ((inst->opcode==op_globalvar)||(inst->opcode==op_subvar)||(inst->opcode==op_localvar)){
		wordT* var = inst->val.as.ptr.address.word;
		prog = switch_asmi(prog, inst->opcode,var->offset, inst);
		return prog;
	}

	if ((inst->opcode==op_load)){
		printf("compile load %d bytes from %s\n", inst->result_type->size,inst->result_type->key );
		prog = switch_asmi(prog, inst->opcode, inst->result_type->size, inst);
		return prog;
	}

	if (inst->opcode==op_store){
		printf("compile store %d bytes\n", inst->args[0]->result_type->size);
		prog = switch_asmi(prog, inst->opcode, inst->args[0]->result_type->size, inst);
		return prog;
	}

	if (inst->opcode == op_call){
		//need to compile the func

		if (inst->val_type == tWord){

			wordT* word = inst->val.as.ptr.address.word;

			//printf("To compile switch:%s:%s\n", word->name, word->type->key);

			compile_for_switch(exe, word->target_pctx);

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

			else
				printf("%d", prog[i].imm.as.z32);

			printf(":%s", prog[i].immtype->key);

		}

		printf("\n");

	}

}

//void exec_func(exectxT* exe, switchrunnerT* sw

void run_switch(exectxT* exe, runnerI* r, int start){

	switchrunnerT* sw = (switchrunnerT*) r;

	switchopT* pc = sw->prog + start;

	for(;;){

		printf("sp:%d|bp:%d|", exe->sp, exe->bp);

		for (int i=0;i<exe->sp;i++){

			printf("%d:%lld+%d|", i, (long long) (exe->stack[i].as.ptr.address.bytes)  ,  exe->stack[i].as.ptr.offset );

		}
		printf("%d:<>\n", exe->sp);


		switch (pc->opcode) {

			case op_constant:  exe->stack[exe->sp++] = pc->imm;  pc++; continue;
			case op_print32:   printf("print32: %d\n", exe->stack[--exe->sp].as.z32); pc++;
			getc(stdin); continue;

			case op_printptr:   printf("printptr:%p+%x\n", exe->stack[exe->sp-1].as.ptr.address.bytes, exe->stack[exe->sp-1].as.ptr.offset); pc++; exe->sp--;

			getc(stdin); continue;

			case op_add32:     exe->stack[exe->sp-2].as.u32 += exe->stack[exe->sp-1].as.u32; exe->sp--; pc++; continue;
			case op_argpick:   exe->stack[exe->sp++] = exe->stack[exe->bp+pc->imm.as.z32];  pc++; continue;

			case op_load: //TODO check this and also make sure the imm value is set (its not, fix it on switch compiler)
				printf(" load from %p:%d (%p)\n", exe->stack[exe->sp-1].as.ptr.address.bytes,exe->stack[exe->sp-1].as.ptr.offset, exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);

				memcpy( &exe->stack[exe->sp-1] ,
						exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset,
						pc->imm.as.z32);
				pc++;
				continue;


			case op_store:
				printf(" store to %p:%d (%p)\n", exe->stack[exe->sp-1].as.ptr.address.bytes,exe->stack[exe->sp-1].as.ptr.offset, exe->stack[exe->sp-1].as.ptr.address.bytes + exe->stack[exe->sp-1].as.ptr.offset);

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

			case op_globalvar:
				exe->stack[exe->sp].as.ptr.address.block = exe->globals;
				exe->stack[exe->sp].as.ptr.offset = pc->imm.as.z32;
				printf(" ADDRESS FROM global+%d   %p\n", pc->imm.as.z32, exe->globals);

				exe->sp++;
				pc++;
				continue;

			case op_subvar:
				exe->stack[exe->sp-1].as.ptr.offset += pc->imm.as.z32;
				pc++;
				continue;

			case op_arrayindex:
				//sp-2 is array
				//sp-1 is the array
				printf(" --offset is %d , size is %d\n", exe->stack[exe->sp-1].as.z32, pc->imm.as.z32);
				getc(stdin);

				exe->stack[ exe->sp-2].as.ptr.offset += (exe->stack[exe->sp-1].as.z32 * pc->imm.as.z32);
				exe->sp--;
				pc++;
				continue;

			case op_dim:

				ptrT* arrayvar = (void*) exe->stack[exe->sp-2].as.ptr.address.bytes +  exe->stack[exe->sp-2].as.ptr.offset;

				int count = exe->stack[exe->sp-1].as.z32;
				//int elemsize = pc->imm.as.z32;
				typeT* arraytype = pc->imm.as.ptr.address.type;
				debugf(" DIMing array %s for elem size %d\n", arraytype->key, arraytype->ref->size);

				arrayvar->address.block = zarray_alloc_size(arraytype->ref->size, count, NULL);
				arrayvar->offset = 0;

				zarray_set_meta( arrayvar->address.block, arraytype );

				exe->sp-=2;
				pc++;
				continue;

			case op_call:
				if (pc->immtype != tWord){
					errorf(" expected word pointer\n");
					exit(1);
				}
				wordT* word = pc->imm.as.ptr.address.word;
				int argc = word->type->argc;

				printf(" to exec %s %s with %d args\n", word->name, word->type->key, argc);
				switchrunnerT* callee = (switchrunnerT*) word->target_pctx->runners[SWITCHRUNNER];
				print_switch_listing(callee->prog);


				int prebp = exe->bp;	//bp-1 is last arg. bp-2 is one before that.  bp is first empty val on stack
				exe->bp = exe->sp;

				callee->runner.execute(exe, &callee->runner, 0);

				if (word->type->ref){ //returns value
					printf(" returns value\n");
					exe->stack[exe->bp-argc] = exe->stack[exe->sp-1];  //last value on stack is copied to bp position
					exe->sp = exe->bp-argc+1;
				} else {
					exe->sp = exe->bp-argc; //CHECK THIS
				}

				exe->bp = prebp; //restore stack bp

				pc++;
				continue;

			case op_stop:
			case op_return:
			case op_returnval:
				return;

			default:
				printf(" unhandled %d %s\n", pc->opcode, instruction_names[pc->opcode]?instruction_names[pc->opcode]:"noname");
				return;

		}

	}

}

zbool cleansw(void* v){
	switchrunnerT* sw = v;
	printf(" free prog\n");
	ram_free(sw->prog);
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

	print_switch_listing(prog);
	sw->prog = prog;

	return &sw->runner;
}



//end of simple runner


//find words
wordT* match_word(parsectxT* pctx, parsectxT* searchpctx,  char* name, int inflags, int* rarg){

	int argc = 0;
	int maxarg = zvec_count(pctx->codestack); //most args possible is whole stack
	int flags=0;
	wordT* word = NULL;
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
			printf("failed to match exactly... trying phase %d on %s   %x\n", phase, name, flags);
		}

		//get all the words with this name


		//see if any of these words with this name match the args

		for (argc=0;argc <= maxarg; argc++){

			for (int i=0;i<argc;i++){
				int s = zvec_count(pctx->codestack)-argc+i;
				instructionT* arginst = zvec_get_at(pctx->codestack, s);
				argtypes[i] = arginst->result_type;
			}
			zarray_use(argtypes, argc);

			//check all contexts up to the parent
			for (parsectxT* sc = searchpctx; sc; sc=  (flags&MATCH_RECURSE_PCTX)? sc->parent : NULL) {
				//printf(" pctx:%p  sc:%p  sc->parent:%p\n", pctx, sc, sc->parent);
				//get words in this frame with the name
				zvecT* words = zstringmap_get(sc->dictionary, name); //get

				//debugf(" try to match %s with %d args in word frame %p \n", name, argc, sc);

				for (int i=0;i< zvec_count(words); i++){

					word = zvec_get_at(words, i);

					//test if the word from sc matches the stack in pctx
					if (test_word(pctx, argtypes, word, flags, argc)){
						if (rarg)
							*rarg = word->type->argc;

						//find return value for word if its a like-type
						if(word->type && word->type->ref && word->type->ref->is_wild){
								printf(" return type wild %s\n", word->type->ref->key);

								typeT* rettype = resolve_like_type_for_caller(pctx, word->type->ref, word->type, argtypes, flags);
								printf(" rettype is %s\n", rettype->key);
								typeT* pt = type_find(pctx, NULL, rettype, PROC, 0, argtypes);
								printf(" new type for word is %s\n", pt->key);


								wordT* new_word = word_alias_mk(NULL, pctx, word->name, pt, word);
								return new_word;

						}

						ram_free(argtypes);
						return word; //got one
					}


				}//for words

			}	 //for frame
		} //for argc
	} //for phase
	ram_free(argtypes);
	return NULL;

}

tokenT*  parse(exectxT* exe, parsectxT* pctx, tokenT* t, char** stop_tokens);

tokenT* parse_number(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

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


#define OF(PCTX, CAT, REF) type_find(PCTX, NULL, REF, CAT, 0, NULL)

void dereference_assembly(parsectxT* pctx){

		wordT* wloader = match_word(pctx, pctx, "@", MATCH_RECURSE_PCTX| MATCH_ALLOW_LIKE , NULL);  //find a loader for it

		if (wloader){

			printf(" Found loader OPCODE IS %d\n", wloader->opcode);
			//pop it off
			instructionT** insts = pop_args(pctx, 1); //get the instruction
			instructionT* loadinst = push_assembly(pctx, wloader->opcode,0, NULL, NULL, insts, wloader->type->ref);
		}else {
			printf(" @ undefined for type\n");
			exit(1);
		}

}




tokenT* autoload(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t){
	//if the result of this word is a reference, automatically load it some times
	zbool isReference= ZFALSE;


	if (w->type->category == VARIABLE) {
		//word is a variable

		if (w->type->ref->category == FRAME)	//if variable is a struct, keep it reference
			isReference = ZTRUE;
	}

	if (w->type->category == PROC){
		// if word is a proc (that had autoload flag set)
		if (w->type->ref->category == REFERENCE){  //and the proc returns a pointer
			if (w->type->ref->ref->category == FRAME) // if the reference returned is to a frame, keep it a reference
				isReference=ZTRUE;
		}

	}



//	printf(" autoload called on %s\n", t->str);
//	printf(" next is %p\n", tnext(t));
//	printf(" next is str %s\n", tnext(t)->str);


		printf("- autoload on %s (%s next) for %s\n", w->name,tnext(t)? tnext(t)->str:"EOF", w->type->key);

	if (t&& tnext(t) && !strcmp(tnext(t)->str , "&")){  //OR if next token is &, its pushed a pointer
		t = tnext(t);
		isReference = ZTRUE;
	}



	if (!isReference){
		printf("  dereferencing\n");
		dereference_assembly(pctx);
	}

	getc(stdin);
	return t;
}


tokenT* parse_default(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	printf("default parse for %s %s  (%d args)\n", t->str, w->type->key, w->type->argc);

	instructionT** args = pop_args(pctx, argc);

	//if word is a PROC, assemble it using the word's opcode and value
	if (w->type->category == PROC) {
		instructionT* inst = push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, args, w->type->ref);
		inst->comment = zstrdup(w->comment);


		if (w->autoload){

			t = autoload(exe, pctx, w, t);

		}

		return t;
	}

	//if its not a proc, then its a variable of some kind
	//note: thislooks just like a function but with no args
	//not sure if it will stay that way, butis that way for now, so maybe it will merge with the above

	if (w->type->category == ARG){

			printf("%s arg  at bp %d\n", w->name, w->val.as.z32);

			push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, w->type->ref );
			return t;
	}

	if (w->type->category == VARIABLE){


		push_assembly(pctx, w->opcode, 0, &w->val, w->val_type, NULL, OF(pctx, REFERENCE, w->type->ref));

		t = autoload(exe, pctx, w, t);

		return t;
	}

	errorf("Don't know what to do\n");
	exit(1);
}


parsectxT* parsectx_mk(parsectxT* parent);

typeT* type_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args );


tokenT* parse_array(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

		int depth = codestack_depth(pctx);
		char* stops[] = {"]", NULL};
		t = tnext(t);
		t = parse(exe,pctx, t, stops);

		//should be a datatype on the stack now

		depth = codestack_depth(pctx) - depth;

		if (depth == 1 ){
			instructionT* typearg = pop_arg(pctx);

			if (typearg->result_type == tType){


				typeT* type = type_find(pctx, NULL, typearg->val.as.ptr.address.type, ARRAY, 0, NULL);
				valueT v = {0};
				v.as.ptr.address.type = type;

				push_assembly(pctx, op_constant, 0, &v, tType, assembly_args_mk(1, typearg) , tType);


				return t;
			}
		}

		seterrorf(exe, ERROR_PARSE, "expected type\n");
		return t;
}


tokenT* parse_type_list(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

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

		t = parse(exe, pctx, t, stops);	//parse until a stop

		if (!t)
			break;

		if ((t->tok == ':') ) {
			t=tnext(t);
			printf("arg name %s\n", t->str);
			zvec_add(argnames, t->str);

			int count = codestack_depth(pctx) - depth;
			if (count != zvec_count(argnames)){
				errorf("Format must be  type:name type:name ...\n");
				exit(1);
			}

			t=tnext(t);
		}else if (t->tok == TOKEN_PAIR('-','>')){
			printf(" next type is return\n");
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
					v.as.ptr.address.type = type_mk(exe, NULL, t->str, NULL, LIKE, 0, NULL);
					v.as.ptr.address.type->argc = is_like;
					push_assembly(pctx, op_constant, INST_FREE_VALUE, &v, tType, NULL, tType);
				} else {
						errorf("like no named arg %s\n", t->str);
						exit(1);
				}
				t=tnext(t);
		}

	}

	iferr(exe)
		return NULL;

	if (expect_ret){

		ret_arg = pop_arg(pctx);
		if (ret_arg->result_type != tType || ret_arg->val_type != tType || ret_arg->val.as.ptr.address.type == NULL){
			seterrorf(exe, ERROR_PARSE,"missing or bad type name in arglist\n");
		}
		ret = ret_arg->val.as.ptr.address.type;


		printf(" return value: ");
		type_print(ret);
	}
	printf("\n");

	//now process unnamed args
	int count = codestack_depth(pctx) - depth;
	if (count>0){
		args = pop_args(pctx, count);
	}

	if ((zvec_count(argnames) > 0) && (count != zvec_count(argnames))){
		errorf("Either all parameters must be named, or none of them\n");
		exit(1);
	}

printf("parsed proto:\n");
	for (int i=0;i<count;i++){

		if (zvec_count(argnames)> i){
			printf("  %d %s ", i, zvec_get_x_at(argnames,char*, i));
		}

		type_print( args[i]->val.as.ptr.address.type   );
		printf("\n");

	}

	//create the proctype with named args
	//terminate argnames and argtypes will null
	zvec_add(argnames, NULL);


	//create the type

	typeT* ptype = type_mk(NULL, NULL, NULL, ret, PROC, 0, inst_const_types(args));

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

wordT* proc_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* proctype);

#if 1


tokenT* parse_var(exectxT* exe, parsectxT* pctx, wordT* wi, tokenT* t, int argc){


	instructionT* arg0 = pop_arg(pctx);
	t = tnext(t);
	char* varname = t->str;
	wordT* w = NULL;

	if ( arg0->val_type == tType){

		typeT* vartype = arg0->val.as.ptr.address.type;  //this is a datatype, like a Z32 or whatever
		vartype = OF(pctx, VARIABLE, vartype);

		w = word_mk(exe, pctx, varname, vartype); //word for the variable
		printf(" %d CREATING VAR %s of %s  @%d %d\n", argc,varname,  vartype->key , w->offset, vartype->size );
	} else {
		seterrorf(exe, ERROR_PARSE, "Type constant not found\n");
	}


	if (w) {
		w->val.as.ptr.address.word =w;
		w->val_type = tWord;

		if( pctx->parent)
			w->opcode = op_localvar;
		else
			w->opcode = op_globalvar;

		w->autoload = ZTRUE;

		printf(" set opcode.  w is %s\n", w->val.as.ptr.address.word->name);

	}
	ram_free(arg0);

	return t;
}

wordT* proc_opcode_mk(exectxT* exe, parsectxT* pctx,  int opcode, char* name,  typeT* proctype);
wordT* proc_opcode_mk2(exectxT* exe, parsectxT* pctx,  int opcode, char* name,  char* typestr);

//make a procedure type
typeT* type_proc_mk(typeT* ret, int n, ...){

	va_list args;
	typeT** types = NULL;

	if (n){
		types = zarray_alloc( typeT*, n);
		va_start(args, n);
		for (int i=0;i<n;i++){
			types[i] = va_arg(args, typeT*);	//get next arg
		}
		va_end(args);
		zarray_use(types, n);
	}

	return type_mk(NULL, NULL, NULL, ret,PROC,0,types);

}

tokenT* parse_proc(exectxT* exe, parsectxT* pctx, wordT* wi, tokenT* t, int argc){


	instructionT* arg0 = pop_arg(pctx);
	t = tnext(t);
	char* varname = t->str;
	wordT* w = NULL;

	if ( arg0->val_type == tType){

		typeT* vartype = arg0->val.as.ptr.address.type;  //this is a datatype, like a Z32 or whatever

		printf(" %d CREATING PROC %s of %s\n", argc,varname,  vartype->key  );
		w = word_mk(exe, pctx, varname, vartype); //word for the variable
	} else {
		seterrorf(exe, ERROR_PARSE, "Proc type constant not found\n");
	}
	t = tnext(t); //skip over name

	if	(w){

		//create parse context
		parsectxT* innerpctx = parsectx_mk(pctx);

		w->target_pctx = innerpctx;

		//create the variable arg names
		for (int i=0;i<w->type->argc;i++){
			wordT* argvar = word_mk(exe, innerpctx, w->type->opt_argnames[i], OF(pctx, ARG, w->type->argtypes[i]));
			argvar->val.as.z32 = -w->type->argc + i; //offset from bp
			argvar->val_type = tZ32;
			argvar->opcode = op_argpick;
		}

		//create the 'return' keyword for this context, using the return value
		if (w->type->ref)
			//proc_opcode_mk(exe, innerpctx, op_returnval, "return", NULL, type_args_mk(1,w->type->ref));
			proc_opcode_mk(exe, innerpctx, op_returnval, "return", type_proc_mk(NULL, 1, w->type->ref));
		 else //no return value
			proc_opcode_mk(exe, innerpctx, op_return, "return", type_proc_mk(NULL, 0));

		char* stops[]={"end", NULL};

		printf(" parse inner pctx %p for word %p\n", innerpctx, w);


		t= parse(exe, innerpctx, t, stops);

		w->val.as.ptr.address.word =w;
		w->val_type = tWord;
		w->opcode = op_call;

	}
	ram_free(arg0);

	return t;
}



tokenT* parse_frame(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	t=tnext(t); //skip 'type' string

	//should be the name now
	char* name = t->str;
	debugf(" parsing frame for %s\n", name);

	t=tnext(t);

	//create incomplete data tyoe
	typeT* type = type_mk(exe, pctx, name, NULL, PENDING, 0, NULL);


	//create parse context
	parsectxT* innerpctx = parsectx_mk(pctx);

	type->word->target_pctx = innerpctx;

	char* stops[]={"end", NULL};

	printf(" parse inner pctx %p for word %p\n", innerpctx, type->word);
	//getc(stdin);

	t= parse(exe, innerpctx, t, stops);

	type->category = FRAME; //it's done
	type->size = innerpctx->size;
	
	debugf("done at %s\n", t->str);
	return t;
	
}
#endif

tokenT* parse_type_name(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	if (w->val_type == tType && w->val.as.ptr.address.type){

			valueT v = {0};
			v.as.ptr.address.type = w->val.as.ptr.address.type; //push the type that the word represents

			printf("pushing %s\n",  w->val.as.ptr.address.type->key );
		
			push_assembly(pctx, op_constant, 0, &v, tType, NULL, tType);
			//typestack_push_value(pctx, tType, &v);  //and that value we just pushed, is a type
			//t->type = tType;
			return t;
	}

	errorf("Unknown type\n");
	exit(1);
}

tokenT* parse_related_type(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	//typestack_pop(pctx, argc);
	instructionT* arg0 = pop_arg(pctx);

	typeT* type = arg0->val.as.ptr.address.type;

	int instflags=0;

	if (type){		//if the type is a constant, get it


		if (!strcmp(t->str, "&"))
			type = type_find(pctx, NULL, type, REFERENCE, 0, NULL);
		if (!strcmp(t->str, "@"))
			if (type->is_wild){
				type = type_mk(NULL, NULL, NULL,type, DEREFERENCE,0, NULL);
				instflags = INST_FREE_VALUE; //the type created above is not attached to a pctx, so free it when done
			} else {
				type = type->ref;
			}
	} else {
			printf(" This requires a constant\n");
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
tokenT* parse_flow(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){


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
				t = parse(exe, pctx, t, ifstops); //parse until end/if/elseif

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

			printf("HERE\n\n");
		codestack_print(pctx->codestack,0, ZTRUE);

		//exit(1);

		return t;



	} else if(!strcmp(t->str, "loop")){

		instructionT** body = NULL;
		instructionT** step = NULL;


		char* loopstops[]= { "end", "step", NULL};
		char* stepstops[]= { "end",  NULL};
		int depth = codestack_depth(pctx);
		t=tnext(t);
		t = parse(exe, pctx,t, loopstops);
		body = pop_args(pctx, codestack_depth(pctx) - depth);

		push_assembly(pctx, op_block, 0,NULL, NULL, body,NULL);


		if (!strcmp(t->str, "step")){
			depth = codestack_depth(pctx);
			t = tnext(t);
			t = parse(exe, pctx,t, loopstops);
			step = pop_args(pctx, codestack_depth(pctx) - depth);
			push_assembly(pctx, op_block, 0, NULL, NULL, step, NULL);
		}

		instructionT* parts = NULL;

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
tokenT* parse(exectxT* exe, parsectxT* pctx, tokenT* t, char** stop_tokens){
	//skip past this stub
	if (t->tok == TOKEN_STARTFILE){
		t=tnext(t);
	}

	while(t){
		//print type stack
		printf(" Code Stack:\n");
		codestack_print(pctx->codestack,0, ZTRUE);
		printf("--\n");

		if (stop_tokens){
			char** s = stop_tokens;
			while(*s){
				if (!strcmp(*s, t->str)){
					//printf("match stop %s\n", t->str);
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
				t = parse_number(exe, pctx, NULL, t, 0);
				t=tnext(t);
				continue;
		}

		//try to match words through this and all parent contexts

		int argc=0;
		wordT* w = match_word(pctx, pctx,  t->str, MATCH_RECURSE_PCTX| MATCH_ALLOW_LIKE, &argc);

		if (w) {
			printf("found word %s %s   %s\n", w->name, w->type?w->type->key:"notype" , t->str  );

			tokenT* tstart = t;

			if (!w->parse){
				printf("do default parse\n");
				t = parse_default(exe, pctx, w, t, argc);
			} else{
				printf("do %s parse\n", w->name);
				t = w->parse(exe, pctx, w, t, argc);
			}

			for (int i=0;i<argc;i++)
				tstart = tprev(tstart);

			if (!t){
					seterrorf(exe, ERROR_PARSE, "unexpected end of input\n");

					return NULL;
			}

			t=tnext(t);
			continue;
		}
		//did not match a word

		//is it a frame member?

		if (zvec_count(pctx->codestack) >= 1 && t->str[0] == '.' ){
			printf("-------------\n");
			instructionT* inst = zvec_get_at(pctx->codestack, zvec_count(pctx->codestack)-1);
			typeT* ty = inst->result_type;
			if (ty->category == VARIABLE || ty->category == REFERENCE)
				ty = ty->ref;


			type_print(ty);

			if (ty->word && ty->word->target_pctx){
				printf("  aaaaa %p %p\n", ty->word, ty->word->target_pctx);

				w = match_word(pctx, ty->word->target_pctx, t->str+1 /*skip dot*/, 0,NULL );

				printf(" found %s\n", w?w->name : "nothing");
				valueT v = {0};
				v.as.ptr.address.word = w;

				push_assembly(pctx, op_subvar, 0, &v, tWord, assembly_args_mk(1, pop_arg(pctx)), OF(pctx, REFERENCE, w->type->ref));


				t = autoload(exe, pctx, w,t);



				DEBUG_MATCH=1;
/*
				if (t&& tnext(t) && !strcmp(tnext(t)->str , "&")){
					valueT nv={0};
					instructionT** largs = pop_args(pctx, 1);
					instructionT* inst = push_assembly(pctx, op_reference, 0, nv, NULL, largs, OF(pctx, REFERENCE, w->type->ref)      );
					inst->comment = zstrdup(w->comment);
					t = tnext(t);

				}
*/
				t=tnext(t);
				continue;
			}


		}

		seterrorf(exe, ERROR_PARSE, "UNDEFINED %s\n from stack ", t->str);
		for (int i=0;i<zvec_count(pctx->codestack);i++){
			instructionT* inst = zvec_get_at(pctx->codestack, i);
			exe->error_string =  zstrprintf(exe->error_string, " %s ", inst->result_type?inst->result_type->key : "notype");

		}
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

		printf(" ref is %p\n", ref);
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
				return zstrprintf(NULL, "{%s}&", ref->key);

			case DEREFERENCE:
				return zstrprintf(NULL, "{%s}@", ref->key);

			case VARIABLE:
				return zstrprintf(NULL, "{%s}var", ref->key);

			case ARG:
				return zstrprintf(NULL, "{%s}arg", ref->key);

			case ARRAY:
				return zstrprintf(NULL, "{[%s]}", ref->key);

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

wordT* proc_parser_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* proctype, void* parsefunc);

//create a type.  takes ownership of 'args' if passed
typeT* type_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args ){
									
	typeT* type = ram_alloc(sizeof(typeT), typecleanup); type_mk_line = __LINE__;

	type->name = zstrdup(name);

	type->ref = ram_addref(ref);
	if (ref)
		type->is_wild = type->ref->is_wild;	//anything based on wild type is wild

	type->category = category;

	type->size = size;

	if (type->category == LIKE)
		type->is_wild = ZTRUE;	//like types are wild

	if (type->category == ARRAY) {
		type->size = sizeof(ptrT);
	}


	type->key = type_key(name, ref, category, size, args);
	debugf("Creating type %s   %s, %d bytes\n", name?name:"noname", type->key, type->size);

	if (args){
		type->argc = zarray_count(args);
		type->argtypes = args;
	}
	
	if (pctx){
		if (zstringmap_get(pctx->types, type->key))
			errorf("!!!/There is already a type entry for %s\n", type->key);

		else{
			printf(" putting type in %p -> pctx %d\n",type, pctx->id);
			zstringmap_put(pctx->types, type->key, type);
			//store it as a word
			if (name && strlen(name) > 0){
				printf("Making word %s to push type\n", name);
				wordT* w = proc_parser_mk(exe, pctx, name,  type_proc_mk( tType, 0), parse_type_name);//create a proc word, that runs parse_type_name at parse time
				type->word =w; //the type needs a word to find it
				w->val.as.ptr.address.type = type;
				w->val_type = tType; //is a type
			}
		}
	} else printf("creating type with no pctx\n");

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
			printf(" Can't find dereference of wild or nhull\n");
			exit(1);
		}
	}

	//lookup type
	char* key = type_key(name, ref, category, size, args);

	typeT* type = zstringmap_get(pctx->types, key);

	if (type) {
		debugf("Found existing type %s %p\n", key, type);
		//todo : free args?
		if (category != NAMED  && category != NAMEDEXISTING && category != type->category){
			errorf("Type is not expected category %d\n", category);
		}
		ram_free(key);
		ram_free(args);
		return type;
	}




	//create a new type if we have to
	if (category == REFERENCE || category == PROC || category == VARIABLE || category == ARG || category == ARRAY){
		type = type_mk(NULL, pctx, NULL, ref, category, size, args);
	} else if (category == NAMED){
		//wanted a type by name (like a struct), but the type is not yet
		//defined.
		type = type_mk(NULL, pctx, name, NULL, PENDING, 0, NULL);
	}

	ram_free(key);

	//if we made a type return it

	return type;

}



//parse context
zbool cleanparsectx(void* v){
	parsectxT* pctx = v;
	tracef("cleaning pctx %d\n", pctx->id);
	ram_free(pctx->dictionary);
	ram_free(pctx->types);
	ram_free(pctx->codestack);

	for (int i=0;i<MAXRUNNERS;i++)
		ram_free(pctx->runners[i]);

	return ZTRUE;
}

parsectxT* parsectx_mk(parsectxT* parent){
	static int debugid=0;

	parsectxT* pctx	= ram_alloc(sizeof(parsectxT), cleanparsectx);
	pctx->dictionary = zstringmap_mk(64);
	pctx->types = zstringmap_mk(64);
	pctx->parent = parent;
	pctx->id = ++debugid;
	printf("creating pctx %d\n", debugid);
	return pctx;
}


//Makes a procedure word in a dictionary
wordT* proc_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* proctype){

		if (!proctype)
			proctype = type_proc_mk(NULL, 0);  //if no type specified, pick a proc with no args and no return

		wordT* procword= word_mk(exe, pctx, name, proctype);
		ram_free(proctype); //word above has a reference to it

		return procword;
}

//Makes a procedure word that runs an opcode. Most primitives are these.
wordT* proc_opcode_mk(exectxT* exe, parsectxT* pctx,  int opcode, char* name, typeT* proctype){
		wordT* w = proc_mk(exe, pctx, name, proctype);
		iferr(exe) {
			return NULL;
		}
		w->opcode = opcode;
		w->comment = zstrdup(name);
		return w;
}


typeT* type_from_str(exectxT* exe, parsectxT* pctx, char* str);

wordT* proc_opcode_mk2(exectxT* exe, parsectxT* pctx,  int opcode, char* name, char* typestr){
	typeT* proctype = type_from_str(exe, pctx, typestr);
	wordT* w = proc_mk(exe, pctx, name, proctype);
	iferr(exe) {
		return NULL;
	}
	w->opcode = opcode;
	w->comment = zstrdup(name);
	return w;
}

//Makes a procedure word that runs a C function in the parser.  The parser is made of these.
wordT* proc_parser_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* proctype, void* parsefunc){
		wordT* w = proc_mk(exe, pctx, name, proctype);
		iferr(exe) {
			return NULL;
		}
		w->parse = parsefunc;
		return w;
}

//Takes a string and produces the datatype it represents as a constant.  This is much easier than chaining together multiple lines of C building a data type from the actual structs.
//The caller of this function is responsible to free this typeT struct, or store it somewhere that will free it
typeT* type_from_str(exectxT* exe, parsectxT* pctx, char* str){
	//takes a string and makes a type
	zlistT tokens;
    zlist_init(&tokens);

    tokenT* first = token_mk(TOKEN_STARTFILE, NULL, 0);
    zlist_addhead(&tokens, &(first->zlistnode));

    tokenize(first, str, "type_from_str", 1);

    parse(exe, pctx, zlist_head(&tokens), NULL);

	//codestack_print(pctx->codestack, ZTRUE);
	instructionT* it = pop_arg(pctx);


	if (it->result_type != tType){
		printf("is not a type: %s'n", str);
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
#define ADD_INST(NAME)	instruction_names[ op_ ## NAME ] = #NAME;

void leak_viewer(void* item, char* file, int line){

		if (line == type_mk_line){	//this is
			printf("\t\tLeaked type: %s\n", ((typeT*)item)->key);
		}

}

//tokenize and parse (evnetually... run)
void int_run_str(char* src, char* filename){
	instruction_names = zarray_alloc( char*, op_MAX);
	ADD_INST(constant);
	ADD_INST(globalvar);
	ADD_INST(load);
	ADD_INST(subvar);
	ADD_INST(store);
	ADD_INST(print32);
	ADD_INST(printptr);
	ADD_INST(add32);
	ADD_INST(call);
	ADD_INST(return);
	ADD_INST(returnval);
	ADD_INST(argpick);
	ADD_INST(if);
	ADD_INST(loop);
	ADD_INST(block);
	ADD_INST(break);
	ADD_INST(continue);
	ADD_INST(switch_jump);
	ADD_INST(switch_jumpfalse);
	ADD_INST(arrayindex);

	parsectxT* pctx= parsectx_mk(NULL);
	exectxT* exe = exectx_mk();

	//create basic types
	tType = type_mk(exe, pctx, "Type", NULL, OPAQUE, sizeof(typeT*), NULL);
	tZ32  = type_mk(exe,pctx, "Z32",  NULL, SIMPLE, sizeof(zint32), NULL);
	tReal = type_mk(exe,pctx, "Real", NULL, SIMPLE, sizeof(FLOAT),  NULL);
	tBit = type_mk(exe,pctx, "Bit", NULL, SIMPLE, sizeof(int),  NULL);
	tWord = type_mk(exe,pctx, "Word", NULL, OPAQUE, sizeof(wordT*),  NULL);
	tAny =  type_mk(exe, pctx, "Any", NULL, OPAQUE, 0, NULL);  //'Any' does not have a size (but any& does)

	//create parser words that work in type constants
	proc_parser_mk(exe,pctx, "(", NULL, parse_type_list); //handle argument lists for functions
	proc_parser_mk(exe,pctx, "&", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into its reference type
	proc_parser_mk(exe,pctx, "@", type_proc_mk(tType, 1, tType), parse_related_type); //makes a type into its dereferenced type
	proc_parser_mk(exe,pctx, "[", NULL, parse_array); //create array type TODO:consider moving from [Z32] syntax to Z32[]

	//create control structures
	proc_parser_mk(exe,pctx, "if", type_proc_mk(NULL, 1, tZ32), parse_flow); //parse flow structures
	proc_parser_mk(exe,pctx, "loop", NULL, parse_flow); //parse flow structures

	//define parser words for variables, types and procs
	proc_parser_mk(exe,pctx, "var",  type_proc_mk(NULL, 1, tType), parse_var); //makes a variable in current context
	proc_parser_mk(exe,pctx, "type", type_proc_mk(NULL, 0), parse_frame);
	proc_parser_mk(exe,pctx, "proc", type_proc_mk(NULL, 1, tType), parse_proc);

	//create arrays
	proc_opcode_mk2(exe,pctx, op_dim, "dim", "([Any]&:array Z32:len)");
	wordT* arrayindex = proc_opcode_mk2(exe,pctx, op_arrayindex, "[]", "( [Any]:a Z32:idx -> like a@&)");
	arrayindex->autoload = ZTRUE;


	proc_opcode_mk2(exe,pctx, op_break, "break", "()");
	proc_opcode_mk2(exe,pctx, op_continue, "continue", "()");

	//create primitives. minimal at the moment
	//thse use the above specified parser
	proc_opcode_mk2(exe,pctx, op_print32, "print", "(Z32)");
	proc_opcode_mk2(exe,pctx, op_add32, "+", "(Z32:a Z32:b -> Z32)");
	proc_opcode_mk2(exe,pctx, op_printptr, "printptr", "(Any)");

	proc_opcode_mk2(exe,pctx, op_load, "@",  "(Any&:a  -> like a@ )" );
	proc_opcode_mk2(exe,pctx, op_store, "=",  "(Any:value  like value&:dst)" );
//	proc_opcode_mk2(exe,pctx, 3232, "=", "(Z32 Z32&)");

	//proc_opcode_mk2(exe,pctx, 888, "@",  "(Z32&  -> Z32 )" );

	//proc_opcode_mk2(exe,pctx, 999, "add",  "(Any: a   like a&:b   like a&&:c )" );
	//proc_opcode_mk2(exe,pctx, 999, "add",  "(Any&: a   like a@:b   like b:c )" );

	//load integers
	//proc_opcode_mk2(exe,pctx, 888, "@",  "(Z32.var -> Z32)" );


	dump_dictionary(pctx);
	dump_types(pctx);

    zlistT tokens;
    zlist_init(&tokens);

    tokenT* first = token_mk(TOKEN_STARTFILE, NULL, 0);
    zlist_addhead(&tokens, &(first->zlistnode));

    tokenize(first, src, filename, 1);

    parse(exe, pctx, zlist_head(&tokens), NULL);

	codestack_print(pctx->codestack,0, ZTRUE);

	iferr(exe){
		fprintf(stderr, "Error(%d): %s\n", exe->error_code, exe->error_string);
	}



	ifok(exe){
		runnerI* runme = compile_for_switch(exe, pctx);

		exe->globals = ram_alloc(pctx->size, NULL); //todo: clean, using the shadow to track type

		printf(" Alloc %d global space\n", pctx->size);
		runme->execute(exe, runme, 0);

		ram_free(exe->globals);

		//ram_free(runme); //runner is part of the pctx, and is freed there
	}
//	dump_dictionary(pctx);
//	dump_types(pctx);

    zlist_cleanup(&tokens);
	ram_free(pctx);
	ram_free(exe);
	ram_free(instruction_names);
	abyss = leak_viewer;
}
