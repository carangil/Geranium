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
}argT;


#define PROCARG  1
#define PROCNAME 2
#define PROCRET  3

typedef struct mapping {
	char* c;  //name of type or function in c
	//int cstar;
	char* z;  //name of it in z w/all type info
	int proc; //1 if this a proc, 2 is this a return value
	int numargs;
	int noproto; //for functions
} mappingT;
zvecT *typemap;

mappingT* mkMapping(char* c, char* z, int proc, int numargs) {
	
	if (proc == 0) {
		mkMapping(c, z, PROCRET, 0);
		return mkMapping(c, z, PROCARG, 0);
		
	}

	printf(" MAPPING %s->%s   %d\n", c, z, proc);
	
	mappingT* m = ram_alloc(sizeof(mappingT), NULL);
	
	m->c = zstrdup(c);
	m->z = zstrdup(z);
	m->proc = proc;
	m->numargs = numargs;
		
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

mappingT* get_zz_map(char* c, int proc) {
	for (int i = 0; i < zvec_count(typemap); i++) {

		mappingT* m = zvec_get_at(typemap, i);

		if (!strcmp(m->c, c) && (proc == m->proc)) {
			printf(" FOUND %s\n", m->c);
			return m;
		}
	}
	return NULL;
}


char* get_zz(char* c, int proc) {

	if (!c)
		return NULL;

	int stars = 0;
	int percents = 0;
	int i;

	//count stars
	for (i = 0; c[i]; i++)
		if (c[i] == '*')
			stars++;
	int end = i-1 ;//pointing at last char
	
	mappingT* selected = NULL;

	//check for explicit mapping at each level
	do {

		printf("looking for %s stars:%d fproc:%d\n", c, stars, proc);

		selected = get_zz_map(c, proc);

		if (selected)
			break;

		if (stars)
			c[end--] = 0;
		
		if (stars)
			percents++;

	} while (stars--);
	
	char* z = NULL;

	if (selected) {
		printf(" Found %s %d\n", selected->c, percents);
		z= zstrdup(selected->z);
	}
	else {
		printf(" did not find... need to make %s  +%d%%\n", c, percents);
		z = zstrdup(c);
		printf(" made %s\n", z);
	}

	while (z&&percents) {

		if ((proc != PROCRET) && (percents == 1))
			z = zstrcat(z, "&");
		else
			z = zstrcat(z, "%");
		percents--;
	}
		
	return z;
	
}

void create_handler_function(int isunsigned, char* return_type, char* name , zvecT* args){
	
	int return_pointer = 0;

	if (strchr(return_type, '*')) //returning via pointer
		return_pointer = 1;
	
	fprintf(outc, handler_template_start, name);
	int returnsavalue = 0;
	int argcount = zvec_count(args);
	char* as = NULL;
		
	if (!strcmp(return_type, "void"))
		return_type = NULL;
	
	printf(" Handler for name:%s  return_type:%s   pointer:%d\n", name, return_type, return_pointer);
	
	fprintf(outc, "	void* firstArg = ex->stack[ex->sp-%d].as.ptr.block;\n", argcount);

	//set destination
	if (return_pointer){
		fprintf(outc, "	ex->stack[ex->sp-%d].as.ptr.block=(void*) ", argcount);
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
		
		if (strchr(arg->ctype, '*')){
			fprintf(outc, "(void*)((ex->stack[ex->sp-%d].as.ptr.block)+(ex->stack[ex->sp-%d].as.ptr.offset))" , j+1, j+1 );
		}
		else{
			as = get_as(arg->ctype, 0);
			fprintf(outc, "ex->stack[ex->sp-%d].as.%s", j + 1, as);
		}
		
		if (j!=0)
			fprintf(outc, ",\n			");		
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

void create_primitive(int isunsigned, char* return_type, char* name, zvecT* args) {
	printf(" Create primitive for: _%s_%s_\n", return_type, name);

	char* z = get_zz(name, PROCNAME);
	char* zr = get_zz(return_type,PROCRET);
	
	if (strchr(z, ':')) {  //pass thru parameter list if header file specified one
		fprintf(outz, "primitive C_%s %s\n", name, z);
		return;
	}

	fprintf(outz, "primitive C_%s %s:(", name, z);

	int i;
	for (i = zvec_count(args) ; i-- > 0; ) {
		argT* arg = zvec_get_at(args, i);

		char* zt = get_zz(arg->ctype, PROCARG);
		fprintf(outz, "c_%s:%s", arg->name, zt);
		if (i != 0)
			fprintf(outz, ";");
	}
	
	fprintf(outz, "->%s);\n", zr);

}
char* to_chars(char* s, char* to);

void process_line(char* buf){
	int i;
	
	char b[128];
			
	if (!strchr(buf, '(')){
		fprintf(stderr, " skip non-prototype .. ( is required to define a function\n");
		return;
	}
	fprintf(stderr, "----------------------------------");
	fprintf(stderr," CANDIDATE PROTOTYPE: %s\n", buf);
	
	char* proto = zstrdup(buf);
	
	zvecT* args = zvec_mk(NULL, 10);
		
	int pos=0;
	int pstart=0;
	int ns=0;
	for (i=strlen(buf)-1 ;i>=0;i--){ //backwards from end
	
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

			argT* arg = ram_alloc(sizeof(argT), argT_cleanup);
			
			zvec_add(args, arg);
			
			while(buf[i]=='*' || buf[i]==' '){
				if (buf[i]=='*'){
					typename = zstrcat(typename, "*");
				}
				i++;
			}
			arg->ctype = typename;
			
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
				if ((buf[i]==' ') || (buf[i]=='*'))
					break;
				i--;
			}
			
			fprintf(stderr," Function name <%s>\n", buf+i+1); 
			char* fname = buf+i+1;

			int noproto = 0;
			
			mappingT* fm = get_zz_map(fname, PROCNAME);

			if (!fm || !fm->noproto)
				fprintf(outc, "%s;\n", proto);

			//skip any spaces and stars
			int pointer=0;
			while( (i>=0) &&( (buf[i]=='*') || (buf[i]==' '))){
				
				if (buf[i]=='*') {
					pointer++;
				}
				
				buf[i] = 0;
				
				i--;
				
			}
			pos =i;
			
			while((i>=0) && buf[i]!=' ')
				i--;
			
			char* rtype = zstrdup(buf + i + 1);
			while (pointer--)
				rtype = zstrcat(rtype, "*");
			
			
			int isunsigned=0;
			
			if ((  buf+i+1-9 >=0) &&(!strncmp(buf+i+1-9, "unsigned", 8)))
				isunsigned=1;
							
			
			create_handler_function(isunsigned, rtype, fname, args);
			int j;
			for (j=zvec_count(args)-1;j>=0; j--){
				
				argT* arg= zvec_get_at(args, j);
				
				fprintf(stderr, "  %s  %s  \n", arg->name, arg->ctype);
				
			}
			
			create_primitive(isunsigned,rtype, fname, args);
			
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

int process_define(char* buf) {

	//printf(" DEF0 '%s'  %d %d\n", buf, buf[strlen(buf) - 2], buf[strlen(buf) - 1]);
	if (strchr(buf, '(')) {
		//skip function-like macros
		return;
	}
	//printf(" DEF1 '%s'\n", buf);

	char* word = buf;
	word = eat_chars(word, " \t\n\r");

	//printf("DEF1.5: word:'%s'\n", word);

	char* definition = to_chars(word, " \t\n\r");

	if ((*definition == '\n') || (*definition == '\r')) //end of line... end the string
		*definition = 0;

	if (*definition) {

		*definition = 0;
		definition++;
		definition = eat_chars(definition, " \t");
		char* defend = to_chars(definition, "\n\r");
		//char* defend = to_chars(definition, "\n\r \t");
		*defend = 0;
	}

	

	if (strchr(word, '('))
		return;  //skip function-like macros

	if (strlen(definition) == 0) {
		//skip empty definitions
		return;
	}
	
	printf(" DEF2 '%s' -> '%s'\n", word, definition);

	int num = atoi(definition);

	if (num == 0) {
		//try hex
		num = strtol(definition, NULL, 16);
	}
	
	//instead of making a constKant, just make it variable for now
	//		fprintf(outz, "%d #%s //constant\n", num, word);
	fprintf(outz, "%d constant %s\n", num, word);
	
}

void process_zdef(char* zdef) {

	printf(" start at %s\n", zdef);

	zdef = eat_chars(zdef, " \t"); //get rid of space
	

	char* s = to_chars(zdef, " \t\r\n/");

	*s = 0;

	printf(" zdef {%s} %s\n", zdef, s+1);

	char* cname = eat_chars(s +1, " \t"); //skip over space
	s = to_chars(cname, " \t\r\n/");
	*s = 0;
	printf(" cname {%s}\n", cname);

	char* definition = eat_chars(s+1, " \t"); //skip over space

	char* defend = to_chars(definition, " \t\r\n/");

	char* defword = zstrndup(definition, defend - definition);

	fprintf(stderr, "-ZZZ-- {%s} {%s} {%s} //{%s}\\\\ \n", zdef, cname, defword, definition );  
	
	int noproto;
	mappingT* m = NULL;

	if ((noproto = !strcmp(zdef, "noproto")) || !strcmp(zdef, "proc")) {
		m = mkMapping(cname, definition, PROCNAME, 0);

		m->noproto = noproto;
	}

	if (!strcmp(zdef, "opaque")) {
		fprintf(outz, "opaque @%s %s;\n", cname, definition);
		char* tmp = zstrprintf(NULL, "\taddCSize(\"%s\", sizeof(%s));\n", cname, cname);
		zvec_add(collected, tmp);
		m = mkMapping(cname, definition,0,0);
	}

	if (!strcmp(zdef, "type"))	//mapping a type used for either args or returns
		m = mkMapping(cname, defword, 0, 0);
		

	if (!strcmp(zdef, "arg"))	//mapping a type when used as an arg
		m=mkMapping(cname, defword, PROCARG, 0);

	if (!strcmp(zdef, "return")) //mapping a type when used as a return value
		m=mkMapping(cname, defword, PROCRET, 0);

	if (!strcmp(zdef, "stacked")) {
		fprintf(outz, "stacked %s;\n", cname);
	}

	if (!strcmp(zdef, "handler")) {
		fprintf(outz, "primitive C_%s %s\n", cname, defword);

		char* tmp = zstrprintf (NULL, "\tmkSymbol(global, \"C_%s\", tPrimitive, h_%s);\n", cname, cname);


		zvec_add(collected, tmp);

	}

	if (!strcmp(zdef, "struct")) {

		char* zname = definition;
		definition = strchr(definition, ':');

		if (definition) {
			*definition = 0;
			definition++;

			//printf("\t ty = mkType(PENDING, NULL, \"%s\", 0);\n", zname);

			fprintf(outz, "type @%s %s\n", cname, zname);


			//Add the mapping from C pointer  cname*  to a user pointer  zname&

			mkMapping(cname, zname, 0, 0);

			char* tmp = zstrprintf(NULL, "\taddCSize(\"%s\", sizeof(%s));\n", cname, cname);
			zvec_add(collected, tmp);

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
							fprintf(outz, "\t@%s_%s\t%s:%s;\n", cname, oldname, newname, type);

							tmp = zstrprintf(NULL, "\taddCSize(\"%s_%s\", offsetof(%s,%s));\n", cname, oldname, cname, oldname);
							zvec_add(collected, zstrdup(tmp));

						}
					}
				}
			}

			fprintf(outz, "end\n");
		}

	}
	
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

	mkMapping("zbool", "Bit", 0, 0);
	mkMapping("zuint32", "N32", 0,0);
	mkMapping("zint32", "Z32", 0, 0);
	mkMapping("int", "Z32", 0, 0);
	mkMapping("float", "Real", 0, 0);
	mkMapping("zfloat32", "Real", 0, 0);
	mkMapping("void*", "Void&", 0, 0);
	mkMapping("void", "", 0, 0);
	mkMapping("char", "N8", 0, 0);
	mkMapping("zchar", "N8", 0, 0);
	mkMapping("char*", "String&", PROCARG, 0);
	mkMapping("char*", "String%", PROCRET, 0);
	mkMapping("zchar*", "String&", PROCARG, 0);
	mkMapping("zchar*", "String%", PROCRET, 0);



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
		
		//skip preprocessor directives or '//' until the end of the line
		if ( (c=='#') || ((c=='/')&&(lc=='/'))     ){
			//skip to next line
			
			if ((lc=='/')&&(pos>0))  // [why did I put this here?
				pos--;				 //          
									 // ]
			
			pos2 = 0;

			while (1){

				c = fgetc(f);
				if ( (c == '\n') || (c == '\r') || (c <= 0))
					break;

				if (pos2 < sizeof(buf2)-1)
					buf2[pos2++] = c;

			}
			buf2[pos2] = 0;
			lc=' ';
			printf(" CHECK %s\n", buf2);
			if (!strncmp("define", buf2, 6))
				process_define(buf2+6);
			
			if (!strncmp("Zstop", buf2, 5))
				exit(1);

			if (!strncmp("Zdef", buf2, 4)) {
				process_zdef(buf2 + 4);
				
			}
			pos = 0;
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
