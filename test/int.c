#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"

/* Program is a linked list of tokens*/

typedef struct tokenS{
    zlistnodeT zlistnode;
    zuint32 tok;
    char* str;
}tokenT;

tokenT* mkToken(zuint32 tok){
    tokenT* t = ram_alloc( sizeof(tokenT) , NULL );
    t->tok = tok;
    return t;
}

char safechar(zuint32 c){
    if ((c >= ' ')&&(c<=0x7f))
	return c;
    return ' ';
}

char* safestr(char* s){
   return s ? s:""; 
}

void printList(zlistT* list){
    zlistnodeT* x;
    tokenT* t; 
    
    for (x=zlist_head(list); x; x=zlist_next(t)){
	t = (tokenT*) x;
	printf("{%c %x '%s'}->", safechar(t->tok),t->tok, safestr(t->str));
    }
    printf("{end}\n\n");
        
}


/* This first part parses an input string and returns a linked list of tokens
 * Very little is verified, things are broadly classified as pairs (such as ->, --, etc),
 * as single character tokens (+,-, etc), names (alpha_numeric_123), numbers (1.0E-4, 0x100, etc)
 * literals  "boo", whitespace, etc.
 * As whether a particiar token such as '-' is really on operator or part of a number it's preceding, that's figured later
 * Things like 0x10.4E-4 is a 'NUMBER', but will later fail as it isn't a valid form
 */

//used to recognize 2-letter combinations like ->, etc
char* findPair(char* patterns, char a, char b){
    for(  ;*patterns;patterns+=2){
	if ( ((*patterns)==a) &&(*(patterns+1)==b))
	    return patterns;
    }
    return NULL;
}

//used to either recognize names or numbers
int acceptPatterns(char* s, char* startChars, char* continuePairs,  char* continueChars){
    int i=0;
    char* st =s;
    char* p=0;
    

    if (strchr(startChars, *(s++))){
	i++;
	
	for(;*s;i++,s++){
	
	    	if (p=findPair(continuePairs,*s,*(s+1))){
		    s++,i++;
		    continue;
		}

		if (strchr(continueChars, (*s))){
		    continue;
		}
	
		break;
	}
			
    }
    // printf(" took %d bytes %.*s  \n",i, i, st);
    return i;
}

//scans through string literals
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

#define PAIR	0x1000
#define NAME	0x2000
#define NUMBER	0x3000
#define LITERAL 0x4000

zlistT* tokenize(zlistT* list, char* in){
    
    int c,next;
    
    int i;
    
    if (!list)
	list = ram_alloc(sizeof(zlistT), NULL);
     	
    while (c = *in){
	next = *(in+1);
	tokenT* t=NULL;
	char*p;
	
	//find twochar patterns like ->,etc. including comment start/end markers
	if (p=findPair("<<>>--++->==||&&+=-=/=*=&=|=^=/**///", c, next)){
	   t = mkToken(PAIR);
	   
	   if (!strncmp(p, "//",2)) { //special handling for // comments
	       while(*in!= '\n')
		   in++;
	   }
	   
	    t->str = zstrndup(p,2);
	   
	   zlist_addtail(list, &t->zlistnode);
	   in+=2;
	   continue;
	} 
		
	//check for string literals
	int lit = acceptLiteral(in, '\'' , '\\');  //single quote
	if (!lit)
	    lit = acceptLiteral(in, '"' , '\\' ); //double quote
	   
	if (lit) {
	     t = mkToken(LITERAL);
	     t->str = zstrndup(in, lit);
	     zlist_addtail(list, &t->zlistnode);
	     in+=lit;
	     continue;
	}
	
	//fold spaces and tabs together
	int space = acceptPatterns(in, " \t\n\r", "", " \t\n\r");
	
	if (space) {
	    t = mkToken(' ');
	    in+= space;
	    t->str=zstrdup(" ");
	    zlist_addtail(list, &t->zlistnode);
	    continue;
	}
	
	int digits  = acceptPatterns(in,
			".0123456789", //start with digit or decimal point
			"e-E-e+E+",  //- and + only accepted after an e or E
			"0123456789abcde.fABCDEFxlLuUfF"); //ontinues with figits, deccimal point, hex letters, type suffix letters
		
	int name = acceptPatterns(in, 
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_",
			"",
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
	
//	printf(" %d accepted as space %.*s\n", space, space, in);
//	printf(" %d accepted as digits %.*s\n", digits, digits, in);
//	printf(" %d accepted as name %.*s\n", name, name, in);
		
	if (digits && name  ){
		printf(" ambiguous name or number %s\n", in);
		exit(1);
	}
	
	if (digits || name){
	    t = mkToken( digits? NUMBER : NAME);
	    t->str = zstrndup(in, digits|name);
	    in += digits|name;
	     zlist_addtail(list, &t->zlistnode);
	     continue;
	}
	
	//just some char
	t = mkToken( *in);
	zlist_addtail(list, &t->zlistnode);
	in++;
		
    }
    return list;
}



int main(int argc, char** args){
    
	//char* t = "int x; void main(int c, char x){printf(\"boo\"";
	  
    if (argc < 2)
	exit(1);
    
	char* x = ram_loadstr(args[1]);
	
	zlistT* tokens = tokenize(NULL, x);
    
	printList(tokens);
    
 return 0;   
}

