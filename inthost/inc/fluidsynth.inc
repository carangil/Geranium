section fsynth

	type constint end
	type audio_driver_t end
	type sample_t end
	type sfont_t end
	type constfluid_mod_t end
	type event_t end
	type mod_t end
	type constfluid_voice_t end
	type constdouble end
	type synth_t end
	type midi_event_t end
	type shell_t end
	type settings_t end
	type preset_t end
	type player_t end
	type sequencer_t end
	type voice_t end
	type constfluid_midi_event_t end
	type cmd_handler_t end
	type server_t end
	type ladspa_fx_t end
	type file_renderer_t end
	type midi_router_rule_t end
	type midi_driver_t end
	type midi_router_t end
	type sfloader_t end
  0x1 constant HINT_BOUNDED_BELOW

  0x2 constant HINT_BOUNDED_ABOVE

        0x4 constant HINT_TOGGLED

	// FLUIDSYNTH_API fluid_settings_t * new_fluid_settings (void)
	 (  -> settings_t* ) "libfluidsynth.so" "new_fluid_settings" sys optional new_fluid_settings 

	// FLUIDSYNTH_API void delete_fluid_settings (fluid_settings_t * settings)
	 ( settings_t*:_settings ) "libfluidsynth.so" "delete_fluid_settings" sys optional delete_fluid_settings 

	// FLUIDSYNTH_API int settings_get_type (fluid_settings_t * settings,const char * name)
	 ( settings_t*:_settings String:_name  -> Z32 ) "libfluidsynth.so" "fluid_settings_get_type" sys optional settings_get_type 

	// FLUIDSYNTH_API int settings_get_hints (fluid_settings_t * settings,const char * name,int * val)
	 ( settings_t*:_settings String:_name Z32*:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_get_hints" sys optional settings_get_hints 

	// FLUIDSYNTH_API int settings_is_realtime (fluid_settings_t * settings,const char * name)
	 ( settings_t*:_settings String:_name  -> Z32 ) "libfluidsynth.so" "fluid_settings_is_realtime" sys optional settings_is_realtime 

	// FLUIDSYNTH_API int settings_setstr (fluid_settings_t * settings,const char * name,const char * str)
	 ( settings_t*:_settings String:_name String:_str  -> Z32 ) "libfluidsynth.so" "fluid_settings_setstr" sys optional settings_setstr 

	// FLUIDSYNTH_API int settings_copystr (fluid_settings_t * settings,const char * name,char * str,int len)
	 ( settings_t*:_settings String:_name String:_str Z32:_len  -> Z32 ) "libfluidsynth.so" "fluid_settings_copystr" sys optional settings_copystr 

	// FLUIDSYNTH_API int settings_dupstr (fluid_settings_t * settings,const char * name,char ** str)
	 ( settings_t*:_settings String:_name String*:_str  -> Z32 ) "libfluidsynth.so" "fluid_settings_dupstr" sys optional settings_dupstr 

	// FLUIDSYNTH_API int settings_getstr_default (fluid_settings_t * settings,const char * name,char ** def)
	 ( settings_t*:_settings String:_name String*:_def  -> Z32 ) "libfluidsynth.so" "fluid_settings_getstr_default" sys optional settings_getstr_default 

	// FLUIDSYNTH_API int settings_str_equal (fluid_settings_t * settings,const char * name,const char * value)
	 ( settings_t*:_settings String:_name String:_value  -> Z32 ) "libfluidsynth.so" "fluid_settings_str_equal" sys optional settings_str_equal 

	// FLUIDSYNTH_API int settings_setnum (fluid_settings_t * settings,const char * name,double val)
	 ( settings_t*:_settings String:_name Double32:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_setnum" sys optional settings_setnum 

	// FLUIDSYNTH_API int settings_getnum (fluid_settings_t * settings,const char * name,double * val)
	 ( settings_t*:_settings String:_name Double32*:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_getnum" sys optional settings_getnum 

	// FLUIDSYNTH_API int settings_getnum_default (fluid_settings_t * settings,const char * name,double * val)
	 ( settings_t*:_settings String:_name Double32*:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_getnum_default" sys optional settings_getnum_default 

	// FLUIDSYNTH_API int settings_getnum_range (fluid_settings_t * settings,const char * name,double * min,double * max)
	 ( settings_t*:_settings String:_name Double32*:_min Double32*:_max  -> Z32 ) "libfluidsynth.so" "fluid_settings_getnum_range" sys optional settings_getnum_range 

	// FLUIDSYNTH_API int settings_setint (fluid_settings_t * settings,const char * name,int val)
	 ( settings_t*:_settings String:_name Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_setint" sys optional settings_setint 

	// FLUIDSYNTH_API int settings_getint (fluid_settings_t * settings,const char * name,int * val)
	 ( settings_t*:_settings String:_name Z32*:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_getint" sys optional settings_getint 

	// FLUIDSYNTH_API int settings_getint_default (fluid_settings_t * settings,const char * name,int * val)
	 ( settings_t*:_settings String:_name Z32*:_val  -> Z32 ) "libfluidsynth.so" "fluid_settings_getint_default" sys optional settings_getint_default 

	// FLUIDSYNTH_API int settings_getint_range (fluid_settings_t * settings,const char * name,int * min,int * max)
	 ( settings_t*:_settings String:_name Z32*:_min Z32*:_max  -> Z32 ) "libfluidsynth.so" "fluid_settings_getint_range" sys optional settings_getint_range 

	// FLUIDSYNTH_API void settings_foreach_option (fluid_settings_t * settings,const char * name,void * data,fluid_settings_foreach_option_t func)
	 ( settings_t*:_settings String:_name Any&:_data Any*:_func ) "libfluidsynth.so" "fluid_settings_foreach_option" sys optional settings_foreach_option 

	// FLUIDSYNTH_API int settings_option_count (fluid_settings_t * settings,const char * name)
	 ( settings_t*:_settings String:_name  -> Z32 ) "libfluidsynth.so" "fluid_settings_option_count" sys optional settings_option_count 

	// FLUIDSYNTH_API char * settings_option_concat (fluid_settings_t * settings,const char * name,const char * separator)
	 ( settings_t*:_settings String:_name String:_separator  -> String ) "libfluidsynth.so" "fluid_settings_option_concat" sys optional settings_option_concat 

	// FLUIDSYNTH_API void settings_foreach (fluid_settings_t * settings,void * data,fluid_settings_foreach_t func)
	 ( settings_t*:_settings Any&:_data Any*:_func ) "libfluidsynth.so" "fluid_settings_foreach" sys optional settings_foreach 

	// FLUIDSYNTH_API fluid_synth_t * new_fluid_synth (fluid_settings_t * settings)
	 ( settings_t*:_settings  -> synth_t* ) "libfluidsynth.so" "new_fluid_synth" sys optional ^new_fluid_synth 

	// FLUIDSYNTH_API void delete_fluid_synth (fluid_synth_t * synth)
	 ( synth_t*:_synth ) "libfluidsynth.so" "delete_fluid_synth" sys optional delete_fluid_synth 

	// FLUIDSYNTH_API double synth_get_cpu_load (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_cpu_load" sys optional synth_get_cpu_load 

	// FLUID_DEPRECATED FLUIDSYNTH_API const char * synth_error (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> String ) "libfluidsynth.so" "fluid_synth_error" sys optional synth_error 

	// FLUIDSYNTH_API int synth_noteon (fluid_synth_t * synth,int chan,int key,int vel)
	 ( synth_t*:_synth Z32:_chan Z32:_key Z32:_vel  -> Z32 ) "libfluidsynth.so" "fluid_synth_noteon" sys optional ^noteon 

	// FLUIDSYNTH_API int synth_noteoff (fluid_synth_t * synth,int chan,int key)
	 ( synth_t*:_synth Z32:_chan Z32:_key  -> Z32 ) "libfluidsynth.so" "fluid_synth_noteoff" sys optional ^noteoff 

	// FLUIDSYNTH_API int synth_cc (fluid_synth_t * synth,int chan,int ctrl,int val)
	 ( synth_t*:_synth Z32:_chan Z32:_ctrl Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_synth_cc" sys optional synth_cc 

	// FLUIDSYNTH_API int synth_get_cc (fluid_synth_t * synth,int chan,int ctrl,int * pval)
	 ( synth_t*:_synth Z32:_chan Z32:_ctrl Z32*:_pval  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_cc" sys optional synth_get_cc 

	// FLUIDSYNTH_API int synth_sysex (fluid_synth_t * synth,const char * data,int len,char * response,int * response_len,int * handled,int dryrun)
	 ( synth_t*:_synth String:_data Z32:_len String:_response Z32*:_response_len Z32*:_handled Z32:_dryrun  -> Z32 ) "libfluidsynth.so" "fluid_synth_sysex" sys optional synth_sysex 

	// FLUIDSYNTH_API int synth_pitch_bend (fluid_synth_t * synth,int chan,int val)
	 ( synth_t*:_synth Z32:_chan Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_synth_pitch_bend" sys optional synth_pitch_bend 

	// FLUIDSYNTH_API int synth_get_pitch_bend (fluid_synth_t * synth,int chan,int * ppitch_bend)
	 ( synth_t*:_synth Z32:_chan Z32*:_ppitch_bend  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_pitch_bend" sys optional synth_get_pitch_bend 

	// FLUIDSYNTH_API int synth_pitch_wheel_sens (fluid_synth_t * synth,int chan,int val)
	 ( synth_t*:_synth Z32:_chan Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_synth_pitch_wheel_sens" sys optional synth_pitch_wheel_sens 

	// FLUIDSYNTH_API int synth_get_pitch_wheel_sens (fluid_synth_t * synth,int chan,int * pval)
	 ( synth_t*:_synth Z32:_chan Z32*:_pval  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_pitch_wheel_sens" sys optional synth_get_pitch_wheel_sens 

	// FLUIDSYNTH_API int synth_program_change (fluid_synth_t * synth,int chan,int program)
	 ( synth_t*:_synth Z32:_chan Z32:_program  -> Z32 ) "libfluidsynth.so" "fluid_synth_program_change" sys optional ^program_change 

	// FLUIDSYNTH_API int synth_channel_pressure (fluid_synth_t * synth,int chan,int val)
	 ( synth_t*:_synth Z32:_chan Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_synth_channel_pressure" sys optional synth_channel_pressure 

	// FLUIDSYNTH_API int synth_key_pressure (fluid_synth_t * synth,int chan,int key,int val)
	 ( synth_t*:_synth Z32:_chan Z32:_key Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_synth_key_pressure" sys optional synth_key_pressure 

	// FLUIDSYNTH_API int synth_bank_select (fluid_synth_t * synth,int chan,int bank)
	 ( synth_t*:_synth Z32:_chan Z32:_bank  -> Z32 ) "libfluidsynth.so" "fluid_synth_bank_select" sys optional synth_bank_select 

	// FLUIDSYNTH_API int synth_sfont_select (fluid_synth_t * synth,int chan,int sfont_id)
	 ( synth_t*:_synth Z32:_chan Z32:_sfont_id  -> Z32 ) "libfluidsynth.so" "fluid_synth_sfont_select" sys optional synth_sfont_select 

	// FLUIDSYNTH_API int synth_program_select (fluid_synth_t * synth,int chan,int sfont_id,int bank_num,int preset_num)
	 ( synth_t*:_synth Z32:_chan Z32:_sfont_id Z32:_bank_num Z32:_preset_num  -> Z32 ) "libfluidsynth.so" "fluid_synth_program_select" sys optional synth_program_select 

	// FLUIDSYNTH_API int synth_program_select_by_sfont_name (fluid_synth_t * synth,int chan,const char * sfont_name,int bank_num,int preset_num)
	 ( synth_t*:_synth Z32:_chan String:_sfont_name Z32:_bank_num Z32:_preset_num  -> Z32 ) "libfluidsynth.so" "fluid_synth_program_select_by_sfont_name" sys optional synth_program_select_by_sfont_name 

	// FLUIDSYNTH_API int synth_get_program (fluid_synth_t * synth,int chan,int * sfont_id,int * bank_num,int * preset_num)
	 ( synth_t*:_synth Z32:_chan Z32*:_sfont_id Z32*:_bank_num Z32*:_preset_num  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_program" sys optional synth_get_program 

	// FLUIDSYNTH_API int synth_unset_program (fluid_synth_t * synth,int chan)
	 ( synth_t*:_synth Z32:_chan  -> Z32 ) "libfluidsynth.so" "fluid_synth_unset_program" sys optional synth_unset_program 

	// FLUIDSYNTH_API int synth_program_reset (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_program_reset" sys optional synth_program_reset 

	// FLUIDSYNTH_API int synth_system_reset (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_system_reset" sys optional synth_system_reset 

	// FLUIDSYNTH_API int synth_all_notes_off (fluid_synth_t * synth,int chan)
	 ( synth_t*:_synth Z32:_chan  -> Z32 ) "libfluidsynth.so" "fluid_synth_all_notes_off" sys optional synth_all_notes_off 

	// FLUIDSYNTH_API int synth_all_sounds_off (fluid_synth_t * synth,int chan)
	 ( synth_t*:_synth Z32:_chan  -> Z32 ) "libfluidsynth.so" "fluid_synth_all_sounds_off" sys optional synth_all_sounds_off 

	// FLUIDSYNTH_API int synth_set_gen (fluid_synth_t * synth,int chan,int param,float value)
	 ( synth_t*:_synth Z32:_chan Z32:_param Real:_value  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_gen" sys optional synth_set_gen 

	// FLUIDSYNTH_API float synth_get_gen (fluid_synth_t * synth,int chan,int param)
	 ( synth_t*:_synth Z32:_chan Z32:_param  -> Real ) "libfluidsynth.so" "fluid_synth_get_gen" sys optional synth_get_gen 

	// FLUIDSYNTH_API int synth_start (fluid_synth_t * synth,unsigned int id,fluid_preset_t * preset,int audio_chan,int midi_chan,int key,int vel)
	 ( synth_t*:_synth Z32:_id preset_t*:_preset Z32:_audio_chan Z32:_midi_chan Z32:_key Z32:_vel  -> Z32 ) "libfluidsynth.so" "fluid_synth_start" sys optional synth_start 

	// FLUIDSYNTH_API int synth_stop (fluid_synth_t * synth,unsigned int id)
	 ( synth_t*:_synth Z32:_id  -> Z32 ) "libfluidsynth.so" "fluid_synth_stop" sys optional synth_stop 

	// FLUIDSYNTH_API fluid_voice_t * synth_alloc_voice (fluid_synth_t * synth,fluid_sample_t * sample,int channum,int key,int vel)
	 ( synth_t*:_synth sample_t*:_sample Z32:_channum Z32:_key Z32:_vel  -> voice_t* ) "libfluidsynth.so" "fluid_synth_alloc_voice" sys optional synth_alloc_voice 

	// FLUIDSYNTH_API void synth_start_voice (fluid_synth_t * synth,fluid_voice_t * voice)
	 ( synth_t*:_synth voice_t*:_voice ) "libfluidsynth.so" "fluid_synth_start_voice" sys optional synth_start_voice 

	// FLUIDSYNTH_API void synth_get_voicelist (fluid_synth_t * synth,fluid_voice_t * buf[],int bufsize,int ID)
	 ( synth_t*:_synth [voice_t*]:_buf Z32:_bufsize Z32:_ID ) "libfluidsynth.so" "fluid_synth_get_voicelist" sys optional synth_get_voicelist 

	// FLUIDSYNTH_API int synth_sfload (fluid_synth_t * synth,const char * filename,int reset_presets)
	 ( synth_t*:_synth String:_filename Z32:_reset_presets  -> Z32 ) "libfluidsynth.so" "fluid_synth_sfload" sys optional ^sfload 

	// FLUIDSYNTH_API int synth_sfreload (fluid_synth_t * synth,int id)
	 ( synth_t*:_synth Z32:_id  -> Z32 ) "libfluidsynth.so" "fluid_synth_sfreload" sys optional synth_sfreload 

	// FLUIDSYNTH_API int synth_sfunload (fluid_synth_t * synth,int id,int reset_presets)
	 ( synth_t*:_synth Z32:_id Z32:_reset_presets  -> Z32 ) "libfluidsynth.so" "fluid_synth_sfunload" sys optional synth_sfunload 

	// FLUIDSYNTH_API int synth_add_sfont (fluid_synth_t * synth,fluid_sfont_t * sfont)
	 ( synth_t*:_synth sfont_t*:_sfont  -> Z32 ) "libfluidsynth.so" "fluid_synth_add_sfont" sys optional synth_add_sfont 

	// FLUIDSYNTH_API int synth_remove_sfont (fluid_synth_t * synth,fluid_sfont_t * sfont)
	 ( synth_t*:_synth sfont_t*:_sfont  -> Z32 ) "libfluidsynth.so" "fluid_synth_remove_sfont" sys optional synth_remove_sfont 

	// FLUIDSYNTH_API int synth_sfcount (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_sfcount" sys optional synth_sfcount 

	// FLUIDSYNTH_API fluid_sfont_t * synth_get_sfont (fluid_synth_t * synth,unsigned int num)
	 ( synth_t*:_synth Z32:_num  -> sfont_t* ) "libfluidsynth.so" "fluid_synth_get_sfont" sys optional synth_get_sfont 

	// FLUIDSYNTH_API fluid_sfont_t * synth_get_sfont_by_id (fluid_synth_t * synth,int id)
	 ( synth_t*:_synth Z32:_id  -> sfont_t* ) "libfluidsynth.so" "fluid_synth_get_sfont_by_id" sys optional synth_get_sfont_by_id 

	// FLUIDSYNTH_API fluid_sfont_t * synth_get_sfont_by_name (fluid_synth_t * synth,const char * name)
	 ( synth_t*:_synth String:_name  -> sfont_t* ) "libfluidsynth.so" "fluid_synth_get_sfont_by_name" sys optional synth_get_sfont_by_name 

	// FLUIDSYNTH_API int synth_set_bank_offset (fluid_synth_t * synth,int sfont_id,int offset)
	 ( synth_t*:_synth Z32:_sfont_id Z32:_offset  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_bank_offset" sys optional synth_set_bank_offset 

	// FLUIDSYNTH_API int synth_get_bank_offset (fluid_synth_t * synth,int sfont_id)
	 ( synth_t*:_synth Z32:_sfont_id  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_bank_offset" sys optional synth_get_bank_offset 

	// FLUID_DEPRECATED FLUIDSYNTH_API void synth_set_reverb_on (fluid_synth_t * synth,int on)
	 ( synth_t*:_synth Z32:_on ) "libfluidsynth.so" "fluid_synth_set_reverb_on" sys optional synth_set_reverb_on 

	// FLUIDSYNTH_API int synth_reverb_on (fluid_synth_t * synth,int fx_group,int on)
	 ( synth_t*:_synth Z32:_fx_group Z32:_on  -> Z32 ) "libfluidsynth.so" "fluid_synth_reverb_on" sys optional synth_reverb_on 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_reverb (fluid_synth_t * synth,double roomsize,double damping,double width,double level)
	 ( synth_t*:_synth Double32:_roomsize Double32:_damping Double32:_width Double32:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb" sys optional synth_set_reverb 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_reverb_roomsize (fluid_synth_t * synth,double roomsize)
	 ( synth_t*:_synth Double32:_roomsize  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_roomsize" sys optional synth_set_reverb_roomsize 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_reverb_damp (fluid_synth_t * synth,double damping)
	 ( synth_t*:_synth Double32:_damping  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_damp" sys optional synth_set_reverb_damp 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_reverb_width (fluid_synth_t * synth,double width)
	 ( synth_t*:_synth Double32:_width  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_width" sys optional synth_set_reverb_width 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_reverb_level (fluid_synth_t * synth,double level)
	 ( synth_t*:_synth Double32:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_level" sys optional synth_set_reverb_level 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_reverb_roomsize (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_reverb_roomsize" sys optional synth_get_reverb_roomsize 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_reverb_damp (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_reverb_damp" sys optional synth_get_reverb_damp 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_reverb_level (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_reverb_level" sys optional synth_get_reverb_level 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_reverb_width (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_reverb_width" sys optional synth_get_reverb_width 

	// FLUIDSYNTH_API int synth_set_reverb_group_roomsize (fluid_synth_t * synth,int fx_group,double roomsize)
	 ( synth_t*:_synth Z32:_fx_group Double32:_roomsize  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_group_roomsize" sys optional synth_set_reverb_group_roomsize 

	// FLUIDSYNTH_API int synth_set_reverb_group_damp (fluid_synth_t * synth,int fx_group,double damping)
	 ( synth_t*:_synth Z32:_fx_group Double32:_damping  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_group_damp" sys optional synth_set_reverb_group_damp 

	// FLUIDSYNTH_API int synth_set_reverb_group_width (fluid_synth_t * synth,int fx_group,double width)
	 ( synth_t*:_synth Z32:_fx_group Double32:_width  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_group_width" sys optional synth_set_reverb_group_width 

	// FLUIDSYNTH_API int synth_set_reverb_group_level (fluid_synth_t * synth,int fx_group,double level)
	 ( synth_t*:_synth Z32:_fx_group Double32:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_reverb_group_level" sys optional synth_set_reverb_group_level 

	// FLUIDSYNTH_API int synth_get_reverb_group_roomsize (fluid_synth_t * synth,int fx_group,double * roomsize)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_roomsize  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_reverb_group_roomsize" sys optional synth_get_reverb_group_roomsize 

	// FLUIDSYNTH_API int synth_get_reverb_group_damp (fluid_synth_t * synth,int fx_group,double * damping)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_damping  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_reverb_group_damp" sys optional synth_get_reverb_group_damp 

	// FLUIDSYNTH_API int synth_get_reverb_group_width (fluid_synth_t * synth,int fx_group,double * width)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_width  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_reverb_group_width" sys optional synth_get_reverb_group_width 

	// FLUIDSYNTH_API int synth_get_reverb_group_level (fluid_synth_t * synth,int fx_group,double * level)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_reverb_group_level" sys optional synth_get_reverb_group_level 

	// FLUID_DEPRECATED FLUIDSYNTH_API void synth_set_chorus_on (fluid_synth_t * synth,int on)
	 ( synth_t*:_synth Z32:_on ) "libfluidsynth.so" "fluid_synth_set_chorus_on" sys optional synth_set_chorus_on 

	// FLUIDSYNTH_API int synth_chorus_on (fluid_synth_t * synth,int fx_group,int on)
	 ( synth_t*:_synth Z32:_fx_group Z32:_on  -> Z32 ) "libfluidsynth.so" "fluid_synth_chorus_on" sys optional synth_chorus_on 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_chorus (fluid_synth_t * synth,int nr,double level,double speed,double depth_ms,int type)
	 ( synth_t*:_synth Z32:_nr Double32:_level Double32:_speed Double32:_depth_ms Z32:_type  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus" sys optional synth_set_chorus 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_chorus_nr (fluid_synth_t * synth,int nr)
	 ( synth_t*:_synth Z32:_nr  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_nr" sys optional synth_set_chorus_nr 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_chorus_level (fluid_synth_t * synth,double level)
	 ( synth_t*:_synth Double32:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_level" sys optional synth_set_chorus_level 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_chorus_speed (fluid_synth_t * synth,double speed)
	 ( synth_t*:_synth Double32:_speed  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_speed" sys optional synth_set_chorus_speed 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_chorus_depth (fluid_synth_t * synth,double depth_ms)
	 ( synth_t*:_synth Double32:_depth_ms  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_depth" sys optional synth_set_chorus_depth 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_set_chorus_type (fluid_synth_t * synth,int type)
	 ( synth_t*:_synth Z32:_type  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_type" sys optional synth_set_chorus_type 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_get_chorus_nr (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_nr" sys optional synth_get_chorus_nr 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_chorus_level (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_chorus_level" sys optional synth_get_chorus_level 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_chorus_speed (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_chorus_speed" sys optional synth_get_chorus_speed 

	// FLUID_DEPRECATED FLUIDSYNTH_API double synth_get_chorus_depth (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Double32 ) "libfluidsynth.so" "fluid_synth_get_chorus_depth" sys optional synth_get_chorus_depth 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_get_chorus_type (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_type" sys optional synth_get_chorus_type 

	// FLUIDSYNTH_API int synth_set_chorus_group_nr (fluid_synth_t * synth,int fx_group,int nr)
	 ( synth_t*:_synth Z32:_fx_group Z32:_nr  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_group_nr" sys optional synth_set_chorus_group_nr 

	// FLUIDSYNTH_API int synth_set_chorus_group_level (fluid_synth_t * synth,int fx_group,double level)
	 ( synth_t*:_synth Z32:_fx_group Double32:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_group_level" sys optional synth_set_chorus_group_level 

	// FLUIDSYNTH_API int synth_set_chorus_group_speed (fluid_synth_t * synth,int fx_group,double speed)
	 ( synth_t*:_synth Z32:_fx_group Double32:_speed  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_group_speed" sys optional synth_set_chorus_group_speed 

	// FLUIDSYNTH_API int synth_set_chorus_group_depth (fluid_synth_t * synth,int fx_group,double depth_ms)
	 ( synth_t*:_synth Z32:_fx_group Double32:_depth_ms  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_group_depth" sys optional synth_set_chorus_group_depth 

	// FLUIDSYNTH_API int synth_set_chorus_group_type (fluid_synth_t * synth,int fx_group,int type)
	 ( synth_t*:_synth Z32:_fx_group Z32:_type  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_chorus_group_type" sys optional synth_set_chorus_group_type 

	// FLUIDSYNTH_API int synth_get_chorus_group_nr (fluid_synth_t * synth,int fx_group,int * nr)
	 ( synth_t*:_synth Z32:_fx_group Z32*:_nr  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_group_nr" sys optional synth_get_chorus_group_nr 

	// FLUIDSYNTH_API int synth_get_chorus_group_level (fluid_synth_t * synth,int fx_group,double * level)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_level  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_group_level" sys optional synth_get_chorus_group_level 

	// FLUIDSYNTH_API int synth_get_chorus_group_speed (fluid_synth_t * synth,int fx_group,double * speed)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_speed  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_group_speed" sys optional synth_get_chorus_group_speed 

	// FLUIDSYNTH_API int synth_get_chorus_group_depth (fluid_synth_t * synth,int fx_group,double * depth_ms)
	 ( synth_t*:_synth Z32:_fx_group Double32*:_depth_ms  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_group_depth" sys optional synth_get_chorus_group_depth 

	// FLUIDSYNTH_API int synth_get_chorus_group_type (fluid_synth_t * synth,int fx_group,int * type)
	 ( synth_t*:_synth Z32:_fx_group Z32*:_type  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_chorus_group_type" sys optional synth_get_chorus_group_type 

	// FLUIDSYNTH_API int synth_count_midi_channels (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_count_midi_channels" sys optional synth_count_midi_channels 

	// FLUIDSYNTH_API int synth_count_audio_channels (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_count_audio_channels" sys optional synth_count_audio_channels 

	// FLUIDSYNTH_API int synth_count_audio_groups (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_count_audio_groups" sys optional synth_count_audio_groups 

	// FLUIDSYNTH_API int synth_count_effects_channels (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_count_effects_channels" sys optional synth_count_effects_channels 

	// FLUIDSYNTH_API int synth_count_effects_groups (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_count_effects_groups" sys optional synth_count_effects_groups 

	// FLUID_DEPRECATED FLUIDSYNTH_API void synth_set_sample_rate (fluid_synth_t * synth,float sample_rate)
	 ( synth_t*:_synth Real:_sample_rate ) "libfluidsynth.so" "fluid_synth_set_sample_rate" sys optional synth_set_sample_rate 

	// FLUIDSYNTH_API void synth_set_gain (fluid_synth_t * synth,float gain)
	 ( synth_t*:_synth Real:_gain ) "libfluidsynth.so" "fluid_synth_set_gain" sys optional synth_set_gain 

	// FLUIDSYNTH_API float synth_get_gain (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Real ) "libfluidsynth.so" "fluid_synth_get_gain" sys optional synth_get_gain 

	// FLUIDSYNTH_API int synth_set_polyphony (fluid_synth_t * synth,int polyphony)
	 ( synth_t*:_synth Z32:_polyphony  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_polyphony" sys optional synth_set_polyphony 

	// FLUIDSYNTH_API int synth_get_polyphony (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_polyphony" sys optional synth_get_polyphony 

	// FLUIDSYNTH_API int synth_get_active_voice_count (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_active_voice_count" sys optional synth_get_active_voice_count 

	// FLUIDSYNTH_API int synth_get_internal_bufsize (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_internal_bufsize" sys optional synth_get_internal_bufsize 

	// FLUIDSYNTH_API int synth_set_interp_method (fluid_synth_t * synth,int chan,int interp_method)
	 ( synth_t*:_synth Z32:_chan Z32:_interp_method  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_interp_method" sys optional synth_set_interp_method 

	// FLUIDSYNTH_API int synth_add_default_mod (fluid_synth_t * synth,const fluid_mod_t * mod,int mode)
	 ( synth_t*:_synth constfluid_mod_t*:_mod Z32:_mode  -> Z32 ) "libfluidsynth.so" "fluid_synth_add_default_mod" sys optional synth_add_default_mod 

	// FLUIDSYNTH_API int synth_remove_default_mod (fluid_synth_t * synth,const fluid_mod_t * mod)
	 ( synth_t*:_synth constfluid_mod_t*:_mod  -> Z32 ) "libfluidsynth.so" "fluid_synth_remove_default_mod" sys optional synth_remove_default_mod 

	// FLUIDSYNTH_API int synth_activate_key_tuning (fluid_synth_t * synth,int bank,int prog,const char * name,const double * pitch,int apply)
	 ( synth_t*:_synth Z32:_bank Z32:_prog String:_name constdouble*:_pitch Z32:_apply  -> Z32 ) "libfluidsynth.so" "fluid_synth_activate_key_tuning" sys optional synth_activate_key_tuning 

	// FLUIDSYNTH_API int synth_activate_octave_tuning (fluid_synth_t * synth,int bank,int prog,const char * name,const double * pitch,int apply)
	 ( synth_t*:_synth Z32:_bank Z32:_prog String:_name constdouble*:_pitch Z32:_apply  -> Z32 ) "libfluidsynth.so" "fluid_synth_activate_octave_tuning" sys optional synth_activate_octave_tuning 

	// FLUIDSYNTH_API int synth_tune_notes (fluid_synth_t * synth,int bank,int prog,int len,const int * keys,const double * pitch,int apply)
	 ( synth_t*:_synth Z32:_bank Z32:_prog Z32:_len constint*:_keys constdouble*:_pitch Z32:_apply  -> Z32 ) "libfluidsynth.so" "fluid_synth_tune_notes" sys optional synth_tune_notes 

	// FLUIDSYNTH_API int synth_activate_tuning (fluid_synth_t * synth,int chan,int bank,int prog,int apply)
	 ( synth_t*:_synth Z32:_chan Z32:_bank Z32:_prog Z32:_apply  -> Z32 ) "libfluidsynth.so" "fluid_synth_activate_tuning" sys optional synth_activate_tuning 

	// FLUIDSYNTH_API int synth_deactivate_tuning (fluid_synth_t * synth,int chan,int apply)
	 ( synth_t*:_synth Z32:_chan Z32:_apply  -> Z32 ) "libfluidsynth.so" "fluid_synth_deactivate_tuning" sys optional synth_deactivate_tuning 

	// FLUIDSYNTH_API void synth_tuning_iteration_start (fluid_synth_t * synth)
	 ( synth_t*:_synth ) "libfluidsynth.so" "fluid_synth_tuning_iteration_start" sys optional synth_tuning_iteration_start 

	// FLUIDSYNTH_API int synth_tuning_iteration_next (fluid_synth_t * synth,int * bank,int * prog)
	 ( synth_t*:_synth Z32*:_bank Z32*:_prog  -> Z32 ) "libfluidsynth.so" "fluid_synth_tuning_iteration_next" sys optional synth_tuning_iteration_next 

	// FLUIDSYNTH_API int synth_tuning_dump (fluid_synth_t * synth,int bank,int prog,char * name,int len,double * pitch)
	 ( synth_t*:_synth Z32:_bank Z32:_prog String:_name Z32:_len Double32*:_pitch  -> Z32 ) "libfluidsynth.so" "fluid_synth_tuning_dump" sys optional synth_tuning_dump 

	// FLUIDSYNTH_API int synth_write_s16 (fluid_synth_t * synth,int len,void * lout,int loff,int lincr,void * rout,int roff,int rincr)
	 ( synth_t*:_synth Z32:_len Any&:_lout Z32:_loff Z32:_lincr Any&:_rout Z32:_roff Z32:_rincr  -> Z32 ) "libfluidsynth.so" "fluid_synth_write_s16" sys optional synth_write_s16 

	// FLUIDSYNTH_API int synth_write_float (fluid_synth_t * synth,int len,void * lout,int loff,int lincr,void * rout,int roff,int rincr)
	 ( synth_t*:_synth Z32:_len Any&:_lout Z32:_loff Z32:_lincr Any&:_rout Z32:_roff Z32:_rincr  -> Z32 ) "libfluidsynth.so" "fluid_synth_write_float" sys optional synth_write_float 

	// FLUID_DEPRECATED FLUIDSYNTH_API int synth_nwrite_float (fluid_synth_t * synth,int len,float ** left,float ** right,float ** fx_left,float ** fx_right)
	 ( synth_t*:_synth Z32:_len Real**:_left Real**:_right Real**:_fx_left Real**:_fx_right  -> Z32 ) "libfluidsynth.so" "fluid_synth_nwrite_float" sys optional synth_nwrite_float 

	// FLUIDSYNTH_API int synth_process (fluid_synth_t * synth,int len,int nfx,float * fx[],int nout,float * out[])
	 ( synth_t*:_synth Z32:_len Z32:_nfx [Real*]:_fx Z32:_nout [Real*]:_out  -> Z32 ) "libfluidsynth.so" "fluid_synth_process" sys optional synth_process 

	// FLUIDSYNTH_API int synth_set_custom_filter (fluid_synth_t *,int type,int flags)
	 ( synth_t*:___anonbcfda8e5010d Z32:_type Z32:_flags  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_custom_filter" sys optional synth_set_custom_filter 

	// FLUIDSYNTH_API int synth_set_channel_type (fluid_synth_t * synth,int chan,int type)
	 ( synth_t*:_synth Z32:_chan Z32:_type  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_channel_type" sys optional synth_set_channel_type 

	// FLUIDSYNTH_API int synth_reset_basic_channel (fluid_synth_t * synth,int chan)
	 ( synth_t*:_synth Z32:_chan  -> Z32 ) "libfluidsynth.so" "fluid_synth_reset_basic_channel" sys optional synth_reset_basic_channel 

	// FLUIDSYNTH_API int synth_get_basic_channel (fluid_synth_t * synth,int chan,int * basic_chan_out,int * mode_chan_out,int * basic_val_out)
	 ( synth_t*:_synth Z32:_chan Z32*:_basic_chan_out Z32*:_mode_chan_out Z32*:_basic_val_out  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_basic_channel" sys optional synth_get_basic_channel 

	// FLUIDSYNTH_API int synth_set_basic_channel (fluid_synth_t * synth,int chan,int mode,int val)
	 ( synth_t*:_synth Z32:_chan Z32:_mode Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_basic_channel" sys optional synth_set_basic_channel 

	// FLUIDSYNTH_API int synth_set_legato_mode (fluid_synth_t * synth,int chan,int legatomode)
	 ( synth_t*:_synth Z32:_chan Z32:_legatomode  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_legato_mode" sys optional synth_set_legato_mode 

	// FLUIDSYNTH_API int synth_get_legato_mode (fluid_synth_t * synth,int chan,int * legatomode)
	 ( synth_t*:_synth Z32:_chan Z32*:_legatomode  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_legato_mode" sys optional synth_get_legato_mode 

	// FLUIDSYNTH_API int synth_set_portamento_mode (fluid_synth_t * synth,int chan,int portamentomode)
	 ( synth_t*:_synth Z32:_chan Z32:_portamentomode  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_portamento_mode" sys optional synth_set_portamento_mode 

	// FLUIDSYNTH_API int synth_get_portamento_mode (fluid_synth_t * synth,int chan,int * portamentomode)
	 ( synth_t*:_synth Z32:_chan Z32*:_portamentomode  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_portamento_mode" sys optional synth_get_portamento_mode 

	// FLUIDSYNTH_API int synth_set_breath_mode (fluid_synth_t * synth,int chan,int breathmode)
	 ( synth_t*:_synth Z32:_chan Z32:_breathmode  -> Z32 ) "libfluidsynth.so" "fluid_synth_set_breath_mode" sys optional synth_set_breath_mode 

	// FLUIDSYNTH_API int synth_get_breath_mode (fluid_synth_t * synth,int chan,int * breathmode)
	 ( synth_t*:_synth Z32:_chan Z32*:_breathmode  -> Z32 ) "libfluidsynth.so" "fluid_synth_get_breath_mode" sys optional synth_get_breath_mode 

	// FLUIDSYNTH_API fluid_settings_t * synth_get_settings (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> settings_t* ) "libfluidsynth.so" "fluid_synth_get_settings" sys optional synth_get_settings 

	// FLUIDSYNTH_API void synth_add_sfloader (fluid_synth_t * synth,fluid_sfloader_t * loader)
	 ( synth_t*:_synth sfloader_t*:_loader ) "libfluidsynth.so" "fluid_synth_add_sfloader" sys optional synth_add_sfloader 

	// FLUIDSYNTH_API fluid_preset_t * synth_get_channel_preset (fluid_synth_t * synth,int chan)
	 ( synth_t*:_synth Z32:_chan  -> preset_t* ) "libfluidsynth.so" "fluid_synth_get_channel_preset" sys optional synth_get_channel_preset 

	// FLUIDSYNTH_API int synth_handle_midi_event (void * data,fluid_midi_event_t * event)
	 ( Any&:_data midi_event_t*:_event  -> Z32 ) "libfluidsynth.so" "fluid_synth_handle_midi_event" sys optional synth_handle_midi_event 

	// FLUIDSYNTH_API int synth_pin_preset (fluid_synth_t * synth,int sfont_id,int bank_num,int preset_num)
	 ( synth_t*:_synth Z32:_sfont_id Z32:_bank_num Z32:_preset_num  -> Z32 ) "libfluidsynth.so" "fluid_synth_pin_preset" sys optional synth_pin_preset 

	// FLUIDSYNTH_API int synth_unpin_preset (fluid_synth_t * synth,int sfont_id,int bank_num,int preset_num)
	 ( synth_t*:_synth Z32:_sfont_id Z32:_bank_num Z32:_preset_num  -> Z32 ) "libfluidsynth.so" "fluid_synth_unpin_preset" sys optional synth_unpin_preset 

	// FLUIDSYNTH_API fluid_ladspa_fx_t * synth_get_ladspa_fx (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> ladspa_fx_t* ) "libfluidsynth.so" "fluid_synth_get_ladspa_fx" sys optional synth_get_ladspa_fx 

	// FLUIDSYNTH_API fluid_istream_t get_stdin (void)
	 (  -> Z32 ) "libfluidsynth.so" "fluid_get_stdin" sys optional get_stdin 

	// FLUIDSYNTH_API fluid_ostream_t get_stdout (void)
	 (  -> Z32 ) "libfluidsynth.so" "fluid_get_stdout" sys optional get_stdout 

	// FLUIDSYNTH_API char * get_userconf (char * buf,int len)
	 ( String:_buf Z32:_len  -> String ) "libfluidsynth.so" "fluid_get_userconf" sys optional get_userconf 

	// FLUIDSYNTH_API char * get_sysconf (char * buf,int len)
	 ( String:_buf Z32:_len  -> String ) "libfluidsynth.so" "fluid_get_sysconf" sys optional get_sysconf 

	// FLUIDSYNTH_API fluid_cmd_handler_t * new_fluid_cmd_handler (fluid_synth_t * synth,fluid_midi_router_t * router)
	 ( synth_t*:_synth midi_router_t*:_router  -> cmd_handler_t* ) "libfluidsynth.so" "new_fluid_cmd_handler" sys optional new_fluid_cmd_handler 

	// FLUIDSYNTH_API fluid_cmd_handler_t * new_fluid_cmd_handler2 (fluid_settings_t * settings,fluid_synth_t * synth,fluid_midi_router_t * router,fluid_player_t * player)
	 ( settings_t*:_settings synth_t*:_synth midi_router_t*:_router player_t*:_player  -> cmd_handler_t* ) "libfluidsynth.so" "new_fluid_cmd_handler2" sys optional new_fluid_cmd_handler2 

	// FLUIDSYNTH_API void delete_fluid_cmd_handler (fluid_cmd_handler_t * handler)
	 ( cmd_handler_t*:_handler ) "libfluidsynth.so" "delete_fluid_cmd_handler" sys optional delete_fluid_cmd_handler 

	// FLUIDSYNTH_API void cmd_handler_set_synth (fluid_cmd_handler_t * handler,fluid_synth_t * synth)
	 ( cmd_handler_t*:_handler synth_t*:_synth ) "libfluidsynth.so" "fluid_cmd_handler_set_synth" sys optional cmd_handler_set_synth 

	// FLUIDSYNTH_API int command (fluid_cmd_handler_t * handler,const char * cmd,fluid_ostream_t out)
	 ( cmd_handler_t*:_handler String:_cmd Z32:_out  -> Z32 ) "libfluidsynth.so" "fluid_command" sys optional command 

	// FLUIDSYNTH_API int source (fluid_cmd_handler_t * handler,const char * filename)
	 ( cmd_handler_t*:_handler String:_filename  -> Z32 ) "libfluidsynth.so" "fluid_source" sys optional source 

	// FLUIDSYNTH_API fluid_shell_t * new_fluid_shell (fluid_settings_t * settings,fluid_cmd_handler_t * handler,fluid_istream_t in,fluid_ostream_t out,int thread)
	 ( settings_t*:_settings cmd_handler_t*:_handler Z32:_in Z32:_out Z32:_thread  -> shell_t* ) "libfluidsynth.so" "new_fluid_shell" sys optional new_fluid_shell 

	// FLUIDSYNTH_API void usershell (fluid_settings_t * settings,fluid_cmd_handler_t * handler)
	 ( settings_t*:_settings cmd_handler_t*:_handler ) "libfluidsynth.so" "fluid_usershell" sys optional usershell 

	// FLUIDSYNTH_API void delete_fluid_shell (fluid_shell_t * shell)
	 ( shell_t*:_shell ) "libfluidsynth.so" "delete_fluid_shell" sys optional delete_fluid_shell 

	// FLUIDSYNTH_API fluid_server_t * new_fluid_server (fluid_settings_t * settings,fluid_synth_t * synth,fluid_midi_router_t * router)
	 ( settings_t*:_settings synth_t*:_synth midi_router_t*:_router  -> server_t* ) "libfluidsynth.so" "new_fluid_server" sys optional new_fluid_server 

	// FLUIDSYNTH_API fluid_server_t * new_fluid_server2 (fluid_settings_t * settings,fluid_synth_t * synth,fluid_midi_router_t * router,fluid_player_t * player)
	 ( settings_t*:_settings synth_t*:_synth midi_router_t*:_router player_t*:_player  -> server_t* ) "libfluidsynth.so" "new_fluid_server2" sys optional new_fluid_server2 

	// FLUIDSYNTH_API void delete_fluid_server (fluid_server_t * server)
	 ( server_t*:_server ) "libfluidsynth.so" "delete_fluid_server" sys optional delete_fluid_server 

	// FLUIDSYNTH_API int server_join (fluid_server_t * server)
	 ( server_t*:_server  -> Z32 ) "libfluidsynth.so" "fluid_server_join" sys optional server_join 

	// FLUIDSYNTH_API fluid_sfloader_t * new_fluid_sfloader (fluid_sfloader_load_t load,fluid_sfloader_free_t free)
	 ( Any*:_load Any*:_free  -> sfloader_t* ) "libfluidsynth.so" "new_fluid_sfloader" sys optional new_fluid_sfloader 

	// FLUIDSYNTH_API void delete_fluid_sfloader (fluid_sfloader_t * loader)
	 ( sfloader_t*:_loader ) "libfluidsynth.so" "delete_fluid_sfloader" sys optional delete_fluid_sfloader 

	// FLUIDSYNTH_API fluid_sfloader_t * new_fluid_defsfloader (fluid_settings_t * settings)
	 ( settings_t*:_settings  -> sfloader_t* ) "libfluidsynth.so" "new_fluid_defsfloader" sys optional new_fluid_defsfloader 

	// FLUIDSYNTH_API int sfloader_set_callbacks (fluid_sfloader_t * loader,fluid_sfloader_callback_open_t open,fluid_sfloader_callback_read_t read,fluid_sfloader_callback_seek_t seek,fluid_sfloader_callback_tell_t tell,fluid_sfloader_callback_close_t close)
	 ( sfloader_t*:_loader Any*:_open Any*:_read Any*:_seek Any*:_tell Any*:_close  -> Z32 ) "libfluidsynth.so" "fluid_sfloader_set_callbacks" sys optional sfloader_set_callbacks 

	// FLUIDSYNTH_API int sfloader_set_data (fluid_sfloader_t * loader,void * data)
	 ( sfloader_t*:_loader Any&:_data  -> Z32 ) "libfluidsynth.so" "fluid_sfloader_set_data" sys optional sfloader_set_data 

	// FLUIDSYNTH_API void * sfloader_get_data (fluid_sfloader_t * loader)
	 ( sfloader_t*:_loader  -> Any& ) "libfluidsynth.so" "fluid_sfloader_get_data" sys optional sfloader_get_data 

	// FLUIDSYNTH_API fluid_sfont_t * new_fluid_sfont (fluid_sfont_get_name_t get_name,fluid_sfont_get_preset_t get_preset,fluid_sfont_iteration_start_t iter_start,fluid_sfont_iteration_next_t iter_next,fluid_sfont_free_t free)
	 ( Any*:_get_name Any*:_get_preset Any*:_iter_start Any*:_iter_next Any*:_free  -> sfont_t* ) "libfluidsynth.so" "new_fluid_sfont" sys optional new_fluid_sfont 

	// FLUIDSYNTH_API int delete_fluid_sfont (fluid_sfont_t * sfont)
	 ( sfont_t*:_sfont  -> Z32 ) "libfluidsynth.so" "delete_fluid_sfont" sys optional delete_fluid_sfont 

	// FLUIDSYNTH_API int sfont_set_data (fluid_sfont_t * sfont,void * data)
	 ( sfont_t*:_sfont Any&:_data  -> Z32 ) "libfluidsynth.so" "fluid_sfont_set_data" sys optional sfont_set_data 

	// FLUIDSYNTH_API void * sfont_get_data (fluid_sfont_t * sfont)
	 ( sfont_t*:_sfont  -> Any& ) "libfluidsynth.so" "fluid_sfont_get_data" sys optional sfont_get_data 

	// FLUIDSYNTH_API int sfont_get_id (fluid_sfont_t * sfont)
	 ( sfont_t*:_sfont  -> Z32 ) "libfluidsynth.so" "fluid_sfont_get_id" sys optional sfont_get_id 

	// FLUIDSYNTH_API const char * sfont_get_name (fluid_sfont_t * sfont)
	 ( sfont_t*:_sfont  -> String ) "libfluidsynth.so" "fluid_sfont_get_name" sys optional sfont_get_name 

	// FLUIDSYNTH_API fluid_preset_t * sfont_get_preset (fluid_sfont_t * sfont,int bank,int prenum)
	 ( sfont_t*:_sfont Z32:_bank Z32:_prenum  -> preset_t* ) "libfluidsynth.so" "fluid_sfont_get_preset" sys optional sfont_get_preset 

	// FLUIDSYNTH_API void sfont_iteration_start (fluid_sfont_t * sfont)
	 ( sfont_t*:_sfont ) "libfluidsynth.so" "fluid_sfont_iteration_start" sys optional sfont_iteration_start 

	// FLUIDSYNTH_API fluid_preset_t * sfont_iteration_next (fluid_sfont_t * sfont)
	 ( sfont_t*:_sfont  -> preset_t* ) "libfluidsynth.so" "fluid_sfont_iteration_next" sys optional sfont_iteration_next 

	// FLUIDSYNTH_API fluid_preset_t * new_fluid_preset (fluid_sfont_t * parent_sfont,fluid_preset_get_name_t get_name,fluid_preset_get_banknum_t get_bank,fluid_preset_get_num_t get_num,fluid_preset_noteon_t noteon,fluid_preset_free_t free)
	 ( sfont_t*:_parent_sfont Any*:_get_name Any*:_get_bank Any*:_get_num Any*:_noteon Any*:_free  -> preset_t* ) "libfluidsynth.so" "new_fluid_preset" sys optional new_fluid_preset 

	// FLUIDSYNTH_API void delete_fluid_preset (fluid_preset_t * preset)
	 ( preset_t*:_preset ) "libfluidsynth.so" "delete_fluid_preset" sys optional delete_fluid_preset 

	// FLUIDSYNTH_API int preset_set_data (fluid_preset_t * preset,void * data)
	 ( preset_t*:_preset Any&:_data  -> Z32 ) "libfluidsynth.so" "fluid_preset_set_data" sys optional preset_set_data 

	// FLUIDSYNTH_API void * preset_get_data (fluid_preset_t * preset)
	 ( preset_t*:_preset  -> Any& ) "libfluidsynth.so" "fluid_preset_get_data" sys optional preset_get_data 

	// FLUIDSYNTH_API const char * preset_get_name (fluid_preset_t * preset)
	 ( preset_t*:_preset  -> String ) "libfluidsynth.so" "fluid_preset_get_name" sys optional preset_get_name 

	// FLUIDSYNTH_API int preset_get_banknum (fluid_preset_t * preset)
	 ( preset_t*:_preset  -> Z32 ) "libfluidsynth.so" "fluid_preset_get_banknum" sys optional preset_get_banknum 

	// FLUIDSYNTH_API int preset_get_num (fluid_preset_t * preset)
	 ( preset_t*:_preset  -> Z32 ) "libfluidsynth.so" "fluid_preset_get_num" sys optional preset_get_num 

	// FLUIDSYNTH_API fluid_sfont_t * preset_get_sfont (fluid_preset_t * preset)
	 ( preset_t*:_preset  -> sfont_t* ) "libfluidsynth.so" "fluid_preset_get_sfont" sys optional preset_get_sfont 

	// FLUIDSYNTH_API fluid_sample_t * new_fluid_sample (void)
	 (  -> sample_t* ) "libfluidsynth.so" "new_fluid_sample" sys optional new_fluid_sample 

	// FLUIDSYNTH_API void delete_fluid_sample (fluid_sample_t * sample)
	 ( sample_t*:_sample ) "libfluidsynth.so" "delete_fluid_sample" sys optional delete_fluid_sample 

	// FLUIDSYNTH_API size_t sample_sizeof (void)
	 (  -> Size32 ) "libfluidsynth.so" "fluid_sample_sizeof" sys optional sample_sizeof 

	// FLUIDSYNTH_API int sample_set_name (fluid_sample_t * sample,const char * name)
	 ( sample_t*:_sample String:_name  -> Z32 ) "libfluidsynth.so" "fluid_sample_set_name" sys optional sample_set_name 

	// FLUIDSYNTH_API int sample_set_sound_data (fluid_sample_t * sample,short * data,char * data24,unsigned int nbframes,unsigned int sample_rate,short copy_data)
	 ( sample_t*:_sample ZL16*:_data String:_data24 Z32:_nbframes Z32:_sample_rate ZL16:_copy_data  -> Z32 ) "libfluidsynth.so" "fluid_sample_set_sound_data" sys optional sample_set_sound_data 

	// FLUIDSYNTH_API int sample_set_loop (fluid_sample_t * sample,unsigned int loop_start,unsigned int loop_end)
	 ( sample_t*:_sample Z32:_loop_start Z32:_loop_end  -> Z32 ) "libfluidsynth.so" "fluid_sample_set_loop" sys optional sample_set_loop 

	// FLUIDSYNTH_API int sample_set_pitch (fluid_sample_t * sample,int root_key,int fine_tune)
	 ( sample_t*:_sample Z32:_root_key Z32:_fine_tune  -> Z32 ) "libfluidsynth.so" "fluid_sample_set_pitch" sys optional sample_set_pitch 

	// FLUIDSYNTH_API fluid_audio_driver_t * new_fluid_audio_driver (fluid_settings_t * settings,fluid_synth_t * synth)
	 ( settings_t*:_settings synth_t*:_synth  -> audio_driver_t* ) "libfluidsynth.so" "new_fluid_audio_driver" sys optional ^new_fluid_audio_driver 

	// FLUIDSYNTH_API fluid_audio_driver_t * new_fluid_audio_driver2 (fluid_settings_t * settings,fluid_audio_func_t func,void * data)
	 ( settings_t*:_settings Any*:_func Any&:_data  -> audio_driver_t* ) "libfluidsynth.so" "new_fluid_audio_driver2" sys optional new_fluid_audio_driver2 

	// FLUIDSYNTH_API void delete_fluid_audio_driver (fluid_audio_driver_t * driver)
	 ( audio_driver_t*:_driver ) "libfluidsynth.so" "delete_fluid_audio_driver" sys optional delete_fluid_audio_driver 

	// FLUIDSYNTH_API int audio_driver_register (const char ** adrivers)
	 ( String*:_adrivers  -> Z32 ) "libfluidsynth.so" "fluid_audio_driver_register" sys optional audio_driver_register 

	// FLUIDSYNTH_API fluid_file_renderer_t * new_fluid_file_renderer (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> file_renderer_t* ) "libfluidsynth.so" "new_fluid_file_renderer" sys optional new_fluid_file_renderer 

	// FLUIDSYNTH_API void delete_fluid_file_renderer (fluid_file_renderer_t * dev)
	 ( file_renderer_t*:_dev ) "libfluidsynth.so" "delete_fluid_file_renderer" sys optional delete_fluid_file_renderer 

	// FLUIDSYNTH_API int file_renderer_process_block (fluid_file_renderer_t * dev)
	 ( file_renderer_t*:_dev  -> Z32 ) "libfluidsynth.so" "fluid_file_renderer_process_block" sys optional file_renderer_process_block 

	// FLUIDSYNTH_API int file_set_encoding_quality (fluid_file_renderer_t * dev,double q)
	 ( file_renderer_t*:_dev Double32:_q  -> Z32 ) "libfluidsynth.so" "fluid_file_set_encoding_quality" sys optional file_set_encoding_quality 

	// FLUIDSYNTH_API fluid_event_t * new_fluid_event (void)
	 (  -> event_t* ) "libfluidsynth.so" "new_fluid_event" sys optional new_fluid_event 

	// FLUIDSYNTH_API void delete_fluid_event (fluid_event_t * evt)
	 ( event_t*:_evt ) "libfluidsynth.so" "delete_fluid_event" sys optional delete_fluid_event 

	// FLUIDSYNTH_API void event_set_source (fluid_event_t * evt,fluid_seq_id_t src)
	 ( event_t*:_evt ZL16:_src ) "libfluidsynth.so" "fluid_event_set_source" sys optional event_set_source 

	// FLUIDSYNTH_API void event_set_dest (fluid_event_t * evt,fluid_seq_id_t dest)
	 ( event_t*:_evt ZL16:_dest ) "libfluidsynth.so" "fluid_event_set_dest" sys optional event_set_dest 

	// FLUIDSYNTH_API void event_timer (fluid_event_t * evt,void * data)
	 ( event_t*:_evt Any&:_data ) "libfluidsynth.so" "fluid_event_timer" sys optional event_timer 

	// FLUIDSYNTH_API void event_note (fluid_event_t * evt,int channel,short key,short vel,unsigned int duration)
	 ( event_t*:_evt Z32:_channel ZL16:_key ZL16:_vel Z32:_duration ) "libfluidsynth.so" "fluid_event_note" sys optional event_note 

	// FLUIDSYNTH_API void event_noteon (fluid_event_t * evt,int channel,short key,short vel)
	 ( event_t*:_evt Z32:_channel ZL16:_key ZL16:_vel ) "libfluidsynth.so" "fluid_event_noteon" sys optional event_noteon 

	// FLUIDSYNTH_API void event_noteoff (fluid_event_t * evt,int channel,short key)
	 ( event_t*:_evt Z32:_channel ZL16:_key ) "libfluidsynth.so" "fluid_event_noteoff" sys optional event_noteoff 

	// FLUIDSYNTH_API void event_all_sounds_off (fluid_event_t * evt,int channel)
	 ( event_t*:_evt Z32:_channel ) "libfluidsynth.so" "fluid_event_all_sounds_off" sys optional event_all_sounds_off 

	// FLUIDSYNTH_API void event_all_notes_off (fluid_event_t * evt,int channel)
	 ( event_t*:_evt Z32:_channel ) "libfluidsynth.so" "fluid_event_all_notes_off" sys optional event_all_notes_off 

	// FLUIDSYNTH_API void event_bank_select (fluid_event_t * evt,int channel,short bank_num)
	 ( event_t*:_evt Z32:_channel ZL16:_bank_num ) "libfluidsynth.so" "fluid_event_bank_select" sys optional event_bank_select 

	// FLUIDSYNTH_API void event_program_change (fluid_event_t * evt,int channel,int preset_num)
	 ( event_t*:_evt Z32:_channel Z32:_preset_num ) "libfluidsynth.so" "fluid_event_program_change" sys optional event_program_change 

	// FLUIDSYNTH_API void event_program_select (fluid_event_t * evt,int channel,unsigned int sfont_id,short bank_num,short preset_num)
	 ( event_t*:_evt Z32:_channel Z32:_sfont_id ZL16:_bank_num ZL16:_preset_num ) "libfluidsynth.so" "fluid_event_program_select" sys optional event_program_select 

	// FLUIDSYNTH_API void event_control_change (fluid_event_t * evt,int channel,short control,int val)
	 ( event_t*:_evt Z32:_channel ZL16:_control Z32:_val ) "libfluidsynth.so" "fluid_event_control_change" sys optional event_control_change 

	// FLUIDSYNTH_API void event_pitch_bend (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_pitch_bend" sys optional event_pitch_bend 

	// FLUIDSYNTH_API void event_pitch_wheelsens (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_pitch_wheelsens" sys optional event_pitch_wheelsens 

	// FLUIDSYNTH_API void event_modulation (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_modulation" sys optional event_modulation 

	// FLUIDSYNTH_API void event_sustain (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_sustain" sys optional event_sustain 

	// FLUIDSYNTH_API void event_pan (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_pan" sys optional event_pan 

	// FLUIDSYNTH_API void event_volume (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_volume" sys optional event_volume 

	// FLUIDSYNTH_API void event_reverb_send (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_reverb_send" sys optional event_reverb_send 

	// FLUIDSYNTH_API void event_chorus_send (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_chorus_send" sys optional event_chorus_send 

	// FLUIDSYNTH_API void event_key_pressure (fluid_event_t * evt,int channel,short key,int val)
	 ( event_t*:_evt Z32:_channel ZL16:_key Z32:_val ) "libfluidsynth.so" "fluid_event_key_pressure" sys optional event_key_pressure 

	// FLUIDSYNTH_API void event_channel_pressure (fluid_event_t * evt,int channel,int val)
	 ( event_t*:_evt Z32:_channel Z32:_val ) "libfluidsynth.so" "fluid_event_channel_pressure" sys optional event_channel_pressure 

	// FLUIDSYNTH_API void event_system_reset (fluid_event_t * evt)
	 ( event_t*:_evt ) "libfluidsynth.so" "fluid_event_system_reset" sys optional event_system_reset 

	// FLUIDSYNTH_API void event_unregistering (fluid_event_t * evt)
	 ( event_t*:_evt ) "libfluidsynth.so" "fluid_event_unregistering" sys optional event_unregistering 

	// FLUIDSYNTH_API void event_scale (fluid_event_t * evt,double new_scale)
	 ( event_t*:_evt Double32:_new_scale ) "libfluidsynth.so" "fluid_event_scale" sys optional event_scale 

	// FLUIDSYNTH_API int event_from_midi_event (fluid_event_t *,const fluid_midi_event_t *)
	 ( event_t*:___anonbcfda8e5030d constfluid_midi_event_t*:___anonbcfda8e5040d  -> Z32 ) "libfluidsynth.so" "fluid_event_from_midi_event" sys optional event_from_midi_event 

	// FLUIDSYNTH_API int event_get_type (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_type" sys optional event_get_type 

	// FLUIDSYNTH_API fluid_seq_id_t event_get_source (fluid_event_t * evt)
	 ( event_t*:_evt  -> ZL16 ) "libfluidsynth.so" "fluid_event_get_source" sys optional event_get_source 

	// FLUIDSYNTH_API fluid_seq_id_t event_get_dest (fluid_event_t * evt)
	 ( event_t*:_evt  -> ZL16 ) "libfluidsynth.so" "fluid_event_get_dest" sys optional event_get_dest 

	// FLUIDSYNTH_API int event_get_channel (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_channel" sys optional event_get_channel 

	// FLUIDSYNTH_API short event_get_key (fluid_event_t * evt)
	 ( event_t*:_evt  -> ZL16 ) "libfluidsynth.so" "fluid_event_get_key" sys optional event_get_key 

	// FLUIDSYNTH_API short event_get_velocity (fluid_event_t * evt)
	 ( event_t*:_evt  -> ZL16 ) "libfluidsynth.so" "fluid_event_get_velocity" sys optional event_get_velocity 

	// FLUIDSYNTH_API short event_get_control (fluid_event_t * evt)
	 ( event_t*:_evt  -> ZL16 ) "libfluidsynth.so" "fluid_event_get_control" sys optional event_get_control 

	// FLUIDSYNTH_API int event_get_value (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_value" sys optional event_get_value 

	// FLUIDSYNTH_API int event_get_program (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_program" sys optional event_get_program 

	// FLUIDSYNTH_API void * event_get_data (fluid_event_t * evt)
	 ( event_t*:_evt  -> Any& ) "libfluidsynth.so" "fluid_event_get_data" sys optional event_get_data 

	// FLUIDSYNTH_API unsigned int event_get_duration (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_duration" sys optional event_get_duration 

	// FLUIDSYNTH_API short event_get_bank (fluid_event_t * evt)
	 ( event_t*:_evt  -> ZL16 ) "libfluidsynth.so" "fluid_event_get_bank" sys optional event_get_bank 

	// FLUIDSYNTH_API int event_get_pitch (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_pitch" sys optional event_get_pitch 

	// FLUIDSYNTH_API double event_get_scale (fluid_event_t * evt)
	 ( event_t*:_evt  -> Double32 ) "libfluidsynth.so" "fluid_event_get_scale" sys optional event_get_scale 

	// FLUIDSYNTH_API unsigned int event_get_sfont_id (fluid_event_t * evt)
	 ( event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_event_get_sfont_id" sys optional event_get_sfont_id 

	// FLUIDSYNTH_API fluid_midi_event_t * new_fluid_midi_event (void)
	 (  -> midi_event_t* ) "libfluidsynth.so" "new_fluid_midi_event" sys optional new_fluid_midi_event 

	// FLUIDSYNTH_API void delete_fluid_midi_event (fluid_midi_event_t * event)
	 ( midi_event_t*:_event ) "libfluidsynth.so" "delete_fluid_midi_event" sys optional delete_fluid_midi_event 

	// FLUIDSYNTH_API int midi_event_set_type (fluid_midi_event_t * evt,int type)
	 ( midi_event_t*:_evt Z32:_type  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_type" sys optional midi_event_set_type 

	// FLUIDSYNTH_API int midi_event_get_type (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_type" sys optional midi_event_get_type 

	// FLUIDSYNTH_API int midi_event_set_channel (fluid_midi_event_t * evt,int chan)
	 ( midi_event_t*:_evt Z32:_chan  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_channel" sys optional midi_event_set_channel 

	// FLUIDSYNTH_API int midi_event_get_channel (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_channel" sys optional midi_event_get_channel 

	// FLUIDSYNTH_API int midi_event_get_key (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_key" sys optional midi_event_get_key 

	// FLUIDSYNTH_API int midi_event_set_key (fluid_midi_event_t * evt,int key)
	 ( midi_event_t*:_evt Z32:_key  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_key" sys optional midi_event_set_key 

	// FLUIDSYNTH_API int midi_event_get_velocity (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_velocity" sys optional midi_event_get_velocity 

	// FLUIDSYNTH_API int midi_event_set_velocity (fluid_midi_event_t * evt,int vel)
	 ( midi_event_t*:_evt Z32:_vel  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_velocity" sys optional midi_event_set_velocity 

	// FLUIDSYNTH_API int midi_event_get_control (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_control" sys optional midi_event_get_control 

	// FLUIDSYNTH_API int midi_event_set_control (fluid_midi_event_t * evt,int ctrl)
	 ( midi_event_t*:_evt Z32:_ctrl  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_control" sys optional midi_event_set_control 

	// FLUIDSYNTH_API int midi_event_get_value (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_value" sys optional midi_event_get_value 

	// FLUIDSYNTH_API int midi_event_set_value (fluid_midi_event_t * evt,int val)
	 ( midi_event_t*:_evt Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_value" sys optional midi_event_set_value 

	// FLUIDSYNTH_API int midi_event_get_program (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_program" sys optional midi_event_get_program 

	// FLUIDSYNTH_API int midi_event_set_program (fluid_midi_event_t * evt,int val)
	 ( midi_event_t*:_evt Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_program" sys optional midi_event_set_program 

	// FLUIDSYNTH_API int midi_event_get_pitch (const fluid_midi_event_t * evt)
	 ( constfluid_midi_event_t*:_evt  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_pitch" sys optional midi_event_get_pitch 

	// FLUIDSYNTH_API int midi_event_set_pitch (fluid_midi_event_t * evt,int val)
	 ( midi_event_t*:_evt Z32:_val  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_pitch" sys optional midi_event_set_pitch 

	// FLUIDSYNTH_API int midi_event_set_sysex (fluid_midi_event_t * evt,void * data,int size,int dynamic)
	 ( midi_event_t*:_evt Any&:_data Z32:_size Z32:_dynamic  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_sysex" sys optional midi_event_set_sysex 

	// FLUIDSYNTH_API int midi_event_set_text (fluid_midi_event_t * evt,void * data,int size,int dynamic)
	 ( midi_event_t*:_evt Any&:_data Z32:_size Z32:_dynamic  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_text" sys optional midi_event_set_text 

	// FLUIDSYNTH_API int midi_event_get_text (fluid_midi_event_t * evt,void ** data,int * size)
	 ( midi_event_t*:_evt Any&*:_data Z32*:_size  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_text" sys optional midi_event_get_text 

	// FLUIDSYNTH_API int midi_event_set_lyrics (fluid_midi_event_t * evt,void * data,int size,int dynamic)
	 ( midi_event_t*:_evt Any&:_data Z32:_size Z32:_dynamic  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_set_lyrics" sys optional midi_event_set_lyrics 

	// FLUIDSYNTH_API int midi_event_get_lyrics (fluid_midi_event_t * evt,void ** data,int * size)
	 ( midi_event_t*:_evt Any&*:_data Z32*:_size  -> Z32 ) "libfluidsynth.so" "fluid_midi_event_get_lyrics" sys optional midi_event_get_lyrics 

	// FLUIDSYNTH_API fluid_midi_router_t * new_fluid_midi_router (fluid_settings_t * settings,handle_midi_event_func_t handler,void * event_handler_data)
	 ( settings_t*:_settings Any*:_handler Any&:_event_handler_data  -> midi_router_t* ) "libfluidsynth.so" "new_fluid_midi_router" sys optional new_fluid_midi_router 

	// FLUIDSYNTH_API void delete_fluid_midi_router (fluid_midi_router_t * handler)
	 ( midi_router_t*:_handler ) "libfluidsynth.so" "delete_fluid_midi_router" sys optional delete_fluid_midi_router 

	// FLUIDSYNTH_API int midi_router_set_default_rules (fluid_midi_router_t * router)
	 ( midi_router_t*:_router  -> Z32 ) "libfluidsynth.so" "fluid_midi_router_set_default_rules" sys optional midi_router_set_default_rules 

	// FLUIDSYNTH_API int midi_router_clear_rules (fluid_midi_router_t * router)
	 ( midi_router_t*:_router  -> Z32 ) "libfluidsynth.so" "fluid_midi_router_clear_rules" sys optional midi_router_clear_rules 

	// FLUIDSYNTH_API int midi_router_add_rule (fluid_midi_router_t * router,fluid_midi_router_rule_t * rule,int type)
	 ( midi_router_t*:_router midi_router_rule_t*:_rule Z32:_type  -> Z32 ) "libfluidsynth.so" "fluid_midi_router_add_rule" sys optional midi_router_add_rule 

	// FLUIDSYNTH_API fluid_midi_router_rule_t * new_fluid_midi_router_rule (void)
	 (  -> midi_router_rule_t* ) "libfluidsynth.so" "new_fluid_midi_router_rule" sys optional new_fluid_midi_router_rule 

	// FLUIDSYNTH_API void delete_fluid_midi_router_rule (fluid_midi_router_rule_t * rule)
	 ( midi_router_rule_t*:_rule ) "libfluidsynth.so" "delete_fluid_midi_router_rule" sys optional delete_fluid_midi_router_rule 

	// FLUIDSYNTH_API void midi_router_rule_set_chan (fluid_midi_router_rule_t * rule,int min,int max,float mul,int add)
	 ( midi_router_rule_t*:_rule Z32:_min Z32:_max Real:_mul Z32:_add ) "libfluidsynth.so" "fluid_midi_router_rule_set_chan" sys optional midi_router_rule_set_chan 

	// FLUIDSYNTH_API void midi_router_rule_set_param1 (fluid_midi_router_rule_t * rule,int min,int max,float mul,int add)
	 ( midi_router_rule_t*:_rule Z32:_min Z32:_max Real:_mul Z32:_add ) "libfluidsynth.so" "fluid_midi_router_rule_set_param1" sys optional midi_router_rule_set_param1 

	// FLUIDSYNTH_API void midi_router_rule_set_param2 (fluid_midi_router_rule_t * rule,int min,int max,float mul,int add)
	 ( midi_router_rule_t*:_rule Z32:_min Z32:_max Real:_mul Z32:_add ) "libfluidsynth.so" "fluid_midi_router_rule_set_param2" sys optional midi_router_rule_set_param2 

	// FLUIDSYNTH_API int midi_router_handle_midi_event (void * data,fluid_midi_event_t * event)
	 ( Any&:_data midi_event_t*:_event  -> Z32 ) "libfluidsynth.so" "fluid_midi_router_handle_midi_event" sys optional midi_router_handle_midi_event 

	// FLUIDSYNTH_API int midi_dump_prerouter (void * data,fluid_midi_event_t * event)
	 ( Any&:_data midi_event_t*:_event  -> Z32 ) "libfluidsynth.so" "fluid_midi_dump_prerouter" sys optional midi_dump_prerouter 

	// FLUIDSYNTH_API int midi_dump_postrouter (void * data,fluid_midi_event_t * event)
	 ( Any&:_data midi_event_t*:_event  -> Z32 ) "libfluidsynth.so" "fluid_midi_dump_postrouter" sys optional midi_dump_postrouter 

	// FLUIDSYNTH_API fluid_midi_driver_t * new_fluid_midi_driver (fluid_settings_t * settings,handle_midi_event_func_t handler,void * event_handler_data)
	 ( settings_t*:_settings Any*:_handler Any&:_event_handler_data  -> midi_driver_t* ) "libfluidsynth.so" "new_fluid_midi_driver" sys optional new_fluid_midi_driver 

	// FLUIDSYNTH_API void delete_fluid_midi_driver (fluid_midi_driver_t * driver)
	 ( midi_driver_t*:_driver ) "libfluidsynth.so" "delete_fluid_midi_driver" sys optional delete_fluid_midi_driver 

	// FLUIDSYNTH_API fluid_player_t * new_fluid_player (fluid_synth_t * synth)
	 ( synth_t*:_synth  -> player_t* ) "libfluidsynth.so" "new_fluid_player" sys optional new_fluid_player 

	// FLUIDSYNTH_API void delete_fluid_player (fluid_player_t * player)
	 ( player_t*:_player ) "libfluidsynth.so" "delete_fluid_player" sys optional delete_fluid_player 

	// FLUIDSYNTH_API int player_add (fluid_player_t * player,const char * midifile)
	 ( player_t*:_player String:_midifile  -> Z32 ) "libfluidsynth.so" "fluid_player_add" sys optional player_add 

	// FLUIDSYNTH_API int player_add_mem (fluid_player_t * player,const void * buffer,size_t len)
	 ( player_t*:_player Any&:_buffer Size32:_len  -> Z32 ) "libfluidsynth.so" "fluid_player_add_mem" sys optional player_add_mem 

	// FLUIDSYNTH_API int player_play (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_play" sys optional player_play 

	// FLUIDSYNTH_API int player_stop (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_stop" sys optional player_stop 

	// FLUIDSYNTH_API int player_join (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_join" sys optional player_join 

	// FLUIDSYNTH_API int player_set_loop (fluid_player_t * player,int loop)
	 ( player_t*:_player Z32:_loop  -> Z32 ) "libfluidsynth.so" "fluid_player_set_loop" sys optional player_set_loop 

	// FLUIDSYNTH_API int player_set_tempo (fluid_player_t * player,int tempo_type,double tempo)
	 ( player_t*:_player Z32:_tempo_type Double32:_tempo  -> Z32 ) "libfluidsynth.so" "fluid_player_set_tempo" sys optional player_set_tempo 

	// FLUID_DEPRECATED FLUIDSYNTH_API int player_set_midi_tempo (fluid_player_t * player,int tempo)
	 ( player_t*:_player Z32:_tempo  -> Z32 ) "libfluidsynth.so" "fluid_player_set_midi_tempo" sys optional player_set_midi_tempo 

	// FLUID_DEPRECATED FLUIDSYNTH_API int player_set_bpm (fluid_player_t * player,int bpm)
	 ( player_t*:_player Z32:_bpm  -> Z32 ) "libfluidsynth.so" "fluid_player_set_bpm" sys optional player_set_bpm 

	// FLUIDSYNTH_API int player_set_playback_callback (fluid_player_t * player,handle_midi_event_func_t handler,void * handler_data)
	 ( player_t*:_player Any*:_handler Any&:_handler_data  -> Z32 ) "libfluidsynth.so" "fluid_player_set_playback_callback" sys optional player_set_playback_callback 

	// FLUIDSYNTH_API int player_set_tick_callback (fluid_player_t * player,handle_midi_tick_func_t handler,void * handler_data)
	 ( player_t*:_player Any*:_handler Any&:_handler_data  -> Z32 ) "libfluidsynth.so" "fluid_player_set_tick_callback" sys optional player_set_tick_callback 

	// FLUIDSYNTH_API int player_get_status (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_get_status" sys optional player_get_status 

	// FLUIDSYNTH_API int player_get_current_tick (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_get_current_tick" sys optional player_get_current_tick 

	// FLUIDSYNTH_API int player_get_total_ticks (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_get_total_ticks" sys optional player_get_total_ticks 

	// FLUIDSYNTH_API int player_get_bpm (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_get_bpm" sys optional player_get_bpm 

	// FLUIDSYNTH_API int player_get_division (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_get_division" sys optional player_get_division 

	// FLUIDSYNTH_API int player_get_midi_tempo (fluid_player_t * player)
	 ( player_t*:_player  -> Z32 ) "libfluidsynth.so" "fluid_player_get_midi_tempo" sys optional player_get_midi_tempo 

	// FLUIDSYNTH_API int player_seek (fluid_player_t * player,int ticks)
	 ( player_t*:_player Z32:_ticks  -> Z32 ) "libfluidsynth.so" "fluid_player_seek" sys optional player_seek 

	// FLUID_DEPRECATED FLUIDSYNTH_API fluid_sequencer_t * new_fluid_sequencer (void)
	 (  -> sequencer_t* ) "libfluidsynth.so" "new_fluid_sequencer" sys optional new_fluid_sequencer 

	// FLUIDSYNTH_API fluid_sequencer_t * new_fluid_sequencer2 (int use_system_timer)
	 ( Z32:_use_system_timer  -> sequencer_t* ) "libfluidsynth.so" "new_fluid_sequencer2" sys optional new_fluid_sequencer2 

	// FLUIDSYNTH_API void delete_fluid_sequencer (fluid_sequencer_t * seq)
	 ( sequencer_t*:_seq ) "libfluidsynth.so" "delete_fluid_sequencer" sys optional delete_fluid_sequencer 

	// FLUIDSYNTH_API int sequencer_get_use_system_timer (fluid_sequencer_t * seq)
	 ( sequencer_t*:_seq  -> Z32 ) "libfluidsynth.so" "fluid_sequencer_get_use_system_timer" sys optional sequencer_get_use_system_timer 

	// FLUIDSYNTH_API fluid_seq_id_t sequencer_register_client (fluid_sequencer_t * seq,const char * name,fluid_event_callback_t callback,void * data)
	 ( sequencer_t*:_seq String:_name Any*:_callback Any&:_data  -> ZL16 ) "libfluidsynth.so" "fluid_sequencer_register_client" sys optional sequencer_register_client 

	// FLUIDSYNTH_API void sequencer_unregister_client (fluid_sequencer_t * seq,fluid_seq_id_t id)
	 ( sequencer_t*:_seq ZL16:_id ) "libfluidsynth.so" "fluid_sequencer_unregister_client" sys optional sequencer_unregister_client 

	// FLUIDSYNTH_API int sequencer_count_clients (fluid_sequencer_t * seq)
	 ( sequencer_t*:_seq  -> Z32 ) "libfluidsynth.so" "fluid_sequencer_count_clients" sys optional sequencer_count_clients 

	// FLUIDSYNTH_API fluid_seq_id_t sequencer_get_client_id (fluid_sequencer_t * seq,int index)
	 ( sequencer_t*:_seq Z32:_index  -> ZL16 ) "libfluidsynth.so" "fluid_sequencer_get_client_id" sys optional sequencer_get_client_id 

	// FLUIDSYNTH_API char * sequencer_get_client_name (fluid_sequencer_t * seq,fluid_seq_id_t id)
	 ( sequencer_t*:_seq ZL16:_id  -> String ) "libfluidsynth.so" "fluid_sequencer_get_client_name" sys optional sequencer_get_client_name 

	// FLUIDSYNTH_API int sequencer_client_is_dest (fluid_sequencer_t * seq,fluid_seq_id_t id)
	 ( sequencer_t*:_seq ZL16:_id  -> Z32 ) "libfluidsynth.so" "fluid_sequencer_client_is_dest" sys optional sequencer_client_is_dest 

	// FLUIDSYNTH_API void sequencer_process (fluid_sequencer_t * seq,unsigned int msec)
	 ( sequencer_t*:_seq Z32:_msec ) "libfluidsynth.so" "fluid_sequencer_process" sys optional sequencer_process 

	// FLUIDSYNTH_API void sequencer_send_now (fluid_sequencer_t * seq,fluid_event_t * evt)
	 ( sequencer_t*:_seq event_t*:_evt ) "libfluidsynth.so" "fluid_sequencer_send_now" sys optional sequencer_send_now 

	// FLUIDSYNTH_API int sequencer_send_at (fluid_sequencer_t * seq,fluid_event_t * evt,unsigned int time,int absolute)
	 ( sequencer_t*:_seq event_t*:_evt Z32:_time Z32:_absolute  -> Z32 ) "libfluidsynth.so" "fluid_sequencer_send_at" sys optional sequencer_send_at 

	// FLUIDSYNTH_API void sequencer_remove_events (fluid_sequencer_t * seq,fluid_seq_id_t source,fluid_seq_id_t dest,int type)
	 ( sequencer_t*:_seq ZL16:_source ZL16:_dest Z32:_type ) "libfluidsynth.so" "fluid_sequencer_remove_events" sys optional sequencer_remove_events 

	// FLUIDSYNTH_API unsigned int sequencer_get_tick (fluid_sequencer_t * seq)
	 ( sequencer_t*:_seq  -> Z32 ) "libfluidsynth.so" "fluid_sequencer_get_tick" sys optional sequencer_get_tick 

	// FLUIDSYNTH_API void sequencer_set_time_scale (fluid_sequencer_t * seq,double scale)
	 ( sequencer_t*:_seq Double32:_scale ) "libfluidsynth.so" "fluid_sequencer_set_time_scale" sys optional sequencer_set_time_scale 

	// FLUIDSYNTH_API double sequencer_get_time_scale (fluid_sequencer_t * seq)
	 ( sequencer_t*:_seq  -> Double32 ) "libfluidsynth.so" "fluid_sequencer_get_time_scale" sys optional sequencer_get_time_scale 

	// FLUIDSYNTH_API fluid_seq_id_t sequencer_register_fluidsynth (fluid_sequencer_t * seq,fluid_synth_t * synth)
	 ( sequencer_t*:_seq synth_t*:_synth  -> ZL16 ) "libfluidsynth.so" "fluid_sequencer_register_fluidsynth" sys optional sequencer_register_fluidsynth 

	// FLUIDSYNTH_API int sequencer_add_midi_event_to_buffer (void * data,fluid_midi_event_t * event)
	 ( Any&:_data midi_event_t*:_event  -> Z32 ) "libfluidsynth.so" "fluid_sequencer_add_midi_event_to_buffer" sys optional sequencer_add_midi_event_to_buffer 

	// FLUIDSYNTH_API fluid_log_function_t set_log_function (int level,fluid_log_function_t fun,void * data)
	 ( Z32:_level Any*:_fun Any&:_data  -> Any* ) "libfluidsynth.so" "fluid_set_log_function" sys optional set_log_function 

	// FLUIDSYNTH_API void default_log_function (int level,const char * message,void * data)
	 ( Z32:_level String:_message Any&:_data ) "libfluidsynth.so" "fluid_default_log_function" sys optional default_log_function 

	// FLUIDSYNTH_API int log (int level,const char * fmt,...)
	 ( Z32:_level String:_fmt  -> Z32 ) "libfluidsynth.so" "fluid_log" sys optional log 

	// FLUIDSYNTH_API int is_soundfont (const char * filename)
	 ( String:_filename  -> Z32 ) "libfluidsynth.so" "fluid_is_soundfont" sys optional is_soundfont 

	// FLUIDSYNTH_API int is_midifile (const char * filename)
	 ( String:_filename  -> Z32 ) "libfluidsynth.so" "fluid_is_midifile" sys optional is_midifile 

	// FLUIDSYNTH_API void free (void * ptr)
	 ( Any&:_ptr ) "libfluidsynth.so" "fluid_free" sys optional free 

	// FLUIDSYNTH_API fluid_mod_t * new_fluid_mod (void)
	 (  -> mod_t* ) "libfluidsynth.so" "new_fluid_mod" sys optional new_fluid_mod 

	// FLUIDSYNTH_API void delete_fluid_mod (fluid_mod_t * mod)
	 ( mod_t*:_mod ) "libfluidsynth.so" "delete_fluid_mod" sys optional delete_fluid_mod 

	// FLUIDSYNTH_API size_t mod_sizeof (void)
	 (  -> Size32 ) "libfluidsynth.so" "fluid_mod_sizeof" sys optional mod_sizeof 

	// FLUIDSYNTH_API void mod_set_source1 (fluid_mod_t * mod,int src,int flags)
	 ( mod_t*:_mod Z32:_src Z32:_flags ) "libfluidsynth.so" "fluid_mod_set_source1" sys optional mod_set_source1 

	// FLUIDSYNTH_API void mod_set_source2 (fluid_mod_t * mod,int src,int flags)
	 ( mod_t*:_mod Z32:_src Z32:_flags ) "libfluidsynth.so" "fluid_mod_set_source2" sys optional mod_set_source2 

	// FLUIDSYNTH_API void mod_set_dest (fluid_mod_t * mod,int dst)
	 ( mod_t*:_mod Z32:_dst ) "libfluidsynth.so" "fluid_mod_set_dest" sys optional mod_set_dest 

	// FLUIDSYNTH_API void mod_set_amount (fluid_mod_t * mod,double amount)
	 ( mod_t*:_mod Double32:_amount ) "libfluidsynth.so" "fluid_mod_set_amount" sys optional mod_set_amount 

	// FLUIDSYNTH_API int mod_get_source1 (const fluid_mod_t * mod)
	 ( constfluid_mod_t*:_mod  -> Z32 ) "libfluidsynth.so" "fluid_mod_get_source1" sys optional mod_get_source1 

	// FLUIDSYNTH_API int mod_get_flags1 (const fluid_mod_t * mod)
	 ( constfluid_mod_t*:_mod  -> Z32 ) "libfluidsynth.so" "fluid_mod_get_flags1" sys optional mod_get_flags1 

	// FLUIDSYNTH_API int mod_get_source2 (const fluid_mod_t * mod)
	 ( constfluid_mod_t*:_mod  -> Z32 ) "libfluidsynth.so" "fluid_mod_get_source2" sys optional mod_get_source2 

	// FLUIDSYNTH_API int mod_get_flags2 (const fluid_mod_t * mod)
	 ( constfluid_mod_t*:_mod  -> Z32 ) "libfluidsynth.so" "fluid_mod_get_flags2" sys optional mod_get_flags2 

	// FLUIDSYNTH_API int mod_get_dest (const fluid_mod_t * mod)
	 ( constfluid_mod_t*:_mod  -> Z32 ) "libfluidsynth.so" "fluid_mod_get_dest" sys optional mod_get_dest 

	// FLUIDSYNTH_API double mod_get_amount (const fluid_mod_t * mod)
	 ( constfluid_mod_t*:_mod  -> Double32 ) "libfluidsynth.so" "fluid_mod_get_amount" sys optional mod_get_amount 

	// FLUIDSYNTH_API int mod_test_identity (const fluid_mod_t * mod1,const fluid_mod_t * mod2)
	 ( constfluid_mod_t*:_mod1 constfluid_mod_t*:_mod2  -> Z32 ) "libfluidsynth.so" "fluid_mod_test_identity" sys optional mod_test_identity 

	// FLUIDSYNTH_API int mod_has_source (const fluid_mod_t * mod,int cc,int ctrl)
	 ( constfluid_mod_t*:_mod Z32:_cc Z32:_ctrl  -> Z32 ) "libfluidsynth.so" "fluid_mod_has_source" sys optional mod_has_source 

	// FLUIDSYNTH_API int mod_has_dest (const fluid_mod_t * mod,int gen)
	 ( constfluid_mod_t*:_mod Z32:_gen  -> Z32 ) "libfluidsynth.so" "fluid_mod_has_dest" sys optional mod_has_dest 

	// FLUIDSYNTH_API void mod_clone (fluid_mod_t * mod,const fluid_mod_t * src)
	 ( mod_t*:_mod constfluid_mod_t*:_src ) "libfluidsynth.so" "fluid_mod_clone" sys optional mod_clone 

	// FLUIDSYNTH_API void voice_add_mod (fluid_voice_t * voice,fluid_mod_t * mod,int mode)
	 ( voice_t*:_voice mod_t*:_mod Z32:_mode ) "libfluidsynth.so" "fluid_voice_add_mod" sys optional voice_add_mod 

	// FLUIDSYNTH_API float voice_gen_get (fluid_voice_t * voice,int gen)
	 ( voice_t*:_voice Z32:_gen  -> Real ) "libfluidsynth.so" "fluid_voice_gen_get" sys optional voice_gen_get 

	// FLUIDSYNTH_API void voice_gen_set (fluid_voice_t * voice,int gen,float val)
	 ( voice_t*:_voice Z32:_gen Real:_val ) "libfluidsynth.so" "fluid_voice_gen_set" sys optional voice_gen_set 

	// FLUIDSYNTH_API void voice_gen_incr (fluid_voice_t * voice,int gen,float val)
	 ( voice_t*:_voice Z32:_gen Real:_val ) "libfluidsynth.so" "fluid_voice_gen_incr" sys optional voice_gen_incr 

	// FLUIDSYNTH_API unsigned int voice_get_id (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_get_id" sys optional voice_get_id 

	// FLUIDSYNTH_API int voice_get_channel (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_get_channel" sys optional voice_get_channel 

	// FLUIDSYNTH_API int voice_get_key (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_get_key" sys optional voice_get_key 

	// FLUIDSYNTH_API int voice_get_actual_key (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_get_actual_key" sys optional voice_get_actual_key 

	// FLUIDSYNTH_API int voice_get_velocity (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_get_velocity" sys optional voice_get_velocity 

	// FLUIDSYNTH_API int voice_get_actual_velocity (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_get_actual_velocity" sys optional voice_get_actual_velocity 

	// FLUIDSYNTH_API int voice_is_playing (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_is_playing" sys optional voice_is_playing 

	// FLUIDSYNTH_API int voice_is_on (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_is_on" sys optional voice_is_on 

	// FLUIDSYNTH_API int voice_is_sustained (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_is_sustained" sys optional voice_is_sustained 

	// FLUIDSYNTH_API int voice_is_sostenuto (const fluid_voice_t * voice)
	 ( constfluid_voice_t*:_voice  -> Z32 ) "libfluidsynth.so" "fluid_voice_is_sostenuto" sys optional voice_is_sostenuto 

	// FLUIDSYNTH_API int voice_optimize_sample (fluid_sample_t * s)
	 ( sample_t*:_s  -> Z32 ) "libfluidsynth.so" "fluid_voice_optimize_sample" sys optional voice_optimize_sample 

	// FLUIDSYNTH_API void voice_update_param (fluid_voice_t * voice,int gen)
	 ( voice_t*:_voice Z32:_gen ) "libfluidsynth.so" "fluid_voice_update_param" sys optional voice_update_param 

	// FLUIDSYNTH_API void version (int * major,int * minor,int * micro)
	 ( Z32*:_major Z32*:_minor Z32*:_micro ) "libfluidsynth.so" "fluid_version" sys optional version 

	// FLUIDSYNTH_API char * version_str (void)
	 (  -> String ) "libfluidsynth.so" "fluid_version_str" sys optional version_str 

	// FLUIDSYNTH_API int ladspa_is_active (fluid_ladspa_fx_t * fx)
	 ( ladspa_fx_t*:_fx  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_is_active" sys optional ladspa_is_active 

	// FLUIDSYNTH_API int ladspa_activate (fluid_ladspa_fx_t * fx)
	 ( ladspa_fx_t*:_fx  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_activate" sys optional ladspa_activate 

	// FLUIDSYNTH_API int ladspa_deactivate (fluid_ladspa_fx_t * fx)
	 ( ladspa_fx_t*:_fx  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_deactivate" sys optional ladspa_deactivate 

	// FLUIDSYNTH_API int ladspa_reset (fluid_ladspa_fx_t * fx)
	 ( ladspa_fx_t*:_fx  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_reset" sys optional ladspa_reset 

	// FLUIDSYNTH_API int ladspa_check (fluid_ladspa_fx_t * fx,char * err,int err_size)
	 ( ladspa_fx_t*:_fx String:_err Z32:_err_size  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_check" sys optional ladspa_check 

	// FLUIDSYNTH_API int ladspa_host_port_exists (fluid_ladspa_fx_t * fx,const char * name)
	 ( ladspa_fx_t*:_fx String:_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_host_port_exists" sys optional ladspa_host_port_exists 

	// FLUIDSYNTH_API int ladspa_add_buffer (fluid_ladspa_fx_t * fx,const char * name)
	 ( ladspa_fx_t*:_fx String:_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_add_buffer" sys optional ladspa_add_buffer 

	// FLUIDSYNTH_API int ladspa_buffer_exists (fluid_ladspa_fx_t * fx,const char * name)
	 ( ladspa_fx_t*:_fx String:_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_buffer_exists" sys optional ladspa_buffer_exists 

	// FLUIDSYNTH_API int ladspa_add_effect (fluid_ladspa_fx_t * fx,const char * effect_name,const char * lib_name,const char * plugin_name)
	 ( ladspa_fx_t*:_fx String:_effect_name String:_lib_name String:_plugin_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_add_effect" sys optional ladspa_add_effect 

	// FLUIDSYNTH_API int ladspa_effect_can_mix (fluid_ladspa_fx_t * fx,const char * name)
	 ( ladspa_fx_t*:_fx String:_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_effect_can_mix" sys optional ladspa_effect_can_mix 

	// FLUIDSYNTH_API int ladspa_effect_set_mix (fluid_ladspa_fx_t * fx,const char * name,int mix,float gain)
	 ( ladspa_fx_t*:_fx String:_name Z32:_mix Real:_gain  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_effect_set_mix" sys optional ladspa_effect_set_mix 

	// FLUIDSYNTH_API int ladspa_effect_port_exists (fluid_ladspa_fx_t * fx,const char * effect_name,const char * port_name)
	 ( ladspa_fx_t*:_fx String:_effect_name String:_port_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_effect_port_exists" sys optional ladspa_effect_port_exists 

	// FLUIDSYNTH_API int ladspa_effect_set_control (fluid_ladspa_fx_t * fx,const char * effect_name,const char * port_name,float val)
	 ( ladspa_fx_t*:_fx String:_effect_name String:_port_name Real:_val  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_effect_set_control" sys optional ladspa_effect_set_control 

	// FLUIDSYNTH_API int ladspa_effect_link (fluid_ladspa_fx_t * fx,const char * effect_name,const char * port_name,const char * name)
	 ( ladspa_fx_t*:_fx String:_effect_name String:_port_name String:_name  -> Z32 ) "libfluidsynth.so" "fluid_ladspa_effect_link" sys optional ladspa_effect_link 

end

