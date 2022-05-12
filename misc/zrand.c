#include <zrand.h>

#ifdef _WIN32

#define ZRANDMAX (0xFFFFFFFF)
zuint32 zrand() {
	unsigned int r;
	if (!rand_s(&r))
		return r;

	fprintf(stdout, "rand_s failed\n");
	exit(1);

}


#else

//unix implementation of random
#include <stdio.h>
#define ZRANDMAX (0xFFFFFFFF)

zuint32 zrand(){

	zuint32 r;
	int c = 0;
	static FILE* urandom = NULL;
	static zbool broken = ZFALSE;

	//try to open first time
	if (!urandom && !broken)
		urandom = fopen ("/dev/urandom", "rb");

	//if file is open (now or from before), read from it
	if (urandom)
		c = fread(&r, sizeof(r), 1, urandom);
		
	//if re read, return the number
	if (c==1)
		return r;

	//if the above didn't work, its broken.
	//set broken flag so we don't try to open the file again
	//and fallback to garbage

	if (!broken)
		fprintf(stderr, " zrand is broken, falling back to rand (that's bad)\n");

	broken = ZTRUE;

	return (rand()<<16) ^ rand(); //shift left and xor is in case RAND is only 16-bi(t

}

#endif


zfloat32 zrandf(zfloat32 min, zfloat32 max)
{

	zfloat32 range = max-min;

	zfloat32 r = range * (zrand()) / ZRANDMAX;
	r+= min;
	if (r<min)
		return min;

	if (r>max)
		return max;


	return  r;
}

