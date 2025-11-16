// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


/* Basic datatypes for my projects**/

#include <stdlib.h>
#include <stdio.h>

#ifndef ZTYPES
#define ZTYPES

typedef int			zint32;
typedef float			zfloat32;
typedef double			zfloat64;
typedef char			zchar;

//unsigned types
typedef unsigned char           zbyte;
typedef unsigned int		zuint32;
typedef unsigned long long	zuint64;
typedef size_t			zsize;

typedef unsigned short		zuint16;

/* Handy constants to make code look a little cleaner*/
typedef unsigned char		zbool;
#define ZTRUE 1
#define ZFALSE 0

typedef unsigned int zerror;
#define ZOK 0
#define ZERR 0xFFFFFFFF
/* ZERR is a generic error value.  Use any nonzero value for custom error codes*/

/* A few small 'translations' between linux and windows*/

#ifdef linux

#include <unistd.h> 		//brings in sleep()
#define stricmp strcasecmp	//stricmp is from windows land
#endif

#ifdef _WIN32
#include <windows.h>
#define sleep(sec)  Sleep( (int)((sec)*1000))
#define stricmp _stricmp
#endif

#endif
