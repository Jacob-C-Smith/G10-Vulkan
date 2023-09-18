#include <G10/GXRenderer.h>

// Constants
#define PRESENTATION_MODES_COUNT 4
#define ATTACHMENT_LOAD_OPERATION_COUNT 3
#define ATTACHMENT_STORE_OPERATION_COUNT 3
#define IMAGE_LAYOUTS_COUNT 18
#define SUBPASS_FUNCTION_COUNT 11
#define FORMAT_ENUM_COUNT 130
#define PIPELINE_STAGE_FLAG_BITS_COUNT 25
#define ACCESS_FLAG_BITS_COUNT 33
#define DEPENDENCY_FLAG_BITS_COUNT 3
#define TEXTURING_MODE_COUNT 5
#define FILTERING_MODE_COUNT 2
#define TILING_COUNT 2
#define USAGE_COUNT 8
#define VIEW_TYPE_COUNT 7
#define SWIZZLE_COUNT 7
#define ASPECT_COUNT 4

VkPresentModeKHR presentation_mode_enums[PRESENTATION_MODES_COUNT] =
{
    VK_PRESENT_MODE_IMMEDIATE_KHR,
    VK_PRESENT_MODE_MAILBOX_KHR,
    VK_PRESENT_MODE_FIFO_KHR,
    VK_PRESENT_MODE_FIFO_RELAXED_KHR
};
char *presentation_mode_names[PRESENTATION_MODES_COUNT] =
{
    "immediate",
    "mailbox",
    "FIFO",
    "FIFO relaxed"
};

VkAttachmentLoadOp attachment_load_operation_enums[ATTACHMENT_LOAD_OPERATION_COUNT] =
{
    VK_ATTACHMENT_LOAD_OP_LOAD,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
    VK_ATTACHMENT_LOAD_OP_DONT_CARE
};
char *attachment_load_operation_names[ATTACHMENT_LOAD_OPERATION_COUNT] =
{
    "load",
    "clear",
    "don't care"
};

VkAttachmentStoreOp attachment_store_operation_enums[ATTACHMENT_STORE_OPERATION_COUNT] =
{
    VK_ATTACHMENT_STORE_OP_STORE,
    VK_ATTACHMENT_STORE_OP_DONT_CARE,
    VK_ATTACHMENT_STORE_OP_NONE,
};
char *attachment_store_operation_names[ATTACHMENT_STORE_OPERATION_COUNT] =
{
    "store",
    "don't care",
    "none"
};

VkImageLayout image_layout_enums[IMAGE_LAYOUTS_COUNT] =
{
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    VK_IMAGE_LAYOUT_PREINITIALIZED,
    VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
};
char *image_layout_names[IMAGE_LAYOUTS_COUNT] =
{
    "undefined",
    "general",
    "color attachment",
    "depth stencil attachment",
    "depth stencil read only",
    "shader read only",
    "transfer source",
    "transfer destination",
    "pre initialized",
    "depth read only stencil attachment",
    "depth attachment stencil read only",
    "depth attachment",
    "depth read only",
    "stencil attachment",
    "stencil read only",
    "read only",
    "attachment",
    "present source"
};

void *subpass_function_callbacks[SUBPASS_FUNCTION_COUNT] =
{
    0,
    0, //&draw_material,
    0, //&draw_depth_pass,
    0, //&draw_direct_lighting,
    0, //&draw_shadows,
    0, //&draw_translucency,
    0, //&draw_indirect_lighting,
    0, //&draw_depth_of_field,
    0, //&draw_motion_blur,
    0, //&draw_bloom_blur,
    0, //&draw_tone_map_gamma_correct
};
char *subpass_function_names[SUBPASS_FUNCTION_COUNT] =
{
    "draw scene",
    "material output",
    "depth pass",
    "direct lighting",
    "shadows",
    "translucency",
    "indirect lighting",
    "depth of field",
    "motion blur",
    "bloom blur",
    "tone map gamma correct"
};

VkFormat format_enums[FORMAT_ENUM_COUNT] =
{
    VK_FORMAT_UNDEFINED,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    VK_FORMAT_R8_UNORM,
    VK_FORMAT_R8_SNORM,
    VK_FORMAT_R8_USCALED,
    VK_FORMAT_R8_SSCALED,
    VK_FORMAT_R8_UINT,
    VK_FORMAT_R8_SINT,
    VK_FORMAT_R8_SRGB,
    VK_FORMAT_R8G8_UNORM,
    VK_FORMAT_R8G8_SNORM,
    VK_FORMAT_R8G8_USCALED,
    VK_FORMAT_R8G8_SSCALED,
    VK_FORMAT_R8G8_UINT,
    VK_FORMAT_R8G8_SINT,
    VK_FORMAT_R8G8_SRGB,
    VK_FORMAT_R8G8B8_UNORM,
    VK_FORMAT_R8G8B8_SNORM,
    VK_FORMAT_R8G8B8_USCALED,
    VK_FORMAT_R8G8B8_SSCALED,
    VK_FORMAT_R8G8B8_UINT,
    VK_FORMAT_R8G8B8_SINT,
    VK_FORMAT_R8G8B8_SRGB,
    VK_FORMAT_B8G8R8_UNORM,
    VK_FORMAT_B8G8R8_SNORM,
    VK_FORMAT_B8G8R8_USCALED,
    VK_FORMAT_B8G8R8_SSCALED,
    VK_FORMAT_B8G8R8_UINT,
    VK_FORMAT_B8G8R8_SINT,
    VK_FORMAT_B8G8R8_SRGB,
    VK_FORMAT_R8G8B8A8_UNORM,
    VK_FORMAT_R8G8B8A8_SNORM,
    VK_FORMAT_R8G8B8A8_USCALED,
    VK_FORMAT_R8G8B8A8_SSCALED,
    VK_FORMAT_R8G8B8A8_UINT,
    VK_FORMAT_R8G8B8A8_SINT,
    VK_FORMAT_R8G8B8A8_SRGB,
    VK_FORMAT_B8G8R8A8_UNORM,
    VK_FORMAT_B8G8R8A8_SNORM,
    VK_FORMAT_B8G8R8A8_USCALED,
    VK_FORMAT_B8G8R8A8_SSCALED,
    VK_FORMAT_B8G8R8A8_UINT,
    VK_FORMAT_B8G8R8A8_SINT,
    VK_FORMAT_B8G8R8A8_SRGB,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    VK_FORMAT_R16_UNORM,
    VK_FORMAT_R16_SNORM,
    VK_FORMAT_R16_USCALED,
    VK_FORMAT_R16_SSCALED,
    VK_FORMAT_R16_UINT,
    VK_FORMAT_R16_SINT,
    VK_FORMAT_R16_SFLOAT,
    VK_FORMAT_R16G16_UNORM,
    VK_FORMAT_R16G16_SNORM,
    VK_FORMAT_R16G16_USCALED,
    VK_FORMAT_R16G16_SSCALED,
    VK_FORMAT_R16G16_UINT,
    VK_FORMAT_R16G16_SINT,
    VK_FORMAT_R16G16_SFLOAT,
    VK_FORMAT_R16G16B16_UNORM,
    VK_FORMAT_R16G16B16_SNORM,
    VK_FORMAT_R16G16B16_USCALED,
    VK_FORMAT_R16G16B16_SSCALED,
    VK_FORMAT_R16G16B16_UINT,
    VK_FORMAT_R16G16B16_SINT,
    VK_FORMAT_R16G16B16_SFLOAT,
    VK_FORMAT_R16G16B16A16_UNORM,
    VK_FORMAT_R16G16B16A16_SNORM,
    VK_FORMAT_R16G16B16A16_USCALED,
    VK_FORMAT_R16G16B16A16_SSCALED,
    VK_FORMAT_R16G16B16A16_UINT,
    VK_FORMAT_R16G16B16A16_SINT,
    VK_FORMAT_R16G16B16A16_SFLOAT,
    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_UINT,
    VK_FORMAT_R32G32B32A32_SINT,
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R64_UINT,
    VK_FORMAT_R64_SINT,
    VK_FORMAT_R64_SFLOAT,
    VK_FORMAT_R64G64_UINT,
    VK_FORMAT_R64G64_SINT,
    VK_FORMAT_R64G64_SFLOAT,
    VK_FORMAT_R64G64B64_UINT,
    VK_FORMAT_R64G64B64_SINT,
    VK_FORMAT_R64G64B64_SFLOAT,
    VK_FORMAT_R64G64B64A64_UINT,
    VK_FORMAT_R64G64B64A64_SINT,
    VK_FORMAT_R64G64B64A64_SFLOAT,
    0,
    0,
    VK_FORMAT_D16_UNORM,
    0,
    VK_FORMAT_D32_SFLOAT,
    VK_FORMAT_S8_UINT,
    VK_FORMAT_D16_UNORM_S8_UINT,
    VK_FORMAT_D24_UNORM_S8_UINT
};
char *format_names[FORMAT_ENUM_COUNT] =
{
    "undefined",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "unorm r8",
    "snorm r8",
    "uscaled r8",
    "sscaled r8",
    "uint r8",
    "sint r8",
    "sRGB r8",
    "unorm r8g8",
    "snorm r8g8",
    "uscaled r8g8",
    "sscaled r8g8",
    "uint r8g8",
    "sint r8g8",
    "sRGB r8g8",
    "unorm r8g8b8",
    "snorm r8g8b8",
    "uscaled r8g8b8",
    "sscaled r8g8b8",
    "uint r8g8b8",
    "sint r8g8b8",
    "sRGB r8g8b8",
    "unorm b8g8r8",
    "snorm b8g8r8",
    "uscaled b8g8r8",
    "sscaled b8g8r8",
    "uint b8g8r8",
    "sint b8g8r8",
    "sRGB b8g8r8",
    "unorm r8g8b8a8",
    "snorm r8g8b8a8",
    "uscaled r8g8b8a8",
    "sscaled r8g8b8a8",
    "uint r8g8b8a8",
    "sint r8g8b8a8",
    "sRGB r8g8b8a8",
    "unorm b8g8r8a8",
    "snorm b8g8r8a8",
    "uscaled b8g8r8a8",
    "sscaled b8g8r8a8",
    "uint b8g8r8a8",
    "sint b8g8r8a8",
    "sRGB b8g8r8a8",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "unorm r16",
    "snorm r16",
    "uscaled r16",
    "sscaled r16",
    "uint r16",
    "sint r16",
    "sfloat r16",
    "unorm r16g16",
    "snorm r16g16",
    "uscaled r16g16",
    "sscaled r16g16",
    "uint r16g16",
    "sint r16g16",
    "sfloat r16g16",
    "unorm r16g16b16",
    "snorm r16g16b16",
    "uscaled r16g16b16",
    "sscaled r16g16b16",
    "uint r16g16b16",
    "sint r16g16b16",
    "sfloat r16g16b16",
    "unorm r16g16b16a16",
    "snorm r16g16b16a16",
    "uscaled r16g16b16a16",
    "sscaled r16g16b16a16",
    "uint r16g16b16a16",
    "sint r16g16b16a16",
    "sfloat r16g16b16a16",
    "uint r32",
    "sint r32",
    "sfloat r32",
    "uint r32g32",
    "sint r32g32",
    "sfloat r32g32",
    "uint r32g32b32",
    "sint r32g32b32",
    "sfloat r32g32b32",
    "uint r32g32b32a32",
    "sint r32g32b32a32",
    "sfloat r32g32b32a32",
    "uint r64",
    "sint r64",
    "sfloat r64",
    "uint r64g64",
    "sint r64g64",
    "sfloat r64g64",
    "uint r64g64b64",
    "sint r64g64b64",
    "sfloat r64g64b64",
    "uint r64g64b64a64",
    "sint r64g64b64a64",
    "sfloat r64g64b64a64",
    "",
    "",
    "unorm d16",
    "",
    "sfloat d32",
    "uint s8",
    "unorm d16 uint s8",
    "unorm d24 uint s8"
};

VkPipelineStageFlagBits2 pipeline_stage_flag_bits_enum[PIPELINE_STAGE_FLAG_BITS_COUNT] =
{
    VK_PIPELINE_STAGE_2_NONE,
    VK_PIPELINE_STAGE_2_TOP_OF_PIPE_BIT,
    VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT,
    VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT,
    VK_PIPELINE_STAGE_2_VERTEX_SHADER_BIT,
    VK_PIPELINE_STAGE_2_TESSELLATION_CONTROL_SHADER_BIT,
    VK_PIPELINE_STAGE_2_TESSELLATION_EVALUATION_SHADER_BIT,
    VK_PIPELINE_STAGE_2_GEOMETRY_SHADER_BIT,
    VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
    VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT,
    VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
    VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
    VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
    VK_PIPELINE_STAGE_2_TRANSFER_BIT,
    VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
    VK_PIPELINE_STAGE_2_HOST_BIT,
    VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
    VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
    VK_PIPELINE_STAGE_2_TRANSFORM_FEEDBACK_BIT_EXT,
    VK_PIPELINE_STAGE_2_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
    VK_PIPELINE_STAGE_2_RAY_TRACING_SHADER_BIT_KHR,
    VK_PIPELINE_STAGE_2_FRAGMENT_DENSITY_PROCESS_BIT_EXT,
    VK_PIPELINE_STAGE_2_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR,
    VK_PIPELINE_STAGE_2_TASK_SHADER_BIT_EXT,
    VK_PIPELINE_STAGE_2_MESH_SHADER_BIT_EXT,
};
char *pipeline_stage_flag_bits_names[PIPELINE_STAGE_FLAG_BITS_COUNT] =
{
    "none",
    "top of pipe",
    "draw indirect",
    "vertex input",
    "vertex shader",
    "tessellation control shader",
    "tessellation evaluation shader",
    "geometry shader",
    "fragment shader",
    "early fragment tests",
    "late fragment tests",
    "color attachment output",
    "compute shader",
    "transfer",
    "bottom of pipe",
    "host",
    "all graphics",
    "all commands",
    "transform feedback",
    "acceleration structure build",
    "ray tracing",
    "fragment density process",
    "fragment shading rate attachment",
    "task",
    "mesh"
};

VkDependencyFlagBits dependency_flag_bits_enum[DEPENDENCY_FLAG_BITS_COUNT] =
{
    VK_DEPENDENCY_BY_REGION_BIT,
    VK_DEPENDENCY_DEVICE_GROUP_BIT,
    VK_DEPENDENCY_VIEW_LOCAL_BIT
};
char *dependency_flag_bits_name[DEPENDENCY_FLAG_BITS_COUNT] =
{
    "by region",
    "device group",
    "view local"
};

VkAccessFlags access_flag_bits_enum[ACCESS_FLAG_BITS_COUNT] =
{
    VK_ACCESS_NONE,
    VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
    VK_ACCESS_INDEX_READ_BIT,
    VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    VK_ACCESS_UNIFORM_READ_BIT,
    VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    VK_ACCESS_SHADER_READ_BIT,
    VK_ACCESS_SHADER_WRITE_BIT,
    VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_TRANSFER_READ_BIT,
    VK_ACCESS_TRANSFER_WRITE_BIT,
    VK_ACCESS_HOST_READ_BIT,
    VK_ACCESS_HOST_WRITE_BIT,
    VK_ACCESS_MEMORY_READ_BIT,
    VK_ACCESS_MEMORY_WRITE_BIT,
    VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT,
    VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT,
    VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT,
    VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT,
    VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT,
    VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR,
    VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR,
    VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT,
    VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR,
    VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV,
    VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV,
    VK_ACCESS_SHADING_RATE_IMAGE_READ_BIT_NV,
    VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_NV,
    VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_NV,
    VK_ACCESS_NONE_KHR
};
char *access_flag_bits_names[ACCESS_FLAG_BITS_COUNT] =
{
    "none",
    "indirect command read",
    "index read",
    "vertex attribute read",
    "uniform read",
    "input attachment read",
    "shader read",
    "shader write",
    "color attachment read",
    "color attachment write",
    "depth stencil attachment read",
    "depth stencil attachment write",
    "transfer read",
    "transfer write",
    "host read",
    "host write",
    "memory read",
    "memory write",
    "transform feedback write",
    "transform feedback counter read",
    "transform feedback counter write",
    "conditional rendering read",
    "color attachment read noncoherent",
    "acceleration structure read khr",
    "acceleration structure write",
    "fragment density map read",
    "fragment shading rate attachment read",
    "command preprocess read",
    "command preprocess write",
    "shading rate image read",
    "acceleration structure read",
    "acceleration structure write",
    "none khr"
};

char *texturing_addressing_keys [TEXTURING_MODE_COUNT] =
{
    "repeat",
    "mirror repeat",
    "clamp edge",
    "clamp border",
    "mirror clamp edge"
};
enum VkSamplerAddressMode  texturing_addressing_values [TEXTURING_MODE_COUNT] =
{
    VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
};

char *texture_filtering_keys [TILING_COUNT] =
{
    "linear",
    "nearest"
};
enum VkFilter  texture_filtering_values [TILING_COUNT] =
{
    VK_FILTER_LINEAR,
    VK_FILTER_NEAREST
};

char *tiling_keys [TILING_COUNT] =
{
    "optimal",
    "linear"
};
enum VkImageTiling  tiling_enum [TILING_COUNT] =
{
    VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_TILING_LINEAR
};

char *usage_keys [USAGE_COUNT] =
{
    "transfer source",
    "transfer destination",
    "sampled",
    "storage",
    "color attachment",
    "depth attachment",
    "transient attachment",
    "input attachment"
};
enum VkImageUsageFlagBits usage_enum [USAGE_COUNT] =
{
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_USAGE_STORAGE_BIT,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
};

char *view_type_names [VIEW_TYPE_COUNT] =
{
    "1D",
    "2D",
    "3D",
    "cubemap",
    "1D array",
    "2D array",
    "cube array"
};
enum VkImageViewType view_type_enums [VIEW_TYPE_COUNT] =
{
    VK_IMAGE_VIEW_TYPE_1D,
    VK_IMAGE_VIEW_TYPE_2D,
    VK_IMAGE_VIEW_TYPE_3D,
    VK_IMAGE_VIEW_TYPE_CUBE,
    VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
};

char *swizzle_names [SWIZZLE_COUNT] =
{
    "identity",
    "zero",
    "one",
    "r",
    "g",
    "b",
    "a"
};
enum VkComponentSwizzle swizzle_enums [SWIZZLE_COUNT] =
{
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_ZERO,
    VK_COMPONENT_SWIZZLE_ONE,
    VK_COMPONENT_SWIZZLE_R,
    VK_COMPONENT_SWIZZLE_G,
    VK_COMPONENT_SWIZZLE_B,
    VK_COMPONENT_SWIZZLE_A
};

char *aspect_names [ASPECT_COUNT] =
{
    "color",
    "depth",
    "stencil",
    "metadata"
};
enum VkImageAspectFlagBits aspect_enums [ASPECT_COUNT] =
{
    VK_IMAGE_ASPECT_COLOR_BIT,
    VK_IMAGE_ASPECT_DEPTH_BIT,
    VK_IMAGE_ASPECT_STENCIL_BIT,
    VK_IMAGE_ASPECT_METADATA_BIT
};

dict *presentation_modes_lut      = 0,
     *attachment_load_operations  = 0,
     *image_layouts               = 0,
     *subpass_functions           = 0,
     *attachment_store_operations = 0,
     *format_enumeration_lookup   = 0,
     *pipeline_stage_flag_bits    = 0,
     *access_flag_bits            = 0,
     *dependency_flag_bits        = 0,
     *texturing_modes             = 0,
     *filtering_modes             = 0,
     *tiling_lut                  = 0,
     *usage_lut                   = 0,
     *view_type_lut               = 0,
     *swizzle_lut                 = 0,
     *aspect_lut                  = 0;

int print_subpass ( GXSubpass_t *p_subpass );
int update_image_layout ( GXImage_t *p_image, char *format, char *new_layout );

int init_renderer ( void )
{

    // Construct lookup tables
    {
        if ( dict_construct(&presentation_modes_lut, PRESENTATION_MODES_COUNT)              == 0 ) goto failed_to_construct_presentation_modes_lut;
        if ( dict_construct(&attachment_load_operations, ATTACHMENT_LOAD_OPERATION_COUNT)   == 0 ) goto failed_to_construct_attachment_load_operations_lut;
        if ( dict_construct(&attachment_store_operations, ATTACHMENT_STORE_OPERATION_COUNT) == 0 ) goto failed_to_construct_attachment_store_operations_lut;
        if ( dict_construct(&image_layouts, IMAGE_LAYOUTS_COUNT)                            == 0 ) goto failed_to_construct_image_layouts_lut;
        if ( dict_construct(&subpass_functions, SUBPASS_FUNCTION_COUNT)                     == 0 ) goto failed_to_construct_subpass_functions_lut;
        if ( dict_construct(&format_enumeration_lookup, FORMAT_ENUM_COUNT)                  == 0 ) goto failed_to_construct_format_enumeration_lookup_lut;
        if ( dict_construct(&pipeline_stage_flag_bits, PIPELINE_STAGE_FLAG_BITS_COUNT)      == 0 ) goto failed_to_construct_pipeline_stage_flag_bits_lut;
        if ( dict_construct(&access_flag_bits, ACCESS_FLAG_BITS_COUNT)                      == 0 ) goto failed_to_construct_access_flag_bits_lut;
        if ( dict_construct(&dependency_flag_bits, DEPENDENCY_FLAG_BITS_COUNT)              == 0 ) goto failed_to_construct_dependency_flag_bits_lut;
        if ( dict_construct(&texturing_modes, TEXTURING_MODE_COUNT)                         == 0 ) goto failed_to_construct_texturing_modes_lut;
        if ( dict_construct(&filtering_modes, FILTERING_MODE_COUNT)                         == 0 ) goto failed_to_construct_filtering_modes_lut;
        if ( dict_construct(&tiling_lut, TILING_COUNT)                                      == 0 ) goto failed_to_construct_tiling_lut_lut;
        if ( dict_construct(&usage_lut, USAGE_COUNT)                                        == 0 ) goto failed_to_construct_usage_lut_lut;
        if ( dict_construct(&view_type_lut, VIEW_TYPE_COUNT)                                == 0 ) goto failed_to_construct_view_type_lut_lut;
        if ( dict_construct(&swizzle_lut, SWIZZLE_COUNT)                                    == 0 ) goto failed_to_construct_swizzle_lut_lut;
        if ( dict_construct(&aspect_lut, ASPECT_COUNT)                                      == 0 ) goto failed_to_construct_aspect_lut_lut;
    }

    // Populate lookup tables
    {

        // Populate the presentation modes
        for (size_t i = 0; i < PRESENTATION_MODES_COUNT; i++)
            dict_add(presentation_modes_lut, presentation_mode_names[i], (void *)presentation_mode_enums[i]);

        // Populate attachment load operations
        for (size_t i = 0; i < ATTACHMENT_LOAD_OPERATION_COUNT; i++)
            dict_add(attachment_load_operations, attachment_load_operation_names[i], (void *)attachment_load_operation_enums[i]);

        // Populate attachment store operations
        for (size_t i = 0; i < ATTACHMENT_STORE_OPERATION_COUNT; i++)
            dict_add(attachment_store_operations, attachment_store_operation_names[i], (void *)attachment_store_operation_enums[i]);

        // Populate image layouts
        for (size_t i = 0; i < IMAGE_LAYOUTS_COUNT; i++)
            dict_add(image_layouts, image_layout_names[i], (void *)image_layout_enums[i]);

        // Populate subpass functions
        for (size_t i = 0; i < SUBPASS_FUNCTION_COUNT; i++)
            dict_add(subpass_functions, subpass_function_names[i], (void *)subpass_function_callbacks[i]);

        // Populate formats
        for (size_t i = 0; i < FORMAT_ENUM_COUNT; i++)
            dict_add(format_enumeration_lookup, format_names[i], (void *)format_enums[i]);

        // Populate pipeline stage flag bits
        for (size_t i = 0; i < PIPELINE_STAGE_FLAG_BITS_COUNT; i++)
            dict_add(pipeline_stage_flag_bits, pipeline_stage_flag_bits_names[i], (void *)pipeline_stage_flag_bits_enum[i]);

        // Populate access flag bits
        for (size_t i = 0; i < ACCESS_FLAG_BITS_COUNT; i++)
            dict_add(access_flag_bits, access_flag_bits_names[i], (void *)(size_t)access_flag_bits_enum[i]);

        // Populate dependency flag bits
        for (size_t i = 0; i < DEPENDENCY_FLAG_BITS_COUNT; i++)
            dict_add(dependency_flag_bits, dependency_flag_bits_name[i], (void *)dependency_flag_bits_enum[i]);

        // Populate the texturing modes
        for (size_t i = 0; i < TEXTURING_MODE_COUNT; i++)
            dict_add(texturing_modes, texturing_addressing_keys[i], (void *) texturing_addressing_values[i]);

        // Populate the filtering modes
        for (size_t i = 0; i < FILTERING_MODE_COUNT; i++)
            dict_add(filtering_modes, texture_filtering_keys[i], (void *) texture_filtering_values[i]);

        // Populate tiling
        for (size_t i = 0; i < TILING_COUNT; i++)
            dict_add(tiling_lut, tiling_keys[i], (void *) tiling_enum[i]);

        // Populate usage flag bits
        for (size_t i = 0; i < USAGE_COUNT; i++)
            dict_add(usage_lut, usage_keys[i], (void *) usage_enum[i]);

        // Populate view types
        for (size_t i = 0; i < VIEW_TYPE_COUNT; i++)
            dict_add(view_type_lut, view_type_names[i], (void *) view_type_enums[i]);

        // Populate swizzles
        for (size_t i = 0; i < SWIZZLE_COUNT; i++)
            dict_add(swizzle_lut, swizzle_names[i], (void *) swizzle_enums[i]);

        // Populate aspects
        for (size_t i = 0; i < ASPECT_COUNT; i++)
            dict_add(aspect_lut, aspect_names[i], (void *) aspect_enums[i]);
    }

    // Success
    return 1;

    // Error handling
    {

        // Dictionary errors
        {
            failed_to_construct_presentation_modes_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct presentation_modes_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_attachment_load_operations_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct attachment_load_operations_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_attachment_store_operations_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct attachment_store_operations_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_image_layouts_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct image_layouts_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_subpass_functions_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct subpass_functions_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_format_enumeration_lookup_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct format_enumeration_lookup_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_pipeline_stage_flag_bits_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct pipeline_stage_flag_bits_lut dict in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_access_flag_bits_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct access_flag_bits_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_dependency_flag_bits_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct dependency_flag_bits_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_texturing_modes_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct texturing_modes_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_filtering_modes_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct filtering_modes_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_tiling_lut_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct tiling_lut_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_usage_lut_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct usage_lut_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_view_type_lut_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct view_type_lut_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_swizzle_lut_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct swizzle_lut_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_aspect_lut_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct aspect_lut_lut in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_renderer ( GXRenderer_t **pp_renderer )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_renderer == (void *) 0 ) goto no_ret;
    #endif

    // Initialized data
    GXRenderer_t *p_renderer = calloc(1, sizeof(GXRenderer_t));

    // Error check
    if ( p_renderer == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_renderer = p_renderer;

    // Successs
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_render_pass ( GXRenderPass_t **pp_render_pass )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_render_pass == (void *) 0 ) goto no_ret;
    #endif

    // Initialized data
    GXRenderPass_t *p_render_pass = calloc(1, sizeof(GXRenderPass_t));

    // Error check
    if ( p_render_pass == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_subpass ( GXSubpass_t **pp_subpass )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_subpass == (void *) 0 ) goto no_ret;
    #endif

    // Initialized data
    GXSubpass_t *p_subpass = calloc(1, sizeof(GXSubpass_t));

    // Error check
    if ( p_subpass == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_subpass = p_subpass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_image ( GXImage_t **pp_image )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_image == (void *) 0 ) goto no_image;
    #endif

    // Initialized data
    GXImage_t *p_image = calloc(1, sizeof(GXImage_t));

    // Error check
    if ( pp_image == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_image = p_image;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_image\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_attachment ( GXAttachment_t **pp_attachment )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_attachment == (void *) 0 ) goto no_attachment;
    #endif

    // Initialized data
    GXAttachment_t *p_attachment = calloc(1, sizeof(GXAttachment_t));

    // Error check
    if ( p_attachment == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_attachment = p_attachment;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_attachment:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_attachment\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_texture ( GXTexture_t **pp_texture )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_texture == (void *) 0 ) goto no_texture;
    #endif

    // Initialized data
    GXTexture_t *p_texture = calloc(1, sizeof(GXTexture_t));

    // Error check
    if ( p_texture == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_texture = p_texture;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_framebuffer ( GXFramebuffer_t **pp_framebuffer )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_framebuffer == (void *) 0 ) goto no_framebuffer;
    #endif

    // Initialized data
    GXFramebuffer_t *p_framebuffer = calloc(1, sizeof(GXSubpass_t));

    // Error check
    if ( p_framebuffer == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_framebuffer = p_framebuffer;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_framebuffer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_framebuffer\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_renderer ( GXRenderer_t **pp_renderer, char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_renderer == (void *) 0 ) goto no_renderer;
        if ( path        == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(1 + len, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_read_file;

    // Construct a renderer
    if ( load_renderer_as_json_text(pp_renderer, text) == 0 ) goto failed_to_load_renderer;

    // Clean the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load renderer from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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

int load_renderer_as_json_text ( GXRenderer_t **pp_renderer, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_renderer == (void *) 0 ) goto no_renderer;
        if ( text        == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    GXRenderer_t *p_renderer = 0;
    json_value  *p_value    = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Load the renderer from the JSON value
    if ( load_renderer_as_json_value(pp_renderer, p_value) == 0 ) goto failed_to_construct_renderer_from_json_value;

    // Clean the scope
    //free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_construct_renderer_from_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct renderer in in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to parse JSON text in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_renderer_as_json_value ( GXRenderer_t **pp_renderer, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_renderer == (void *) 0 ) goto no_return;
        if ( p_value     == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance        = g_get_active_instance();
    GXRenderer_t *p_renderer        = 0;
    json_value  *p_name            = 0,
                 *p_passes          = 0,
                 *p_attachments     = 0,
                 *p_synchronization = 0,
                 *p_clear_color     = 0;

    // Parse the value as an object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name            = dict_get(p_dict, "name");
        p_passes          = dict_get(p_dict, "passes");
        p_clear_color     = dict_get(p_dict, "clear color");
        p_synchronization = dict_get(p_dict, "synchronization");

        // Optional properties
        p_attachments = dict_get(p_dict, "attachments");

        // Error check
        if ( ! (
            p_name        &&
            p_passes      &&
            p_clear_color &&
            p_synchronization
        ) )
            goto missing_properties;
    }
    // Parse the value as a path
    else if ( p_value->type == JSON_VALUE_STRING )
    {

        // Load the renderer as a pass
        if ( load_renderer(pp_renderer, p_value->string) == 0 ) goto faild_to_load_renderer;

        // Success
        return 1;
    }
    // Default
    else
        goto wrong_value_type;

    // Construct the renderer
    {

        // Initialized data
        VkClearValue *clear_color = 0;

        // Allocate a renderer
        if ( create_renderer(&p_renderer) == 0 ) goto failed_to_allocate_renderer;

        // Set the active instance's loading scene
        p_instance->context.loading_renderer = p_renderer;

        // Copy the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for the name
            p_renderer->name = calloc(len + 1, sizeof(char));

            // Error check
            if ( p_renderer->name == (void *) 0 ) goto no_mem;

            // Copy the name to the renderer
            strncpy(p_renderer->name, p_name->string, len);
        }
        // Default
        else
            goto wrong_name_type;

        // Parse each attachment
        if ( p_attachments->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            size_t array_len = 0;
            json_value **pp_array_contents = 0;

            // Get the array contents
            {

                // Get the array length
                array_get(p_attachments->list, 0, &array_len);

                // Allocate memory for array
                pp_array_contents = calloc(array_len, sizeof(json_value *));

                // Error check
                if ( pp_array_contents == (void *) 0 ) goto no_mem;

                // Dump array to memory
                array_get(p_attachments->list, (void **)pp_array_contents, 0);
            }

            // Construct a dictionary for the attachments
            dict_construct(&p_renderer->attachments, array_len);

            // Iterate over each attachment JSON value
            for (size_t i = 0; i < array_len; i++)
            {

                // Initialized data
                GXAttachment_t *p_attachment = 0;

                // Load the attachment as a JSON value
                if ( load_attachment_as_json_value(&p_attachment, pp_array_contents[i]) == 0 ) goto failed_to_load_attachment_as_json_value;

                // Add the attachment
                dict_add(p_renderer->attachments, p_attachment->name, p_attachment);
            }
        }
        // Default
        else
            goto wrong_attachments_type;

        // Parse the clear color as an array
        if ( p_clear_color->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            size_t        array_len = 0;
            json_value **pp_array_contents = 0;
            size_t        attachment_count = dict_values(p_renderer->attachments, 0);;

            // Get the array contents
            {

                // Get the array length
                array_get(p_clear_color->list, 0, &array_len);

                // Allocate memory for array
                pp_array_contents = calloc(array_len, sizeof(json_value *));

                // Error check
                if ( pp_array_contents == (void *) 0 ) goto no_mem;

                // Dump array to memory
                array_get(p_clear_color->list, (void **)pp_array_contents, 0);
            }

            // TODO: vvvvvv Fix this vvvvv

            // Allocate clear colors for each attachment
            clear_color = calloc(2*attachment_count, sizeof(VkClearValue));

            // Error check
            if ( clear_color == (void *) 0 ) goto no_mem;

            // Iterate over each array element
            for (size_t i = 0; i < array_len; i++)

                // Set the clear color
                clear_color[0].color.float32[i] = (float)pp_array_contents[i]->number;

            // Set the depth stencil clear color
            clear_color[1].depthStencil.depth = 1.f;
            clear_color[1].depthStencil.stencil = 0;

            p_renderer->clear_colors = calloc(attachment_count*2,sizeof(VkClearValue));

            // Store the clear color in the renderer
            for (size_t i = 0; i < attachment_count*2; i++)
                p_renderer->clear_colors[i] = *clear_color;

        }
        // Default
        else
            goto wrong_renderer_clear_color_type;

        // Parse each render pass
        if ( p_passes->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            size_t array_len = 0;
            json_value **pp_array_contents = 0;

            // Get the array contents
            {

                // Get the array length
                array_get(p_passes->list, 0, &array_len);

                // Allocate memory for array
                pp_array_contents = calloc(array_len, sizeof(json_value *));

                // Error check
                if ( pp_array_contents == (void *) 0 ) goto no_mem;

                // Dump array to memory
                array_get(p_passes->list, (void **)pp_array_contents, 0);
            }

            // Allocate memory for render passes array
            p_renderer->render_passes_data = calloc(array_len, sizeof(GXRenderPass_t *));

            // Error check
            if ( p_renderer->render_passes_data == (void *) 0 ) goto no_mem;

            // Set the render pass count
            p_renderer->render_pass_count = array_len;

            // Iterate over each render pass
            for (size_t i = 0; i < array_len; i++)
            {

                // Load the render pass as a JSON value
                if ( load_render_pass_as_json_value(&p_renderer->render_passes_data[i], pp_array_contents[i]) == 0 ) goto failed_to_load_renderer_as_json_value;

                dict_add(p_renderer->render_passes, p_renderer->render_passes_data[i]->name, p_renderer->render_passes_data[i]);
            }
        }
        else
            goto wrong_passes_type;

        // This is no longer the loading renderer
        p_instance->context.loading_renderer = 0;

        // Return a pointer to the caller
        *pp_renderer = p_renderer;
    }

    // Construct a command pool for the renderer
    {

        // Initialized data
        VkQueue queue = VK_NULL_HANDLE;
        VkCommandPoolCreateInfo command_pool_create_info =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = 0,
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = p_instance->vulkan.queues.graphics_family_indicies[0] // TODO: Some type of g_get_vulkan_queue_family_index function?
        };

        // Create the command pool
        if ( 
            vkCreateCommandPool(
                p_instance->vulkan.device,
                &command_pool_create_info,
                (void *) 0,
                &p_renderer->command_pool
            ) != VK_SUCCESS
        )
            goto failed_to_create_command_pool;
    }

    // Allocate command buffer(s) for the renderer
    {

        // Initialized data
        VkCommandBufferAllocateInfo command_buffer_allocate_info =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = 0,
            .commandPool = p_renderer->command_pool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, // TODO: change this contingent on something.
            .commandBufferCount = p_instance->vulkan.max_buffered_frames
        };

        // Allocate memory for command buffers
        p_renderer->command_buffers = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkCommandBuffer));

        // Error chcek
        // TODO:
        //

        // Allocate command buffers
        if (
            vkAllocateCommandBuffers(
                p_instance->vulkan.device,
                &command_buffer_allocate_info,
                p_renderer->command_buffers
            ) != VK_SUCCESS
        )
            goto failed_to_allocate_command_buffers;
    }

    // Construct synchronization objects
    if ( p_synchronization->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        json_value *p_synchronization_binary_semaphores = 0,
                    *p_synchronization_fences            = 0;

        // Parse the JSON
        {

            // Initialized data
            dict *p_dict = p_synchronization->object;

            // Required properties
            p_synchronization_binary_semaphores = dict_get(p_dict, "binary semaphores");
            p_synchronization_fences            = dict_get(p_dict, "fences");

            // Error check
            if ( ! (
                p_synchronization_binary_semaphores &&
                p_synchronization_fences
            ) )
                goto missing_synchronization_properties;
        }

        // Construct synchronization objects
        {

            // Initialized data
            size_t binary_semaphores_count = 0,
                   fences_count            = 0;

            // Construct binary semaphores
            {

                // Initialized data
                json_value **pp_binary_semaphore_names = 0;
                dict         *p_semaphore_dict          = 0;

                // Dump the array
                {

                    // Get the quantity of semaphores
                    array_get(p_synchronization_binary_semaphores->list, 0, &binary_semaphores_count);

                    // Allocate memory for semaphores
                    pp_binary_semaphore_names = calloc(binary_semaphores_count, sizeof(json_value *));

                    // Error check
                    if ( pp_binary_semaphore_names == (void *) 0 ) goto no_mem;

                    // Get the contents of the array
                    array_get(p_synchronization_binary_semaphores->list, pp_binary_semaphore_names, 0);
                }

                // Construct a dict 
                dict_construct(&p_semaphore_dict, binary_semaphores_count);

                // Iterate over each value
                for (size_t i = 0; i < binary_semaphores_count; i++)
                {

                    // Initialized data
                    VkSemaphoreCreateInfo semaphore_create_info = 
                    {
                        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                        .pNext = 0,                                       
                        .flags = 0 // Must be zero
                    };
                    VkSemaphore *p_semaphore = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkSemaphore));

                    // Iterate over each graphics command buffer
                    for (size_t j = 0; j < p_instance->vulkan.max_buffered_frames; j++)
                        
                        // Construct a semaphore
                        if ( 
                            vkCreateSemaphore(
                                p_instance->vulkan.device,
                                &semaphore_create_info,
                                0,
                                &p_semaphore[j]
                            ) != VK_SUCCESS
                        ) 
                            goto failed_to_create_semaphore;

                    // Add the semaphore to the semaphore dictionary
                    dict_add(p_semaphore_dict, pp_binary_semaphore_names[i]->string, p_semaphore);
                }
                
                // Store the binary semaphore dictionary in the renderer
                p_renderer->synchronization.binary_semaphores = p_semaphore_dict;

                // Clean the scope
                free(pp_binary_semaphore_names);
            }
        
            // Construct the fences
            {

                // Initialized data
                char        **fences_names    = 0;
                json_value **fences_signaled = 0;
                dict         *p_fence_dict    = 0;

                // Parse the JSON
                {
                    
                    // Initialized data
                    dict   *p_dict = p_synchronization_fences->object;
                    
                    // Get the quantity of properties in the dictionary
                    fences_count = dict_keys(p_dict, 0);

                    // Allocate memory for properties
                    fences_names    = calloc(fences_count, sizeof(char *));
                    fences_signaled = calloc(fences_count, sizeof(json_value *));

                    // Error check
                    if ( fences_names    == (void *) 0 ) goto no_mem;
                    if ( fences_signaled == (void *) 0 ) goto no_mem;

                    // Dump the contents of the dictionary
                    dict_keys(p_dict, fences_names);
                    dict_values(p_dict, fences_signaled);
                }

                // Construct a dict 
                dict_construct(&p_fence_dict, fences_count);

                // Iterate over each fence
                for (size_t i = 0; i < fences_count; i++)
                {
                    
                    // Initialized data
                    VkFenceCreateInfo fence_create_info = 
                    {
                        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
                        .pNext = 0,
                        .flags = 0
                    };
                    VkFence *p_fences = 0;

                    // Is this semaphore meant to be signaled?
                    if ( fences_signaled[i]->boolean )
                        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
                    
                    // Allocate memory for fences
                    p_fences = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkFence));

                    // Error check
                    if ( p_fences == (void *) 0 ) goto no_mem;

                    // Iterate over each value
                    for (size_t j = 0; j < p_instance->vulkan.max_buffered_frames; j++)
                    {

                        // Create a fence
                        if (
                            vkCreateFence(
                                p_instance->vulkan.device,
                                &fence_create_info,
                                0,
                                &p_fences[j]
                            ) != VK_SUCCESS
                        )
                            goto failed_to_create_fence;
                    }

                    // Copy the name and add it to the dictionary
                    {

                        // Initialized data
                        size_t  len = strlen(fences_names[i]);
                        char   *name = calloc(len+1, sizeof(char));

                        // Error check
                        if ( name == (void *) 0 ) goto no_mem;
                        
                        // Copy the name
                        strncpy(name, fences_names[i], len);

                        // Add the fence to the dictionary
                        dict_add(p_fence_dict, name, p_fences);
                    }
                }

                // Store the fences dictionary in the renderer
                p_renderer->synchronization.fences = p_fence_dict;

                // Clean the scope
                free(fences_names);
                free(fences_signaled);
            }
        }
    }
    // Default
    else
        goto wrong_synchronization_type;

    // Success
    return 1;

    missing_synchronization_properties:
    wrong_synchronization_type:
    failed_to_create_semaphore:
    failed_to_create_fence:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_return:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"p_value\" must be of type [ string | object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_attachments_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"attachments\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_renderer_clear_color_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"clear color\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_passes_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"passes\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Missing properties in parameter \"p_value\" in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to allocate renderer in in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_renderer_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load renderer as JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_attachment_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load attachment as JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            faild_to_load_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load renderer in call to function \"%s\"\n", __FUNCTION__);
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

        // Vulkan errors
        {
            failed_to_create_command_pool:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to function \"vkCreateCommandPool\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_command_buffers:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to function \"vkAllocateCommandBuffers\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_render_pass ( GXRenderPass_t **pp_render_pass, char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
        if ( path           == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(1 + len, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_read_file;

    // Construct a render pass
    if ( load_render_pass_as_json_text(pp_render_pass, text) == 0 ) goto failed_to_load_render_pass;

    // Clean the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load render pass from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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

int load_render_pass_as_json_text ( GXRenderPass_t **pp_render_pass, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
        if ( text           == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXRenderPass_t *p_render_pass = 0;
    json_value *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json_as_value;

    // Load the renderer as a JSON value
    if ( load_render_pass_as_json_value(pp_render_pass, p_value) == 0 ) goto failed_to_load_renderer_as_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_parse_json_as_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to parse JSON text into JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_renderer_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct renderer from JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_render_pass_as_json_value ( GXRenderPass_t **pp_render_pass, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
        if ( p_value        == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t    *p_instance            = g_get_active_instance();
    GXRenderPass_t  *p_render_pass         = 0;
    json_value     *p_name                = 0,
                    *p_attachments         = 0,
                    *p_shaders             = 0,
                    *p_subpasses           = 0,
                    *p_dependencies        = 0,
                   **pp_subpasses_contents = 0;
    size_t           subpass_count         = 0;

    // Parse the render pass JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name      = dict_get(p_dict, "name");
        p_subpasses = dict_get(p_dict, "subpasses");

        // Optional properties
        p_shaders      = dict_get(p_dict, "shaders");
        p_attachments  = dict_get(p_dict, "attachments");
        p_dependencies = dict_get(p_dict, "dependencies");

        // Check for missing properties
        if ( ! (
            p_name      &&
            p_subpasses
        ) )
            goto missing_properties;
    }
    // Default
    else
        goto wrong_type;

    // Construct the render pass
    {

        // Initialized data
        VkRenderPassCreateInfo2   render_pass_2_create_info = { 0 };
        VkSubpassDependency       dependency                = { 0 };
        size_t                    attachment_count          = 0,
                                  dependency_count          = 0,
                                  correlated_mask_count     = 0;
        unsigned int             *correlated_mask_views     = 0;
        VkAttachmentDescription2 *attachments               = 0;
        VkAttachmentReference    *attachment_references     = 0;
        VkSubpassDescription2    *subpasses                 = 0;
        VkSubpassDependency2     *dependencies              = 0;

        // Allocate memory for a render pass
        if ( create_render_pass(&p_render_pass) == 0 ) goto failed_to_create_render_pass;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for name
            p_render_pass->name = calloc(len + 1, sizeof(char));

            // Error check
            if ( p_render_pass->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_render_pass->name, p_name->string, len);
        }
        // Default
        else
            goto wrong_name_type;

        // Populate attachments
        if ( p_attachments )
        {
            if ( p_attachments->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t array_len = 0;
                json_value **pp_array_contents = 0;

                // Get the array contents
                {

                    // Get the array length
                    array_get(p_attachments->list, 0, &array_len);

                    // Allocate memory for array
                    pp_array_contents = calloc(array_len, sizeof(json_value *));

                    // Error check
                    if ( pp_array_contents == (void *) 0 ) goto no_mem;

                    // Dump array to memory
                    array_get(p_attachments->list, (void **)pp_array_contents, 0);
                }

                // Allocate memory for attachment descriptions
                attachments = calloc(array_len, sizeof(VkAttachmentDescription));

                // Error check
                if ( attachments == (void *) 0 ) goto no_mem;

                attachment_count = array_len;

                // Allocate a list of attachments
                if ( dict_construct(&p_render_pass->attachments, array_len) == 0 )
                    return 0; //goto failed_to_construct_attachments_list;

                // Iterate over each attachment JSON object text
                for (size_t i = 0; i < array_len; i++)
                {
                    if ( pp_array_contents[i]->type == JSON_VALUE_STRING )
                    {

                        // Initialized data
                        GXAttachment_t *p_attachment = dict_get(p_instance->context.loading_renderer->attachments, pp_array_contents[i]->string);

                        dict_add(p_render_pass->attachments, p_attachment->name, p_attachment->name);

                        attachments[i] = p_attachment->attachment_description2;
                    }
                }

                p_render_pass->attachments_count = array_len;
            }
            // Default
            else
                goto wrong_attachments_type;
        }
        // Default
        else
        {
        }

        // Populate subpasses
        if ( p_subpasses->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            size_t array_len = 0;

            // Get the array contents
            {

                // Get the array length
                array_get(p_subpasses->list, 0, &array_len);

                // Allocate memory for array
                pp_subpasses_contents = calloc(array_len, sizeof(json_value *));

                // Error check
                if ( pp_subpasses_contents == (void *) 0 ) goto no_mem;

                // Dump array to memory
                array_get(p_subpasses->list, (void **)pp_subpasses_contents, 0);
            }

            // Allocate memory for subpass description
            subpasses                     = calloc(array_len, sizeof(VkSubpassDescription2));
            p_render_pass->subpasses_data = calloc(array_len, sizeof(GXSubpass_t *));

            subpass_count = array_len;

            // Error check
            if ( subpasses == (void *) 0 ) goto no_mem;

            // Construct a dictionary
            dict_construct(&p_render_pass->subpasses, array_len);
            // Initialized data
            GXSubpass_t *p_subpass = 0;

            // Iterate over each subpass JSON object text
            for (size_t i = 0; i < array_len; i++)
            {

                // Load the subpass as a JSON value
                if ( load_subpass_as_json_value(&p_subpass, pp_subpasses_contents[i]) == 0 ) goto failed_to_load_subpass_as_json_value;

                subpasses[i] = p_subpass->subpass_description;
                p_render_pass->subpasses_data[i] = p_subpass;

                dict_add(p_render_pass->subpasses, p_subpass->name, p_subpass);
            }
            p_render_pass->subpasses_count = array_len;
        }
        // Default
        else
            goto wrong_subpasses_type;

        // Populate dependencies
        if ( p_dependencies )
        {
            if ( p_dependencies->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t        array_len         = 0;
                json_value **pp_array_contents = 0;

                // Get the array contents
                {

                    // Get the array length
                    array_get(p_dependencies->list, 0, &array_len);

                    // Allocate memory for array
                    pp_array_contents = calloc(array_len, sizeof(json_value *));

                    // Error check
                    if ( pp_array_contents == (void *) 0 ) goto no_mem;

                    // Dump array to memory
                    array_get(p_dependencies->list, (void **)pp_array_contents, 0);
                }

                // Allocate memory for dependencies
                dependencies     = calloc(array_len, sizeof(VkSubpassDescription));
                dependency_count = array_len;

                // Error check
                if ( dependencies == (void *) 0 ) goto no_mem;

                // Iterate over each dependencies JSON object text
                for (size_t i = 0; i < array_len; i++)
                {

                    // Initialized data
                    json_value *p_dependency = pp_array_contents[i],
                                *p_dependency_flags = 0,
                                *p_dependency_source_subpass = 0,
                                *p_dependency_destination_subpass = 0,
                                *p_dependency_source_stage = 0,
                                *p_dependency_destination_stage = 0,
                                *p_dependency_source_access = 0,
                                *p_dependency_destination_access = 0;

                    // Parse the JSON
                    {

                        // Initialized data
                        dict *p_dict = p_dependency->object;

                        // Required properties
                        p_dependency_flags               = dict_get(p_dict, "flags");
                        p_dependency_destination_subpass = dict_get(p_dict, "destination subpass");
                        p_dependency_source_stage        = dict_get(p_dict, "source stage");
                        p_dependency_destination_stage   = dict_get(p_dict, "destination stage");
                        p_dependency_destination_access  = dict_get(p_dict, "destination access");

                        // Optional properties
                        p_dependency_source_subpass      = dict_get(p_dict, "source subpass");
                        p_dependency_source_access       = dict_get(p_dict, "source access");

                        // Check properties
                        if ( ! (
                            p_dependency_flags &&
                            p_dependency_destination_subpass &&
                            p_dependency_source_stage &&
                            p_dependency_destination_stage &&
                            p_dependency_destination_access
                        ) )
                            goto missing_properties;
                    }

                    // Construct the dependency
                    {
                        uint32_t             source_subpass_index      = 0,
                                             destination_subpass_index = 0;
                        VkPipelineStageFlags source_stage_mask         = 0,
                                             destination_stage_mask    = 0;
                        VkAccessFlags        source_access_mask        = 0,
                                             destination_access_mask   = 0;
                        VkDependencyFlags    dependency_flags          = (VkDependencyFlags) (size_t) dict_get(dependency_flag_bits, p_dependency_flags->string);

                        if ( p_dependency_source_subpass )
                        {

                            // Parse the flags as an array
                            if ( p_dependency_source_subpass->type == JSON_VALUE_ARRAY )
                            {

                                // Initialized data
                                size_t flags_count = 0;
                                json_value **pp_flags = 0;

                                // Get the array contents
                                {

                                    // Get the size of the array
                                    array_get(p_dependency_source_subpass->list, 0, &flags_count);

                                    // Allocate memory for the array
                                    pp_flags = calloc(flags_count, sizeof(json_value *));

                                    // Error handling
                                    if ( pp_flags == (void *) 0 ) goto no_mem;

                                    // Get the contents of the array
                                    array_get(p_dependency_source_subpass->list, (void **)pp_flags, 0);
                                }

                                // Iterate over each dependency_source_subpass
                                for (size_t i = 0; i < flags_count; i++)
                                {

                                    // Initialized data
                                    json_value *i_flag = pp_flags[i];

                                    // Set the flag
                                    if ( i_flag->type == JSON_VALUE_STRING )
                                    {

                                    }
                                    // Default
                                    else
                                        goto wrong_dependency_source_subpass_type;
                                }

                            }
                            // Parse the flags as a string
                            else if ( p_dependency_source_subpass->type == JSON_VALUE_STRING )
                            {
                                // TODO
                            }
                            // Default
                            else
                                goto wrong_dependency_source_subpass_type;
                        }

                        if ( p_dependency_destination_subpass )
                        {

                            // Parse the flags as an array
                            if ( p_dependency_destination_subpass->type == JSON_VALUE_ARRAY )
                            {

                                // Initialized data
                                size_t flags_count = 0;
                                json_value **pp_flags = 0;

                                // Get the array contents
                                {

                                    // Get the size of the array
                                    array_get(p_dependency_destination_subpass->list, 0, &flags_count);

                                    // Allocate memory for the array
                                    pp_flags = calloc(flags_count, sizeof(json_value *));

                                    // Error handling
                                    if ( pp_flags == (void *) 0 ) goto no_mem;

                                    // Get the contents of the array
                                    array_get(p_dependency_destination_subpass->list, (void **)pp_flags, 0);
                                }

                                // Iterate over each dependency_destination_subpass
                                for (size_t i = 0; i < flags_count; i++)
                                {

                                    // Initialized data
                                    json_value *i_flag = pp_flags[i];

                                    // Set the flag
                                    if ( i_flag->type == JSON_VALUE_STRING )
                                    {

                                    }
                                    // Default
                                    else
                                        goto wrong_dependency_destination_subpass_type;
                                }

                                // Clean the scope
                                free(pp_flags);
                            }
                            // Parse the flags as a string
                            else if ( p_dependency_destination_subpass->type == JSON_VALUE_STRING )
                            {

                            }
                            // Default
                            else
                                goto wrong_dependency_destination_subpass_type;
                        }

                        if ( p_dependency_source_stage )
                        {

                            // Parse the flags as an array
                            if ( p_dependency_source_stage->type == JSON_VALUE_ARRAY )
                            {

                                // Initialized data
                                size_t flags_count = 0;
                                json_value **pp_flags = 0;

                                // Get the array contents
                                {

                                    // Get the size of the array
                                    array_get(p_dependency_source_stage->list, 0, &flags_count);

                                    // Allocate memory for the array
                                    pp_flags = calloc(flags_count, sizeof(json_value *));

                                    // Error handling
                                    if ( pp_flags == (void *) 0 ) goto no_mem;

                                    // Get the contents of the array
                                    array_get(p_dependency_source_stage->list, (void **)pp_flags, 0);
                                }

                                // Iterate over each dependency_source_stage
                                for (size_t i = 0; i < flags_count; i++)
                                {

                                    // Initialized data
                                    json_value *i_flag = pp_flags[i];

                                    // Set the flag
                                    if ( i_flag->type == JSON_VALUE_STRING )
                                        source_stage_mask |= (VkPipelineStageFlags) (size_t) dict_get(pipeline_stage_flag_bits, i_flag->string);
                                    // Default
                                    else
                                        goto wrong_dependency_source_stage_type;
                                }
                            }
                            // Parse the flags as a string
                            else if ( p_dependency_source_stage->type == JSON_VALUE_STRING )
                                source_stage_mask = (VkPipelineStageFlags) (size_t) dict_get(pipeline_stage_flag_bits, p_dependency_source_stage->string);
                            // Default
                            else
                                goto wrong_dependency_source_stage_type;
                        }

                        if ( p_dependency_destination_stage )
                        {

                            // Parse the flags as an array
                            if ( p_dependency_destination_stage->type == JSON_VALUE_ARRAY )
                            {

                                // Initialized data
                                size_t flags_count = 0;
                                json_value **pp_flags = 0;

                                // Get the array contents
                                {

                                    // Get the size of the array
                                    array_get(p_dependency_destination_stage->list, 0, &flags_count);

                                    // Allocate memory for the array
                                    pp_flags = calloc(flags_count+1, sizeof(json_value *));

                                    // Error handling
                                    if ( pp_flags == (void *) 0 ) goto no_mem;

                                    // Get the contents of the array
                                    array_get(p_dependency_destination_stage->list, (void **)pp_flags, 0);
                                }

                                // Iterate over each dependency_destination_stage
                                for (size_t i = 0; i < flags_count; i++)
                                {

                                    // Initialized data
                                    json_value *i_flag = pp_flags[i];

                                    // Set the flag
                                    if ( i_flag->type == JSON_VALUE_STRING )
                                        destination_stage_mask |= (VkPipelineStageFlags) (size_t) dict_get(pipeline_stage_flag_bits, i_flag->string);
                                    // Default
                                    else
                                        goto wrong_dependency_destination_stage_type;
                                }

                                // Clean the scope
                                free(pp_flags);
                            }
                            // Parse the flags as a string
                            else if ( p_dependency_destination_stage->type == JSON_VALUE_STRING )
                                destination_stage_mask = (VkPipelineStageFlags) (size_t) dict_get(pipeline_stage_flag_bits, p_dependency_destination_stage->string);
                            // Default
                            else
                                goto wrong_dependency_destination_stage_type;
                        }

                        if ( p_dependency_source_access )
                        {

                            // Parse the flags as an array
                            if ( p_dependency_source_access->type == JSON_VALUE_ARRAY )
                            {

                                // Initialized data
                                size_t flags_count = 0;
                                json_value **pp_flags = 0;

                                // Get the array contents
                                {

                                    // Get the size of the array
                                    array_get(p_dependency_source_access->list, 0, &flags_count);

                                    // Allocate memory for the array
                                    pp_flags = calloc(flags_count, sizeof(json_value *));

                                    // Error handling
                                    if ( pp_flags == (void *) 0 ) goto no_mem;

                                    // Get the contents of the array
                                    array_get(p_dependency_source_access->list, (void **)pp_flags, 0);
                                }

                                // Iterate over each dependency_source_access
                                for (size_t i = 0; i < flags_count; i++)
                                {

                                    // Initialized data
                                    json_value *i_flag = pp_flags[i];

                                    // Set the flag
                                    if ( i_flag->type == JSON_VALUE_STRING )
                                        source_access_mask |= (VkAccessFlags) (size_t) dict_get(access_flag_bits, i_flag->string);
                                    // Default
                                    else
                                        goto wrong_dependency_source_access_type;
                                }

                                // Clean the scope
                                free(pp_flags);
                            }
                            // Parse the flags as a string
                            else if ( p_dependency_source_access->type == JSON_VALUE_STRING )
                                source_access_mask = (VkAccessFlags) (size_t) dict_get(access_flag_bits, p_dependency_source_access->string);
                            // Default
                            else
                                goto wrong_dependency_source_access_type;
                        }

                        if ( p_dependency_destination_access )
                        {

                            // Parse the flags as an array
                            if ( p_dependency_destination_access->type == JSON_VALUE_ARRAY )
                            {

                                // Initialized data
                                size_t flags_count = 0;
                                json_value **pp_flags = 0;

                                // Get the array contents
                                {

                                    // Get the size of the array
                                    array_get(p_dependency_destination_access->list, 0, &flags_count);

                                    // Allocate memory for the array
                                    pp_flags = calloc(flags_count, sizeof(json_value *));

                                    // Error handling
                                    if ( pp_flags == (void *) 0 ) goto no_mem;

                                    // Get the contents of the array
                                    array_get(p_dependency_destination_access->list, (void **)pp_flags, 0);
                                }

                                // Iterate over each dependency_destination_access
                                for (size_t i = 0; i < flags_count; i++)
                                {

                                    // Initialized data
                                    json_value *i_flag = pp_flags[i];

                                    // Set the flag
                                    if ( i_flag->type == JSON_VALUE_STRING )
                                        destination_access_mask |= (VkAccessFlags) (size_t)  dict_get(access_flag_bits, i_flag->string);
                                    // Default
                                    else
                                        goto wrong_dependency_destination_access_type;
                                }

                                // Clean the scope
                                //free(pp_flags);
                            }
                            // Parse the flags as a string
                            else if ( p_dependency_destination_access->type == JSON_VALUE_STRING )
                                destination_access_mask = (VkAccessFlags) (size_t) dict_get(access_flag_bits, p_dependency_destination_access->string);
                            // Default
                            else
                                goto wrong_dependency_destination_access_type;
                        }

                        dependencies[i] = (VkSubpassDependency2)
                        {
                            .sType           = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2,
                            .pNext           = 0,
                            .srcSubpass      = VK_SUBPASS_EXTERNAL,
                            .dstSubpass      = destination_subpass_index,
                            .srcStageMask    = source_stage_mask,
                            .dstStageMask    = destination_stage_mask,
                            .srcAccessMask   = source_access_mask,
                            .dstAccessMask   = destination_access_mask,
                            .dependencyFlags = dependency_flags,
                            .viewOffset      = 0
                        };
                    }
                }
            }
            // Default
            else
                goto wrong_dependencies_type;
        }
        // Default
        else
        {
        }

        // Populate render pass create info struct
        render_pass_2_create_info =
        (VkRenderPassCreateInfo2)
        {
            .sType                   = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2,
            .pNext                   = 0,
            .flags                   = 0,
            .attachmentCount         = (u32)attachment_count,
            .pAttachments            = attachments,
            .subpassCount            = (u32)subpass_count,
            .pSubpasses              = subpasses,
            .dependencyCount         = (u32)dependency_count,
            .pDependencies           = dependencies,
            .correlatedViewMaskCount = (u32)correlated_mask_count,
            .pCorrelatedViewMasks    = correlated_mask_views
        };

        // Create a render pass2
        if ( 
            vkCreateRenderPass2(
                p_instance->vulkan.device,
                &render_pass_2_create_info,
                0,
                &p_render_pass->render_pass
            ) != VK_SUCCESS
        )
            goto failed_to_create_render_pass;

    }

    // Set the render pass
    p_instance->context.loading_renderer->current_render_pass = p_render_pass;

    // Construct a dictionary
    dict_construct(&p_instance->context.loading_renderer->current_render_pass->draw_queue_types, subpass_count);

    // Load each subpasses shader
    for ( size_t i = 0; i < subpass_count; i++ )
    {

        // Initialized data
        GXSubpass_t *p_subpass        = 0;
        GXShader_t  *p_shader         = 0;
        json_value *p_subpass_value  = pp_subpasses_contents[i],
                    *p_name           = 0,
                    *p_shaders_value  = 0;
        queue       *p_draw_queue     = 0;

        // Get the required information to load a subpasses shader
        if ( p_subpass_value->type == JSON_VALUE_OBJECT )
        {

            // Initialized data
            dict *p_dict = p_subpass_value->object;

            // Required properties
            p_name          = dict_get(p_dict, "name");
            p_shaders_value = dict_get(p_dict, "shaders");

            // Error check
            if ( ! (
                p_name          &&
                p_shaders_value
            ) )
                goto missing_properties;
        }

        // Get the shaders
        {

            // Initialized data
            size_t        shader_count     = 0;
            json_value **pp_shader_values = 0;

            // Get the contents of the array
            if ( p_shaders_value->type == JSON_VALUE_ARRAY )
            {

                // Get the quantity of shaders
                array_get(p_shaders_value->list, 0, &shader_count);

                // Allocate for the array contents
                pp_shader_values = calloc(shader_count, sizeof(json_value *));

                // Error check
                if ( pp_shader_values == (void *) 0 ) goto no_mem;

                // Get the array contents
                array_get(p_shaders_value->list, (void **)pp_shader_values, 0);
            }

            // Get the subpass
            p_subpass = dict_get(p_render_pass->subpasses, p_name->string);

            // Allocate memory for an array of shader names
            p_subpass->shader_names = calloc(shader_count, sizeof(char *));

            // Error check
            if ( p_subpass->shader_names == (void *) 0 ) goto no_mem;

            // Set the shader count
            p_subpass->shader_count = shader_count;

            // Iterate over each shader
            for (size_t i = 0; i < shader_count; i++)
            {

                // Initialized data
                json_value *p_shader_value = pp_shader_values[i];

                // Load the shader
                if ( load_shader_as_json_value(&p_shader, p_shader_value) == 0 ) goto failed_to_load_shader_as_json_value;

                // Construct a draw queue for the shader
                queue_construct(&p_draw_queue);

                // Add the draw queue to the subpass
                dict_add(p_instance->context.loading_renderer->current_render_pass->draw_queue_types, p_shader->name, p_draw_queue);

                // Set the subpasses shader
                p_subpass->shader_names[i] = p_shader->name;
            }
        }
    }

    // TODO: Check
    p_render_pass->framebuffers = calloc(p_instance->vulkan.image_count, sizeof(VkFramebuffer));

    // TODO: Improve
    // Create frame buffers
    for (size_t i = 0; i < p_instance->vulkan.image_count; i++)
    {

        // Initialized data
        size_t        attachment_count = p_render_pass->attachments_count;
        VkImageView  *attachment_views = calloc(attachment_count, sizeof(VkImageView));
        char        **attachment_names = calloc(attachment_count, sizeof(char *));

        // Error check
        if ( attachment_views == (void *) 0 ) goto no_mem;

        // Get each attachment's name
        dict_keys(p_render_pass->attachments, attachment_names);

        // Iterate over each attachment
        //for (size_t i = 0; i < attachment_count; i++)
        //    {
        //        GXAttachment_t *p_attachment = dict_get(p_instance->context.loading_renderer->attachments, attachment_names[i]);
        //        if ( strcmp(p_attachment->name, "depth") != 0 )
        //            update_image_layout(p_attachment->p_image, "unorm b8g8r8a8", "color attachment");
        //        else
        //            update_image_layout(p_attachment->p_image, "unorm d24 uint s8", "depth stencil attachment");
        //    }

        attachment_views[0] = p_instance->vulkan.swap_chain.image_views[i];
        attachment_views[1] = ((GXAttachment_t*)dict_get(p_instance->context.loading_renderer->attachments, "depth"))->image_view;

        // TODO: allocate attachments
        // TODO: iterate over this render passes attachments
        // TODO: Get each image view, store in attachments

        VkFramebufferCreateInfo framebuffer_create_info =
        {
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass      = p_render_pass->render_pass,
            .attachmentCount = attachment_count,
            .pAttachments    = attachment_views,
            .width           = p_instance->vulkan.swap_chain.extent.width,
            .height          = p_instance->vulkan.swap_chain.extent.height,
            .layers          = 1
        };

        // Create the frame buffer
        if (
            vkCreateFramebuffer(
                p_instance->vulkan.device,
                &framebuffer_create_info,
                0,
                &p_render_pass->framebuffers[i]
            ) != VK_SUCCESS
        )
            goto failed_to_create_render_pass;
    }

    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    wrong_attachments_type:
    wrong_subpasses_type:
    wrong_dependencies_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Can not parse render pass. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Not enough properties to construct remder pass in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_dependency_source_subpass_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"\" must be of type [ string | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_dependency_destination_subpass_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"\" must be of type [ string | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_dependency_source_stage_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"\" must be of type [ string | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_dependency_destination_stage_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"\" must be of type [ string | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_dependency_source_access_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"\" must be of type [ string | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_dependency_destination_access_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"\" must be of type [ string | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_subpass_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load subpass from JSON value in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_load_shader_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load shader as JSON value in call to function \"%s\"\n", __FUNCTION__);
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

        // Vulkan errors
        {

            failed_to_create_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create render pass in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_subpass ( GXSubpass_t **pp_subpass, char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_subpass == (void *) 0 ) goto no_subpass;
        if ( path       == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t len = g_load_file(path, 0, true);
    char *buffer = calloc(1 + len, sizeof(char));

    // Error check
    if ( buffer == (void *) 0 ) goto no_mem;

    // Load the file
    if ( g_load_file(path, buffer, true) == 0 ) goto failed_to_read_file;

    // Construct a subpass
    if ( load_subpass_as_json_text(pp_subpass, buffer) == 0 ) goto failed_to_load_subpass;

    // Clean the scope
    free(buffer);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load subpass from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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

int load_subpass_as_json_text ( GXSubpass_t **pp_subpass, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_subpass == (void *) 0 ) goto no_subpass;
        if ( text       == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXSubpass_t  *p_render_pass = 0;
    json_value  *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json_as_value;

    // Load the renderer as a JSON value
    if ( load_subpass_as_json_value(pp_subpass, p_value) == 0 ) goto failed_to_load_subpass_as_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_load_subpass_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load subpass in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            failed_to_parse_json_as_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to parse JSON text in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_subpass_as_json_value ( GXSubpass_t **pp_subpass, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_subpass == (void *) 0 ) goto no_subpass;
        if ( p_value    == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance              = g_get_active_instance();
    GXSubpass_t  *p_subpass               = 0;
    json_value  *p_name                  = 0,
                 *p_shaders               = 0,
                 *p_shader_name           = 0,
                 *p_input_attachments     = 0,
                 *p_color_attachments     = 0,
                 *p_preserved_attachments = 0,
                 *p_depth_attachment      = 0;

    // Parse the JSON value into constructor parameters
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name                  = dict_get(p_dict, "name");

        // Optional properties
        p_shaders               = dict_get(p_dict, "shaders");
        p_input_attachments     = dict_get(p_dict, "input attachments");
        p_color_attachments     = dict_get(p_dict, "color attachments");
        p_preserved_attachments = dict_get(p_dict, "preserved attachments");
        p_depth_attachment      = dict_get(p_dict, "depth attachment");

        // Error check
        if ( ! ( p_name ) ) goto missing_properties;
    }

    // Construct the subpass
    {

        // Initialized data
        size_t                   input_attachment_count          = 0,
                                 color_attachment_count          = 0,
                                 preserved_attachment_count      = 0;
        char                    *name                            = 0;
        VkAttachmentReference2  *input_attachment_references     = 0,
                                *color_attachment_references     = 0,
                                *preserved_attachment_references = 0,
                                *depth_attachment_reference      = 0;
        json_value            **pp_input_attachments            = 0,
                               **pp_color_attachments            = 0,
                               **pp_preserved_attachments        = 0;

        // Parse the JSON value
        {

            // Parse input attachments
            if ( p_input_attachments )
            {

                // Parse the input attachments as an array
                if ( p_input_attachments->type == JSON_VALUE_ARRAY )
                {

                    // Get the quantity of input attachments
                    array_get(p_input_attachments->list, 0, &input_attachment_count);

                    // Allocate memory for input attachment names
                    pp_input_attachments = calloc(input_attachment_count, sizeof(json_value *));

                    // Error check
                    if ( pp_input_attachments == (void *) 0 ) goto no_mem;

                    // Get the contents of the array
                    array_get(p_input_attachments->list, (void **)pp_input_attachments, 0);
                }
                // Type error
                else
                    goto wrong_input_attachments_type;
            }

            // Parse color attachments
            if ( p_color_attachments )
            {

                // Parse the color attachments as an array
                if ( p_color_attachments->type == JSON_VALUE_ARRAY )
                {

                    // Get the size of the array
                    array_get(p_color_attachments->list, 0, &color_attachment_count);

                    // Allocate memory for array
                    pp_color_attachments = calloc(color_attachment_count, sizeof(json_value *));

                    // Error check
                    if ( pp_color_attachments == (void *) 0 ) goto no_mem;

                    // Dump the array contents
                    array_get(p_color_attachments->list, (void **)pp_color_attachments, 0);
                }
                // Type error
                else
                    goto wrong_color_attachments_type;
            }

            // Parse preserved attachments
            if ( p_preserved_attachments )
            {

                // Parse the preserved attachments as an array
                if (p_preserved_attachments->type == JSON_VALUE_ARRAY)
                {
                    array_get(p_preserved_attachments->list, 0, &preserved_attachment_count);
                    pp_preserved_attachments = calloc(preserved_attachment_count, sizeof(json_value *));

                    if ( pp_preserved_attachments == (void *) 0 ) goto no_mem;

                    array_get(p_preserved_attachments->list, (void **)pp_preserved_attachments, 0);
                }
                // Type error
                else
                    goto wrong_preserved_attachments_type;
            }

            if ( p_depth_attachment )
            {
                if (p_depth_attachment->type == JSON_VALUE_STRING)
                    ;
                else
                    goto wrong_depth_attachments_type;
            }
        }

        // Create the subpass
        {

            // Allocate memory for a subpass
            if ( create_subpass(&p_subpass) == 0 ) goto failed_to_allocate_subpass;

            // Copy the name of the subpass
            if ( p_name->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t len = strlen(p_name->string);

                // Allocate memory for name
                name = calloc(len + 1, sizeof(char));

                // Error check
                if ( name == (void *) 0 ) goto no_mem;

                // Copy the string
                strncpy(name, p_name->string, len);
            }
            // Default
            else
                goto wrong_name_type;

            // Parse input attachments
            if ( pp_input_attachments )
            {
                input_attachment_references = calloc(input_attachment_count, sizeof(VkAttachmentReference));

                // Error check
                if ( input_attachment_references == (void *) 0 ) goto no_mem;
                if ( input_attachment_references == (void *) 0 ) goto no_mem;

                // Iterate over each attachment name
                for (size_t i = 0; i < input_attachment_count; i++)
                {

                    // Initialized data
                    json_value *p_input_attachment = pp_color_attachments[i];

                    if (p_input_attachment->type == JSON_VALUE_STRING)
                    {
                        GXAttachment_t *p_attachment = dict_get(p_instance->context.loading_renderer->attachments, p_input_attachment->string);

                        input_attachment_references[i] = (VkAttachmentReference2)
                        {
                            .sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                            .pNext      = 0,
                            .attachment = (u32)i,
                            // TODO: Set the image layout
                            .layout = p_attachment->attachment_description2.initialLayout,
                            .aspectMask = 0
                        };
                    }
                }
            }

            // Parse color attachments
            if ( pp_color_attachments )
            {
                color_attachment_references = calloc(color_attachment_count, sizeof(VkAttachmentReference));

                // Error check
                if ( color_attachment_references == (void *) 0 ) goto no_mem;

                // Iterate over each attachment name
                for (size_t i = 0; i < color_attachment_count; i++)
                {

                    // Initialized data
                    json_value *p_color_attachment = pp_color_attachments[i];

                    if (p_color_attachment->type == JSON_VALUE_STRING)
                    {
                        GXAttachment_t *p_attachment = dict_get(p_instance->context.loading_renderer->attachments, p_color_attachment->string);

                        color_attachment_references[i] = (VkAttachmentReference2)
                        {
                            .sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                            .pNext      = 0,
                            .attachment = (u32)i,
                            // TODO: Set the image layout
                            .layout = p_attachment->attachment_description2.initialLayout,
                            .aspectMask = 0
                        };
                    }
                }
            }

            // Parse preserved attachments
            if ( pp_preserved_attachments )
            {

                preserved_attachment_references = calloc(preserved_attachment_count, sizeof(VkAttachmentReference));

                // Error check
                if ( preserved_attachment_references == (void *) 0 ) goto no_mem;

                // Iterate over each attachment name
                for (size_t i = 0; i < preserved_attachment_count; i++)
                {

                    // Initialized data
                    json_value *p_preserved_attachment = pp_preserved_attachments[i];

                    if (p_preserved_attachment->type == JSON_VALUE_STRING)
                    {
                        GXAttachment_t *p_attachment = dict_get(p_instance->context.loading_renderer->attachments, p_preserved_attachment->string);

                        preserved_attachment_references[i] = (VkAttachmentReference2)
                        {
                            .sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                            .pNext      = 0,
                            .attachment = (u32)i,
                            // TODO: Set the image layout
                            .layout = p_attachment->attachment_description2.initialLayout,
                            .aspectMask = 0
                        };
                    }
                }
            }

            // Parse depth attachments
            if ( p_depth_attachment )
            {
                depth_attachment_reference = calloc(1, sizeof(VkAttachmentReference));

                // Error check
                if ( depth_attachment_reference == (void *) 0 ) goto no_mem;

                *depth_attachment_reference = (VkAttachmentReference2)
                {
                    .sType      = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                    .pNext      = 0,
                    .attachment = 1,
                    .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    .aspectMask = 0
                };
            }
        }

        // TODO: Finish attachments
        *p_subpass = (GXSubpass_t)
        {
            .name                = name,
            .subpass_description =
            (VkSubpassDescription2)
            {
                .sType                   = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2,
                .pNext                   = 0,
                .flags                   = 0,
                .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
                .viewMask                = 0,
                .inputAttachmentCount    = (u32)input_attachment_count,
                .pInputAttachments       = input_attachment_references,
                .colorAttachmentCount    = (u32)color_attachment_count,
                .pColorAttachments       = color_attachment_references,
                .pResolveAttachments     = 0,
                .pDepthStencilAttachment = depth_attachment_reference,
                .preserveAttachmentCount = (u32)preserved_attachment_count,
                .pPreserveAttachments    = 0
            }
        };
    }

    // Return a pointer to the caller
    *pp_subpass = p_subpass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Not enough properties to construct subpass in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_input_attachments_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"input attachments\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_color_attachments_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"color attachments\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_preserved_attachments_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"preserved attachments\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_depth_attachments_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"depth attachment\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_allocate_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to allocate subpass in call to function \"%s\"\n", __FUNCTION__);
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

int load_attachment_as_json_value ( GXAttachment_t **pp_attachment, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_attachment == (void *) 0 ) goto no_attachment;
        if ( p_value       == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t   *p_instance        = g_get_active_instance();
    GXAttachment_t *p_attachment      = 0;
    json_value    *p_name            = 0,
                   *p_samples         = 0,
                   *p_format          = 0,
                   *p_load_operation  = 0,
                   *p_store_operation = 0,
                   *p_initial_layout  = 0,
                   *p_final_layout    = 0;
    bool            is_final          = false;

    // Parse the attachment JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name            = dict_get(p_dict, "name");
        p_samples         = dict_get(p_dict, "samples");
        p_format          = dict_get(p_dict, "format");
        p_initial_layout  = dict_get(p_dict, "initial layout");
        p_final_layout    = dict_get(p_dict, "final layout");

        // Optional properties
        p_load_operation  = dict_get(p_dict, "load operation");  // Default to VK_ATTACHMENT_LOAD_OP_DONT_CARE
        p_store_operation = dict_get(p_dict, "store operation"); // Default to VK_ATTACHMENT_STORE_OP_DONT_CARE

        // Check for missing parameters
        if ( ! (
            p_name           &&
            p_samples        &&
            p_format         &&
            p_initial_layout &&
            p_final_layout
        ) )
            goto missing_properties;
    }
    else
        goto wrong_type;

    // Construct the attachment
    {

        // Initialized data
        VkFormat                 format                  = 0;
        VkSampleCountFlagBits    samples                 = 0;
        VkAttachmentLoadOp       loadOp                  = 0;
        VkAttachmentStoreOp      storeOp                 = 0;
        VkAttachmentLoadOp       stencilLoadOp           = 0;
        VkAttachmentStoreOp      stencilStoreOp          = 0;
        VkImageLayout            initialLayout           = 0;
        VkImageLayout            finalLayout             = 0;
        VkAttachmentDescription2 attachment_description2 = { 0 };

        // Allocate memory for an attachment
        if ( create_attachment(&p_attachment) == 0 ) goto failed_to_create_attachment;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for name
            p_attachment->name = calloc(len + 1, sizeof(char));

            // Error check
            if ( p_attachment->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_attachment->name, p_name->string, len);

            if ( strcmp(p_attachment->name, "final") == 0 )
                is_final = true;

        }
        // Default
        else
            goto wrong_name_type;

        // Set the format
        if ( p_format->type == JSON_VALUE_STRING )
        {
            format = (VkFormat) (size_t) dict_get(format_enumeration_lookup, p_format->string);

            if ( format == 0 ) goto wrong_format;
        }
        // Default
        else
            goto wrong_format_type;

        // Set the sample count
        if ( p_samples->type == JSON_VALUE_INTEGER )
            samples = p_samples->integer;
        // Default
        else
            goto wrong_name_type;

        // Set the load operation
        if ( p_load_operation->type == JSON_VALUE_STRING )
            loadOp = (VkAttachmentLoadOp) (size_t) dict_get(attachment_load_operations, p_load_operation->string);
        // Default
        else
            goto wrong_load_operation_type;

        // Set the store operation
        if ( p_store_operation->type == JSON_VALUE_STRING )
            storeOp = (VkAttachmentStoreOp) (size_t) dict_get(attachment_store_operations, p_store_operation->string);
        // Default
        else
            goto wrong_store_operation_type;

        // (Maybe) set the stencil load operation

        // (Maybe) set the stencil store operation

        // Set the initial layout
        if ( p_initial_layout->type == JSON_VALUE_STRING )
        {
            initialLayout = (VkImageLayout) (size_t) dict_get(image_layouts, p_initial_layout->string);
        }
        // Default
        else
            goto wrong_initial_layout_type;

        // Set the final layout
        if ( p_final_layout->type == JSON_VALUE_STRING )
        {
            finalLayout = (VkImageLayout) (size_t) dict_get(image_layouts, p_final_layout->string);
        }
        // Default
        else
            goto wrong_final_layout_type;

        // Populate the attachment description
        p_attachment->attachment_description2 = (VkAttachmentDescription2)
        {
            .sType          = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2,
            .pNext          = 0,
            .flags          = 0,
            .format         = format,
            .samples        = samples,
            .loadOp         = loadOp,
            .storeOp        = storeOp,
            .stencilLoadOp  = stencilLoadOp,
            .stencilStoreOp = stencilStoreOp,
            .initialLayout  = initialLayout,
            .finalLayout    = finalLayout
        };

        // Create an image
        {

            // Initialized data
            GXImage_t          *p_image      = 0;
            VkImageAspectFlags  aspect_flags = 0;
            VkImageUsageFlags   usage_flags  = 0;

            if (
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL ||
                p_attachment->attachment_description2.finalLayout == VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL
            )
            {
                aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT;
                usage_flags = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

            }
            else
            {
                aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
                usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            }

            // Allocate memory for an image
            if ( create_image(&p_image) == 0 ) goto failed_to_create_image;

            // Construct the image
            (void)construct_image(
                p_image, 0,
                VK_IMAGE_TYPE_2D, p_attachment->attachment_description2.format,
                1280,720,
                1,1,
                1,1,
                VK_IMAGE_TILING_OPTIMAL, usage_flags,
                VK_SHARING_MODE_EXCLUSIVE, p_attachment->attachment_description2.finalLayout );

            p_image->name = p_attachment->name;

            // Set the attachment image
            p_attachment->p_image = p_image;

            if ( is_final == false )
            {

                // Construct the image view
                construct_image_view(
                    &p_attachment->image_view,
                    p_image,
                    VK_IMAGE_VIEW_TYPE_2D,
                    p_attachment->attachment_description2.format,
                    (VkComponentMapping)
                    {
                        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                        .a = VK_COMPONENT_SWIZZLE_IDENTITY
                    },
                    aspect_flags
                );
            }
        }

    }

    // Return a pointer to the caller
    *pp_attachment = p_attachment;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_attachment:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10
        {
            failed_to_create_attachment:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to allocate memory for attachment in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create image in call to function \"%s\"\n", __FUNCTION__);
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
            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Attachment JSON value is of wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Missing JSON parameters in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_format:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Wrong \"format\" VkFormat value in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_format_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"format\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_load_operation_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"load operation\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_store_operation_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"store operation\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_initial_layout_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"initial layout\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_final_layout_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"final layout\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int construct_image ( GXImage_t *p_image, VkImageCreateFlags flags, VkImageType image_type, VkFormat format, int width, int height, int depth, size_t mip_levels, size_t array_layers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharing_mode, VkImageLayout initial_layout )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_image == (void *) 0 ) goto no_image;
    #endif

    // External functions
    extern u32 find_memory_type ( u32 type_filter, VkMemoryPropertyFlags properties );

    // Initialized data
    GXInstance_t         *instance            = g_get_active_instance();
    size_t                dim                 = 0;
    VkMemoryAllocateInfo  allocate_info       = { 0 };
    VkMemoryRequirements  memory_requirements = { 0 };
    VkImageCreateInfo     image_create_info   =
    {
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .flags         = 0,
        .imageType     = image_type,
        .format        = format,
        .extent.width  = (u32)width,
        .extent.height = (u32)height,
        .extent.depth  = (u32)depth,
        .mipLevels     = (u32)mip_levels,
        .arrayLayers   = (u32)array_layers,
        .tiling        = tiling,
        .usage         = usage,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .samples       = samples
    };

    // Set the dimension
    if ( height > 1 ) dim++;
    if ( depth  > 1 ) dim++;

    // Create the image
    if (
        vkCreateImage(
            instance->vulkan.device,
            &image_create_info,
            0,
            &p_image->image
        ) != VK_SUCCESS
    )
        goto failed_to_create_image;

    // Figure out how much memory the image will use
    vkGetImageMemoryRequirements(instance->vulkan.device, p_image->image, &memory_requirements);

    // Popultate the allocate info struct
    allocate_info = (VkMemoryAllocateInfo)
    {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize  = memory_requirements.size,
        .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };

    // Allocate memory for the image
    if ( 
        vkAllocateMemory(
            instance->vulkan.device,
            &allocate_info,
            0,
            &p_image->image_memory
        ) != VK_SUCCESS
    )
        goto failed_to_allocate_image_memory;

    // Bind the image to the image memory
    if (
        vkBindImageMemory(
            instance->vulkan.device,
            p_image->image,
            p_image->image_memory,
            0
        ) != VK_SUCCESS
    )
        goto failed_to_bind_image_memory;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_image\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to create image in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_image_memory:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to vkAllocateMemory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_bind_image_memory:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to vkBindImageMemory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int construct_image_view ( VkImageView *ret, GXImage_t *p_image, VkImageViewType view_type, VkFormat format, VkComponentMapping swizzle, VkImageAspectFlags aspect_mask )
{

    // Argument check
    #ifndef NDEBUG
        if ( ret     == (void *) 0 ) goto no_ret;
        if ( p_image == (void *) 0 ) goto no_image;
    #endif

    // Initialized data
    GXInstance_t          *instance               = g_get_active_instance();
    VkImageViewCreateInfo  image_view_create_info =
    {

        .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image                           = p_image->image,
        .viewType                        = view_type,
        .format                          = format,
        .subresourceRange.aspectMask     = aspect_mask,
        .subresourceRange.baseMipLevel   = 0,
        .subresourceRange.levelCount     = 1,
        .subresourceRange.baseArrayLayer = 0,
        .subresourceRange.layerCount     = 1
    };

    // Create an image view
    if ( 
        vkCreateImageView(
            instance->vulkan.device,
            &image_view_create_info,
            0,
            ret
        ) != VK_SUCCESS
    )
        goto failed_to_create_image_view;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"ret\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;

            no_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_image\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_image_view:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create image view in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_image_as_json_value ( GXImage_t **pp_image, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_image == (void *) 0 ) goto no_image;
        if ( p_value  == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance        = g_get_active_instance();
    GXImage_t    *p_image           = 0;
    json_value  *p_name            = 0,
                 *p_samples         = 0,
                 *p_format          = 0,
                 *p_load_operation  = 0,
                 *p_store_operation = 0,
                 *p_initial_layout  = 0,
                 *p_final_layout    = 0;

    // Parse the image JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialzed data
        dict *p_dict = p_value->object;

        // Required parameters
        p_name            = dict_get(p_dict, "name");
        p_samples         = dict_get(p_dict, "samples");
        p_format          = dict_get(p_dict, "format");
        p_initial_layout  = dict_get(p_dict, "initial layout");
        p_final_layout    = dict_get(p_dict, "final layout");

        // Optional parameters
        p_load_operation  = dict_get(p_dict, "load operation");  // Default to VK_ATTACHMENT_LOAD_OP_DONT_CARE
        p_store_operation = dict_get(p_dict, "store operation"); // Default to VK_ATTACHMENT_STORE_OP_DONT_CARE

        // Check for missing parameters
        if ( ! (
            p_name           &&
            p_samples        &&
            p_format         &&
            p_initial_layout &&
            p_final_layout
        ) )
            goto missing_properties;
    }
    // Default
    else
        goto wrong_type;

    // Construct the image
    {

        // Initialized data
        VkFormat                 format                  = 0;
        VkSampleCountFlagBits    samples                 = 0;
        VkAttachmentLoadOp       loadOp                  = 0;
        VkAttachmentStoreOp      storeOp                 = 0;
        VkAttachmentLoadOp       stencilLoadOp           = 0;
        VkAttachmentStoreOp      stencilStoreOp          = 0;
        VkImageLayout            initialLayout           = 0;
        VkImageLayout            finalLayout             = 0;
        VkAttachmentDescription2 attachment_description2 = { 0 };

        // Allocate memory for an image
        if ( create_image(&p_image) == 0 ) goto failed_to_create_image;

        // Set the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for name
            p_image->name = calloc(len + 1, sizeof(char));

            // Error check
            if ( p_image->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_image->name, p_name->string, len);
        }
        // Default
        else
            goto wrong_name_type;

        // Set the format
        if ( p_format->type == JSON_VALUE_STRING )
        {
            format = (VkFormat) (size_t) dict_get(format_enumeration_lookup, p_format->string);

            if ( format == 0 ) goto wrong_format;
        }
        // Default
        else
            goto wrong_format_type;

        // Set the sample count
        if ( p_samples->type == JSON_VALUE_INTEGER )
            samples = p_samples->integer;
        // Default
        else
            goto wrong_samples_type;

        // Set the load operation
        if ( p_load_operation->type == JSON_VALUE_STRING )
            loadOp = (VkAttachmentLoadOp) (size_t) dict_get(attachment_load_operations, p_load_operation->string);
        // Default
        else
            goto wrong_load_operation_type;

        // Set the store operation
        if ( p_store_operation->type == JSON_VALUE_STRING )
            storeOp = (VkAttachmentStoreOp) (size_t) dict_get(attachment_store_operations, p_store_operation->string);
        // Default
        else
            goto wrong_store_operation_type;

        // (Maybe) set the stencil load operation

        // (Maybe) set the stencil store operation

        // Set the initial layout
        if ( p_initial_layout->type == JSON_VALUE_STRING )
        {
            initialLayout = (VkImageLayout) (size_t) dict_get(image_layouts, p_initial_layout->string);
        }
        // Default
        else
            goto wrong_initial_layout_type;

        // Set the final layout
        if ( p_final_layout->type == JSON_VALUE_STRING )
        {
            finalLayout = (VkImageLayout) (size_t) dict_get(image_layouts, p_final_layout->string);
        }
        // Default
        else
            goto wrong_final_layout_type;

        // Populate the attachment description
        /*
        (VkImageCreateInfo)
        {
            .sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext                 = 0,
            .flags                 = 0,
            .imageType             = 0,
            .format                = 0,
            .extent                = 0,
            .mipLevels             = 0,
            .arrayLayers           = 0,
            .samples               = 0,
            .tiling                = 0,
            .usage                 = 0,
            .sharingMode           = 0,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices   = 0,
            .initialLayout         = 0
        };
        */
        /**/
        // Create a texture
        {
            /*
            // Initialized data
            GXTexture_t *p_texture = 0;

            // Allocate memory for a texture
            if ( create_texture(&p_texture) == 0 )
                goto failed_to_allocate_texture;

            if ( strcmp(p_attachment->name, "final") == 0 )
            {
                p_texture->texture_image = p_instance->vulkan.swap_chain_images[i];
                construct_image_view(p_texture, VK_IMAGE_VIEW_TYPE_2D, p_instance->vulkan.swap_chain_image_format, (VkComponentMapping){VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY}, VK_IMAGE_ASPECT_COLOR_BIT);
            }
            else
            {
                construct_image(
                    p_texture,
                    0,
                    VK_IMAGE_TYPE_2D,
                    VK_FORMAT_D32_SFLOAT,
                    p_instance->vulkan.swap_chain_extent.width,
                    p_instance->vulkan.swap_chain_extent.height,
                    1,
                    1,
                    1,
                    1,
                    VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                    VK_SHARING_MODE_EXCLUSIVE,
                    VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL
                );
                construct_image_view(
                    p_texture,
                    VK_IMAGE_VIEW_TYPE_2D,
                    VK_FORMAT_D32_SFLOAT,
                    (VkComponentMapping)
                    {
                        VK_COMPONENT_SWIZZLE_IDENTITY,
                        VK_COMPONENT_SWIZZLE_IDENTITY,
                        VK_COMPONENT_SWIZZLE_IDENTITY,
                        VK_COMPONENT_SWIZZLE_IDENTITY
                    },
                    VK_IMAGE_ASPECT_DEPTH_BIT
                );
            }


            p_render_pass->image_attachments[i] = p_texture->texture_image_view;

            if ( i == 0 )
                p_render_pass->image_attachments[i] = p_instance->vulkan.swap_chain_image_views[p_instance->vulkan.image_index];

        */
        }

    }

    // Return a pointer to the caller
    *pp_image = p_image;

    // Success
    return 1;

    missing_properties:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
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
            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_samples_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"samples\" must be of type [ integer ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_format:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"format\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_format_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"format\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_load_operation_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"load operation\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_store_operation_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"store operation\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_initial_layout_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"initial layout\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_final_layout_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Property \"final layout\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_create_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to allocate memory for image in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/renderer.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int update_image_layout ( GXImage_t *p_image, char *format, char *new_layout )
{

    // TODO: Argument check

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    VkCommandBuffer command_buffer = 0;
    VkImageMemoryBarrier2 memory_barrier2 =
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = 0,
        .srcStageMask = 0,
        .srcAccessMask = 0,
        .dstStageMask = 0,
        .dstAccessMask = 0,
        .oldLayout = p_image->image_layout,
        .newLayout = (VkImageLayout) (u32) (dict_get(image_layouts, new_layout)),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = p_image->image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };
    VkDependencyInfo dependency_info =
    {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = 0,
        .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT ,
        .memoryBarrierCount = 0,
        .pMemoryBarriers = 0,
        .bufferMemoryBarrierCount = 0,
        .pBufferMemoryBarriers = 0,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &memory_barrier2,
    };

    VkPipelineStageFlags source_stage_mask = 0,
                         destination_stage_mask = 0;
    VkDependencyFlags dependency_flags = 0;

    VkCommandBufferAllocateInfo alloc_info =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = 0,
        .commandPool = p_instance->vulkan.command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    VkCommandBufferBeginInfo begin_info =
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = 0, // Zero or pointer to VkDeviceGroupCommandBufferBeginInfo struct
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = 0
    };
    VkCommandBufferSubmitInfo submit_info = { 0 };

    vkAllocateCommandBuffers(p_instance->vulkan.device, &alloc_info, &command_buffer);

    vkBeginCommandBuffer(command_buffer, &begin_info);

    vkCmdPipelineBarrier2(command_buffer, &dependency_info);

    vkEndCommandBuffer(command_buffer);

    submit_info = (VkCommandBufferSubmitInfo)
    {
        .sType    = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext    = 0,
        .commandBuffer = command_buffer,
        .deviceMask = 0
    };

    vkQueueSubmit(p_instance->vulkan.queues.graphics_queues[2], 1, &submit_info, VK_NULL_HANDLE);

    vkQueueWaitIdle(p_instance->vulkan.queues.graphics_queues[2]);

    vkFreeCommandBuffers(p_instance->vulkan.device, p_instance->vulkan.command_pool, 1, &command_buffer);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

        }
    }
}

int print_renderer ( GXRenderer_t *p_renderer )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_renderer == (void *) 0 ) goto no_renderer;
    #endif

    // Formatting
    g_print_log(" - Renderer info - \n");

    // Print the name
    g_print_log("name            : \"%s\"\n", p_renderer->name);

    // Formatting
    g_print_log("attachments     :\n");

    // TODO: FIX
    // Print each attachment
    // dict_foreach(p_renderer->attachments, print_attachment);
    putchar('\n');

    // Iterate over each render pass
    for ( size_t i = 0; i < p_renderer->render_pass_count; i++ )
        print_render_pass(p_renderer->render_passes_data[i]);

    // Formatting
    putchar('\n');
    
    // Formatting
    g_print_log(" - Synchronization info - \n");
    
    // Print synchronization objects
    {
        
        // Print binary semaphores
        size_t   binary_semaphore_count = dict_keys(p_renderer->synchronization.binary_semaphores, 0),
                 fences_count           = dict_keys(p_renderer->synchronization.fences, 0);
        char   **binary_semaphore_names = calloc(binary_semaphore_count, sizeof(char *)),
               **fences_names           = calloc(fences_count, sizeof(char *));
            
        // Error check
        if ( binary_semaphore_names == (void *) 0 ) goto no_mem;
        
        // Error check
        if ( fences_names == (void *) 0 ) goto no_mem;
        
        // Get the name of each semaphore
        dict_keys(p_renderer->synchronization.binary_semaphores, binary_semaphore_names);

        // Get the name of each fence
        dict_keys(p_renderer->synchronization.fences, fences_names);

        // Formatting
        g_print_log("binary semaphores:\n");

        // Print binary semaphores
        for (size_t i = 0; i < binary_semaphore_count; i++)
            g_print_log("\t\"%s\"\n", binary_semaphore_names[i]);

        // Formatting
        g_print_log("fences:\n");

        // Print binary semaphores
        for (size_t i = 0; i < fences_count; i++)
            g_print_log("\t\"%s\"\n", fences_names[i]);
    }

    // Success
    return 1;

    no_mem:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void println ( char *str )
{
    printf("\t\"%s\"\n", str);

    return;
}

int print_render_pass ( GXRenderPass_t *p_render_pass )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_render_pass == (void *) 0 ) goto no_render_pass;
    #endif

    // Formatting
    g_print_log(" - Render pass info - \n");

    // Print the name
    g_print_log("name           : \"%s\"\n", p_render_pass->name);

    // Formatting
    g_print_log("attachments[%d]: \n", p_render_pass->attachments_count);

    // TODO: Fix
    // Print the names of the attachments
    //dict_foreach(p_render_pass->attachments, println);

    // Formatting
    g_print_log("subpasses[%d]  : \n", p_render_pass->attachments_count);
    
    // TODO: Fix
    //dict_foreach(p_render_pass->subpasses, print_subpass);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int print_subpass ( GXSubpass_t *p_subpass )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_subpass == (void *) 0 ) goto no_subpass;
    #endif

    // Formatting
    g_print_log(" - Subpass info - \n");

    // Print the name
    g_print_log("name: \"%s\"\n", p_subpass->name);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int print_image ( GXImage_t *p_image )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_image == (void *) 0 ) goto no_image;
    #endif

    // Formatting
    g_print_log(" - Image info - \n");

    // Print the name
    g_print_log("name : \"%s\"\n", p_image->name);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_image\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int print_attachment ( GXAttachment_t *p_attachment )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_attachment == (void *) 0 ) goto no_attachment;
    #endif

    // Formatting
    g_print_log(" - Attachment info - \n");

    // Print the name
    g_print_log("name            : \"%s\"\n", p_attachment->name);

    // Print the attachment description
    if ( p_attachment->attachment_description2.format > FORMAT_ENUM_COUNT )
        g_print_log("format          : \"%s\"\n", format_names[p_attachment->attachment_description2.format]);

    // Print the sanple count
    g_print_log("samples         : %d\n", p_attachment->attachment_description2.samples);

    // Print the load operation
    g_print_log("load operation  : \"%s\"\n", attachment_load_operation_names[p_attachment->attachment_description2.loadOp]);

    // Print the store operation
    g_print_log("store operation : \"");
    if (p_attachment->attachment_description2.storeOp == VK_ATTACHMENT_STORE_OP_NONE)
        g_print_log("none\"\n");
    else
        g_print_log("%s\"\n", attachment_store_operation_names[p_attachment->attachment_description2.storeOp]);

    // Print the initial layout
    g_print_log("initial layout  : \"");
    if (p_attachment->attachment_description2.initialLayout > 8)
        g_print_log("none\"\n");
    else
        g_print_log("%s\"\n", image_layout_names[p_attachment->attachment_description2.initialLayout]);

    g_print_log("final layout    : \"");
    if (p_attachment->attachment_description2.finalLayout > 8)
        g_print_log("none\"\n");
    else
        g_print_log("%s\"\n", image_layout_names[p_attachment->attachment_description2.finalLayout]);

    putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
        no_attachment:
            #ifndef NDEBUG
                g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_attachment\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
        }
    }
}

int add_subpass_callback ( char *name, void (*function_pointer)() )
{

    // Argument check
    #ifndef NDEBUG
        if ( name             == (void *) 0 ) goto no_name;
        if ( function_pointer == (void *) 0 ) goto no_function_pointer;
    #endif

    // TODO:
    //

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Renderer] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
        no_function_pointer:
            #ifndef NDEBUG
                g_print_error("[G10] [Renderer] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
        }
    }
}

int render_frame ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance                   == (void *) 0 ) goto no_instance;
        if ( p_instance->context.renderer == (void *) 0 ) goto no_instance_context_renderer;
    #endif

    // Initialized data
    u64              start                     = 0,
                     end                       = 0;
    size_t           current_frame             = p_instance->vulkan.current_frame;
    VkResult         result                    = 0;
    GXRenderer_t    *p_renderer                = p_instance->context.renderer;
    VkCommandBuffer  command_buffer            = p_renderer->command_buffers[current_frame];
    VkSemaphore image_available_semaphore = ( (VkSemaphore *)dict_get(p_renderer->synchronization.binary_semaphores, "IMAGE AVAILABLE") )[current_frame],
                render_finished_semaphore = ( (VkSemaphore *)dict_get(p_renderer->synchronization.binary_semaphores, "RENDER FINISHED") )[current_frame];
    VkFence     in_flight_fence           = ( (VkFence     *)dict_get(p_renderer->synchronization.fences           , "IN FLIGHT"      ) )[current_frame];
    VkSemaphore wait_semaphores [ ] = 
    {
        image_available_semaphore
    },
                signal_semaphores [ ] =
    {
        render_finished_semaphore
    };
    VkPipelineStageFlags wait_stages [ ] =
    {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    VkSwapchainKHR swap_chains [ ] =
    {
        p_instance->vulkan.swap_chain.swap_chain
    };

    // Get the microsecond counter
    start = SDL_GetPerformanceCounter();

    // Prepare the command buffer for rendering
    {

        // Wait for the previous frame to finish rendering
        vkWaitForFences(p_instance->vulkan.device, 1, &in_flight_fence, VK_TRUE, 0xffffffffffffffff);

        // Grab an image from the swapchain
        result = vkAcquireNextImageKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, 0xffffffffffffffff, image_available_semaphore, VK_NULL_HANDLE, &p_instance->vulkan.image_index);

        // Make sure the image is usable
        if ( result == VK_ERROR_OUT_OF_DATE_KHR )
        {
            g_window_resize(p_instance);
            goto fail;
        }
        else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
        {
            printf("Failed to acquire swap chain image!\n");
            goto fail;
        }

        // Only reset the fence if the program is submitting work
        vkResetFences(p_instance->vulkan.device, 1, &in_flight_fence);

        // Clear out the command buffer
        vkResetCommandBuffer(command_buffer, 0);
    }

    // Draw the frame
    {

        // Initialized data
        GXRenderer_t *active_renderer = p_instance->context.renderer;
        VkCommandBufferBeginInfo begin_info =
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
        };

        // Begin the command buffer
        vkBeginCommandBuffer(command_buffer, &begin_info);

        // Iterate over each render pass
        for (size_t i = 0; i < active_renderer->render_pass_count; i++)
        {

            // Initialized data
            GXRenderPass_t         *p_render_pass          = active_renderer->render_passes_data[i];
            size_t                  subpass_count          = p_render_pass->subpasses_count;
            dict                   *draw_item_queues       = p_render_pass->draw_queue_types;
            VkRenderPassBeginInfo   render_pass_begin_info =
            {
                .sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass          = p_render_pass->render_pass,
                .framebuffer         = p_render_pass->framebuffers[current_frame],
                .renderArea.offset.x = 0,
                .renderArea.offset.y = 0,
                .renderArea.extent   = p_instance->vulkan.swap_chain.extent,
                .clearValueCount     = (u32)active_renderer->current_render_pass->attachments_count,
                .pClearValues        = active_renderer->clear_colors
            };
            
            // Start the render pass
            vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

            // Iterate over each subpass
            for (size_t j = 0; j < p_render_pass->subpasses_count; j++)
            {

                // Initialized data
                GXSubpass_t *p_subpass = p_render_pass->subpasses_data[i];
                VkSubpassBeginInfo subpass_begin_info =
                {
                    .sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO,
                    .pNext = 0,
                    .contents = VK_SUBPASS_CONTENTS_INLINE
                };
                VkSubpassEndInfo subpass_end_info =
                {
                    .sType = VK_STRUCTURE_TYPE_SUBPASS_END_INFO,
                    .pNext = 0
                };

                // Iterate over each shader
                for (size_t k = 0; k < p_subpass->shader_count; k++)
                {
                    
                    // Initialized data
                    GXShader_t *p_shader = g_find_shader(p_instance, p_subpass->shader_names[k]);
                    queue *draw_item_queue = dict_get(draw_item_queues, p_shader->name);
                    
                    while( queue_empty(draw_item_queue) == false )
                    {

                        // Initialized data
                        GXEntity_t *draw_item = 0;
                        queue_dequeue(draw_item_queue, &draw_item);
                        
                        // Print the name of the entity
                        //printf("0x%p : %s\n", draw_item, draw_item->name);

                        //fflush(stdout);
                    }

                    /*
                    // Set up the pipeline
                    {

                        // Graphics pipeline
                        if ( p_shader->type == g10_pipeline_graphics )
                        {

                            // Initialized data
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

                            // Bind the pipeline
                            vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_shader->graphics.pipeline);

                            // Set the viewport state
                            vkCmdSetViewport(command_buffer, 0, 1, &viewport);

                            // Set the scissors state
                            vkCmdSetScissor(command_buffer, 0, 1, &scissor);
                        }
                        // Compute pipeline
                        else if ( p_shader->type == g10_pipeline_compute )
                            vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_COMPUTE, p_shader->compute.pipeline);
                        // Ray pipeline
                        else if ( p_shader->type == g10_pipeline_ray )
                            vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, p_shader->ray.pipeline);
                        // Default
                        else
                            goto wrong_pipeline_type;
                    }

                    // Use the graphics pipeline
                    if ( p_shader->type == g10_pipeline_graphics )
                    {

                        // Draw each draw object
                        while ( !queue_empty(draw_item_queue) )
                        {

                            // Initialized data
                            GXEntity_t *p_entity = 0;

                            // Get a draw item
                            queue_dequeue(draw_item_queue, (void **)&p_entity);

                            // Draw the item
                            dict_foreach(p_entity->parts, &draw_part);

                        }
                    }
                    // Use the compute pipeline
                    else if ( p_shader->type == g10_pipeline_compute )
                    {
                        //vkCmdDispatch(command_buffer,p_shader->compute.x_groups,p_shader->compute.y_groups,p_shader->compute.z_groups);
                    }
                    // Use the ray pipeline
                    else if ( p_shader->type == g10_pipeline_ray )
                    {

                        // TODO:
                        
                        // * 1. Generate N rays via compute shader, and store them in memory.
                        // * 2. Use the closest intersection shader to put rays in material queues
                        // * 3. Process gasses and liquids
                        // * 4. Process emissive materials and skybox
                        // * 5. Compute material BSDFs, sample lights, and generate more rays.
                        // * 6. Trace shadow rays
                        // * 7. Repeat step 1-7 until frame is finished
                         
                    }
                    */
                }

                // Next subpass, if this is not the last subpass
                if ( p_render_pass->subpasses_count-1 != j )
                    vkCmdNextSubpass2(command_buffer, &subpass_begin_info, &subpass_end_info);
            }

            // End the render pass
            vkCmdEndRenderPass(command_buffer);
        }

        // End the command buffer
        if ( vkEndCommandBuffer(command_buffer) != VK_SUCCESS ) goto failed_to_end_command_buffer;
    }

    // Submit the commands
    {

        // Initialized data
        VkSubmitInfo submit_info =
        {
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = &wait_semaphores,
            .pWaitDstStageMask    = &wait_stages,
            .commandBufferCount   = 1,
            .pCommandBuffers      = &p_renderer->command_buffers[current_frame],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = &signal_semaphores
        };
        VkPresentInfoKHR present_info =
        {
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = &signal_semaphores,
            .swapchainCount     = 1,
            .pSwapchains        = swap_chains,
            .pImageIndices      = &p_instance->vulkan.image_index
        };

        // Submit the draw commands
        if ( 
            vkQueueSubmit(
                p_instance->vulkan.queues.present_queue, 
                1,
                &submit_info,
                in_flight_fence
            ) != VK_SUCCESS 
        ) goto failed_to_submit_command_buffer;

        // Present the image to the swapchain
        result = vkQueuePresentKHR(p_instance->vulkan.queues.present_queue, &present_info);

        // Does the window need to be resized?
        switch (result)
        {
            case VK_ERROR_OUT_OF_DATE_KHR:
            case VK_SUBOPTIMAL_KHR:
                g_window_resize(p_instance);
            case VK_SUCCESS:
                break;
            default:
                goto failed_to_queue_presentation;
        }
    }

    // Tick the frame counter
    p_instance->vulkan.current_frame = (current_frame + 1) % p_instance->vulkan.max_buffered_frames;
    
fail:

    end = SDL_GetPerformanceCounter();
    p_instance->time.delta_time = (double)(((double)(end - start)) / (double)(p_instance->time.clock_div));
    //if (p_instance->time.ticks % 1000 == 1)
    //{
        printf("Render time: %.5fms \r", (double)(p_instance->time.delta_time * 1000.0));
        fflush(stdout);
    //}
    p_instance->time.ticks += 1;

    // Success
    return 1;

    failed_to_submit_command_buffer:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Context errors
        {
            no_instance_context_renderer:
                #ifndef NDEBUG
                    //g_print_error("[G10] [Renderer] Parameter \"p_instance->context.renderer\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            wrong_pipeline_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Shader pipeline was of wrong type in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan error
        {

            failed_to_queue_presentation:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to present swapchain in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_end_command_buffer:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to end command buffer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void draw_part ( void *vp_part )
{

    // Argument check
    #ifndef NDEBUG
        if ( vp_part == (void *) 0 ) goto no_part;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXPart_t *p_part = vp_part;
    VkBuffer vertex_buffers[] = { p_part->vertex_buffer };
    VkDeviceSize offsets[] = { 0 };

    // Bind the vertex buffers for the draw call
    vkCmdBindVertexBuffers(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0, 1, vertex_buffers, offsets);

    // Bind the face buffers for the draw call
    vkCmdBindIndexBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], p_part->element_buffer, 0, VK_INDEX_TYPE_UINT32);

    // Draw the part
    //vkCmdDrawIndexed(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], (u32)p_part->index_count*3, 1, 0, 0, 0);

    // Success
    return;

    // Error handling
    {

        // Argument errors
        {
            no_part:
                #ifndef NDEBUG
                    g_print_error("[G10] [Part] Null pointer provided for parameter \"p_part\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return;
        }
    }
}

int present_frame ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Success
    return 1;

    // Error handling
    {
        no_instance:
            #ifndef NDEBUG
                g_print_error("[G10] [Renderer] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int destroy_renderer ( GXRenderer_t **pp_renderer )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_renderer == (void *) 0 ) goto no_renderer;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXRenderer_t *p_renderer = *pp_renderer;

    // No more pointer for caller
    *pp_renderer = 0;

    // Error check
    if ( p_renderer == (void *) 0 ) goto pointer_to_null_pointer;

    // Free the name
    free(p_renderer->name);

    // Free the shaders
    // TODO:

    // Free the render passes
    // Iterate over each render pass
    for (size_t i = 0; i < p_renderer->render_pass_count; i++)
    {

        // Initialized data
        //GXRenderPass_t *p_render_pass = p_renderer->render_passes_data[i];

        // Destroy the render pass
        //destroy_render_pass(&p_render_pass);
    }

    // Destroy the render passes dictionary
    //dict_destroy(&p_renderer->render_passes);

    // TODO: Free the attachments
    // TODO: Free the attachments dict

    // TODO: Free the clear colors

    // TODO: Destroy the shader LUT

    // Free the synchronization primatives
    {
        
        // Free the semaphores
        {

            // Initialized data
            size_t        len           = dict_values(p_renderer->synchronization.binary_semaphores, 0);
            VkSemaphore **pp_semaphores = calloc(len, sizeof(VkSemaphore *));

            // Get the contents of the dictionary
            dict_values(p_renderer->synchronization.binary_semaphores, pp_semaphores);

            // Iterate over each set of semaphores
            for (size_t i = 0; i < len; i++)
            {
            
                // Initialized data
                VkSemaphore *p_semaphore = pp_semaphores[i];

                // Iterate over each semaphore in the set
                for (size_t j = 0; j < p_instance->vulkan.max_buffered_frames; j++)

                    // Destroy the semaphore
                    vkDestroySemaphore(p_instance->vulkan.device, p_semaphore[j], 0);
            }
        }

        // Free the fences
        {

            // Initialized data
            size_t    len       = dict_values(p_renderer->synchronization.fences, 0);
            VkFence **pp_fences = calloc(len, sizeof(VkFence *));

            // Get the contents of the dictionary
            dict_values(p_renderer->synchronization.fences, pp_fences);

            // Iterate over each set of fences
            for (size_t i = 0; i < len; i++)
            {
            
                // Initialized data
                VkFence *p_fence = pp_fences[i];

                // Iterate over each fence in the set
                for (size_t j = 0; j < p_instance->vulkan.max_buffered_frames; j++)

                    // Destroy the fence
                    vkDestroyFence(p_instance->vulkan.device, p_fence[j], 0);
            }
        }
    
        // Free the command buffers
        {
            p_renderer->command_buffers_count;
        }

        // Free the command pool
        vkDestroyCommandPool(p_instance->vulkan.device, p_renderer->command_pool, 0);
    }

    // Free the render
    free(p_renderer);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"pp_renderer\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int destroy_render_pass ( GXRenderPass_t **pp_render_pass )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
    #endif

    // Initialized data
    GXInstance_t   *p_instance    = g_get_active_instance();
    GXRenderPass_t *p_render_pass = *pp_render_pass;

    // No more pointer for caller
    *pp_render_pass = 0;

    // Error check
    if ( p_render_pass == (void *) 0 ) goto no_render_pass;

    // Free the render pass name
    free(p_render_pass->name);

    // Free the subpasses
    // Iterate over each subpass
    for (size_t i = 0; i < p_render_pass->subpasses_count; i++)
    {

        // Initialized data
        GXSubpass_t *p_subpass = p_render_pass->subpasses_data[i];

        // Destroy the subpass
        destroy_subpass(&p_subpass);
    }

    // Destroy the subpass disctionary
    dict_destroy(&p_render_pass->subpasses);

    // Free the framebuffers
    // Iterate over each framebuffer
    // TODO:
    //for (size_t i = 0; i < p_instance->vulkan.current_frame; i++)
    //{
    //    /* code */
    //}


    // Free the vulkan render pass
    vkDestroyRenderPass(p_instance->vulkan.device, p_render_pass->render_pass, 0);

    // Free the render pass
    free(p_render_pass);

    /*
    dict *subpasses,
         *draw_queue_types,
         *attachments; // type: char *

    size_t subpasses_count,
           attachments_count;

    GXSubpass_t **subpasses_data;
    VkFramebuffer *framebuffers;
    GXAttachment_t **attachments_data;

    VkImageView *attachment_image_views;
    */

    free(p_render_pass);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"pp_image\" pointed to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_subpass ( GXSubpass_t **pp_subpass )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_subpass == (void *) 0 ) goto no_subpass;
    #endif

    // Initialized data
    GXSubpass_t *p_subpass = *pp_subpass;

    // No more pointer for caller
    *pp_subpass = 0;

    // Error check
    if ( p_subpass == (void *) 0 ) goto pointer_to_null_pointer;

    // Free the subpass name
    free(p_subpass->name);

    // Free the shader names
    for (size_t i = 0; i > p_subpass->shader_count; i++)
    {

        // Initialized data
        char *i_shader_name = p_subpass->shader_names[i];

        // Free the shader name
        free (i_shader_name);
    }

    // Free the list of shader names
    free(p_subpass->shader_names);


    // Free the subpass attachment references
    {
        //if ( p_subpass->subpass_description.pColorAttachments )
        //    free(p_subpass->subpass_description.pColorAttachments);

        //if ( p_subpass->subpass_description.pInputAttachments )
        //    free(p_subpass->subpass_description.pInputAttachments);

        //if ( p_subpass->subpass_description.pPreserveAttachments )
        //    free(p_subpass->subpass_description.pPreserveAttachments);

        //if ( p_subpass->subpass_description.pDepthStencilAttachment )
        //    free(p_subpass->subpass_description.pDepthStencilAttachment);
    }

    // TODO: Destroy the shader
    //destroy_shader(&p_subpass->shader);

    // Free the subpass
    free(p_subpass);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"pp_image\" pointed to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_image ( GXImage_t **pp_image )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_image == (void *) 0 ) goto no_image;
    #endif

    // Initializd data
    GXInstance_t *p_instance = g_get_active_instance();
    GXImage_t    *p_image    = *pp_image;

    // Error check
    if ( p_image == (void *) 0 ) goto pointer_to_null_pointer;

    // No more pointer for caller
    *pp_image = 0;

    // Free the name
    free(p_image->name);

    // Free the VkImage
    vkDestroyImage(p_instance->vulkan.device, p_image->image, 0);

    // Free the vulkan memory
    vkFreeMemory(p_instance->vulkan.device, p_image->image_memory, 0);

    // Free the image
    free(p_image);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_image\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"pp_image\" pointed to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_attachment ( GXAttachment_t **pp_attachment )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_attachment == (void *) 0 ) goto no_attachment;
    #endif

    // Initializd data
    GXInstance_t   *p_instance   = g_get_active_instance();
    GXAttachment_t *p_attachment = *pp_attachment;

    // Error check
    if ( p_attachment == (void *) 0 ) goto pointer_to_null_pointer;

    // No more pointer for caller
    *pp_attachment = 0;

    // Free the name
    free(p_attachment->name);

    // Destroy the attachments image view
    vkDestroyImageView(p_instance->vulkan.device, p_attachment->image_view, 0);

    // Destroy the attachment image
    destroy_image(&p_attachment->p_image);

    // Free the attachment
    free(p_attachment);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_attachment:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_attachment\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"pp_attachment\" pointed to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_texture ( GXTexture_t **pp_texture )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_texture == (void *) 0 ) goto no_texture;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXTexture_t  *p_texture  = *pp_texture;

    // Error check
    if ( p_texture == (void *) 0 ) goto pointer_to_null_pointer;

    // No more pointer for end user
    *pp_texture = 0;

    // Free the name
    free(p_texture->name);

    // Free the sampler
    vkDestroySampler(p_instance->vulkan.device, p_texture->sampler, 0);

    // Free the image view
    vkDestroyImageView(p_instance->vulkan.device, p_texture->image_view, 0);

    // Free the image
    destroy_image(&p_texture->p_image);

    // Free the texture
    free(p_texture);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"pp_texture\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}