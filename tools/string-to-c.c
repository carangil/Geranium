#include <stdio.h>



int main(int argc, char** args){
	
	FILE* in;
	FILE* out;
	char* varname;
	int c;
	int cnt;
	
	if (argc < 4)
		return 3;
	
	in = fopen(args[1], "rb");
	out = fopen(args[2], "wb");
	varname = args[3];
	
	if (in && out ) {
		
		fprintf(out, "char * %s=\n\t\"", varname);
		
		while(1) {
			c = fgetc(in);
			if (feof(in))
				break;
			
			if (c=='\n')
				fprintf(out, "\\n");
		
			else if (c=='\r')
				fprintf(out, "\\r");
			
			else if (c=='\t')
				fprintf(out,"\\t");
			
			else if (c=='"')
				fprintf(out, "\\\"");
			
			else if (c>=' ') 
				fprintf(out, "%c",c);
			else
				printf(" Ignoring character $%x\n", c);
			
	
		}
		
		fprintf(out,"\";\n");
		return 0;
		
	}
	
	return 1;
}
