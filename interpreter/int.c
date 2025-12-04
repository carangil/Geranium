#include "int.h"
#include "string.h"
#include "stdarg.h"

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
	return ZTRUE;
}

tokenT* token_mk(zuint32 tok, char* str, zuint32 len){
	tokenT* t = ram_alloc( sizeof(tokenT) , token_cleanup );
	t->tok = tok;

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
//filname is optionsl

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




void print_token_list(zlistT* li, int level){
    tokenT* t =  (tokenT*) zlist_head( li);

    while(t){
        printf(" %x\t%c\t%s\n",
               t->tok,
               (t->tok > ' ' && t->tok < 127)? t->tok : '.' ,
               t->str?t->str:"nil");
        t = tnext(t);
    }

}


//dictionary

zbool cleanWord(void* v){
	wordT* w = v;
	ram_free(w->name);
	ram_free(w->type);
}

static int wordid = 0;

wordT* word_mk(parsectxT* pctx, char* name, typeT* type){

	wordT* w = ram_alloc(sizeof(wordT), cleanWord);
	w->name = zstrdup(name); //note: null string will give a valid empty string non-null result

	w->type = ram_addref(type);
	

	if (pctx){

		//add to the dictionary
		zvecT* p =  zstringmap_get(pctx->dictionary, name);
		if (!p){
				p = zvec_mk(NULL, 10);	//make new vector
				zstringmap_put(pctx->dictionary, name, p);
		}

		zvec_add(p, w);

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

void type_print(typeT* t){
	if (!t)
		return;

	switch (t->category){
		case PENDING:
			printf("pending ");
			break;
	}
	printf("%s\n", t->key);

	if (t->members){
		char*	name=NULL;
		typeT* 	type = NULL;
		void*	cursor=NULL;
		while (zstringmap_nextkey(t->members, &name, &type, &cursor)) {
			printf(". %s %s\n", name, type->key);
		}
	}

}

void dump_types(parsectxT* pctx){
	char*	name=NULL;
	typeT* 	type = NULL;
	void*	cursor=NULL;
	printf(" types:\n");
	while (zstringmap_nextkey(pctx->types, &name, &type, &cursor)) {

		type_print(type);

	}
}

//most basic types
typeT *tZ32, *tType, *tReal, *tType, *tFloat; 


//Compare two types to see if they are compatible/equivalent
zbool type_cmp(typeT* t1, typeT* t2, int flags){

	if (t1 == t2)
		return ZTRUE;
	else
		return ZFALSE;

}

//t is the token trying to be matched
//tprev(t) is the last arg to it, if there are any

wordT* match_word(parsectxT* pctx,  char* name, int flags, int* rarg ){

	int argc = 0;
	int maxarg = pctx->sp; //most args possible is whole stack

	wordT* word = NULL;

	//get all the words with this name
	zvecT* words = zstringmap_get(pctx->dictionary, name);

	if (!words)
		return NULL; //completely unknown word
	
	for (argc=0;argc <= maxarg; argc++){

		debugf(" try to match %s with %d args\n", name, argc);

		for (int i=0;i< zvec_count(words); i++){

			word = zvec_get_at(words, i);

			if (argc != word->type->argc){
				debugf("Skipping mismatch arg count %d %d\n", argc, word->type->argc);
				continue;
			}
		
			if (argc==0 && word->type->argc==0){
				debugf("Word %s has no members, name matches, returning it\n", word->name);
				*rarg = 0;
				return word;
			}

			//try to match args	
			void* csr=NULL;
			zbool match = ZTRUE;
			for (int i=pctx->sp - argc; i<pctx->sp; i++){
				char* key;
				typeT* argtype;
				if (!zstringmap_nextkey( word->type->members, &key, &argtype,&csr))				{
					match = ZFALSE;
					errorf("Out of parameters!\n");

					break;
				}


				debugf("Compare sp%d  %s %s %s\n", i, key, argtype->key, pctx->typestack[i]->key);
				if (!type_cmp( argtype, pctx->typestack[i], 0)){
					match = ZFALSE;
					break;
				}
			

			} //for comparing args

			if (match){
				*rarg = argc;
				return word;

			}



		}//for words

		
	} //for argc

	return NULL;

}

int typestack_pop(parsectxT* pctx, int n){
	pctx->sp--;
	return pctx->sp;
}
int typestack_push(parsectxT* pctx, typeT* type){
	pctx->typestack[pctx->sp] = type;
	pctx->valuestack[pctx->sp++].as.ptr.address.block = NULL; //don't want garbage pointers around
	return pctx->sp;
}
int typestack_push_value(parsectxT* pctx, typeT* type, valueT* val){
	pctx->typestack[pctx->sp] = type;
	pctx->valuestack[pctx->sp++] = *val;
	return pctx->sp;
}


tokenT* parse_number(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	if (strchr(t->str, '.')){
		debugf("FLOAT %s \n", t->str);
		t->type = tReal;
	} else if (strchr(t->str, 'x') == (t->str +1 ) ){
		debugf("HEX %s \n", t->str);
		t->type = tZ32;
	} else {
		debugf("DEC %s \n", t->str);
		t->type = tZ32;
	}

	typestack_push(pctx, t->type);
	return tnext(t);
}

tokenT* parse_default(parsectxT* pctx, wordT* w, tokenT* t, int argc){

	typestack_pop(pctx, argc);

	if (w->type->category == WORD) {
		t->type = w->type->ref; //token will have the return value
		if (w->type->ref){ //if function has return value
			typestack_push(pctx, w->type->ref);
		}
		return tnext(t);
	}
	errorf("Don't know what to do\n");
}

typeT* type_find(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args);

void parse(parsectxT* pctx, tokenT* t){
	//skip past this stub
	if (t->tok == TOKEN_STARTFILE){
		t=tnext(t);
	}

	while(t){
		//print type stack
		for (int i=0;i<pctx->sp;i++){
			printf("%s ", pctx->typestack[i]->key);
		}
		printf("\n");

		switch (t->tok){
			case TOKEN_STARTFILE:
				t=tnext(t);
				continue;

			case TOKEN_NUMBER:
				t = parse_number(pctx, NULL, t, 0);
				continue;
		}

		//to match
		int argc=0;
		wordT* w = match_word(pctx,  t->str, 0 , &argc);

		if (w) {
			printf("found word %s %s\n", w->name, w->type?w->type->key:"notype"   );

			if (!w->parse){
				t = parse_default(pctx, w, t, argc);
				continue;
			}

		}
		//did not match a word
		//put it on the typestack as an unquoted
		typestack_push(pctx, type_find(pctx, t->str, NULL, LITERALTOKEN, 0, NULL));
		t=tnext(t);	

	}

}
//types

zbool typecleanup(void* v){
		typeT* type = v;
		ram_free(type->ref);
		ram_free(type->key);
		ram_free(type->name);
		ram_free(type->members);
		return ZTRUE;
}

char* type_key(char* name, typeT* ref, categoryE category, int size, typeT** args){
	char* key = NULL;

	//'function' types.. These don't have a name
	if (category == WORD){
		key = zstrdup("(");
		if (args) {
			for (int i=0; args[i];i++){
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

		}

		//other cases not handled
		return zstrprintf(NULL, "%s:C%d.%s", ref->key, category, size);
	}

	//types that just have a name
	if (name)
		return zstrdup(name);

	//these ones shouldn't happen:
	return zstrprintf(NULL, "bad/%d/%d", category, size);

}

typeT* type_mk(parsectxT* pctx, char* name, typeT* ref, categoryE category, int size, typeT** args ){ 
									
	typeT* type = ram_alloc(sizeof(typeT), typecleanup);

	type->name = zstrdup(name);
	type->ref = ram_addref(ref);

	type->category = category;
	type->size = size;
	type->key = type_key(name, ref, category, size, args);
	debugf("Creating type %s   %s\n", name?name:"noname", type->key);

	if (args){
		int argc=0;
		type->members = zstringmap_mk(8);

		char name[2];
		name[0]='a';
		name[1]=0;

		while(args[argc]){
			printf("adding %s  %p %d\n", name, args[argc], argc);
			zstringmap_put(type->members,name, ram_addref(args[argc]));
			name[0]++;
			argc++;
		}
		type->argc=argc;
	}
	
	if (pctx){
		if (zstringmap_get(pctx->types, type->key))
			errorf("There is already a type entry for %s\n", type->key);
		else
			zstringmap_put(pctx->types, type->key, type);
	}

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
		if (category != NAMED && category != type->category){
			errorf("Type is not expected category %d\n", category);
		}	
		ram_free(key);
		return type;
	}

	//create a new type if we have to
	if (category == LITERALTOKEN){
		type = type_mk(pctx, name, ref, category, size, args);
	} else if (category == REFERENCE || category == WORD ){
		type = type_mk(pctx, NULL, ref, category, size, args);
	} else if (category == NAMED){
		//wanted a type by name (like a struct), but the type is not yet
		//defined.
		type = type_mk(pctx, name, NULL, PENDING, 0, NULL);
	}

	ram_free(key);

	//if we made a type return it

	return type;

}


typeT* type_struct_mk(parsectxT* pctx, char* name){ 

	//so if there is a pending type
	typeT* type = type_find(pctx, name, NULL, PENDING, 0, NULL);
	
	//if not create it
	if (!type)
		type = type_mk(pctx, name, NULL, PENDING, 0, NULL);

	if (!type->members){
		type->members = zstringmap_mk(8);
	}
	return type;	
}

zbool type_struct_field(typeT* type, char* name, typeT* fieldType){

	if (zstringmap_get(type->members, name)){
		errorf("Type %s already has %s\n", type->name, name);
		return ZFALSE;
	}

	zstringmap_put(type->members, name, ram_addref(fieldType));
	
	return ZTRUE;	
}

typeT* type_struct_complete(typeT* t){
	if (t->category == PENDING){
		t->category = STRUCT;
		return t;
	}
	errorf("Can't finalize %s\n", t->key);
	return NULL;
}

//parse context
zbool cleanparsectx(void* v){
	parsectxT* pctx = v;
	ram_free(pctx->dictionary);
	ram_free(pctx->types);
	ram_free(pctx->typestack);
	ram_free(pctx->valuestack);

	return ZTRUE;
}

parsectxT* parsectx_mk(parsectxT* parent){

	parsectxT* pctx	= ram_alloc(sizeof(parsectxT), cleanparsectx);
	pctx->dictionary = zstringmap_mk(64);
	pctx->types = zstringmap_mk(64);
	pctx->typestack = zarray_alloc(typeT*, 128);
	pctx->valuestack = zarray_alloc(typeT*, 128);

	return pctx;
}

/*
void printTypeList(int n, ...){
	va_list args;
	va_start(args, n);
	for (int i=0;i<n;i++){
		type_print(va_arg(args, typeT*));
	}
	va_end(args);
	exit(2);
}
*/



//tokenize and parse (evnetually... run)
void int_run_str(char* src, char* filename){

	parsectxT* pctx= parsectx_mk(NULL);

	tType = type_mk(pctx, "Type", NULL, OPAQUE, sizeof(typeT*), NULL);
	tZ32 = type_mk(pctx, "Z32",  NULL, SIMPLE, sizeof(zint32), NULL);
	tReal = type_mk(pctx, "Real", NULL, SIMPLE, sizeof(FLOAT), NULL);
	typeT* retz = type_find(pctx, NULL, tZ32, WORD, 0, NULL);
	
	//make a word that returns z32
	
	wordT* ten = word_mk(pctx, "ten", retz);

	//	ten->parse = parse_ten;

	type_find(pctx, "Z32", NULL, NAMED,0, NULL);
	type_find(pctx, NULL, tZ32, REFERENCE,0, NULL);
	
	typeT* z[] = { tZ32, NULL};	
	typeT* arg_z = type_mk(pctx, NULL, NULL, WORD, 0, z);
	word_mk(pctx, "print", arg_z);


	typeT* zz[] = { tZ32, tZ32, NULL};	
	type_find(pctx, NULL, tZ32, WORD,0, zz );

//	typeT* s = type_find(pctx, "cats", NULL, NAMED, 0, NULL);
	typeT* s = type_struct_mk(pctx, "cats");
	type_struct_field(s, "aa", tZ32);
	type_struct_field(s, "cacc", tZ32);
	type_struct_field(s, "cc", tZ32);
	type_struct_field(s, "BB", tZ32);
	type_struct_complete(s);
	type_print(s);

//	type_find(pctx, NULL, z, REFERENCE,0, NULL);

	dump_dictionary(pctx);
	dump_types(pctx);

    zlistT tokens;
    zlist_init(&tokens);

    tokenT* first = token_mk(TOKEN_STARTFILE, NULL, 0);

    zlist_addhead(&tokens, &(first->zlistnode));

    tokenize(first, src, filename, 1);

    print_token_list(&tokens, 0);

    parse(pctx, zlist_head(&tokens));

    zlist_cleanup(&tokens);
	 ram_free(pctx);

}
