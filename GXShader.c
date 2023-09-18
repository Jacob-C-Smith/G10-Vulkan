#include <G10/GXShader.h>

#define FORMATS_COUNT                 17
#define DESCRIPTOR_TYPE_COUNT         11
#define PUSH_CONSTANT_GETTERS_COUNT   2
#define RASTERIZER_POLYGON_MODE_COUNT 3
#define BLEND_OPERATIONS_COUNT        5
#define BLEND_FACTORS_COUNT           19
#define SHADER_STAGES_COUNT           14
#define SHADER_PIPELINE_CONSTRUCTORS  3
#define DESCRIPTOR_TYPE_CONSTRUCTORS  11

// Forward declarations
int load_graphics_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value );
int load_compute_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value );
int load_ray_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value );
int load_layout_as_json_value ( GXLayout_t **pp_layout, json_value *p_value );
int load_set_as_json_value ( GXSet_t **pp_set, json_value *p_value );
int load_descriptor_as_json_value ( GXDescriptor_t **pp_descriptor, json_value *p_value );
int destroy_graphics_shader ( GXShader_t **pp_shader );
int destroy_compute_shader ( GXShader_t **pp_shader );
int destroy_ray_shader ( GXShader_t **pp_shader );

char *shader_pipeline_names [SHADER_PIPELINE_CONSTRUCTORS] =
{
    "graphics",
    "compute",
    "ray"
};
void *graphics_pipeline_type_constructors [SHADER_PIPELINE_CONSTRUCTORS] =
{
    &load_graphics_shader_as_json_value,
    &load_compute_shader_as_json_value,
    &load_ray_shader_as_json_value
};

char *format_type_names [FORMATS_COUNT] =
{
    "float",
    "vec2",
    "vec3",
    "vec4",
    "int",
    "ivec2",
    "ivec3",
    "ivec4",
    "uint",
    "uvec2",
    "uvec3",
    "uvec4",
    "double",
    "mat4",
    "mat3",
    "u8vec4",
    "u8vec4 sRGB"
};
VkFormat format_type_enums [FORMATS_COUNT] =
{
    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32A32_SINT,
    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32A32_UINT,
    VK_FORMAT_R64_SFLOAT,
    // TODO:
    0,
    0,
    VK_FORMAT_R8G8B8A8_UINT,
    VK_FORMAT_R8G8B8A8_SRGB
};
size_t format_type_sizes [FORMATS_COUNT] =
{
    4,
    8,
    12,
    16,
    4,
    8,
    12,
    16,
    4,
    8,
    12,
    16,
    8,
    64,
    36,
    32,
    32
};

char *descriptor_type_names [DESCRIPTOR_TYPE_COUNT] =
{
    "sampler",
    "combined image sampler",
    "sampled image",
    "storage image",
    "uniform texel buffer",
    "storage texel buffer",
    "uniform buffer",
    "storage buffer",
    "uniform buffer dynamic",
    "storage buffer dynamic",
    "input attachment"
};
VkDescriptorType  descriptor_type_enums [DESCRIPTOR_TYPE_COUNT] =
{
    VK_DESCRIPTOR_TYPE_SAMPLER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
};

char *push_constant_getter_names [PUSH_CONSTANT_GETTERS_COUNT] =
{
    "camera position",
    "model matrix"
};
void *push_constant_getter_functions [PUSH_CONSTANT_GETTERS_COUNT] =
{
    &get_camera_position,
    &get_model_matrix
};

char *rasterizer_polygon_mode_names [RASTERIZER_POLYGON_MODE_COUNT] =
{
    "fill",
    "line",
    "point"
};
VkPolygonMode rasterizer_polygon_mode_enum [RASTERIZER_POLYGON_MODE_COUNT] =
{
    VK_POLYGON_MODE_FILL,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_POINT
};

char *blend_operation_names [BLEND_OPERATIONS_COUNT] =
{
    "add",
    "subtract",
    "reverse subtract",
    "minimum",
    "maximum"
};
VkBlendOp blend_operation_enums [BLEND_OPERATIONS_COUNT] =
{
    VK_BLEND_OP_ADD,
    VK_BLEND_OP_SUBTRACT,
    VK_BLEND_OP_REVERSE_SUBTRACT,
    VK_BLEND_OP_MIN,
    VK_BLEND_OP_MAX
};

char *blend_factor_names [BLEND_FACTORS_COUNT] =
{
    "zero",
    "one",
    "source color",
    "one minus source color",
    "destination color",
    "one minus destination color",
    "source alpha",
    "one minus source alpha",
    "destination alpha",
    "one minus destination alpha",
    "constant color",
    "one minus constant color",
    "constant alpha",
    "one minus constant alpha",
    "source alpha saturate",
    "source one color",
    "one minus source one color",
    "source one alpha",
    "one minus source one alpha"
};
VkBlendFactor blend_factor_enums [BLEND_FACTORS_COUNT] =
{
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    VK_BLEND_FACTOR_DST_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_CONSTANT_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    VK_BLEND_FACTOR_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    VK_BLEND_FACTOR_SRC1_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    VK_BLEND_FACTOR_SRC1_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
};

char *shader_stages_names [SHADER_STAGES_COUNT] =
{
    "vertex",
    "tessellation control",
    "tessellation evaluation",
    "geometry",
    "fragment",
    "compute",
    "task",
    "mesh",
    "ray generation",
    "miss",
    "intersection",
    "closest hit",
    "any hit",
    "callable"
};
VkShaderStageFlagBits shader_stages_enum [SHADER_STAGES_COUNT] =
{
    VK_SHADER_STAGE_VERTEX_BIT,
    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    VK_SHADER_STAGE_GEOMETRY_BIT,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    VK_SHADER_STAGE_COMPUTE_BIT,
    VK_SHADER_STAGE_TASK_BIT_EXT,
    VK_SHADER_STAGE_MESH_BIT_EXT,
    VK_SHADER_STAGE_RAYGEN_BIT_KHR,
    VK_SHADER_STAGE_MISS_BIT_KHR,
    VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
    VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
    VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
    VK_SHADER_STAGE_CALLABLE_BIT_KHR
};

dict *format_types                  = 0,
     *format_sizes                  = 0,
     *descriptor_types              = 0,
     *push_constant_getters         = 0,
     *polygon_modes                 = 0,
     *blend_operations              = 0,
     *blend_factors                 = 0,
     *shader_stagesD                = 0,
     *shader_pipeline_constructors  = 0,
     *pipeline_loader_lookup_tables = 0;

typedef struct {
    char *name;
    VkDescriptorType descriptor_type;
    VkDescriptorSetLayoutBinding descriptor_set_layout_binding;
    size_t index;
} GXDescriptor_s;

int init_shader ( void )
{

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();

    // Construct lookup tables
    {
        if ( dict_construct(&format_types                 , FORMATS_COUNT)                 == 0 ) goto failed_to_allocate_format_types_lut;
        if ( dict_construct(&format_sizes                 , FORMATS_COUNT)                 == 0 ) goto failed_to_allocate_format_sizes_lut;
        if ( dict_construct(&descriptor_types             , DESCRIPTOR_TYPE_COUNT)         == 0 ) goto failed_to_allocate_descriptor_types_lut;
        if ( dict_construct(&push_constant_getters        , PUSH_CONSTANT_GETTERS_COUNT)   == 0 ) goto failed_to_allocate_push_constant_getters_lut;
        if ( dict_construct(&polygon_modes                , RASTERIZER_POLYGON_MODE_COUNT) == 0 ) goto failed_to_allocate_polygon_modes_lut;
        if ( dict_construct(&blend_operations             , BLEND_OPERATIONS_COUNT)        == 0 ) goto failed_to_allocate_blend_operations_lut;
        if ( dict_construct(&blend_factors                , BLEND_FACTORS_COUNT)           == 0 ) goto failed_to_allocate_blend_factors_lut;
        if ( dict_construct(&shader_stagesD               , SHADER_STAGES_COUNT)           == 0 ) goto failed_to_allocate_shader_stagesD_lut;
        if ( dict_construct(&pipeline_loader_lookup_tables, SHADER_PIPELINE_CONSTRUCTORS)  == 0 ) goto failed_to_allocate_pipeline_loader_lookup_tables_lut;
    }

    // Populate lookup tables
    {

        // Iterate over format counts
        for (size_t i = 0; i < FORMATS_COUNT; i++)
        {

            // Add the enumeration
            dict_add(format_types , format_type_names[i]    , (void *)format_type_enums[i]);

            // Add the type
            dict_add(format_sizes , format_type_names[i]    , (void *)format_type_sizes[i]);
        }

        // Iterate over descriptor types
        for (size_t i = 0; i < DESCRIPTOR_TYPE_COUNT; i++)

            // Add the enumeration
            dict_add(descriptor_types, descriptor_type_names[i], (void*)descriptor_type_enums[i]);

        // Iterate over push constant getters
        for (size_t i = 0; i < PUSH_CONSTANT_GETTERS_COUNT; i++)

            // Add the push constant getter
            dict_add(push_constant_getters, push_constant_getter_names[i], (void*)push_constant_getter_functions[i]);

        // Iterate over rasterizer polygon modes
        for (size_t i = 0; i < RASTERIZER_POLYGON_MODE_COUNT; i++)

            // Add the enumeration
            dict_add(polygon_modes, rasterizer_polygon_mode_names[i], (void*)rasterizer_polygon_mode_enum[i]);

        // Iterate over blend operations
        for (size_t i = 0; i < BLEND_OPERATIONS_COUNT; i++)

            // Add the enumeration
            dict_add(blend_operations, blend_operation_names[i], (void*)blend_operation_enums[i]);

        // Iterate over shader stages
        for (size_t i = 0; i < SHADER_STAGES_COUNT; i++)

            // Add the enumeration
            dict_add(shader_stagesD, shader_stages_names[i], (void*)shader_stages_enum[i]);

        // Iterate over blend factors
        for (size_t i = 0; i < BLEND_FACTORS_COUNT; i++)

            // Add the enumeration
            dict_add(blend_factors, blend_factor_names[i], (void*)blend_factor_enums[i]);

        // Iterate over pipeline constructors
        for (size_t i = 0; i < SHADER_PIPELINE_CONSTRUCTORS; i++)

            // Add the functions
            dict_add(pipeline_loader_lookup_tables, shader_pipeline_names[i], graphics_pipeline_type_constructors[i]);

    }

    // Create a shader cache mutex
    p_instance->mutexes.shader_cache = SDL_CreateMutex();

    // Success
    return 1;

    // Error handling
    {

        // G10 errors
        {
            failed_to_allocate_format_types_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate format types lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_format_sizes_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate format sizes lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_descriptor_types_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate descriptor types lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_push_constant_getters_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate push constant getters lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_polygon_modes_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate polygon modes lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_blend_operations_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate blend operations lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_blend_factors_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate blend factors lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_shader_stagesD_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate shader stages lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_pipeline_loader_lookup_tables_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate pipeline loader lookup tables lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_shader_module ( char *code, size_t code_len, VkShaderModule *shader_module )
{

    // Argument check
    #ifndef NDEBUG
        if ( code          == (void *) 0 ) goto no_code;
        if ( code_len      ==          0 ) goto no_code_len;
        if ( shader_module == (void *) 0 ) goto no_shader_module;
    #endif

    // Initialized data
    GXInstance_t             *p_instance                = g_get_active_instance();
    VkShaderModuleCreateInfo  shader_module_create_info =
    {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code_len,
        .pCode    = code
    };
    VkResult r = vkCreateShaderModule(p_instance->vulkan.device, &shader_module_create_info, (void*)0, shader_module);

    // Error check
    switch ( r )
    {
        case VK_SUCCESS:
            goto success;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            goto vulkan_out_of_host_memory;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            goto vulkan_out_of_device_memory;
        case VK_ERROR_INVALID_SHADER_NV:
            goto vulkan_invalid_shader_NV;
        default:
            goto vulkan_unknown;
    }

    success:

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_code:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"code\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_code_len:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"code_len\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"shader_module\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Vulkan errors
        {
            vulkan_out_of_host_memory:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned out of host memory, in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vulkan_out_of_device_memory:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned out of device memory,  in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vulkan_invalid_shader_NV:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned invalid shader NV, in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vulkan_unknown:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned an unknown error, in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_shader ( GXShader_t **pp_shader )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // Initialized data
    GXShader_t *p_shader = calloc(1, sizeof(GXShader_t));

    // Check memory
    if ( p_shader == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_shader = p_shader;

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_layout ( GXLayout_t **pp_layout )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_layout == (void *) 0 ) goto no_layout;
    #endif

    // Initialized data
    GXLayout_t *p_layout = calloc(1, sizeof(GXLayout_t));

    // Error check
    if ( p_layout == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_layout = p_layout;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_layout:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_layout\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_set ( GXSet_t **pp_set )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_set == (void *) 0 ) goto no_set;
    #endif

    // Initialized data
    GXSet_t *p_set = calloc(1, sizeof(GXSet_t));

    // Error check
    if ( p_set == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_set = p_set;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_descriptor ( GXDescriptor_t **pp_descriptor )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_descriptor == (void *) 0 ) goto no_descriptor;
    #endif

    // Initialized data
    GXDescriptor_t *p_descriptor = calloc(1, sizeof(GXDescriptor_t));

    // Error check
    if ( p_descriptor == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_descriptor = p_descriptor;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_descriptor:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_descriptor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_shader ( GXShader_t **pp_shader, const char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
        if ( path      == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXShader_t *p_shader = dict_get(p_instance->cache.shaders, (char *)path);
    size_t  len  = 0;
    char   *text = 0;

    if ( p_shader )
    {

        // Return a pointer to the caller
        *pp_shader = p_shader;

        // Increment the users
        p_shader->users++;

        // Success
        return 1;
    }

    len  = g_load_file(path, (void *) 0, false);
    text = calloc(len + 1, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load a file
    if ( g_load_file(path, text, false) == 0 ) goto failed_to_load_file;

    // Load a shader as JSON text
    if ( load_shader_as_json_text(pp_shader, text) == 0 ) goto failed_to_load_shader_as_json_text;

    // Clean the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_shader_as_json_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load shader as JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_shader_as_json_text ( GXShader_t **pp_shader, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
        if ( text      == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    json_value *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto no_mem;

    // Load a shader as JSON text
    if ( load_shader_as_json_value(pp_shader, p_value) == 0 ) goto failed_to_load_shader_as_json_value;

    // Clean the scope
    // free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {

            failed_to_load_shader_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load shader as JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
        if ( p_value   == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t  *p_instance                  = g_get_active_instance();
    GXShader_t    *p_shader                    = 0;
    json_value   *p_type                      = 0,
                  *p_name                      = 0,
                  *p_vertex_shader_path        = 0,
                  *p_geometry_shader_path      = 0,
                  *p_fragment_shader_path      = 0,
                  *p_compute_shader_path       = 0,
                  *p_rasterizer_json           = 0,
                  *p_vertex_shader_json_data   = 0,
                  *p_geometry_shader_json_data = 0,
                  *p_fragment_shader_json_data = 0,
                  *p_compute_shader_json_data  = 0,
                  *p_vertex_groups             = 0,
                  *p_attachments               = 0,
                  *p_sets                      = 0,
                  *p_push_constant_text        = 0;
    size_t         set_count                   = 0;

    // Load a shader as a JSON object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Get the pipeline type
        p_name = dict_get(p_dict, "name");
        p_type = dict_get(p_dict, "type");

        if ( p_name )
        {

            // Check for the right type
            if ( p_name->type == JSON_VALUE_STRING )
            {

                // Initialized data
                GXShader_t *p_shader = dict_get(p_instance->cache.shaders, p_name->string);

                if ( p_shader )
                {

                    // Return a pointer to the caller
                    *pp_shader = p_shader;

                    // Increment the users
                    p_shader->users++;

                    // Success
                    return 1;
                }

            }
            else
                goto wrong_name_type;
        }
        else
            goto no_name_property;

        // Was a pipeline type provided?
        if ( p_type )
        {

            // Get a function pointer from the lookup table
            int (*pipeline_load_as_json_value)(GXShader_t **pp_shader, json_value *p_value) = dict_get(pipeline_loader_lookup_tables, p_type->string);

            // If the function is valid...
            if ( pipeline_load_as_json_value )

                // ... call the function
                if ( pipeline_load_as_json_value(pp_shader, p_value) == 0 ) goto failed_to_parse_shader_pipeline;
        }
        else
            goto unrecognized_type;
    }
    // Load the shader as a path
    else if ( p_value->type == JSON_VALUE_STRING )
    {

        // Load the shader as a path
        if ( load_shader(pp_shader, p_value->string) == 0 ) goto failed_to_load_shader_as_path;

        // Success
        return 1;
    }
    // Type error
    else
        goto wrong_type;

    (*pp_shader)->users++;
    dict_add(p_instance->cache.shaders,(*pp_shader)->name,*pp_shader);

    // Success
    return 1;

    wrong_name_type:
    no_name_property:
        return 0;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {

            failed_to_load_shader_as_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load shader from path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            // TODO: More specific
            failed_to_parse_shader_pipeline:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load shader in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load shader in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            unrecognized_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Unrecognized type property value in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int use_shader ( GXShader_t *p_shader )
{

    // Argument errors
    #ifndef NDEBUG
        if ( p_shader ) goto no_shader;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    VkRect2D scissor =
    {
        .offset.x = 0,
        .offset.y = 0,
        .extent   = p_instance->vulkan.swap_chain.extent
    };
    VkViewport viewport =
    {
        .x        = 0.f,
        .y        = 0.f,
        .width    = (float)p_instance->vulkan.swap_chain.extent.width,
        .height   = (float)p_instance->vulkan.swap_chain.extent.height,
        .minDepth = 0.f,
        .maxDepth = 1.f
    };

    // Use the shader
    vkCmdBindPipeline(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, p_shader->graphics.pipeline);

    // Set the viewport
    vkCmdSetViewport(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0, 1, &viewport);

    // Set the scissors
    vkCmdSetScissor(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0, 1, &scissor);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/*
int update_shader_push_constant ( GXShader_t *shader )
{
    size_t offset = 0;

    for (size_t i = 0; shader->graphics.push_constant_properties[i]; i++)
    {
        char   *push_constant_property_name = shader->graphics.push_constant_properties[i];
        int   (*getter) ( void * )          = (int (*)( void * )) dict_get(push_constant_getters, push_constant_property_name);

        if (getter)
            offset = getter((u8*)shader->graphics.push_constant_data + offset);

    }
    return 1;
}

int add_shader_push_constant_getter ( char *getter_name, int(*getter_function)(void*) )
{

    // Argument errors
    {
        #ifndef NDEBUG
            if (getter_name == (void *)0)
                goto no_getter_name;
            if (getter_function == (void*)0)
                goto no_getter_function;
        #endif
    }

    // Error check
    {
        #ifndef NDEBUG
            if (dict_get(push_constant_getters, getter_name))
                goto already_has_getter;
        #endif
    }

    dict_add(push_constant_getters, getter_name, getter_function);

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_getter_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"getter_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            no_getter_function:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"getter_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // User errors
        {
            already_has_getter:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to add getter in call to function \"%s\". There is already a getter called \"%s\"\n", __FUNCTION__, getter_name);
                #endif

                // Error
                return 0;
        }
    }
}

int set_shader_camera               ( GXEntity_t   *p_entity )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_entity == (void *) 0 )
                goto no_shader;
        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance = g_get_active_instance();
    GXCamera_t    *camera     = p_instance->context.scene->active_camera;
    GXTransform_t *transform  = p_entity->transform;

    mat4 uniform_buffer[4];

    {
        // Update the transform
        transform_model_matrix(transform, &uniform_buffer[0]);

        vec3 a;
        add_vec3(&a, camera->target, camera->location);

        // Update the view matrix
        camera->view = look_at(camera->location, a, camera->up);

        // Update the projection matrix
        camera->projection = perspective_matrix(camera->fov, camera->aspect_ratio, camera->near_clip, camera->far_clip);

        uniform_buffer[1] = camera->view;
        uniform_buffer[2] = camera->projection;

        uniform_buffer[3].a = camera->location.x;
        uniform_buffer[3].b = camera->location.y;
        uniform_buffer[3].c = camera->location.z;
    }

    void *data;

    vkMapMemory(p_instance->vulkan.device, p_entity->shader->graphics.uniform_buffers_memory[p_instance->vulkan.current_frame], 0, sizeof(vec3) + 3 * sizeof(mat4), 0, &data);
    memcpy(data, uniform_buffer, sizeof(vec3) + 3*sizeof(mat4));
    vkUnmapMemory(p_instance->vulkan.device, p_entity->shader->graphics.uniform_buffers_memory[p_instance->vulkan.current_frame]);

    return 1;

    // Error handling
    {

        // Argument error
        {
            #ifndef NDEBUG
                no_shader:
                    g_print_log("[G10] [Shader] Null pointer provided for parameter \"shader\" in call to function \"%s\"\n", __FUNCTION__);
                    return 0;
            #endif
        }
    }
}
*/
int destroy_shader ( GXShader_t **pp_shader )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // TODO: Cache destruction

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXShader_t   *p_shader = *pp_shader;

    // Error check
    if ( p_shader == (void *) 0 ) goto pointer_to_null_pointer;

    // Destroy the pipeline
    vkDestroyPipeline(p_instance->vulkan.device, p_shader->graphics.pipeline, 0);

    // Destroy the pipeline layout
    vkDestroyPipelineLayout(p_instance->vulkan.device, p_shader->graphics.pipeline_layout, 0);

    // Destroy the shader modules
    {

    }

    vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.vertex_shader_module, 0);
    vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.fragment_shader_module, 0);

    free(p_shader->name);
    free(p_shader);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Parameter \"pp_shader\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_graphics_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value )
{
    // Argument notes
    // p_value will always be valid. p_value->type will always be JSON_VALUE_OBJECT

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // Initialized data
    GXInstance_t *p_instance                            = g_get_active_instance();
    GXShader_t   *p_shader                              = 0;
    json_value  *p_name                                = 0,
                 *p_vertex_shader_path                  = 0,
                 *p_tessellation_control_shader_path    = 0,
                 *p_tessellation_evaluation_shader_path = 0,
                 *p_task_shader_path                    = 0,
                 *p_mesh_shader_path                    = 0,
                 *p_geometry_shader_path                = 0,
                 *p_fragment_shader_path                = 0,
                 *p_multisampler                        = 0,
                 *p_layout                              = 0,
                 *p_in                                  = 0,
                 *p_attachments                         = 0,
                 *p_layout_sets                         = 0,
                 *p_layout_push_constant                = 0,
                 *p_rasterizer                          = 0;

    // Parse the JSON value
    {

        // Initialized data
        dict *p_dict = p_value->object;

        p_name                                = dict_get(p_dict, "name");
        p_vertex_shader_path                  = dict_get(p_dict, "vertex shader path");
        p_tessellation_control_shader_path    = dict_get(p_dict, "tessellation control shader path");
        p_tessellation_evaluation_shader_path = dict_get(p_dict, "tessellation evaluation shader path");
        p_geometry_shader_path                = dict_get(p_dict, "geometry shader path");
        p_task_shader_path                    = dict_get(p_dict, "task shader path");
        p_mesh_shader_path                    = dict_get(p_dict, "mesh shader path");
        p_fragment_shader_path                = dict_get(p_dict, "fragment shader path");
        p_multisampler                        = dict_get(p_dict, "multisampler");
        p_layout                              = dict_get(p_dict, "layout");
        p_in                                  = dict_get(p_dict, "in");
        p_attachments                         = dict_get(p_dict, "attachments");
        p_rasterizer                          = dict_get(p_dict, "rasterizer");

        // Check properties
        if ( ! ( p_name && p_fragment_shader_path ) )                                                   goto missing_properties;
        if ( p_in == (void *) 0 )                                                                       goto missing_properties;
        if ( p_vertex_shader_path && p_task_shader_path && p_mesh_shader_path )                         goto missing_properties;
        if ( ( (bool)(p_vertex_shader_path) ^ (bool)(p_task_shader_path && p_mesh_shader_path) ) == 0 ) goto missing_properties;
    }

    // TODO: Cache check

    // Construct the shader
    {

        // Allocate memory for a shader
        if ( create_shader(&p_shader) == 0 ) goto failed_to_allocate_shader;

        // Set the shader type
        p_shader->type = g10_pipeline_graphics;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_shader->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_shader->name, p_name->string, name_len);
        }
        else
            goto name_wrong_type;

        // Load the layout
        if ( p_layout )
        {
            if ( load_layout_as_json_value(&p_shader->layout, p_layout) == 0 ) goto failed_to_load_layout_as_json_value;
        }

        // Load the shader binaries
        {

            // Load the vertex shader binary
            if ( p_vertex_shader_path )
            {

                // Parse the vertex shader path as a string
                if ( p_vertex_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t vertex_shader_data_len = g_load_file(p_vertex_shader_path->string, 0, true);
                    char *vertex_shader_data = calloc(vertex_shader_data_len, sizeof(char));

                    // Error check
                    if ( vertex_shader_data == (void *) 0 ) goto no_mem;

                    // Load the vertex shader binary file
                    if ( g_load_file(p_vertex_shader_path->string, vertex_shader_data, true) == 0 ) goto failed_to_load_vertex_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(vertex_shader_data, vertex_shader_data_len, &p_shader->graphics.vertex_shader_module) == 0 ) goto failed_to_create_vertex_shader_module;

                    // Clean the scope
                    free(vertex_shader_data);
                }
                else
                    goto wrong_vertex_shader_path_type;
            }

            // Load tessellation control shader
            if ( p_tessellation_control_shader_path )
            {

                // Parse the tessellation control shader path as a string
                if ( p_tessellation_control_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t tessellation_control_shader_data_len = g_load_file(p_tessellation_control_shader_path->string, 0, true);
                    char* tessellation_control_shader_data = calloc(tessellation_control_shader_data_len, sizeof(char));

                    // Error check
                    if ( tessellation_control_shader_data == (void *) 0 ) goto no_mem;

                    // Load the tessellation_control shader binary file
                    if ( g_load_file(p_tessellation_control_shader_path->string, tessellation_control_shader_data, true) == 0 ) goto failed_to_load_tessellation_control_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(tessellation_control_shader_data, tessellation_control_shader_data_len, &p_shader->graphics.tessellation_control_shader_module) == 0 ) goto failed_to_create_tessellation_control_shader_module;

                    // Clean the scope
                    free(tessellation_control_shader_data);
                }
                else
                    goto wrong_tessellation_control_shader_path_type;
            }

            // Load tessellation evaluation shader
            if ( p_tessellation_evaluation_shader_path )
            {

                // Parse the tessellation evaluation shader path as a string
                if ( p_tessellation_evaluation_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t tessellation_evaluation_shader_data_len = g_load_file(p_tessellation_evaluation_shader_path->string, 0, true);
                    char* tessellation_evaluation_shader_data = calloc(tessellation_evaluation_shader_data_len, sizeof(char));

                    // Error check
                    if ( tessellation_evaluation_shader_data == (void *) 0 ) goto no_mem;

                    // Load the tessellation evaluation shader binary file
                    if ( g_load_file(p_tessellation_evaluation_shader_path->string, tessellation_evaluation_shader_data, true) == 0 ) goto failed_to_load_tessellation_evaluation_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(tessellation_evaluation_shader_data, tessellation_evaluation_shader_data_len, &p_shader->graphics.tessellation_control_shader_module) == 0 ) goto failed_to_create_tessellation_evaluation_shader_module;

                    // Clean the scope
                    free(tessellation_evaluation_shader_data);
                }
                else
                    goto wrong_tessellation_evaluation_shader_path_type;
            }

            // Load the geometry shader binary
            if ( p_geometry_shader_path )
            {

                // Parse the geometry shader path as a string
                if ( p_geometry_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t geometry_shader_data_len = g_load_file(p_geometry_shader_path->string, 0, true);
                    char* geometry_shader_data = calloc(geometry_shader_data_len, sizeof(char));

                    // Error check
                    if ( geometry_shader_data == (void *) 0 ) goto no_mem;

                    // Load the geometry shader binary file
                    if ( g_load_file(p_geometry_shader_path->string, geometry_shader_data, true) == 0 ) goto failed_to_load_geometry_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(geometry_shader_data, geometry_shader_data_len, &p_shader->graphics.geometry_shader_module) == 0 ) goto failed_to_create_geometry_shader_module;

                    // Clean the scope
                    free(geometry_shader_data);
                }
                else
                    goto wrong_geometry_shader_path_type;
            }

            // Load the task shader binary
            if ( p_task_shader_path )
            {

                // Parse the task shader path as a string
                if ( p_task_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t task_shader_data_len = g_load_file(p_task_shader_path->string, 0, true);
                    char* task_shader_data = calloc(task_shader_data_len, sizeof(char));

                    // Error check
                    if ( task_shader_data == (void *) 0 ) goto no_mem;

                    // Load the task shader binary file
                    if ( g_load_file(p_task_shader_path->string, task_shader_data, true) == 0 ) goto failed_to_load_task_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(task_shader_data, task_shader_data_len, &p_shader->graphics.task_shader_module) == 0 ) goto failed_to_create_task_shader_module;

                    // Clean the scope
                    free(task_shader_data);
                }
                else
                    goto wrong_task_shader_path_type;
            }

            // Load the mesh shader binary
            if ( p_mesh_shader_path )
            {

                // Parse the mesh shader path as a string
                if ( p_mesh_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t mesh_shader_data_len = g_load_file(p_mesh_shader_path->string, 0, true);
                    char* mesh_shader_data = calloc(mesh_shader_data_len, sizeof(char));

                    // Error check
                    if ( mesh_shader_data == (void *) 0 ) goto no_mem;

                    // Load the mesh shader binary file
                    if ( g_load_file(p_mesh_shader_path->string, mesh_shader_data, true) == 0 ) goto failed_to_load_mesh_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(mesh_shader_data, mesh_shader_data_len, &p_shader->graphics.mesh_shader_module) == 0 ) goto failed_to_create_mesh_shader_module;

                    // Clean the scope
                    free(mesh_shader_data);
                }
                else
                    goto wrong_mesh_shader_path_type;
            }

            // Load the fragment shader binary
            if ( p_fragment_shader_path )
            {

                // Parse the fragment shader path as a string
                if ( p_fragment_shader_path->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t fragment_shader_data_len = g_load_file(p_fragment_shader_path->string, 0, true);
                    char* fragment_shader_data = calloc(fragment_shader_data_len + 1, sizeof(char));

                    // Error check
                    if ( fragment_shader_data == (void *) 0 ) goto no_mem;

                    // Load the fragment shader binary file
                    if ( g_load_file(p_fragment_shader_path->string, fragment_shader_data, true) == 0 ) goto failed_to_load_fragment_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(fragment_shader_data, fragment_shader_data_len, &p_shader->graphics.fragment_shader_module) == 0 ) goto failed_to_create_fragment_shader_module;

                    // Clean the scope
                    free(fragment_shader_data);
                }
                else
                    goto wrong_fragment_shader_path_type;
            }
        }

        // Construct the graphics shader
        {

            // Initialized data
            size_t shader_stage_iterator = 0,
                   attachment_count      = 0;
            VkPipelineShaderStageCreateInfo        *shader_stages                       = calloc(8, sizeof(VkPipelineShaderStageCreateInfo));
            VkPipelineVertexInputStateCreateInfo    vertex_input_info_create_info       = { 0 };
            VkPipelineInputAssemblyStateCreateInfo  input_assembly_create_info          = { 0 };
            VkPipelineTessellationStateCreateInfo   tessellation_state_create_info      = { 0 };
            VkPipelineViewportStateCreateInfo       viewport_state_create_info          = { 0 };
            VkPipelineRasterizationStateCreateInfo  rasterizer_create_info              = { 0 };
            VkPipelineMultisampleStateCreateInfo    multisampling_create_info           = { 0 };
            VkPipelineColorBlendAttachmentState    *color_blend_attachment_create_infos = 0;
            VkPipelineColorBlendStateCreateInfo     color_blend_create_info             = { 0 };
            VkPipelineDepthStencilStateCreateInfo   depth_stencil_state_create_info     = { 0 };
            VkPipelineDynamicStateCreateInfo        dynamic_state_create_info           = { 0 };
            VkPipelineLayoutCreateInfo              pipeline_layout_create_info         = { 0 };
            VkDynamicState                          dynamic_states[2]                   = { 0 };
            VkPushConstantRange                    *push_constant                       = calloc(1, sizeof(VkPushConstantRange));
            VkGraphicsPipelineCreateInfo            graphics_pipeline_create_info       = { 0 };
            VkVertexInputBindingDescription         binding_description                 = { 0 };
            VkViewport                              viewport                            = { 0 };
            VkRect2D                                scissor                             = { 0 };
            VkVertexInputAttributeDescription      *vertex_input_attribute_descriptions = 0;
            json_value                            *p_topology                          = 0;

            //////////////////////////////
            // Set up the shader stages //
            //////////////////////////////
            {

                // Set up the vertex shader
                if ( p_shader->graphics.vertex_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_VERTEX_BIT,
                        .module = p_shader->graphics.vertex_shader_module,
                        .pName  = "main"
                    };

                }

                // Set up the tessellation control shader
                if ( p_shader->graphics.tessellation_control_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
                        .module = p_shader->graphics.tessellation_control_shader_module,
                        .pName  = "main"
                    };
                }

                // Set up the tessellation evaluation shader
                if ( p_shader->graphics.tessellation_evaluation_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
                        .module = p_shader->graphics.tessellation_evaluation_shader_module,
                        .pName  = "main"
                    };
                }

                // Set up the geometry shader
                if ( p_shader->graphics.geometry_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_GEOMETRY_BIT,
                        .module = p_shader->graphics.geometry_shader_module,
                        .pName  = "main"
                    };
                }

                // Set up the task shader
                if ( p_shader->graphics.task_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_TASK_BIT_EXT,
                        .module = p_shader->graphics.task_shader_module,
                        .pName  = "main"
                    };
                }

                // Set up the mesh shader
                if ( p_shader->graphics.mesh_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_MESH_BIT_EXT,
                        .module = p_shader->graphics.mesh_shader_module,
                        .pName  = "main"
                    };
                }

                // Set up the fragment shader
                if ( p_shader->graphics.fragment_shader_module )
                {
                    shader_stages[shader_stage_iterator++] =
                    (VkPipelineShaderStageCreateInfo)
                    {
                        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
                        .module = p_shader->graphics.fragment_shader_module,
                        .pName  = "main"
                    };
                }
            }

            ///////////////////////////////////
            // Set up the vertex input state //
            ///////////////////////////////////
            if ( p_in->type == JSON_VALUE_OBJECT )
            {

                // Initialized data
                size_t       vertex_group_count        = 0,
                             stride                    = 0,
                             binding_description_count = 0;
                json_value *p_vertex_attributes       = 0;

                // Parse the JSON object
                {

                    // Initialized data
                    dict *p_dict = p_in->object;

                    // Required properties
                    p_vertex_attributes = dict_get(p_dict, "vertex attributes");
                    p_topology          = dict_get(p_dict, "topology");

                    // Error check
                    if ( ! (
                        p_vertex_attributes &&
                        p_topology
                    ) )
                        goto missing_properties;
                }

                // Did the user specify vertex groups?
                if ( p_vertex_attributes->type == JSON_VALUE_ARRAY )
                {

                    // Initialized data
                    json_value **pp_vertex_groups = 0;

                    // Get the contents of the array
                    if ( p_vertex_attributes->type == JSON_VALUE_ARRAY )
                    {

                        // Get the array size
                        array_get(p_vertex_attributes->list, 0, &vertex_group_count);

                        // Allocate for vertex group JSON values
                        pp_vertex_groups = calloc(vertex_group_count+1, sizeof(json_value *));

                        // Error check
                        if ( pp_vertex_groups == (void *) 0 ) goto no_mem;

                        // Get the array contents
                        array_get(p_vertex_attributes->list, (void **)pp_vertex_groups, 0);
                    }

                    // Allocate vertex attributes
                    vertex_input_attribute_descriptions = calloc(vertex_group_count, sizeof(VkVertexInputAttributeDescription));

                    // Iterate over each vertex group
                    for (size_t i = 0; i < vertex_group_count; i++)
                    {

                        // Initialized data
                        dict *vertex_group = 0;
                        char *input_attribute_type = 0;
                        signed input_attribute_location = 0;
                        json_value *p_vertex_group = pp_vertex_groups[i],
                                    *p_input_attribute_type = 0;

                        // Parse the JSON value
                        if ( p_vertex_group->type == JSON_VALUE_OBJECT )
                        {

                            // Initialized data
                            dict *p_dict = p_vertex_group->object;

                            // Required properties
                            p_input_attribute_type = dict_get(p_dict, "type");

                            // Error check
                            if ( ! ( p_input_attribute_type ) ) goto wrong_input_attribute_properties;

                        }
                        else
                            goto vertex_group_wrong_type;

                        if ( p_input_attribute_type->type == JSON_VALUE_STRING )
                            input_attribute_type = p_input_attribute_type->string;

                        if ( p_input_attribute_type->type == JSON_VALUE_INTEGER )
                            input_attribute_location = i;

                        // Construct a vertex input attribute
                        vertex_input_attribute_descriptions[i] =
                        (VkVertexInputAttributeDescription)
                        {
                            .binding  = 0,
                            .location = i,
                            .format   = (VkFormat) (size_t) dict_get(format_types, input_attribute_type),
                            .offset   = (u32)stride
                        };

                        // Accumulate stride
                        stride += (size_t) dict_get(format_sizes, input_attribute_type);
                    }

                    // Construct a binding description
                    binding_description =
                    (VkVertexInputBindingDescription)
                    {
                        .binding   = 0,
                        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                        .stride    = (u32)stride
                    };
                }

                // Set up the vertex input
                vertex_input_info_create_info =
                (VkPipelineVertexInputStateCreateInfo)
                {
                    .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                    .pNext                           = 0,
                    .flags                           = 0,
                    .vertexBindingDescriptionCount   = (u32)1,
                    .pVertexBindingDescriptions      = &binding_description,
                    .vertexAttributeDescriptionCount = (u32)vertex_group_count,
                    .pVertexAttributeDescriptions    = vertex_input_attribute_descriptions
                };
            }
            // ^^^ TODO: Default ^^^
            else
                ;

            ///////////////////////////////
            // Set up the input assembly //
            ///////////////////////////////
            input_assembly_create_info =
            (VkPipelineInputAssemblyStateCreateInfo)
            {
                .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .pNext                  = 0,
                .flags                  = 0,
                .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // TODO: Replace with p_topology->string
                .primitiveRestartEnable = VK_FALSE,
            };

            ///////////////////////////////////
            // Set up the tessellation state //
            ///////////////////////////////////
            tessellation_state_create_info =
            (VkPipelineTessellationStateCreateInfo) { 0 };

            ///////////////////////////////
            // Set up the viewport state //
            ///////////////////////////////
            {

                // Set up the viewport
                viewport =
                (VkViewport)
                {
                    .x        = 0.f,
                    .y        = 0.f,
                    .width    = (float)p_instance->vulkan.swap_chain.extent.width,
                    .height   = (float)p_instance->vulkan.swap_chain.extent.height,
                    .minDepth = 0.0f,
                    .maxDepth = 1.0f
                };

                // Set up the scissor
                scissor =
                (VkRect2D)
                {
                    .offset.x = 0,
                    .offset.y = 0,
                    .extent   = p_instance->vulkan.swap_chain.extent
                };

                // Populate the viewport state create struct
                viewport_state_create_info =
                (VkPipelineViewportStateCreateInfo)
                {
                    .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                    .viewportCount = 1,
                    .pViewports    = &viewport,
                    .scissorCount  = 1,
                    .pScissors     = &scissor
                };
            }

            ///////////////////////////
            // Set up the rasterizer //
            ///////////////////////////
            if ( p_rasterizer )
            {

                // Parse the multisampler as a JSON object
                if ( p_rasterizer->type == JSON_VALUE_OBJECT )
                {

                    // Initialized data
                    VkPolygonMode polygon_mode                 = VK_POLYGON_MODE_FILL;
                    json_value  *p_line_width                 = 0,
                                 *p_depth_bias_constant_factor = 0,
                                 *p_depth_bias_clamp           = 0,
                                 *p_depth_bias_slope_factor    = 0,
                                 *p_depth_clamp_enable         = 0,
                                 *p_rasterizer_discard_enable  = 0,
                                 *p_clockwise                  = 0,
                                 *p_depth_bias_enable          = 0;
                    float         line_width                   = 1.f,
                                  depth_bias_constant_factor   = 0.f,
                                  depth_bias_clamp             = 0.f,
                                  depth_bias_slope_factor      = 0.f;
                    bool          depth_clamp_enable           = false,
                                  rasterizer_discard_enable    = false,
                                  clockwise                    = false,
                                  depth_bias_enable            = false;

                    // Parse the rasterizer
                    {

                        // Initialized data
                        dict *p_dict = p_rasterizer->object;

                        // Optional properties
                        p_line_width                 = dict_get(p_dict, "line width");                 // Default to 1.0
                        p_depth_bias_constant_factor = dict_get(p_dict, "depth bias constant factor"); // Default to 0.0
                        p_depth_bias_clamp           = dict_get(p_dict, "depth bias clamp");           // Default to 0.0
                        p_depth_bias_slope_factor    = dict_get(p_dict, "depth bias slope factor");    // Default to 0.0
                        p_depth_clamp_enable         = dict_get(p_dict, "depth clamp enable");         // Default to false
                        p_rasterizer_discard_enable  = dict_get(p_dict, "rasterizer discard enable");  // Default to false
                        p_clockwise                  = dict_get(p_dict, "clockwise");                  // Default to false
                        p_depth_bias_enable          = dict_get(p_dict, "depth bias enable");          // Default to false
                    }

                    // Construct the rasterizer
                    {

                        if ( p_line_width )
                        {

                            // Check for the right type
                            if ( p_line_width->type == JSON_VALUE_NUMBER )
                                line_width = (float) p_line_width->number;
                            // Default
                            else
                                goto no_rasterizer_line_width;
                        }

                        if ( p_depth_bias_constant_factor )
                        {

                            // Check for the right type
                            if ( p_depth_bias_constant_factor->type == JSON_VALUE_NUMBER )
                                depth_bias_constant_factor = (float) p_depth_bias_constant_factor->number;
                            // Default
                            else
                                goto no_rasterizer_depth_bias_constant_factor;
                        }

                        if ( p_depth_bias_clamp )
                        {

                            // Check for the right type
                            if ( p_depth_bias_clamp->type == JSON_VALUE_NUMBER )
                                depth_bias_clamp = (float) p_depth_bias_clamp->number;
                            // Default
                            else
                                goto no_rasterizer_depth_bias_clamp;
                        }

                        if ( p_depth_bias_slope_factor )
                        {

                            // Check for the right type
                            if ( p_depth_bias_slope_factor->type == JSON_VALUE_NUMBER )
                                depth_bias_slope_factor = (float) p_depth_bias_slope_factor->number;
                            // Default
                            else
                                goto no_rasterizer_depth_bias_slope_factor;
                        }

                        if ( p_depth_clamp_enable )
                        {

                            // Check for the right type
                            if ( p_depth_clamp_enable->type == JSON_VALUE_BOOLEAN )
                                depth_clamp_enable = p_depth_clamp_enable->boolean;
                            // Default
                            else
                                goto no_depth_clamp_enable;
                        }

                        if ( p_rasterizer_discard_enable )
                        {

                            // Check for the right type
                            if ( p_rasterizer_discard_enable->type == JSON_VALUE_BOOLEAN )
                                rasterizer_discard_enable = p_rasterizer_discard_enable->boolean;
                            // Default
                            else
                                goto no_rasterizer_rasterizer_discard_enable;
                        }

                        if ( p_clockwise )
                        {

                            // Check for the right type
                            if ( p_clockwise->type == JSON_VALUE_BOOLEAN )
                                clockwise = p_clockwise->boolean;
                            // Default
                            else
                                goto no_rasterizer_clockwise;
                        }

                        if ( p_depth_bias_enable )
                        {

                            // Check for the right type
                            if ( p_depth_bias_enable->type == JSON_VALUE_BOOLEAN )
                                depth_bias_enable = p_depth_bias_enable->boolean;
                            // Default
                            else
                                goto no_rasterizer_depth_bias_enable;
                        }
                    }

                    // Populate the rasterizer create struct
                    rasterizer_create_info =
                    (VkPipelineRasterizationStateCreateInfo)
                    {
                        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                        .pNext                   = 0,
                        .flags                   = 0,
                        .depthClampEnable        = depth_clamp_enable,
                        .rasterizerDiscardEnable = rasterizer_discard_enable,
                        .polygonMode             = polygon_mode,
                        .cullMode                = VK_CULL_MODE_FRONT_BIT,
                        .frontFace               = clockwise,
                        .depthBiasEnable         = depth_bias_enable,
                        .depthBiasConstantFactor = depth_bias_constant_factor,
                        .depthBiasClamp          = depth_bias_clamp,
                        .depthBiasSlopeFactor    = depth_bias_slope_factor,
                        .lineWidth               = line_width,
                    };
                }
                else
                    goto wrong_rasterizer_type;
            }
            // Default
            else
            {
                // Populate the rasterizer create struct
                rasterizer_create_info =
                (VkPipelineRasterizationStateCreateInfo)
                {
                    .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                    .pNext                   = 0,
                    .flags                   = 0,
                    .depthClampEnable        = false,
                    .rasterizerDiscardEnable = false,
                    .polygonMode             = VK_POLYGON_MODE_FILL,
                    .cullMode                = VK_CULL_MODE_FRONT_BIT,
                    .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                    .depthBiasEnable         = true,
                    .depthBiasConstantFactor = 0.0,
                    .depthBiasClamp          = 0.0,
                    .depthBiasSlopeFactor    = 0.0,
                    .lineWidth               = 1.0
                };
            }

            /////////////////////////////
            // Set up the multisampler //
            /////////////////////////////
            if ( p_multisampler )
            {

                // Parse the multisampler as a JSON object
                if ( p_multisampler->type == JSON_VALUE_OBJECT )
                {

                    // Initialized data
                    json_value *p_samples                  = 0,
                                *p_sample_shading_enable    = 0,
                                *p_minimum_sample_shading   = 0,
                                *p_alpha_to_coverage_enable = 0,
                                *p_alpha_to_one_enable      = 0;
                    bool         sample_shading_enable      = false,
                                 alpha_to_coverage_enable   = false,
                                 alpha_to_one_enable        = false;
                    signed       samples                    = 1;
                    float        minimum_sample_shading     = 0.0;

                    // Parse the JSON
                    {

                        // Initialized data
                        dict *p_dict = p_multisampler->object;

                        // Optional properties
                        p_samples                  = dict_get(p_dict, "samples");                  // Default to 1
                        p_sample_shading_enable    = dict_get(p_dict, "sample shading enable");    // Default to false
                        p_minimum_sample_shading   = dict_get(p_dict, "minimum sample shading");   // Default to 0.0
                        p_alpha_to_coverage_enable = dict_get(p_dict, "alpha to coverage enable"); // Default to false
                        p_alpha_to_one_enable      = dict_get(p_dict, "alpha to one enable");      // Default to false
                    }

                    // Parse the multisampler
                    {

                        if ( p_samples )
                        {
                            if ( p_samples->type == JSON_VALUE_INTEGER )
                                samples = (signed int) p_samples->integer;
                            else
                                goto no_multisampler_samples;
                        }

                        if ( p_sample_shading_enable )
                        {
                            if ( p_sample_shading_enable->type == JSON_VALUE_BOOLEAN )
                                sample_shading_enable = p_sample_shading_enable->boolean;
                            else
                                goto no_multisampler_sample_shading_enable;
                        }

                        if ( p_alpha_to_coverage_enable )
                        {
                            if ( p_alpha_to_coverage_enable->type == JSON_VALUE_BOOLEAN )
                                alpha_to_coverage_enable = p_alpha_to_coverage_enable->boolean;
                            else
                                goto no_multisampler_alpha_to_coverage_enable;
                        }

                        if ( p_alpha_to_one_enable )
                        {
                            if ( p_alpha_to_one_enable->type == JSON_VALUE_BOOLEAN )
                                alpha_to_one_enable = p_alpha_to_one_enable->boolean;
                            else
                                goto no_multisampler_alpha_to_one_enable;
                        }

                        if ( p_minimum_sample_shading )
                        {
                            if ( p_minimum_sample_shading->type == JSON_VALUE_NUMBER )
                                minimum_sample_shading = (float) p_minimum_sample_shading->number;
                            else
                                goto no_minimum_sample_shading;
                        }
                    }

                    multisampling_create_info =
                    (VkPipelineMultisampleStateCreateInfo)
                    {
                        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                        .pNext                 = 0,
                        .flags                 = 0,
                        .rasterizationSamples  = samples,
                        .sampleShadingEnable   = sample_shading_enable,
                        .minSampleShading      = minimum_sample_shading,
                        .pSampleMask           = 0,
                        .alphaToCoverageEnable = alpha_to_coverage_enable,
                        .alphaToOneEnable      = alpha_to_one_enable
                    };
                }
                // Default
                else
                    goto wrong_multisampler_type;
            }
            // Default
            else
            {
                multisampling_create_info =
                (VkPipelineMultisampleStateCreateInfo)
                {
                    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                    .pNext                 = 0,
                    .flags                 = 0,
                    .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
                    .sampleShadingEnable   = VK_FALSE,
                    .minSampleShading      = 1.f,
                    .pSampleMask           = 0,
                    .alphaToCoverageEnable = 0,
                    .alphaToOneEnable      = 0
                };
            }

            //////////////////////////////////////
            // Set up the depth / stencil state //
            //////////////////////////////////////
            depth_stencil_state_create_info =
            (VkPipelineDepthStencilStateCreateInfo)
            {
                .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .depthTestEnable       = VK_TRUE,
                .depthWriteEnable      = VK_TRUE,
                .depthCompareOp        = VK_COMPARE_OP_LESS,
                .depthBoundsTestEnable = VK_FALSE,
                .minDepthBounds        = 0.f,
                .maxDepthBounds        = 1.f,
                .stencilTestEnable     = VK_FALSE
            };


            // TODO: Parse as "attachments" property

            if ( p_attachments->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                dict         *attachments            = p_instance->context.loading_renderer->current_render_pass->attachments;
                size_t        attachments_count      = 0;
                json_value **pp_attachments         = 0;
                size_t        attachment_names_count = dict_values(attachments, 0);
                char        **pp_attachment_names    = 0;

                // Get the array
                {

                    // Get the array size
                    attachments_count = array_get(p_attachments->list, 0, &attachments_count);

                    // Allocate memory for an array
                    pp_attachments = calloc(attachments_count, sizeof(json_value *));

                    // Error check
                    if ( pp_attachments == (void *) 0 ) goto no_mem;

                    // Get the array contents
                    array_get(p_attachments->list, (void **)pp_attachments, 0);

                }

                // Get the name of each attachment
                attachment_count = attachments_count;
                dict_values(attachments, (void **) pp_attachment_names);
                pp_attachment_names    = calloc(attachments_count, sizeof(GXAttachment_t *));

                // TODO: Allocate pAttachments (color_blend_attachment_create_info)
                color_blend_attachment_create_infos = calloc(attachments_count, sizeof(VkPipelineColorBlendAttachmentState));

                // Error check
                if ( color_blend_attachment_create_infos == (void *) 0 ) goto no_mem;

                // Iterate over each attachment
                for (size_t i = 0; i < attachments_count; i++)
                {

                    // Initialized data
                    json_value *i_attachment_value               = pp_attachments[i],
                                *p_blend_enable                   = 0,
                                *p_source_color_blend_factor      = 0,
                                *p_destination_color_blend_factor = 0,
                                *p_color_blend_operation          = 0,
                                *p_source_alpha_blend_factor      = 0,
                                *p_destination_alpha_blend_factor = 0,
                                *p_alpha_blend_operation          = 0;

                    // Parse the JSON
                    {

                        // Initialized data
                        dict *p_dict = i_attachment_value->object;

                        // Required properties
                        p_blend_enable                   = dict_get(p_dict, "blend enable");
                        p_source_color_blend_factor      = dict_get(p_dict, "source color blend factor");
                        p_destination_color_blend_factor = dict_get(p_dict, "destination color blend factor");
                        p_color_blend_operation          = dict_get(p_dict, "color blend operation");
                        p_source_alpha_blend_factor      = dict_get(p_dict, "source alpha blend factor");
                        p_destination_alpha_blend_factor = dict_get(p_dict, "destination alpha blend factor");
                        p_alpha_blend_operation          = dict_get(p_dict, "alpha blend operation");

                        // Error check
                        if ( ! (
                            p_blend_enable                   &&
                            p_source_color_blend_factor      &&
                            p_destination_color_blend_factor &&
                            p_color_blend_operation          &&
                            p_source_alpha_blend_factor      &&
                            p_destination_alpha_blend_factor &&
                            p_alpha_blend_operation
                        ) )
                            goto missing_properties;
                    }

                    // Construct a color blend state
                    {

                        // Initialized data
                        VkBool32 blend_enable                        = 0;
                        VkBlendFactor source_color_blend_factor      = 0,
                                      destination_color_blend_factor = 0,
                                      source_alpha_blend_factor      = 0,
                                      destination_alpha_blend_factor = 0;
                        VkBlendOp color_blend_op                     = 0,
                                  alpha_blend_op                     = 0;
                        VkColorComponentFlags color_write_mask       = 0;

                        // Set the blend enable
                        if ( p_blend_enable->type == JSON_VALUE_BOOLEAN )
                            blend_enable = p_blend_enable->boolean;
                        else
                            goto wrong_blend_enable_type;

                        // Set the source color blend factor
                        if ( p_source_color_blend_factor->type == JSON_VALUE_STRING )
                            source_color_blend_factor = (VkBlendFactor) (size_t) dict_get(blend_factors, p_source_color_blend_factor->string);
                        else
                            goto wrong_source_color_blend_factor_type;

                        // Set the destination color blend factor
                        if ( p_destination_color_blend_factor->type == JSON_VALUE_STRING )
                            destination_color_blend_factor = (VkBlendFactor) (size_t) dict_get(blend_factors, p_destination_color_blend_factor->string);
                        else
                            goto wrong_destination_color_blend_factor;

                        // Set the color blend operation
                        if ( p_color_blend_operation->type == JSON_VALUE_STRING )
                            color_blend_op = (VkBlendOp) (size_t) dict_get(blend_operations, p_color_blend_operation->string);
                        else
                            goto wrong_color_blend_operation_type;

                        // Set the source alpha blend factor
                        if ( p_source_alpha_blend_factor->type == JSON_VALUE_STRING )
                            source_alpha_blend_factor = (VkBlendFactor) (size_t) dict_get(blend_factors, p_source_alpha_blend_factor->string);
                        else
                            goto wrong_source_alpha_blend_factor;

                        // Set the destination alpha blend factor
                        if ( p_destination_alpha_blend_factor->type == JSON_VALUE_STRING )
                            destination_alpha_blend_factor = (VkBlendFactor) (size_t) dict_get(blend_factors, p_destination_alpha_blend_factor->string);
                        else
                            goto wrong_destination_alpha_blend_factor;

                        // Set the alpha blend operation
                        if ( p_alpha_blend_operation->type == JSON_VALUE_STRING )
                            alpha_blend_op = (VkBlendOp) (size_t) dict_get(blend_operations, p_alpha_blend_operation->string);
                        else
                            goto wrong_alpha_blend_op;

                        // Populate the color blend attachment struct
                        color_blend_attachment_create_infos[i] =
                        (VkPipelineColorBlendAttachmentState)
                        {
                            .blendEnable         = blend_enable,
                            .srcColorBlendFactor = source_color_blend_factor,
                            .dstColorBlendFactor = destination_color_blend_factor,
                            .colorBlendOp        = color_blend_op,
                            .srcAlphaBlendFactor = source_alpha_blend_factor,
                            .dstAlphaBlendFactor = destination_alpha_blend_factor,
                            .alphaBlendOp        = alpha_blend_op,
                            .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
                        };
                    }
                }
            }

            //////////////////////////////////
            // Set up the color blend state //
            //////////////////////////////////
            color_blend_create_info =
            (VkPipelineColorBlendStateCreateInfo)
            {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .pNext = 0,
                .flags = 0,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = attachment_count,
                .pAttachments = color_blend_attachment_create_infos,
                .blendConstants = { 0.f, 0.f, 0.f, 0.f }
            };

            //////////////////////////////
            // Set up the dynamic state //
            //////////////////////////////
            {
                dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
                dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR;

                dynamic_state_create_info =
                (VkPipelineDynamicStateCreateInfo)
                {
                    .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                    .dynamicStateCount = 2,
                    .pDynamicStates    = (const VkDynamicState *) &dynamic_states
                };
            }

            ////////////////////////////////
            // Create the pipeline layout //
            ////////////////////////////////
            {

                // Initialized data
                VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
                    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                    .pNext                  = 0,
                    .flags                  = 0,
                    .setLayoutCount         = 0,
                    .pSetLayouts            = 0,
                    .pushConstantRangeCount = 0,
                    .pPushConstantRanges    = 0
                };

                // Create a pipeline layout
                if (
                    vkCreatePipelineLayout(
                        p_instance->vulkan.device,
                        &pipeline_layout_create_info,
                        0,
                        &p_shader->graphics.pipeline_layout
                    ) != VK_SUCCESS
                )
                    goto failed_to_create_pipeline_layout;
            }

            //////////////////////////////
            // Set up the push constant //
            //////////////////////////////
            if ( p_layout_push_constant )
            {

                /*
                // Initialized data
                dict    *push_constant_dict           = 0;
                char   **push_constant_struct         = 0;
                size_t   push_constant_size           = 0;
                size_t   push_constant_property_count = 0;

                // Parse the push constant JSON into a dictionary
                parse_json(push_constant_text, strlen(push_constant_text), &push_constant_dict);

                // Parse the push constant dictionary
                {

                    // Initialized data
                    JSONToken_t  *token                = 0;

                    // Get the push constant struct
                    token                = (JSONToken_t *) dict_get(push_constant_dict, "struct");
                    push_constant_struct = JSON_VALUE(token, JSON_VALUE_ARRAY);
                }

                while (push_constant_struct[++push_constant_property_count]);

                i_shader->graphics.push_constant_properties = calloc(push_constant_property_count + 1, sizeof(char*));

                for (size_t i = 0; i < push_constant_property_count; i++)
                {

                    // Initialized data
                    char   *push_constant_property     = push_constant_struct[i];
                    size_t  push_constant_property_len = strlen(push_constant_property);
                    dict   *property_dict              = 0;

                    char   *property_name              = 0;
                    size_t  property_size              = 0;

                    parse_json(push_constant_property, push_constant_property_len, &property_dict);

                    // Parse the property dictionary
                    {

                        // Initialized data
                        JSONToken_t *token = 0;

                        token = (JSONToken_t *) dict_get(property_dict, "name");
                        property_name = JSON_VALUE(token, JSON_VALUE_STRING);

                        token = (JSONToken_t *) dict_get(property_dict, "type");
                        char* property_type = JSON_VALUE(token, JSON_VALUE_STRING);

                        // TODO: Check property_type?
                        property_size = (size_t) dict_get(format_sizes, property_type);

                    }

                    // Copy the property name
                    {

                        // Initialized data
                        size_t property_name_len              = strlen(property_name);

                        // Allocate the property name
                        i_shader->graphics.push_constant_properties[i] = calloc(property_name_len + 1, sizeof(char));

                        // Copy the propery name
                        strncpy(i_shader->graphics.push_constant_properties[i], property_name, property_name_len);

                    }

                    // Copy the property size
                    push_constant_size += property_size;

                }

                if (push_constant_size > 128)
                    goto push_constant_is_too_large;

                i_shader->graphics.push_constant_size = push_constant_size;

                i_shader->graphics.push_constant_data = calloc(128, sizeof(u8));

                push_constant->offset     = 0;
                push_constant->size       = (u32)push_constant_size;
                push_constant->stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
                */
            }

            //////////////////////////////////
            // Set up the graphics pipeline //
            //////////////////////////////////
            graphics_pipeline_create_info =
            (VkGraphicsPipelineCreateInfo)
            {
                .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                .pNext               = 0,
                .flags               = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
                .stageCount          = (u32)shader_stage_iterator,      // Done
                .pStages             = shader_stages,                   // Done
                .pVertexInputState   = &vertex_input_info_create_info,  // Done
                .pInputAssemblyState = &input_assembly_create_info,     // Done

                // TODO:
                .pTessellationState  = &tessellation_state_create_info, // Done
                .pViewportState      = &viewport_state_create_info,     // Done
                .pRasterizationState = &rasterizer_create_info,         // Done
                .pMultisampleState   = &multisampling_create_info,      // Done

                // TODO:
                .pDepthStencilState = &depth_stencil_state_create_info, // Done
                .pColorBlendState   = &color_blend_create_info,         // Not done
                .pDynamicState      = &dynamic_state_create_info,       // Done
                .layout             = p_shader->layout->pipeline_layout,
                .renderPass         = p_instance->context.loading_renderer->current_render_pass->render_pass,
                .subpass            = 0,
                .basePipelineHandle = VK_NULL_HANDLE,
                .basePipelineIndex  = -1
            };

            /////////////////////////////////////
            // Construct the graphics pipeline //
            /////////////////////////////////////
            if (
                vkCreateGraphicsPipelines(
                    p_instance->vulkan.device,
                    VK_NULL_HANDLE,
                    1,
                    &graphics_pipeline_create_info,
                    0,
                    &p_shader->graphics.pipeline
                ) != VK_SUCCESS
            )
                goto failed_to_create_graphics_pipeline;
        }

        // Return the pointer to the caller
        *pp_shader = p_shader;
    }

    // Success
    return 1;

    // TODO:
    wrong_input_attribute_properties:
    failed_to_load_layout_as_json_value:
    wrong_blend_enable_type:
    wrong_source_color_blend_factor_type:
    wrong_destination_color_blend_factor:
    wrong_color_blend_operation_type:
    wrong_source_alpha_blend_factor:
    wrong_destination_alpha_blend_factor:
    wrong_alpha_blend_op:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_graphics_pipeline:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create graphics pipeline in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_pipeline_layout:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create pipeline layout in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // JSON errors
        {

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Not enough properties to construct graphics shader in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"name\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vertex_group_wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"vertex attributes\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_multisampler_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"multisampler\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_samples:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"samples\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_sample_shading_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"sample shading enable\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_alpha_to_coverage_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"alpha to coverage enable\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_alpha_to_one_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"alpha to one enable\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_minimum_sample_shading:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"minimum sample shading\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_rasterizer_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"rasterizer\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_line_width:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"line width\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_depth_bias_constant_factor:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias constant factor\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_depth_bias_clamp:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias clamp\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_depth_bias_slope_factor:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias slope factor\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_depth_clamp_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth clamp enable\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_rasterizer_discard_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"rasterizer discard enable\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_clockwise:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"clockwise\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_depth_bias_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias enable\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_vertex_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load vertex shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_vertex_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create vertex shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vertex_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse vertex shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_tessellation_control_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load tessellation control shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_tessellation_control_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create tessellation control shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_tessellation_control_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse tessellation control shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_tessellation_evaluation_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load tessellation evaluation shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_tessellation_evaluation_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create tessellation evaluation shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_tessellation_evaluation_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse tessellation evaluation shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_geometry_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load geometry shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_geometry_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create geometry shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_geometry_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse geometry shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_task_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load task shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_task_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create task shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_task_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse task shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_mesh_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load mesh shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_mesh_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create mesh shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_mesh_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse vertex shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_fragment_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load fragment shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_fragment_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create fragment shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_fragment_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse fragment shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate shader in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_compute_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value )
{

    // Argument notes
    // p_value will always be valid. p_value->type will always be JSON_VALUE_OBJECT

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // Initialized data
    GXInstance_t  *p_instance            = g_get_active_instance();
    GXShader_t    *p_shader              = 0,
                   *p_cache_shader        = 0;
    json_value   *p_name                = 0,
                  *p_compute_shader_path = 0,
                  *p_layout              = 0;

    // Parse the JSON value
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name                = dict_get(p_dict, "name");
        p_compute_shader_path = dict_get(p_dict, "compute shader path");
        p_layout              = dict_get(p_dict, "layout");

        // Check properties
        if ( ! (
            p_name                &&
            p_compute_shader_path &&
            p_layout
        ) )
            goto missing_properties;
    }

    // Lock the shader cache mutex
    SDL_LockMutex(p_instance->mutexes.shader_cache);

    // Search the cache for the shader
    p_cache_shader = g_find_shader(p_instance, p_name->string);

    // If the shader is in the cache ...
    if ( p_cache_shader )
    {

        // TODO

    }

    // ... the shader is not in the cache

    // Construct the shader
    {

        // Initialized data
        VkShaderModule compute_shader_module = 0;

        // Allocate memory for a shader
        if ( create_shader(&p_shader) == 0 ) goto failed_to_allocate_shader;

        // Set the shader type
        p_shader->type = g10_pipeline_compute;
        p_shader->compute.x_groups = 1;
        p_shader->compute.y_groups = 1;
        p_shader->compute.z_groups = 1;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_shader->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_shader->name, p_name->string, name_len);
        }
        else
            goto name_wrong_type;

        // Load the compute shader binary
        if ( p_compute_shader_path )
        {

            // Parse the compute shader path as a string
            if ( p_compute_shader_path->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t compute_shader_data_len = g_load_file(p_compute_shader_path->string, 0, true);
                char* compute_shader_data = calloc(compute_shader_data_len, sizeof(char));

                // Error check
                if ( compute_shader_data == (void *) 0 ) goto no_mem;

                // Load the compute shader binary file
                if ( g_load_file(p_compute_shader_path->string, compute_shader_data, true) == 0 ) goto failed_to_load_compute_shader_binary;

                // Create a shader module
                if ( create_shader_module(compute_shader_data, compute_shader_data_len, &compute_shader_module) == 0 ) goto failed_to_create_compute_shader_module;

                // Clean the scope
                free(compute_shader_data);
            }
            else
                goto wrong_compute_shader_path_type;
        }
        else
            goto no_compute_shader_path;

        // Load the layout
        if ( p_layout )
            if ( load_layout_as_json_value(&p_shader->layout, p_layout) == 0 ) goto failed_to_load_layout_as_json_value;

        // Construct the compute shader
        {

            // Initialized data
            VkPipelineShaderStageCreateInfo shader_stage                 = { 0 };
            VkPipelineLayoutCreateInfo      pipeline_layout_create_info  = { 0 };
            VkComputePipelineCreateInfo     compute_pipeline_create_info = { 0 };

            // Set up the compute shader stage
            if ( compute_shader_module )
            {

                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
                    .module = compute_shader_module,
                    .pName  = "main"
                };
            }
            else
                ;// TODO:

            // Set up the compute pipeline
            compute_pipeline_create_info = (VkComputePipelineCreateInfo)
            {
                .sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
                .pNext              = 0,
                .flags              = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
                .stage              = shader_stage,
                .layout             = p_shader->layout->pipeline_layout,
                .basePipelineHandle = 0,
                .basePipelineIndex  = 0,
            };

            // Construct the compute pipeline
            if (
                vkCreateComputePipelines(
                    p_instance->vulkan.device,
                    VK_NULL_HANDLE,
                    1,
                    &compute_pipeline_create_info,
                    0,
                    &p_shader->compute.pipeline
                ) != VK_SUCCESS
            ) goto failed_to_create_compute_pipeline;
        }

        // Cache the shader
        g_cache_shader(p_instance, p_shader);

        // Return the pointer to the caller
        *pp_shader = p_shader;
    }

    // Success
    return 1;

    // TODO:
    no_compute_shader_path:
    failed_to_load_layout_as_json_value:
    failed_to_create_descriptor_set_layout:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_pipeline_layout:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create pipeline layout in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_compute_pipeline:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create compute pipeline in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate shader in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Not enough properties to construct compute shader in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"name\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_compute_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"compute shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_compute_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load compute shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_compute_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create compute shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_ray_shader_as_json_value ( GXShader_t **pp_shader, json_value *p_value )
{

    // Argument notes
    // p_value will always be valid. p_value->type will always be JSON_VALUE_OBJECT

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
        if ( p_value   == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t  *p_instance                     = g_get_active_instance();
    GXShader_t    *p_shader                       = 0;
    json_value   *p_name                         = 0,
                  *p_ray_generation_shader_path   = 0,
                  *p_ray_any_hit_shader_path      = 0,
                  *p_ray_closest_hit_shader_path  = 0,
                  *p_ray_miss_shader_path         = 0,
                  *p_ray_intersection_shader_path = 0,
                  *p_ray_callable_shader_path     = 0;

    // Parse the JSON value
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name                         = dict_get(p_dict, "name");
        p_ray_generation_shader_path   = dict_get(p_dict, "generation shader path");
        p_ray_any_hit_shader_path      = dict_get(p_dict, "any hit shader path");
        p_ray_closest_hit_shader_path  = dict_get(p_dict, "closest hit shader path");
        p_ray_miss_shader_path         = dict_get(p_dict, "miss shader path");
        p_ray_intersection_shader_path = dict_get(p_dict, "intersection shader path");
        p_ray_callable_shader_path     = dict_get(p_dict, "callable shader path");

        // Check properties
        if ( ! (
            p_name                         &&
            p_ray_generation_shader_path   &&
            p_ray_any_hit_shader_path      &&
            p_ray_closest_hit_shader_path  &&
            p_ray_miss_shader_path         &&
            p_ray_intersection_shader_path &&
            p_ray_callable_shader_path
        ) )
            goto missing_properties;
    }

    // TODO: Cache check

    // Construct the shader
    {

        // Initialized data
        VkShaderModule ray_generation_shader_module   = 0,
                       ray_any_hit_shader_module      = 0,
                       ray_closest_hit_shader_module  = 0,
                       ray_miss_shader_module         = 0,
                       ray_intersection_shader_module = 0,
                       ray_callable_shader_module     = 0;

        // Allocate memory for a shader
        if ( create_shader(&p_shader) == 0 ) goto failed_to_allocate_shader;

        // Set the shader type
        p_shader->type = g10_pipeline_ray;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_shader->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_shader->name, p_name->string, name_len);
        }
        else
            goto name_wrong_type;

        // Load the ray generation shader binary
        if ( p_ray_generation_shader_path )
        {

            // Parse the ray generation shader path as a string
            if ( p_ray_generation_shader_path->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t ray_generation_shader_data_len = g_load_file(p_ray_generation_shader_path->string, 0, true);
                char* ray_generation_shader_data = calloc(ray_generation_shader_data_len, sizeof(char));

                // Error check
                if ( ray_generation_shader_data == (void *) 0 ) goto no_mem;

                // Load the ray generation shader binary file
                if ( g_load_file(p_ray_generation_shader_path->string, ray_generation_shader_data, true) == 0 ) goto failed_to_load_ray_generation_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_generation_shader_data, ray_generation_shader_data_len, &ray_generation_shader_module) == 0 ) goto failed_to_create_ray_generation_shader_module;

                // Clean the scope
                free(ray_generation_shader_data);
            }
            else
                goto wrong_ray_generation_shader_path_type;
        }
        else
            goto no_ray_generation_shader_path;

        // Load the ray any hit shader binary
        if ( p_ray_any_hit_shader_path )
        {

            // Parse the ray any hit shader path as a string
            if ( p_ray_any_hit_shader_path->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t ray_any_hit_shader_data_len = g_load_file(p_ray_any_hit_shader_path->string, 0, true);
                char* ray_any_hit_shader_data = calloc(ray_any_hit_shader_data_len, sizeof(char));

                // Error check
                if ( ray_any_hit_shader_data == (void *) 0 ) goto no_mem;

                // Load the ray any hit shader binary file
                if ( g_load_file(p_ray_any_hit_shader_path->string, ray_any_hit_shader_data, true) == 0 ) goto failed_to_load_ray_any_hit_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_any_hit_shader_data, ray_any_hit_shader_data_len, &ray_any_hit_shader_module) == 0 ) goto failed_to_create_ray_any_hit_shader_module;

                // Clean the scope
                free(ray_any_hit_shader_data);
            }
            else
                goto wrong_ray_any_hit_shader_path_type;
        }
        else
            goto no_ray_any_hit_shader_path;

        // Load the ray closest hit shader binary
        if ( p_ray_closest_hit_shader_path )
        {

            // Parse the ray closest hit shader path as a string
            if ( p_ray_closest_hit_shader_path->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t ray_closest_hit_shader_data_len = g_load_file(p_ray_closest_hit_shader_path->string, 0, true);
                char* ray_closest_hit_shader_data = calloc(ray_closest_hit_shader_data_len, sizeof(char));

                // Error check
                if ( ray_closest_hit_shader_data == (void *) 0 ) goto no_mem;

                // Load the ray closest hit shader binary file
                if ( g_load_file(p_ray_closest_hit_shader_path->string, ray_closest_hit_shader_data, true) == 0 ) goto failed_to_load_ray_closest_hit_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_closest_hit_shader_data, ray_closest_hit_shader_data_len, &ray_closest_hit_shader_module) == 0 ) goto failed_to_create_ray_closest_hit_shader_module;

                // Clean the scope
                free(ray_closest_hit_shader_data);
            }
            else
                goto wrong_ray_closest_hit_shader_path_type;
        }
        else
            goto no_ray_closest_hit_shader_path;

        // Load the ray miss shader binary
        if ( p_ray_miss_shader_path )
        {

            // Parse the ray miss shader path as a string
            if ( p_ray_miss_shader_path->type == JSON_VALUE_STRING )
            {
                // Initialized data
                size_t ray_miss_shader_data_len = g_load_file(p_ray_miss_shader_path->string, 0, true);
                char* ray_miss_shader_data = calloc(ray_miss_shader_data_len, sizeof(char));

                // Error check
                if ( ray_miss_shader_data == (void *) 0 ) goto no_mem;

                // Load the ray miss shader binary file
                if ( g_load_file(p_ray_miss_shader_path->string, ray_miss_shader_data, true) == 0 ) goto failed_to_load_ray_miss_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_miss_shader_data, ray_miss_shader_data_len, &ray_miss_shader_module) == 0 ) goto failed_to_create_ray_miss_shader_module;

                // Clean the scope
                free(ray_miss_shader_data);
            }
            else
                goto wrong_ray_miss_shader_path_type;
        }
        else
            goto no_ray_miss_shader_path;

        // Load the ray intersection shader binary
        if ( p_ray_intersection_shader_path )
        {

            // Parse the ray intersection shader path as a string
            if ( p_ray_intersection_shader_path->type == JSON_VALUE_STRING )
            {
                // Initialized data
                size_t ray_intersection_shader_data_len = g_load_file(p_ray_intersection_shader_path->string, 0, true);
                char* ray_intersection_shader_data = calloc(ray_intersection_shader_data_len, sizeof(char));

                // Error check
                if ( ray_intersection_shader_data == (void *) 0 ) goto no_mem;

                // Load the ray intersection shader binary file
                if ( g_load_file(p_ray_intersection_shader_path->string, ray_intersection_shader_data, true) == 0 ) goto failed_to_load_ray_intersection_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_intersection_shader_data, ray_intersection_shader_data_len, &ray_intersection_shader_module) == 0 ) goto failed_to_create_ray_intersection_shader_module;

                // Clean the scope
                free(ray_intersection_shader_data);
            }
            else
                goto wrong_ray_intersection_shader_path_type;
        }
        else
            goto no_ray_intersection_shader_path;

        // Load the ray callable shader binary
        if ( p_ray_callable_shader_path )
        {

            // Parse the ray callable shader path as a string
            if ( p_ray_callable_shader_path->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t ray_callable_shader_data_len = g_load_file(p_ray_callable_shader_path->string, 0, true);
                char* ray_callable_shader_data = calloc(ray_callable_shader_data_len, sizeof(char));

                // Error check
                if ( ray_callable_shader_data == (void *) 0 ) goto no_mem;

                // Load the ray callable shader binary file
                if ( g_load_file(p_ray_callable_shader_path->string, ray_callable_shader_data, true) == 0 ) goto failed_to_load_ray_callable_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_callable_shader_data, ray_callable_shader_data_len, &ray_callable_shader_module) == 0 ) goto failed_to_create_ray_callable_shader_module;

                // Clean the scope
                free(ray_callable_shader_data);
            }
            else
                goto wrong_ray_callable_shader_path_type;
        }
        else
            goto no_ray_callable_shader_path;

        // // TODO: Construct the ray shader
        {

            // Initialized data
            VkPipelineShaderStageCreateInfo   shader_stage                 = { 0 };
            VkPipelineLayoutCreateInfo        pipeline_layout_create_info  = { 0 };
            VkRayTracingPipelineCreateInfoKHR ray_pipeline_create_info     = { 0 };

            // Set up the ray generation shader stage
            if ( ray_generation_shader_module )
            {

                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray any hit shader stage
            if ( ray_any_hit_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray closest hit shader stage
            if ( ray_closest_hit_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray miss shader stage
            if ( ray_miss_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_MISS_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray intersection shader stage
            if ( ray_intersection_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray callable shader stage
            if ( ray_callable_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_CALLABLE_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the compute pipeline
            ray_pipeline_create_info = (VkRayTracingPipelineCreateInfoKHR)
            {
                .sType                        = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
                .pNext                        = 0,
                .flags                        = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT,
                .stageCount                   = 0,
                .pStages                      = 0,
                .groupCount                   = 0,
                .pGroups                      = 0,
                .maxPipelineRayRecursionDepth = 0,
                .pLibraryInfo                 = 0,
                .pLibraryInterface            = 0,
                .pDynamicState                = 0,
                .layout                       = 0,
                .basePipelineHandle           = 0,
                .basePipelineIndex            = 0
            };

            // TODO: Construct the compute pipeline
            //if ( vkCreateRayTracingPipelinesKHR(p_instance->vulkan.device, VK_NULL_HANDLE, 1, &ray_pipeline_create_info, 0, &p_shader->ray.pipeline) != VK_SUCCESS )
            //    g_print_error("failed to create ray pipeline!\n");

        }

        // Return the pointer to the caller
        *pp_shader = p_shader;
    }

    // Success
    return 1;

    // TODO:
    missing_properties:
        #ifndef NDEBUG
            g_print_error("[G10] [Shader] Not enough properties to construct ray shader in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json", __FUNCTION__);
        #endif

        // Error
        return 0;

    failed_to_allocate_shader:

    name_wrong_type:

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // JSON errors
        {
            wrong_ray_generation_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse generation ray shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_any_hit_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse any hit ray shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_closest_hit_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse closest hit ray shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_miss_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse miss ray shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_intersection_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse intersection ray shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_callable_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse callable ray shader path in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_generation_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"generation shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_any_hit_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"any hit shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_closest_hit_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"closest_hit shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_miss_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"miss shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_intersection_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"intersection shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_callable_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"callable shader path\" property in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_generation_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load generation ray shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_any_hit_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load any hit ray shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_closest_hit_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load closest hit ray shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_miss_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load miss ray shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_intersection_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load intersection ray shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_callable_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load callable ray shader binary in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_generation_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create generation ray shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_any_hit_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create any hit ray shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_closest_hit_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create closest hit ray shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_miss_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create miss ray shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_intersection_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create intersection ray shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_callable_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create callable ray shader module in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_layout_as_json_value ( GXLayout_t **pp_layout, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_layout == (void *) 0 ) goto no_layout;
        if ( p_value   == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance      = g_get_active_instance();
    GXLayout_t   *p_layout        = 0;
    json_value  *p_sets          = 0,
                 *p_push_constant = 0;

    // Parse the JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_sets          = dict_get(p_dict, "sets");

        // Optional properties
        p_push_constant = dict_get(p_dict, "push constant");

        // Check properties
        if ( ! ( p_sets ) ) goto missing_properties;
    }
    else
        goto wrong_type;

    // Construct the pipeline layout
    {

        // Initialized data
        json_value           **pp_sets       = 0;
        size_t                  set_count     = 0;
        VkDescriptorSetLayout  *p_set_layouts = 0;

        // Allocate memory for a shader layout
        if ( create_layout(&p_layout) == 0 ) goto failed_to_allocate_layout;

        // Construct sets
        if ( p_sets->type == JSON_VALUE_ARRAY )
        {

            // Get the contents of the array
            {

                // Get the size of the array
                array_get(p_sets->list, 0, &set_count);

                // Allocate memory for the set JSON values
                pp_sets = calloc(set_count, sizeof(json_value *));

                // Error check
                if ( pp_sets == (void *) 0 ) goto no_mem;

                // Get the array contents
                array_get(p_sets->list, (void **)pp_sets, 0);

            }

            // Allocate memory for the sets
            p_layout->sets = calloc(set_count, sizeof(GXSet_t *));
            p_set_layouts  = calloc(set_count, sizeof(VkDescriptorSetLayout));

            // Error check
            if ( p_layout->sets  == (void *) 0 ) goto no_mem;
            if ( p_set_layouts   == (void *) 0 ) goto no_mem;

            // Iterate over each JSONValue
            for (size_t i = 0; i < set_count; i++)
            {

                // Initialized data
                json_value                     *i_set = pp_sets[i];
                GXSet_t                         *p_set = 0;

                // Construct a set from a JSON value
                if ( load_set_as_json_value(&p_set, i_set) == 0 ) goto failed_to_load_set_as_json_value;

                // I don't like this specific line. -Jake +
                                                       // |
                p_set->index = i; // <--------------------+

                p_layout->sets[i] = p_set;

                p_set_layouts[i] = p_set->set_layout;
            }
        }


        // TODO: Construct a push constant

        // Construct the layout
        VkPipelineLayoutCreateInfo pipeline_layout_create_info =
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = 0,
            .flags = 0,
            .setLayoutCount = set_count,
            .pSetLayouts = p_set_layouts,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = 0
        };

        // Create a pipeline layout
        if (
            vkCreatePipelineLayout(
                p_instance->vulkan.device,
                &pipeline_layout_create_info,
                0,
                &p_layout->pipeline_layout
            ) != VK_SUCCESS
        )
            goto vulkan_failed_to_create_pipeline_layout;

        *pp_layout = p_layout;
    }

    // Success
    return 1;

    missing_properties:
        #ifndef NDEBUG
            g_print_error("[G10] [Shader] Not enough properties to construct layout in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json", __FUNCTION__);
        #endif

        // Error
        return 0;
    wrong_type:
    failed_to_allocate_layout:
    failed_to_load_set_as_json_value:
    vulkan_failed_to_create_pipeline_layout:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_layout:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_layout\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_set_as_json_value ( GXSet_t **pp_set, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_set == (void *) 0 ) goto no_set;
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t                     *p_instance                          = g_get_active_instance();
    GXSet_t                          *p_set                               = 0;
    json_value                      *p_name                              = 0,
                                     *p_descriptors                       = 0;
    VkDescriptorSetLayoutCreateInfo   i_descriptor_set_layout_create_info = { 0 };
    size_t                            descriptor_count                    = 0;
    json_value                     **pp_descriptors                      = 0;

    // Parse the JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name        = dict_get(p_dict, "name");
        p_descriptors = dict_get(p_dict, "descriptors");

        // Error check
        if ( ! (
            p_name        &&
            p_descriptors
        ) )
            goto missing_properties;
    }
    // Default
    else
        goto wrong_value_type;

    // Construct the set
    {

        // Initialized data
        VkDescriptorSetLayoutCreateInfo set_layout_create_info = { 0 };

        // Allocate memory for the set
        if ( create_set(&p_set) == 0 ) goto failed_to_create_set;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for the name
            p_set->name = calloc(len+1, sizeof(char));

            // Error handling
            if ( p_set->name == 0 ) goto no_mem;

            // Copy the name
            strncpy(p_set->name, p_name->string, len);
        }
        // Default
        else
            goto wrong_name_type;

        // Parse each descriptor
        if ( p_descriptors->type == JSON_VALUE_ARRAY )
        {

            // Get the contents of the array
            {

                // Get the size of the array
                array_get(p_descriptors->list, 0, &descriptor_count);

                // Allocate memory for the set JSON values
                pp_descriptors = calloc(descriptor_count, sizeof(json_value *));

                // Error check
                if ( pp_descriptors == (void *) 0 ) goto no_mem;

                // Get the array contents
                array_get(p_descriptors->list, (void **)pp_descriptors, 0);

            }

            // Set the descriptor count
            p_set->descriptor_count = descriptor_count;

            // Allocate memory
            p_set->descriptors_data = calloc(descriptor_count+1, sizeof(GXDescriptor_t *));
            p_set->p_bindings       = calloc(descriptor_count, sizeof(VkDescriptorSetLayoutBinding));

            // Error check
            if ( p_set->descriptors_data == (void *) 0 ) goto no_mem;

            // Iterate over each descriptor
            for (size_t i = 0; i < descriptor_count; i++)
            {

                // Initialized data
                json_value    *p_descriptor_value = pp_descriptors[i],
                               *p_descriptor_name  = 0,
                               *p_descriptor_type  = 0;
                GXDescriptor_t *p_descriptor       = 0;

                // Parse the JSON value
                if ( p_descriptor_value->type == JSON_VALUE_OBJECT )
                {

                    // Initialized data
                    dict *p_dict = p_descriptor_value->object;

                    // Required properties
                    p_descriptor_name = dict_get(p_dict, "name");
                    p_descriptor_type = dict_get(p_dict, "type");

                    // Check for missing properties
                    if ( ! (
                        p_descriptor_name &&
                        p_descriptor_type
                    ) )
                        goto missing_descriptor_properties;
                }

                // Construct the descriptor
                {

                    // Allocate memory for a descriptor
                    if ( create_descriptor(&p_descriptor) == 0 ) goto failed_to_allocate_descriptor;

                    // Copy the name
                    if ( p_descriptor_name->type == JSON_VALUE_STRING )
                    {

                        // Initialized data
                        size_t descriptor_name_len = strlen(p_descriptor_name->string);

                        // Allocate memory for the descriptor's name
                        p_descriptor->name = calloc(descriptor_name_len+1, sizeof(char));

                        // Error check
                        if ( p_descriptor->name == (void *) 0 ) goto failed_to_allocate_descriptor_name;

                        // Copy the string
                        strncpy(p_descriptor->name, p_descriptor_name->string, descriptor_name_len);
                    }

                    // Set the type
                    if ( p_descriptor_type->type == JSON_VALUE_STRING )
                        p_descriptor->type = (VkDescriptorType) (size_t) dict_get(descriptor_types, p_descriptor_type->string);

                    // Set the index
                    p_descriptor->index = i;
                }

                // Store the descriptor
                p_set->descriptors_data[i] = p_descriptor;

                p_set->p_bindings[i] = (VkDescriptorSetLayoutBinding)
                {
                    .binding            = p_descriptor->index,
                    .descriptorType     = p_descriptor->type,
                    .descriptorCount    = 1,
                    .stageFlags         = VK_SHADER_STAGE_ALL,
                    .pImmutableSamplers = 0
                };
            }
        }
        // Default
        else
            goto wrong_descriptor_type;

        // Populate the descriptor set layout create info struct
        set_layout_create_info = (VkDescriptorSetLayoutCreateInfo)
        {
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext        = 0,
            .flags        = 0,
            .bindingCount = p_set->descriptor_count,
            .pBindings    = p_set->p_bindings
        };

        // Construct a set layout
        if ( 
            vkCreateDescriptorSetLayout(
                p_instance->vulkan.device,
                &set_layout_create_info,
                0,
                &p_set->set_layout
            ) != VK_SUCCESS
        )
            goto failed_to_create_set_layout;

        // Return a pointer to the caller
        *pp_set = p_set;
    }

    // Success
    return 1;

    failed_to_load_descriptor_as_json_value:
    wrong_descriptors_type:
    missing_descriptor_properties:
    failed_to_allocate_descriptor:
    failed_to_allocate_descriptor_name:
    failed_to_create_set_layout:
    wrong_descriptor_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_set:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_set\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_create_set:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to allocate memory for set in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Value was of wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Not enough properties to construct set in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Not enough properties to construct set in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/shader.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

/*
int load_descriptor_as_json_value ( GXDescriptor_t **pp_descriptor, json_value *p_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_descriptor == (void *) 0 ) goto no_descriptor;
            if ( p_value       == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXDescriptor_t *p_descriptor = 0;
    json_value    *p_name       = 0,
                   *p_type       = 0;

    // Parse the JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        p_name = dict_get(p_value->object, "name");
        p_type = dict_get(p_value->object, "type");

        if ( ! ( p_name && p_type ) )
            goto missing_properties;
    }
    // Default
    else
        goto wrong_value_type;

    printf("\n\n\n\n\n\n");
    print_json_value(p_value, stdout);
    printf("\n\n\n\n\n\n");

    // Construct the descriptor
    {

        // Allocate memory for the descriptor
        if ( create_descriptor(&p_descriptor) == 0 )
            goto failed_to_create_descriptor;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for the name
            p_descriptor->name = calloc(len+1, sizeof(char));

            // Error handling
            if ( p_descriptor->name == 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_descriptor->name, p_name->string, len);
        }
        else
            goto wrong_name_type;

        // Set the type
        if ( p_type->type == JSON_VALUE_STRING )
            p_descriptor->type = (VkDescriptorType) dict_get(descriptor_types, p_type->string);

        p_descriptor->

        // Return a pointer to the caller
        *pp_descriptor = p_descriptor;
    }

    // Success
    return 1;

    missing_properties:
    wrong_value_type:
    wrong_name_type:
    failed_to_create_descriptor:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_descriptor:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_descriptor\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
*/

int destroy_graphics_shader ( GXShader_t **pp_shader )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXShader_t   *p_shader   = *pp_shader;

    // Error check
    if ( p_shader == (void *) 0 ) goto pointer_to_null_pointer;

    // Destroy the pipeline
    vkDestroyPipeline(p_instance->vulkan.device, p_shader->graphics.pipeline, 0);

    // Destroy the pipeline layout
    vkDestroyPipelineLayout(p_instance->vulkan.device, p_shader->graphics.pipeline_layout, 0);

    // Destroy shader modules
    {

        // Destroy the vertex module
        if ( p_shader->graphics.vertex_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.vertex_shader_module, 0);

        // Destroy the tessellation control module
        if ( p_shader->graphics.tessellation_control_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.tessellation_control_shader_module, 0);

        // Destroy the tessellation evaluation module
        if ( p_shader->graphics.tessellation_evaluation_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.tessellation_evaluation_shader_module, 0);

        // Destroy the geometry module
        if ( p_shader->graphics.geometry_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.geometry_shader_module, 0);

        // Destroy the task module
        if ( p_shader->graphics.task_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.task_shader_module, 0);

        // Destroy the mesh module
        if ( p_shader->graphics.mesh_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.mesh_shader_module, 0);

        // Destroy the fragment module
        if ( p_shader->graphics.fragment_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->graphics.fragment_shader_module, 0);

    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Parameter \"pp_shader\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_compute_shader ( GXShader_t **pp_shader )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXShader_t   *p_shader   = *pp_shader;

    // Error check
    if ( p_shader == (void *) 0 ) goto pointer_to_null_pointer;

    // Destroy the pipeline
    vkDestroyPipeline(p_instance->vulkan.device, p_shader->compute.pipeline, 0);

    // Destroy the pipeline layout
    //vkDestroyPipelineLayout(p_instance->vulkan.device, p_shader->layout->pipeline_layout, 0);

    // Destroy the compute shader module
    vkDestroyShaderModule(p_instance->vulkan.device, p_shader->compute.compute_shader_module, 0);

    // Destroy the descriptor set layout
    //vkDestroyDescriptorSetLayout(p_instance->vulkan.device, p_shader->compute.set_layout, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Parameter \"pp_shader\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_ray_shader ( GXShader_t **pp_shader )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_shader == (void *) 0 ) goto no_shader;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXShader_t   *p_shader   = *pp_shader;

    // Error check
    if ( p_shader == (void *) 0 ) goto pointer_to_null_pointer;

    // Destroy the pipeline
    vkDestroyPipeline(p_instance->vulkan.device, p_shader->ray.pipeline, 0);

    // Destroy the pipeline layout
    vkDestroyPipelineLayout(p_instance->vulkan.device, p_shader->ray.pipeline_layout, 0);

    // Destroy shader modules
    {

        // Destroy the ray generation shader module
        if ( p_shader->ray.ray_generation_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->ray.ray_generation_shader_module, 0);

        // Destroy the any hit shader module
        if ( p_shader->ray.ray_any_hit_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->ray.ray_any_hit_shader_module, 0);

        // Destroy the closest hit shader module
        if ( p_shader->ray.ray_closest_hit_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->ray.ray_closest_hit_shader_module, 0);

        // Destroy the miss shader module
        if ( p_shader->ray.ray_miss_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->ray.ray_miss_shader_module, 0);

        // Destroy the intersection shader module
        if ( p_shader->ray.ray_intersection_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->ray.ray_intersection_shader_module, 0);

        // Destroy the callable shader module
        if ( p_shader->ray.ray_callable_shader_module )
            vkDestroyShaderModule(p_instance->vulkan.device, p_shader->ray.ray_callable_shader_module, 0);

    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Parameter \"pp_shader\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}