#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"
#include "zvector.h"

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


char* safestr(char* s){
   return s ? s:""; 
}

void printList(zlistnodeT* x, tokenT* cur, zuint32 stop_tok){
    tokenT* t; 
    char* iscur;
    
    for (;x; x=zlist_next(t)){
	t = (tokenT*) x;
	if (t==cur)
		iscur="CUR";
	else 
		iscur ="";

	if ( (t->tok >20) && (t->tok < 0x7f))

		printf("{%s %c %s}->",iscur, t->tok , safestr(t->str));
	else
		printf("{%s %x %s}->",iscur, t->tok , safestr(t->str));


	if (t->tok == stop_tok)
		break;
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

#define PAIR		0x1000
#define NAME		0x2000
#define NUMBER		0x3000
#define LITERAL 	0x4000
#define ENDFILE		0x5000
#define PASTENDFILE	0x6000
#define STARTFILE	0x7000

zlistT* tokenize(zlistT* list, char* in){
    
    int c,next;
	tokenT* t=NULL;
    
    int i;
    
    if (!list) {
	list = ram_alloc(sizeof(zlistT), NULL);
	t = mkToken(STARTFILE);
	zlist_addhead(list,t);
	t->zlistnode.prev=t;  //'trap' so ->prev->prev is always safe
    }
     	
    while (c = *in){
	next = *(in+1);
	char*p;
	
	//find twochar patterns like ->,etc. including comment start/end markers
	if (p=findPair("<<>>--++->==||&&+=-=/=*=&=|=^=/**///", c, next)){
	  
	   
	   if (!strncmp(p, "//",2)) { //special handling for // comments
	       while(*in!= '\n')
		   in++;
	       continue;
	   }
	   
	    t = mkToken(PAIR);
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
	    in+= space;
	//ignore whitespace
	#if 0
	    t = mkToken(' ');
	    t->str=zstrdup(" ");
	    zlist_addtail(list, &t->zlistnode);
	#endif
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
    //something should stop reading at endfile
    tokenT* end = mkToken(ENDFILE);
    end->str = zstrdup("ENDFILE");
    zlist_addtail(list, &end->zlistnode);

    //if program ever advances reads PASTENDFILE, its an error
    //this is a trap so 'next->next->next' always is safe
    end = mkToken(PASTENDFILE);
    zlist_addtail(list, &end->zlistnode);
    end->str = zstrdup("PASTENDFILE");
    end->zlistnode.next = (zlistnodeT*) end;

    return list;
}


zuint32 strSelect(char** options, char* in){
    zuint32 i;
    for(i=0;options[i];i++)
	if (!strcmp(options[i], in))
	    return i+1;
	
    return 0;
}

#define ERR( ...) { fprintf(stderr,__VA_ARGS__); exit(1);}

/* Parse out stuff */

//#define tNext(TTT)   (((TTT)?(  (tokenT*)( (TTT)->zlistnode.next)):end))


#define tnext(ITEM) ((tokenT*)(ITEM)->zlistnode.next)
#define tprev(ITEM)    ((ITEM)?((tokenT*)(ITEM)->zlistnode.prev):NULL)


zlistnodeT*  parseVar(zlistnodeT* pos, zbool name_required) ;
zlistnodeT*  parseTypeList(zlistnodeT* pos);

zlistnodeT*  parseType(zlistnodeT* pos) {
    
    tokenT* t = (tokenT*) pos; 
	char* count=NULL;

	printf(" Enter parseType\n");

	zbool named=ZFALSE;

    for (t= (tokenT*)pos; t;  t = (tokenT*)( t->zlistnode.next)) {

	printList(tprev(tprev(t)),(void*)t,ENDFILE);


	if (t->tok == '['){ //array type

		if (tnext(t)->tok == NUMBER){
			t=tnext(t);
			count = t->str;
		}
				
		t  = parseType( tnext(t)); //parse the type
		if (t->tok != ']'){
			printf(" missing ]\n");
		}


		t->str = zstrcat( zstrdup("array ("), tprev(t)->str );
		if (count){
			t->str = zstrcat( t->str, " x ");
			t->str = zstrcat( t->str,  count );
		}

		t->str = zstrcat( t->str, ")");

		printf(" t->str is %s\n", t->str);

		
		continue;

	}

	if (!named && t->tok == NAME){ //simple typename, but only 1 per 'type'
		printf(" type %s\n", t->str);
		t->str = zstrcat( t->str, "$type" );
		named=ZTRUE;
		continue;
	}
	if (t->tok == '*'){ //pointer type
		printf(" mk pointer to %s\n", tprev(t)->str);
		t->str = zstrcat( zstrdup("{pointer to("), tprev(t)->str);
		t->str = zstrcat( t->str,")}");
		printf(" made %s\n", t->str );
		continue;
	}
	if (t->tok =='('){ //type list
		printf(" start parseTypeList\n");

		t = parseTypeList(tnext(t));

		
		
		printf(" parseTypeList returned at\n");
		//printList(tprev(tprev(t)), t, ENDFILE);

		if (t->tok !=')'){
			fprintf(stderr, "Expected )\n");
				exit(1);
		}
		t->str = tprev(t)->str;  //typelist is done, so the closing ')' takes all the data
		
		named=ZTRUE;

		continue;
		
	}

	printf(" last was %s\n", tprev(t)->str);
	break;
    }

	return t;
}

zlistnodeT*  parseTypeList(zlistnodeT* pos) {
    		tokenT* t = (tokenT*) pos; 
		zvecT* strs=zvec_mk(NULL,10);
		zvec_disown(strs);//don't free the things we store in here
		for(;t;t=tnext(t)){

		
				
			t = parseVar(t, ZFALSE);
			printf(" got var %s\n", tprev(t)->str);
			zvec_add(strs, tprev(t)->str);

			if (t->tok == ';')
				continue; //list item seperator
			
			if ( (t->tok == PAIR) && !strcmp(t->str, ">>")){
				//is a function
				printf("FUNCTION\n");
				zvec_add(strs, " FUNCTION returning ");
				continue;

			}
			break;

		}

		//reached end of a type list, guess should group them up

		tprev(t)->str = zstrbuild(strs, '+');
		//t->str = zstrbuild(strs, '+'); //CLOSING PAREN OR 
		ram_free(strs);	
		printf(" endtype list at \n");
		printList(tprev(t), t, ENDFILE);

		return t; //closing paren on func parm list OR 'end' in typedef
}

zlistnodeT*  parseVar(zlistnodeT* pos, zbool name_required) {
    char* name=NULL;
    tokenT* t; 
    t= (tokenT*)pos; 

	if (t->tok == NAME && tnext(t)->tok == ':'){
		name = t->str;
		t=tnext(tnext(t));
		printf("Name is %s and type follows\n", name );
	}
	if (name==NULL && name_required){
		fprintf(stderr," var name required\n");
		exit(1);
	}
	t = parseType(t);
	printf("parsetype returned at\n");
	
	printList(tprev(tprev(t)), t, ENDFILE);
	if(name){
		
		tprev(t)->str = zstrcat(tprev(t)->str, " named ");
		tprev(t)->str = zstrcat(tprev(t)->str, name);

	}
	return t;

}

zlistnodeT*  parse(zlistnodeT* pos) {
    
    tokenT* t; 

    
    for (t= (tokenT*)pos; t;  t = (tokenT*)( t->zlistnode.next)) {

	if (t->tok==ENDFILE || t->tok== PASTENDFILE)
	    break;
		
	if (t->tok == NAME){

	    if (!strcmp(t->str, "var")){

	    	t = parseVar( tnext(t), ZTRUE); //parse variable; name is required

		printf(" Type returned was %s\n", tprev(t)->str);
		if (t->tok != ';')
		    printf(" missing ;\n");
		
		continue;
	    }

           if (!strcmp (t->str, "type")){
	   	char* name = NULL;
		if (tnext(t)->tok == NAME) 
			name = tnext(t)->str;
		else {
			fprintf(stderr,"expected type name\n");
			exit(1);
		}
	
		t = parseTypeList(tnext(tnext(t)));
		
		if(t->tok == NAME && !strcmp(t->str, "end")) {
			printf(" Defined type %s as %s\n", name, tprev(t)->str);
			continue;
		}
		fprintf(stderr, " Expected 'end' for type\n");
		exit(1);
	   }

	}
    }

}



 
int func(int a, int b){
    printf(" two %d %d\n", a, b);
    return a+b;
}

int   (*getFunc(int a)) (int a, int b)      {
    printf(" getFunc called %d %d n", a );
    return func;
}

int (*(*getGetFunc(void))(int a))(int a, int b){
	
	return getFunc;
   }



int main(int argc, char** args){
    
	//char* t = "int x; void main(int c, char x){printf(\"boo\"";
	  
 //   int (*f)(int,int) = getGetFunc()(5);
   // f(1,2);
    
    if (argc < 2)
	exit(1);
    
	char* x = ram_loadstr(args[1]);
	
	zlistT* tokens = tokenize(NULL, x);
    
	printList(tokens->head,NULL,ENDFILE);
    
	parse(tokens->head->next);
    
	return 0;   
}

