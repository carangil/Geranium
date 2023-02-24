#include "zmem.h"
#include "zarray.h"
#include "zvector.h"
#include "zstring.h"
#include "ztime.h"
#include "zrand.h"
#include "zlist.h"
#include "blank.h"


void compare(char* s, int a, int b){
	if (a==b) 
		printf("OK\t");
	else 
		printf("ERR\t");

	printf("%s: expected %d, got %d\n",s, a, b);

}

void mem_array_string_test(){

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



	zvecT* ss = zstrsplit(NULL, "This|is|a|test", '|');
	zstrsplit(ss, "More:strings", ':');

	for (i=0;i< zvec_count(ss); i++){
		printf("%d:%s\n", i, zvec_elements_as(char*,ss)[i]);
	}

	char* together = zstrbuild(ss, '-');

	char* str = ram_addref(zvec_elements(ss)[3]);
	ram_free(ss);

	char* copy = zstrdup(together);
	copy = zstrcat(copy, "AAAAA");

	printf(" final string: %s\n", together);
	printf(" copy string: %s\n", copy);


	ram_free(together);
	ram_free(copy);
	ram_free(str);

	printf(" Final allocations %d\n", ram_allocs());
	#endif


}



#include "zthread.h"
typedef struct myThreadType_s{
	zthreadT	th;

	//can define whatever
	int input;
	int output;
} myThreadType;

void myThreadTypeFunc(zthreadT* th){
	myThreadType* mth = (myThreadType*)th;

	printf(" Thread %d presleep\n", mth->input);
	sleep( (mth->input) % 4 );
//	printf(" Thread %d postsleep\n", mth->input);


	mth->output = mth->input * mth->input;
}

void thread_test(){

	int i;
	int N = 100;
	int j;

	myThreadType* threads = zarray_alloc(myThreadType, N);
	for (i=0;i<N;i++){
		threads[i].input = i;
		zthread_start(&threads[i].th, myThreadTypeFunc);
	}

	for (j=0;j<3;j++){
		sleep(1);

		for (i=0;i<N;i++){
		
			printf("%d", zthread_isfinished(&threads[i].th));
		}
		printf("\n");
	}



	for (i=0;i<N;i++){
		zthread_join(&threads[i].th);
		printf(" %d finished with %d\n", threads[i].input, threads[i].output);
	}	

}

#define BS 4096
char buffer[BS];

void outbuffer(){
	fprintf( stderr, " BUFFER HAS {%s}\n", buffer);
	fflush(stdout);
	memset(buffer, 0, sizeof(buffer));
	

}




typedef struct intList{
	zlistnodeT zlistnode;
	int v;
}intListT;

zbool intListCleanup(void* v){
	printf(" destructor for listnode %p\n", v);
	return ZTRUE;
}

intListT* mkIntNode(int a){
	intListT* ln = ram_alloc(sizeof(intListT), intListCleanup);
	ln->v=a;
	return ln;
}

#define zclear(BOO) memset(&(BOO), 0, sizeof(BOO))



int vp(void* v){
	return 0xFFFFFF & ((int)(v));
}

void printList(zlistT* list){

	
	intListT* ln = zlist_head(list);
	intListT* lno =&list->sentinal_head;
	
	printf("[HEAD]");
	//printf("[(%x<%x>%x)-", vp(list->sentinal_head.prev), vp(&list->sentinal_head),  vp(list->sentinal_head.next) );
	while(ln){
		printf(" %d ", ln->v);
		//printf("%d (%p<%p>%p)-", (int)ln->v, vp(ln->zlistnode.prev), vp(ln), vp(ln->zlistnode.next) );
		lno=ln;
		ln = zlist_next(ln);
		
	}
	
	if (lno->zlistnode.next && !lno->zlistnode.next->next){
		printf(" [TAIL]");
		//printf("(%x<%x>%x)", vp(list->sentinal_tail.prev), vp(&list->sentinal_tail),  vp(list->sentinal_tail.next) );
		
	}
	printf("\n ");
	
	
	
	lno =&list->sentinal_tail;
	ln = zlist_tail(list);
	
	
	//printf("(%x<%x>%x) ", vp(list->sentinal_tail.prev), vp(&list->sentinal_tail),  vp(list->sentinal_tail.next) );
	printf("[TAIL]");
	
	while(ln){
		
		printf(" %d ", ln->v);
		//printf("%d (%p<%p>%p)-", (int)ln->v, vp(ln->zlistnode.prev), vp(ln), vp(ln->zlistnode.next) );
		lno=ln;
		ln = zlist_prev(ln);
		
	}
	
	if (lno->zlistnode.prev && !lno->zlistnode.prev->prev){
		printf("[HEAD]");
		//printf("[(%x<%x>%x)", vp(list->sentinal_head.prev), vp(&list->sentinal_head),  vp(list->sentinal_head.next) );		
		
		
	}
	printf("\n ");
	
}

void list_test(){
	
	zlistT list ={0};
	
	
	printList(&list);
		
	zlist_addhead(&list, mkIntNode(3) );
	zlist_addhead(&list, mkIntNode(2) );
	zlist_addhead(&list, mkIntNode(1) );
	zlist_addtail(&list, mkIntNode(4) );
	zlist_addtail(&list, mkIntNode(5) );
	
	printList(&list);
	
	//remove the head node
	ram_free(zlist_remove_mid( zlist_head(&list)));
	ram_free(zlist_remove_mid( zlist_head(&list)));
	printList(&list);
	
	
	//remove the tail node
	ram_free(zlist_remove_mid( zlist_tail(&list)));
	ram_free(zlist_remove_mid( zlist_tail(&list)));
	printList(&list);
	
	printf(" head %p    tail %p\n", zlist_head(&list), zlist_tail(&list));
	
	//remove the last node
	
	ram_free(zlist_remove_mid( zlist_tail(&list)));
	printList(&list);
	printf(" head %p    tail %p\n", zlist_head(&list), zlist_tail(&list));
	
	zlist_addtail(&list, mkIntNode(100) );
	zlist_addhead(&list, mkIntNode(10) );
	zlist_addhead(&list, mkIntNode(1) );
	zlist_addtail(&list, mkIntNode(200) );
	printList(&list);

	zlist_cleanup(&list);
	ram_allocs();
}


	
	

int main(int argc, char** args){

	int i;

	blank_example("banana");
/*
	memset(buffer, '$', 200);
	FILE* f = freopen("out", "w", stdout);

	//setvbuf(f, buffer, _IOLBF,BS); //line buffer
	setvbuf(stdout, buffer, _IOFBF,BS); //full buffer

	

	printf("BOO\n");
	for (i=0;i<10;i++){
		printf("T %d ", i);
	}
		outbuffer();


	exit(0);	
*/
	mem_array_string_test();

//	thread_test();
/*
	for (i=0;i<10;i++){
		printf("unix time: %d (then msleep 250...)\n", tm_epoch());
		tm_msleep(250);
	}
*/
	//random numbers:
	for (i=0;i<10;i++)
		printf(" %x  %f\n", zrand(), zrandf(-2,5));

	list_test();
	
}



