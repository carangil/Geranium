#include <stdio.h>

#include "ztypes.h"
#include "zvector.h"
#include "zstring.h"
#include "zmem.h"


FILE* outc = NULL;

typedef struct argS{
	char* ctype;
	char* name;
	int isunsigned;
	int pointer;
}argT;

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
	
	
	fprintf(stderr, " Unknown type, assume z32: %s \n", type);
	
	
	return "z32";
	
}

zvecT* fnames = NULL;

void create_handler_function(int isunsigned, char* return_type, int return_pointer, char* name , zvecT* args){
	
	fprintf(outc, handler_template_start, name);
	int returnsavalue = 0;
	int argcount = zvec_count(args);
	char* as = NULL;
		
	if ( (!strcmp(return_type, "void"))&&(!return_pointer))
		return_type = NULL;
	
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
	fprintf(outc, ");");
	
	
	if (return_pointer){
		fprintf(outc, "	ex->stack[ex->sp-%d].as.ptr.level=0; \n", argcount);
		fprintf(outc, "	ex->stack[ex->sp-%d].as.ptr.offset=0; \n", argcount);
	}
	
	
	//adjust stack pointer
	fprintf(outc, "\n	ex->sp+= (-%d+%d);\n", zvec_count(args), return_type? 1:0 );
	
	fprintf(outc, "\n%s", handler_template_end);
		
}

void process_line(char* buf){
	

	int i;
	
	char b[128];
			
	
	if (!strchr(buf, '(')){
		fprintf(stderr, " skip non-prototype .. ( is required to defind a function\n");
		return;
	}
	
	
	fprintf(stderr," CANDIDATE PROTOTYPE: %s\n", buf);
	
	//output prototype to gen header
	fprintf(outc,  "%s;\n",  buf);
	
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
					
				
				if(!strncmp("unsigned", buf+pos, i-pos)){
					fprintf(stderr, " skip unsigned\n");
					isunsigned=1;
					i++;
					continue;
				}
				
				break;
			}
			
			char* typename = strndup(buf+pos, i-pos);
			
			fprintf(stderr, " NAME IS <%s>  unsigned:%d\n", typename, isunsigned);
			
			
			argT* arg = ram_alloc(sizeof(argT), argT_cleanup);
			arg->ctype = zstrdup(typename);
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
			
			
			break;
			
		}
		
		
	}

	
	ram_free(args);
	
}


int main (int argc, char** args){
	int bracelevel=0;
	char lc=' ';
	
	if(argc<2)
		return 1;

	FILE* f = fopen (args[1], "rb");

	if (!f)
		return 2;

	
	if (argc > 2)
		outc = fopen(args[2], "wb");
	
	if (outc == NULL)
		outc = stdout;
	
	char buf[1024];
	int pos=0;

	
	fnames = zvec_mk(NULL, 16);
	
	for(;;){
		int c = fgetc(f);
		if (c<0)
			break;
		
		//skip preprocessor direcives or '//' until the end of the line
		if ((c=='#') ||   ((c=='/')&&(lc=='/'))     ){
			//skip to next line
			if ((lc=='/')&&(pos>0))
				pos--;
			
			while ((c !='\n')&&(c!='\r')&&(c>0)){

				c = fgetc(f);

			}
			lc=' ';
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
		fprintf(outc, "	mkSymbol(global, \"C_%s\", tPrimitive, hc_%s);\n", name, name);
	}
	fprintf(outc, "}\n");
	
	ram_allocs();
		
	
	if (outc != stdout)
		fclose(outc);
}
