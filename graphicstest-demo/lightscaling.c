#include <stdio.h>

int main (int argc, char** args)
{
	float unityrange=5;
	float maxlight=1000;
	float c,l,s;
	float d;
	float a;
	float sharpness=.2;
	
	c = 1/maxlight;
	s = (1-c) / (unityrange * unityrange);
	s=s*sharpness;
	
	l= (1-c - sharpness*(1-c)) / unityrange;
	
	printf( " cls = %f %f %f\n", c, l, s);
	
	for(d=0;d<20;d++){
		
		
		
		a = 1/(c+s*d*d + l*d);
		
		printf(" %f\t%f\n", d, a);
		
		
	}
	
	
	
	
	return 0;
}
