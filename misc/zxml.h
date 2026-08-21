#include "yxml.h"

//writes the specified datatype to *(userdata+offset)
#define ZXML_WRITE_INT		1
#define ZXML_WRITE_STRING	2
#define ZXML_ADD_VECTOR		3
#define ZXML_WRITE_FLOAT32	4
#define ZXML_ADD_STRING		5

typedef struct xhandlers {
	char* name; //attributes start with '@'.  "." is content
	//int (*handler)(yxml_t* parser, void* rs, int(*reader)(void*), struct xhandlers* subtable, void* userdata);

	
	zuint32 op;
	size_t offset;

	size_t allocate;
	void* destructor;
	struct xhandlers* subhandler;
} zxmlhandlerT;


yxml_t* zxml_mk(size_t stacksizeK);

int zparsexml(yxml_t* parser, void* rs, int (*reader)(void*), zxmlhandlerT* handlers, void* userdata);

zxmlhandlerT* zxml_set_handler(zxmlhandlerT* h, char* name, zuint32 op, zuint32 offset, size_t allocate, zxmlhandlerT* subhandlers, void* destructor);