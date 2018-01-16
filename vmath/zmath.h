//platform-dependent Vector math definitions
//These may be implemented as pure macros, or as calls into functions, or even inline asm.
//This version is simple, slow, and should work 'everywhere'
//It is presented as macros to 1)remove overhead of function calls and 
// 2)maybe a good (auto-vectorizing?) compiler can tweak the asm code to hell


#define PI 3.14159


//#define VMATH_PAD4

#define VEC4LEN 4


#ifdef VMATH_PAD4
#define VEC3LEN 4
#else
#define VEC3LEN 3
#endif


typedef struct namedv3_s
{
	zfloat32 x;
	zfloat32 y;
	zfloat32 z;
#ifdef VMATH_PAD4
	zfloat32 wpadding;
	/* Note: in a padded vec3, the 'w' element has a different name than in a real vec4
			This is to prevent a padded vec3 being used as a vec4
	*/
#endif
} namedv3;


typedef struct namedv4_s
{
	zfloat32 x;
	zfloat32 y;
	zfloat32 z;
	zfloat32 w;
} namedv4;


typedef union 
{
	namedv3 named;
	zfloat32 array[VEC3LEN];
} vec3;


typedef union 
{
	namedv3 v3;
	namedv4 named;
	zfloat32 array[VEC4LEN];
} vec4;



/* for compatibility with my old-ass code */
#define vec3x named.x
#define vec3y named.y
#define vec3z named.z
#define vec3p array

/* the preferred accessors */
#define VX named.x
#define VY named.y
#define VZ named.z
#define VW named.w
#define VP array



//set a vec3 from 3 floats
// a = (x,y,z)
#define vec3set(_v3_a,_v3_x,_v3_y,_v3_z)  {	\
	(_v3_a).named.x = (_v3_x);					\
	(_v3_a).named.y = (_v3_y);					\
	(_v3_a).named.z = (_v3_z);					\
}

//set a vec4 from  4 floats
#define vec4set(_v4_a,_v4_x,_v4_y,_v4_z,_v4_w)  {	\
	(_v4_a).named.x = (_v4_x);					\
	(_v4_a).named.y = (_v4_y);					\
	(_v4_a).named.z = (_v4_z);					\
	(_v4_a).named.w = (_v4_w);					\
}


// a=b
#define vec3mov(_v3_a, _v3_b)  _v3_a = _v3_b;
#define vec4mov(_v3_a, _v3_b)  _v3_a = _v3_b;


/* the following are supported for vec3 only (for now) */

// a +=b

#define vec3add(_v3_a, _v3_b) {		\
	(_v3_a).named.x += (_v3_b).named.x;	\
	(_v3_a).named.y += (_v3_b).named.y;	\
	(_v3_a).named.z += (_v3_b).named.z;	\
}

#define vec3sub(_v3_a, _v3_b) {		\
	(_v3_a).named.x -= (_v3_b).named.x;	\
	(_v3_a).named.y -= (_v3_b).named.y;	\
	(_v3_a).named.z -= (_v3_b).named.z;	\
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


#define vec3dot(_v3_a,_v3_b) \
(((_v3_a).named.x *  (_v3_b).named.x) + ((_v3_a).named.y *  (_v3_b).named.y) + ((_v3_a).named.z *  (_v3_b).named.z))


/* Square of the absolute value of vector*/

#define vec3abs_sq(_v3_a) (__sq((_v3_a).named.x)+__sq((_v3_a).named.y)+__sq((_v3_a).named.z))

#define vec3madd(_v3_a, _v3_s, _v3_b) {	\
	(_v3_a).named.x += (_v3_b).named.x *(_v3_s);\
	(_v3_a).named.y += (_v3_b).named.y *(_v3_s);\
	(_v3_a).named.z += (_v3_b).named.z *(_v3_s);\
}

#define vec3print(_v3_b) printf("<%f %f %f>", (_v3_b).named.x, (_v3_b).named.y, (_v3_b).named.z );


#define vec4print(_v4_b) printf("<%f %f %f %f>", (_v4_b).named.x, (_v4_b).named.y, (_v4_b).named.z,(_v4_b).named.w );



//normalize a vec3 to unit length
void vec3normalize( vec3* p);


/* returns true if a point is contained with an axis aligned box*/
/* min MUST contain the smaller x, y, and z values, and max the larger ones */
/* a positive border value shrinks the box in all dimenstions.  (negative enlarges) */

zbool vec3_point_in_box( vec3* min, vec3* point, vec3* max, float border);




//some misc non-vector match




//random number 0 to 1
float randf();


//random number -1 to 1
float randfs();

