/** !
 * @file G10/GXShader.h
 * @author Jacob Smith
 *
 * Include header for graphics, compute, mesh, and ray shaders
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
#include <G10/GXCamera.h>

enum g10_pipeline_e
{
    g10_pipeline_graphics,
    g10_pipeline_compute,
    g10_pipeline_ray
};

struct GXDescriptor_s
{
    char                         *name;
    //VkDescriptorType              type;
    size_t                        index;
    //VkDescriptorSetLayoutBinding  descriptor_set_layout_binding;
};

struct GXSet_s
{
    char                          *name;
    size_t                         index;
    //VkDescriptorSetLayout          set_layout;
    //VkDescriptorSetLayoutBinding  *p_bindings;
    size_t                         descriptor_count;
    GXDescriptor_t               **descriptors_data;
};

struct GXLayout_s
{
    GXSet_t               **sets;
    size_t                  set_count;
    VkDescriptorSetLayout   p_sets;
    VkPipelineLayout        pipeline_layout;
};

struct GXShader_s
{

    char                 *name;
    size_t                users;
    enum g10_pipeline_e   type;
    GXLayout_t           *layout;

    int                 (*pfn_draw_item)(void *, GXRenderer_t *, GXRenderPass_t *, GXSubpass_t *, GXShader_t *);

    union
    {
        struct
        {
            char                  **push_constant_properties;
            void                   *push_constant_data;

            VkShaderModule          vertex_shader_module,
                                    tessellation_control_shader_module,
                                    tessellation_evaluation_shader_module,
                                    geometry_shader_module,
                                    task_shader_module,
                                    mesh_shader_module,
                                    fragment_shader_module;

            VkPipeline              pipeline;
            VkPipelineLayout        pipeline_layout;
            VkDescriptorPool        descriptor_pool;
            VkDescriptorSet        *descriptor_sets;
            VkDescriptorSetLayout   descriptor_set_layout;
            VkDescriptorSetLayout  *descriptor_set_layouts;

            VkBuffer               *uniform_buffers;
            VkDeviceMemory         *uniform_buffers_memory;

            size_t                  push_constant_size,
                                    set_count;

            GXSet_t                *sets_data;
        } graphics; // g10_pipeline_graphics

        struct {
            VkPipeline            pipeline;
            VkShaderModule        compute_shader_module;
            u32                   x_groups,
                                  y_groups,
                                  z_groups;
        } compute; // g10_pipeline_compute

        struct {
            VkPipeline       pipeline;
            VkPipelineLayout pipeline_layout;
            VkShaderModule   ray_generation_shader_module,
                             ray_any_hit_shader_module,
                             ray_closest_hit_shader_module,
                             ray_miss_shader_module,
                             ray_intersection_shader_module,
                             ray_callable_shader_module;
        } ray; // g10_pipeline_ray
    };
};

DLLEXPORT int create_shader_module ( char *code, size_t code_len, VkShaderModule* shader_module );

// Allocators
/** !
 *  Allocate memory for a shader
 *
 * @param pp_shader : return
 *
 * @sa destroy_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_shader ( GXShader_t **pp_shader );

// Constructors
/** !
 *  Load a shader from a JSON file
 *
 * @param pp_shader : return
 * @param path      : The path to a JSON file containing a shader object
 *
 * @sa load_shader_as_json_text
 * @sa load_shader_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_shader ( GXShader_t **pp_shader, const char* path );

/** !
 *  Load a shader from JSON text
 *
 * @param pp_shader : return
 * @param text      : The shader JSON text
 *
 * @sa load_shader
 * @sa load_shader_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_shader_as_json_text ( GXShader_t **pp_shader, char* text );

/** !
 *  Load a shader from a JSON value
 *
 * @param pp_shader : return
 * @param p_value   : The camera JSON value
 *
 * @sa load_shader
 * @sa load_shader_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value );

DLLEXPORT int use_shader ( GXShader_t *p_shader );

DLLEXPORT int update_shader_push_constant ( GXShader_t  *p_shader );
DLLEXPORT int add_shader_push_constant_getter ( char *getter_name, int (*getter_function) (void *) );

DLLEXPORT int set_shader_camera ( GXEntity_t  *p_entity );

// Destructors
/** !
 *  Destroy a shader
 *
 * @param pp_shader : Pointer to pointer to shader
 *
 * @sa create_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_shader ( GXShader_t **pp_shader );