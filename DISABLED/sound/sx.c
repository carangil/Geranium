#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <AL/efx-creative.h>


#include "../ztypes.h"
#include <malloc.h>
#include <math.h>
#include "../memory/ram.h"

#include "sx.h"

#include "../structures/vector.h"

/* Internal defines */
//#define SHOWALERROR   printf(" AL ERROR %x %s %d\n", alGetError(), __FILE__, __LINE__);

#define SHOWALERROR showalerror(__FILE__, __LINE__);

void showalerror(char* file ,  int line)
{
	int x = alGetError();
	if(x)
		printf(" AL ERROR %x %s %d\n", x, file, line);
}


#define _SX_FORMAT_MONO		AL_FORMAT_MONO16
#define _SX_FORMAT_STEREO	AL_FORMAT_STEREO16
#define _SX_SRATE			44100

#define _SX_PREALLOCATE_SOURCES
#define _SX_SOURCE_LIMIT	16

/* Global data */

typedef struct sx_al_data_s
{
	const ALCchar* default_device_name;
	ALCdevice* device;
	ALCcontext* context;


	/* For preallocated sources */
	
#ifdef _SX_PREALLOCATE_SOURCES
	ALuint* al_sources;
	zint32	al_sources_remaining;
#endif

	/* For EFX*/
	ALuint al_fx_slot;
	ALuint al_fx;
	zbool fx_created;

} sx_al_data_t;


static sx_al_data_t g_sx_al_data;

static vec_t sx_source_t_auto_source_list;  //contains all the automatically generated sources

/* EFX Functions (optional) */

LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = NULL;
LPALGENEFFECTS alGenEffects = NULL;
LPALEFFECTI alEffecti = NULL;
LPALEFFECTF alEffectf = NULL;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = NULL;
#define ALIMPORT(xyzzy) printf(" %s is %p\n", #xyzzy , xyzzy = alGetProcAddress(#xyzzy));

/* Perform extra initialization on optional elements */


static void _al_extra_inits()
{
	/* Initializes extended stuff we don't really need, but are extra */

	ALIMPORT(alGenAuxiliaryEffectSlots);
	ALIMPORT(alGenEffects);
	ALIMPORT(alEffecti);
	ALIMPORT(alEffectf);
	ALIMPORT(alAuxiliaryEffectSloti);

	alGetError(); //reset any leftever errors


	/* Create the effects slots*/
	alGenAuxiliaryEffectSlots(1, &g_sx_al_data.al_fx_slot);

	/* Create an effect*/
	alGenEffects(1, &g_sx_al_data.al_fx);

	/* We use a reverb affect in SX */
	alEffecti(g_sx_al_data.al_fx, AL_EFFECT_TYPE, AL_EFFECT_REVERB);


	/* Lets set the gain to zero, so someone has to turn it on to use it */
	alEffectf(g_sx_al_data.al_fx, AL_REVERB_GAIN, 0.0);
	alEffectf(g_sx_al_data.al_fx, AL_REVERB_GAINHF, 0.0);
	

	
	/* Set the effect to the slot */
	alAuxiliaryEffectSloti( g_sx_al_data.al_fx_slot, AL_EFFECTSLOT_EFFECT, g_sx_al_data.al_fx);

	if (alGetError() == AL_NO_ERROR)
	{
		g_sx_al_data.fx_created = ztrue;
	}

	SHOWALERROR;
}

#ifdef _SX_PREALLOCATE_SOURCES



zerror sx_alloc_sources()
{
	/* Called if preallocating sources */

	g_sx_al_data.al_sources = ram_alloc(sizeof(ALuint) * _SX_SOURCE_LIMIT, NULL);

	if (g_sx_al_data.al_sources)
	{
		alGetError(); //reset any leftever errors

		alGenSources(_SX_SOURCE_LIMIT, g_sx_al_data.al_sources);
		
		g_sx_al_data.al_sources_remaining = _SX_SOURCE_LIMIT;

		if( alGetError() == AL_NO_ERROR)
		{
			return ZOK;
		}
		else
		{
			ram_free(g_sx_al_data.al_sources);
			g_sx_al_data.al_sources = NULL;
		}
	}

	return ZERR;
}

zerror sx_get_al_source(sx_source_t* source)
{
	if (g_sx_al_data.al_sources_remaining > 0)
	{
		g_sx_al_data.al_sources_remaining--;
		source->_al_source = g_sx_al_data.al_sources[g_sx_al_data.al_sources_remaining];
		source->_al_source_valid = ztrue;
		printf(" using preallocated source %x  %d remain\n", source->_al_source , g_sx_al_data.al_sources_remaining);


	

		return ZOK;
	}
	return ZERR;
}


zerror sx_release_al_source(sx_source_t* source)
{
	if (g_sx_al_data.al_sources_remaining >= _SX_SOURCE_LIMIT)
	{
		printf(" ERROR sources have been double-releases, or non-sources have been released! \n");
		return ZERR;
	}

	if (source->_al_source_valid)
	{

		g_sx_al_data.al_sources[g_sx_al_data.al_sources_remaining] = source->_al_source;
		
		g_sx_al_data.al_sources_remaining++;

		printf(" releasing preallocated source %x\n", source->_al_source);

		source->_al_source_valid = zfalse;
		source->_al_source = 0;
		
	}

	return ZOK;
}

#endif

zerror sx_init()
{
	ALfloat listener_pos[] = {0,0,0};
	ALfloat listener_vel[] = {0,0,0};
	ALfloat listener_ori[] = {0,0,-1, 0,1,0};

	//memset( &g_sx_al_data, 0, sizeof(g_sx_al_data));
	ram_clear( &g_sx_al_data,  sizeof(g_sx_al_data));

	g_sx_al_data.default_device_name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	printf(" default device name %s\n", g_sx_al_data.default_device_name);

	g_sx_al_data.device = alcOpenDevice(g_sx_al_data.default_device_name);

	if (g_sx_al_data.device)
	{
		g_sx_al_data.context = alcCreateContext(g_sx_al_data.device, NULL);
		if (g_sx_al_data.context)
		{
			alcMakeContextCurrent(g_sx_al_data.context);
			alcProcessContext(g_sx_al_data.context);

			/* Now set some basic 3d positioning (default) */
			alListenerfv(AL_POSITION, listener_pos);
			SHOWALERROR
			alListenerfv(AL_VELOCITY, listener_vel);
			SHOWALERROR
			alListenerfv(AL_ORIENTATION, listener_ori);
			SHOWALERROR


			if (alGetError()== AL_NO_ERROR)
			{
				if (vec_mk(&sx_source_t_auto_source_list, 8))
				{
					_al_extra_inits(); /*Initialize optional parts */

#ifdef _SX_PREALLOCATE_SOURCES
					return sx_alloc_sources();
#else
					return ZOK;
#endif



				}
				else
				{
					return ZERR;
				}
			}
			else
				return ZERR;			
		}
	}
	return ZERR;
}




#if 1

zerror  sx_deinterlace_audio(void* vdata, int bufsize, int nlace, void* vleft, void* vright)
{
	int i;
	int len = bufsize / nlace / 2;

	if (len * 2 * nlace != bufsize)
		return ZERR;
	

	/* 16-bit */
	if (nlace==2)
	{
		zuint16* data	= vdata;
		zuint16* left	= vleft;
		zuint16* right	= vright;

		for (i=0;i < len; i++)
		{
			left[i] = data[2*i];
			right[i] = data[2*i+1];
		}

	}
	return 
		ZERR;  // can't handle this yet
	
	return ZOK;

}
#endif


zbool sx_sound_delete(sx_sound_t * sound )
{
	ram_free(sound->name);

	alDeleteBuffers(1, & (sound->_al_buffer));

	//ram_shallow_free(sound);
	return ztrue;
}

zbool sx_sound_destroy(void* s)
{
	sx_sound_t * sound = s;
	return sx_sound_delete(sound);
}


sx_sound_t* sx_sound_def(zint32 channels, zsize data_len_bytes, zbyte* data, zchar* name)
{
	sx_sound_t* sound = NULL;

	if (channels < 1 || channels > 2)
		return NULL;

	sound = ram_alloc(sizeof(*sound), sx_sound_destroy);
	
	if (name)
	{
		sound->name = ram_strdup(name);
	}
SHOWALERROR
	alGenBuffers(1, &(sound->_al_buffer));
SHOWALERROR
	alBufferData(sound->_al_buffer, (channels == 1? _SX_FORMAT_MONO : _SX_FORMAT_STEREO), data, data_len_bytes, _SX_SRATE); 



SHOWALERROR

	return sound;
}


zbool sx_delete_source(sx_source_t* source)
{

#ifdef _SX_PREALLOCATE_SOURCES
	if (source->_al_source_valid)
	{
		sx_release_al_source( source);
	}
#else

	alDeleteSources( 1, &(source->_al_source));

#endif

	//ram_shallow_free(source);
	return ztrue;
}


sx_source_t* sx_source_mk( sx_sound_t* sound)
{
	sx_source_t* source = NULL;
	zerror error = ZOK;
	
	if (!sound)
		return NULL; //need sound
	
	source = ram_alloc(sizeof(*source), sx_delete_source);
	
	if (source)
	{
SHOWALERROR
#ifdef _SX_PREALLOCATE_SOURCES

	
	
	printf(" deferring source binding\n");

#else
	SHOWALERROR
		alGenSources(1, &(source->_al_source));
		
	SHOWALERROR
#endif

#ifndef _SX_PREALLOCATE_SOURCES
		//apply fx if we have it
		if (g_sx_al_data.fx_created)
		{
			//printf(" applying fx\n");
			SHOWALERROR
			alSource3i(&(source->_al_source), AL_AUXILIARY_SEND_FILTER, g_sx_al_data.al_fx_slot, 0, AL_FILTER_NULL);
			SHOWALERROR
		}
#endif


		source->sound = sound;

		printf(" Created AL source %x with AL buffer %x (%s)\n", source->_al_source, source->sound->_al_buffer, source->sound->name? source->sound->name : "unnamed");

	}
	return source;
}


//assigns an openal source to a sound (if any are available) and plays it  
void _sx_bind_and_play_source(sx_source_t* source)
{
	//ALfloat pos [] = {0,.1,.1};

	//get an al source if we don't have one already
#ifdef _SX_PREALLOCATE_SOURCES
	sx_get_al_source(source);
#endif

//	pos[0] = source->xpos ;
	
SHOWALERROR
	alSourcei( source->_al_source, AL_BUFFER, source->sound->_al_buffer);
SHOWALERROR
	alSourcef( source->_al_source, AL_PITCH, source->pitch);
SHOWALERROR
	alSourcef( source->_al_source, AL_GAIN, source->volume);
SHOWALERROR
	alSourcefv( source->_al_source, AL_POSITION, source->pos);

SHOWALERROR
	alSourcei( source->_al_source, AL_LOOPING, AL_FALSE);
SHOWALERROR

	//experimental:set the slot
	if (g_sx_al_data.fx_created)
	{
			//printf(" applying fx\n");
			SHOWALERROR
 			alSource3i((source->_al_source), AL_AUXILIARY_SEND_FILTER, g_sx_al_data.al_fx_slot, 0, AL_FILTER_NULL);
			SHOWALERROR
	}



	//play immediately
	alSourcePlay(source->_al_source);

}




//generates a source and plays a sound
//the source is automatically destroyed when the sound has completed

void sx_sound_play_at(sx_sound_t* sound, zfloat32 volume, zfloat32 pitch,  zuint32 milliseconds, float* pos)
{
	sx_source_t* source = NULL;


	if (volume > 1.0)
		volume= 1.0;

	if (volume < 0.0)
		volume = 0.0;

	if (sound)
	{
		source = sx_source_mk(sound);

		if (pos)
		{
			source->pos[0]=pos[0];
			source->pos[1]=pos[1];
			source->pos[2]=pos[2];
		}
		else
		{
			source->pos[0]=0;
			source->pos[1]=0;
			source->pos[2]=0;
		}

		if (source)
		{
			source->volume = volume;
			source->pitch = pitch;

			if (milliseconds ==0)
			{
				_sx_bind_and_play_source(source);
			}
			else
			{
				//play later
				tm_get_microstamp(&(source->created));
				source->time_to_start = milliseconds * 1000;
			}


			vec_add( &sx_source_t_auto_source_list, source);
		}	
	}
}

void sx_sound_play(sx_sound_t* sound, zfloat32 volume, zfloat32 pitch,  zuint32 milliseconds)
{
	sx_sound_play_at(sound, volume, pitch, milliseconds, NULL);

}



void sx_process()
{
	zint32 i;
	ALint val = 0;
	sx_source_t* source;
	tm_microstamp_t now;
	zuint32 diff;

	tm_get_microstamp(&now);

	//printf("$");

//	printf(" sound table\n");


	for (i=0; i< vec_count(&sx_source_t_auto_source_list); i++)
	{
		source = vec_get_at(&sx_source_t_auto_source_list, i);

		//playing sounds have time_to_start set to zero
		if (source->time_to_start == 0)
		{
#ifdef  _SX_PREALLOCATE_SOURCES
			if (source->_al_source_valid)
#endif
			{

				alGetSourcei(source->_al_source, AL_SOURCE_STATE, & val);

				if (val == AL_STOPPED)
				{
					ram_free(vec_remove_unordered(&sx_source_t_auto_source_list, i));

					i--; //repeat last index since it has a new one now
					continue;
				}
			}

		}
		else /* nonzero time-to-start means source hasn't started playing yet*/
		{

			diff = tm_diff_microstamp_us( &source->created, &now);
			//printf(" %d microseconds since create, will activate at %d\n", diff, source->time_to_start);

			if (diff > source->time_to_start)
			{
				source->time_to_start = 0;
				
				_sx_bind_and_play_source(source);
				
			}else
			{
				printf(" sound will start in %d\n", source->time_to_start-diff);
			}
		}
	}
}





void sx_set_reverb(sx_reverb_t* reverb)
{

	if (!g_sx_al_data.fx_created)
		return;

/* Total reverb volume */

alEffectf(g_sx_al_data.al_fx, AL_REVERB_GAIN, 1.0);
SHOWALERROR
alEffectf(g_sx_al_data.al_fx, AL_REVERB_GAINHF, 1.0);
SHOWALERROR

//initial reflections

alEffectf(g_sx_al_data.al_fx, AL_REVERB_REFLECTIONS_DELAY, reverb->echo_delay);

SHOWALERROR

alEffectf(g_sx_al_data.al_fx, AL_REVERB_REFLECTIONS_GAIN,  reverb->echo_gain);
SHOWALERROR


//late reverb

alEffectf(g_sx_al_data.al_fx, AL_REVERB_DECAY_TIME, reverb->late_reverb_decay);
SHOWALERROR

alEffectf(g_sx_al_data.al_fx, AL_REVERB_LATE_REVERB_GAIN, reverb->late_reverb_gain);

//alEffectf(g_sx_al_data.al_fx, AL_REVERB_LATE_REVERB_DELAY, .1);


SHOWALERROR


alAuxiliaryEffectSloti( g_sx_al_data.al_fx_slot, AL_EFFECTSLOT_EFFECT, g_sx_al_data.al_fx);
SHOWALERROR


}


// simple file read, 16 bit raw
//if splitchannels is false, data0 contains mono or interlaced stereo, data1 is null
//if splitchannels is true, data0 contains left, data1 contains right
zsize sx_read_raw16(zchar* filename, zbool split_channels, zbyte** data0, zbyte** data1)
{
	zsize len;
	zsize lenread;
	FILE* f = fopen(filename, "rb");
	zbyte* data = NULL;
	
	
	if (f)
	{
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		data = ram_alloc(len, NULL);

		if (data)
		{
			
			lenread = fread(data, 1, len, f);
			if (lenread == len)
			{
				if (data0)
				{
					*data0 = data;
					return len;
				}
			}

			ram_free(data);
		}
		fclose(f);
	}
		

	return 0;
}
