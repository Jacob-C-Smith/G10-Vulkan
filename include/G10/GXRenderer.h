/** !
 * @file G10/GXRenderer.h
 * 
 * @author Jacob Smith
 *
 * Renderer
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vulkan
//#include <vulkan/vulkan.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
//#include <G10/GXShader.h>
#include <G10/GXCameraController.h>

struct GXRenderer_s
{
	char *name;

	dict *shaders,
	     *render_passes,
	     *attachments; // value is of type GXAttachment_t *
	
	size_t render_pass_count,
	       clear_color_count,
		   command_buffers_count;

	GXRenderPass_t **render_passes_data,
	                *current_render_pass;

	VkClearValue *clear_colors;

	VkCommandPool command_pool;

	VkCommandBuffer *command_buffers;

	struct
	{
		dict *binary_semaphores,
		     *fences;
	} synchronization;
};

struct GXRenderPass_s
{
	char *name;

	dict *subpasses,
	     *draw_queue_types,
	     *attachments; // type: char *

	size_t subpasses_count,
	       attachments_count;

	GXSubpass_t **subpasses_data;
	VkFramebuffer *framebuffers;
	GXAttachment_t **attachments_data;
	
	VkRenderPass render_pass;
	VkImageView *attachment_image_views;
};

struct GXSubpass_s
{
	char  *name,
	     **shader_names;
	size_t shader_count;
	VkSubpassDescription2 subpass_description;
};

struct GXImage_s
{
	char *name;
	VkImage image;
	VkImageLayout image_layout;
	VkDeviceMemory image_memory;
};

struct GXAttachment_s
{
	char *name;
	VkAttachmentDescription2 attachment_description2;
	GXImage_t *p_image;
	VkImageView image_view;
};

struct GXTexture_s
{
	char *name;
	GXImage_t *p_image;
	VkImageView image_view;
	VkSampler sampler;
};

// Allocators
/** !
 *  Allocate memory for a renderer
 *
 * @param pp_renderer : return
 *
 * @sa destroy_renderrer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_renderer ( GXRenderer_t **pp_renderer );

/** !
 *  Allocate memory for a render pass
 *
 * @param pp_render_pass : return
 *
 * @sa destroy_render_pass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_render_pass ( GXRenderPass_t **pp_render_pass );

/** !
 *  Allocate memory for a subpass
 *
 * @param pp_subpass : return
 *
 * @sa destroy_subpass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_subpass ( GXSubpass_t **pp_subpass );

/** !
 *  Allocate memory for am image
 *
 * @param pp_image : return
 *
 * @sa destroy_image
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_image ( GXImage_t **pp_image );

/** !
 *  Allocate memory for a attachment
 *
 * @param pp_attachment : return
 *
 * @sa destroy_attachment
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_attachment ( GXAttachment_t **pp_attachment );

/** !
 *  Allocate memory for a texture
 *
 * @param pp_texture : return
 *
 * @sa destroy_texture
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_texture ( GXTexture_t **pp_texture );

/** !
 *  Allocate memory for a framebuffer
 *
 * @param pp_framebuffer : return
 *
 * @sa destroy_framebuffer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_framebuffer ( GXFramebuffer_t **pp_framebuffer );

// Constructors
/** !
 *  Load a renderer from a JSON file
 *
 * @param pp_renderer : return
 * @param path        : The path to a JSON file containing a renderer object
 *
 * @sa load_renderer_as_json_text
 * @sa load_renderer_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_renderer ( GXRenderer_t **pp_renderer, char *path );

/** !
 *  Load a renderer from JSON text
 *
 * @param pp_renderer : return
 * @param text        : The text
 *
 * @sa load_renderer
 * @sa load_renderer_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_renderer_as_json_text ( GXRenderer_t   **pp_renderer, char *text );

/** !
 *  Load a renderer as a JSON value
 *
 * @param pp_renderer : return
 * @param p_value     : The JSON value
 *
 * @sa load_renderer
 * @sa load_renderer_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_renderer_as_json_value ( GXRenderer_t **pp_renderer, JSONValue_t *p_value );

/** !
 *  Load a render pass from a JSON file
 *
 * @param pp_render_pass : return
 * @param path           : The path to a JSON file containing a render pass object
 *
 * @sa load_render_pass_as_json_text
 * @sa load_render_pass_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_render_pass         ( GXRenderPass_t **pp_render_pass, char *path );

/** !
 *  Load a render pass from JSON text
 *
 * @param pp_render_pass : return
 * @param text           : The render pass JSON text
 *
 * @sa load_render_pass
 * @sa load_render_pass_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_render_pass_as_json_text ( GXRenderPass_t **pp_render_pass, char *text );

/** !
 *  Load a render pass from a JSON value
 *
 * @param pp_render_pass : return
 * @param p_value        : The render pass JSON value
 *
 * @sa load_render_pass
 * @sa load_render_pass_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_render_pass_as_json_value ( GXRenderPass_t **pp_render_pass, JSONValue_t *p_value );

/** !
 *  Load a subpass from a JSON file
 *
 * @param pp_subpass : return
 * @param path : path to JSON file
 *
 * @sa load_subpass_as_json_text
 * @sa load_subpass_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_subpass ( GXSubpass_t **pp_subpass, char *path );

/** !
 *  Load a subpass from JSON text
 *
 * @param pp_subpass : return
 * @param text       : The JSON text
 *
 * @sa load_subpass
 * @sa load_subpass_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_subpass_as_json_text ( GXSubpass_t **pp_subpass, char *text );

/** !
 *  Load a subpass from a JSON value
 *
 * @param pp_subpass : return
 * @param p_value    : The JSON value
 *
 * @sa load_subpass
 * @sa load_subpass_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_subpass_as_json_value ( GXSubpass_t **pp_subpass, JSONValue_t *p_value );

/** !
 *  Allocate and construct a Vulkan image
 *
 * @param p_image        : return
 * @param flags          : VkImageCreateFlags
 * @param image_type     : VkImageType
 * @param format         : VkFormat
 * @param width          : int
 * @param height         : int
 * @param depth          : int
 * @param mip_levels     : size_t
 * @param array_layers   : size_t
 * @param samples        : VkSampleCountFlagBits
 * @param tiling         : VkImageTiling
 * @param usage          : VkImageUsageFlags
 * @param sharing_mode   : VkSharingMode
 * @param initial_layout : VkImageLayout
 *
 * @sa load_image
 * @sa load_image_as_json_text
 * @sa load_image_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int construct_image
(
	GXImage_t     *p_image     , VkImageCreateFlags    flags,
	VkImageType    image_type  , VkFormat              format,
	int            width       , int                   height,
	int            depth       , size_t                mip_levels,
	size_t         array_layers, VkSampleCountFlagBits samples,
	VkImageTiling  tiling      , VkImageUsageFlags     usage,
	VkSharingMode  sharing_mode, VkImageLayout         initial_layout
);

DLLEXPORT int construct_image_view
(
	VkImageView        *ret      , GXImage_t          *p_image,
	VkImageViewType     view_type, VkFormat            format,
	VkComponentMapping  swizzle  , VkImageAspectFlags  aspect_mask
);

/** !
 *  Load an attachment from a JSON file
 *
 * @param pp_attachment : return
 * @param path          : path to JSON file
 *
 * @sa load_attachment_as_json_text
 * @sa load_attachment_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_attachment ( GXAttachment_t **pp_attachment, char *path );

/** !
 *  Load an attachment from JSON text
 *
 * @param pp_attachment : return
 * @param text          : The JSON text
 *
 * @sa load_attachment
 * @sa load_attachment_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_attachment_as_json_text ( GXAttachment_t **pp_attachment, char *text );

/** !
 *  Load an attachment from a JSON value
 *
 * @param pp_attachment : return
 * @param p_value       : The JSON value
 *
 * @sa load_attachment
 * @sa load_attachment_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_attachment_as_json_value ( GXAttachment_t **pp_attachment, JSONValue_t *p_value );

// Logging
/** !
 *  Print info about a renderer
 *
 * @param p_renderer : the renderer
 *
 * @sa load_renderer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int print_renderer ( GXRenderer_t *p_renderer );

/** !
 *  Print info about a render pass
 *
 * @param p_render_pass : the render pass
 *
 * @sa load_render_pass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int print_render_pass ( GXRenderPass_t *p_render_pass );

/** !
 *  Print info about an attachment
 *
 * @param p_attachment : the attachment
 *
 * @sa load_attachment
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int print_attachment ( GXAttachment_t *p_attachment );

// Add subpass callback
DLLEXPORT int add_subpass_callback ( char *name, void (*function_pointer)());

// Renderer operations
/** !
 *  Called once a frame by the scheduler to draw the game to the window.
 *
 * Draws every render pass in the instances active renderer to a swapchain, presents the last frame.
 *
 * @param p_instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int render_frame ( GXInstance_t *p_instance );

// Draw operations
/** !
 *  Draw a part in a subpass
 *
 * @param p_instance : The active instance
 * @param p_part : The part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT void draw_part ( void *vp_part );

/** !
 *  Presents the last frame, after the current frame is done drawing
 *
 * @param p_instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int present_frame ( GXInstance_t *p_instance );

// Destructors
/** !
 *  Destroy a renderer
 *
 * @param p_renderer : Pointer to renderer pointer
 *
 * @sa create_renderer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_renderer ( GXRenderer_t **pp_renderer );

/** !
 *  Destroy a render pass
 *
 * @param pp_render_pass : Pointer to render pass pointer
 *
 * @sa create_render_pass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_render_pass ( GXRenderPass_t **pp_render_pass );

/** !
 *  Destroy a subpass
 *
 * @param pp_subpass : Pointer to subpass pointer
 *
 * @sa create_subpass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_subpass ( GXSubpass_t **pp_subpass );

/** !
 *  Destroy an attachment
 *
 * @param pp_attachment : Pointer to attachment pointer
 *
 * @sa create_attachment
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_attachment ( GXAttachment_t **pp_attachment );