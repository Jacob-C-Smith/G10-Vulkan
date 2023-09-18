
/** !
 * @file G10/GXInput.h
 * @author Jacob Smith
 *
 * Input processing for G10.
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// SDL2
#include <SDL.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>

#define UI_M1     0x01
#define UI_M2     0x02
#define UI_M3     0x04
#define UI_M4     0x08
#define UI_M5     0x10
#define UI_SWUP   0x20
#define UI_SWDOWN 0x40

enum input_state_e
{
	KEYBOARD = 1,
	MOUSE    = 2,
	GAMEPAD  = 3
};

struct GXBind_s
{
	bool              active;
	size_t            key_count,
		              callback_count,
		              callback_max;
	char             *name,
	   	            **keys;
	void            **callbacks;
	struct GXBind_s  *next;
};

struct GXInput_s
{
	char  *name;
	dict  *bind_lut,
		  *binds;
	queue *p_key_queue,
	      *p_bind_queue;
	float  mouse_sensitivity;
};

struct callback_parameter_s
{
	enum input_state_e input_state;

	union {
		struct {
			bool depressed;
		} key;

		struct mouse_state_s {
			s32 xrel,
				yrel;
			u8  button;
		} mouse_state;

		// A la xbox one controller
		struct {

			bool A;
			bool B;
			bool X;
			bool Y;

			bool dpad_down;
			bool dpad_left;
			bool dpad_right;
			bool dpad_up;

			vec2 left_stick;
			vec2 right_stick;

			float left_trigger;
			float right_trigger;

			bool left_bumper;
			bool right_bumper;

			bool start;
			bool select;

		} gamepad_state;
	} inputs;
};



// Allocators
/** !
 *  Allocate memory for an input
 *
 * @param pp_input : return
 *
 * @sa destroy_input
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_input ( GXInput_t **pp_input );

/** !
 *  Allocate memory for a bind
 *
 * @param pp_bind : return
 *
 * @sa destroy_bind
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_bind ( GXBind_t **pp_bind );

// Constructors
/** !
 *  Load an input from a JSON file
 *
 * @param pp_input : return
 * @param path     : The path to a JSON file containing an input object
 *
 * @sa load_input_as_json_text
 * @sa load_input_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_input ( GXInput_t **pp_input, const char *path );

/** !
 *  Load an input from JSON text
 *
 * @param pp_input : return
 * @param text     : The input JSON object text
 *
 * @sa load_input
 * @sa load_input_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_input_as_json_text ( GXInput_t **pp_input, char *text );

/** !
 *  Load an input from a JSON value
 *
 * @param pp_input : return
 * @param text     : The input JSON object text
 *
 * @sa load_input
 * @sa load_input_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_input_as_json_value ( GXInput_t **pp_input, json_value *p_value );

/** !
 *  Load a bind from a JSON value
 *
 * @param pp_bind : return
 * @param p_value : The bind JSON value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_bind_as_json_value ( GXBind_t **pp_bind, json_value *p_value );
DLLEXPORT int construct_bind ( GXBind_t **pp_bind , char *name, char **keys );

// Callback registration
/** !
 *  Add a callback to a bind
 *
 * @param p_bind           : The bind
 * @param function_pointer : The pointer to the callback function
 *
 * @sa unregister_bind_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int register_bind_callback ( GXBind_t *p_bind, void *function_pointer );

/** !
 *  Remove a callback from a bind
 *
 * @param p_bind           : The bind
 * @param function_pointer : The pointer to the callback function
 *
 * @sa register_bind_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int unregister_bind_callback ( GXBind_t *p_bind, void *function_pointer );

// Scancode finder
DLLEXPORT SDL_Scancode find_key ( const char *name );

// Key information
DLLEXPORT int print_all_keys ( void );
DLLEXPORT int print_all_binds ( GXInput_t *p_inputs );

// Input processing
DLLEXPORT int process_input ( GXInstance_t *p_instance );

// Bind appending
DLLEXPORT int append_bind ( GXInput_t *p_input, GXBind_t *p_bind );

// Info
/** !
 *  Print info about an input
 *
 * @param p_input : Write information about an input
 *
 * @sa load_input
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int input_info ( GXInput_t *p_input );

// Bind calling
DLLEXPORT int call_bind ( GXBind_t *p_bind, callback_parameter_t input );

// Bind operations
DLLEXPORT GXBind_t *find_bind ( GXInput_t *p_input, char *name );
/** !
 *  Remove a bind from a GXInput. Deallocate the bind if pp_bind == nullptr else return the bind to pp_bind
 * 
 * @param p_input : An input
 * @param name    : The name of the bind 
 * @param pp_bind : return
 *
 * @sa load_input
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int remove_bind ( GXInput_t *p_input, char *name, GXBind_t **pp_bind );

// Destructors
/** !
 *  Free a bind and all its contents
 *
 * @param pp_bind : Pointer to bind pointer
 *
 * @sa create_bind
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_bind ( GXBind_t **pp_bind );

/** !
 *  Destroy an input
 *
 * @param pp_input : Pointer to input pointer
 *
 * @sa create_input
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_input ( GXInput_t **pp_input );