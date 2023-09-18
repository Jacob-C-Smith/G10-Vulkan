/** !
 * @file G10/GXScheduler.h
 * @author Jacob Smith
 *
 * The scheduler specifies and coordinates work over time.
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// SDL
#include <SDL.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// array submodule
#include <array/array.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXScene.h>
#include <G10/GXPhysics.h>
//#include <G10/GXRenderer.h>
#include <G10/GXInput.h>
#include <G10/GXServer.h>
#include <G10/GXUI.h>
#include <G10/GXUserCode.h>

struct GXTask_s
{
	char  *name;
	char  *wait_thread;
	char  *wait_task;
	int  (*function_pointer)(GXInstance_t*);
};
typedef struct GXTask_s GXTask_t;

struct GXScheduler_s
{
	char *name;
	dict *threads;
};

struct GXThread_s
{
	char        *name;
	size_t       task_count,
		        *complete_tasks;
	bool         running;
	GXTask_t   **tasks;
	SDL_Thread  *thread;
};


// Allocators
/** !
 *  Allocate memory for a schedule
 *
 * @param pp_schedule : return
 *
 * @sa destroy_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_schedule ( GXSchedule_t **pp_schedule );

/** !
 *  Allocate memory for a thread
 *
 * @param pp_thread : return
 *
 * @sa destroy_thread
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_thread ( GXThread_t **pp_thread );

// Constructors
/** !
 *  Load a schedule from a JSON file
 *
 * @param pp_schedule : return
 * @param path        : The path to a JSON file containing an schedule object
 *
 * @sa load_schedule_as_json_text
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_schedule ( GXSchedule_t **pp_schedule, char *path );

/** !
 *  Load a schedule from JSON text
 *
 * @param pp_schedule : return
 * @param text        : The schedule JSON object text
 *
 * @sa load_schedule
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_schedule_as_json_text ( GXSchedule_t **pp_schedule, char *text );

/** !
 *  Load a schedule from a JSON value
 *
 * @param pp_schedule : return
 * @param p_value     : The schedule JSON object value
 *
 * @sa load_schedule
 * @sa load_schedule_as_json_text
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_schedule_as_json_value ( GXSchedule_t **pp_schedule, json_value *p_value );

/** !
 *  Load a thread from a JSON file
 *
 * @param pp_thread : return
 * @param path      : The path to a JSON file containing an schedule object
 *
 * @sa load_schedule_as_json_value
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_thread ( GXThread_t **pp_thread, char *path );

/** !
 *  Load a thread from a JSON file
 *
 * @param pp_thread : return
 * @param text      : The thread JSON object text
 *
 * @sa load_thread_as_json_text
 * @sa create_thread
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_thread_as_json_text ( GXThread_t **pp_thread , char *text );

// Scheduling
/** !
 *  Start running a schedule
 *
 * @param p_schedule  : Pointer to a schedule
 *
 * @sa stop_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int start_schedule ( GXSchedule_t *p_schedule );

/** !
 *  Stop running a schedule
 *
 * @param p_schedule  : Pointer to a schedule
 *
 * @sa start_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int stop_schedule ( GXSchedule_t *p_schedule );

// Destructors
/** !
 *  Destroy a schedule
 *
 * @param pp_schedule : Pointer to a schedule
 *
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_schedule ( GXSchedule_t **pp_schedule );

/** !
 *  Destroy a thread
 *
 * @param pp_thread : Pointer to a thread
 *
 * @sa create_thread
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_thread ( GXThread_t **pp_thread );
