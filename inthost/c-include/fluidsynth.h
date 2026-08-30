/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_TYPES_H
#define _FLUIDSYNTH_TYPES_H



#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup Types Types
 * @brief Type declarations
 *
 * @{
 */

typedef struct _fluid_hashtable_t fluid_settings_t;             /**< Configuration settings instance */
typedef struct _fluid_synth_t fluid_synth_t;                    /**< Synthesizer instance */
typedef struct _fluid_voice_t fluid_voice_t;                    /**< Synthesis voice instance */
typedef struct _fluid_sfloader_t fluid_sfloader_t;              /**< SoundFont loader plugin */
typedef struct _fluid_sfont_t fluid_sfont_t;                    /**< SoundFont */
typedef struct _fluid_preset_t fluid_preset_t;                  /**< SoundFont preset */
typedef struct _fluid_sample_t fluid_sample_t;                  /**< SoundFont sample */
typedef struct _fluid_mod_t fluid_mod_t;                        /**< SoundFont modulator */
typedef struct _fluid_audio_driver_t fluid_audio_driver_t;      /**< Audio driver instance */
typedef struct _fluid_file_renderer_t fluid_file_renderer_t;    /**< Audio file renderer instance */
typedef struct _fluid_player_t fluid_player_t;                  /**< MIDI player instance */
typedef struct _fluid_midi_event_t fluid_midi_event_t;          /**< MIDI event */
typedef struct _fluid_midi_driver_t fluid_midi_driver_t;        /**< MIDI driver instance */
typedef struct _fluid_midi_router_t fluid_midi_router_t;        /**< MIDI router instance */
typedef struct _fluid_midi_router_rule_t fluid_midi_router_rule_t;      /**< MIDI router rule */
typedef struct _fluid_hashtable_t fluid_cmd_hash_t;             /**< Command handler hash table */
typedef struct _fluid_shell_t fluid_shell_t;                    /**< Command shell */
typedef struct _fluid_server_t fluid_server_t;                  /**< TCP/IP shell server instance */
typedef struct _fluid_event_t fluid_event_t;                    /**< Sequencer event */
typedef struct _fluid_sequencer_t fluid_sequencer_t;            /**< Sequencer instance */
typedef struct _fluid_ramsfont_t fluid_ramsfont_t;              /**< RAM SoundFont */
typedef struct _fluid_rampreset_t fluid_rampreset_t;            /**< RAM SoundFont preset */
typedef struct _fluid_cmd_handler_t fluid_cmd_handler_t;        /**< Shell Command Handler */
typedef struct _fluid_ladspa_fx_t fluid_ladspa_fx_t;            /**< LADSPA effects instance */
typedef struct _fluid_file_callbacks_t fluid_file_callbacks_t;  /**< Callback struct to perform custom file loading of soundfonts */

typedef int fluid_istream_t;    /**< Input stream descriptor */
typedef int fluid_ostream_t;    /**< Output stream descriptor */

typedef short fluid_seq_id_t; /**< Unique client IDs used by the sequencer and #fluid_event_t, obtained by fluid_sequencer_register_client() and fluid_sequencer_register_fluidsynth() */

#if defined(_MSC_VER) && (_MSC_VER < 1800)
typedef __int64 fluid_long_long_t; // even on 32bit windows
#else
/**
 * A typedef for C99's type long long, which is at least 64-bit wide, as guaranteed by the C99.
 * @p __int64 will be used as replacement for VisualStudio 2010 and older.
 */
typedef long long fluid_long_long_t;
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_TYPES_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SETTINGS_H
#define _FLUIDSYNTH_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup settings Settings
 *
 * Functions for settings management
 *
 * To create a synthesizer object you will have to specify its
 * settings. These settings are stored in a fluid_settings_t object.
 * @code
 *     void
 *     my_synthesizer ()
 *     {
 *       fluid_settings_t *settings;
 *       fluid_synth_t *synth;
 *       fluid_audio_driver_t *adriver;
 *
 *       settings = new_fluid_settings ();
 *       fluid_settings_setstr(settings, "audio.driver", "alsa");
 *       // ... change settings ...
 *       synth = new_fluid_synth (settings);
 *       adriver = new_fluid_audio_driver (settings, synth);
 *       // ...
 *     }
 * @endcode
 * @sa @ref CreatingSettings
 *
 * @{
 */

/**
 * Hint FLUID_HINT_BOUNDED_BELOW indicates that the LowerBound field
 * of the FLUID_PortRangeHint should be considered meaningful. The
 * value in this field should be considered the (inclusive) lower
 * bound of the valid range. If FLUID_HINT_SAMPLE_RATE is also
 * specified then the value of LowerBound should be multiplied by the
 * sample rate.
 */
#define FLUID_HINT_BOUNDED_BELOW   0x1

/** Hint FLUID_HINT_BOUNDED_ABOVE indicates that the UpperBound field
   of the FLUID_PortRangeHint should be considered meaningful. The
   value in this field should be considered the (inclusive) upper
   bound of the valid range. If FLUID_HINT_SAMPLE_RATE is also
   specified then the value of UpperBound should be multiplied by the
   sample rate. */
#define FLUID_HINT_BOUNDED_ABOVE   0x2

/**
 * Hint FLUID_HINT_TOGGLED indicates that the data item should be
 * considered a Boolean toggle. Data less than or equal to zero should
 * be considered `off' or `false,' and data above zero should be
 * considered `on' or `true.' FLUID_HINT_TOGGLED may not be used in
 * conjunction with any other hint.
 */
#define FLUID_HINT_TOGGLED         0x4

#define FLUID_HINT_OPTIONLIST      0x02         /**< Setting is a list of string options */


/**
 * Settings type
 *
 * Each setting has a defined type: numeric (double), integer, string or a
 * set of values. The type of each setting can be retrieved using the
 * function fluid_settings_get_type()
 */
enum fluid_types_enum
{
    FLUID_NO_TYPE = -1, /**< Undefined type */
    FLUID_NUM_TYPE,     /**< Numeric (double) */
    FLUID_INT_TYPE,     /**< Integer */
    FLUID_STR_TYPE,     /**< String */
    FLUID_SET_TYPE      /**< Set of values */
};

/** @startlifecycle{Settings} */
FLUIDSYNTH_API fluid_settings_t *new_fluid_settings(void);
FLUIDSYNTH_API void delete_fluid_settings(fluid_settings_t *settings);
/** @endlifecycle */

FLUIDSYNTH_API
int fluid_settings_get_type(fluid_settings_t *settings, const char *name);

FLUIDSYNTH_API
int fluid_settings_get_hints(fluid_settings_t *settings, const char *name, int *val);

FLUIDSYNTH_API
int fluid_settings_is_realtime(fluid_settings_t *settings, const char *name);

FLUIDSYNTH_API
int fluid_settings_setstr(fluid_settings_t *settings, const char *name, const char *str);

FLUIDSYNTH_API
int fluid_settings_copystr(fluid_settings_t *settings, const char *name, char *str, int len);

FLUIDSYNTH_API
int fluid_settings_dupstr(fluid_settings_t *settings, const char *name, char **str);

FLUIDSYNTH_API
int fluid_settings_getstr_default(fluid_settings_t *settings, const char *name, char **def);

FLUIDSYNTH_API
int fluid_settings_str_equal(fluid_settings_t *settings, const char *name, const char *value);

FLUIDSYNTH_API
int fluid_settings_setnum(fluid_settings_t *settings, const char *name, double val);

FLUIDSYNTH_API
int fluid_settings_getnum(fluid_settings_t *settings, const char *name, double *val);

FLUIDSYNTH_API
int fluid_settings_getnum_default(fluid_settings_t *settings, const char *name, double *val);

FLUIDSYNTH_API
int fluid_settings_getnum_range(fluid_settings_t *settings, const char *name,
                                double *min, double *max);

FLUIDSYNTH_API
int fluid_settings_setint(fluid_settings_t *settings, const char *name, int val);

FLUIDSYNTH_API
int fluid_settings_getint(fluid_settings_t *settings, const char *name, int *val);

FLUIDSYNTH_API
int fluid_settings_getint_default(fluid_settings_t *settings, const char *name, int *val);

FLUIDSYNTH_API
int fluid_settings_getint_range(fluid_settings_t *settings, const char *name,
                                int *min, int *max);

/**
 * Callback function type used with fluid_settings_foreach_option()
 *
 * @param data User defined data pointer
 * @param name Setting name
 * @param option A string option for this setting (iterates through the list)
 */
typedef void (*fluid_settings_foreach_option_t)(void *data, const char *name, const char *option);

FLUIDSYNTH_API
void fluid_settings_foreach_option(fluid_settings_t *settings,
                                   const char *name, void *data,
                                   fluid_settings_foreach_option_t func);
FLUIDSYNTH_API
int fluid_settings_option_count(fluid_settings_t *settings, const char *name);
FLUIDSYNTH_API char *fluid_settings_option_concat(fluid_settings_t *settings,
        const char *name,
        const char *separator);

/**
 * Callback function type used with fluid_settings_foreach()
 *
 * @param data User defined data pointer
 * @param name Setting name
 * @param type Setting type (#fluid_types_enum)
 */
typedef void (*fluid_settings_foreach_t)(void *data, const char *name, int type);

FLUIDSYNTH_API
void fluid_settings_foreach(fluid_settings_t *settings, void *data,
                            fluid_settings_foreach_t func);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_SETTINGS_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SYNTH_H
#define _FLUIDSYNTH_SYNTH_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup synth Synthesizer
 *
 * SoundFont synthesizer
 *
 * You create a new synthesizer with new_fluid_synth() and you destroy
 * it with delete_fluid_synth(). Use the fluid_settings_t structure to specify
 * the synthesizer characteristics.
 *
 * You have to load a SoundFont in order to hear any sound. For that
 * you use the fluid_synth_sfload() function.
 *
 * You can use the audio driver functions to open
 * the audio device and create a background audio thread.
 *
 * The API for sending MIDI events is probably what you expect:
 * fluid_synth_noteon(), fluid_synth_noteoff(), ...
 *
 * @{
 */

/** @startlifecycle{Synthesizer} */
FLUIDSYNTH_API fluid_synth_t *new_fluid_synth(fluid_settings_t *settings);
FLUIDSYNTH_API void delete_fluid_synth(fluid_synth_t *synth);
/** @endlifecycle */

FLUIDSYNTH_API double fluid_synth_get_cpu_load(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API const char *fluid_synth_error(fluid_synth_t *synth);
/** @} */

/**
 * @defgroup midi_messages MIDI Channel Messages
 * @ingroup synth
 *
 * The MIDI channel message functions are mostly directly named after their
 * counterpart MIDI messages. They are a high-level interface to controlling
 * the synthesizer, playing notes and changing note and channel parameters.
 *
 * @{
 */
FLUIDSYNTH_API int fluid_synth_noteon(fluid_synth_t *synth, int chan, int key, int vel);
FLUIDSYNTH_API int fluid_synth_noteoff(fluid_synth_t *synth, int chan, int key);
FLUIDSYNTH_API int fluid_synth_cc(fluid_synth_t *synth, int chan, int ctrl, int val);
FLUIDSYNTH_API int fluid_synth_get_cc(fluid_synth_t *synth, int chan, int ctrl, int *pval);
FLUIDSYNTH_API int fluid_synth_sysex(fluid_synth_t *synth, const char *data, int len,
                                     char *response, int *response_len, int *handled, int dryrun);
FLUIDSYNTH_API int fluid_synth_pitch_bend(fluid_synth_t *synth, int chan, int val);
FLUIDSYNTH_API int fluid_synth_get_pitch_bend(fluid_synth_t *synth, int chan, int *ppitch_bend);
FLUIDSYNTH_API int fluid_synth_pitch_wheel_sens(fluid_synth_t *synth, int chan, int val);
FLUIDSYNTH_API int fluid_synth_get_pitch_wheel_sens(fluid_synth_t *synth, int chan, int *pval);
FLUIDSYNTH_API int fluid_synth_program_change(fluid_synth_t *synth, int chan, int program);
FLUIDSYNTH_API int fluid_synth_channel_pressure(fluid_synth_t *synth, int chan, int val);
FLUIDSYNTH_API int fluid_synth_key_pressure(fluid_synth_t *synth, int chan, int key, int val);
FLUIDSYNTH_API int fluid_synth_bank_select(fluid_synth_t *synth, int chan, int bank);
FLUIDSYNTH_API int fluid_synth_sfont_select(fluid_synth_t *synth, int chan, int sfont_id);
FLUIDSYNTH_API
int fluid_synth_program_select(fluid_synth_t *synth, int chan, int sfont_id,
                               int bank_num, int preset_num);
FLUIDSYNTH_API int
fluid_synth_program_select_by_sfont_name(fluid_synth_t *synth, int chan,
        const char *sfont_name, int bank_num,
        int preset_num);
FLUIDSYNTH_API
int fluid_synth_get_program(fluid_synth_t *synth, int chan, int *sfont_id,
                            int *bank_num, int *preset_num);
FLUIDSYNTH_API int fluid_synth_unset_program(fluid_synth_t *synth, int chan);
FLUIDSYNTH_API int fluid_synth_program_reset(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_system_reset(fluid_synth_t *synth);

FLUIDSYNTH_API int fluid_synth_all_notes_off(fluid_synth_t *synth, int chan);
FLUIDSYNTH_API int fluid_synth_all_sounds_off(fluid_synth_t *synth, int chan);

FLUIDSYNTH_API int fluid_synth_set_gen(fluid_synth_t *synth, int chan,
                                       int param, float value);
FLUIDSYNTH_API float fluid_synth_get_gen(fluid_synth_t *synth, int chan, int param);
/** @} MIDI Channel Messages */


/**
 * @defgroup voice_control Synthesis Voice Control
 * @ingroup synth
 *
 * Low-level access to synthesis voices.
 *
 * @{
 */
FLUIDSYNTH_API int fluid_synth_start(fluid_synth_t *synth, unsigned int id,
                                     fluid_preset_t *preset, int audio_chan,
                                     int midi_chan, int key, int vel);
FLUIDSYNTH_API int fluid_synth_stop(fluid_synth_t *synth, unsigned int id);

FLUIDSYNTH_API fluid_voice_t *fluid_synth_alloc_voice(fluid_synth_t *synth,
        fluid_sample_t *sample,
        int channum, int key, int vel);
FLUIDSYNTH_API void fluid_synth_start_voice(fluid_synth_t *synth, fluid_voice_t *voice);
FLUIDSYNTH_API void fluid_synth_get_voicelist(fluid_synth_t *synth,
        fluid_voice_t *buf[], int bufsize, int ID);
/** @} Voice Control */


/**
 * @defgroup soundfont_management SoundFont Management
 * @ingroup synth
 *
 * Functions to load and unload SoundFonts.
 *
 * @{
 */
FLUIDSYNTH_API
int fluid_synth_sfload(fluid_synth_t *synth, const char *filename, int reset_presets);
FLUIDSYNTH_API int fluid_synth_sfreload(fluid_synth_t *synth, int id);
FLUIDSYNTH_API int fluid_synth_sfunload(fluid_synth_t *synth, int id, int reset_presets);
FLUIDSYNTH_API int fluid_synth_add_sfont(fluid_synth_t *synth, fluid_sfont_t *sfont);
FLUIDSYNTH_API int fluid_synth_remove_sfont(fluid_synth_t *synth, fluid_sfont_t *sfont);
FLUIDSYNTH_API int fluid_synth_sfcount(fluid_synth_t *synth);
FLUIDSYNTH_API fluid_sfont_t *fluid_synth_get_sfont(fluid_synth_t *synth, unsigned int num);
FLUIDSYNTH_API fluid_sfont_t *fluid_synth_get_sfont_by_id(fluid_synth_t *synth, int id);
FLUIDSYNTH_API fluid_sfont_t *fluid_synth_get_sfont_by_name(fluid_synth_t *synth,
        const char *name);
FLUIDSYNTH_API int fluid_synth_set_bank_offset(fluid_synth_t *synth, int sfont_id, int offset);
FLUIDSYNTH_API int fluid_synth_get_bank_offset(fluid_synth_t *synth, int sfont_id);
/** @} Soundfont Management */


/**
 * @defgroup reverb_effect Effect - Reverb
 * @ingroup synth
 *
 * Functions for configuring the built-in reverb effect
 *
 * @{
 */
FLUID_DEPRECATED FLUIDSYNTH_API void fluid_synth_set_reverb_on(fluid_synth_t *synth, int on);
FLUIDSYNTH_API int fluid_synth_reverb_on(fluid_synth_t *synth, int fx_group, int on);

FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_reverb(fluid_synth_t *synth, double roomsize,
                                          double damping, double width, double level);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_reverb_roomsize(fluid_synth_t *synth, double roomsize);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_reverb_damp(fluid_synth_t *synth, double damping);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_reverb_width(fluid_synth_t *synth, double width);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_reverb_level(fluid_synth_t *synth, double level);

FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_reverb_roomsize(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_reverb_damp(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_reverb_level(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_reverb_width(fluid_synth_t *synth);

FLUIDSYNTH_API int fluid_synth_set_reverb_group_roomsize(fluid_synth_t *synth, int fx_group, double roomsize);
FLUIDSYNTH_API int fluid_synth_set_reverb_group_damp(fluid_synth_t *synth, int fx_group, double damping);
FLUIDSYNTH_API int fluid_synth_set_reverb_group_width(fluid_synth_t *synth, int fx_group, double width);
FLUIDSYNTH_API int fluid_synth_set_reverb_group_level(fluid_synth_t *synth, int fx_group, double level);

FLUIDSYNTH_API int fluid_synth_get_reverb_group_roomsize(fluid_synth_t *synth, int fx_group, double *roomsize);
FLUIDSYNTH_API int fluid_synth_get_reverb_group_damp(fluid_synth_t *synth, int fx_group, double *damping);
FLUIDSYNTH_API int fluid_synth_get_reverb_group_width(fluid_synth_t *synth, int fx_group, double *width);
FLUIDSYNTH_API int fluid_synth_get_reverb_group_level(fluid_synth_t *synth, int fx_group, double *level);
 /** @} Reverb */


/**
 * @defgroup chorus_effect Effect - Chorus
 * @ingroup synth
 *
 * Functions for configuring the built-in chorus effect
 *
 * @{
 */

/**
 * Chorus modulation waveform type.
 */
enum fluid_chorus_mod
{
    FLUID_CHORUS_MOD_SINE = 0,            /**< Sine wave chorus modulation */
    FLUID_CHORUS_MOD_TRIANGLE = 1         /**< Triangle wave chorus modulation */
};


FLUID_DEPRECATED FLUIDSYNTH_API void fluid_synth_set_chorus_on(fluid_synth_t *synth, int on);
FLUIDSYNTH_API int fluid_synth_chorus_on(fluid_synth_t *synth, int fx_group, int on);

FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_chorus(fluid_synth_t *synth, int nr, double level,
                                          double speed, double depth_ms, int type);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_chorus_nr(fluid_synth_t *synth, int nr);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_chorus_level(fluid_synth_t *synth, double level);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_chorus_speed(fluid_synth_t *synth, double speed);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_chorus_depth(fluid_synth_t *synth, double depth_ms);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_set_chorus_type(fluid_synth_t *synth, int type);

FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_get_chorus_nr(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_chorus_level(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_chorus_speed(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API double fluid_synth_get_chorus_depth(fluid_synth_t *synth);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_get_chorus_type(fluid_synth_t *synth); /* see fluid_chorus_mod */

FLUIDSYNTH_API int fluid_synth_set_chorus_group_nr(fluid_synth_t *synth, int fx_group, int nr);
FLUIDSYNTH_API int fluid_synth_set_chorus_group_level(fluid_synth_t *synth, int fx_group, double level);
FLUIDSYNTH_API int fluid_synth_set_chorus_group_speed(fluid_synth_t *synth, int fx_group, double speed);
FLUIDSYNTH_API int fluid_synth_set_chorus_group_depth(fluid_synth_t *synth, int fx_group, double depth_ms);
FLUIDSYNTH_API int fluid_synth_set_chorus_group_type(fluid_synth_t *synth, int fx_group, int type);

FLUIDSYNTH_API int fluid_synth_get_chorus_group_nr(fluid_synth_t *synth, int fx_group, int *nr);
FLUIDSYNTH_API int fluid_synth_get_chorus_group_level(fluid_synth_t *synth, int fx_group, double *level);
FLUIDSYNTH_API int fluid_synth_get_chorus_group_speed(fluid_synth_t *synth, int fx_group, double *speed);
FLUIDSYNTH_API int fluid_synth_get_chorus_group_depth(fluid_synth_t *synth, int fx_group, double *depth_ms);
FLUIDSYNTH_API int fluid_synth_get_chorus_group_type(fluid_synth_t *synth, int fx_group, int *type);
/** @} Chorus */

/**
 * @defgroup synthesis_params Synthesis Parameters
 * @ingroup synth
 *
 * Functions to control and query synthesis parameters like gain and
 * polyphony count.
 *
 * @{
 */
FLUIDSYNTH_API int fluid_synth_count_midi_channels(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_count_audio_channels(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_count_audio_groups(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_count_effects_channels(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_count_effects_groups(fluid_synth_t *synth);

FLUID_DEPRECATED FLUIDSYNTH_API void fluid_synth_set_sample_rate(fluid_synth_t *synth, float sample_rate);
FLUIDSYNTH_API void fluid_synth_set_gain(fluid_synth_t *synth, float gain);
FLUIDSYNTH_API float fluid_synth_get_gain(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_set_polyphony(fluid_synth_t *synth, int polyphony);
FLUIDSYNTH_API int fluid_synth_get_polyphony(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_get_active_voice_count(fluid_synth_t *synth);
FLUIDSYNTH_API int fluid_synth_get_internal_bufsize(fluid_synth_t *synth);

FLUIDSYNTH_API
int fluid_synth_set_interp_method(fluid_synth_t *synth, int chan, int interp_method);

/**
 * Synthesis interpolation method.
 */
enum fluid_interp
{
    FLUID_INTERP_NONE = 0,        /**< No interpolation: Fastest, but questionable audio quality */
    FLUID_INTERP_LINEAR = 1,      /**< Straight-line interpolation: A bit slower, reasonable audio quality */
    FLUID_INTERP_4THORDER = 4,    /**< Fourth-order interpolation, good quality, the default */
    FLUID_INTERP_7THORDER = 7,    /**< Seventh-order interpolation */

    FLUID_INTERP_DEFAULT = FLUID_INTERP_4THORDER, /**< Default interpolation method */
    FLUID_INTERP_HIGHEST = FLUID_INTERP_7THORDER, /**< Highest interpolation method */
};

/**
 * Enum used with fluid_synth_add_default_mod() to specify how to handle duplicate modulators.
 */
enum fluid_synth_add_mod
{
    FLUID_SYNTH_OVERWRITE,        /**< Overwrite any existing matching modulator */
    FLUID_SYNTH_ADD,              /**< Sum up modulator amounts */
};

FLUIDSYNTH_API int fluid_synth_add_default_mod(fluid_synth_t *synth, const fluid_mod_t *mod, int mode);
FLUIDSYNTH_API int fluid_synth_remove_default_mod(fluid_synth_t *synth, const fluid_mod_t *mod);
/** @} Synthesis Parameters */


/**
 * @defgroup tuning MIDI Tuning
 * @ingroup synth
 *
 * The functions in this section implement the MIDI Tuning Standard interface.
 *
 * @{
 */
FLUIDSYNTH_API
int fluid_synth_activate_key_tuning(fluid_synth_t *synth, int bank, int prog,
                                    const char *name, const double *pitch, int apply);
FLUIDSYNTH_API
int fluid_synth_activate_octave_tuning(fluid_synth_t *synth, int bank, int prog,
                                       const char *name, const double *pitch, int apply);
FLUIDSYNTH_API
int fluid_synth_tune_notes(fluid_synth_t *synth, int bank, int prog,
                           int len, const int *keys, const double *pitch, int apply);
FLUIDSYNTH_API
int fluid_synth_activate_tuning(fluid_synth_t *synth, int chan, int bank, int prog,
                                int apply);
FLUIDSYNTH_API
int fluid_synth_deactivate_tuning(fluid_synth_t *synth, int chan, int apply);
FLUIDSYNTH_API void fluid_synth_tuning_iteration_start(fluid_synth_t *synth);
FLUIDSYNTH_API
int fluid_synth_tuning_iteration_next(fluid_synth_t *synth, int *bank, int *prog);
FLUIDSYNTH_API int fluid_synth_tuning_dump(fluid_synth_t *synth, int bank, int prog,
        char *name, int len, double *pitch);
/** @} MIDI Tuning */


/**
 * @defgroup audio_rendering Audio Rendering
 * @ingroup synth
 *
 * The functions in this section can be used to render audio directly to
 * memory buffers. They are used internally by the \ref audio_driver and \ref file_renderer,
 * but can also be used manually for custom processing of the rendered audio.
 *
 * @note Please note that all following functions block during rendering. If your goal is to
 * render real-time audio, ensure that you call these functions from a high-priority
 * thread with little to no other duties other than calling the rendering functions.
 *
 * @warning
 * If a concurrently running thread calls any other sound affecting synth function
 * (e.g. fluid_synth_noteon(), fluid_synth_cc(), etc.) it is unspecified whether the event triggered by such a call
 * will be effective in the recently synthesized audio. While this is inaudible when only requesting small chunks from the
 * synth with every call (cf. fluid_synth_get_internal_bufsize()), it will become evident when requesting larger sample chunks:
 * With larger sample chunks it will get harder for the synth to react on those spontaneously occurring events in time
 * (like events received from a MIDI driver, or directly made synth API calls).
 * In those real-time scenarios, prefer requesting smaller
 * sample chunks from the synth with each call, to avoid poor quantization of your events in the synthesized audio.
 * This issue is not applicable when using the MIDI player or sequencer for event dispatching. Also
 * refer to the documentation of \setting{audio_period-size}.
 *
 * @{
 */
FLUIDSYNTH_API int fluid_synth_write_s16(fluid_synth_t *synth, int len,
        void *lout, int loff, int lincr,
        void *rout, int roff, int rincr);
FLUIDSYNTH_API int fluid_synth_write_float(fluid_synth_t *synth, int len,
        void *lout, int loff, int lincr,
        void *rout, int roff, int rincr);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_synth_nwrite_float(fluid_synth_t *synth, int len,
        float **left, float **right,
        float **fx_left, float **fx_right);
FLUIDSYNTH_API int fluid_synth_process(fluid_synth_t *synth, int len,
                                       int nfx, float *fx[],
                                       int nout, float *out[]);
/** @} Audio Rendering */


/**
 * @defgroup iir_filter Effect - IIR Filter
 * @ingroup synth
 *
 * Functions for configuring the built-in IIR filter effect
 *
 * @{
 */

/**
 * Specifies the type of filter to use for the custom IIR filter
 */
enum fluid_iir_filter_type
{
    FLUID_IIR_DISABLED = 0, /**< Custom IIR filter is not operating */
    FLUID_IIR_LOWPASS, /**< Custom IIR filter is operating as low-pass filter */
    FLUID_IIR_HIGHPASS, /**< Custom IIR filter is operating as high-pass filter */
    FLUID_IIR_LAST /**< @internal Value defines the count of filter types (#fluid_iir_filter_type) @warning This symbol is not part of the public API and ABI stability guarantee and may change at any time! */
};

/**
 * Specifies optional settings to use for the custom IIR filter. Can be bitwise ORed.
 */
enum fluid_iir_filter_flags
{
    FLUID_IIR_Q_LINEAR = 1 << 0, /**< The Soundfont spec requires the filter Q to be interpreted in dB. If this flag is set the filter Q is instead assumed to be in a linear range */
    FLUID_IIR_Q_ZERO_OFF = 1 << 1, /**< If this flag the filter is switched off if Q == 0 (prior to any transformation) */
    FLUID_IIR_NO_GAIN_AMP = 1 << 2 /**< The Soundfont spec requires to correct the gain of the filter depending on the filter's Q. If this flag is set the filter gain will not be corrected. */
};

FLUIDSYNTH_API int fluid_synth_set_custom_filter(fluid_synth_t *, int type, int flags);
/** @} IIR Filter */




/**
 * @defgroup channel_setup MIDI Channel Setup
 * @ingroup synth
 *
 * The functions in this section provide interfaces to change the channel type
 * and to configure basic channels, legato and portamento setups.
 *
 * @{
 */

/** @name Channel Type
 * @{
 */

/**
 * The midi channel type used by fluid_synth_set_channel_type()
 */
enum fluid_midi_channel_type
{
    CHANNEL_TYPE_MELODIC = 0, /**< Melodic midi channel */
    CHANNEL_TYPE_DRUM = 1 /**< Drum midi channel */
};

FLUIDSYNTH_API int fluid_synth_set_channel_type(fluid_synth_t *synth, int chan, int type);
/** @} Channel Type */


/** @name Basic Channel Mode
 * @{
 */

/**
 * Channel mode bits OR-ed together so that it matches with the midi spec: poly omnion (0), mono omnion (1), poly omnioff (2), mono omnioff (3)
 */
enum fluid_channel_mode_flags
{
    FLUID_CHANNEL_POLY_OFF = 0x01, /**< if flag is set, the basic channel is in mono on state, if not set poly is on */
    FLUID_CHANNEL_OMNI_OFF = 0x02, /**< if flag is set, the basic channel is in omni off state, if not set omni is on */
};

/**
 * Indicates the mode a basic channel is set to
 */
enum fluid_basic_channel_modes
{
    FLUID_CHANNEL_MODE_MASK = (FLUID_CHANNEL_OMNI_OFF | FLUID_CHANNEL_POLY_OFF), /**< Mask Poly and Omni bits of #fluid_channel_mode_flags, usually only used internally */
    FLUID_CHANNEL_MODE_OMNION_POLY = FLUID_CHANNEL_MODE_MASK & (~FLUID_CHANNEL_OMNI_OFF & ~FLUID_CHANNEL_POLY_OFF), /**< corresponds to MIDI mode 0 */
    FLUID_CHANNEL_MODE_OMNION_MONO = FLUID_CHANNEL_MODE_MASK & (~FLUID_CHANNEL_OMNI_OFF & FLUID_CHANNEL_POLY_OFF), /**< corresponds to MIDI mode 1 */
    FLUID_CHANNEL_MODE_OMNIOFF_POLY = FLUID_CHANNEL_MODE_MASK & (FLUID_CHANNEL_OMNI_OFF & ~FLUID_CHANNEL_POLY_OFF), /**< corresponds to MIDI mode 2 */
    FLUID_CHANNEL_MODE_OMNIOFF_MONO = FLUID_CHANNEL_MODE_MASK & (FLUID_CHANNEL_OMNI_OFF | FLUID_CHANNEL_POLY_OFF), /**< corresponds to MIDI mode 3 */
    FLUID_CHANNEL_MODE_LAST /**< @internal Value defines the count of basic channel modes (#fluid_basic_channel_modes) @warning This symbol is not part of the public API and ABI stability guarantee and may change at any time! */
};

FLUIDSYNTH_API int fluid_synth_reset_basic_channel(fluid_synth_t *synth, int chan);

FLUIDSYNTH_API int  fluid_synth_get_basic_channel(fluid_synth_t *synth, int chan,
        int *basic_chan_out,
        int *mode_chan_out,
        int *basic_val_out);
FLUIDSYNTH_API int fluid_synth_set_basic_channel(fluid_synth_t *synth, int chan, int mode, int val);

/** @} Basic Channel Mode */

/** @name Legato Mode
 * @{
 */

/**
 * Indicates the legato mode a channel is set to
 * n1,n2,n3,.. is a legato passage. n1 is the first note, and n2,n3,n4 are played legato with previous note. */
enum fluid_channel_legato_mode
{
    FLUID_CHANNEL_LEGATO_MODE_RETRIGGER, /**< Mode 0 - Release previous note, start a new note */
    FLUID_CHANNEL_LEGATO_MODE_MULTI_RETRIGGER, /**< Mode 1 - On contiguous notes retrigger in attack section using current value, shape attack using current dynamic and make use of previous voices if any */
    FLUID_CHANNEL_LEGATO_MODE_LAST /**< @internal Value defines the count of legato modes (#fluid_channel_legato_mode) @warning This symbol is not part of the public API and ABI stability guarantee and may change at any time! */
};

FLUIDSYNTH_API int fluid_synth_set_legato_mode(fluid_synth_t *synth, int chan, int legatomode);
FLUIDSYNTH_API int fluid_synth_get_legato_mode(fluid_synth_t *synth, int chan, int  *legatomode);
/** @} Legato Mode */

/** @name Portamento Mode
 * @{
 */

/**
 * Indicates the portamento mode a channel is set to
 */
enum fluid_channel_portamento_mode
{
    FLUID_CHANNEL_PORTAMENTO_MODE_EACH_NOTE, /**< Mode 0 - Portamento on each note (staccato or legato) */
    FLUID_CHANNEL_PORTAMENTO_MODE_LEGATO_ONLY, /**< Mode 1 - Portamento only on legato note */
    FLUID_CHANNEL_PORTAMENTO_MODE_STACCATO_ONLY, /**< Mode 2 - Portamento only on staccato note */
    FLUID_CHANNEL_PORTAMENTO_MODE_LAST /**< @internal Value defines the count of portamento modes
                                         @warning This symbol is not part of the public API and ABI
                                         stability guarantee and may change at any time! */
};

FLUIDSYNTH_API int fluid_synth_set_portamento_mode(fluid_synth_t *synth,
        int chan, int portamentomode);
FLUIDSYNTH_API int fluid_synth_get_portamento_mode(fluid_synth_t *synth,
        int chan, int   *portamentomode);
/** @} Portamento Mode */

/**@name Breath Mode
 * @{
 */

/**
 * Indicates the breath mode a channel is set to
 */
enum fluid_channel_breath_flags
{
    FLUID_CHANNEL_BREATH_POLY = 0x10,  /**< when channel is poly, this flag indicates that the default velocity to initial attenuation modulator is replaced by a breath to initial attenuation modulator */
    FLUID_CHANNEL_BREATH_MONO = 0x20,  /**< when channel is mono, this flag indicates that the default velocity to initial attenuation modulator is replaced by a breath modulator */
    FLUID_CHANNEL_BREATH_SYNC = 0x40,  /**< when channel is mono, this flag indicates that the breath controller(MSB)triggers noteon/noteoff on the running note */
};

FLUIDSYNTH_API int fluid_synth_set_breath_mode(fluid_synth_t *synth,
        int chan, int breathmode);
FLUIDSYNTH_API int fluid_synth_get_breath_mode(fluid_synth_t *synth,
        int chan, int  *breathmode);
/** @} Breath Mode */
/** @} MIDI Channel Setup */


/** @ingroup settings */
FLUIDSYNTH_API fluid_settings_t *fluid_synth_get_settings(fluid_synth_t *synth);

/** @ingroup soundfont_loader */
FLUIDSYNTH_API void fluid_synth_add_sfloader(fluid_synth_t *synth, fluid_sfloader_t *loader);

/** @ingroup soundfont_loader */
FLUIDSYNTH_API fluid_preset_t *fluid_synth_get_channel_preset(fluid_synth_t *synth, int chan);

/** @ingroup midi_input */
FLUIDSYNTH_API int fluid_synth_handle_midi_event(void *data, fluid_midi_event_t *event);

/** @ingroup soundfonts */
FLUIDSYNTH_API
int fluid_synth_pin_preset(fluid_synth_t *synth, int sfont_id, int bank_num, int preset_num);

/** @ingroup soundfonts */
FLUIDSYNTH_API
int fluid_synth_unpin_preset(fluid_synth_t *synth, int sfont_id, int bank_num, int preset_num);

/** @ingroup ladspa */
FLUIDSYNTH_API fluid_ladspa_fx_t *fluid_synth_get_ladspa_fx(fluid_synth_t *synth);

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_SYNTH_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SHELL_H
#define _FLUIDSYNTH_SHELL_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup command_interface Command Interface
 *
 * Control and configuration interface
 *
 * The command interface allows you to send textual commands to
 * the synthesizer, to parse a command file, or to read commands
 * from the stdin or other input streams (like a TCP socket).
 *
 * For a full list of available commands, type \c help in the
 * \ref command_shell or send the same command via a command handler.
 * Further documentation can be found at
 * https://github.com/FluidSynth/fluidsynth/wiki/UserManual#shell-commands
 *
 * @{
 */
FLUIDSYNTH_API fluid_istream_t fluid_get_stdin(void);
FLUIDSYNTH_API fluid_ostream_t fluid_get_stdout(void);
FLUIDSYNTH_API char *fluid_get_userconf(char *buf, int len);
FLUIDSYNTH_API char *fluid_get_sysconf(char *buf, int len);
/** @} */


/**
 * @defgroup command_handler Command Handler
 * @ingroup command_interface
 * @brief Handles text commands and reading of configuration files
 *
 * @{
 */

/** @startlifecycle{Command Handler} */
FLUIDSYNTH_API
fluid_cmd_handler_t *new_fluid_cmd_handler(fluid_synth_t *synth, fluid_midi_router_t *router);

FLUIDSYNTH_API
fluid_cmd_handler_t *new_fluid_cmd_handler2(fluid_settings_t *settings, fluid_synth_t *synth,
                                            fluid_midi_router_t *router, fluid_player_t *player);

FLUIDSYNTH_API
void delete_fluid_cmd_handler(fluid_cmd_handler_t *handler);
/** @endlifecycle */

FLUIDSYNTH_API
void fluid_cmd_handler_set_synth(fluid_cmd_handler_t *handler, fluid_synth_t *synth);

FLUIDSYNTH_API
int fluid_command(fluid_cmd_handler_t *handler, const char *cmd, fluid_ostream_t out);

FLUIDSYNTH_API
int fluid_source(fluid_cmd_handler_t *handler, const char *filename);
/** @} */


/**
 * @defgroup command_shell Command Shell
 * @ingroup command_interface
 *
 * Interactive shell to control and configure a synthesizer instance.
 *
 * If you need a platform independent way to get the standard input
 * and output streams, use fluid_get_stdin() and fluid_get_stdout().
 *
 * For a full list of available commands, type \c help in the shell.
 *
 * @{
 */

/** @startlifecycle{Command Shell} */
FLUIDSYNTH_API
fluid_shell_t *new_fluid_shell(fluid_settings_t *settings, fluid_cmd_handler_t *handler,
                               fluid_istream_t in, fluid_ostream_t out, int thread);

FLUIDSYNTH_API
void fluid_usershell(fluid_settings_t *settings, fluid_cmd_handler_t *handler);

FLUIDSYNTH_API void delete_fluid_shell(fluid_shell_t *shell);
/** @endlifecycle */

/** @} */


/**
 * @defgroup command_server Command Server
 * @ingroup command_interface
 *
 * TCP socket server for a command handler.
 *
 * The socket server will open the TCP port set by \ref settings_shell_port 
 * (default 9800) and starts a new thread and \ref command_handler for each 
 * incoming connection.
 *
 * @note The server is only available if libfluidsynth has been compiled
 * with network support (enable-network). Without network support, all related
 * functions will return FLUID_FAILED or NULL.
 *
 * @{
 */

/** @startlifecycle{Command Server} */
FLUIDSYNTH_API
fluid_server_t *new_fluid_server(fluid_settings_t *settings,
                                 fluid_synth_t *synth, fluid_midi_router_t *router);

FLUIDSYNTH_API
fluid_server_t *new_fluid_server2(fluid_settings_t *settings,
                                 fluid_synth_t *synth, fluid_midi_router_t *router,
                                 fluid_player_t *player);

FLUIDSYNTH_API void delete_fluid_server(fluid_server_t *server);

FLUIDSYNTH_API int fluid_server_join(fluid_server_t *server);
/** @endlifecycle */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_SHELL_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SFONT_H
#define _FLUIDSYNTH_SFONT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup soundfonts SoundFonts
 *
 * SoundFont related functions
 *
 * This part of the API contains functions, defines and types that are mostly
 * only used by internal or custom SoundFont loaders or client code that
 * modifies loaded presets, SoundFonts or voices directly.
 */

/**
 * @defgroup soundfont_loader SoundFont Loader
 * @ingroup soundfonts
 *
 * Create custom SoundFont loaders
 *
 * It is possible to add new SoundFont loaders to the
 * synthesizer. This API allows for virtual SoundFont files to be loaded
 * and synthesized, which may not actually be SoundFont files, as long as they
 * can be represented by the SoundFont synthesis model.
 *
 * To add a new SoundFont loader to the synthesizer, call
 * fluid_synth_add_sfloader() and pass a pointer to an
 * #fluid_sfloader_t instance created by new_fluid_sfloader().
 * On creation, you must specify a callback function \p load
 * that will be called for every file attempting to load it and
 * if successful returns a #fluid_sfont_t instance, or NULL if it fails.
 *
 * The #fluid_sfont_t structure contains a callback to obtain the
 * name of the SoundFont. It contains two functions to iterate
 * though the contained presets, and one function to obtain a
 * preset corresponding to a bank and preset number. This
 * function should return a #fluid_preset_t instance.
 *
 * The #fluid_preset_t instance contains some functions to obtain
 * information from the preset (name, bank, number). The most
 * important callback is the noteon function. The noteon function
 * is called by fluidsynth internally and
 * should call fluid_synth_alloc_voice() for every sample that has
 * to be played. fluid_synth_alloc_voice() expects a pointer to a
 * #fluid_sample_t instance and returns a pointer to the opaque
 * #fluid_voice_t structure. To set or increment the values of a
 * generator, use fluid_voice_gen_set() or fluid_voice_gen_incr(). When you are
 * finished initializing the voice call fluid_voice_start() to
 * start playing the synthesis voice.
 *
 * @{
 */

/**
 * Some notification enums for presets and samples.
 */
enum
{
    FLUID_PRESET_SELECTED,                /**< Preset selected notify */
    FLUID_PRESET_UNSELECTED,              /**< Preset unselected notify */
    FLUID_SAMPLE_DONE,                    /**< Sample no longer needed notify */
    FLUID_PRESET_PIN,                     /**< Request to pin preset samples to cache */
    FLUID_PRESET_UNPIN                    /**< Request to unpin preset samples from cache */
};

/**
 * Indicates the type of a sample used by the _fluid_sample_t::sampletype field.
 *
 * This enum corresponds to the \c SFSampleLink enum in the SoundFont spec.
 * One \c flag may be bit-wise OR-ed with one \c value.
 */
enum fluid_sample_type
{
    FLUID_SAMPLETYPE_MONO = 0x1, /**< Value used for mono samples */
    FLUID_SAMPLETYPE_RIGHT = 0x2, /**< Value used for right samples of a stereo pair */
    FLUID_SAMPLETYPE_LEFT = 0x4, /**< Value used for left samples of a stereo pair */
    FLUID_SAMPLETYPE_LINKED = 0x8, /**< Value used for linked sample, which is currently not supported */
    FLUID_SAMPLETYPE_OGG_VORBIS = 0x10, /**< Flag used for Ogg Vorbis compressed samples (non-standard compliant extension) as found in the program "sftools" developed by Werner Schweer from MuseScore @since 1.1.7 */
    FLUID_SAMPLETYPE_ROM = 0x8000 /**< Flag that indicates ROM samples, causing the sample to be ignored */
};


/**
 * Method to load an instrument file (does not actually need to be a real file name,
 * could be another type of string identifier that the \a loader understands).
 *
 * @param loader SoundFont loader
 * @param filename File name or other string identifier
 * @return The loaded instrument file (SoundFont) or NULL if an error occurred.
 */
typedef fluid_sfont_t *(*fluid_sfloader_load_t)(fluid_sfloader_t *loader, const char *filename);

/**
 * The free method should free the memory allocated for a fluid_sfloader_t instance in
 * addition to any private data.
 *
 * @param loader SoundFont loader
 *
 * Any custom user provided cleanup function must ultimately call
 * delete_fluid_sfloader() to ensure proper cleanup of the #fluid_sfloader_t struct. If no private data
 * needs to be freed, setting this to delete_fluid_sfloader() is sufficient.
 *
 */
typedef void (*fluid_sfloader_free_t)(fluid_sfloader_t *loader);


/** @startlifecycle{SoundFont Loader} */
FLUIDSYNTH_API fluid_sfloader_t *new_fluid_sfloader(fluid_sfloader_load_t load, fluid_sfloader_free_t free);
FLUIDSYNTH_API void delete_fluid_sfloader(fluid_sfloader_t *loader);

FLUIDSYNTH_API fluid_sfloader_t *new_fluid_defsfloader(fluid_settings_t *settings);
/** @endlifecycle */

/**
 * Opens the file or memory indicated by \c filename in binary read mode.
 *
 * @return returns a file handle on success, NULL otherwise
 *
 * \c filename matches the string provided during the fluid_synth_sfload() call.
 */
typedef void *(* fluid_sfloader_callback_open_t)(const char *filename);

/**
 * Reads \c count bytes to the specified buffer \c buf.
 *
 * @return returns #FLUID_OK if exactly \c count bytes were successfully read, else returns #FLUID_FAILED and leaves \a buf unmodified.
 */
typedef int (* fluid_sfloader_callback_read_t)(void *buf, fluid_long_long_t count, void *handle);

/**
 * Same purpose and behaviour as fseek.
 *
 * @param origin either \c SEEK_SET, \c SEEK_CUR or \c SEEK_END
 * @return returns #FLUID_OK if the seek was successfully performed while not seeking beyond a buffer or file, #FLUID_FAILED otherwise
 */
typedef int (* fluid_sfloader_callback_seek_t)(void *handle, fluid_long_long_t offset, int origin);

/**
 * Closes the handle returned by #fluid_sfloader_callback_open_t and frees used resources.
 *
 * @return returns #FLUID_OK on success, #FLUID_FAILED on error
 */
typedef int (* fluid_sfloader_callback_close_t)(void *handle);

/** @return returns current file offset or #FLUID_FAILED on error */
typedef fluid_long_long_t (* fluid_sfloader_callback_tell_t)(void *handle);


FLUIDSYNTH_API int fluid_sfloader_set_callbacks(fluid_sfloader_t *loader,
        fluid_sfloader_callback_open_t open,
        fluid_sfloader_callback_read_t read,
        fluid_sfloader_callback_seek_t seek,
        fluid_sfloader_callback_tell_t tell,
        fluid_sfloader_callback_close_t close);

FLUIDSYNTH_API int fluid_sfloader_set_data(fluid_sfloader_t *loader, void *data);
FLUIDSYNTH_API void *fluid_sfloader_get_data(fluid_sfloader_t *loader);



/**
 * Method to return the name of a virtual SoundFont.
 *
 * @param sfont Virtual SoundFont
 * @return The name of the virtual SoundFont.
 */
typedef const char *(*fluid_sfont_get_name_t)(fluid_sfont_t *sfont);

/**
 * Get a virtual SoundFont preset by bank and program numbers.
 *
 * @param sfont Virtual SoundFont
 * @param bank MIDI bank number (0-16383)
 * @param prenum MIDI preset number (0-127)
 * @return Should return an allocated virtual preset or NULL if it could not
 *   be found.
 */
typedef fluid_preset_t *(*fluid_sfont_get_preset_t)(fluid_sfont_t *sfont, int bank, int prenum);

/**
 * Start virtual SoundFont preset iteration method.
 *
 * @param sfont Virtual SoundFont
 *
 * Starts/re-starts virtual preset iteration in a SoundFont.
 */
typedef void (*fluid_sfont_iteration_start_t)(fluid_sfont_t *sfont);

/**
 * Virtual SoundFont preset iteration function.
 *
 * @param sfont Virtual SoundFont
 * @return NULL when no more presets are available, otherwise the a pointer to the current preset
 *
 * Returns preset information to the caller. The returned buffer is only valid until a subsequent
 * call to this function.
 */
typedef fluid_preset_t *(*fluid_sfont_iteration_next_t)(fluid_sfont_t *sfont);

/**
 * Method to free a virtual SoundFont bank.
 *
 * @param sfont Virtual SoundFont to free.
 * @return Should return 0 when it was able to free all resources or non-zero
 *   if some of the samples could not be freed because they are still in use,
 *   in which case the free will be tried again later, until success.
 *
 * Any custom user provided cleanup function must ultimately call
 * delete_fluid_sfont() to ensure proper cleanup of the #fluid_sfont_t struct. If no private data
 * needs to be freed, setting this to delete_fluid_sfont() is sufficient.
 */
typedef int (*fluid_sfont_free_t)(fluid_sfont_t *sfont);


/** @startlifecycle{SoundFont} */
FLUIDSYNTH_API fluid_sfont_t *new_fluid_sfont(fluid_sfont_get_name_t get_name,
        fluid_sfont_get_preset_t get_preset,
        fluid_sfont_iteration_start_t iter_start,
        fluid_sfont_iteration_next_t iter_next,
        fluid_sfont_free_t free);

FLUIDSYNTH_API int delete_fluid_sfont(fluid_sfont_t *sfont);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_sfont_set_data(fluid_sfont_t *sfont, void *data);
FLUIDSYNTH_API void *fluid_sfont_get_data(fluid_sfont_t *sfont);

FLUIDSYNTH_API int fluid_sfont_get_id(fluid_sfont_t *sfont);
FLUIDSYNTH_API const char *fluid_sfont_get_name(fluid_sfont_t *sfont);
FLUIDSYNTH_API fluid_preset_t *fluid_sfont_get_preset(fluid_sfont_t *sfont, int bank, int prenum);
FLUIDSYNTH_API void fluid_sfont_iteration_start(fluid_sfont_t *sfont);
FLUIDSYNTH_API fluid_preset_t *fluid_sfont_iteration_next(fluid_sfont_t *sfont);

/**
 * Method to get a virtual SoundFont preset name.
 *
 * @param preset Virtual SoundFont preset
 * @return Should return the name of the preset.  The returned string must be
 *   valid for the duration of the virtual preset (or the duration of the
 *   SoundFont, in the case of preset iteration).
 */
typedef const char *(*fluid_preset_get_name_t)(fluid_preset_t *preset);

/**
 * Method to get a virtual SoundFont preset MIDI bank number.
 *
 * @param preset Virtual SoundFont preset
 * @param return The bank number of the preset
 */
typedef int (*fluid_preset_get_banknum_t)(fluid_preset_t *preset);

/**
 * Method to get a virtual SoundFont preset MIDI program number.
 *
 * @param preset Virtual SoundFont preset
 * @param return The program number of the preset
 */
typedef int (*fluid_preset_get_num_t)(fluid_preset_t *preset);

/**
 * Method to handle a noteon event (synthesize the instrument).
 *
 * @param preset Virtual SoundFont preset
 * @param synth Synthesizer instance
 * @param chan MIDI channel number of the note on event
 * @param key MIDI note number (0-127)
 * @param vel MIDI velocity (0-127)
 * @return #FLUID_OK on success (0) or #FLUID_FAILED (-1) otherwise
 *
 * This method may be called from within synthesis context and therefore
 * should be as efficient as possible and not perform any operations considered
 * bad for realtime audio output (memory allocations and other OS calls).
 *
 * Call fluid_synth_alloc_voice() for every sample that has
 * to be played. fluid_synth_alloc_voice() expects a pointer to a
 * #fluid_sample_t structure and returns a pointer to the opaque
 * #fluid_voice_t structure. To set or increment the values of a
 * generator, use fluid_voice_gen_set() or fluid_voice_gen_incr(). When you are
 * finished initializing the voice call fluid_voice_start() to
 * start playing the synthesis voice.  Starting with FluidSynth 1.1.0 all voices
 * created will be started at the same time.
 */
typedef int (*fluid_preset_noteon_t)(fluid_preset_t *preset, fluid_synth_t *synth, int chan, int key, int vel);

/**
 * Method to free a virtual SoundFont preset.
 *
 * @param preset Virtual SoundFont preset
 * @return Should return 0
 *
 * Any custom user provided cleanup function must ultimately call
 * delete_fluid_preset() to ensure proper cleanup of the #fluid_preset_t struct. If no private data
 * needs to be freed, setting this to delete_fluid_preset() is sufficient.
 */
typedef void (*fluid_preset_free_t)(fluid_preset_t *preset);

/** @startlifecycle{Preset} */
FLUIDSYNTH_API fluid_preset_t *new_fluid_preset(fluid_sfont_t *parent_sfont,
        fluid_preset_get_name_t get_name,
        fluid_preset_get_banknum_t get_bank,
        fluid_preset_get_num_t get_num,
        fluid_preset_noteon_t noteon,
        fluid_preset_free_t free);
FLUIDSYNTH_API void delete_fluid_preset(fluid_preset_t *preset);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_preset_set_data(fluid_preset_t *preset, void *data);
FLUIDSYNTH_API void *fluid_preset_get_data(fluid_preset_t *preset);

FLUIDSYNTH_API const char *fluid_preset_get_name(fluid_preset_t *preset);
FLUIDSYNTH_API int fluid_preset_get_banknum(fluid_preset_t *preset);
FLUIDSYNTH_API int fluid_preset_get_num(fluid_preset_t *preset);
FLUIDSYNTH_API fluid_sfont_t *fluid_preset_get_sfont(fluid_preset_t *preset);

/** @startlifecycle{Sample} */
FLUIDSYNTH_API fluid_sample_t *new_fluid_sample(void);
FLUIDSYNTH_API void delete_fluid_sample(fluid_sample_t *sample);
/** @endlifecycle */

FLUIDSYNTH_API size_t fluid_sample_sizeof(void);

FLUIDSYNTH_API int fluid_sample_set_name(fluid_sample_t *sample, const char *name);
FLUIDSYNTH_API int fluid_sample_set_sound_data(fluid_sample_t *sample,
        short *data,
        char *data24,
        unsigned int nbframes,
        unsigned int sample_rate,
        short copy_data);

FLUIDSYNTH_API int fluid_sample_set_loop(fluid_sample_t *sample, unsigned int loop_start, unsigned int loop_end);
FLUIDSYNTH_API int fluid_sample_set_pitch(fluid_sample_t *sample, int root_key, int fine_tune);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_SFONT_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_AUDIO_H
#define _FLUIDSYNTH_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup audio_output Audio Output
 *
 * Functions for managing audio drivers and file renderers.
 *
 * The file renderer is used for fast rendering of MIDI files to
 * audio files. The audio drivers are a high-level interface to
 * connect the synthesizer with external audio sinks or to render
 * real-time audio to files.
 */

/**
 * @defgroup audio_driver Audio Driver
 * @ingroup audio_output
 *
 * Functions for managing audio drivers.
 *
 * Defines functions for creating audio driver output.  Use
 * new_fluid_audio_driver() to create a new audio driver for a given synth
 * and configuration settings.
 *
 * The function new_fluid_audio_driver2() can be
 * used if custom audio processing is desired before the audio is sent to the
 * audio driver (although it is not as efficient).
 *
 * @sa @ref CreatingAudioDriver
 *
 * @{
 */

/**
 * Callback function type used with new_fluid_audio_driver2() to allow for
 * custom user audio processing before the audio is sent to the driver.
 *
 * @param data The user data parameter as passed to new_fluid_audio_driver2().
 * @param len Count of audio frames to synthesize.
 * @param nfx Count of arrays in \c fx.
 * @param fx Array of buffers to store effects audio to. Buffers may alias with buffers of \c out.
 * @param nout Count of arrays in \c out.
 * @param out Array of buffers to store (dry) audio to. Buffers may alias with buffers of \c fx.
 * @return Should return #FLUID_OK on success, #FLUID_FAILED if an error occurred.
 *
 * This function is responsible for rendering audio to the buffers.
 * The buffers passed to this function are allocated and owned by the respective
 * audio driver and are only valid during that specific call (do not cache them).
 * The buffers have already been zeroed-out.
 * For further details please refer to fluid_synth_process().
 *
 * @parblock
 * @note Whereas fluid_synth_process() allows aliasing buffers, there is the guarantee that @p out
 * and @p fx buffers provided by fluidsynth's audio drivers never alias. This prevents downstream
 * applications from e.g. applying a custom effect accidentally to the same buffer multiple times.
 * @endparblock
 *
 * @parblock
 * @note Also note that the Jack driver is currently the only driver that has dedicated @p fx buffers
 * (but only if \setting{audio_jack_multi} is true). All other drivers do not provide @p fx buffers.
 * In this case, users are encouraged to mix the effects into the provided dry buffers when calling
 * fluid_synth_process().
 * @code{.cpp}
int myCallback(void *, int len, int nfx, float *fx[], int nout, float *out[])
{
    int ret;
    if(nfx == 0)
    {
        float *fxb[4] = {out[0], out[1], out[0], out[1]};
        ret = fluid_synth_process(synth, len, sizeof(fxb) / sizeof(fxb[0]), fxb, nout, out);
    }
    else
    {
        ret = fluid_synth_process(synth, len, nfx, fx, nout, out);
    }
    // ... client-code ...
    return ret;
}
 * @endcode
 * For other possible use-cases refer to \ref fluidsynth_process.c .
 * @endparblock
 */
typedef int (*fluid_audio_func_t)(void *data, int len,
                                  int nfx, float *fx[],
                                  int nout, float *out[]);

/** @startlifecycle{Audio Driver} */
FLUIDSYNTH_API fluid_audio_driver_t *new_fluid_audio_driver(fluid_settings_t *settings,
        fluid_synth_t *synth);

FLUIDSYNTH_API fluid_audio_driver_t *new_fluid_audio_driver2(fluid_settings_t *settings,
        fluid_audio_func_t func,
        void *data);

FLUIDSYNTH_API void delete_fluid_audio_driver(fluid_audio_driver_t *driver);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_audio_driver_register(const char **adrivers);
/** @} */

/**
 * @defgroup file_renderer File Renderer
 * @ingroup audio_output
 *
 * Functions for managing file renderers and triggering the rendering.
 *
 * The file renderer is only used to render a MIDI file to audio as fast
 * as possible. Please see \ref FileRenderer for a full example.
 *
 * If you are looking for a way to write audio generated
 * from real-time events (for example from an external sequencer or a MIDI controller) to a file,
 * please have a look at the \c file \ref audio_driver instead.
 *
 *
 * @{
 */

/** @startlifecycle{File Renderer} */
FLUIDSYNTH_API fluid_file_renderer_t *new_fluid_file_renderer(fluid_synth_t *synth);
FLUIDSYNTH_API void delete_fluid_file_renderer(fluid_file_renderer_t *dev);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_file_renderer_process_block(fluid_file_renderer_t *dev);
FLUIDSYNTH_API int fluid_file_set_encoding_quality(fluid_file_renderer_t *dev, double q);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_AUDIO_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_EVENT_H
#define _FLUIDSYNTH_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup sequencer_events Sequencer Events
 * @ingroup sequencer
 *
 * Create, modify, query and destroy sequencer events.
 *
 * @{
 */

/**
 * Sequencer event type enumeration.
 */
enum fluid_seq_event_type
{
    FLUID_SEQ_NOTE = 0,		/**< Note event with duration */
    FLUID_SEQ_NOTEON,		/**< Note on event */
    FLUID_SEQ_NOTEOFF,		/**< Note off event */
    FLUID_SEQ_ALLSOUNDSOFF,	/**< All sounds off event */
    FLUID_SEQ_ALLNOTESOFF,	/**< All notes off event */
    FLUID_SEQ_BANKSELECT,		/**< Bank select message */
    FLUID_SEQ_PROGRAMCHANGE,	/**< Program change message */
    FLUID_SEQ_PROGRAMSELECT,	/**< Program select message */
    FLUID_SEQ_PITCHBEND,		/**< Pitch bend message */
    FLUID_SEQ_PITCHWHEELSENS,	/**< Pitch wheel sensitivity set message @since 1.1.0 was misspelled previously */
    FLUID_SEQ_MODULATION,		/**< Modulation controller event */
    FLUID_SEQ_SUSTAIN,		/**< Sustain controller event */
    FLUID_SEQ_CONTROLCHANGE,	/**< MIDI control change event */
    FLUID_SEQ_PAN,		/**< Stereo pan set event */
    FLUID_SEQ_VOLUME,		/**< Volume set event */
    FLUID_SEQ_REVERBSEND,		/**< Reverb send set event */
    FLUID_SEQ_CHORUSSEND,		/**< Chorus send set event */
    FLUID_SEQ_TIMER,		/**< Timer event (useful for giving a callback at a certain time) */
    FLUID_SEQ_CHANNELPRESSURE,    /**< Channel aftertouch event @since 1.1.0 */
    FLUID_SEQ_KEYPRESSURE,        /**< Polyphonic aftertouch event @since 2.0.0 */
    FLUID_SEQ_SYSTEMRESET,        /**< System reset event @since 1.1.0 */
    FLUID_SEQ_UNREGISTERING,      /**< Called when a sequencer client is being unregistered. @since 1.1.0 */
    FLUID_SEQ_SCALE,              /**< Sets a new time scale for the sequencer @since 2.2.0 */
    FLUID_SEQ_LASTEVENT		/**< @internal Defines the count of events enums @warning This symbol 
                              is not part of the public API and ABI stability guarantee and 
                              may change at any time! */
};

/* Event alloc/free */
/** @startlifecycle{Sequencer Event} */
FLUIDSYNTH_API fluid_event_t *new_fluid_event(void);
FLUIDSYNTH_API void delete_fluid_event(fluid_event_t *evt);
/** @endlifecycle */

/* Initializing events */
FLUIDSYNTH_API void fluid_event_set_source(fluid_event_t *evt, fluid_seq_id_t src);
FLUIDSYNTH_API void fluid_event_set_dest(fluid_event_t *evt, fluid_seq_id_t dest);

/* Timer events */
FLUIDSYNTH_API void fluid_event_timer(fluid_event_t *evt, void *data);

/* Note events */
FLUIDSYNTH_API void fluid_event_note(fluid_event_t *evt, int channel,
                                     short key, short vel,
                                     unsigned int duration);

FLUIDSYNTH_API void fluid_event_noteon(fluid_event_t *evt, int channel, short key, short vel);
FLUIDSYNTH_API void fluid_event_noteoff(fluid_event_t *evt, int channel, short key);
FLUIDSYNTH_API void fluid_event_all_sounds_off(fluid_event_t *evt, int channel);
FLUIDSYNTH_API void fluid_event_all_notes_off(fluid_event_t *evt, int channel);

/* Instrument selection */
FLUIDSYNTH_API void fluid_event_bank_select(fluid_event_t *evt, int channel, short bank_num);
FLUIDSYNTH_API void fluid_event_program_change(fluid_event_t *evt, int channel, int preset_num);
FLUIDSYNTH_API void fluid_event_program_select(fluid_event_t *evt, int channel, unsigned int sfont_id, short bank_num, short preset_num);

/* Real-time generic instrument controllers */
FLUIDSYNTH_API
void fluid_event_control_change(fluid_event_t *evt, int channel, short control, int val);

/* Real-time instrument controllers shortcuts */
FLUIDSYNTH_API void fluid_event_pitch_bend(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_pitch_wheelsens(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_modulation(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_sustain(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_pan(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_volume(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_reverb_send(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_chorus_send(fluid_event_t *evt, int channel, int val);

FLUIDSYNTH_API void fluid_event_key_pressure(fluid_event_t *evt, int channel, short key, int val);
FLUIDSYNTH_API void fluid_event_channel_pressure(fluid_event_t *evt, int channel, int val);
FLUIDSYNTH_API void fluid_event_system_reset(fluid_event_t *evt);

/* Only when unregistering clients */
FLUIDSYNTH_API void fluid_event_unregistering(fluid_event_t *evt);

FLUIDSYNTH_API void fluid_event_scale(fluid_event_t *evt, double new_scale);
FLUIDSYNTH_API int fluid_event_from_midi_event(fluid_event_t *, const fluid_midi_event_t *);

/* Accessing event data */
FLUIDSYNTH_API int fluid_event_get_type(fluid_event_t *evt);
FLUIDSYNTH_API fluid_seq_id_t fluid_event_get_source(fluid_event_t *evt);
FLUIDSYNTH_API fluid_seq_id_t fluid_event_get_dest(fluid_event_t *evt);
FLUIDSYNTH_API int fluid_event_get_channel(fluid_event_t *evt);
FLUIDSYNTH_API short fluid_event_get_key(fluid_event_t *evt);
FLUIDSYNTH_API short fluid_event_get_velocity(fluid_event_t *evt);
FLUIDSYNTH_API short fluid_event_get_control(fluid_event_t *evt);
FLUIDSYNTH_API int fluid_event_get_value(fluid_event_t *evt);
FLUIDSYNTH_API int fluid_event_get_program(fluid_event_t *evt);
FLUIDSYNTH_API void *fluid_event_get_data(fluid_event_t *evt);
FLUIDSYNTH_API unsigned int fluid_event_get_duration(fluid_event_t *evt);
FLUIDSYNTH_API short fluid_event_get_bank(fluid_event_t *evt);
FLUIDSYNTH_API int fluid_event_get_pitch(fluid_event_t *evt);
FLUIDSYNTH_API double fluid_event_get_scale(fluid_event_t *evt);
FLUIDSYNTH_API unsigned int fluid_event_get_sfont_id(fluid_event_t *evt);
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_EVENT_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_MIDI_H
#define _FLUIDSYNTH_MIDI_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup midi_input MIDI Input
 *
 * MIDI Input Subsystem
 *
 * There are multiple ways to send MIDI events to the synthesizer. They can come
 * from MIDI files, from external MIDI sequencers or raw MIDI event sources,
 * can be modified via MIDI routers and also generated manually.
 *
 * The interface connecting all sources and sinks of MIDI events in libfluidsynth
 * is \ref handle_midi_event_func_t.
 *
 * @{
 */

/**
 * Generic callback function for MIDI event handler.
 *
 * @param data User defined data pointer
 * @param event The MIDI event
 * @return Should return #FLUID_OK on success, #FLUID_FAILED otherwise
 *
 * This callback is used to pass MIDI events
 * - from \ref midi_player, \ref midi_router or \ref midi_driver
 * - to  \ref midi_router via fluid_midi_router_handle_midi_event()
 * - or to \ref synth via fluid_synth_handle_midi_event().
 *
 * Additionally, there is a translation layer to pass MIDI events to
 * a \ref sequencer via fluid_sequencer_add_midi_event_to_buffer().
 */
typedef int (*handle_midi_event_func_t)(void *data, fluid_midi_event_t *event);

/**
 * Generic callback function fired once by MIDI tick change.
 *
 * @param data User defined data pointer
 * @param tick The current (zero-based) tick, which triggered the callback
 * @return Should return #FLUID_OK on success, #FLUID_FAILED otherwise
 *
 * This callback is fired at a constant rate depending on the current BPM and PPQ.
 * e.g. for PPQ = 192 and BPM = 140 the callback is fired 192 * 140 times per minute (448/sec).
 *
 * It can be used to sync external elements with the beat,
 * or stop / loop the song on a given tick.
 * Ticks being BPM-dependent, you can manipulate values such as bars or beats,
 * without having to care about BPM.
 *
 * For example, this callback loops the song whenever it reaches the 5th bar :
 *
 * @code{.cpp}
int handle_tick(void *data, int tick)
{
    fluid_player_t *player = (fluid_player_t *)data;
    int ppq = 192; // From MIDI header
    int beatsPerBar = 4; // From the song's time signature
    int loopBar = 5;
    int loopTick = (loopBar - 1) * ppq * beatsPerBar;

    if (tick == loopTick)
    {
        return fluid_player_seek(player, 0);
    }

    return FLUID_OK;
}
 * @endcode
 */
typedef int (*handle_midi_tick_func_t)(void *data, int tick);
/** @} */

/**
 * @defgroup midi_events MIDI Events
 * @ingroup midi_input
 *
 * Functions to create, modify, query and delete MIDI events.
 *
 * These functions are intended to be used in MIDI routers and other filtering
 * and processing functions in the MIDI event path. If you want to simply
 * send MIDI messages to the synthesizer, you can use the more convenient
 * \ref midi_messages interface.
 *
 * @{
 */
/** @startlifecycle{MIDI Event} */
FLUIDSYNTH_API fluid_midi_event_t *new_fluid_midi_event(void);
FLUIDSYNTH_API void delete_fluid_midi_event(fluid_midi_event_t *event);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_midi_event_set_type(fluid_midi_event_t *evt, int type);
FLUIDSYNTH_API int fluid_midi_event_get_type(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_channel(fluid_midi_event_t *evt, int chan);
FLUIDSYNTH_API int fluid_midi_event_get_channel(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_get_key(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_key(fluid_midi_event_t *evt, int key);
FLUIDSYNTH_API int fluid_midi_event_get_velocity(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_velocity(fluid_midi_event_t *evt, int vel);
FLUIDSYNTH_API int fluid_midi_event_get_control(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_control(fluid_midi_event_t *evt, int ctrl);
FLUIDSYNTH_API int fluid_midi_event_get_value(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_value(fluid_midi_event_t *evt, int val);
FLUIDSYNTH_API int fluid_midi_event_get_program(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_program(fluid_midi_event_t *evt, int val);
FLUIDSYNTH_API int fluid_midi_event_get_pitch(const fluid_midi_event_t *evt);
FLUIDSYNTH_API int fluid_midi_event_set_pitch(fluid_midi_event_t *evt, int val);
FLUIDSYNTH_API int fluid_midi_event_set_sysex(fluid_midi_event_t *evt, void *data,
        int size, int dynamic);
FLUIDSYNTH_API int fluid_midi_event_set_text(fluid_midi_event_t *evt,
        void *data, int size, int dynamic);
FLUIDSYNTH_API int fluid_midi_event_get_text(fluid_midi_event_t *evt,
        void **data, int *size);
FLUIDSYNTH_API int fluid_midi_event_set_lyrics(fluid_midi_event_t *evt,
        void *data, int size, int dynamic);
FLUIDSYNTH_API int fluid_midi_event_get_lyrics(fluid_midi_event_t *evt,
        void **data, int *size);
/** @} */

/**
 * @defgroup midi_router MIDI Router
 * @ingroup midi_input
 *
 * Rule based transformation and filtering of MIDI events.
 *
 * @{
 */

/**
 * MIDI router rule type.
 *
 * @since 1.1.0
 */
typedef enum
{
    FLUID_MIDI_ROUTER_RULE_NOTE,                  /**< MIDI note rule */
    FLUID_MIDI_ROUTER_RULE_CC,                    /**< MIDI controller rule */
    FLUID_MIDI_ROUTER_RULE_PROG_CHANGE,           /**< MIDI program change rule */
    FLUID_MIDI_ROUTER_RULE_PITCH_BEND,            /**< MIDI pitch bend rule */
    FLUID_MIDI_ROUTER_RULE_CHANNEL_PRESSURE,      /**< MIDI channel pressure rule */
    FLUID_MIDI_ROUTER_RULE_KEY_PRESSURE,          /**< MIDI key pressure rule */
    FLUID_MIDI_ROUTER_RULE_COUNT                  /**< @internal Total count of rule types. This symbol
                                                    is not part of the public API and ABI stability
                                                    guarantee and may change at any time!*/
} fluid_midi_router_rule_type;


/** @startlifecycle{MIDI Router} */
FLUIDSYNTH_API fluid_midi_router_t *new_fluid_midi_router(fluid_settings_t *settings,
        handle_midi_event_func_t handler,
        void *event_handler_data);
FLUIDSYNTH_API void delete_fluid_midi_router(fluid_midi_router_t *handler);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_midi_router_set_default_rules(fluid_midi_router_t *router);
FLUIDSYNTH_API int fluid_midi_router_clear_rules(fluid_midi_router_t *router);
FLUIDSYNTH_API int fluid_midi_router_add_rule(fluid_midi_router_t *router,
        fluid_midi_router_rule_t *rule, int type);


/** @startlifecycle{MIDI Router Rule} */
FLUIDSYNTH_API fluid_midi_router_rule_t *new_fluid_midi_router_rule(void);
FLUIDSYNTH_API void delete_fluid_midi_router_rule(fluid_midi_router_rule_t *rule);
/** @endlifecycle */

FLUIDSYNTH_API void fluid_midi_router_rule_set_chan(fluid_midi_router_rule_t *rule,
        int min, int max, float mul, int add);
FLUIDSYNTH_API void fluid_midi_router_rule_set_param1(fluid_midi_router_rule_t *rule,
        int min, int max, float mul, int add);
FLUIDSYNTH_API void fluid_midi_router_rule_set_param2(fluid_midi_router_rule_t *rule,
        int min, int max, float mul, int add);
FLUIDSYNTH_API int fluid_midi_router_handle_midi_event(void *data, fluid_midi_event_t *event);
FLUIDSYNTH_API int fluid_midi_dump_prerouter(void *data, fluid_midi_event_t *event);
FLUIDSYNTH_API int fluid_midi_dump_postrouter(void *data, fluid_midi_event_t *event);
/** @} */

/**
 * @defgroup midi_driver MIDI Driver
 * @ingroup midi_input
 *
 * Functions for managing MIDI drivers.
 *
 * The available MIDI drivers depend on your platform. See \ref settings_midi for all
 * available configuration options.
 *
 * To create a MIDI driver, you need to specify a source for the MIDI events to be
 * forwarded to via the \ref fluid_midi_event_t callback. Normally this will be
 * either a \ref midi_router via fluid_midi_router_handle_midi_event() or the synthesizer
 * via fluid_synth_handle_midi_event().
 *
 * But you can also write your own handler function that preprocesses the events and
 * forwards them on to the router or synthesizer instead.
 *
 * @{
 */

/** @startlifecycle{MIDI Driver} */
FLUIDSYNTH_API
fluid_midi_driver_t *new_fluid_midi_driver(fluid_settings_t *settings,
        handle_midi_event_func_t handler,
        void *event_handler_data);

FLUIDSYNTH_API void delete_fluid_midi_driver(fluid_midi_driver_t *driver);
/** @endlifecycle */

/** @} */

/**
 * @defgroup midi_player MIDI File Player
 * @ingroup midi_input
 *
 * Parse standard MIDI files and emit MIDI events.
 *
 * @{
 */

/**
 * MIDI File Player status enum.
 * @since 1.1.0
 */
enum fluid_player_status
{
    FLUID_PLAYER_READY,           /**< Player is ready */
    FLUID_PLAYER_PLAYING,         /**< Player is currently playing */
    FLUID_PLAYER_STOPPING,        /**< Player is stopping, but hasn't finished yet (currently unused) */
    FLUID_PLAYER_DONE             /**< Player is finished playing */
};

/**
 * MIDI File Player tempo enum.
 * @since 2.2.0
 */
enum fluid_player_set_tempo_type
{
    FLUID_PLAYER_TEMPO_INTERNAL,      /**< Use midi file tempo set in midi file (120 bpm by default). Multiplied by a factor */
    FLUID_PLAYER_TEMPO_EXTERNAL_BPM,  /**< Set player tempo in bpm, supersede midi file tempo */
    FLUID_PLAYER_TEMPO_EXTERNAL_MIDI, /**< Set player tempo in us per quarter note, supersede midi file tempo */
    FLUID_PLAYER_TEMPO_NBR        /**< @internal Value defines the count of player tempo type (#fluid_player_set_tempo_type) @warning This symbol is not part of the public API and ABI stability guarantee and may change at any time! */
};

/** @startlifecycle{MIDI File Player} */
FLUIDSYNTH_API fluid_player_t *new_fluid_player(fluid_synth_t *synth);
FLUIDSYNTH_API void delete_fluid_player(fluid_player_t *player);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_player_add(fluid_player_t *player, const char *midifile);
FLUIDSYNTH_API int fluid_player_add_mem(fluid_player_t *player, const void *buffer, size_t len);
FLUIDSYNTH_API int fluid_player_play(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_stop(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_join(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_set_loop(fluid_player_t *player, int loop);
FLUIDSYNTH_API int fluid_player_set_tempo(fluid_player_t *player, int tempo_type, double tempo);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_player_set_midi_tempo(fluid_player_t *player, int tempo);
FLUID_DEPRECATED FLUIDSYNTH_API int fluid_player_set_bpm(fluid_player_t *player, int bpm);
FLUIDSYNTH_API int fluid_player_set_playback_callback(fluid_player_t *player, handle_midi_event_func_t handler, void *handler_data);
FLUIDSYNTH_API int fluid_player_set_tick_callback(fluid_player_t *player, handle_midi_tick_func_t handler, void *handler_data);

FLUIDSYNTH_API int fluid_player_get_status(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_get_current_tick(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_get_total_ticks(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_get_bpm(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_get_division(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_get_midi_tempo(fluid_player_t *player);
FLUIDSYNTH_API int fluid_player_seek(fluid_player_t *player, int ticks);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_MIDI_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SEQ_H
#define _FLUIDSYNTH_SEQ_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup sequencer MIDI Sequencer
 *
 * MIDI event sequencer.
 *
 * The MIDI sequencer can be used to play MIDI events in a more flexible way than
 * using the MIDI file player, which expects the events to be stored as
 * Standard MIDI Files. Using the sequencer, you can provide the events one by
 * one, with an optional timestamp for scheduling.
 *
 * @{
 */

/**
 * Event callback prototype for destination clients.
 *
 * @param time Current sequencer tick value (see fluid_sequencer_get_tick()).
 * @param event The event being received
 * @param seq The sequencer instance
 * @param data User defined data registered with the client
 *
 * @note @p time may not be of the same tick value as the scheduled event! In fact, depending on
 * the sequencer's scale and the synth's sample-rate, @p time may be a few ticks too late. Although this
 * itself is inaudible, it is important to consider,
 * when you use this callback for enqueuing additional events over and over again with
 * fluid_sequencer_send_at(): If you enqueue new events with a relative tick value you might introduce
 * a timing error, which causes your sequence to sound e.g. slower than it's supposed to be. If this is
 * your use-case, make sure to enqueue events with an absolute tick value.
 */
typedef void (*fluid_event_callback_t)(unsigned int time, fluid_event_t *event,
                                       fluid_sequencer_t *seq, void *data);


/** @startlifecycle{MIDI Sequencer} */
FLUID_DEPRECATED FLUIDSYNTH_API fluid_sequencer_t *new_fluid_sequencer(void);
FLUIDSYNTH_API fluid_sequencer_t *new_fluid_sequencer2(int use_system_timer);
FLUIDSYNTH_API void delete_fluid_sequencer(fluid_sequencer_t *seq);
/** @endlifecycle */

FLUIDSYNTH_API int fluid_sequencer_get_use_system_timer(fluid_sequencer_t *seq);
FLUIDSYNTH_API
fluid_seq_id_t fluid_sequencer_register_client(fluid_sequencer_t *seq, const char *name,
        fluid_event_callback_t callback, void *data);
FLUIDSYNTH_API void fluid_sequencer_unregister_client(fluid_sequencer_t *seq, fluid_seq_id_t id);
FLUIDSYNTH_API int fluid_sequencer_count_clients(fluid_sequencer_t *seq);
FLUIDSYNTH_API fluid_seq_id_t fluid_sequencer_get_client_id(fluid_sequencer_t *seq, int index);
FLUIDSYNTH_API char *fluid_sequencer_get_client_name(fluid_sequencer_t *seq, fluid_seq_id_t id);
FLUIDSYNTH_API int fluid_sequencer_client_is_dest(fluid_sequencer_t *seq, fluid_seq_id_t id);
FLUIDSYNTH_API void fluid_sequencer_process(fluid_sequencer_t *seq, unsigned int msec);
FLUIDSYNTH_API void fluid_sequencer_send_now(fluid_sequencer_t *seq, fluid_event_t *evt);
FLUIDSYNTH_API
int fluid_sequencer_send_at(fluid_sequencer_t *seq, fluid_event_t *evt,
                            unsigned int time, int absolute);
FLUIDSYNTH_API
void fluid_sequencer_remove_events(fluid_sequencer_t *seq, fluid_seq_id_t source, fluid_seq_id_t dest, int type);
FLUIDSYNTH_API unsigned int fluid_sequencer_get_tick(fluid_sequencer_t *seq);
FLUIDSYNTH_API void fluid_sequencer_set_time_scale(fluid_sequencer_t *seq, double scale);
FLUIDSYNTH_API double fluid_sequencer_get_time_scale(fluid_sequencer_t *seq);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_SEQ_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_SEQBIND_H
#define _FLUIDSYNTH_SEQBIND_H

#include "seq.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup sequencer
 *
 * @{
 */
FLUIDSYNTH_API
fluid_seq_id_t fluid_sequencer_register_fluidsynth(fluid_sequencer_t *seq, fluid_synth_t *synth);
FLUIDSYNTH_API 
int fluid_sequencer_add_midi_event_to_buffer(void *data, fluid_midi_event_t *event);
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_SEQBIND_H */

/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_LOG_H
#define _FLUIDSYNTH_LOG_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup logging Logging
 *
 * Logging interface
 *
 * The default logging function of the fluidsynth prints its messages to the
 * stderr. The synthesizer uses five level of messages: #FLUID_PANIC,
 * #FLUID_ERR, #FLUID_WARN, #FLUID_INFO, and #FLUID_DBG.
 *
 * A client application can install a new log function to handle the messages
 * differently. In the following example, the application sets a callback
 * function to display #FLUID_PANIC messages in a dialog, and ignores all other
 * messages by setting the log function to NULL:
 *
 * @code
 * fluid_set_log_function(FLUID_PANIC, show_dialog, (void*) root_window);
 * fluid_set_log_function(FLUID_ERR, NULL, NULL);
 * fluid_set_log_function(FLUID_WARN, NULL, NULL);
 * fluid_set_log_function(FLUID_DBG, NULL, NULL);
 * @endcode
 *
 * @note The logging configuration is global and not tied to a specific
 * synthesizer instance. That means that all synthesizer instances created in
 * the same process share the same logging configuration.
 *
 * @{
 */

/**
 * FluidSynth log levels.
 */
enum fluid_log_level
{
    FLUID_PANIC,   /**< The synth can't function correctly any more */
    FLUID_ERR,     /**< Serious error occurred */
    FLUID_WARN,    /**< Warning */
    FLUID_INFO,    /**< Verbose informational messages */
    FLUID_DBG,     /**< Debugging messages */
    LAST_LOG_LEVEL /**< @internal This symbol is not part of the public API and ABI
                     stability guarantee and may change at any time! */
};

/**
 * Log function handler callback type used by fluid_set_log_function().
 *
 * @param level Log level (#fluid_log_level)
 * @param message Log message text
 * @param data User data pointer supplied to fluid_set_log_function().
 */
typedef void (*fluid_log_function_t)(int level, const char *message, void *data);

FLUIDSYNTH_API
fluid_log_function_t fluid_set_log_function(int level, fluid_log_function_t fun, void *data);

FLUIDSYNTH_API void fluid_default_log_function(int level, const char *message, void *data);

FLUIDSYNTH_API int fluid_log(int level, const char *fmt, ...)
#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
__attribute__ ((format (printf, 2, 3)))
#endif
;
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_LOG_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_MISC_H
#define _FLUIDSYNTH_MISC_H


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup misc Miscellaneous
 *
 * Miscellaneous utility functions and defines
 *
 * @{
 */

/**
 * Value that indicates success, used by most libfluidsynth functions.
 *
 * @note This was not publicly defined prior to libfluidsynth 1.1.0.  When
 * writing code which should also be compatible with older versions, something
 * like the following can be used:
 *
 * @code
 *   #include <fluidsynth.h>
 *
 *   #ifndef FLUID_OK
 *   #define FLUID_OK      (0)
 *   #define FLUID_FAILED  (-1)
 *   #endif
 * @endcode
 *
 * @since 1.1.0
 */
#define FLUID_OK        (0)

/**
 * Value that indicates failure, used by most libfluidsynth functions.
 *
 * @note See #FLUID_OK for more details.
 *
 * @since 1.1.0
 */
#define FLUID_FAILED    (-1)


FLUIDSYNTH_API int fluid_is_soundfont(const char *filename);
FLUIDSYNTH_API int fluid_is_midifile(const char *filename);
FLUIDSYNTH_API void fluid_free(void* ptr);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_MISC_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_MOD_H
#define _FLUIDSYNTH_MOD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup modulators SoundFont Modulators
 * @ingroup soundfonts
 *
 * SoundFont modulator functions and constants.
 *
 * @{
 */

/**
 * Flags defining the polarity, mapping function and type of a modulator source.
 * Compare with SoundFont 2.04 PDF section 8.2.
 *
 * Note: Bit values do not correspond to the SoundFont spec!  Also note that
 * #FLUID_MOD_GC and #FLUID_MOD_CC are in the flags field instead of the source field.
 */
enum fluid_mod_flags
{
    FLUID_MOD_POSITIVE = 0,       /**< Mapping function is positive */
    FLUID_MOD_NEGATIVE = 1,       /**< Mapping function is negative */
    FLUID_MOD_UNIPOLAR = 0,       /**< Mapping function is unipolar */
    FLUID_MOD_BIPOLAR = 2,        /**< Mapping function is bipolar */
    FLUID_MOD_LINEAR = 0,         /**< Linear mapping function */
    FLUID_MOD_CONCAVE = 4,        /**< Concave mapping function */
    FLUID_MOD_CONVEX = 8,         /**< Convex mapping function */
    FLUID_MOD_SWITCH = 12,        /**< Switch (on/off) mapping function */
    FLUID_MOD_GC = 0,             /**< General controller source type (#fluid_mod_src) */
    FLUID_MOD_CC = 16,             /**< MIDI CC controller (source will be a MIDI CC number) */

    FLUID_MOD_SIN = 0x80,            /**< Custom non-standard sinus mapping function */
};

/**
 * General controller (if #FLUID_MOD_GC in flags).  This
 * corresponds to SoundFont 2.04 PDF section 8.2.1
 */
enum fluid_mod_src
{
    FLUID_MOD_NONE = 0,                   /**< No source controller */
    FLUID_MOD_VELOCITY = 2,               /**< MIDI note-on velocity */
    FLUID_MOD_KEY = 3,                    /**< MIDI note-on note number */
    FLUID_MOD_KEYPRESSURE = 10,           /**< MIDI key pressure */
    FLUID_MOD_CHANNELPRESSURE = 13,       /**< MIDI channel pressure */
    FLUID_MOD_PITCHWHEEL = 14,            /**< Pitch wheel */
    FLUID_MOD_PITCHWHEELSENS = 16         /**< Pitch wheel sensitivity */
};

/** @startlifecycle{Modulator} */
FLUIDSYNTH_API fluid_mod_t *new_fluid_mod(void);
FLUIDSYNTH_API void delete_fluid_mod(fluid_mod_t *mod);
/** @endlifecycle */

FLUIDSYNTH_API size_t fluid_mod_sizeof(void);

FLUIDSYNTH_API void fluid_mod_set_source1(fluid_mod_t *mod, int src, int flags);
FLUIDSYNTH_API void fluid_mod_set_source2(fluid_mod_t *mod, int src, int flags);
FLUIDSYNTH_API void fluid_mod_set_dest(fluid_mod_t *mod, int dst);
FLUIDSYNTH_API void fluid_mod_set_amount(fluid_mod_t *mod, double amount);

FLUIDSYNTH_API int fluid_mod_get_source1(const fluid_mod_t *mod);
FLUIDSYNTH_API int fluid_mod_get_flags1(const fluid_mod_t *mod);
FLUIDSYNTH_API int fluid_mod_get_source2(const fluid_mod_t *mod);
FLUIDSYNTH_API int fluid_mod_get_flags2(const fluid_mod_t *mod);
FLUIDSYNTH_API int fluid_mod_get_dest(const fluid_mod_t *mod);
FLUIDSYNTH_API double fluid_mod_get_amount(const fluid_mod_t *mod);

FLUIDSYNTH_API int fluid_mod_test_identity(const fluid_mod_t *mod1, const fluid_mod_t *mod2);
FLUIDSYNTH_API int fluid_mod_has_source(const fluid_mod_t *mod, int cc, int ctrl);
FLUIDSYNTH_API int fluid_mod_has_dest(const fluid_mod_t *mod, int gen);

FLUIDSYNTH_API void fluid_mod_clone(fluid_mod_t *mod, const fluid_mod_t *src);
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_MOD_H */

/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_GEN_H
#define _FLUIDSYNTH_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup generators SoundFont Generators
 * @ingroup soundfonts
 *
 * Functions and defines for SoundFont generator effects.
 *
 * @{
 */

/**
 * Generator (effect) numbers (Soundfont 2.01 specifications section 8.1.3)
 */
enum fluid_gen_type
{
    GEN_STARTADDROFS,		/**< Sample start address offset (0-32767) */
    GEN_ENDADDROFS,		/**< Sample end address offset (-32767-0) */
    GEN_STARTLOOPADDROFS,		/**< Sample loop start address offset (-32767-32767) */
    GEN_ENDLOOPADDROFS,		/**< Sample loop end address offset (-32767-32767) */
    GEN_STARTADDRCOARSEOFS,	/**< Sample start address coarse offset (X 32768) */
    GEN_MODLFOTOPITCH,		/**< Modulation LFO to pitch */
    GEN_VIBLFOTOPITCH,		/**< Vibrato LFO to pitch */
    GEN_MODENVTOPITCH,		/**< Modulation envelope to pitch */
    GEN_FILTERFC,			/**< Filter cutoff */
    GEN_FILTERQ,			/**< Filter Q */
    GEN_MODLFOTOFILTERFC,		/**< Modulation LFO to filter cutoff */
    GEN_MODENVTOFILTERFC,		/**< Modulation envelope to filter cutoff */
    GEN_ENDADDRCOARSEOFS,		/**< Sample end address coarse offset (X 32768) */
    GEN_MODLFOTOVOL,		/**< Modulation LFO to volume */
    GEN_UNUSED1,			/**< Unused */
    GEN_CHORUSSEND,		/**< Chorus send amount */
    GEN_REVERBSEND,		/**< Reverb send amount */
    GEN_PAN,			/**< Stereo panning */
    GEN_UNUSED2,			/**< Unused */
    GEN_UNUSED3,			/**< Unused */
    GEN_UNUSED4,			/**< Unused */
    GEN_MODLFODELAY,		/**< Modulation LFO delay */
    GEN_MODLFOFREQ,		/**< Modulation LFO frequency */
    GEN_VIBLFODELAY,		/**< Vibrato LFO delay */
    GEN_VIBLFOFREQ,		/**< Vibrato LFO frequency */
    GEN_MODENVDELAY,		/**< Modulation envelope delay */
    GEN_MODENVATTACK,		/**< Modulation envelope attack */
    GEN_MODENVHOLD,		/**< Modulation envelope hold */
    GEN_MODENVDECAY,		/**< Modulation envelope decay */
    GEN_MODENVSUSTAIN,		/**< Modulation envelope sustain */
    GEN_MODENVRELEASE,		/**< Modulation envelope release */
    GEN_KEYTOMODENVHOLD,		/**< Key to modulation envelope hold */
    GEN_KEYTOMODENVDECAY,		/**< Key to modulation envelope decay */
    GEN_VOLENVDELAY,		/**< Volume envelope delay */
    GEN_VOLENVATTACK,		/**< Volume envelope attack */
    GEN_VOLENVHOLD,		/**< Volume envelope hold */
    GEN_VOLENVDECAY,		/**< Volume envelope decay */
    GEN_VOLENVSUSTAIN,		/**< Volume envelope sustain */
    GEN_VOLENVRELEASE,		/**< Volume envelope release */
    GEN_KEYTOVOLENVHOLD,		/**< Key to volume envelope hold */
    GEN_KEYTOVOLENVDECAY,		/**< Key to volume envelope decay */
    GEN_INSTRUMENT,		/**< Instrument ID (shouldn't be set by user) */
    GEN_RESERVED1,		/**< Reserved */
    GEN_KEYRANGE,			/**< MIDI note range */
    GEN_VELRANGE,			/**< MIDI velocity range */
    GEN_STARTLOOPADDRCOARSEOFS,	/**< Sample start loop address coarse offset (X 32768) */
    GEN_KEYNUM,			/**< Fixed MIDI note number */
    GEN_VELOCITY,			/**< Fixed MIDI velocity value */
    GEN_ATTENUATION,		/**< Initial volume attenuation */
    GEN_RESERVED2,		/**< Reserved */
    GEN_ENDLOOPADDRCOARSEOFS,	/**< Sample end loop address coarse offset (X 32768) */
    GEN_COARSETUNE,		/**< Coarse tuning */
    GEN_FINETUNE,			/**< Fine tuning */
    GEN_SAMPLEID,			/**< Sample ID (shouldn't be set by user) */
    GEN_SAMPLEMODE,		/**< Sample mode flags */
    GEN_RESERVED3,		/**< Reserved */
    GEN_SCALETUNE,		/**< Scale tuning */
    GEN_EXCLUSIVECLASS,		/**< Exclusive class number */
    GEN_OVERRIDEROOTKEY,		/**< Sample root note override */

    /**
     * Initial Pitch
     *
     * @note This is not "standard" SoundFont generator, because it is not
     * mentioned in the list of generators in the SF2 specifications.
     * It is used by FluidSynth internally to compute the nominal pitch of
     * a note on note-on event. By nature it shouldn't be allowed to be modulated,
     * however the specification defines a default modulator having "Initial Pitch"
     * as destination (cf. SF2.01 page 57 section 8.4.10 MIDI Pitch Wheel to Initial Pitch).
     * Thus it is impossible to cancel this default modulator, which would be required
     * to let the MIDI Pitch Wheel controller modulate a different generator.
     * In order to provide this flexibility, FluidSynth >= 2.1.0 uses a default modulator
     * "Pitch Wheel to Fine Tune", rather than Initial Pitch. The same "compromise" can
     * be found on the Audigy 2 ZS for instance.
     */
    GEN_PITCH,

    GEN_CUSTOM_BALANCE,          /**< Balance @note Not a real SoundFont generator */
    /* non-standard generator for an additional custom high- or low-pass filter */
    GEN_CUSTOM_FILTERFC,		/**< Custom filter cutoff frequency */
    GEN_CUSTOM_FILTERQ,		/**< Custom filter Q */

    GEN_LAST			/**< @internal Value defines the count of generators (#fluid_gen_type)
                          @warning This symbol is not part of the public API and ABI
                          stability guarantee and may change at any time! */
};
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_GEN_H */

/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_VOICE_H
#define _FLUIDSYNTH_VOICE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup voices Voice Manipulation
 * @ingroup soundfonts
 *
 * Synthesis voice manipulation functions.
 *
 * The interface to the synthesizer's voices.
 * Examples on using them can be found in the source code of the default SoundFont
 * loader (fluid_defsfont.c).
 *
 * Most of these functions should only be called from within synthesis context,
 * such as the SoundFont loader's noteon method.
 *
 * @{
 */

/**
 * Enum used with fluid_voice_add_mod() to specify how to handle duplicate modulators.
 */
enum fluid_voice_add_mod
{
    FLUID_VOICE_OVERWRITE,        /**< Overwrite any existing matching modulator */
    FLUID_VOICE_ADD,              /**< Add (sum) modulator amounts */
    FLUID_VOICE_DEFAULT           /**< For default modulators only, no need to check for duplicates */
};

FLUIDSYNTH_API void fluid_voice_add_mod(fluid_voice_t *voice, fluid_mod_t *mod, int mode);
FLUIDSYNTH_API float fluid_voice_gen_get(fluid_voice_t *voice, int gen);
FLUIDSYNTH_API void fluid_voice_gen_set(fluid_voice_t *voice, int gen, float val);
FLUIDSYNTH_API void fluid_voice_gen_incr(fluid_voice_t *voice, int gen, float val);

FLUIDSYNTH_API unsigned int fluid_voice_get_id(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_get_channel(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_get_key(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_get_actual_key(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_get_velocity(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_get_actual_velocity(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_is_playing(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_is_on(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_is_sustained(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_is_sostenuto(const fluid_voice_t *voice);
FLUIDSYNTH_API int fluid_voice_optimize_sample(fluid_sample_t *s);
FLUIDSYNTH_API void fluid_voice_update_param(fluid_voice_t *voice, int gen);
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* _FLUIDSYNTH_VOICE_H */

/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *  
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_VERSION_H
#define _FLUIDSYNTH_VERSION_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup misc
 *
 * @{
 */
#define FLUIDSYNTH_VERSION       "2.3.4"           /**< String constant of libfluidsynth version. */
#define FLUIDSYNTH_VERSION_MAJOR 2     /**< libfluidsynth major version integer constant. */
#define FLUIDSYNTH_VERSION_MINOR 3     /**< libfluidsynth minor version integer constant. */
#define FLUIDSYNTH_VERSION_MICRO 4     /**< libfluidsynth micro version integer constant. */

FLUIDSYNTH_API void fluid_version(int *major, int *minor, int *micro);
FLUIDSYNTH_API char* fluid_version_str(void);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_VERSION_H */
/* FluidSynth - A Software Synthesizer
 *
 * Copyright (C) 2003  Peter Hanappe and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */

#ifndef _FLUIDSYNTH_LADSPA_H
#define _FLUIDSYNTH_LADSPA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ladspa Effect - LADSPA
 * @ingroup synth
 *
 * Functions for configuring the LADSPA effects unit
 *
 * This header defines useful functions for programmatically manipulating the ladspa
 * effects unit of the synth that can be retrieved via fluid_synth_get_ladspa_fx().
 *
 * Using any of those functions requires fluidsynth to be compiled with LADSPA support.
 * Else all of those functions are useless dummies.
 *
 * @{
 */
FLUIDSYNTH_API int fluid_ladspa_is_active(fluid_ladspa_fx_t *fx);
FLUIDSYNTH_API int fluid_ladspa_activate(fluid_ladspa_fx_t *fx);
FLUIDSYNTH_API int fluid_ladspa_deactivate(fluid_ladspa_fx_t *fx);
FLUIDSYNTH_API int fluid_ladspa_reset(fluid_ladspa_fx_t *fx);
FLUIDSYNTH_API int fluid_ladspa_check(fluid_ladspa_fx_t *fx, char *err, int err_size);

FLUIDSYNTH_API int fluid_ladspa_host_port_exists(fluid_ladspa_fx_t *fx, const char *name);

FLUIDSYNTH_API int fluid_ladspa_add_buffer(fluid_ladspa_fx_t *fx, const char *name);
FLUIDSYNTH_API int fluid_ladspa_buffer_exists(fluid_ladspa_fx_t *fx, const char *name);

FLUIDSYNTH_API int fluid_ladspa_add_effect(fluid_ladspa_fx_t *fx, const char *effect_name,
        const char *lib_name, const char *plugin_name);
FLUIDSYNTH_API int fluid_ladspa_effect_can_mix(fluid_ladspa_fx_t *fx, const char *name);
FLUIDSYNTH_API int fluid_ladspa_effect_set_mix(fluid_ladspa_fx_t *fx, const char *name, int mix, float gain);
FLUIDSYNTH_API int fluid_ladspa_effect_port_exists(fluid_ladspa_fx_t *fx, const char *effect_name, const char *port_name);
FLUIDSYNTH_API int fluid_ladspa_effect_set_control(fluid_ladspa_fx_t *fx, const char *effect_name,
        const char *port_name, float val);
FLUIDSYNTH_API int fluid_ladspa_effect_link(fluid_ladspa_fx_t *fx, const char *effect_name,
        const char *port_name, const char *name);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _FLUIDSYNTH_LADSPA_H */

