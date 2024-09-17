

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


zbool ptrequal(void* a, void* b);


//Zdef handler glslprocbody	immediate% glsl:(vsource:String&;fsource:String&->GLSLBody);
//Zdef handler prepshader	prep:(p:GLSLBody->GLSLDrawProc);
//Zdef handler execdraw		draw:(p:GLSLDrawProc; prim:N32;start:N32; stop:N32);



//Reflection

//Traverse data types
//Zdef  struct	typeT	Type:name=name:String cpointer;size=size:N32;category=category:N32;ref=ref:Type&;len=len:N32;


//Zdef type size_t Z32
//Zdef noproto findType findType:(category:Z32; ref:Type&; name:String&; size:Z32->Type&);
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



//manipulate tokens and stuff

//Zdef stacked CodeToken
//Zdef opaque vptrT CodeToken

//Zdef stacked ExecToken
//Zdef opaque vptrT ExecToken
//Zdef opaque vptrT Symbol

//uncompiled code
//Zdef handler heretoken		here:(->CodeToken);
//Zdef handler tokenclip		clip:(t:CodeToken->Code%);
//Zdef handler codecat			++:(c:Code&;d:Code%);  //appends d to c.  frees d
//Zdef handler tokeninsert		insert:(c:Code%->);
//Zdef handler tokennext		.next:(t:CodeToken->CodeToken);
//Zdef handler tokenstring		.text:(t:CodeToken->String&);

//compiled code

//Zdef opaque valueT TokenValue
//Zdef stacked TokenValue


//Zdef handler firsttoken		.sub:(c:ExecToken->ExecToken);
//Zdef handler tokenvaltype		.consttype:(c:ExecToken->Type&);
//Zdef handler tokenevaltype	.evaltype:(c:ExecToken->Type&);
//Zdef handler tokenprim		.prim:(c:ExecToken->String&);
//Zdef handler tokensymbol		.symbol:(c:ExecToken->Symbol);
//Zdef handler symboltype		.type:(s:Symbol->Type&);
//Zdef code primitive C_tokenstring	.text:(t:ExecToken->String&);
//Zdef code primitive C_tokennext	.next:(t:ExecToken->ExecToken);
//Zdef code primitive tokenval		.value:(t:ExecToken->TokenValue);
//Zdef code primitive symtoken  	.tokens:(s:Symbol->ExecToken);

/* type categories */
#define SIMPLE	1
#define POINTERUSER 2
#define STRUCT 	3
#define ARRAYSTATIC 	4
#define ARRAYDYNAMIC 	5
#define FUNCTION 6
#define PRIMITIVE 7
#define POINTERPOSSESSIVE 8
#define CPOINTER 9
#define OPAQUE 10
#define VIRTUAL 11
#define LAST_REAL_TYPE 11