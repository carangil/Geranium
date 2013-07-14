#include <stdio.h>

#include "../ztypes.h"
#include <malloc.h>
#include <math.h>
#include "../sound/sx.h"
#include "../memory/ram.h"

#include <windows.h>


#define SHOWERRORS

#ifdef SHOWERRORS

#define ERRORDISPLAY(zzyzzy)	printf("error %x at %s:%d\n", (zzyzzy), __FILE__, __LINE__)

#else
#define ERRORDISPLAY(zzyzzy) zzyzzy
#endif



int main(int argc, char** args)
{

	sx_sound_t* music = NULL; 
	sx_sound_t* sound = NULL; 
	sx_sound_t* sound2 = NULL; 
	sx_sound_t* sound3 = NULL; 
	

//	FILE* file = NULL;
	zbyte* data = NULL;
	zbyte* ldata = NULL;
	zbyte* rdata = NULL;

//	data = ram_alloc(BUFSIZE, NULL);
//	ldata = ram_alloc(BUFSIZE/2, NULL);
//	rdata = ram_alloc(BUFSIZE/2, NULL);

	
/*	file = fopen("music.raw", "rb");
	
	if (file)
	{
		fread(data, 1, BUFSIZE, file);
		fclose(file);
	}
	else
	{
		printf(" no file\n");
	}
*/

	

	ERRORDISPLAY(sx_init());

	
	//sx_set_echo(10.0);

/*	if ( 0 == sx_read_raw16("music.raw", zfalse, &data, NULL))
		printf(" not read file\n");

	sx_deinterlace_audio( data, BUFSIZE, 2,  ldata, rdata);
	*/
	{
		short rnd[44100];
		

		int i;
		int size = 44100 * 100 ;
		 short* ss = ram_alloc( size * sizeof(*ss) ,0);

		 int acc=0;

		for (i=0;i< 44100;i++)
			rnd[i] = rand();



		for (i=0;i<size;i++)
		{
			
			//int sec = i/440.0;
			//float f =  sin( 3.14159/32 * i) *(i/1000)*1000         ;
			//1double f = i/10000.0*sin(i*i/100000000.0);
			
		//	ss[i] =  10000*sin( i * f* 2 * 3.14159 / 44100   );

		//	int f = 1+sec*10;
			//int j = 100000.0/(i*.1+.01);

		//	int p =   512 + 512* sin((i/10000 )  );

		//	ss[i] = rnd[( i +(100000*(i/100))  )  % (p+3)% 44100];


			int t = i;
			int x,y;

			unsigned char c =   ((t*("36364689"[t>>13&7]&15))/12&128)
+(((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127);


			ss[i]= c*50;

			//ss[i]=ss[i]/5;

			//ss[i] =  10000* sin(i* ( f )*3.141*2/44100)   ;

		}

		

		music = sx_sound_def(1, size * sizeof(*ss), ss, "music");
	}


	//music = sx_sound_def(2, BUFSIZE, data, "music");

	

	//ram_free(data);


	{
		zsize buffer_size = 0;

		if (buffer_size = sx_read_raw16("fx/hit1.raw", zfalse, &data, NULL))
		{
			sound = sx_sound_def(1, buffer_size, data, "hit");
		}

		
		if (buffer_size = sx_read_raw16("fx/hit2.raw", zfalse, &data, NULL))
		{
			sound2 = sx_sound_def(1, buffer_size, data, "hit2");
		}


		if (buffer_size = sx_read_raw16("fx/rip.raw", zfalse, &data, NULL))
		{
			sound3 = sx_sound_def(1, buffer_size, data, "rip");
		}

	}



	printf(" Music is %p\n", music);

	sx_sound_play(music, 1,1,0);

	while(1);



	//sx_sound_play(music, 1.0);
/*
	Sleep(100);
	sx_sound_play(music, 0.8);
	Sleep(100);
	sx_sound_play(music, 0.6);
	Sleep(100);
	sx_sound_play(music, 0.4);
	Sleep(100);
	sx_sound_play(music, 0.2);

	*/

	printf(" entering loop\n");
	{
		int i=0;

		sx_reverb_t reverb;

	

		reverb.echo_gain = .3;
		reverb.echo_delay = .1;

		reverb.late_reverb_gain = 1;
		reverb.late_reverb_decay = 20;

		
		
		sx_sound_play(sound2, 0.5, 2,  0);

		Sleep(1000);

		sx_set_reverb(&reverb);
		sx_sound_play(sound2, 0.5, 2,  0);
		
		
	//	sx_sound_play(sound, 1.0, 000);
	//	sx_sound_play(sound, 1.0, 1000);
	//	sx_sound_play(sound, 1.0, 1500);
	//	sx_sound_play(sound, 1.0, 1400);
	//	sx_sound_play(sound, 1.0, 110);
	//	sx_sound_play(sound, 1.0, 4000);
	//	sx_sound_play(sound, 1.0, 3500);

	
		Sleep(1000);
		i=-1;
		while(1)
		{

			Sleep(5);
			
			if(((((i=i+1))%500))==0)
			{
				printf("BOOOO-------------------------------------\n");
				sx_sound_play(sound, (rand() & 255) / 255.0 , (rand() & 127) / 255.0+.01, rand()&511 );
				sx_sound_play(sound2, (rand() & 255) / 255.0 , (rand() & 127) / 255.0+.01, rand()&511 );
				sx_sound_play(sound2, (rand() & 255) / 255.0 , (rand() & 127) / 255.0+.01, rand()&511 );
				
				//if((i%300)==0)
				//	sx_sound_play(sound3, (rand() & 255) / 255.0, (rand() & 511) / 255.0 + .01 , rand()&127);

				//printf(" echo delay %f\n", reverb.echo_delay);
				//reverb.echo_delay +=.01;
				//sx_set_reverb(&reverb);

			}


			sx_process();

			

		}
	}
	

	return 0;
}