/** !
 * @file G10/GXPart.h
 * @author Jacob Smith
 *
 * 3D meshes
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXTransform.h>

// SDL2
#include <SDL.h>

// Vulkan
//#include <vulkan/vulkan.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

struct GXPart_s
{
	char           *name;

	GXMaterial_t   *active_material;
	GXShader_t     *active_shader;
	GXTransform_t  *p_transform;

	/*VkBuffer        vertex_buffer,
	                element_buffer;
	VkDeviceMemory  vertex_buffer_memory,
	                element_buffer_memory;
	*/
	size_t          vertex_count,
	                index_count,
		            users;
};

// Allocators
/** !
 *  Allocates memory for a part.
 *
 * @param pp_part : return
 *
 * @sa destroy_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_part ( GXPart_t   **pp_part );

// Constructors
/** !
 *  Load a part from the filesystem
 *
 * @param pp_part : return
 * @param path    : The path to a part JSON file
 *
 * @sa load_part_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_part ( GXPart_t   **pp_part, char *path );

/** !
 *  Load a part from a JSON object
 *
 * @param pp_part : return
 * @param text    : The part JSON text
 *
 * @sa load_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_part_as_json_text ( GXPart_t **pp_part, char *text );
DLLEXPORT int load_part_as_json_value ( GXPart_t **pp_part, JSONValue_t *p_value );

// Info

// TODO: Document
DLLEXPORT int part_info ( GXPart_t *p_part );

// Destructors
/** !
 *  Destroy a part
 *
 * @param pp_part : pointer to part pointer
 *
 * @sa create_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_part ( GXPart_t **pp_part );