#include <stdio.h>
#include "../ztypes.h"
#include "../memory/ram.h"
#include "vector.h"
#include "zarray.h"
#include "zstring.h"

void zstring_init(zstring* s)
{
	s->chars=NULL;
}

zbool zstring_set(zstring* s, char* cs)
{
	int len = strlen(cs);
	int lenalloc = len+1;

	if (lenalloc < 8)
		lenalloc = 8;

	if (zarray_init(s->chars, len, lenalloc))
	{
		strcpy(s->chars, cs);
		return ZTRUE;
	}
	
	return ZFALSE;
}

//makes space for a string to be placed here
zbool zstring_setspace(zstring* s, int len)
{

	if (zarray_init(s->chars,len, (len+3 )  ))
	{
		//printf(" %d + 1 allocated\n", len);

		memset( s->chars, '*', len+3);
		s->chars[len+1] = '<';


		return ZTRUE;
	}

	
	return ZFALSE;
}

#if 0
//called by zstring_catf
zbool zstring_extraspace(zstring* s, int extra)
{
	//get current string length plus extra
	int newlen = zarray_count(s->chars) +extra ;
	char* oldplace = s->chars;
	printf(" make sure at least %d\n", newlen+3);
	if (zarray_atleast(&s->chars, (newlen +3))  ) 
	{
		if (oldplace != s->chars)
				printf("moved %p to %p\n", oldplace, s->chars);
		return ZTRUE;
	}
	else
	{
		printf("resize fail\n");
		return ZFALSE;
	}
}
#endif


//how many byes in this string
int zstring_len(zstring* s)
{
	if (!s)
		return 0;

	return zarray_count( s->chars);
}


//how many bytes, allocated, including null terminator
int zstring_size(zstring* s)
{
	if (!s)
		return 0;

	return zarray_size( s->chars);
}

//append null terminated string to this string
//growing if necessary
//allow start offset to be specified
//allow length of copy to be specified (0 means copy whole string)
//return is the new string length

//cs len is the actual length of cs.
//len_copy is the length of stuff to copy


int zstring_cat_internal(zstring* s, char* cs, int extralen)
{
	int len;

	if (!s || !cs)
		return 0;


/*	printf(" original string %d %d count, added string %d\n",
		zstring_len(s), 
		strlen(s->chars), 
		strlen(cs));
		*/

	len = zstring_len(s);	//length of existing string
	len += extralen;		//new length

	if (zarray_atleast(&s->chars, len + 1))
	{
		//memset(s->chars+zarray_count(s->chars), '.', extralen);
		strncpy( s->chars + zarray_count(s->chars) , cs, extralen);
		s->chars[ len ] = 0; //manually terminate end

		zarray_count_set(s->chars, len);
/*		printf(" %d asked to be allocated, %d is new string count, %d is size\n",
			len +1,
			zstring_len(s),
			zarray_size(s->chars));
*/
		return len;
	}

	return 0;
}


int zstring_catc(zstring* s, char* cs, int start, int len_copy)
{
	int input_len = strlen(cs);

	//don't start beyond the input string
	if (start >= input_len)
		return zstring_len(s);
	
	//advance to wanted position of input string
	input_len -= start;
	cs += start;

	//copy whole string if length not specified
	if (len_copy == 0)
		len_copy = input_len;

	if (len_copy > input_len)
		len_copy = input_len;

	return zstring_cat_internal( s, cs, len_copy );
}


int zstring_catz(zstring* s, zstring* s2, int start, int len_copy)
{
	char* cs = s2->chars;
	int input_len = zstring_len(s2);

//	printf(" zstring_catz  %s + %s \n", s->chars, s2->chars);

	//don't start beyond the input string
	if (start >= input_len)
		return zstring_len(s);
	
	//advance to wanted position of input string
	input_len -= start;
	cs += start;

	//copy whole string if length not specified
	if (len_copy == 0)
		len_copy = input_len;

	if (len_copy > input_len)
		len_copy = input_len;

	return zstring_cat_internal( s, cs, len_copy );
}

//add a character to the end
int zstring_catchar(zstring* s, char c)
{
	char cc[2];
	cc[0]=c;
	cc[1]=0;

	if (s) {
		return zstring_cat_internal(s, cc, 1 );
	}

	return 0;
}

zbool zstring_delete(zstring* s)
{
	if (s && s->chars) 
	{
	//	printf(" FREEING %s\n", s->chars);
		ram_free(s->chars);	
		s->chars = NULL;
	}

	return ZTRUE;
}


zbool zstring_valid(zstring* s)
{
	if (s->chars)
		return ZTRUE;

	return ZFALSE;

}

char* zstring_detach(zstring* s)
{
	char* c = s->chars;

	s->chars = NULL;
	
	return c;
}


#if 0
//puts the result of sprintf onto the end of a zstring
#define zstring_catf(dest,  ...) (    zstring_extraspace(dest, snprintf(NULL, 0,  __VA_ARGS__)) ?(   (dest)->chars_za.count+= snprintf( (dest)->chars+ zstring_len(dest)  ,  zstring_size(dest)-zstring_len(dest) ,  __VA_ARGS__)      ) : 0   )
#endif


