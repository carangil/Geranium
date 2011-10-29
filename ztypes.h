// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


/* Basic datatypes for my projects**/

#include <stdlib.h>

typedef int					zint32;
typedef unsigned int		zuint32;
typedef float				zfloat32;
typedef double				zfloat64;
typedef unsigned char		zbyte;
typedef size_t				zsize;
typedef unsigned long long	zuint64;
typedef unsigned char		zbool;
typedef char				zchar;

/* Handy constants to make code look a little cleaner*/
#define ztrue 1
#define zfalse 0

//I actually got a compile error that NULL was undefined!
#ifndef NULL
#define NULL 0
#endif
