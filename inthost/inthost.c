#include <stdio.h>
#include <strings.h>

#include <ctype.h>
#include "int.h"


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


//zstringmapT* ignores = NULL;
zstringmapT* types = NULL;
zstringmapT* unmapped_types = NULL;
zvecT* protos = NULL;
zvecT* defs = NULL;



char* maptype(char* ctype_in){


	zvecT* argparts = zstrsplit(NULL, ctype_in, ' ');


	for (int i=0;i<zvec_count(argparts);i++){
		char* part = zvec_get_at(argparts, i);
		char* replace = zstringmap_get(types, part);
		if (replace && !strcmp(replace, "")){
			ram_free(part);
			zvec_set_at(argparts, i, zstrdup(replace));

		}

	}

	char* ctype = zstrbuild(argparts,0);


	if  ( !strcmp(ctype, "void"))
		return NULL;

	char* mapped = zstringmap_get(types, ctype);

	printf(" replace '%s'->'%s'->'%s'", ctype_in, ctype, mapped?mapped:"none");

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

				//	//if (zstringmap_get(ignores, name))
				//		return;
					char* d = zstrprintf(NULL, "%s constant %s\n", val, name );

					zvec_add(defs, d);
				}


		}

}

typedef struct ctypedT{
	char* type;
	char* name;
	char* comment;
	zvecT* subs;
}ctypedT;

zbool freectyped(void* v){
	ctypedT* c = v;
	ram_free(c->type);
	ram_free(c->name);
	ram_free(c->subs);
	ram_free(c->comment);
}
//	zstringmap_disown(ignores);


int strcommon(char* s, char* t){
	int common = 0;

	printf(" %s vs %s \n", s, t);
	while (*s && *t){

		if (*s == *t)
			common++;
		else
			break;

		s++;
		t++;
	}

	return common;
}

int scanmain(int argc, char** args){

	char* inname= NULL;
	char* outname=NULL;
	char* incname= NULL;
	char* typename="typemap.txt";

	char* section = NULL;
	char* prefix = NULL;
	char* t = "t";


//	protos = zvec_mk(NULL, 10);
	//defs = zvec_mk(NULL, 10);

	for (int i=1;i<argc;i++){



		if (args[i][0]=='o')
			outname = args[i]+1;

		else if (args[i][0]=='t')
			typename = args[i]+1;

		if (args[i][0]=='L')
			libname = args[i]+1;

		if (args[i][0]=='i')
			inname = args[i]+1;

		if (args[i][0]=='p')
			prefix = args[i]+1;

		if (args[i][0]=='s')
			section = args[i]+1;

		if (args[i][0]=='+')
			incname = args[i]+1;


	}


	if (!inname || !outname || !libname){
		printf(" iINFILE oOUTFILE Lsofile   with optional tTYPEMAP\n");
		exit(1);
	}

	char* buf = ram_loadstr(typename);




	unmapped_types = zstringmap_disown(zstringmap_mk(10));

	types = zstringmap_mk(10);


	zvecT* lines = zstrsplit(NULL, buf, '\n');
	ram_free(buf);


	for (int i=0;i<zvec_count(lines);i++){
		char* p = zvec_get_at(lines, i);
		if (strlen(p)==0)
			continue; //skip blank lines
		char* semi = strchr(p, ';');
		if (!semi){
			printf(" expected semicolon\n");
			exit(1);
		}

		*semi=0;

		char* cname = zstrtrim(p);
		char* zname = zstrtrim(semi+1);

		printf(" '%s' '%s'\n", cname, zname);

		//if (!strcmp(zname, "ignore")){
			//zstringmap_put(ignores, cname, t);
		//} else {
			zstringmap_put(types, cname, zname);
		//}

	}


	printf("//%s\n", args[1]);

	zvecT* functions = zvec_mk(NULL, 8);

	char* in = ram_loadstr( inname);

	char* capture = zstr_mk(1000);

	zvecT* cols = zvec_mk(NULL, 8);

	if (!cols)
		abort();

	ctypedT* func = NULL;

	for (;*in;in++){

		if (*in == '\t' || *in =='\n'){

			//printf("Column with %s\n", capture);
			zvec_add(cols, zstrdup(capture));
			zstr_reset(capture);


			if (*in == '\n'){

				//printf("LINE with %s\n",  zvec_get_at(cols, 0) );

				if (zvec_count(cols) >3){
					if (!strcmp(zvec_get_at(cols, 3), "p")){

						char* rtype = NULL;
						char* sig = NULL;
						func = ram_alloc(sizeof(ctypedT), freectyped);

						printf(" function '%s' ", (char*)zvec_get_at(cols,0));

						zvec_add( functions, func);

						int common=0;

						if (prefix){
							common = strcommon(zvec_get_at(cols,0), prefix);
						}

						func->name = zstrdup(common+zvec_get_at(cols,0));

						for (int i=4; i<zvec_count(cols);i++){
							char*s = zvec_get_at(cols,i);


							if (strstr(s, "typeref:typename:") == s){
								rtype = s+ 17;
								printf("returns '%s'\n", rtype);

								func->type = maptype( rtype);


							}

							if (strstr(s, "signature:") == s)
								sig = s+ 10;;

							printf("\n");
						}

						func->subs = zvec_mk(NULL,8);
						func->comment = zstrprintf(NULL, "%s %s %s\n", rtype?rtype:"none" , func->name, sig?sig:"(?)");
					}


					if (!strcmp(zvec_get_at(cols, 3), "z")){

						ctypedT* arg = ram_alloc(sizeof(ctypedT), freectyped);

						arg->name = zstrdup(zvec_get_at(cols,0));

						printf(" arg:'%s' ", zvec_get_x_at(cols,char*,0));

						for (int i=4; i<zvec_count(cols);i++){
							char*s = zvec_get_at(cols,i);
							char* atype = NULL;
							char* funcname = NULL;
							if (strstr(s, "typeref:typename:") == s){
								atype = s+ 17;

								arg->type = maptype(atype);
								printf("type '%s'->'%s'\n", atype, arg->type);
							}

							if (strstr(s, "prototype:") == s){
								funcname = s+ 10;
								printf("for func '%s'\n", funcname);
							}
						}

						//attacch to last function
						zvec_add(func->subs, arg);
						printf("\n");
					}



				}


				while(zvec_count(cols))
					ram_free(zvec_remove_last(cols));


			}
			continue;
		}


		capture = zstrcatsub( capture, in, 0, 1);



	}

	FILE* out = fopen(outname, "wb");

	if (section)
		fprintf(out, "section %s\n\n", section);


	if (incname){
		char* i = ram_loadstr(incname);
		fprintf(out, "%s\n",  i);
		ram_free(i);

	}


	printf("unmapped types:\n");

	char* val = NULL;
	char*	name=NULL;
	zstringmap_cursorT	cursor = 0;


	while (zstringmap_nextkey(unmapped_types, &name, &val, &cursor)) {
		printf("%s\n", name);
		fprintf (out, "\ttype %s end\n", name);

	}




	for (int i=0;i < zvec_count(functions); i++){
		ctypedT* function = zvec_get_at(functions, i);

		printf(" func %s\n", function->name);

		char* override = zstringmap_get(types, function->name);



		fprintf(out, "\t// %s\n", function-> comment);

		if (override){
			fprintf(out, "\t%s\n\n", override);
			continue;
		}

		fprintf(out, "\t ( ");

		for (int j=0;j<zvec_count(function->subs);j++){
			ctypedT* arg = zvec_get_at(function->subs, j);

			fprintf(out, "%s:%s ",arg->type, arg->name );

		}

		if (function->type && strlen(function->type)){
			fprintf(out, " -> %s ", function->type );  //return type
		}
		fprintf(out, ") \"%s\" \"%s\" sys %s \n\n",  libname, function->name, function->name);

	}

	if (section)
		fprintf(out, "end\n\n");



	return 0;
}


int main(int argc, char** args){

	//stringtest();const,
	if (argc>1 && !strcmp(args[1], "-scan"))
		return scanmain(argc-1, args+1);

	char* src = NULL;
	char* filename = "start.src";

	if (argc > 1){
		filename = args[1];

	}

	src = ram_loadstr(filename);
	if (src){
		int_run_str(src, filename);
		ram_free(src);
	} else {
			errorf("Cannot load %s\n", filename);
	}

	ram_allocs();

	return 0;
}
