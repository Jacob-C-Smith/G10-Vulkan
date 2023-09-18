/** !
 * @file G10/G10.h
 * 
 * @author Jacob Smith
 *
 * Include header for G10.
 */

// Include guard
#pragma once

// Build version
#define G10_VERSION_MAJOR 1
#define G10_VERSION_MINOR 0
#define G10_VERSION_PATCH 0

// Build options. 
#define BUILD_G10_WITH_ANSI_COLOR
#define BUILD_G10_WITH_SDL_NET
//#define BUILD_G10_WITH_DISCORD
//#define BUILD_G10_WITH_FMOD

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Vulkan
//#include <vulkan/vulkan.h>

// SDL 2
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_vulkan.h>

// SDL 2 Bug
#ifdef main
#undef main
#endif

// Dictionary submodule
#include <dict/dict.h>

// Arrary submodule
#include <array/array.h>

// Queue submodule
#include <queue/queue.h>

// Stack submodule
#include <stack/stack.h>

// JSON submodule
#include <json/json.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/GXScene.h>
//#include <G10/GXRenderer.h>
#include <G10/GXInput.h>
#include <G10/GXScheduler.h>
#include <G10/GXMaterial.h>
#include <G10/GXCollider.h>
#include <G10/GXServer.h>
#include <G10/GXAudio.h>

// 3rd party

// Discord
#ifdef BUILD_G10_WITH_DISCORD
#include <G10/GXDiscordIntegration.h>
#include <discord_game_sdk.h>
#endif

// FMOD
#ifdef BUILD_G10_WITH_FMOD
#include <FMOD-core/fmod.h>
#endif

// Uncomment when implementing UI
//#include <UI/UI.h>


// Set the reallocator for the dict submodule
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array submodule
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the stack submodule
#ifdef STACK_REALLOC
    #undef STACK_REALLOC
    #define STACK_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the queue submodule
#ifdef QUEUE_REALLOC
    #undef QUEUE_REALLOC
    #define QUEUE_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the JSON submodule
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef G10_REALLOC
#define G10_REALLOC(p, sz) realloc(p,sz)
#endif

// Structures
struct GXInstance_s
{

    // Name
    char *name;

    // SDL2
    struct
    {
        SDL_Window        *window;
        SDL_Event          event;
        SDL_AudioDeviceID  audio_device;
    } sdl2;

    // Vulkan
    /*struct
    {
        VkInstance                instance;

        VkDebugUtilsMessengerEXT  debug_messenger;
        VkSurfaceKHR              surface;

        VkPhysicalDevice          physical_device;
        VkDevice                  device;

        VkCommandPool             command_pool;
        VkCommandBuffer          *command_buffers;

        VkSemaphore              *image_available_semaphores;
        VkSemaphore              *render_finished_semaphores;
        VkFence                  *in_flight_fences;

        i32                       max_buffered_frames;
        u32                       current_frame,
                                  image_count,
                                  image_index;
        

        char                     *requested_device_name;
        dict                     *device_extensions;

        #pragma pack(push)
        #pragma pack (4)
        struct { int g, p; }      queue_family_indices;
        #pragma pack(pop)  

        struct
        {
            VkSwapchainKHR  swap_chain;
            VkImage        *images;
            VkFormat        image_format;
            VkExtent2D      extent;
            VkImageView    *image_views;
            VkFramebuffer  *framebuffers;
            
            struct { 
                VkSurfaceCapabilitiesKHR capabilities;
                size_t format_count,
                       present_mode_count;
                VkSurfaceFormatKHR* formats;
                VkPresentModeKHR* present_modes;
            } details;
        } swap_chain;

        struct
        {
            VkQueue *graphics_queues,
                    *compute_queues,
                    *transfer_queues,
                     present_queue;
            u32     *graphics_family_indicies,
                    *compute_family_indicies,
                    *transfer_family_indicies,
                     present_family_index;
            size_t   graphics_count,
                     compute_count,
                     transfer_count,
                     graphics_family_count,
                     compute_family_count,
                     transfer_family_count;
            float    priority[8];
        } queues;
    } vulkan;*/

    // Window parameters
    struct
    {
        char *title;
        u32   width,
              height;
        bool  fullscreen;

    } window;

    // Context
    struct
    {
        GXSchedule_t  *schedule;
        GXScene_t     *scene,
                      *loading_scene;
        GXRenderer_t  *renderer,
                      *loading_renderer;
        int          (*user_code_callback) (GXInstance_t *instance);
    } context;

    // Data
    struct
    {
        dict *schedules,
             *scenes;
    } data;

    // Networking
    struct
    {
        GXServer_t *server;
        GXClient_t *client;
    } networking;

    // Cache
    struct 
    {
        dict *parts,
             *materials,
             *shaders,
             *ais;
    } cache;

    // Queues
    struct
    {
        queue *load_entity,
              *load_light_probe,
              *actor_move,
              *actor_collision,
              *actor_force,
              *ai_preupdate,
              *ai_update;
    } queues;

    // Mutexes
    struct
    {
        SDL_mutex *load_entity,
                  *shader_cache,
                  *part_cache,
                  *ai_cache,
                  *material_cache,
                  *move_object,
                  *update_force,
                  *resolve_collision,
                  *ai_preupdate,
                  *ai_update;
    } mutexes;

    // Time
    struct
    {
        size_t  ticks;
        size_t  fixed_tick_rate;
        u32     d,
                last_time;
        u64     clock_div;
        double  delta_time;
    } time;

    // Discord integration
    #ifdef BUILD_G10_WITH_DISCORD

    struct
    {
        struct IDiscordCore                *core;
        struct IDiscordUserManager         *users;
        struct IDiscordAchievementManager  *achievements;
        struct IDiscordActivityManager     *activities;
        struct IDiscordRelationshipManager *relationships;
        struct IDiscordApplicationManager  *application;
        struct IDiscordLobbyManager        *lobbies;
        struct DiscordUser                  user;
        DiscordUserId                       user_id;
    } discord_integration;

    #endif

    // FMOD Integratdion
    #ifdef BUILD_G10_WITH_FMOD

    struct
    {
        FMOD_SYSTEM* system;


    } fmod;

    #endif

    // Input
    GXInput_t *input;

    // How many threads should be used to load a scene
    size_t     loading_thread_count;

    bool       running;
};

// Allocators

/** !
 *  Construct a G10 instance from a JSON file
 *
 * @param pp_instance : return
 * @param path        : Path to instance JSON file
 *
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_init ( GXInstance_t **pp_instance, const char *path );

/** !
 * TODO: DOCUMENT
 *  Create a Vulkan buffer
 *
 * @param size          : Double pointer to instance
 * @param usage         : Path to instance JSON
 * @param properties    :
 * @param buffer        :
 * @param buffer_memory :
 * @return 1 on success, 0 on error
 */
//DLLEXPORT void create_buffer ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory );

// File operations
/** !
 *  Load a file. If buffer is null pointer, returns size of file
 *
 * @param path        : Path to file
 * @param buffer      : Buffer to read file
 * @param binary_mode : "r" if false else "rb"
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT size_t g_load_file ( const char *path, void *buffer, bool binary_mode );

// Window operations
/** !
 *  Resize the window using instance->window_width and instance->window_height
 *
 * @param p_instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int   g_window_resize ( GXInstance_t *p_instance );

// Debug logging
/** !
 *  printf in ANSI red
 *
 * @param format : printf format parameter
 * @param ...    : Additional arguments
 *
 * @sa g_print_warning
 * @sa g_print_log
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_print_error ( const char *const format, ... );

/** !
 *  printf in ANSI yellow
 *
 * @param format : printf format parameter
 * @param ...    : Additional arguments
 *
 * @sa g_print_error
 * @sa g_print_log
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_print_warning ( const char *const format, ... );

/** !
 *  printf in ANSI blue
 *
 * @param format : printf format parameter
 * @param ...    : Additional arguments
 *
 * @sa g_print_error
 * @sa g_print_warning
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_print_log ( const char *const format, ... );

/** !
 *  Set an instances active schedule, and start running said schedule
 *
 *  @param instance : the instance
 *  @param name     : the name of the schedule to start running
 *
 *  @sa g_stop_schedule
 *
 *  @return 1 on success, 0 on error
 */
DLLEXPORT int g_start_schedule ( GXInstance_t *p_instance, const char *name );

/** !
 *  Stop the instances active schedule
 *
 *  @param p_instance : the instance
 *
 *  @sa g_stop_schedule
 *  @sa g_user_exit
 *
 *  @return 1 on success, 0 on error
 */
DLLEXPORT int g_stop_schedule ( GXInstance_t *p_instance );

// State copy
/** !
 *  Copies state from context data into instance
 *
 * @param instance: The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int copy_state ( GXInstance_t *p_instance );

// Getters
/** !
 *  Returns the active instance
 *
 *  @return pointer to active instance on success, 0 on error
 */
DLLEXPORT GXInstance_t* g_get_active_instance ( void );

// Input
/** !
 * Find a bind in the instance's active input structure
 * 
 * @param p_instance : the instance
 * @param bind_name  : the name of the bind
 * @param pp_bind    : return        
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int g_find_bind ( GXInstance_t *p_instance, char *bind_name, GXBind_t **pp_bind );

// Cache operations
/** !
 * Cache a material. Caching a material adds it to the garbage collector.
 *
 * @param instance : The active instance
 * @param material : A pointer to a material
 *
 * @sa g_find_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_cache_material ( GXInstance_t *p_instance, GXMaterial_t *p_material );

/** !
 * Cache a part. Caching a part adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param p_part     : A pointer to a part
 *
 * @sa g_find_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_cache_part ( GXInstance_t *p_instance, GXPart_t *p_part );

/** !
 * Cache a shader. Caching a shader adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param p_shader   : A pointer to a shader
 *
 * @sa g_find_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_cache_shader ( GXInstance_t *p_instance, GXShader_t *p_shader );

/** !
 * Cache an ai. Caching an ai adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param p_ai     : A pointer to an ai
 *
 * @sa g_find_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_cache_ai ( GXInstance_t *p_instance, GXAI_t *p_ai );

/** !
 * Search the cache for a material. Caching a material adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param name       : The name of the ai
 *
 * @sa g_find_ai
 * @sa g_find_part
 * @sa g_find_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT GXMaterial_t *g_find_material ( GXInstance_t *p_instance, char *name );

/** !
 * Search the cache for a part. Caching a part adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param name       : The name of the part
 *
 * @sa g_find_material
 * @sa g_find_shader
 * @sa g_find_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT GXPart_t *g_find_part ( GXInstance_t *p_instance, char *name );

/** !
 * Search the cache for a shader. Caching a shader adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param name       : The name of the shader
 *
 * @sa g_find_material
 * @sa g_find_part
 * @sa g_find_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT GXShader_t *g_find_shader ( GXInstance_t *p_instance, char *name );

/** !
 * Search the ai for a shader. Caching an ai adds it to the garbage collector.
 *
 * @param p_instance : The active instance
 * @param name       : The name of the ai
 *
 * @sa g_find_material
 * @sa g_find_part
 * @sa g_find_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT GXAI_t *g_find_ai ( GXInstance_t *p_instance, char *name );

// User operations
/** !
 * The user calls this function to exit the game
 *
 * @param state      : Input state
 * @param p_instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT void g_user_exit         ( callback_parameter_t state, GXInstance_t *p_instance );

/** !
 * Open the G10 website in the web browser
 * 
 * @param input      : Input state
 * @param p_instance : The active instance
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT void g_user_help ( callback_parameter_t input, GXInstance_t* p_instance );

/** !
 * Toggle mouse lock
 *
 * @param state      : Input state
 * @param p_instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT void g_toggle_mouse_lock ( callback_parameter_t state, GXInstance_t *p_instance );

/** !
 * Play a sound
 *
 * @param state      : Input state
 * @param p_instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT void g_play_sound        ( callback_parameter_t state, GXInstance_t *p_instance );

// Conversions
/** !
 * Convert radians to degrees
 *
 * @param radians : Radians
 *
 * @sa to_radians
 *
 * @return degrees
 */
inline float to_degrees ( float radians )
{
    return radians * (180.f / (float)3.141593);
}

/** !
 * Convert degrees to radians
 *
 * @param degrees : Degrees
 *
 * @sa to_degrees
 *
 * @return Radians
 */
inline float to_radians ( float degrees )
{
    return  degrees * ((float)3.141593 / 180.f);
}

// Destructors
/** !
 * Destroy a G10 instance
 *
 * @param p_instance : pointer to the instance pointer
 *
 * @sa g_init
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_exit ( GXInstance_t **pp_instance );