/** !
 * @file G10/GXAI.h
 * @author Jacob Smith
 *
 * Simple finite state automaton
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXEntity.h>

struct GXAI_s
{
	char    *name,
	        *current_state;
	dict    *states;
	int    (*pre_ai)(GXEntity_t* p_entity);
	size_t   users;
};

// Allocators
/** !
 *  Allocate memory for an AI
 *
 * @param pp_ai : return
 *
 * @sa destroy_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_ai ( GXAI_t **pp_ai );

// Constructors
/** !
 *  Load an AI from a JSON file
 *
 * @param pp_ai : return
 * @param path  : The path to a JSON file containing an AI object
 *
 * @sa load_ai_as_json_text
 * @sa load_ai_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_ai ( GXAI_t **pp_ai, char *path );

/** !
 *  Load an AI from JSON text
 *
 * @param pp_ai : return
 * @param text  : The AI JSON object text
 *
 * @sa load_ai
 * @sa load_ai_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_ai_as_json_text ( GXAI_t **pp_ai, char *text );

/** !
 *  Load an AI from a JSON value
 *
 * @param pp_ai   : return
 * @param p_value : The JSON value
 *
 * @sa load_ai
 * @sa load_ai_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_ai_as_json_value ( GXAI_t **pp_ai, json_value *p_value );


/** !
 *  Load an AI from JSON text
 *
 * @param pp_ai : return
 * @param p_ai  : pointer to ai to copy
 *
 * @sa create_ai
 * @sa load_ai
 * @sa load_ai_as_json_text
 * @sa load_ai_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int copy_ai ( GXAI_t **pp_ai, GXAI_t *p_ai );

// Info
/** !
 *  Print info about an AI
 *
 * @param p_ai  : pointer to ai to log
 *
 * @sa load_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_info ( GXAI_t *p_ai );

// Tasks
/** !
 *  Update each AI entity in the instances active scene
 *
 * @param instance : Pointer to instance
 *
 * @sa pre_update_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_ai ( GXInstance_t *p_instance );

/** !
 *  Preupdate each AI entity in the instances active scene
 *
 * @param instance : Pointer to instance
 *
 * @sa update_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int pre_update_ai ( GXInstance_t *p_instance );

// User callbacks
/** !
 *  Add a callback function for a state
 *
 * @param p_ai             : Pointer to AI
 * @param state_name       : The name of the state
 * @param function_pointer : Pointer to a function with GXEntity_t pointer argument and int return
 *
 * @sa set_ai_state
 * @sa set_ai_pre_update_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int add_ai_state_callback ( GXAI_t *p_ai, char *state_name, int (*function_pointer)(GXEntity_t *entity));

/** !
 *  Changes the state of the AI
 *
 * @param p_ai             : Pointer to AI
 * @param state_name       : The name of the state
 *
 * @sa add_ai_state_callback
 * @sa set_ai_pre_update_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int set_ai_state ( GXAI_t *p_ai    , const char *state_name );

/** !
 *  Changes the AI preupdate function
 *
 * @param p_ai             : Pointer to AI
 * @param function_pointer : Pointer to a function that with GXEntity_t * argument and int return
 *
 * @sa add_ai_state_callback
 * @sa set_ai_state
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int set_ai_pre_update_callback ( GXAI_t *p_ai, int (*function_pointer)(GXEntity_t *entity) );

// Destructors
/** !
 *  Free an AI and all its contents
 *
 * @param pp_ai : Pointer to AI pointer
 *
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_ai ( GXAI_t **pp_ai );