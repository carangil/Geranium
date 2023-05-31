#include "ztypes.h"
#include "zmem.h"
#include "zvector.h"
#include "zarray.h"
#include "zstring.h"

#include "zxml.h"

/* Very basic minimal wrapper around YXML 

	YXML is from here: https://dev.yorhel.nl/yxml  Copyright (c) 2013-2014 Yoran Heling

*/

/*
* 
* Can parse subelements into structs that are added to zvectors.  Structs are allocated dynamically, and a destructor can be specified.
* Can parse attributes into string, int, or float elements of structs
* Can parse tag content into a string.  
* 
* Mixed content where subtags and content strings are both present is not well supported (or maybe not at all)
* 
* Order of the same type of tag is preserved in the vectors, but order of different tag types is not captured into the structure, since each type is its own struct
* 
* TODO:  mixed content (if I ever need it)
* add a '#' property to tags so order can be recovered (if I ever need it)
* 
*/

/* Reader is a function that returns 1 byte at a time from void* 'rs' , as an int.  Uses -1 to handle end of data
* Note, intentionally, reader can be fgetc, and rs can be a FILE*.  But they could be anything.
*/

int zparsexml(yxml_t* parser, void* rs, int (*reader)(void*), zxmlhandlerT* handlers, void* userdata) {

#define XNEXT c = reader(rs) ; if (c != -1) { cs[0]= c&0xFF; ret = yxml_parse(parser, cs[0]); }
	int i;
	char* buf = zstr_mk(12);
	char cs[2] = { 0,0 };
	int c = 0;
	int fhandler;
	yxml_ret_t ret = 0;
	int ignored = 0;
	char* content=NULL;

	while (1) {


		fhandler = -1;

		XNEXT;

		if (c < 0)
			break;

		if (ret < 0)
			return ret;

		switch (ret) {


		case YXML_ATTRSTART:

			for (i = 0; handlers[i].name; i++) {
				if (!handlers->name)
					break;

				if (handlers[i].name[0] != '@')
					continue;
				//only looking for attributes

				if (!strcmp(handlers[i].name + 1, parser->attr)) {
					printf(" found %s\n", handlers[i].name);
					zstr_reset(buf);
					while (1) {
						ret = XNEXT;
						if (ret == YXML_ATTRVAL)
							buf = zstrcat(buf, cs);
						else if (ret == YXML_ATTREND)
							break;
						else if (ret) {

							printf(" ERROR %d\n", ret);
							exit(2);
						}

					}//end while
					//x has the value
					printf(" - '%s'\n", buf);
					fhandler = i;

					break;
				}//end if
				//look for values		

			}//end i
			break;


		case YXML_ELEMEND:

			if (ignored == 0) {
				for (i = 0; handlers[i].name; i++) {
					if (!strcmp(handlers[i].name, ".")) {
						//store content
						void* v = (handlers[i].offset) + (char*)(userdata);
						if (handlers[i].op == ZXML_ADD_STRING) {
							char** as = v;
							*as = content;
							content = NULL;
						}

					}

				}

			}

			ignored--;
			break;

		case YXML_CONTENT:

			if (ignored == 0) {
				//if content is for this tag level (not a deeper ignored tag)
				if (!content)
					content = zstr_mk(32);

				content = zstrcat(content, cs); //accumulate content

			}

			break;

		case YXML_ELEMSTART:
	
			for (i = 0; handlers[i].name; i++) {
			

				if (!strcmp(handlers[i].name, parser->elem)) {
					printf("				IN %s\n", parser->elem);
					fhandler = i;
					break;
				}
			}

			if (fhandler == -1) {
				ignored++;
				printf(" Ignoring start tag %s  level %d\n", parser->elem, ignored);

			}
			break;


		}// end switch

		
		if (fhandler >= 0) {
			//have a handler to deal with somethinging
			void* v = (handlers[fhandler].offset) + (char*)(userdata);


			if (handlers[fhandler].op == ZXML_WRITE_INT) {
				int* as = v;
				*as = atoi(buf);
			}

			if (handlers[fhandler].op == ZXML_WRITE_FLOAT32) {
				zfloat32* as = v;
				*as = atof(buf);
			}

			if (handlers[fhandler].op == ZXML_ADD_STRING) {
				char** as = v;
				*as = zstrdup(buf);
			}

			if (handlers[fhandler].op == ZXML_ADD_VECTOR) {
				//allocate an object, iterate the subtable and add item to vector
				void* obj = ram_alloc(handlers[fhandler].allocate, handlers[fhandler].destructor); //TODO add destructor
				zparsexml(parser, rs, reader, handlers[fhandler].subhandler, obj);
				//then add to vector
				zvecT* vec = handlers[fhandler].offset + (char*)userdata;
				zvec_add(vec, obj);
			}

			zstr_reset(buf);




		}

		if (ignored < 0) {
			break;
		}

	}//end while

	ram_free(buf);
	ram_free(content);
	return 0;

}


yxml_t* zxml_mk(size_t stacksizeK) {

	if (stacksizeK == 0)
		stacksizeK = 32;

	size_t stacksize = stacksizeK * 1024;
	
	char* buffer = ram_alloc( sizeof(yxml_t) + stacksize, NULL);
	yxml_t* parser = (yxml_t*) buffer;  //front of buffer is the parser

	buffer += sizeof(yxml_t); //after the parser is the buffer the parser will use
		
	printf("  parser: %p    buffer: %p  difference: %d, %d\n", parser, buffer, (int)(buffer - parser), sizeof(yxml_t));

	yxml_init(parser, buffer, stacksize);

	return parser;
}



zxmlhandlerT* zxml_set_handler(zxmlhandlerT* h, char* name, zuint32 op, zuint32 offset, size_t allocate, zxmlhandlerT* subhandlers, void* destructor) {
	h->name = name;
	h->op = op;
	h->offset = offset;
	h->allocate = allocate;
	h->subhandler = subhandlers;
	h->destructor = destructor;
	return h;
}