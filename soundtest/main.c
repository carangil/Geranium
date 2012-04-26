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

#define BUFSIZE (44100*2*2*100)

int main(int argc, char** args)
{

	sx_sound_t* music = NULL; 
	sx_sound_t* sound = NULL; 
	

//	FILE* file = NULL;
	zbyte* data = NULL;
	zbyte* ldata = NULL;
	zbyte* rdata = NULL;

	data = ram_alloc(BUFSIZE, NULL);
	ldata = ram_alloc(BUFSIZE/2, NULL);
	rdata = ram_alloc(BUFSIZE/2, NULL);

	/*
	file = fopen("music.raw", "rb");
	
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

	if ( 0 == sx_read_raw16("music.raw", zfalse, &data, NULL))
		printf(" not read file\n");

	sx_deinterlace_audio( data, BUFSIZE, 2,  ldata, rdata);


	//music = sx_sound_def(2, BUFSIZE, data, "music");

	music = sx_sound_def(1, BUFSIZE/2, rdata, "music");

	ram_free(data);


	{
		zsize buffer_size = 0;

		if (buffer_size = sx_read_raw16("fx/hit1.raw", zfalse, &data, NULL))
		{
			sound = sx_sound_def(1, buffer_size, data, "hit");
		}

	}



	printf(" Music is %p\n", music);

//	sx_sound_play(music, 1.0);

	



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



		reverb.echo_gain = .1;
		reverb.late_reverb_gain = .5;
		reverb.late_reverb_decay = 10;
		reverb.echo_delay = .1;

		sx_set_reverb(&reverb);


		sx_sound_play(sound, 1.0, 0);
	//	sx_sound_play(sound, 1.0, 000);
	//	sx_sound_play(sound, 1.0, 1000);
	//	sx_sound_play(sound, 1.0, 1500);
	//	sx_sound_play(sound, 1.0, 1400);
	//	sx_sound_play(sound, 1.0, 110);
	//	sx_sound_play(sound, 1.0, 4000);
	//	sx_sound_play(sound, 1.0, 3500);

	
		Sleep(1000);

		while(1)
		{

			Sleep(20);
			
			if((i=  ((i+1)%5))==0)
			{
				printf("BOOOO-------------------------------------\n");
				sx_sound_play(sound, 1.0, 10000 );
			}


			sx_process();

			

		}
	}
	

	return 0;
}