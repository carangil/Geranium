#include "int.h"
#include "string.h"
#include "stdarg.h"
#include "zmem.h"
#include "zarray.h"

#define tracef(...)

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
		if ((p = find_pair(".&.%.@--++==->/**///[]>=<=!=.-###=\\\\/\\\\/", c, next))){
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

            if ( (*in) == '0' || *(in+1) == 'x'){
				int hexdigits = 2 + accept_patterns(in+2, "0123456789abcdefABCDEF", "", "0123456789abcdefABCDEF");
                if (hexdigits > digits)
                        digits= hexdigits;
            }
		}

		if (name> digits)
			digits=0; //accept name if it is longer

		if (digits || name){
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
	ram_free(w->target_type);
	ram_free(w->comment);
	return ZTRUE;
}

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
	}else{
		seterrorf(exe, ERROR_PARSE,  "creating word with no name\n");
		return NULL;

	}

	return w;
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

//Compare two types to see if they are compatible/equivalent
zbool type_cmp(typeT* expected, typeT* given, int flags){

	if (expected == given)
		return ZTRUE;

	if (expected->ref && given->ref && expected->category == given->category){
		return type_cmp( expected->ref, given->ref, flags);
	}

	if (given && given->category == VARIABLE){ //treated a variable containing value as that value
		return type_cmp(expected, given->ref, flags);
	}

	if (expected==tAny) //tAny matching anything
		return ZTRUE;

	if (expected->category == LIKE){
		//need to
		printf(" Need to compare like types\n");
		exit(1);
	}

	return ZFALSE;

}



typeT* type_find(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args);

void indent(int n){
		for (int i=0;i<n;i++)
			printf(" ");
}

//instruction set
char** instruction_names = NULL;
void instruction_print(instructionT* inst, int level, zbool recurse){


	if (!inst)  {
		printf("(null code)\n");
		return;
	}

	if (recurse && inst->args){
		indent(level); printf("{\n");
		for (int i=0; i<zarray_count(inst->args); i++){
			instruction_print(inst->args[i], level+1, recurse);
		}
		indent(level); printf("} ");
	}

	indent(level);
	char* name = instruction_names[inst->opcode];
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

	printf("\n");


}

void codestack_print(zvecT* cs,  zbool recurse){

	if (!cs)
		return ;

	for (int i=0; i < zvec_count(cs); i++){
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

typeT** inst_types(instructionT** a){

	typeT** types = zarray_alloc(typeT*, zarray_count(a) );
	for (int i=0;i<zarray_count(a);i++){
		types[i] = a[i]->result_type;
	}
	zarray_use(types, zarray_count(a));
	return types;
}

typeT** inst_const_types(instructionT** a){

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
		ram_free(inst->val.as.ptr.address.bytes);
	}

	return ZTRUE;
}

//returns an array item representing one instruction.
instructionT* push_assembly(parsectxT* pctx, int opcode, int flags, valueT val, typeT* val_type,  instructionT** args , typeT* ret){

	if (!pctx->codestack){
		pctx->codestack = zvec_mk(NULL, 8);
	}

	instructionT* inst = ram_alloc(sizeof(instructionT), clean_inst);


	inst->opcode=opcode;
	inst->result_type =ret;
	inst->val_type = val_type;
	inst->val= val;
	inst->flags = flags;

	///printf(" args have %d\n", args?zarray_count(args):-9999);
	inst->args = args;

	//push it onto the codestack
	zvec_add(pctx->codestack, inst);

	return inst;
}



//t is the token trying to be matched
//tprev(t) is the last arg to it, if there are any

#define MATCH_RECURSE 0x10000000
int DEBUG_MATCH=0;
//tests of a word matches the current typestack of pctx

zbool test_word(parsectxT* pctx, wordT* word, int flags, int argc){

	if (DEBUG_MATCH)
		debugf("test %s:%d to %d\n", word->name, word->type->argc,argc);


	if (argc != word->type->argc){ 
		debugf("Skipping mismatch arg count %d %d\n", argc, word->type->argc);
		return ZFALSE;
	}

	//argless words	
	if (argc==0 && word->type->argc==0){
	//	debugf("Word %s has no members, name matches, returning it\n", word->name);
		return ZTRUE;
	}

	//try to match args	
	void* csr=NULL;
	for (int i=0;i<argc;i++){
		int s = zvec_count(pctx->codestack)-argc+i;
		instructionT* arginst = zvec_get_at(pctx->codestack, s);
		typeT* passed_type = arginst->result_type;

		if (!passed_type){
				errorf(" No result type!\n");
				return ZFALSE;
		}


		typeT* argtype = word->type->argtypes[i];
	//	debugf("Compare sp%d  %s s %s\n", i, argtype->key,  arginst->result_type->key);



		if (!type_cmp( argtype, passed_type, 0)){
			printf(" arg %d    %p %s vs %p %s \n", i, argtype, argtype->key, passed_type, passed_type->key);
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
	valueT imm;	    //any immediate value
	typeT* immtype; //type of the immediate value
	instructionT* source;
}switchopT;

typedef struct switchrunnerS{
	runnerI runner;
	switchopT* prog;
} switchrunnerT;
runnerI* compile_for_switch(exectxT* exe, parsectxT* pctx);

switchopT* compile_switch_subtree(exectxT* exe, switchopT* prog, instructionT* inst){


	if (inst->opcode == op_globalvar){
		printf(" compiling globalvar used as value\n");

	} else if (inst->opcode == op_reference){
		printf("compiling reference\n");
	}

	if (inst->args){
		for (int i=0;i<zarray_count(inst->args);i++){
			prog = compile_switch_subtree(exe, prog, inst->args[i]);
		}
	}

	switchopT op = {0};
	op.opcode = inst->opcode;
	op.imm = inst->val;
	op.immtype = inst->val_type;
	op.source=inst;

	if (op.opcode == op_call){
		//need to compile the func


		if (op.immtype == tWord){

			wordT* word = op.imm.as.ptr.address.word;

			printf("To compile switch:%s:%s\n", word->name, word->type->key);

			compile_for_switch(exe, word->target_pctx);


		} else{
			errorf("bad op_call \n");
			exit(1);
		}

	}

	prog = zarray_more(prog, 1, NULL);
	prog = zarray_append(prog, op);

	return prog;
}



void print_switch_listing(switchopT* prog){
		for (int i=0;i<zarray_count(prog);i++){

		char* name = instruction_names[prog[i].opcode];

		if (name)
			printf("%s\t", name);
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
				printf("%d:%d|", i, exe->stack[i].as.z32);
		}
		printf("%d:<>\n", exe->sp);


		switch (pc->opcode) {

			case op_constant:  exe->stack[exe->sp++] = pc->imm;  pc++; continue;
			case op_print32:   printf("print32: %d\n", exe->stack[--exe->sp].as.z32); pc++; continue;
			case op_add32:     exe->stack[exe->sp-2].as.u32 += exe->stack[exe->sp-1].as.u32; exe->sp--; pc++; continue;

			case op_call:
				if (pc->immtype != tWord){
					errorf(" expected word pointer\n");
					exit(1);
				}
				wordT* word = pc->imm.as.ptr.address.word;
				int argc = word->type->argc;

				printf(" to exec %s %s with %d args\n", word->name, word->type->key, argc);
				switchrunnerT* callee = word->target_pctx->runners[SWITCHRUNNER];
				print_switch_listing(callee->prog);


				int prebp = exe->bp;	//bp-1 is last arg. bp-2 is one before that.  bp is first empty val on stack
				exe->bp = exe->sp;

				callee->runner.execute(exe, &callee->runner, 0);

				if (word->type->ref){ //returns value
					printf(" returns value\n");
					exe->stack[exe->bp] = exe->stack[exe->sp-1];  //last value on stack is copied to bp position
					exe->sp = exe->bp+1;
				} else {
					exe->sp = exe->bp;
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

	switchrunnerT* sw = ram_alloc(sizeof(switchrunnerT), cleansw);
	sw->runner.execute = run_switch;

	pctx->runners[SWITCHRUNNER] = &sw->runner;

	switchopT* prog = zarray_alloc( switchopT, 1);

	for (int i=0;i<zvec_count(pctx->codestack);i++){

		instructionT* subtree = zvec_get_at(pctx->codestack, i);


		prog = compile_switch_subtree(exe, prog, subtree);

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
wordT* match_word(parsectxT* pctx, parsectxT* searchpctx,  char* name, int flags, int* rarg){

	int argc = 0;
	int maxarg = zvec_count(pctx->codestack); //most args possible is whole stack

	wordT* word = NULL;

	if (DEBUG_MATCH){
			printf(" DEBUG MATCH FOR %s\n", name);
	}

	//get all the words with this name


	//see if any of these words with this name match the args

	for (argc=0;argc <= maxarg; argc++){

		for (parsectxT* sc = searchpctx; sc; sc=  (flags&MATCH_RECURSE)? sc->parent : NULL) {
			//printf(" pctx:%p  sc:%p  sc->parent:%p\n", pctx, sc, sc->parent);
			//get words in this frame with the name	
			zvecT* words = zstringmap_get(sc->dictionary, name); //get 

			//debugf(" try to match %s with %d args in word frame %p \n", name, argc, sc);

			for (int i=0;i< zvec_count(words); i++){

				word = zvec_get_at(words, i);

				//test if the word from sc matches the stack in pctx
				if (test_word(pctx, word, flags, argc)){
					if (rarg)
						*rarg = word->type->argc;
					return word; //got one
				}


			}//for words

		}	 //for frame
	} //for argc

	return NULL;

}

tokenT*  parse(exectxT* exe, parsectxT* pctx, tokenT* t, char** stop_tokens);

tokenT* parse_number(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	valueT val = {0};

	if (strchr(t->str, '.')){
		val.as.f = atof(t->str);
		push_assembly(pctx, op_constant, 0, val, tReal, NULL, tReal);
	} else if (strchr(t->str, 'x') == (t->str +1 ) ){
		val.as.u32 = strtol(t->str, NULL, 16);
		push_assembly(pctx, op_constant, 0, val, tZ32, NULL, tZ32);
	} else {
		val.as.u32 = atoi(t->str);
		push_assembly(pctx, op_constant, 0, val, tZ32, NULL, tZ32);
	}

	return t;
}


#define OF(PCTX, CAT, REF) type_find(PCTX, NULL, REF, CAT, 0, NULL)


tokenT* parse_default(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){
	//printf("default handler for %s\n", w->name);
//	printf("popping %d for %s\n", argc, w->name);

	instructionT** args = pop_args(pctx, argc);

	//if word is a PROC, assemble it using the word's opcode and value
	if (w->type->category == PROC) {
	//	printf(" assembling proc\n");
		instructionT* inst = push_assembly(pctx, w->opcode, 0, w->val, w->val_type, args, w->type->ref);
		inst->comment = zstrdup(w->comment);
		return t;
	}


	printf("default parse for %s %s\n", t->str, w->type->key);

	//if its not a proc, then its a variable of some kind
	//note: thislooks just like a function but with no args
	//not sure if it will stay that way, butis that way for now, so maybe it will merge with the above
	if (w->type->category == VARIABLE){
		zbool isReference = ZFALSE;

		if (w->type->ref->category == FRAME)  //a struct value is pushed as a pointer
			isReference = ZTRUE;

		if (t&& tnext(t) && !strcmp(tnext(t)->str , "&")){  //OR if next token is &, its pushed a pointer
			// note, if the & is applied to a struct, then the & is redundent this is intentional, because
			// later there will be small structs that do get pushed on the stack, and perhaps we want to push a pointer instead
			t = tnext(t);
			isReference = ZTRUE;
		}

		valueT novalue={0};


		// push the 'variable' to the stack
		instructionT* inst = push_assembly(pctx,  w->opcode, 0, w->val, w->val_type, NULL, w->type);

		//if this is really should be a reference
		if (isReference){
				instructionT** largs = pop_args(pctx, 1);
				instructionT* inst = push_assembly(pctx, op_reference, 0, novalue, NULL, largs, OF(pctx, REFERENCE, w->type->ref)      );
				inst->comment = zstrdup(w->comment);
				t = tnext(t);
		}

		return t;
	}

	errorf("Don't know what to do\n");
	exit(1);
}


parsectxT* parsectx_mk(parsectxT* parent);

typeT* type_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args );



tokenT* parse_type_list(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	t = tnext(t); //skip over "("
	int depth = codestack_depth(pctx);  //get codestack depth

/*
 *  (typename:varname typename:varname)
 *  (typename:varname typename:varname -> returntype)
 * varnames optional.
 * */
	char* stops[]={ ":", "->", ")" , "like" ,  NULL};

	zvecT* argnames = zvec_mk(NULL, 8);
//	zvecT* argtypes = zvec_mk(NULL, 8);
	instructionT** args = NULL;
	zvec_disown(argnames);
	//zvec_disown(argtypes);

	int expect_ret=0;
	typeT* ret = NULL;

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
		} else if (!strcmp(t->str, "like")){
				valueT v = {0};
				t=tnext(t);
				v.as.ptr.address.type = type_mk(exe, NULL, t->str, NULL, LIKE, 0, NULL);

				type_print(v.as.ptr.address.type);
				push_assembly(pctx, op_constant, INST_FREE_VALUE, v, tType, NULL, tType);
				t=tnext(t);
		}

	}

	iferr(exe)
		return NULL;

	if (expect_ret){
		instructionT* arg = pop_arg(pctx);
		if (arg->result_type != tType || arg->val_type != tType || arg->val.as.ptr.address.type == NULL){
			seterrorf(exe, ERROR_PARSE,"missing or bad type name in arglist\n");
		}
		ret = arg->val.as.ptr.address.type;

		ram_free(arg);  //discard it
		arg = NULL;

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

	valueT v = {0};
	v.as.ptr.address.type= ptype;

	push_assembly(pctx, op_constant, INST_FREE_VALUE, v, tType, args, tType);

	ram_free(argnames);

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

wordT* proc_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ret, typeT** args);

#if 1


tokenT* parse_var(exectxT* exe, parsectxT* pctx, wordT* wi, tokenT* t, int argc){


	instructionT* arg0 = pop_arg(pctx);
	t = tnext(t);
	char* varname = t->str;
	wordT* w = NULL;

	if ( arg0->val_type == tType){

		typeT* vartype = arg0->val.as.ptr.address.type;  //this is a datatype, like a Z32 or whatever
		vartype = OF(pctx, VARIABLE, vartype);
		printf(" %d CREATING VAR %s of %s\n", argc,varname,  vartype->key  );
		w = word_mk(exe, pctx, varname, vartype); //word for the variable
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


	}
	ram_free(arg0);

	return t;
}

wordT* proc_opcode_mk(exectxT* exe, parsectxT* pctx,  int opcode, char* name,  typeT* ret, typeT** args);
typeT** type_args_mk(int n, ...);

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

		//create the 'return' keyword for this context, using the return value
		if (w->type->ref)
			proc_opcode_mk(exe, innerpctx, op_returnval, "return", NULL, type_args_mk(1,w->type->ref));
		 else //no return value
			proc_opcode_mk(exe, innerpctx, op_return, "return", NULL, NULL);

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

	
	debugf("done at %s\n", t->str);
	return t;
	
}
#endif

tokenT* parse_type_name(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	if (w->val_type == tType && w->val.as.ptr.address.type){

			valueT v = {0};
			v.as.ptr.address.type = w->val.as.ptr.address.type; //push the type that the word represents

			printf("pushing %s\n",  w->val.as.ptr.address.type->key );
		
			push_assembly(pctx, op_constant, 0, v, tType, NULL, tType);
			//typestack_push_value(pctx, tType, &v);  //and that value we just pushed, is a type
			//t->type = tType;
			return t;
	}

	errorf("Unknown type\n");
	exit(1);
}

tokenT* parse_reference_type(exectxT* exe, parsectxT* pctx, wordT* w, tokenT* t, int argc){

	//typestack_pop(pctx, argc);
	instructionT* arg0 = pop_arg(pctx);

	typeT* type = arg0->val.as.ptr.address.type;

	if (type){		//if the type is a constant, get it

		if (!strcmp(t->str, "&"))
			type = type_find(pctx, NULL, type, REFERENCE, 0, NULL);
		if (!strcmp(t->str, ".var"))
			type = type_find(pctx, NULL, type, VARIABLE, 0, NULL);

	}

	valueT v = {0};
	v.as.ptr.address.type = type;
	//arg is the 'arglist' but op_constnat does not run its args.
	//the arglist is just to keep track of where the constant came from
	push_assembly(pctx, op_constant, 0, v, tType, assembly_args_mk(1, arg0) , tType);


	return t;
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
		codestack_print(pctx->codestack, ZTRUE);
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
		wordT* w = match_word(pctx, pctx,  t->str, MATCH_RECURSE , &argc);

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

				push_assembly(pctx, op_subvar, 0, v, tWord, assembly_args_mk(1, pop_arg(pctx))  , w->type);
				DEBUG_MATCH=1;

				if (t&& tnext(t) && !strcmp(tnext(t)->str , "&")){
					valueT nv={0};
					instructionT** largs = pop_args(pctx, 1);
					instructionT* inst = push_assembly(pctx, op_reference, 0, nv, NULL, largs, OF(pctx, REFERENCE, w->type->ref)      );
					inst->comment = zstrdup(w->comment);
					t = tnext(t);

				}

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
		//printf(" Freeing type %s\n", type->key);
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
		return zstrprintf(NULL, "like-%s", name);
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
				return zstrprintf(NULL, "%s&", ref->key);

			case VARIABLE:
				return zstrprintf(NULL, "var(%s)", ref->key);

		}

		//other cases not handled
		return zstrprintf(NULL, "%s:C%d.%d", ref->key, category, size);
	}

	//types that just have a name
	if (name)
		return zstrdup(name);

	//these ones shouldn't happen:
	return zstrprintf(NULL, "bad/%d/%d", category, size);

}
wordT* proc_parser_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ret, typeT** args, void* parsefunc);

//create a type.  takes ownership of 'args' if passed
typeT* type_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args ){
									
	typeT* type = ram_alloc(sizeof(typeT), typecleanup);

	type->name = zstrdup(name);

	type->ref = ram_addref(ref);

	type->category = category;
	type->size = size;
	type->key = type_key(name, ref, category, size, args);
	debugf("Creating type %s   %s\n", name?name:"noname", type->key);

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
				wordT* w = proc_parser_mk(exe, pctx, name, tType, NULL, parse_type_name);//create a proc word, that runs parse_type_name at parse time
				type->word =w; //the type needs a word to find it
				w->val.as.ptr.address.type = type;
				w->val_type = tType; //is a type
			}
		}
	} else printf("creating type with no pctx\n");

	return type;
}



typeT* type_find(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args){

	typeT* type = NULL;

	//try to find directly
	//this will take care of
	char* key = type_key(name, ref, category, size, args);

	type = zstringmap_get(pctx->types, key);

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
	if (category == REFERENCE || category == PROC || category == VARIABLE || category == LOADED ){
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




//create array of types
typeT** type_args_mk(int n, ...){


	va_list args;

	if (n==0)
		return NULL;

	typeT** types = zarray_alloc( typeT*, n);

	if (n){

		va_start(args, n);
		for (int i=0;i<n;i++){
			types[i] = va_arg(args, typeT*);	//get next arg
		}
		va_end(args);
		zarray_use(types, n);
	}

	return types;
}


wordT* proc_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ret, typeT** args){

		typeT* type = type_find(pctx, NULL, ret, PROC, 0, args);
		wordT* procword= word_mk(exe, pctx, name, type);

		//add the args to the word's context (so they can act like vars)
	//	if (args) {


	//	}


		return procword;
}

wordT* proc_opcode_mk(exectxT* exe, parsectxT* pctx,  int opcode, char* name,  typeT* ret, typeT** args){
		wordT* w = proc_mk(exe, pctx, name, ret, args);
		iferr(exe) {
			return NULL;
		}
		 w->opcode = opcode;
		w->comment = zstrdup(name);
		return w;
}

wordT* proc_parser_mk(exectxT* exe, parsectxT* pctx, char* name, typeT* ret, typeT** args, void* parsefunc){
		wordT* w = proc_mk(exe, pctx, name, ret, args);
		iferr(exe) {
			return NULL;
		}
		w->parse = parsefunc;
		return w;
}

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

	return it->val.as.ptr.address.type;


}


#define ADD_INST(NAME)	instruction_names[ op_ ## NAME ] = #NAME;


//tokenize and parse (evnetually... run)
void int_run_str(char* src, char* filename){
	instruction_names = zarray_alloc( char*, op_MAX);
	ADD_INST(constant);
	ADD_INST(globalvar);
	ADD_INST(load);
	ADD_INST(subvar);
	ADD_INST(store);
	ADD_INST(reference);
	ADD_INST(handler);
	ADD_INST(print32);
	ADD_INST(add32);
	ADD_INST(call);
	ADD_INST(return);
	ADD_INST(returnval);
	parsectxT* pctx= parsectx_mk(NULL);
	exectxT* exe = exectx_mk();

	//create basic types
	tType = type_mk(exe, pctx, "Type", NULL, OPAQUE, sizeof(typeT*), NULL);
	tZ32  = type_mk(exe,pctx, "Z32",  NULL, SIMPLE, sizeof(zint32), NULL);
	tReal = type_mk(exe,pctx, "Real", NULL, SIMPLE, sizeof(FLOAT),  NULL);
	tBit = type_mk(exe,pctx, "Bit", NULL, SIMPLE, sizeof(int),  NULL);
	tWord = type_mk(exe,pctx, "Word", NULL, OPAQUE, sizeof(wordT*),  NULL);
	tAny =  type_mk(exe, pctx, "Any", NULL, OPAQUE, 0, NULL);  //'Any' does not have a size (but any& does)



	//create primitives. minimal at the moment
	proc_opcode_mk(exe,pctx, op_print32, "print", NULL, type_args_mk(1, tZ32));
	proc_opcode_mk(exe,pctx, op_add32, "+", tZ32, type_args_mk(2, tZ32, tZ32));


	//create parser words
	proc_parser_mk(exe,pctx, "(", NULL, NULL, parse_type_list);
	proc_parser_mk(exe,pctx, "&", NULL, type_args_mk(1, tType), parse_reference_type); //makes a type into a reference type

	proc_parser_mk(exe,pctx, ".var", NULL, type_args_mk(1, tType), parse_reference_type); //makes a type into a var type

	typeT* tt= type_from_str(exe, pctx,  "(Z32:a Z32:b ->Z32");
	type_print(tt);
	//printf("%s\n", tt->key);
	exit(1);


	//define variables, types and procs
	proc_parser_mk(exe,pctx, "var", NULL, type_args_mk(1, tType), parse_var); //makes a variable in current context
	proc_parser_mk(exe,pctx, "type", NULL, NULL, parse_frame);
	proc_parser_mk(exe,pctx, "proc", NULL, type_args_mk(1, tType), parse_proc);


	proc_opcode_mk(exe,pctx, op_store, "=", NULL, type_args_mk(2, tZ32, OF(pctx, VARIABLE, tZ32 ) ));



//	proc_opcode_mk(exe,pctx, op_reference, "&", OF(pctx, REFERENCE, tZ32), type_args_mk(1, OF(pctx, LOADED, tZ32 ) ));


	//load integers
//	proc_opcode_mk(exe,pctx, 888, "@", tZ32, type_args_mk(1, OF(pctx, VARIABLE, tZ32)));




	dump_dictionary(pctx);
	dump_types(pctx);

    zlistT tokens;
    zlist_init(&tokens);

    tokenT* first = token_mk(TOKEN_STARTFILE, NULL, 0);
    zlist_addhead(&tokens, &(first->zlistnode));

    tokenize(first, src, filename, 1);

    parse(exe, pctx, zlist_head(&tokens), NULL);

	codestack_print(pctx->codestack, ZTRUE);

	iferr(exe){
		fprintf(stderr, "Error(%d): %s\n", exe->error_code, exe->error_string);
	}

	ifok(exe){
		runnerI* runme = compile_for_switch(exe, pctx);
		runme->execute(exe, runme, 0);
		//ram_free(runme); //runner is part of the pctx, and is freed there
	}
//	dump_dictionary(pctx);
//	dump_types(pctx);

    zlist_cleanup(&tokens);
	ram_free(pctx);
	ram_free(exe);
	ram_free(instruction_names);
}
