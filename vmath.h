//platform-dependent Vector math definitions
//These may be implemented as pure macros, or as calls into functions, or even inline asm.
//This version is simple, slow, and should work 'everywhere'
//It is presented as macros to 1)remove overhead of function calls and 
// 2)maybe a good (auto-vectorizing?) compiler can tweak the asm code to hell

typedef struct namedv_s
{
	zfloat32 x;
	zfloat32 y;
	zfloat32 z;
} namedv;

typedef union 
{
	namedv named;
	zfloat32 array[3];
} vec3;

#define vec3x named.x
#define vec3y named.y
#define vec3z named.z
#define vec3p array


//set a vec3 from 3 floats
// a = (x,y,z)
#define vec3set(_v3_a,_v3_x,_v3_y,_v3_z)  {	\
	(_v3_a).named.x = (_v3_x);					\
	(_v3_a).named.y = (_v3_y);					\
	(_v3_a).named.z = (_v3_z);					\
}


// a=b
#define vec3mov(_v3_a, _v3_b)  _v3_a = _v3_b;



// a +=b

#define vec3add(_v3_a, _v3_b) {		\
	(_v3_a).named.x += (_v3_b).named.x;	\
	(_v3_a).named.y += (_v3_b).named.y;	\
	(_v3_a).named.z += (_v3_b).named.z;	\
}

// a= a*s

#define vec3scale(_v3_a,  _v3_s) {  \
	(_v3_a).named.x *= (_v3_s); \
	(_v3_a).named.y *= (_v3_s); \
	(_v3_a).named.z *= (_v3_s); \
}

// a = b cross c

#define vec3cross(_v3_a,_v3_b, _v3_c) {  \
	(_v3_a).named.x = (_v3_b).named.y * (_v3_c).named.z - (_v3_c).named.y * (_v3_b).named.z; \
 	(_v3_a).named.y = (_v3_b).named.z * (_v3_c).named.x - (_v3_c).named.z * (_v3_b).named.x; \
	(_v3_a).named.z = (_v3_b).named.x * (_v3_c).named.y - (_v3_c).named.x * (_v3_b).named.y; \
}

#define __sq(__sqx)  ((__sqx)*(__sqx))


/* Square of the absolute value of vector*/

#define vec3abs_sq(_v3_a) (__sq((_v3_a).named.x)+__sq((_v3_a).named.y)+__sq((_v3_a).named.z))

#define vec3madd(_v3_a, _v3_s, _v3_b) {	\
	(_v3_a).named.x += (_v3_b).named.x *(_v3_s);\
	(_v3_a).named.y += (_v3_b).named.y *(_v3_s);\
	(_v3_a).named.z += (_v3_b).named.z *(_v3_s);\
}

