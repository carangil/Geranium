

#include <stdio.h>
#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"
#include "zstring.h"
#include "zarray.h"
#include "zthread.h"
#include "zrand.h"

typedef struct myThreadType_s {
	zthreadT	th;

	//can define whatever
	int input;
	int output;
} myThreadType;

void myThreadTypeFunc(zthreadT* th) {
	myThreadType* mth = (myThreadType*)th;

	printf(" Thread %d presleep\n", mth->input);
	sleep((mth->input) % 4);
	//	printf(" Thread %d postsleep\n", mth->input);


	mth->output = mth->input * mth->input;
}

void thread_test() {

	int i;
	int N = 100;
	int j;

	myThreadType* threads = zarray_alloc(myThreadType, N);
	for (i = 0; i < N; i++) {
		threads[i].input = i;
		zthread_start(&threads[i].th, myThreadTypeFunc);
	}

	for (j = 0; j < 3; j++) {
		sleep(1);

		for (i = 0; i < N; i++) {

			printf("%d", zthread_isfinished(&threads[i].th));
		}
		printf("\n");
	}



	for (i = 0; i < N; i++) {
		zthread_join(&threads[i].th);
		printf(" %d finished with %d\n", threads[i].input, threads[i].output);
	}

}


void main() {

	

	char* x = ram_strdup_cat("test1", "test2");

	printf("test %s\n", x);

	char* s = "abc|def|banana|cat|boo";

	zvecT* vs = zstrsplit(NULL, s, '|');
	zstrsplit(vs, "more;stuff;here", ';');

	ram_free(x);
	
	char* combine1 = zstrbuild(vs, 0);
	char* combine2 = zstrbuild(vs, '/');
	printf(" %s\n%s\n", combine1, combine2);
	ram_free(combine1);
	ram_free(combine2);
	char* j = ram_addref(vs->elements[3]);
	ram_free(vs); //should recursively free everything inside, except one string we addreffed

	printf(" %s %d\n", j, ram_numrefs(j));
	ram_free(j);

	//quick test of different array size

	int* n = zarray_alloc(int, 10);
	zarray_add(n, 60);
	zarray_add(n, 70);

	//thread_test();

	for (int i = 0; i < 100; i++) {

		printf("zrand %d %f\n", zrand(), zrandf(-10, 10));

	}

}

