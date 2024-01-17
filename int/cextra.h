

#define Counted_cstring *CString


#define Noproto_zstrdup Zstrdup:(c:String&->CString%);
char* zstrdup(char*);




#define Typemap_FILE *File
#define Noproto_fopen File_Open
FILE* fopen(char* filename, char* mode);

#define Noproto_fclose Close
int fclose(FILE* f);


#define Noproto_fwrite
int fwrite( void* ptr, int size, int nmemb, FILE* stream);

#define Noproto_fread
int fread( void* ptr, int size, int nmemb, FILE* stream);


#define Procmap_zrand RandomZ32
zuint32 zrand();

#define Procmap_zrandf RandomReal
zfloat32 zrandf(zfloat32 min, zfloat32 max);


//try to make 16-bit integers as an 'extension' instead of the core language
//the language still has 32-bit integers, but can now load/store 16-bit values

//ByteObj_name directive creates a simple type of sizeof(name) bytes.
#define ByteObj_zuint16 *N16

#define Procmap_store16 =
void store16(zint32 val, zuint16* zp);

#define Procmap_load16 @
zint32 load16(zuint16* z); 

//vector math functions

#define Handler_vec3add     +:(a:Vec3;b:Vec3->Vec3);
#define Handler_vec3sub     -:(a:Vec3;b:Vec3->Vec3);
#define Handler_vec3cross   Cross:(a:Vec3;b:Vec3->Vec3);
#define Handler_vec3dot     Dot:(a:Vec3;b:Vec3->Real);
