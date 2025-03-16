

//Zdef proc zstrcatsub zstrcatsub:(s:String%; t:String&; st:Z32; c:Z32 -> String%);
char* zstrcatsub(char* dest, char* src, zsize start, zsize count);


//Zdef type FILE	File


/*

	"Somefilename" "a" :File #f
	f open
	f 100 read #b //read 100 byte
	
	f close

*/


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


//vector math functions

//Zdef stacked Vec3	


////Zdef struct namedv3	Vec3:x=x:Real;y=y:Real;z=z:Real;wpadding=padding:Real;
//Zdef struct namedv3	Vec3:x=x:Real;y=y:Real;z=z:Real;

//Zdef type	vec3		Vec3
//Zdef handler vec3add     +:(a:Vec3;b:Vec3->Vec3);
//Zdef handler vec3sub     -:(a:Vec3;b:Vec3->Vec3);
//Zdef handler vec3cross   Cross:(a:Vec3;b:Vec3->Vec3);
//Zdef handler vec3dot     Dot:(a:Vec3;b:Vec3->Real);




zbool ptrequal(void* a, void* b);


//Reflection
//Zgen genreflect.zz

//Traverse data types
//Zdef  struct	typeT	Type:name=name:String cpointer;size=size:N32;category=category:N32;ref=ref:Type&;len=len:N32;isPer=isPer:Bit;


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


//Zdef opaque vptrT CodeToken

//Zdef stacked ExecToken
//Zdef opaque vptrT ExecToken
//Zdef opaque vptrT Symbol
//Zdef stacked Symbol

/*
//uncompiled code


//
//Zdef handler tokenstring		.text:(t:CodeToken->String&);
*/


//new safer way






//compiled code

//Zdef opaque valueT TokenValue
//Zdef stacked TokenValue


//Zdef handler hasfirsttoken	.hasSub:(c:ExecToken->Bit);
//Zdef handler firsttoken		.sub:(c:ExecToken->ExecToken);
//Zdef handler tokenvaltype		.consttype:(c:ExecToken->Type&);
//Zdef handler tokenevaltype	.evaltype:(c:ExecToken->Type&);
//Zdef handler tokenprim		.prim:(c:ExecToken->String&);
//Zdef handler tokensymbol		.symbol:(c:ExecToken->Symbol);
//Zdef handler symboltype		.type:(s:Symbol->Type&);
//Zdef handler tokenstring	.text:(t:ExecToken->String&);
//Zdef handler tokennext	.next:(t:ExecToken->ExecToken);
//Zdef code primitive tokenval		.value:(t:ExecToken->TokenValue);
//Zdef code primitive symtoken  	.tokens:(s:Symbol->ExecToken);

//uncompiled code:

//Zdef opaque vptrT	SourceToken


//Zdef handler		  tokenforward []:(t:SourceToken&; n:N32 -> SourceToken%); 
//Zdef code primitive C_tokenforward 1 []:(t:SourceToken%; n:N32 -> SourceToken%);

//Zdef handler heretokenP	sys_Here:(->SourceToken%);

//Zdef handler tokenclip	clip:(t:SourceToken%->Code%);

//Zdef handler tokenstringcopy	1  .text:(t:SourceToken%->String%);
//Zdef code primitive C_tokenstringcopy	0  .text:(t:SourceToken&->String%);
//Zdef handler codecat			++:(c:Code&;d:Code%);  //appends d to c.  frees d
//Zdef handler tokeninsert		insert:(c:Code%->);



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



//Graphics (other header files after this)
//Zgen gengraphics.zz

//Zdef opaque gfx_vertex_bufferT VBuffer


//Zdef stacked GLSLBody
//Zdef opaque vptrT GLSLDrawProc
//zdef stacked GLSLDrawProc

//Zdef handler glslprocbody    glslSource:(v:String&;f:String&;s:Symbol->GLSLBody);
//Zdef handler prepshader      Prepare:(p:GLSLBody->GLSLDrawProc);
//Zdef handler execdraw        Draw:(p:GLSLDrawProc; prim:N32;start:N32; stop:N32);



