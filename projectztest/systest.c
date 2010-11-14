#include "..\ztypes.h"
#include "..\memory\ram.h"
#include "..\structures\vector.h"
#include "..\vmath.h"
#include <stdio.h>


typedef struct test_s
{
	char* string;
	struct test_s* next;
} test_t;



void destruct_test(test_t* t)
{
	test_t* p;

	while(t)
	{
		p = t->next;
		ram_free(t->string);
		ram_shallow_free(t);
		t = p;
	}

}

test_t* mk_test(char* z , test_t* next)
{
	test_t* t = ram_alloc( sizeof(test_t), destruct_test);
	t->next = next;
	t->string = z;

	return t;
}

typedef struct inner_s
{
	char* a;
	char* b;
} inner_t;

void inner_free(inner_t* x)
{
	ram_free(x->a);
	ram_free(x->b);

	ram_shallow_free(x);
}

typedef struct outer_s
{
	char* string;
	inner_t* inner;
} outer_t;

void outer_free(outer_t* z)
{
	ram_free(z->inner);
	ram_free(z->string);
	ram_shallow_free(z);
}




void test_mem()
{

test_t* t1=0;

	char* z = NULL;


	t1 = mk_test( ram_strdup("This "), mk_test(ram_strdup("test"), NULL)  );
	

	z  = ram_alloc(100, NULL);

	printf("hello %p %d\n", z, ram_allocs());

	ram_free(z);



	printf("%s %s\n", t1->string, t1->next->string);

	ram_free(t1);

	printf("%d\n", ram_allocs());
	

	{
		outer_t* test = ram_alloc(sizeof(outer_t), outer_free);

		test->inner = ram_alloc(sizeof(inner_t), inner_free);

		test->inner->a = ram_strdup("a string\n");
		test->inner->b = ram_strdup("other string\n");
		test->string = ram_strdup("third string\n");

		printf(" %s %s %s\n", test->inner->a, test->inner->b, test->string);

		ram_free(test);

		printf("%d\n", ram_allocs());

	}


}

void test_vectors()
{
	vec_t  svec;

	vec_t* vec = NULL;
	
	vec = vec_mk(NULL, 2);

	vec_add(vec, ram_strdup("string0\n"));
	vec_add(vec, ram_strdup("string1\n"));
	vec_add(vec, ram_strdup("string2\n"));
	vec_add(vec, ram_strdup("string3\n"));
	vec_add(vec, ram_strdup("string4\n"));

	vec_print(vec);

	//printf(" %s \n", vec_get_at(vec, 7));
	//printf(" %s \n", vec_get_at(vec, 4));
	//printf(" %s \n", vec_get_at(vec, 1));
	//printf(" %s \n", vec_get_at(vec, 0));



	ram_free(vec_remove_ordered(vec, 2));

	vec_print(vec);


	ram_free(vec_get_at(vec, 1)); // free an item in the vector
	//replace item with a new item
	vec_set_at(vec, 1, ram_strdup("THIS IS CRAP"));

	vec_print(vec);

	ram_free(vec);

	printf("allocations left: %d\n", ram_allocs());


	//now try a statically allocated svec structure

	if (!vec_mk(&svec, 2))
	{
		printf("Could not allocate!\n");
	}
	
	vec_add(&svec, ram_strdup("S0"));
	vec_add(&svec, ram_strdup("S1"));
	vec_add(&svec, ram_strdup("S2"));
	vec_add(&svec, ram_strdup("S3"));
	vec_add(&svec, ram_strdup("S4"));

	vec_print(&svec);

	vec_cleanup(&svec);

	printf("allocations left: %d\n", ram_allocs());
}


void test_vec_math()
{
	vec3 a;
	vec3 b;
	
	vec3set(a, 1.1, 2.22, 3.333);
	printf(" %f %f %f \n", a.vec3p[0], a.vec3p[1], a.vec3p[2]);
	vec3mov(b,a);
	printf(" %f %f %f \n", b.vec3p[0], b.vec3p[1], b.vec3p[2]);	
	b.vec3y= 22.222;
	printf(" %f %f %f \n", b.vec3p[0], b.vec3p[1], b.vec3p[2]);	


}


void systest_main()
{


	test_vec_math();

	test_vectors(); //test vector data structure;

}