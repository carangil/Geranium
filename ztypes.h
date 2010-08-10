/* Basic datatypes for my projects**/


typedef int					zint32;
typedef unsigned int		zuint32;
typedef float				zfloat32;
typedef double				zfloat64;
typedef unsigned char		zbyte;
typedef size_t				zsize;
typedef unsigned long long	zuint64;
typedef unsigned char		zbool;

/* Handy constants to make code look a little cleaner*/
#define ztrue 1
#define zfalse 0

//I actually got a compile error that NULL was undefined!
#ifndef NULL
#define NULL 0
#endif
