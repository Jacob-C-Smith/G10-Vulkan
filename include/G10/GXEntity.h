/** !
 * @file G10/GXEntity.h
 * @author Jacob Smith
 *
 * Entity
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXPart.h>
#include <G10/GXTransform.h>
#include <G10/GXRigidbody.h>
#include <G10/GXCollider.h>
#include <G10/GXAI.h>

struct GXEntity_s
{
	char          *name;
	dict          *parts;
	dict          *materials;
	char          *shader_name;
	GXTransform_t *transform;
	GXRigidbody_t *rigidbody;
	GXCollider_t  *collider;
	GXAI_t        *ai;
};

// Allocators
/** !
 *  Allocate memory for an entity
 *
 * @param pp_entity : return
 *
 * @sa destroy_entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_entity ( GXEntity_t **pp_entity );

// Constructors

/** !
 *  Load an entity from a JSON file
 *
 * @param pp_entity : return
 * @param path      : The path to a file containing a entity JSON
 *
 * @sa load_entity_as_json_text
 * @sa load_entity_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_entity ( GXEntity_t **pp_entity, char* path );

/** !
 *  Load an entity as JSON object text
 *
 * @param pp_entity : return
 * @param text      : The entity JSON text
 *
 * @sa load_entity
 * @sa load_entity_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_entity_as_json_text ( GXEntity_t **pp_entity, char* text );

/** !
 *  Load an entity as JSON object text
 *
 * @param pp_entity : return
 * @param p_value   : The entity JSON value
 *
 * @sa load_entity
 * @sa load_entity_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_entity_as_json_value ( GXEntity_t **pp_entity, json_value *p_value );

// Physics
/** !
 * Calculate the net force on the object.
 * 6 Contact forces are summated Gravity,
 * Normal, Applied, Friction, Tension, Spring.
 *
 * @param p_entity : Pointer to entity
 *
 * @sa move_entity
 * TODO: See also, collision function
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int calculate_entity_force ( GXEntity_t *p_entity );

/** !
 * Updates location, and rotation, and their first and second order derivatives from force and torque
 *
 * @param p_entity : Pointer to entity
 *
 * @sa calculate_entity_force
 * TODO: See also, collision function
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int move_entity ( GXEntity_t *p_entity );

// Info
DLLEXPORT int entity_info ( GXEntity_t  *p_entity, size_t idx );

// AI
/** !
 * Updates AI State
 *
 * @param p_entity : Pointer to entity
 *
 * @sa update_entity_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int preupdate_entity_ai ( GXEntity_t *p_entity );

/** !
 * Draw the entity
 * 
 * @param vp_entity void pointer to entity
 * @param p_renderer
 * @param p_render_pass,
 * @param p_subpass,
 * @param p_shader
 *
 * @return 1 on success, 0 on error
*/
DLLEXPORT int pfn_draw_item (
	void           *vp_entity,
	GXRenderer_t   *p_renderer,
	GXRenderPass_t *p_render_pass,
	GXSubpass_t    *p_subpass,
	GXShader_t     *p_shader
);

/** !
 * Run state update function
 *
 * @param p_entity : Pointer to entity
 *
 * @sa preupdate_entity_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_entity_ai ( GXEntity_t *p_entity );

DLLEXPORT int get_model_matrix ( void *ret );

// Destructor
/** !
 * Free an entity, and all its contents
 *
 * @param pp_entity : Pointer to entity pointer
 *
 * @sa create_entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_entity ( GXEntity_t **pp_entity );