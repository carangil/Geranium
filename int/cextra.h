//Zdef   struct		typeT	Type:name=name:String cpointer;size=size:Z32;category=category:Z32;ref=ref:Type&;

//Zdef    noproto zstrdup  Strdup
char* zstrdup(char*);

//Zdef type FILE	File

//Zdef noproto fopen File_Open
FILE* fopen(char* filename, char* mode);

#define Procmap_zrand RandomZ32
zuint32 zrand();

#define Procmap_zrandf RandomReal
zfloat32 zrandf(zfloat32 min, zfloat32 max);


//try to make 16-bit integers as an 'extension' instead of the core language
//the language still has 32-bit integers, but can now load/store 16-bit values
//Zdef opaque zuint16 N16

//Zdef store16 =
void store16(zuint16 val, zuint16* zp);

//Zdef load16 @
zint32 load16(zuint16  * z); 

//Zdef opaque gfx_vertex_bufferT VBuffer

//vector math functions

//Zdef stacked Vec3
//Zdef struct namedv3	Vec3:x=x:Real;y=y:Real;z=z:Real;
//Zdef type	vec3		Vec3
//Zdef handler vec3add     +:(a:Vec3;b:Vec3->Vec3);
//Zdef handler vec3sub     -:(a:Vec3;b:Vec3->Vec3);
//Zdef handler vec3cross   Cross:(a:Vec3;b:Vec3->Vec3);
//Zdef handler vec3dot     Dot:(a:Vec3;b:Vec3->Real);

//Zdef type size_t Z32

//Zdef noproto findType ZFindType:(category:Z32; ref:Type&; name:String&; size:Z32->Type&);
typeT* findType(zuint32 category, typeT* ref, char* name, size_t len);
void printTypeNoRedirect(typeT* ty, zbool line, zbool skipmembers);

void testComputeShader();

//Zdef proc type_member  []:(ty:Type&; n:N32 -> Type&);
typeT* type_member(typeT* t, zuint32 i);
/*	
	can use  ->name
			 ->category
			 ->size
		     ->offset
			 ->ref		//if a member, pointer, or array type, this is what the member is, or is an array or pointer of
*/

zbool ptrequal(void* a, void* b);

//Zdef handler fillvbowrapper  fillvbowrapper:(a:VBO%;n:N32->VBO%);
