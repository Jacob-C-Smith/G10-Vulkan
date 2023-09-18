/** !
 * @file G10/GXUserCode.h
 * @author Benjamin F Daly, Jacob Smith
 *
 * Audio for G10
 */

// Include guard
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>

// SDL2
#ifdef BUILD_G10_WITH_SDL_MIXER
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#endif

// FMOD
#ifdef BUILD_G10_WITH_FMOD
#include <FMOD-core/fmod.h>
#include <FMOD-core/fmod_common.h>
#endif

struct GXSound_s
{
	#ifdef BUILD_G10_WITH_FMOD
		FMOD_SOUND **sound;
	#endif
	int i;
};

// Audio processing
/** !
 * Processes audio once per frame
 *
 * @param p_instance: The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int process_audio ( GXInstance_t *p_instance );

// Constructors
/** !
 *  Load a sound from a JSON file
 *
 * @param pp_sound : return
 * @param path     : The path to an audio JSON object
 *
 * @sa load_sound_as_json
 * @sa load_sound_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_sound ( GXSound_t **pp_sound, const char* path);

/** !
 *  Load a sound from JSON text
 *
 * @param pp_sound : return
 * @param text  : The sound JSON object text
 *
 * @sa load_sound
 * @sa load_sound_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_sound_as_json ( GXSound_t** pp_sound, char* text );

/** !
 *  Load a sound from a JSON value
 *
 * @param pp_sound : return
 * @param p_value  : The sound JSON object text
 *
 * @sa load_sound
 * @sa load_sound_as_json
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_sound_as_json_value ( GXSound_t** pp_sound, json_value *p_value );

// Mutators
/** !
 *  Set the audio listener from a camera
 *
 * @param p_camera : return
 *
 */
DLLEXPORT void set_3d_listener_and_orient ( GXCamera_t* p_camera );

/** !
 *  Set the vector describing the position of an audio source on a given channel
 *
 * @param channel : the index of the channel
 * @param position : vec3
 */
DLLEXPORT void set_channel_3d_position ( int channel, vec3 position );

/** !
 *  Set the volume of a channel in dB
 *
 * @param channel : the index of the channel
 * @param position : vec3
 *
 */
DLLEXPORT void set_channel_volume ( int channel, float dB );

// Actions
/** !
 * Play a sound
 *
 * @param p_sound      : the sound to play
 * @param start_paused : Play the audio immediately, if true
 *
 * @return channel_number
 */
DLLEXPORT int play_sound ( GXSound_t* p_sound, bool start_paused );

/** !
 * Stop a channel
 *
 * @param channel : the index of the channel
 *
 */
DLLEXPORT void stop_channel ( int channel );

/** !
 * Stop all sound
 */
DLLEXPORT void stop_all_channels ( void ) ;

// State
/** !
 *  Is a channel playing?
 *
 * @param channel : the index of the channel
 *
 * @return true if playing else false
 */
DLLEXPORT bool is_playing ( int channel );

// Destructors
/** !
 *  Free a sound
 *
 * @param p_sound : Pointer to sound
 *
 * @sa create_sound
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_sound ( GXSound_t* p_sound );

	#ifdef BUILD_G10_WITH_FMOD
	//TODO: May implement this, gotta talk to Jake about it
	DLLEXPORT const char*		get_fmod_error_code(FMOD_RESULT r);
	#endif
