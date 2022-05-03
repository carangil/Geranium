#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"

void compare(char* s, int a, int b){
	if (a==b) 
		printf("OK\t");
	else 
		printf("ERR\t");

	printf("%s: expected %d, got %d\n",s, a, b);

}

int main(int argc, char** args){
	char* s = ram_strdup("Testing\n");	//1 allocation

	printf(" Copied string: %s %p\n", s, s);

	compare ("xUnfreed objects ", 1, ram_allocs());

	ram_addref(s);
	ram_free(s);
	compare ("yUnfreed objects ", 1, ram_allocs());

	ram_free(s);
	compare ("zUnfreed objects ", 0, ram_allocs());

	zvecT* v = zvec_mk(NULL, 10);	//2 allocation
	char* q;
	zvec_add(v, ram_strdup("Banana"));
	zvec_add(v, q=ram_strdup("Apple"));
	ram_addref(q);  

	ram_free(v);  //frees vector and strings EXCEPT q which was addreffed

	printf(" pre q %d\n", ram_allocs());
	ram_free(q);  //finally free q
	printf(" after q %d\n", ram_allocs());


	s = ram_strdup("Hello");
	char* t = ram_strdup("Bye\n");
	printf("FIRST\n s %p:%s  t %p:%s\n", s, s, t, t);




	char * staticString = "This is a longer string";
	zbool ok = ZFALSE;
	s = ram_resize(s, strlen(staticString)+1, &ok);
	strcpy(s, staticString);

#if 1
	printf("s %p:%s  %d \n", s, s, ok);
	printf("t %p:%s\n", t, t);
	printf(" ASDSD\n");

	ram_free(s);
	ram_free(t);
	ram_allocs();
	
	//now some array crap

	int* x = zarray_alloc(int, 33);

	printf(" size %d count %d\n", zarray_size(x), zarray_count(x));

	ok = ZFALSE;
	x = zarray_resize(x, 1000, &ok);
	printf(" size %d count %d ok:%d\n", zarray_size(x), zarray_count(x), ok);

	x = zarray_resize(x, 10, &ok);
	printf("%p size %d count %d ok:%d\n",x,  zarray_size(x), zarray_count(x), ok);

	x=zarray_expand(x);
	printf(" %p size %d count %d ok:%d\n",x,  zarray_size(x), zarray_count(x), ok);

	int i;
	for (i=0;i<20;i++){
		x = zarray_sizecheck(x,i);

		zarray_add( x, i*3);
		int k = printf(" size %d count %d ok:%d\n", zarray_size(x), zarray_count(x), ok);

	}

	int* sh = zarray_alloc(int, 10);

	x[5]=111;  //also, these are just plain old C arrays

	zarray_copy(sh, 3, x, 4, 8);

	zarray_use(sh, 3);

	x[6]=222;
	

	for (i=0;i<zarray_count(x); i++){
		printf("x %d %d\n", i, x[i]);
	}

	
	
	ram_free(x);
	
	
	for (i=0;i<zarray_size(sh); i++){
		printf("sh %d %d\n", i, sh[i]);
	}
	
	
	int* g   = zarray_alloc(int, 5);
	zarray_add(g, 11);
	zarray_add(g, 22);
	zarray_add(g, 33);
	zarray_add(g, 44);
	
	printf(" append array to sh\n");
	sh = zarray_append(sh, g, ZFALSE, NULL);
	zarray_debug(sh);
	
	sh = zarray_append(sh, g, ZFALSE, NULL);
	zarray_debug(sh);
	sh = zarray_append(sh, g, ZTRUE, NULL);
	zarray_debug(sh);
	sh = zarray_append(sh, g, ZTRUE, NULL);
	zarray_debug(sh);
	
	printf(" appended array  g to sh 4x\n");
	
	for (i=0;i<zarray_size(sh); i++){
		printf("sh %d %d\n", i, sh[i]);
	}
		
	ram_free(sh);

	char *c = zarray_alloc(char, 32);
	zarray_add(c, 'H');
	zarray_add(c, 'a');
	zarray_add(c, '!');

	printf("%p %d %d %s\n", c, zarray_count(c), zarray_size(c), c);

	zarray_copy(c, 3, "abcd", 1,3);
	
	printf("%p %d %d %s\n", c, zarray_count(c), zarray_size(c), c);
	
	ram_free(c);
	ram_free(g);

	printf(" Final allocations %d\n", ram_allocs());
	#endif

}


#if 0
#include "../thread/zthread.h"
#include "../structures/zvector.h"
#include "../structures/zstring.h"


#include "stdio.h"


typedef struct mythreadtype_s{
        zthread_t thread;  //the 'base' class must be first
        char* string;   //data for my thread type
}
mythread_t ;

zlock_t lock;
void testf(void* v) {
	mythread_t* mt = v;
	zlock(&lock);
	sleep(2);
	zunlock(&lock);
	mt->string = "CHANGED";
}

void mythreadfunc(void* thread){
        mythread_t* mt = thread;
        
        int i;
        char* x;
        for(i=0;i<10;i++) {
                
                x = ram_strdup(mt->string);
                printf(" %s %s\n", mt->string,x);
                ram_free(x);
               // sleep(0.1);
        }
}
void mythread_cleanup(mythread_t* mt)
{
        ram_free(mt->string);
        
}

int main(int argc, char** args) {
        mythread_t t;
          
        
        mythread_t* mt;
        int i;
        int j;
        char buffer[100];
		zvec_t v;
		char* s;
		char* a, *b;
		

		
		s = zstrndup("This is a test", 4);
		
		printf("4chars: [%s]\n", s);
		
		ram_free(s);
		
		a = ram_strdup("LEFT");
		b = ram_strdup("rightstring");
		
		
		a = zstrcat(b,a, ZFALSE, ZTRUE);
		
		printf("[%s]\n", a);
		
		ram_free(a);
		ram_free(b);
		
		zsplit(&v, "This;is;a;test;splitting;string;boo", ';');
		
		for (j=0;j<zvec_count(&v);j++) {
			zvec_set_at(&v, j, zstrcat( "LEFT ", (char*) zvec_get_at(&v,j), ZFALSE, ZTRUE));
		}
		
		for (j=0;j<zvec_count(&v);j++) {
			printf("{%s}\n", (char*) zvec_get_at(&v,j));
			
		}
		
		zvec_cleanup(&v);
		
		
		exit (0);
		
		
		


		ram_clear(&t, sizeof(t));
		t.string = "This is a thread";
		
		zlock_init(&lock);
		
		if (zthread_start(&t.thread, testf)) {
			sleep(1);
			while (!ztrylock(&lock)) {
				printf("trylock didn't get lock\n");

			}
			printf("got the lock\n");
			zunlock(&lock);

		}
		zlock_destroy(&lock);


		
		
		
		mt = ram_alloc_array( mythread_t, 10);
        
        for (i=0;i<10;i++){
                snprintf(buffer, sizeof(buffer), "Thread %d", i);
                mt[i].string = ram_strdup(buffer);
              
        }
        for (j=0;j<10;j++) {
                
                for (i=0;i<10;i++){
                zthread_start(&mt[i].thread, mythreadfunc);
                }
                
                for (i=0;i<10;i++){
                        zthread_join(&mt[i].thread);
                        printf("%d joined", i);
                }
        }
           
           
       for (i=0;i<10;i++){
               mythread_cleanup(&mt[i]);
       }
        
       ram_free(mt);
        
       printf("\n%d allocations left\n", ram_allocs());
       	printf("boo %s", 2); 
        
       return 0;
}
#endif
