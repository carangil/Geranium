#include <stdio.h>
#include <strings.h>

#include <ctype.h>

#include "ztypes.h"
#include "zmem.h"
#include "int.h"
#include "zstring.h"

char * read_to_char(char* in, char stop) {

	while(*in && *in != stop ){
		in++;

	}

	return in;

}

int back_match_parens(char* cap, int pos){
	int paren=0;

	while (pos >=0){
		if (cap[pos] == ')')
			paren++;

		 else if (cap[pos] == '(')
			paren--;

		if (paren == 0)
			break;
		pos --;
	}

	return pos;
}


zstringmapT* ignores = NULL;
zstringmapT* types = NULL;
zstringmapT* unmapped_types = NULL;
zvecT* protos = NULL;
zvecT* defs = NULL;

char* process_arg(char* arg, char** type){

	/*if (strstr(arg, "(")){
		printf("can't deal with function pointers\n");
		return;
	}*/

	if (arg[strlen(arg)-1] == ' ')
		arg[strlen(arg)-1] = 0;

	/*
	if (strlen(arg)==0){
			*type = "notype";
			return "noarg";
	}*/

	zvecT* argparts = zstrsplit(NULL, arg, ' '); //split on space
	//arg name is last parts

	if (zvec_count(argparts)<2){
		*type = arg;
		return "noname";
	}

	char* argname = zvec_remove_last(argparts);

	for (int i=0;i<zvec_count(argparts);i++){
		if (zstringmap_get(ignores, zvec_get_at(argparts, i)))
			zvec_get_x_at(argparts, char*, i)[0] = 0; //truncate it
	}

	char* argtype = zstrbuild(argparts, 0);


	*type = argtype;
	return argname;
}


char* maptype(char* ctype){

	char* mapped = zstringmap_get(types, ctype);

	//printf(" check %s   %s\n", ctype,  mapped?mapped:"nomap");



	if (!mapped && ctype[ strlen(ctype)-1] == '*'){

		ctype[ strlen(ctype)-1 ] = 0;

		mapped = maptype(ctype);
		//printf(" got mapped %s\n", mapped?"mapped":"nomap");
		if (mapped)
			mapped = zstrprintf(NULL, "%s*", mapped);

		//else
		//	mapped = zstrprintf(NULL, "%s*", ctype);
	}

	if (!mapped){
		zstringmap_put(unmapped_types, ctype, "unmapped");
		mapped = zstrprintf(NULL, "%s", ctype);
	}



	return mapped;
}


char* libname = NULL;

char* process(char* cap){

	char* line = NULL;

//	line = zstrprintf(line,  "\n// %s\n", cap);



	if (strstr(cap, "typedef")){
		return line;
	}



	int pos = strlen(cap) -1;
	while (pos >= 0 && cap[pos] != ')')
		pos--;

	//printf("parens : %s\n", cap+pos);

	int startargs = back_match_parens( cap, pos);
	cap[pos] = 0; //kill last paren
	cap[startargs] = 0;
	printf("// %s\n", cap);

	char* ftype = NULL;
	char* name = process_arg(cap , &ftype); //get function name and type
	char* fmapped = maptype(ftype);

	printf("func\t'%s'\t'%s'  %s\n",ftype, name, fmapped);

	zvecT* args = zstrsplit(NULL,cap+startargs+1, ',');//split on comma

	line = zstrprintf(line, "(");
	for (int i=0;i<zvec_count(args);i++){


		char* type = NULL;
		printf(" arg %d'%s' ", i, zvec_get_at(args,i));

		char* argname = process_arg( zvec_get_at(args,i) , &type );
		char* mapped = maptype(type);

		if (!strcmp(mapped, "void"))
			continue;

		printf("arg\ttype'%s'\t'argname%s'\tmapped:%s\n",type, argname, mapped?mapped:"nomapping" );
		line = zstrprintf(line,  "%s:%s ", mapped, argname);
	}

	if (0!=strcmp(fmapped, "void")) {

		line = zstrprintf(line, " -> %s", fmapped);
	}


	line = zstrprintf(line,  ") \"%s\" \"%s\" sys %s\n",  libname, name, name);


	return line;
}

void process_define(char* s){

		if (!strncmp(s, "#define", 7)){
				char* st = zstrtrim(s+8);
				printf("def '%s'\n", st);
				zvecT* parts = zstrsplit(NULL, st, ' ');

				if (zvec_count(parts) >=2){
					char* val = zvec_remove_last(parts);
					char* name = zvec_get_at(parts, 0);

					char* l = strchr(val, 'u');

					if (l)
						*l=0;

					if (zstringmap_get(ignores, name))
						return;
					char* d = zstrprintf(NULL, "%s constant %s\n", val, name );

					zvec_add(defs, d);
				}


		}

}


int scanmain(int argc, char** args){

	char* inname= NULL;
	char* outname=NULL;
	char* typename="typemap.txt";

	char* t = "t";
	ignores = zstringmap_mk(10);
	zstringmap_disown(ignores);
	protos = zvec_mk(NULL, 10);
	defs = zvec_mk(NULL, 10);

	for (int i=1;i<argc;i++){



		if (args[i][0]=='o')
			outname = args[i]+1;

		else if (args[i][0]=='t')
			typename = args[i]+1;

		if (args[i][0]=='L')
			libname = args[i]+1;

		if (args[i][0]=='i')
			inname = args[i]+1;

	}


	if (!inname || !outname || !libname){
		printf("%s iINFILE oOUTFILE Lsofile   with optional tTYPEMAP\n");
		exit(1);
	}

	char* buf = ram_loadstr("typemap.txt");





	unmapped_types = zstringmap_disown(zstringmap_mk(10));

	types = zstringmap_mk(10);


	zvecT* lines = zstrsplit(NULL, buf, '\n');
	ram_free(buf);


	for (int i=0;i<zvec_count(lines);i++){
		char* p = zvec_get_at(lines, i);
		if (strlen(p)==0)
			continue; //skip blank lines
		char* comma = strchr(p, ',');
		if (!comma){
			printf(" expected comma\n");
			exit(1);
		}

		*comma=0;

		char* cname = zstrtrim(p);
		char* zname = zstrtrim(comma+1);

		printf(" '%s' '%s'\n", cname, zname);

		if (!strcmp(zname, "ignore")){
			zstringmap_put(ignores, cname, t);
		} else {
			zstringmap_put(types, cname, zname);
		}

	}


	printf("//%s\n", args[1]);

	char* in = ram_loadstr( inname);
	//out = fopen(args[2], "wb");

	char* capture = zstr_mk(100);

	char* s = NULL;
	int brace = 0;
	int didspace = 0;

	for (;*in;in++){

		//ignore space
		if (isspace (*in)){

			didspace ++;
			*in = ' '; //make sure whitespace is a real space
			if (didspace > 1)
				continue;


		} else {
			didspace =0;
		}

		if (*in == '#'){
			char* x = in;
			in = read_to_char(in, '\n');
			s = zstrndup(x, in-x);
			printf("preproc[%s]\n", s );
		//todo: handle defined
			process_define(s);
			ram_free(s);
			zstr_reset(capture);
			continue;
		}

		//not a space, treat as start of something


		if (!strncmp(in, "//", 2 )){
				in = read_to_char(in,'\n');
				continue;
		}


		if (!strncmp(in, "/*", 2 )){

			in = strstr(in, "*/") +2;
			continue;
		}

		if (*in == '{'){

			if (strstr (capture, "extern") && strstr(capture, "\"C\"")){
					continue;
					zstr_reset(capture);
			}

			brace++;

		}

		if (*in == '}'){
			brace--;
			if (brace < 0)
				brace=0;
			continue;
		}

		if (brace){
			printf("-%c", *in);
			continue;  //skip between braces
		}

		if (*in == ';' || *in == '{'){

			printf("%s\n", capture);

			char* def = process(capture);

			if (def != NULL)
				zvec_add(protos, def);

		//	zvecT* parts = zstrsplit(NULL, capture, ' ');

			//getc(stdin);
			zstr_reset(capture);
			printf("reset %s\n", capture);
			continue;
		}

		//printf("+%c", *in);
		//printf("%s\n", capture);
		capture=zstrcatsub(capture, in, 0, 1);

		if (*in == '*')  //insert space after * so there is always a space before function name in cases like  char*func(
			capture=zstrcatsub(capture, " ", 0, 1);


	}

	;
	void* cursor = NULL;

	char* key= NULL;
	char* v;


	FILE* out = fopen(outname, "wb");

	printf("unmapped types:\n");
	while( zstringmap_nextkey(unmapped_types, &key, &v, &cursor)){
		printf("%s\n", key);
		fprintf(out, "type %s end\n", key);
	}

	for (int i=0;i<zvec_count(defs); i++)
		fprintf(out, "%s\n", zvec_get_at(defs,i));

	for (int i=0;i<zvec_count(protos); i++)
		fprintf(out, "%s\n", zvec_get_at(protos,i));
	fclose(out);

	return 0;
}


int main(int argc, char** args){

	//stringtest();
	if (argc>1 && !strcmp(args[1], "-scan"))
		return scanmain(argc-1, args+1);

	char* src = NULL;
	char* filename = "start.src";

	if (argc > 1){
		filename = args[1];

	}

	src = ram_loadstr(filename);
	if (src){
		int_run_str(src, args[1]);
		ram_free(src);
	} else {
			errorf("Cannot load %s\n", filename);
	}


	ram_allocs();

	return 0;
}
