#include <stdio.h>
#include <string.h>
#include "ztypes.h"
#include "zvector.h"
#include "zstring.h"
#include "zmem.h"


FILE* outc = NULL;
FILE* outz = NULL;

typedef struct argS{
	char* ctype;
	char* name;
	int isunsigned;
	int pointer;
}argT;

typedef struct mapping {
	char* ctype;
	int pointer;
	char* ztype;
	int noproto;
	int is_counted;
	int is_byteobj;
} mappingT;
zvecT *typemap;

mappingT* mkMapping(char* ctype, int pointer,  char* ztype, int noproto, int is_counted, int is_byteobj) {
	mappingT* m = ram_alloc(sizeof(mappingT), NULL);

	m->ctype = zstrdup(ctype);
	m->noproto = noproto;  //only for functions, to skip creating a prototype in the generated code
	
	if (noproto && (ztype == NULL) || (strlen(ztype) == 0)) {
		ztype = ctype; //same name
	}

	m->ztype = zstrdup(ztype);

	m->pointer = pointer;
	m->is_counted = is_counted;
	m->is_byteobj = is_byteobj;

	zvec_add(typemap, m);
	return m;
}

zbool argT_cleanup(void* v){
	argT* a = v;
	

	ram_free(a->ctype);
	ram_free(a->name);
	
	return ZTRUE;
}


char* handler_template_start = 
"tokenT* hc_%s (exectxT* ex, tokenT* t) {	\n"
"	exe(ex, tsub(t));			\n";
	
//	ex->stack[ex->sp-2].as.z32 += ex->stack[ex->sp-1].as.z32;
//	ex->sp--;

char* handler_template_end = 
"	return tnext(t); \n"
"}\n";



char* get_as(char* type, int isunsigned){
	
	if (!type)
		return NULL;
		
	//for now, only the 32-bit type is supported
	//will truncate as needed
	if (isunsigned){
		if (!strcmp(type, "char"))
			return "n32";
		if (!strcmp(type, "int"))
			return "n32";
		if (!strcmp(type, "short"))
			return "n32";
	}
	
	if (!strcmp(type, "char"))
		return "z32";
	if (!strcmp(type, "int"))
		return "z32";
	if (!strcmp(type, "short"))
		return "z32";
	
	if (!strcmp(type, "float")|| !strcmp(type, "zfloat32"))
		return "f";
	
	fprintf(stderr, " Unknown type, assume z32: %s \n", type);
	
	
	return "z32";
	
}

zvecT* collected = NULL;
zvecT* fnames = NULL;
zvecT* pendingz = NULL;


char* get_zz(char* type, int isunsigned, int pointer, int isreturn, int* noproto) {

	if (!type)
		return NULL;


	int i;

	//special cases

	if ((pointer == 1) && (!strcmp(type, "char") || (!strcmp(type, "zchar")))) {

		if (isreturn) {
			printf(" Returning string object from C is not supported yet.  Need to copy to zstring\n");
		}
		else {
			return zstrdup("String&");
		}
	}

	mappingT* selected = NULL;

	for (i = 0; i < zvec_count(typemap); i++) {

		mappingT* m  = zvec_get_at(typemap, i);

		if (!strcmp(type, m->ctype)   && (pointer == m->pointer) ) {
			selected = m;
			break;
		}

	}

	if (selected) {

		if (noproto)
			*noproto = selected->noproto;

		if (selected->is_counted || selected->is_byteobj)
		{
			char* s = zstrdup(selected->ztype);
			if (isreturn)
				return zstrcat(s,"%"); //default possessive pointer for returned cdata
			else
				return zstrcat(s,"&"); //default reference pointer for arg cdata (overriding can be done by user specifying a full function prototype)
		}

		return zstrdup(selected->ztype);
	}
	
	if (pointer) {
		//if an unknown pointer type, make a cpointer of the same name
				
		mappingT* m = mkMapping(type, 1, type, 0,0,0);

		char* s = zstrndup("cpointer ", 24);
		s = zstrcat(s, type);
		
		
		//zvec_add(pendingz, s);

		return zstrdup(s);

		//fprintf(outz, "cpointer %s;\n", type);
	}

	char* s = zstrdup("Unknown_");
	return zstrcat(s, type);
	
	
}



void create_handler_function(int isunsigned, char* return_type, int return_pointer, char* name , zvecT* args){
	
	//output prototype to gen header
//	fprintf(outc, "%s;\n", buf);



	fprintf(outc, handler_template_start, name);
	int returnsavalue = 0;
	int argcount = zvec_count(args);
	char* as = NULL;
		
	if ( (!strcmp(return_type, "void"))&&(!return_pointer))
		return_type = NULL;
	
	
	fprintf(outc, "	void* firstArg = ex->stack[ex->sp-%d].as.ptr.block;\n", argcount);

	//set destination
	if (return_pointer){
		fprintf(outc, "	ex->stack[ex->sp-%d].as.ptr.block= ", argcount);
	}
	else {
		as = get_as(return_type, isunsigned);
		if (as)
			fprintf(outc, "	ex->stack[ex->sp-%d].as.%s =\n		", argcount, as);
		else
			fprintf(outc, "		");
	}
	
	//do function call
	fprintf(outc, "%s(\n			", name);
	
	
	zvec_add(fnames, zstrdup(name));
	
	int j;
	
	for (j=zvec_count(args)-1;j>=0; j--){
		
		argT* arg= zvec_get_at(args, j);
		
		if (arg->pointer){
			fprintf(outc, "(void*)((ex->stack[ex->sp-%d].as.ptr.block)+(ex->stack[ex->sp-%d].as.ptr.offset))" , j+1, j+1 );
		}
		else{
			as = get_as(arg->ctype, arg->isunsigned);
			fprintf(outc, "ex->stack[ex->sp-%d].as.%s", j + 1, as);
		}
		
		if (j!=0)
			fprintf(outc, ",\n			");
		
		
		
		//printf("%s  %s  *:%d\n", arg->name, arg->ctype, arg->pointer);
		
	}
	
	
	//end function call
	fprintf(outc, ");\n");
	
	
	if (return_pointer){
		fprintf(outc, "	ex->stack[ex->sp-%d].as.ptr.level=0; \n", argcount);
		fprintf(outc, "	ex->stack[ex->sp-%d].as.ptr.offset=0; \n", argcount);
	}
	
	fprintf(outc, "\n	cleanCCall(ex, t, firstArg);\n");


	//adjust stack pointer
	fprintf(outc, "\n	ex->sp+= (-%d+%d);\n", zvec_count(args), return_type? 1:0 );
	
	fprintf(outc, "\n%s", handler_template_end);
		
}


void create_primitive(int isunsigned, char* return_type, int return_pointer, char* name, zvecT* args) {


	char* nickname = name;

	int i;
	for (i = 0; i < zvec_count(typemap); i++) {
		mappingT* m = zvec_get_at(typemap, i);
		if (!strcmp(m->ctype, name)) {
			nickname = m->ztype;
			break;
		}
	}

	if (strchr(nickname, '(')) {
		//nickname could contain a full parameter list already. in that case just print it and don't generate
		//in this case nickname should already end with ';'
		fprintf(outz, "primitive C_%s %s\n", name, nickname);
		return;
	}

	fprintf(outz, "primitive C_%s %s:(", name, nickname);
	if ((!strcmp(return_type, "void")) && (!return_pointer))
		return_type = NULL;

	int j;
	char* tmp = NULL;

	for (j = zvec_count(args) - 1; j >= 0; j--) {

		argT* arg = zvec_get_at(args, j);

		if (j != zvec_count(args)-1)
			fprintf(outz, ";");

		fprintf(outz, "c_%s:%s", arg->name, tmp=get_zz(arg->ctype, arg->isunsigned, arg->pointer, ZFALSE,0) );
		
		ram_free(tmp);
	}

	fprintf(outz, "->");


	if (return_type ) {
		fprintf(outz, "%s", tmp=get_zz(return_type, isunsigned, return_pointer, ZTRUE,0));
		ram_free(tmp);
	}
	fprintf(outz, ");\n");

}
char* to_chars(char* s, char* to);

void process_line(char* buf){
	

	int i;
	
	char b[128];
			
	
	if (!strchr(buf, '(')){
		fprintf(stderr, " skip non-prototype .. ( is required to define a function\n");
		return;
	}
	
	fprintf(stderr," CANDIDATE PROTOTYPE: %s\n", buf);
	
	char* proto = zstrdup(buf);
	
	zvecT* args = zvec_mk(NULL, 10);
		
	int pos=0;
	int pstart=0;
	int ns=0;
	for (i=strlen(buf)-1 ;i>=0;i--){
	
		
		if (  (buf[i]==')' ||buf[i]==' ') && !ns) {  //if haven't seen a space or ) yet, remove spaces from end of string
			buf[i]=0;
			continue;
		}
		
		ns=1;
		int isunsigned =0;
		
		if (buf[i]=='(' || buf[i]==','){
			
			if (buf[i] =='(')
				pstart=i;
			
			fprintf(stderr, " arg |%s|\n", buf+i);
			buf[i++]=0;
			ns=0;
		
			if (buf[i]==' ') //skip space
				i++;
			
			if ( buf[i] == 0) {
				fprintf(stderr, " NO ARGS\n");
				continue;  //no args
			}

			
			while(1){
				
				pos = i;
				while(buf[i] && buf[i]!=' ' && buf[i] !='*')
					i++; //go until space or star
					
				if (!strncmp("struct", buf + pos, i - pos)) {
					//skip word 'struct'
					i++; 
					continue;
				}
				
				if(!strncmp("unsigned", buf+pos, i-pos)){
					fprintf(stderr, " skip unsigned\n");
					isunsigned=1;
					i++;
					continue;
				}
				
				break;
			}
			
			char* typename = zstrndup(buf+pos, i-pos);
			
			fprintf(stderr, " NAME IS <%s>  unsigned:%d\n", typename, isunsigned);
			
			
			argT* arg = ram_alloc(sizeof(argT), argT_cleanup);
			arg->ctype = typename;
			arg->isunsigned = isunsigned;
			zvec_add(args, arg);
			
			while(buf[i]=='*' || buf[i]==' '){
				if (buf[i]=='*'){
					arg->pointer++;
					fprintf(stderr, " pointer\n");
				}
				i++;
			}
			
			buf[i-1] = 0;

			pos = i;
			while(buf[i] !=',' && buf[i]&& buf[i]!=')' && buf[i]!=' ' ){
				i++;
			}
			buf[i]=0;
			fprintf(stderr, " varname '%s'\n", buf+pos);
			
			if (strlen(buf+pos)){
				arg->name = zstrdup(buf+pos);
			} else {
				char tname[20];
				sprintf(tname, "arg__%d", zvec_count(args));
				arg->name = zstrdup(tname);
			}
		}
		
		if (pstart){
			
			if ( buf[pstart-1] == ' ')
				pstart--;
			
			i=pstart;
			buf[i]=0;
			while( i>=0){
//				printf(" %c %d <- \n", buf[i], buf[i]);
				if ((buf[i]==' ') || (buf[i]=='*'))
					break;
				i--;
			}
			
			fprintf(stderr," Function name <%s>\n", buf+i+1); 
			char* fname = buf+i+1;
	

			int noproto = 0;
			get_zz(fname, 0, 0, 0, &noproto);

			if (!noproto)
				fprintf(outc, "%s;\n", proto);


			//skip any spaces and stars
			int pointer=0;
			while( (i>=0) &&( (buf[i]=='*') || (buf[i]==' '))){
				
				if (buf[i]=='*') {
					fprintf(stderr," rpointer\n");
					pointer++;
				}
				i--;
			}
			pos =i;
			buf[i+1]=0;
			while((i>=0) && buf[i]!=' ')
				i--;
			
			fprintf(stderr," return type <%s>\n", buf+i+1); 
			
			int isunsigned=0;
			
			if ((  buf+i+1-9 >=0) &&(!strncmp(buf+i+1-9, "unsigned", 8)))
				isunsigned=1;
			
			
			
			
			create_handler_function(isunsigned, buf+i+1, pointer, fname, args);
			int j;
			for (j=zvec_count(args)-1;j>=0; j--){
				
				argT* arg= zvec_get_at(args, j);
				
				fprintf(stderr, "%s  %s  *:%d\n", arg->name, arg->ctype, arg->pointer);
				
			}
			
			create_primitive(isunsigned, buf + i + 1, pointer, fname, args);
			
			break;
			
		}
		
		
	}

	
	ram_free(args);
	
}



char* to_chars(char* s, char* to) {
	char* c;

	//starting with non-space character, consume until end of string or a space.  Returned pointer is the space or end of string
	while (s && *s) {
		for (c = to; *c; c++)
			if (*c == *s)
				return s;
		s++;
	}
	
	return s;
}


char* eat_chars(char* s, char* eat) {
	
	char* c;
	//starting with space character, consumes until non-space
	while (s && *s) {
		int a = 0;
		for (c = eat; *c; c++)
			if (*c == *s){
				a = 1;
				break;
			}

		if (!a)
			return s;
		s++;
	}

	return s;
}



//buf:define WORD VALUE
int process_define(char* buf) {

	printf(" DEF0 '%s'  %d %d\n", buf, buf[strlen(buf)-2], buf[strlen(buf)-1]);
	//if (strchr(buf, '(')) {
	//	//skip function-like macros
	//	return;
	//}
	printf(" DEF1 '%s'\n", buf);

	char* word = buf + 6;
	word = eat_chars(word, " \t\n\r");
	
	printf("DEF1.5: word:'%s'\n", word);

	char* definition = to_chars(word, " \t\n\r");

	if ((*definition == '\n') || (*definition =='\r')) //end of line... end the string
		*definition = 0;
	
	if (*definition ) {

		*definition = 0;
		definition++;
		definition = eat_chars(definition, " \t");
		char* defend = to_chars(definition, "\n\r \t");
		*defend = 0;
	}

	printf(" DEF2 '%'s '%s'\n", word, definition);

	if (strchr(word, '('))
		return;  //skip function-like macros


	//#define Structmap_zeventT type=eventType:Z32;a=A:Z32;b=B:Z32;
	int is_tm = 0;
	int np = 0;
	int is_counted = 0;
	int is_byteobj = 0;
	char tmp[100];

	if ((is_byteobj = !strncmp(word, "ByteObj_", 8)) || (is_tm = !strncmp(word, "Typemap_", 8)) || (is_counted = !strncmp(word, "Counted_", 8)) || (!strncmp(word, "Procmap_", 8)) || (np = !strncmp(word, "Noproto_", 8))) {
		int ptr = 0;
		word += 8;


		printf(" --- '%s'   '%s'  '%d'\n", word, definition, np);


		while (*definition == '*')
			ptr++, definition++;



		if (!np && (strlen(definition) == 0)) {
			//skip empty definitions
			return;
		}


		if (is_tm)
			fprintf(outz, "cpointer %s;\n", definition);

		if (is_counted)
			fprintf(outz, "cdata %s;\n", definition);

		if (is_byteobj) {
			fprintf(outz, "cdata @%s %s;\n", word, definition);
			snprintf(tmp, sizeof(tmp), "\taddCSize(\"%s\", sizeof(%s));\n", word, word);
			zvec_add(collected, zstrdup(tmp));

		}

		mkMapping(word , ptr, definition, np, is_counted, is_byteobj);
		return;
	}

	//import a C structure into ZZ, keeping C object size and memory layout.
	//This makes it easy for the interpreter to access structs shared with C, without trying to re-write the equivalent structure directly.
	//If the C layout changes (padding, size of c pointers, etc), the interpreter's layout will always match, because it uses 'offsetof'
	if (!strncmp(word, "Structmap_", 10)) {
		int ptr = 0;
		word += 10;

		printf("Need to process %s as %s\n", word, definition);

		

		char* zname = definition;
		definition = strchr(definition, ':');

		if (definition) {
			*definition = 0;
			definition++;

			//printf("\t ty = mkType(PENDING, NULL, \"%s\", 0);\n", zname);

			fprintf(outz, "type @%s %s\n", word, zname);


			//Add the mapping from C pointer  cname*  to a user pointer  zname&
			char znamep[100];
			snprintf(znamep, sizeof(znamep), "%s&", zname);
			mkMapping(word, 1, znamep,0, 0,0);

			snprintf(tmp, sizeof(tmp), "\taddCSize(\"%s\", sizeof(%s));\n", word, word);
			zvec_add(collected, zstrdup(tmp));

			while (*definition) {
				char* oldname = definition;
				char* newname = strchr(definition, '=');
				if (newname) {
					*newname = 0;
					newname++;
					char* type = strchr(newname, ':');
					if (type) {
						*type = 0;
						type++;
						definition = strchr(type, ';');
						if (definition) {
							*definition = 0;
							definition++;
							//printf(" /%s/  /%s/  /%s/\n", oldname, newname, type);
							fprintf(outz, "\t@%s_%s\t%s:%s;\n", word, oldname, newname, type);

							snprintf(tmp, sizeof(tmp), "\taddCSize(\"%s_%s\", offsetof(%s,%s));\n", word, oldname, word, oldname );
							zvec_add(collected, zstrdup(tmp));

						}
					}
				}
			}

			fprintf(outz, "end\n");

		}

		return;
	}

	//fprintf(outz, "constant %s %s;\n", word, definition); 


	

	int num = atoi(definition);

	if (num == 0) {
		//try hex
		num = strtol(definition, NULL, 16);
	}

	//instead of making a constant, just make it variable for now
	fprintf(outz, "%d #%s \n",  num, word);

}

int scanmain (int argc, char** args){
	int bracelevel=0;
	char lc=' ';
	
	if(argc<2)
		return 1;

	int curfile = 0;
	zvecT* filenames = zstrsplit(NULL, args[1], ',');

	{
		int j;
		for (j = 0; j < zvec_count(filenames); j++) {
			char* x = zvec_get_at(filenames, j);
			printf(" TO scan %s\n", x);
		}
	}

	FILE* f = fopen(zvec_get_at(filenames, curfile), "rb");

	if (!f)
		return 2;
	
	
	if (argc > 2)
		outc = fopen(args[2], "wb");
	
	if (outc == NULL)
		outc = stdout;
	

	if (argc > 3)
		outz = fopen(args[3], "wb");

	if (outz == NULL)
		outz = stdout;


	char buf[1024];
	char buf2[1024];
	int pos = 0;
	int pos2 = 0;

	
	fnames = zvec_mk(NULL, 16);
	collected = zvec_mk(NULL, 16);
	pendingz = zvec_mk(NULL, 16);
	typemap = zvec_mk(NULL, 16);


	mkMapping("int", 0, "Z32",0, 0, 0);
	mkMapping("float", 0, "Real",0, 0, 0);
	mkMapping("zfloat32",0, "Real",0, 0, 0);
	mkMapping("zbool", 0, "Bit", 0, 0, 0);
	mkMapping("zuint16", 0, "Z32", 0, 0, 0);
	mkMapping("zuint32", 0, "Z32", 0, 0, 0);
	mkMapping("zint32", 0, "Z32", 0, 0, 0);
	
	
	mkMapping("void", 1, "Void", 0, 0, 0);


	for(;;){
		int c = fgetc(f);
		if (c < 0) {
			fclose(f);
			f = NULL;
			
			curfile++;
			if (curfile < zvec_count(filenames)) {
				
 				f = fopen(zvec_get_at(filenames, curfile), "rb");
				if (!f)
					printf("Can't open %s\n", zvec_get_at(filenames, curfile));
			}

			if (f)
				continue;
			else
				break;
		}
		
		//skip preprocessor direcives or '//' until the end of the line
		if ((c=='#') ||   ((c=='/')&&(lc=='/'))     ){
			//skip to next line
			if ((lc=='/')&&(pos>0))
				pos--;
			
			pos2 = 0;

			while ((c !='\n')&&(c!='\r')&&(c>0)){

				c = fgetc(f);
				if (pos2 < sizeof(buf2))
					buf2[pos2++] = c;


			}
			buf2[pos2] = 0;
			lc=' ';

			if (!strncmp("define", buf2, 6))
				process_define(buf2);
			

			continue;
		}
		
		//remove /* */ comments
		if ((lc=='/')&&(c=='*')){
			if (pos >0)
				pos--;
			
			c=0;
			lc=0;
			while (  ((lc != '*') || (c!='/')) && (c>=0)){
				lc=c;
				c = fgetc(f);
			}
			continue;

		}
		
		
		if ((c=='\n')||(c=='\r')||(c=='\t')) //convert whitespaces to space
			c=' ';
		
		if ((c == ' ') && (lc==' '))  //skip consecutive spaces
			continue;
		
		
		if (c == '{')
			bracelevel++;
		if (c=='}')
			bracelevel--;
		
		lc=c;
		
		if ((c==';') || (c=='}') ){

			buf[pos] = 0;

			//printf(" LINE:` %d %s`\n" , bracelevel, buf);
			if (bracelevel == 0)
				process_line(buf);
			
			lc=' ';
			pos=0;
			continue;
		}

		if (pos < sizeof(buf))
			buf[pos++]=c;


	}

	fprintf(outc, "#define SET_EXTENSIONS set_handlers\n");
	fprintf(outc, "void set_handlers(){\n");
	int i;
	for (i=0;i<zvec_count(fnames);i++){
		
		char* name = zvec_get_at(fnames, i);
		fprintf(outc, "\tmkSymbol(global, \"C_%s\", tPrimitive, hc_%s);\n", name, name);
	}

	
	for (i = 0; i < zvec_count(collected); i++) {
		fprintf(outc, "%s", zvec_get_at(collected, i));
	}
	
	for (i = 0; i < zvec_count(pendingz); i++) {
		fprintf(outz, "%s", zvec_get_at(pendingz, i));
	}

	fprintf(outc, "}\n");
	
	
	
	if (outc != stdout)
		fclose(outc);
	
	exit(0);
	return 0;

}
