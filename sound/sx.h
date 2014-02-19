// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../time/tm.h"

#include <AL/al.h>


// initialize
zerror sx_init();
// todo: shutdown


#define SX_MONO				1
#define SX_STEREO			2
#define SX_STEREO_SPLIT		3
//STEREO_SPLIT means load the left and right channels into separate buffers

//buffer holds audio
typedef struct sx_sound_s
{
	zchar*		name;			//optional name value for tracking purposes
//	zbyte*		data;
//	zsize		data_size;
//	zint32		channels;
	
	ALuint		_al_buffer;
} sx_sound_t;

//a source plays audio
typedef struct sx_source_s
{
	//which buffer?
	sx_sound_t*	sound;
	
	//for delayed sounds
	tm_microstamp_t created;
	zuint32			time_to_start;

	//sound properties
	zfloat32		volume;  // 1.0 is full scale
	zfloat32		pitch;

	//zfloat32		xpos; //x panning -1 left to 1 right (0 center)
	zfloat32		pos[3];
	
	//openal data 
	ALuint		_al_source;
	zbool		_al_source_valid;
} sx_source_t;

//#define sx_buffer_data(sx_buffer_t_pointer)			((sx_buffer_t_pointer)->data)
//#define sx_buffer_data_size(sx_buffer_t_pointer)    ((sx_buffer_t_pointer)->data_size)

//allocates a sound buffer
//sx_sound_t* sx_sound_buffer_mk(zint32 channels, zsize_t byte_len);

//defines a sound
sx_sound_t* sx_sound_def(zint32 channels, zsize data_len_bytes, zbyte* data, zchar* name);



zbool sx_sound_delete(sx_sound_t* sound); //destroys a buffer

//plays a buffer at 'milliseconds' time from now. 0 means play immediately
void sx_sound_play(sx_sound_t* sound, zfloat32 volume, zfloat32 pitch, zuint32 milliseconds);  

//pans left or right -1 (left) to 1 (right)
void sx_sound_play_at(sx_sound_t* sound, zfloat32 volume, zfloat32 pitch, zuint32 milliseconds, float* pos);  


// OpenAL does all the 'processing' in a seperate thread
// this function just does some housekeeping like delete 'dead' sources, etc
void sx_process();


//define environment
typedef struct sx_reverb_s
{
	//stage 1
	zfloat32 echo_delay;
	zfloat32 echo_gain;

	//stage 2
	zfloat32 late_reverb_gain;
	zfloat32 late_reverb_decay;

} sx_reverb_t;


void sx_set_reverb(sx_reverb_t* reverb);

zerror  sx_deinterlace_audio(void* vdata, int bufsize, int nlace, void* vleft, void* vright);

//read 16 bit raw files
zsize sx_read_raw16(zchar* filename, zbool split_channels, zbyte** data0, zbyte** data1);
