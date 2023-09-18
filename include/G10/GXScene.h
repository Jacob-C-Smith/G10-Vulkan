/** !
 * @file G10/GXScene.h
 * @author Jacob Smith
 *
 * Scene
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// dict submodule
#include <dict/dict.h>

// array submodule
#include <array/array.h>

// json submodule
#include <json/json.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXEntity.h>
#include <G10/GXCamera.h>
#include <G10/GXLight.h>
//#include <G10/GXShader.h>

struct GXScene_s
{

	// The name of the scene
	char           *name;

	// Scene data
	dict           *entities;
	dict           *cameras;
	dict           *lights;
	GXCollision_t **collisions;

	// A list of entities with rigidbodies
	dict *actors;

	// A list of entities with rigs
	dict *rigs;

	// A list of entities with AIs
	dict *ais;

	// A bounding volume hierarchy tree containing entities with colliders
	GXBV_t *bvh;

	// The camera to be used while drawing the scene
	GXCamera_t     *active_camera;
	GXEntity_t     *active_entity;
};

// Allocators
/** !
 *  Allocate memory for a scene
 *
 * @param pp_scene : return
 *
 * @sa destroy_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_scene ( GXScene_t  **pp_scene );

// Constructors
/** !
 *  Load a scene from a JSON file
 *
 * @param pp_scene : return
 * @param path     : The path to a JSON file containing a scene object
 *
 * @sa load_scene_as_json_text
 * @sa load_scene_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_scene ( GXScene_t **pp_scene, const char *path );

/** !
 *  Load a scene from JSON text
 *
 * @param pp_scene : return
 * @param text     : The scene JSON object text
 *
 * @sa load_scene
 * @sa load_scene_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_scene_as_json_text ( GXScene_t **pp_scene, char *text );

/** !
 *  Load a scene from JSON text
 *
 * @param pp_scene : return
 * @param p_value  : The scene JSON object value
 *
 * @sa load_scene
 * @sa load_scene_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_scene_as_json_value ( GXScene_t **pp_scene, JSONValue_t *p_value );

// Appenders
/** !
 *  Append an entity to a scene
 *
 * @param p_scene  : The scene
 * @param p_entity : The entity to append
 *
 * @sa append_camera
 * @sa append_light
 * @sa append_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int append_entity ( GXScene_t *p_scene, GXEntity_t *entity );

/** !
 *  Append a camera to a scene
 *
 * @param p_scene  : The scene
 * @param p_camera : The camera to append
 *
 * @sa append_entity
 * @sa append_light
 * @sa append_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int append_camera ( GXScene_t *p_scene, GXCamera_t *camera );

/** !
 *  Append a light to a scene
 *
 * @param p_scene : The scene
 * @param p_light : The light to append
 *
 * @sa append_entity
 * @sa append_camera
 * @sa append_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int append_light ( GXScene_t *p_scene, GXLight_t *light );

/** !
 *  Append a collision to a scene
 *
 * @param p_scene     : The scene
 * @param p_collision : The collision to append
 *
 * @sa append_entity
 * @sa append_camera
 * @sa append_light
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int append_collision ( GXScene_t *p_scene, GXCollision_t *collision );

// Drawing
DLLEXPORT int draw_lights ( GXScene_t *p_scene, GXPart_t *light_part, GXShader_t *shader );

// Info
/** !
 *  Print info about a scene
 *
 * @param p_scene : pointer to scene to log
 *
 * @sa load_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int scene_info ( GXScene_t   *p_scene );

// Accessors
/** !
 *  Get an entity from the scene by it's name
 *
 * @param p_scene : The scene
 * @param name    : The name
 *
 * @return a pointer to the entity, if the scene contains the entity, else 0
 */
DLLEXPORT GXEntity_t *get_entity ( GXScene_t *p_scene, char *name );

/** !
 *  Get a camera from the scene by it's name
 *
 * @param p_scene : The scene
 * @param name    : The name
 *
 * @return a pointer to the camera, if the scene contains the camera, else 0
 */
DLLEXPORT GXCamera_t *get_camera ( GXScene_t *p_scene, char *name );

/** !
 *  Get an light from the scene by it's name
 *
 * @param p_scene : The scene
 * @param name    : The name
 *
 * @return a pointer to the light, if the scene contains the light, else 0
 */
DLLEXPORT GXLight_t *get_light ( GXScene_t *p_scene, char *name );

// Mutators
/** !
 *  Changes the scene's active camera
 *
 * @param p_ai             : Pointer to AI
 * @param state_name       : The name of the state
 *
 * @sa add_ai_state_callback
 * @sa set_ai_pre_update_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int set_active_camera ( GXScene_t   *scene, char *name );

// Scheduler tasks
/** !
 * Dequeue an entity from the load queue, parse it, and add it to the instance's active scene
 *
 * @param vp_instance : void pointer to active instance
 *
 * @sa load_light_probe_from_queue
 *
 * @return 1 on success, 0 on error
*/
DLLEXPORT int load_entity_from_queue ( void *vp_instance );

/** !
 * Dequeue a light probe from the load queue, parse it, and add it to the instance's active scene
 *
 * @param p_instance : The active instance
 *
 * @sa load_entity_from_queue
 *
 * @return 1 on success, 0 on error
*/
DLLEXPORT int load_light_probe_from_queue ( GXInstance_t *p_instance );

// Removers
/** !
 * Remove an entity from the scene by it's name
 *
 * @param p_scene : The scene
 * @param name    : The name
 *
 * @return a pointer to the entity, if the scene contains the entity, else 0
 */
DLLEXPORT GXEntity_t *remove_entity ( GXScene_t *p_scene, const char *name );

/** !
 * Remove a camera from the scene by it's name
 *
 * @param p_scene : The scene
 * @param name    : The name
 *
 * @return a pointer to the camera, if the scene contains the camera, else 0
 */
DLLEXPORT GXCamera_t *remove_camera ( GXScene_t *p_scene, const char *name );

/** !
 * Remove a light from the scene by it's name
 *
 * @param p_scene : The scene
 * @param name    : The name
 *
 * @return a pointer to the light, if the scene contains the light, else 0
 */
DLLEXPORT GXLight_t *remove_light ( GXScene_t *p_scene, const char *name );

/** !
 * Remove a collision from the scene
 *
 * @param p_scene     : The scene
 * @param p_collision : The collision
 *
 * @return a pointer to the collision, if the scene contains the collision, else 0
 */
DLLEXPORT GXCollision_t *remove_collision ( GXScene_t *p_scene, GXCollision_t *p_collision );

// Destructors
/** !
 *  Free a scene and all its contents
 *
 * @param pp_scene : Pointer to scene pointer
 *
 * @sa create_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_scene ( GXScene_t **pp_scene );