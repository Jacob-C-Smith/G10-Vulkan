#include <G10/G10.h>

// Uninitialized data
FILE* log_file;

// Initialized data
static GXInstance_t *active_instance = 0;

// Forward declared functions
/** !
 * Find a physical device that conforms to the properties described by the
 * p_value parameter, or find the best possible device as evaluated by
 * criteria specified by Jake. This device is stored internally.
 *
 * @param p_value : The "vulkan" property of the G10 instance JSON object -or- a null pointer
 *
 * @return 1 on success, 0 on error
*/
int  g_pick_physical_device ( json_value *p_value );


/** !
 * Create a Vulkan instance
 *
 * @param p_value : The "instance" property of the "vulkan" property G10 instance JSON object -or- a null pointer
 *
 * @return 1 on success, 0 on error
*/
int g_construct_vulkan_instance ( json_value *p_value );

/** !
 * Create a surface using the builds specified WSI
 *
 * @return 1 on success, 0 on error
*/
int g_construct_vulkan_surface ( void );

/** !
 * Create a Vulkan instance
 *
 * @param p_value : The "instance" property of the "vulkan" property G10 instance JSON object -or- a null pointer
 *
 * @return 1 on success, 0 on error
*/
int g_construct_vulkan_physical_device ( json_value *p_value );

/** !
 * Create a logical device
 *
 * @param p_value : The "vulkan" property of the G10 instance JSON object -or- a null pointer
 *
 * @return 1 on success, 0 on error
*/
int g_construct_vulkan_logical_device ( json_value *p_value );

/** !
 * Create a swapchain
 *
 * @param p_value : The "swap chain" property of the "vulkan" property G10 instance JSON object -or- a null pointer
 *
 * @return 1 on success, 0 on error
*/
int g_construct_vulkan_swap_chain ( json_value *p_value );

int g_construct_vulkan_image_views ( json_value *p_value );
int g_construct_vulkan_command_pool ( json_value *p_value );
int g_construct_vulkan_command_buffers ( json_value *p_value );

int  pick_physical_device   ( char **required_extension_names );
int  create_logical_device  ( char **required_extension_names );
void create_swap_chain      ( void );
void cleanup_swap_chain     ( void );
void create_image_views     ( void );
void create_command_pool    ( void );
void create_command_buffers ( void );

/*
void create_buffer          ( VkDeviceSize  size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory );
u32  find_memory_type       ( u32 type_filter, VkMemoryPropertyFlags properties );
int  check_vulkan_device    ( GXInstance_t *p_instance , VkPhysicalDevice physical_device, char **required_extension_names);

VKAPI_ATTR
VkBool32
VKAPI_CALL debug_callback (
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData
)
{

    switch ((u32)messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            //printf("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            g_print_log("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            g_print_warning("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            g_print_error("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
    }

    return VK_FALSE;
}
int setup_debug_messenger(VkDebugUtilsMessengerCreateInfoEXT** debug_messenger_create_info);
VkResult CreateDebugUtilsMessengerEXT ( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    // TODO: Improve documentation
    // Initialized data
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    // Check for the function
    if ( func != 0 )
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}
*/

int g_init ( GXInstance_t **pp_instance, const char *path )
{

    // Argument Check
    #ifndef NDEBUG
        if ( pp_instance == (void *) 0 ) goto no_instance;
        if ( path        == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    GXInstance_t  *p_instance                      = 0;
    size_t         text_len                        = g_load_file(path, 0, true);
    char          *text                            = calloc(text_len, sizeof(u8));
    json_value    *p_value                         = 0,
                  *p_name                          = 0,
                  *p_window                        = 0,
                  *p_requested_physical_device     = 0,
                  *p_max_buffered_frames           = 0,
                  *p_initial_scene                 = 0,
                  *p_cache                         = 0,
                  *p_loading_thread_count          = 0,
                  *p_vulkan                        = 0,
                  *p_log_file_i                    = 0,
                  *p_input                         = 0,
                  *p_audio                         = 0,
                  *p_server                        = 0,
                  *p_renderer                      = 0,
                  *p_requested_validation_layers   = 0,
                  *p_requested_instance_extensions = 0,
                  *p_device_extensions             = 0,
                  *p_schedules                     = 0,
                  *p_fixed_tick_rate               = 0;

    // Load the file
    if ( g_load_file(path, text, true) == 0 ) goto no_file;

    // Parse JSON text into a json_value *
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Parse the JSON
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name = dict_get(p_dict, "name");

        // Optional properties
        p_window               = dict_get(p_dict, "window");
        p_input                = dict_get(p_dict, "input");
        p_log_file_i           = dict_get(p_dict, "log file");
        p_initial_scene        = dict_get(p_dict, "initial scene");
        p_cache                = dict_get(p_dict, "cache");
        p_loading_thread_count = dict_get(p_dict, "loading thread count");
        p_vulkan               = dict_get(p_dict, "vulkan");
        p_renderer             = dict_get(p_dict, "renderer");
        p_server               = dict_get(p_dict, "server");
        p_schedules            = dict_get(p_dict, "schedules");
        p_fixed_tick_rate      = dict_get(p_dict, "fixed tick rate");

        // Error check
        if ( ! ( p_name /* && p_schedule, so on */ ) ) goto missing_properties;
    }

    // Allocate memory for *the* instance
    {

        // Allocate memory for instance
        p_instance = calloc(1, sizeof(GXInstance_t));

        // Error check
        if ( p_instance == (void *) 0 ) goto no_mem;

        // Set the active instance
        active_instance = p_instance;
    }

    // Global initialization
    {

        timer_init();

        // Log file initialization
        if ( p_log_file_i )
        {

            // Parse the log file as a string
            if ( p_log_file_i->type == JSON_VALUE_STRING )

                // Open the log file using the provided path
                log_file = fopen(p_log_file_i->string, "w");

            // Default
            else
                goto wrong_log_file_type;
        }
        // Default
        else
            log_file = stdout;

        // Window initialization
        if ( p_window )
        {

            // Parse the window as a JSON object
            if ( p_window->type == JSON_VALUE_OBJECT )
            {

                // Initialized data
                json_value *p_window_width      = 0,
                            *p_window_height     = 0,
                            *p_window_title      = 0,
                            *p_window_fullscreen = 0;

                // Parse the JSON
                {

                    // Initialized data
                    dict *p_dict = p_window->object;

                    // Required properties
                    p_window_width  = dict_get(p_dict, "width");
                    p_window_height = dict_get(p_dict, "height");
                    p_window_title  = dict_get(p_dict, "title");

                    // Optional properties
                    p_window_fullscreen = dict_get(p_dict, "fullscreen");

                    // Error check
                    if ( ! (
                        p_window_width  &&
                        p_window_height &&
                        p_window_title
                    ) )
                        goto missing_properties;
                }

                // Set the window title
                if ( p_window_title->type == JSON_VALUE_STRING )
                {

                    // Initialized data
                    size_t title_len = strlen(p_window_title->string);

                    // Allocate memory
                    p_instance->window.title = calloc(title_len+1, sizeof(char));

                    // Error check
                    if ( p_instance->window.title == (void *) 0 ) goto wrong_window_title_type;

                    // Copy the title
                    strncpy(p_instance->window.title, p_window_title->string, title_len);
                }
                // Default
                else
                    goto wrong_window_title_type;

                // Set the window width
                if ( p_window_width->type == JSON_VALUE_INTEGER )
                    p_instance->window.width = (u32)p_window_width->integer;
                // Default
                else
                    goto wrong_window_width_type;

                // Set the window height
                if ( p_window_height->type == JSON_VALUE_INTEGER )
                    p_instance->window.height = (u32)p_window_height->integer;
                // Default
                else
                    goto wrong_window_height_type;

                // Set the window fullscreen bit
                if ( p_window_fullscreen )
                {

                    // Parse the fullscreen property as a boolean
                    if ( p_window_fullscreen->type == JSON_VALUE_BOOLEAN )
                        p_instance->window.fullscreen = p_window_fullscreen->boolean;

                    // Wrong type
                    else
                        goto wrong_window_fullscreen_type;
                }
                // Default to windowed
                else
                    p_instance->window.fullscreen = false;
            }
            // Default
            else
                goto wrong_window_type;
        }
        // Default to 1280x720, "Untitled G10 window"
        else
        {
            p_instance->window.width  = 1280,
            p_instance->window.height = 720;
            p_instance->window.title  = "Untitled G10 window";
        }

        // SDL initialization
        if ( p_instance->window.height && p_instance->window.width && p_instance->window.title )
        {

            // Initialize SDL
            if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) ) goto no_sdl2;

            // Initialize SDL Image
            if ( IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG ) == 0 ) goto no_sdl2_image;

            // Initialize SDL Networking
            #ifdef BUILD_G10_WITH_SDL_NET
                if ( SDLNet_Init() ) goto no_sdl2_network;
            #endif

            // TODO: Check boolean
            // Create the window
            p_instance->sdl2.window = SDL_CreateWindow(
                p_instance->window.title,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                (int)p_instance->window.width, (int)p_instance->window.height,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | ((p_instance->window.fullscreen) ? SDL_WINDOW_FULLSCREEN : 0)
            );

            // Check the window
            if ( p_instance->sdl2.window == (void *) 0 ) goto no_window;

            // Display the window
            SDL_ShowWindow(p_instance->sdl2.window);

            // Focus the game window
            (void)SDL_SetWindowInputFocus(p_instance->sdl2.window);

            // Get the clock divisor for high precision timing
            p_instance->time.clock_div = SDL_GetPerformanceFrequency();

        }
        // Default
        else
            goto failed_to_initialize_sdl2;

        // Vulkan initialization
        /*
        if ( p_vulkan )
        {

            // Parse the Vulkan properties
            if ( p_vulkan->type == JSON_VALUE_OBJECT )
            {

                // Initialized data
                char        **device_extensions      = 0;
                size_t        device_extension_count = 0;
                json_value  *p_vulkan_device        = 0,
                             *p_vulkan_instance      = 0,
                             *p_vulkan_swapchain     = 0;

                // Parse JSON value
                {

                    // Initialized data
                    dict *p_dict = p_vulkan->object;

                    // Required properties
                    p_vulkan_instance     = dict_get(p_dict, "instance");
                    p_vulkan_device       = dict_get(p_dict, "device");
                    p_vulkan_swapchain    = dict_get(p_dict, "swap chain");
                    p_max_buffered_frames = dict_get(p_dict, "max buffered frames");

                    // Check for missing properties
                    if ( ! (
                        p_vulkan_instance     &&
                        p_vulkan_device       &&
                        p_vulkan_swapchain    &&
                        p_max_buffered_frames
                    ) )
                        goto missing_vulkan_properties;
                }

                // Set the maximum number of buffered frames
                if ( p_max_buffered_frames->type == JSON_VALUE_INTEGER )
                    p_instance->vulkan.max_buffered_frames = (i32) p_max_buffered_frames->integer;
                // Default
                else
                    goto wrong_max_buffered_frames_type;

                // Initialize some useful LUTs
                {

                    // Initialized data
                    extern int init_renderer ( void );
                    extern int init_shader   ( void );

                    // Renderer initialization
                    if ( init_renderer() == 0 ) goto failed_to_initialize_renderer;

                    // Shader initialization
                    if ( init_shader()   == 0 ) goto failed_to_initialize_shader;
                }

                // Create an instance
                if ( g_construct_vulkan_instance(p_vulkan_instance) == 0 ) goto wrong_vulkan_instance_type;

                // Construct a surface with the WSI
                if ( g_construct_vulkan_surface() == 0 ) goto failed_to_construct_vulkan_surface;

                // Get a physical device
                if ( g_construct_vulkan_physical_device(p_vulkan_device) == 0 ) goto failed_to_construct_vulkan_device;

                // Create a logical device
                if ( g_construct_vulkan_logical_device(p_vulkan_device) == 0 ) goto failed_to_construct_vulkan_device;

                // Create a swap chain
                if ( g_construct_vulkan_swap_chain(p_vulkan_swapchain) == 0 ) goto failed_to_construct_vulkan_swapchain;

            }
            // Default
            else
                goto wrong_vulkan_type;
        }
        // Default
        else
            goto no_vulkan_property;
        */
        
        // G10 Initialization
        {

            // Set the name
            if ( p_name->type == JSON_VALUE_STRING )
            {

                // Initialized data
                size_t name_len = strlen(p_name->string);

                // Error check
                if ( name_len > 255 ) goto name_propery_too_long;

                // Allocate memory for the name
                p_instance->name = calloc(name_len+1, sizeof(u8));

                // Error check
                if ( p_instance->name == (void *) 0 ) goto no_mem;

                // Copy the name to the instance
                strncpy(p_instance->name, p_name->string, name_len);
            }

            // Initialize subsystems
            {

                // External functions
                extern void init_texture    ( void );
                extern void init_input      ( void );
                extern void init_part       ( void );
                extern void init_material   ( void );
                extern void init_scheduler  ( void );
                extern void init_scene      ( void );
                extern void init_collider   ( void );
                extern void init_physics    ( void );
                extern void init_ai         ( void );

                // Set the loading thread limit
                if ( p_loading_thread_count )
                {
                    // Parse the loading thread count as an integer
                    if ( p_loading_thread_count->type == JSON_VALUE_INTEGER )
                        p_instance->loading_thread_count = p_loading_thread_count->integer;
                    // Default
                    else
                        goto wrong_loading_thread_count_type;
                }
                // Default
                else
                    p_instance->loading_thread_count = 4;

                // Input initialization
                init_input();

                // Audio initialization
                //init_audio();

                // Part initialization
                init_part();

                // Material initialization
                init_material();

                // Scheduler initialization
                init_scheduler();

                // Scene initialization
                init_scene();

                // Collider initialization
                //init_collider();

                // Physics initialization
                //init_physics();

                // AI initialization
                init_ai();
            }

            // Data
            {

                // Construct a dictionary for schedules
                (void)dict_construct(&p_instance->data.scenes, 16);

                // Construct a dictionary for schedules
                (void)dict_construct(&p_instance->data.schedules, 8);
            }

            // Caches
            if ( p_cache )
            {

                // Initialized data
                json_value *p_material_cache_count = 0,
                            *p_part_cache_count     = 0,
                            *p_shader_cache_count   = 0,
                            *p_ai_cache_count       = 0;

                // Parse the JSON value
                {

                    // Initialized data
                    dict *p_dict = p_cache->object;

                    // Optional properties
                    p_material_cache_count = dict_get(p_dict, "material count");
                    p_part_cache_count     = dict_get(p_dict, "part count");
                    p_shader_cache_count   = dict_get(p_dict, "shader count");
                    p_ai_cache_count       = dict_get(p_dict, "ai count");
                }

                // Construct the material cache
                if ( p_material_cache_count )
                {

                    // Parse the material cache count as an integer
                    if ( p_material_cache_count->type == JSON_VALUE_INTEGER )
                        dict_construct(&p_instance->cache.materials, p_material_cache_count->integer);
                    // Default
                    else
                        goto wrong_material_cache_count_type;
                }

                // Construct the part cache
                if ( p_part_cache_count )
                {

                    // Parse the part cache count as an integer
                    if ( p_part_cache_count->type == JSON_VALUE_INTEGER )
                        dict_construct(&p_instance->cache.parts, p_part_cache_count->integer);
                    // Default
                    else
                        goto wrong_part_cache_count_type;
                }

                // Construct the shader cache
                if ( p_shader_cache_count )
                {

                    // Parse the shader cache count as an integer
                    if ( p_shader_cache_count->type == JSON_VALUE_INTEGER )
                        dict_construct(&p_instance->cache.shaders, p_shader_cache_count->integer);
                    // Default
                    else
                        goto wrong_shader_cache_count_type;
                }

                // Construct the AI cache
                if ( p_ai_cache_count )
                {
                    // Parse the ai cache count as an integer
                    if ( p_ai_cache_count->type == JSON_VALUE_INTEGER )
                        dict_construct(&p_instance->cache.ais, p_ai_cache_count->integer);
                    // Default
                    else
                        goto wrong_ai_cache_count_type;
                }
            }
            // Default
            else
            {

                // Construct the cache
                (void)dict_construct(&p_instance->cache.materials, 128);
                (void)dict_construct(&p_instance->cache.parts, 128);
                (void)dict_construct(&p_instance->cache.shaders, 32);
                (void)dict_construct(&p_instance->cache.ais, 16);
            }

            // Queues
            {

                // Queue for entities with colliders
                (void)queue_construct(&p_instance->queues.actor_collision);

                // Queues for entities with rigidbodies
                (void)queue_construct(&p_instance->queues.actor_force);
                (void)queue_construct(&p_instance->queues.actor_move);

                // Queues for entities with AIs
                (void)queue_construct(&p_instance->queues.ai_preupdate);
                (void)queue_construct(&p_instance->queues.ai_update);

                // Queue for entities to load
                (void)queue_construct(&p_instance->queues.load_entity);
            }

            // Load a renderer
            /*
            if ( p_renderer )
            {

                // Parse the renderer as a JSON value
                if ( load_renderer_as_json_value(&p_instance->context.renderer, p_renderer) == 0 ) goto failed_to_load_schedule;
            }*/

            // Load an input
            if ( p_input )
            {

                // Parse the input as a JSON value
                if ( load_input_as_json_value(&p_instance->input, p_input) == 0 ) goto failed_to_load_input;
            }

            // Load audio
            if ( p_audio )
            {

            }

            // Load schedules
            if ( p_schedules )
            {

                // Initialized data
                size_t        schedule_count = 0;
                json_value **pp_elements    = 0;

                // Get the contents of the array
                {

                    // Get the quantity of elements
                    (void)array_get(p_schedules->list, 0, &schedule_count );

                    // Allocate an array for the elements
                    pp_elements = calloc(schedule_count+1, sizeof(json_value *));

                    // Error check
                    if ( pp_elements == (void *) 0 ) goto no_mem;

                    // Populate the elements of the threads
                    (void)array_get(p_schedules->list, (void **)pp_elements, 0 );
                }

                // Set up the schedules
                for (size_t i = 0; i < schedule_count; i++)
                {

                    // Initialized data
                    json_value  *p_element  = pp_elements[i];
                    GXSchedule_t *i_schedule = 0;

                    // Parse the schedule as an object
                    if ( load_schedule_as_json_value(&i_schedule, p_element) == 0 ) goto failed_to_load_schedule;

                    // Add the schedule into the schedule dictionary
                    (void)dict_add(p_instance->data.schedules, i_schedule->name, i_schedule);
                }
            }

            // Load the initial scene
            if ( p_initial_scene )
            {

                // Load the initial scene as a string
                if ( load_scene_as_json_value(&p_instance->context.scene, p_initial_scene) == 0 ) goto failed_to_load_scene;

            }
            // Default
            else
                goto no_initial_scene;

            /*
            // Set up the server
            if (server)
            {
                // Load the server as a JSON object
                if (server[0] == '{')
                    load_server_as_json(&p_instance->networking.server, server, strlen(server));

                // Load the server from the filesystem
                else
                    load_server(&p_instance->networking.server, server);
            }*/

            // This prevents divide by zero errors when the game loop starts
            p_instance->time.delta_time = 0.001f;

            // TODO: Check type of json_value
            p_instance->time.fixed_tick_rate = p_fixed_tick_rate->integer;

            // The instance is running
            p_instance->running = true;
        }

        // 3rd party subsystem integration
        {

            // Discord
            #ifdef BUILD_G10_WITH_DISCORD
                extern void init_discord_integration(void);
                init_discord_integration();
            #endif

            // FMOD
            #ifdef BUILD_G10_WITH_FMOD
                extern void init_fmod_integration(void);
                init_fmod_integration();
            #endif

            // etc.

            // TODO: Steam
            //       Steam Audio
            //       etc
        }

    }

    // Return an instance pointer to the caller
    *pp_instance = p_instance;

    // Success
    return 1;

    missing_vulkan_device_properties:
    wrong_vulkan_device_extension_element_type:
    wrong_window_type:
    failed_to_initialize_sdl2:
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // SDL Errors
        {

            // SDL failed to initialize
            no_sdl2:
                #ifndef NDEBUG
                    g_print_error("[SDL2] Failed to initialize SDL2\n");
                #endif

                // Error
                return 0;

            // SDL Image failed to initialize
            no_sdl2_image:
                #ifndef NDEBUG
                    g_print_error("[SDL Image] Failed to initialize SDL Image\n");
                #endif

                // Error
                return 0;

            // SDL Networking failed to initialize
            no_sdl2_network:
                #ifndef NDEBUG
                    g_print_error("[SDL Net] Failed to initialize SDL Networking\n");
                #endif

                // Error
                return 0;

            // The SDL window failed to initialize
            no_window:
                #ifndef NDEBUG
                    g_print_error("[SDL2] Failed to create a window\nSDL Says: %s\n", SDL_GetError());
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {

            layer_not_present:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Missing validation layers to construct Vulkan instance in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            extension_not_present:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Missing extensions to construct Vulkan instance in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_pick_physical_device:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to pick physical device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // User errors
        {
            no_file:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to open file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[JSON] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Not enough properties to construct instance in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_log_file_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"log file\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_window_width_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"width\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_window_height_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"height\" property of \"window\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_window_title_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"title\" property of \"window\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_window_fullscreen_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"fullscreen\" property of \"window\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"vulkan\" property. Wrong property type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_vulkan_property:
                #ifndef NDEBUG
                    g_print_error("[G10] No \"vulkan\" property provided in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_material_cache_count_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"material count\" property of \"cache\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_part_cache_count_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"part count\" property of \"cache\" property. Wron type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_shader_cache_count_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"shader count\" property of \"cache\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ai_cache_count_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"ai count\" property of \"cache\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_loading_thread_count_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to parse \"loading thread count\" property. Wrong type in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_vulkan_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"vulkan\" is missing properties in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_vulkan_instance_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"instance\" of property \"vulkan\" is missing properties in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"name\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_version_array_len:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"version\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of length 3 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_version_element_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Element in property \"version\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of type [ integer ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_requested_instance_extension_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Element in property \"extensions\" of property \"instance\" of property \"vulkan\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_version_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"version\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"instance\" of property \"vulkan\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_max_buffered_frames_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"max buffered frames\" of property \"vulkan\" must be of type [ integer ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"application\" of property \"instance\" of property \"vulkan\" must be of type [ integer ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_propery_too_long:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"name\" must be less than 256 characters long in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_initialize_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to initialize renderer subsystem in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_initialize_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to initialize shader subsystem in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            failed_to_construct_vulkan_surface:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to construct Vulkan surface in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_vulkan_device:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to construct Vulkan device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_vulkan_swapchain:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to construct swapchain in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;


            failed_to_load_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to open initial scene in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_input:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load input in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_initial_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load initial scene in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
/*
int setup_debug_messenger ( VkDebugUtilsMessengerCreateInfoEXT **debug_messenger_create_info )
{

    // Argument check
    #ifndef NDEBUG
        if ( debug_messenger_create_info == (void *) 0 ) goto failed_to_create_debug_messenger_create_info;
    #endif

    // Initialized data
    VkDebugUtilsMessengerCreateInfoEXT *p_debug_messenger_create_info = calloc(1, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

    // Error check
    if ( p_debug_messenger_create_info == (void *) 0 ) goto no_mem;

    // Populate debug messenger create info struct
    *p_debug_messenger_create_info = (VkDebugUtilsMessengerCreateInfoEXT)
    {
        .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext           = 0,
        .flags           = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debug_callback
    };

    // Return
    *debug_messenger_create_info = p_debug_messenger_create_info;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            failed_to_create_debug_messenger_create_info:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] [Debug Messenger] Null pointer provided for \"debug_messenger_create_info\" in call to function \"%s\"\n", __FUNCTION__);
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

int g_construct_vulkan_instance ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    char                 **required_extensions         = 0,
                         **requested_extensions        = 0,
                         **requested_validation_layers = 0,
                          *application_name            = 0;
    u8                     application_version_major   = 1,
                           application_version_minor   = 0,
                           application_version_patch   = 0;
    u32                    required_extension_count    = 0;
    size_t                 requested_extensions_count  = 0,
                           requested_layers_count      = 0,
                           validation_layers_count     = 0;
    json_value           *p_extensions                = 0,
                          *p_validation_layers         = 0,
                          *p_application               = 0;
    VkResult               result                      = 0;
    VkApplicationInfo      application_info            = { 0 };
    VkInstanceCreateInfo   instance_create_info        = { 0 };
    GXInstance_t          *p_instance                  = g_get_active_instance();

    // Parse the JSON
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_extensions        = dict_get(p_dict, "extensions");
        p_validation_layers = dict_get(p_dict, "validation layers");
        p_application       = dict_get(p_dict, "application");

        // Error check
        if ( ! (
            p_extensions        &&
            p_validation_layers &&
            p_application
        ) )
            goto missing_properties;
    }

    // Construct the vulkan instance
    {

        // Initialized data
        dict *validation_layers = 0,
             *extensions        = 0;

        // Construct a dictionary for the validation layers
        dict_construct(&validation_layers, 32);

        // Parse the application object
        if ( p_application->type == JSON_VALUE_OBJECT )
        {

            // Initialized data
            json_value *p_application_name    = 0,
                        *p_application_version = 0;

            // Parse the JSON
            {

                // Initialized data
                dict *p_dict = p_application->object;

                // Optional properties
                p_application_name    = dict_get(p_dict, "name");
                p_application_version = dict_get(p_dict, "version");
            }

            // Parse the application name
            if ( p_application_name )
            {

                // Parse the value as a string
                if ( p_application_name->type == JSON_VALUE_STRING )
                {
                    // Initialized data
                    size_t app_name_len = strlen(p_application_name->string);

                    // Allocate memory for the application name
                    application_name = calloc(app_name_len+1, sizeof(char));

                    // Error check
                    if ( application_name == (void *) 0 ) goto no_mem;

                    // Copy the string
                    strncpy(application_name, p_application_name->string, app_name_len);
                }
                // Default
                else
                    goto wrong_application_name_type;
            }
            // Default
            else
            {

                // Initialized data
                char   *default_app_name = "G10 Application";
                size_t  default_app_name_len = strlen(default_app_name);

                // Allocate memory for the application name
                application_name = calloc(1+default_app_name_len, sizeof(char));

                // Error check
                if ( application_name == (void *) 0 ) goto no_mem;

                // Copy the string
                strncpy(application_name, default_app_name, default_app_name_len);
            }

            // Parse the application version
            if ( p_application_version )
            {

                // Parse the application version as an array
                if ( p_application_version->type == JSON_VALUE_ARRAY )
                {

                    // Initialized data
                    size_t version_array_len = 0;
                    json_value *pp_applicaiton_version[3] = { 0, 0, 0 };

                    // Dump the array
                    {

                        // Get the quantity of contents
                        array_get(p_application_version->list, 0, &version_array_len);

                        // Error check
                        if ( version_array_len != 3 ) goto wrong_vulkan_instance_application_version_array_len;

                        // Dump the contents
                        array_get(p_application_version->list, (void **)&pp_applicaiton_version, 0);
                    }

                    // Set the major version number
                    if ( pp_applicaiton_version[0]->type == JSON_VALUE_INTEGER )
                        application_version_major = pp_applicaiton_version[0]->integer;
                    // Default
                    else
                        goto wrong_vulkan_instance_application_version_element_type;

                    // Set the minor version number
                    if ( pp_applicaiton_version[1]->type == JSON_VALUE_INTEGER )
                        application_version_minor = pp_applicaiton_version[1]->integer;
                    // Default
                    else
                        goto wrong_vulkan_instance_application_version_element_type;

                    // Set the patch version number
                    if ( pp_applicaiton_version[2]->type == JSON_VALUE_INTEGER )
                        application_version_patch = pp_applicaiton_version[2]->integer;
                    // Default
                    else
                        goto wrong_vulkan_instance_application_version_element_type;

                }
                // Default
                else
                    goto wrong_vulkan_instance_application_version_type;
            }
            // Default
            else
            {

                // v1.0.0
                application_version_major = 1;
                application_version_minor = 0;
                application_version_patch = 0;
            }

            // Populate application info struct
            application_info = (VkApplicationInfo)
            {
                .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext              = 0,
                .pApplicationName   = application_name,
                .applicationVersion = VK_MAKE_VERSION(
                    application_version_major,
                    application_version_minor,
                    application_version_patch
                ),
                .pEngineName        = "G10",
                .engineVersion      = VK_MAKE_VERSION(
                    G10_VERSION_MAJOR,
                    G10_VERSION_MINOR,
                    G10_VERSION_PATCH
                ),
                .apiVersion         = VK_API_VERSION_1_3
            };
        }
        // Default application info struct
        else

            application_info = (VkApplicationInfo)
            {
                .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext              = 0,
                .pApplicationName   = "G10 Application",
                .applicationVersion = VK_MAKE_VERSION(
                    1,
                    0,
                    0
                ),
                .pEngineName        = "G10",
                .engineVersion      = VK_MAKE_VERSION(
                    G10_VERSION_MAJOR,
                    G10_VERSION_MINOR,
                    G10_VERSION_PATCH
                ),
                .apiVersion         = VK_API_VERSION_1_3
            };


        // Get the extensions
        if ( p_extensions )
        {

            // Parse the extensions as an array
            if ( p_extensions->type == JSON_VALUE_ARRAY )
            {

                // Dump the requested instance extensions
                {

                    // Get the number of extensions
                    array_get(p_extensions->list, 0, &requested_extensions_count);

                    // TODO: This should accound for instance extensions required by the WSI (ie, SDL2, GLFW (if I ever implement that))
                    // Allocate for each extension, and a surplus.
                    requested_extensions = calloc(requested_extensions_count+16, sizeof(char *));

                    // Error check
                    if ( requested_extensions == (void *) 0 ) goto no_mem;

                    // Get each extension
                    array_get(p_extensions->list, (void **)requested_extensions, 0);
                }

                // Iterate over each requested extension JSON value, and extract the string
                for (size_t i = 0; i < requested_extensions_count; i++)

                    // json_value * -> char *
                    requested_extensions[i] = ((json_value *)requested_extensions[i])->string;

            }
            else
                goto wrong_requested_instance_extensions_type;
        }
        // Default
        else
            goto wrong_extensions_type;

        // Get the validation layers
        if ( p_validation_layers )
        {

            // Parse the validation layers as an array
            if ( p_validation_layers->type == JSON_VALUE_ARRAY )
            {

                // Get the contents of the array
                {
                    // Get the number requested validation layers
                    array_get(p_validation_layers->list, 0, &requested_layers_count);

                    // Allocate for each extension
                    requested_validation_layers = calloc(requested_layers_count+1, sizeof(json_value *));

                    // Error check
                    if ( requested_validation_layers == (void *) 0 ) goto no_mem;

                    // Get each extension
                    array_get(p_validation_layers->list, (void **)requested_validation_layers, 0);
                }

                // Iterate over each requested extension JSON value, and extract the string
                for (size_t i = 0; i < requested_layers_count; i++)
                    requested_validation_layers[i] = ((json_value *)requested_validation_layers[i])->string;
            }
            // Default
            else
                goto wrong_requested_validation_layers_type;
        }
        // Default
        else
            goto wrong_validation_layers_type;

        // Populate instance create info struct
        {

            // Initialized data
            VkInstanceCreateInfo *t = 0;

            // Get a list of the required instance extensions from SDL
            {

                // Get the quantity of required instance extensions
                SDL_Vulkan_GetInstanceExtensions(p_instance->sdl2.window, (unsigned int *)&required_extension_count, (void*)0);

                // TODO: Free
                // Allocate memory for the extensions
                required_extensions = calloc(required_extension_count + 1, sizeof(char*));

                // Get the required instance extensions
                SDL_Vulkan_GetInstanceExtensions(p_instance->sdl2.window, (unsigned int *)&required_extension_count, (const char **) required_extensions);
            }

            // Construct a dictionary from the required extensions
            dict_from_keys(&extensions, required_extensions, required_extension_count);

            // Dump the validation layers
            //dict_keys(validation_layers, requested_validation_layers);
            //validation_layers_count = dict_keys(validation_layers, 0);

            // TODO: Add support for user defined extensions
            instance_create_info  = (VkInstanceCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext                   = 0,
                .flags                   = 0,
                .pApplicationInfo        = &application_info,
                .enabledLayerCount       = (u32) requested_layers_count,
                .ppEnabledLayerNames     = (const char *const *) requested_validation_layers,
                .enabledExtensionCount   = (u32) requested_extensions_count,
                .ppEnabledExtensionNames = (const char *const *) requested_extensions
            };

            // Get a pointer to the instance create info struct
            t = &instance_create_info;

            // Create the debug messenger
            if ( setup_debug_messenger((VkDebugUtilsMessengerCreateInfoEXT **)&t->pNext) == 0 ) goto failed_to_setup_debug_messenger;
        }

        // Create the vulkan instance
        switch (
            vkCreateInstance(&instance_create_info, (void*)0, &p_instance->vulkan.instance)
        )
        {
            case VK_SUCCESS:
                break;
            case VK_ERROR_LAYER_NOT_PRESENT:
                goto layer_not_present;
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                goto extension_not_present;
            default:
                goto failed_to_create_vulkan_instance;
        }
    }

    // Clean the scope
    free(required_extensions);

    // Success
    return 1;

    // Error (unsorted)
    failed_to_setup_debug_messenger:
    wrong_extensions_type:
    wrong_requested_validation_layers_type:
    wrong_requested_instance_extensions_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Not enough properties to construct instance in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_application_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] \"name\" property of \"application\" property of \"instance\" property of \"vulkan\" property of G10 instance JSON object must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_version_array_len:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"version\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of length 3 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vulkan_instance_application_version_element_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Element in property \"version\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of type [ integer ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            wrong_vulkan_instance_application_version_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"version\" of property \"application\" of property \"instance\" of property \"vulkan\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_validation_layers_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"layers\" of property \"vulkan\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_instance_extensions_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"extensions\" of property \"vulkan\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            layer_not_present:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateDevice\" returned \"VK_ERROR_LAYER_NOT_PRESENT\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            extension_not_present:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateDevice\" returned \"VK_ERROR_EXTENSION_NOT_PRESENT\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_vulkan_instance:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create a Vulkan instance\n");
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

int g_construct_vulkan_surface ( void )
{

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();

    // Create a surface with SDL2 WSI
    if (
        SDL_Vulkan_CreateSurface( 
            p_instance->sdl2.window,
            p_instance->vulkan.instance,
            &p_instance->vulkan.surface
        ) == SDL_FALSE
    )
        goto failed_to_create_sdl2_surface;

    // Success
    return 1;

    // Error handling
    {

        // SDL2 errors
        {
            failed_to_create_sdl2_surface:
                #ifndef NDEBUG
                    g_print_error("[SDL2] Failed to create an SDL surface. SDL Says: %s in call to function \"%s\"\n", SDL_GetError(), __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_construct_vulkan_physical_device ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    u32                device_count         = 0;
    size_t             extensions_array_len = 0;
    VkPhysicalDevice  *devices              = 0;
    json_value       *p_extensions         = 0,
                      *p_device_name        = 0,
                     **pp_extensions_array  = 0;
    GXInstance_t      *p_instance           = g_get_active_instance();

    // Parse the JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_extensions = dict_get(p_dict, "extensions");

        // Optional properties
        p_device_name = dict_get(p_dict, "name");

        // Error check
        if ( ! (
            p_extensions
        ) )
            goto missing_properties;
    }
    // Default
    else
        goto wrong_value_type;

    // Did the user request a device?
    if ( p_device_name )
    {

        // Parse the device name as a string
        if ( p_device_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t len = strlen(p_device_name->string);

            // Allocate memory for the device
            p_instance->vulkan.requested_device_name = calloc(len+1, sizeof(char));

            // Error check
            if ( p_instance->vulkan.requested_device_name == (void *) 0 ) goto no_mem;

            // Copy the string
            strncpy(p_instance->vulkan.requested_device_name, p_device_name->string, len);
        }
        // Default
        else
            goto wrong_name_type;

    }
    // Default
    else
        p_instance->vulkan.requested_device_name = "";

    // Get a list of the available physical devices
    {

        // Get the number of physical devices
        switch (
            vkEnumeratePhysicalDevices(p_instance->vulkan.instance, (unsigned int *)&device_count, (void *) 0)
        )
        {
            case VK_SUCCESS:
            case VK_INCOMPLETE:
                break;
            default:
                goto failed_to_enumerate_physical_devices;
        }

        // Error check
        if ( device_count == 0 ) goto no_devices;

        // Allocate memory for devices list
        devices = calloc(device_count + 1, sizeof(VkPhysicalDevice));

        // Error check
        if ( devices == (void *) 0 ) goto no_mem;

        // Enumerate physical devices
        switch (
            vkEnumeratePhysicalDevices(p_instance->vulkan.instance, (unsigned int *)&device_count, devices)
        )
        {
            case VK_SUCCESS:
            case VK_INCOMPLETE:
                break;
            default:
                goto failed_to_enumerate_physical_devices;
        }
    }

    // Iterate over each physical device
    for (size_t i = 0; i < device_count; i++)
    {

        // Initialized data
        VkPhysicalDevice           i_physical_device = devices[i];
        bool                       has_queues        = false,
                                   has_extensions    = true,
                                   has_swap_chain    = false;
        VkPhysicalDeviceDescriptorBufferPropertiesEXT db = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT,
            .pNext = 0
        };
        VkPhysicalDeviceProperties2 device_properties = {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
            .pNext = &db,
            .properties = { 0 }
        };

        // Get the physical devices properties
        vkGetPhysicalDeviceProperties2(i_physical_device, &device_properties);

        // Check if this is the requested device
        //if ( strcmp(device_properties.deviceName, p_instance->vulkan.requested_device_name) == 0 )
        //    goto found_requested_device;

        // TODO:

        found_requested_device:

        // Check queues
        {

            // Initialized data
            u32                      queue_family_count                   = 0,
                                    *maybe_graphics_family                = 0,
                                    *maybe_async_compute_family           = 0,
                                    *maybe_transfer_family                = 0,
                                     maybe_graphics_family_count          = 0,
                                     maybe_async_compute_family_count     = 0,
                                     maybe_transfer_family_count          = 0,
                                    *certainly_graphics_family            = 0,
                                    *certainly_async_compute_family       = 0,
                                    *certainly_transfer_family            = 0,
                                     certainly_graphics_family_count      = 0,
                                     certainly_async_compute_family_count = 0,
                                     certainly_transfer_family_count      = 0;
            VkQueueFamilyProperties *queue_families                       = 0;

            // Get the devices queue families
            {

                // Get the number of queue family properties
                vkGetPhysicalDeviceQueueFamilyProperties(i_physical_device, (unsigned int *)&queue_family_count, (void*)0);

                // Allocate memory for the queue family properties
                queue_families = calloc(queue_family_count + 1, sizeof(VkQueueFamilyProperties));

                // Error check
                if ( queue_families == (void *) 0 ) goto no_mem;

                // Get the queue family properties
                vkGetPhysicalDeviceQueueFamilyProperties(i_physical_device, (unsigned int *)&queue_family_count, queue_families);
            }

            // Allocate memory for queue family lists
            {
                maybe_graphics_family          = calloc(queue_family_count, sizeof(u32)),
                maybe_async_compute_family     = calloc(queue_family_count, sizeof(u32)),
                maybe_transfer_family          = calloc(queue_family_count, sizeof(u32)),
                certainly_graphics_family      = calloc(queue_family_count, sizeof(u32)),
                certainly_async_compute_family = calloc(queue_family_count, sizeof(u32)),
                certainly_transfer_family      = calloc(queue_family_count, sizeof(u32));

                // Error check
                if ( maybe_graphics_family          == 0 ) goto no_mem;
                if ( maybe_async_compute_family     == 0 ) goto no_mem;
                if ( maybe_transfer_family          == 0 ) goto no_mem;
                if ( certainly_graphics_family      == 0 ) goto no_mem;
                if ( certainly_async_compute_family == 0 ) goto no_mem;
                if ( certainly_transfer_family      == 0 ) goto no_mem;
            }

            // Iterate over each queue family property
            for (size_t i = 0; i < queue_family_count; i++)
            {

                // Is this a graphics family?
                if (
                    queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && // The queue family supports graphics operations
                    queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT    // The queue family supports transfer operations
                )
                    maybe_graphics_family[maybe_graphics_family_count] = i,
                    maybe_graphics_family_count++;

                // Is this an async compute family?
                if (
                    queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT  && // The queue family supports compute operations
                    queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT    // The queue family supports transfer operations
                )
                    maybe_async_compute_family[maybe_async_compute_family_count] = i,
                    maybe_async_compute_family_count++;

                // Is this a transfer family?
                if (
                    queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT    // The queue family supports transfer operations
                )
                    maybe_transfer_family[maybe_transfer_family_count] = i,
                    maybe_transfer_family_count++;
            }

            // Process the candidate graphics queue(s)
            {

                // Error check
                if ( maybe_graphics_family_count == 0 ) goto no_graphics_family;

                // Iterate over each graphics queue candidate
                for (size_t i = 0; i < maybe_graphics_family_count; i++)
                {

                    // Initialized data
                    size_t j = maybe_graphics_family[i];
                    VkBool32 present_support = 0;

                    // Get the present support for the current queue family
                    vkGetPhysicalDeviceSurfaceSupportKHR(i_physical_device, (u32)i, p_instance->vulkan.surface, &present_support);

                    // Skip this graphics queue if it doesn't support present operations
                    if ( present_support == VK_FALSE )
                        continue;

                    // Store the graphics queue family
                    certainly_graphics_family[certainly_graphics_family_count] = j;

                    // Increment the counter
                    certainly_graphics_family_count++;
                }

                // Copy the queues to the instance
                {

                    // Set the family count
                    p_instance->vulkan.queues.graphics_family_count = certainly_graphics_family_count;

                    // Allocate memory for queue family indicies
                    p_instance->vulkan.queues.graphics_family_indicies = calloc(certainly_graphics_family_count, sizeof(u32));

                    // Copy the indicies
                    memcpy(p_instance->vulkan.queues.graphics_family_indicies, certainly_graphics_family, certainly_graphics_family_count * sizeof(u32));

                    // Set the present queue index
                    p_instance->vulkan.queues.present_family_index = p_instance->vulkan.queues.graphics_family_indicies[0];
                }
            }

            // Process the candidate async compute queue(s)
            {

                // Error check
                if ( maybe_async_compute_family_count == 0 ) goto no_compute_family;

                // Iterate over each async compute queue candidate
                for (size_t i = 0; i < maybe_async_compute_family_count; i++)
                {

                    // Initialized data
                    size_t j = maybe_async_compute_family[i];

                    // Are there more queues to process?
                    if ( i != maybe_async_compute_family_count-1 )
                    {

                        // Does this compute queue support graphics operations?
                        if ( queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT )

                            // Skip it. There will be another async compute queue family
                            continue;
                    }

                    // Store the async compute queue family
                    certainly_async_compute_family[certainly_async_compute_family_count] = j;

                    // Increment the counter
                    certainly_async_compute_family_count++;
                }

                // Copy the queues to the instance
                {

                    // Set the family count
                    p_instance->vulkan.queues.compute_family_count  = certainly_async_compute_family_count;

                    // Allocate memory for queue family indicies
                    p_instance->vulkan.queues.compute_family_indicies  = calloc(certainly_async_compute_family_count, sizeof(u32));

                    // Copy the indicies
                    memcpy(p_instance->vulkan.queues.compute_family_indicies , certainly_async_compute_family, certainly_async_compute_family_count * sizeof(u32));
                }
            }

            // Process the candidate transfer queue(s)
            {

                // Error check
                if ( maybe_transfer_family_count == 0 ) goto no_transfer_family;

                // Iterate over each async compute queue candidate
                for (size_t i = 0; i < maybe_transfer_family_count; i++)
                {

                    // Initialized data
                    size_t j = maybe_transfer_family[i];

                    // Are there more queues to process?
                    if ( i != maybe_transfer_family_count-1 )
                    {

                        // Does this compute queue support graphics operations?
                        if (
                            queue_families[j].queueFlags & VK_QUEUE_GRAPHICS_BIT ||
                            queue_families[j].queueFlags & VK_QUEUE_COMPUTE_BIT
                        )

                            // Skip it. There will be another async compute queue family
                            continue;
                    }

                    // Store the async compute queue family
                    certainly_transfer_family[certainly_transfer_family_count] = j;

                    // Increment the counter
                    certainly_transfer_family_count++;
                }

                // Copy the queues to the instance
                {

                    // Set the transfer family count
                    p_instance->vulkan.queues.transfer_family_count = certainly_transfer_family_count;

                    // Allocate memory for transfer queue family indicies
                    p_instance->vulkan.queues.transfer_family_indicies = calloc(certainly_transfer_family_count, sizeof(u32));

                    // Copy the indicies
                    memcpy(p_instance->vulkan.queues.transfer_family_indicies, certainly_transfer_family, certainly_transfer_family_count * sizeof(u32));
                }
            }

            // Clean the scope
            free(maybe_graphics_family);
            free(maybe_async_compute_family);
            free(maybe_transfer_family);
            free(certainly_graphics_family);
            free(certainly_async_compute_family);
            free(certainly_transfer_family);
        }

        // Check device extensions
        if ( p_extensions->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            u32                     device_extension_count           = 0;
            VkExtensionProperties  *available_device_extensions      = 0;
            dict                   *device_extension_name_dict       = 0,
                                   *available_device_extensions_lut  = 0;
            char                  **available_device_extension_names = 0;

            // Get the quantity of device extension properties
            switch (
                vkEnumerateDeviceExtensionProperties(i_physical_device, (void*)0, (unsigned int *)&device_extension_count, (void*)0)
            )
            {
                case VK_SUCCESS:
                case VK_INCOMPLETE:
                    break;
                default:
                    goto failed_to_enumerate_device_extension_properties;
            }

            // Allocate a list of available device extensions
            available_device_extensions = calloc(device_extension_count + 1, sizeof(VkExtensionProperties));

            // Error check
            if ( available_device_extensions == (void *) 0 ) goto no_mem;

            // Get the device extension properties
            switch (
                vkEnumerateDeviceExtensionProperties(i_physical_device, (void*)0, (unsigned int *)&device_extension_count, available_device_extensions)
            )
            {
                case VK_SUCCESS:
                case VK_INCOMPLETE:
                    break;
                default:
                    goto failed_to_enumerate_device_extension_properties;
            }

            // Construct a dictionary from the available device extensions
            dict_construct(&p_instance->vulkan.device_extensions, device_extension_count);

            // Construct an extension lookup table
            for (size_t i = 0; i < device_extension_count; i++)
                dict_add(p_instance->vulkan.device_extensions, available_device_extensions[i].extensionName, (void *)1);
        }

        // Get the surface capabilities
        switch (
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(i_physical_device, p_instance->vulkan.surface, &p_instance->vulkan.swap_chain.details.capabilities)
        )
        {
            case VK_INCOMPLETE:
                g_print_warning("[G10] Unable to get some surface capabilities in call to function \"%s\"\n", __FUNCTION__);
            case VK_SUCCESS:
                break;
            default:
                goto failed_to_get_surface_capabilities;
        }

        // Get the surface formats
        {

            // Initialized data
            u32 format_count = 0;

            // Get the quantity of surface formats
            switch (
                vkGetPhysicalDeviceSurfaceFormatsKHR(i_physical_device, p_instance->vulkan.surface, (unsigned int *)&format_count, (void*)0)
            )
            {
                case VK_INCOMPLETE:
                case VK_SUCCESS:
                    break;
                default:
                    goto failed_to_get_surface_formats;
            }

            // Store the surface formats quantity
            p_instance->vulkan.swap_chain.details.format_count = format_count;

            // Allocate memory for surface formats
            p_instance->vulkan.swap_chain.details.formats = calloc(format_count + 1, sizeof(VkSurfaceFormatKHR));

            // Error check
            if ( p_instance->vulkan.swap_chain.details.formats == (void *) 0 ) goto no_mem;

            // Get the surface formats
            switch (
                vkGetPhysicalDeviceSurfaceFormatsKHR(i_physical_device, p_instance->vulkan.surface, (unsigned int *)&format_count, p_instance->vulkan.swap_chain.details.formats)
            )
            {
                case VK_INCOMPLETE:
                    g_print_warning("[G10] Unable to get some surface formats in call to function \"%s\"\n", __FUNCTION__);
                case VK_SUCCESS:
                    break;
                default:
                    goto failed_to_get_surface_formats;
            }
        }

        // Get the surface presentation modes
        {

            // Initialized data
            u32 present_mode_count = 0;

            // Get the quantity of surface presentation modes
            switch (
                vkGetPhysicalDeviceSurfacePresentModesKHR(i_physical_device, p_instance->vulkan.surface, (unsigned int *)&present_mode_count, (void *)0)
            )
            {
                case VK_INCOMPLETE:
                case VK_SUCCESS:
                    break;
                default:
                    goto failed_to_get_presentation_modes;
            }

            // Allocate memory for presentation modes
            p_instance->vulkan.swap_chain.details.present_modes = calloc(present_mode_count + 1, sizeof(VkPresentModeKHR));

            // Error check
            if ( p_instance->vulkan.swap_chain.details.present_modes == (void *) 0 ) goto no_mem;

            // Get the surface presentation modes
            switch (
                vkGetPhysicalDeviceSurfacePresentModesKHR(i_physical_device, p_instance->vulkan.surface,(unsigned int *)&present_mode_count, p_instance->vulkan.swap_chain.details.present_modes)
            )
            {
                case VK_INCOMPLETE:
                case VK_SUCCESS:
                    break;
                default:
                    goto failed_to_get_presentation_modes;
            }

            //if ( ( (bool) present_mode_count ) & ( (bool) format_count ) )
            //    has_swap_chain = true;
        }

        // Success
        p_instance->vulkan.physical_device = devices[i];
        break;

        free(p_instance->vulkan.queues.transfer_family_indicies);
        no_transfer_family:
        free(p_instance->vulkan.queues.compute_family_indicies);
        no_compute_family:
        free(p_instance->vulkan.queues.graphics_family_indicies);
        no_graphics_family:
        continue;
    }

    // Error check
    if ( p_instance->vulkan.physical_device == VK_NULL_HANDLE ) goto failed_to_find_physical_device;

    // Clean the scope
    free(devices);

    // Success
    return 1;

    failed_to_enumerate_physical_devices:
    wrong_vulkan_device_extension_element_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Not enough properties to construct physical device in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Vulkan errors
        {
            no_devices:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to find any devices in call to function \"%s\"\nYour computer does not support Vulkan\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_enumerate_device_extension_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Call to function \"vkEnumerateDeviceExtensionProperties\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_get_surface_capabilities:
                #ifndef NDEBUG
                    g_print_error("[G10] Call to function \"vkGetPhysicalDeviceSurfaceCapabilitiesKHR\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_logical_device:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create logical device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_find_physical_device:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to find a suitable physical device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_get_presentation_modes:
                #ifndef NDEBUG
                    g_print_error("[G10] Call to function \"vkGetPhysicalDeviceSurfacePresentModesKHR\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_get_surface_formats:
                #ifndef NDEBUG
                    g_print_error("[G10] Call to function \"vkGetPhysicalDeviceSurfaceFormatsKHR\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

int g_construct_vulkan_logical_device ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance     = g_get_active_instance();
    json_value  *p_extensions   = 0,
                 *p_features     = 0,
                 *p_queue_counts = 0;

    // Parse the JSON
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_extensions   = dict_get(p_dict, "extensions");
        p_features     = dict_get(p_dict, "features");
        p_queue_counts = dict_get(p_dict, "queue counts");

        // Error check
        if ( ! (
            p_extensions   &&
            p_features     &&
            p_queue_counts
        ) )
            goto missing_properties;
    }

    // Construct the device
    {

        // Initialized data
        size_t queue_create_info_count       = 0,
               enabled_extension_names_count = 0,
               queue_iter                    = 0;
               
        VkDeviceCreateInfo device_create_info = { 0 };
        VkDeviceQueueCreateInfo *queue_create_infos = 0;
        char **enabled_extension_names = 0;

        VkPhysicalDeviceSynchronization2Features synchronization2_features =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
            .pNext = 0,
            .synchronization2 = VK_TRUE
        };
        VkPhysicalDeviceMaintenance4Features maintainance4_features =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_4_FEATURES,
            .pNext = &synchronization2_features,
            .maintenance4 = VK_TRUE
        };
        VkPhysicalDeviceRobustness2FeaturesEXT robustness_2_features =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT,
            .pNext = &maintainance4_features,
            .robustBufferAccess2 = VK_TRUE,
            .robustImageAccess2  = VK_TRUE,
            .nullDescriptor      = VK_TRUE
        };
        VkPhysicalDeviceMeshShaderFeaturesEXT mesh_shader_features =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT,
            .pNext = &robustness_2_features,
            .taskShader                             = VK_TRUE,
            .meshShader                             = VK_TRUE,
            .multiviewMeshShader                    = VK_FALSE,
            .primitiveFragmentShadingRateMeshShader = VK_FALSE,
            .meshShaderQueries                      = VK_FALSE,
        };
        VkPhysicalDeviceDescriptorBufferFeaturesEXT descriptor_buffer_features =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_FEATURES_EXT,
            .pNext = &mesh_shader_features,
            .descriptorBuffer                   = VK_TRUE,
            .descriptorBufferCaptureReplay      = VK_FALSE,
            .descriptorBufferImageLayoutIgnored = VK_FALSE,
            .descriptorBufferPushDescriptors    = VK_FALSE,
        };
        VkPhysicalDeviceFeatures2 device_features2 =
        {
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
            .pNext = &descriptor_buffer_features,
            .features = {
                .robustBufferAccess                      = VK_TRUE,
                .fullDrawIndexUint32                     = VK_TRUE,
                .imageCubeArray                          = VK_TRUE,
                .independentBlend                        = VK_TRUE,
                .geometryShader                          = VK_TRUE,
                .tessellationShader                      = VK_TRUE,
                .sampleRateShading                       = VK_TRUE,
                .dualSrcBlend                            = VK_TRUE,
                .logicOp                                 = VK_TRUE,
                .multiDrawIndirect                       = VK_TRUE,
                .drawIndirectFirstInstance               = VK_TRUE,
                .depthClamp                              = VK_TRUE,
                .depthBiasClamp                          = VK_TRUE,
                .fillModeNonSolid                        = VK_TRUE,
                .depthBounds                             = VK_TRUE,
                .wideLines                               = VK_TRUE,
                .largePoints                             = VK_TRUE,
                .alphaToOne                              = VK_TRUE,
                .multiViewport                           = VK_TRUE,
                .samplerAnisotropy                       = VK_TRUE,
                .textureCompressionETC2                  = VK_FALSE,
                .textureCompressionASTC_LDR              = VK_FALSE,
                .textureCompressionBC                    = VK_TRUE,
                .occlusionQueryPrecise                   = VK_TRUE,
                .pipelineStatisticsQuery                 = VK_TRUE,
                .vertexPipelineStoresAndAtomics          = VK_TRUE,
                .fragmentStoresAndAtomics                = VK_TRUE,
                .shaderTessellationAndGeometryPointSize  = VK_TRUE,
                .shaderImageGatherExtended               = VK_TRUE,
                .shaderStorageImageExtendedFormats       = VK_TRUE,
                .shaderStorageImageMultisample           = VK_TRUE,
                .shaderStorageImageReadWithoutFormat     = VK_TRUE,
                .shaderStorageImageWriteWithoutFormat    = VK_TRUE,
                .shaderUniformBufferArrayDynamicIndexing = VK_TRUE,
                .shaderSampledImageArrayDynamicIndexing  = VK_TRUE,
                .shaderStorageBufferArrayDynamicIndexing = VK_TRUE,
                .shaderStorageImageArrayDynamicIndexing  = VK_TRUE,
                .shaderClipDistance                      = VK_TRUE,
                .shaderCullDistance                      = VK_TRUE,
                .shaderFloat64                           = VK_TRUE,
                .shaderInt64                             = VK_TRUE,
                .shaderInt16                             = VK_TRUE,
                .shaderResourceResidency                 = VK_TRUE,
                .shaderResourceMinLod                    = VK_TRUE,
                .sparseBinding                           = VK_TRUE,
                .sparseResidencyBuffer                   = VK_TRUE,
                .sparseResidencyImage2D                  = VK_TRUE,
                .sparseResidencyImage3D                  = VK_TRUE,
                .sparseResidency2Samples                 = VK_TRUE,
                .sparseResidency4Samples                 = VK_TRUE,
                .sparseResidency8Samples                 = VK_TRUE,
                .sparseResidency16Samples                = VK_TRUE,
                .sparseResidencyAliased                  = VK_TRUE,
                .variableMultisampleRate                 = VK_TRUE,
                .inheritedQueries                        = VK_TRUE
            }
        };

        // Get the physical device features
        vkGetPhysicalDeviceFeatures2(p_instance->vulkan.physical_device, &device_features2);

        // Construct queue create infos
        {

            // Initialized data
            json_value *p_graphics                   = 0,
                        *p_compute                    = 0,
                        *p_transfer                   = 0;
            size_t       graphics_queue_count         = 0,
                         compute_queue_count          = 0,
                         transfer_queue_count         = 0;
            bool         has_required_transfer_queues = false;

            // Parse the JSON
            if ( p_queue_counts->type == JSON_VALUE_OBJECT )
            {

                // Initialized data
                dict *p_dict = p_queue_counts->object;

                // Required properties
                p_graphics = dict_get(p_dict, "graphics");
                p_compute  = dict_get(p_dict, "compute");
                p_transfer = dict_get(p_dict, "transfer");

                // Error check
                if ( ! (
                    p_graphics &&
                    p_compute  &&
                    p_transfer
                ) )
                    goto missing_properties;
            }
            // Default
            else
                goto wrong_queue_count_type;

            for (size_t i = 0; i < 8; i++)
                p_instance->vulkan.queues.priority[i] = 1.f;
            

            // Count queues
            {

                // Count graphics queues
                if ( p_graphics->type == JSON_VALUE_INTEGER )
                    graphics_queue_count += p_graphics->integer;
                else
                    goto wrong_graphics_type;

                // Count compute queues
                if ( p_compute->type == JSON_VALUE_INTEGER )
                    compute_queue_count += p_compute->integer;
                else
                    goto wrong_compute_type;

                // Count transfer queues
                if ( p_transfer->type == JSON_VALUE_INTEGER )
                    transfer_queue_count += p_transfer->integer;
                else
                    goto wrong_transfer_type;

                // Sum
                queue_create_info_count += graphics_queue_count + compute_queue_count + transfer_queue_count;

                // Store the graphics queue count
                p_instance->vulkan.queues.graphics_count = graphics_queue_count;

                // Store the compute queue count
                p_instance->vulkan.queues.compute_count = compute_queue_count;

                // Store the compute queue count
                p_instance->vulkan.queues.transfer_count = transfer_queue_count;
            }

            // Allocate memory for queues
            {

                // Allocate memory for graphics queues
                p_instance->vulkan.queues.graphics_queues = calloc(graphics_queue_count, sizeof(VkQueue));

                // Allocate memory for async compute queues
                p_instance->vulkan.queues.compute_queues = calloc(compute_queue_count, sizeof(VkQueue));

                // Allocate memory for transfer queues
                p_instance->vulkan.queues.transfer_queues = calloc(transfer_queue_count, sizeof(VkQueue));

                // Error check
                if ( p_instance->vulkan.queues.graphics_queues == (void *) 0 ) goto no_mem;
                if ( p_instance->vulkan.queues.compute_queues  == (void *) 0 ) goto no_mem;
                if ( p_instance->vulkan.queues.transfer_queues == (void *) 0 ) goto no_mem;

                // Allocate memory for queue create infos
                queue_create_infos = calloc(queue_create_info_count, sizeof(VkDeviceQueueCreateInfo));

                // Error check
                if ( queue_create_infos == (void *) 0 ) goto no_mem;
            }

            // Populate graphics queue create infos
            for (size_t i = 0; i < p_instance->vulkan.queues.graphics_family_count; i++)
            {

                // Initialized data
                VkDeviceQueueCreateInfo *i_queue_create_info = &queue_create_infos[queue_iter];

                // Increment the queue iterator
                queue_iter++;

                // Populate the queue create info struct
                *i_queue_create_info = (VkDeviceQueueCreateInfo)
                {
                    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext            = 0, // VkDeviceQueueGlobalPriorityCreateInfoKHR
                    .flags            = 0,
                    .queueFamilyIndex = p_instance->vulkan.queues.graphics_family_indicies[i],
                    .queueCount       = graphics_queue_count,
                    .pQueuePriorities = &p_instance->vulkan.queues.priority,
                };
            }

            // Populate compute queue create infos
            for (size_t i = 0; i < p_instance->vulkan.queues.compute_family_count; i++)
            {

                // Initialized data
                VkDeviceQueueCreateInfo *i_queue_create_info = &queue_create_infos[queue_iter];

                // Increment the queue iterator
                queue_iter++;

                // Populate the queue create info struct
                *i_queue_create_info = (VkDeviceQueueCreateInfo)
                {
                    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext            = 0, // VkDeviceQueueGlobalPriorityCreateInfoKHR
                    .flags            = 0,
                    .queueFamilyIndex = p_instance->vulkan.queues.compute_family_indicies[i],
                    .queueCount       = compute_queue_count,
                    .pQueuePriorities = &p_instance->vulkan.queues.priority,
                };
            }

            // Populate transfer queue create infos
            for (size_t i = 0; i < p_instance->vulkan.queues.transfer_family_count; i++)
            {
                
                // State check
                if ( has_required_transfer_queues )
                    continue;

                // Initialized data
                VkDeviceQueueCreateInfo *i_queue_create_info = &queue_create_infos[queue_iter];

                // Increment the queue iterator
                queue_iter++;

                // Populate the queue create info struct
                *i_queue_create_info = (VkDeviceQueueCreateInfo)
                {
                    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .pNext            = 0, // VkDeviceQueueGlobalPriorityCreateInfoKHR
                    .flags            = 0,
                    .queueFamilyIndex = p_instance->vulkan.queues.transfer_family_indicies[i],
                    .queueCount       = transfer_queue_count,
                    .pQueuePriorities = &p_instance->vulkan.queues.priority,
                };

                has_required_transfer_queues = true;
            }
        }

        // Get the device extensions
        {

            // Dump the array
            {

                // Get the size of the array
                array_get(p_extensions->list, 0, &enabled_extension_names_count);

                // Allocate memory for extensions
                enabled_extension_names = calloc(enabled_extension_names_count+1, sizeof(char *));

                // Error check
                if ( enabled_extension_names == (void *) 0 ) goto no_mem;

                // Get the contents of the array
                array_get(p_extensions->list, (void **) enabled_extension_names, 0);
            }

            // Iterate over each json_value *
            for (size_t i = 0; i < enabled_extension_names_count; i++)
            {

                // json_value * -> char *
                enabled_extension_names[i] = ((json_value *)enabled_extension_names[i])->string;

                // Does the device support the extension?
                if ( dict_get(p_instance->vulkan.device_extensions, enabled_extension_names[i]) == 0 ) goto missing_extensions;

            }
        }

        // Populate the device create info struct
        device_create_info = (VkDeviceCreateInfo)
        {
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = &descriptor_buffer_features, 
            .flags                   = 0, // Reserved for future use
            .queueCreateInfoCount    = queue_iter,
            .pQueueCreateInfos       = queue_create_infos,
            .enabledLayerCount       = 0, // DEPRICATED AND IGNORED AS OF 1.3
            .ppEnabledLayerNames     = 0, // DEPRICATED AND IGNORED AS OF 1.3
            .enabledExtensionCount   = enabled_extension_names_count,
            .ppEnabledExtensionNames = (const char *const *) enabled_extension_names,
            .pEnabledFeatures        = 0 // Included in pNext chain
        };

        // Create the logical device
        switch (
            vkCreateDevice(p_instance->vulkan.physical_device, &device_create_info, 0, &p_instance->vulkan.device)
        )
        {
            case VK_SUCCESS:
                break;
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                goto extensions_not_present;
            case VK_ERROR_FEATURE_NOT_PRESENT:
                goto features_not_present;
            default:
                goto failed_to_create_device;
        }
    }

    // Get the graphics queues
    for (size_t i = 0; i < p_instance->vulkan.queues.graphics_family_count; i++)
        for (size_t j = 0; j < p_instance->vulkan.queues.graphics_count; j++)

            // Get the device queue
            vkGetDeviceQueue(p_instance->vulkan.device, p_instance->vulkan.queues.graphics_family_indicies[i], j, &p_instance->vulkan.queues.graphics_queues[j]);

    // Get the compute queues
    for (size_t i = 0; i < p_instance->vulkan.queues.compute_family_count; i++)
        for (size_t j = 0; j < p_instance->vulkan.queues.compute_count; j++)

            // Get the device queue
            vkGetDeviceQueue(p_instance->vulkan.device, p_instance->vulkan.queues.compute_family_indicies[i], j, &p_instance->vulkan.queues.compute_queues[j]);
        
    // Get the transfer queues
    for (size_t i = 0; i < 1; i++)
        for (size_t j = 0; j < p_instance->vulkan.queues.transfer_count; j++)

            // Get the device queue
            vkGetDeviceQueue(p_instance->vulkan.device, p_instance->vulkan.queues.transfer_family_indicies[i], j, &p_instance->vulkan.queues.transfer_queues[j]);

    // Set the presentation queue
    p_instance->vulkan.queues.present_queue = p_instance->vulkan.queues.graphics_queues[0];

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_queue_count_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"queue counts\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_graphics_type:
                #ifndef NDEBUG
                    g_print_error("[G10] \"graphics\" property of \"queue counts\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_compute_type:
                #ifndef NDEBUG
                    g_print_error("[G10] \"compute\" property of \"queue counts\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_transfer_type:
                #ifndef NDEBUG
                    g_print_error("[G10] \"transfer\" property of \"queue counts\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Vulkan errors
        {
            failed_to_create_logical_device:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create logical device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_extensions:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Missing specified device extensions in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Missing specified device properties in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            extensions_not_present:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateDevice\" returned a value indicating that not all extensions were present in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            features_not_present:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateDevice\" returned a value indicating that not all features were present in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_device:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateDevice\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

int g_construct_vulkan_swap_chain ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    json_value  *p_present_mode = 0,
                 *p_format       = 0;
    VkFormat      image_format   = p_instance->vulkan.swap_chain.image_format;
    VkExtent2D    image_extent   = { 0 };

    // Parse the JSON
    if ( p_value->type == JSON_VALUE_OBJECT)
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Optional properties
        p_present_mode = dict_get(p_dict, "present mode");
        p_format       = dict_get(p_dict, "format");

    }
    // Default
    else
        goto wrong_value_type;

    // Construct the swapchain
    {

        // Initialized data
        u32                            min_image_count            = 0;
        VkSurfaceKHR                   surface                    = p_instance->vulkan.surface;
        VkSharingMode                  image_sharing_mode         = 0;
        VkColorSpaceKHR                image_color_space          = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        VkSurfaceTransformFlagBitsKHR  pre_transform              = p_instance->vulkan.swap_chain.details.capabilities.currentTransform;
        VkSwapchainCreateInfoKHR       swap_chain_create_info_khr = { 0 };
        VkPresentModeKHR               present_mode               = 0;

        // Parse the presentation mode
        if ( p_present_mode )
        {

            // External data
            extern dict *presentation_modes_lut;

            // Parse the provided presentation mode
            if ( p_present_mode->type == JSON_VALUE_STRING )
                present_mode = (VkPresentModeKHR) (size_t) dict_get(presentation_modes_lut, p_present_mode->string);
            // Default
            else
                goto wrong_present_mode_type;

        }
        // Default to Mailbox.
        else
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;

        // Parse the format
        if ( p_format )
        {

            // External data
            extern dict *format_enumeration_lookup;

            // Parse the provided format
            if ( p_format->type == JSON_VALUE_STRING )
                image_format = (VkFormat) (size_t) dict_get(format_enumeration_lookup, p_format->string);
            // Default
            else
                goto wrong_format_type;

        }
        // Default to Mailbox.
        else
            image_format = VK_FORMAT_B8G8R8A8_SRGB;

        // Get the extent
        {

            // Get the framebuffer size
            SDL_Vulkan_GetDrawableSize(p_instance->sdl2.window, &p_instance->vulkan.swap_chain.extent.width, &p_instance->vulkan.swap_chain.extent.height);

            // Set the framebuffer size
            image_extent = p_instance->vulkan.swap_chain.extent;
        }

        // Set the image count
        p_instance->vulkan.image_count = p_instance->vulkan.max_buffered_frames;

        // Set the minimum image count
        min_image_count = p_instance->vulkan.image_count;

        // Populate the swapchain create info struct
        swap_chain_create_info_khr =
        (VkSwapchainCreateInfoKHR)
        {
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = 0,
            .flags                 = 0,
            .surface               = surface,
            .minImageCount         = min_image_count,
            .imageFormat           = image_format,
            .imageColorSpace       = image_color_space,
            .imageExtent           = image_extent,
            .imageArrayLayers      = 1,
            .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode      = image_sharing_mode,
            .queueFamilyIndexCount = 0, // Optional : 1,
            .pQueueFamilyIndices   = 0, // Optional : (const u32*) p_instance->vulkan.queues.graphics_family_indicies,
            .preTransform          = pre_transform,
            .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode           = present_mode,
            .clipped               = VK_TRUE,
            .oldSwapchain          = VK_NULL_HANDLE
        };

        // Construct the swapchain
        if (
            vkCreateSwapchainKHR(
                p_instance->vulkan.device,
                &swap_chain_create_info_khr,
                0,
                &p_instance->vulkan.swap_chain.swap_chain
            ) != VK_SUCCESS
        )
            goto failed_to_construct_swapchain_khr;

        // Get the quantity of swap chain images
        switch (
            vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (unsigned int *)&p_instance->vulkan.image_count, 0)
        )
        {
            case VK_INCOMPLETE:
                g_print_warning("[Vulkan] Failed to get all swapchain images calling \"vkGetSwapchainImagesKHR\" in call to function \"%s\"\n", __FUNCTION__);
            case VK_SUCCESS:
                break;
            default:
                goto failed_to_get_swapchain_images;
        }

        // Allocate memory for swap chain images
        p_instance->vulkan.swap_chain.images = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkImage));

        // Error check
        if ( p_instance->vulkan.swap_chain.images == (void *) 0 ) goto no_mem;

        // Get the swap chain images
        switch (
            vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (unsigned int *)&p_instance->vulkan.image_count, 0)
        )
        {
            case VK_INCOMPLETE:
                g_print_warning("[Vulkan] Failed to get all swapchain images calling \"vkGetSwapchainImagesKHR\" in call to function \"%s\"\n", __FUNCTION__);
            case VK_SUCCESS:
                break;
            default:
                goto failed_to_get_swapchain_images;
        }

    }

    // Get the swap chain images
    {

        // Get the quantity of swap chain images
        vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (unsigned int *)&p_instance->vulkan.image_count, 0);

        // Allocate memory for image list
        p_instance->vulkan.swap_chain.images = calloc(p_instance->vulkan.image_count, sizeof(VkImage));

        // Error check
        if ( p_instance->vulkan.swap_chain.images == (void *) 0 ) goto no_mem;

        // Get the swap chain images
        vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (unsigned int *)&p_instance->vulkan.image_count, p_instance->vulkan.swap_chain.images);

        // Set the swap chain image format
        p_instance->vulkan.swap_chain.image_format = image_format;

        // Set the framebuffer dimensions
        p_instance->vulkan.swap_chain.extent = image_extent;
    }

    // Construct swap chain image views
    {

        // Allocate memory for swap chain images
        p_instance->vulkan.swap_chain.image_views = calloc(p_instance->vulkan.image_count, sizeof(VkImageView));

        // Error check
        if ( p_instance->vulkan.swap_chain.image_views == (void *) 0 ) goto no_mem;
        
        // Iterate over each image
        for (size_t i = 0; i < p_instance->vulkan.image_count; i++)
        {

            // Initialized data
            VkImageViewCreateInfo  image_view_create_info =
            {
                .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext                           = 0,
                .flags                           = 0,
                .image                           = p_instance->vulkan.swap_chain.images[i],
                .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
                .format                          = p_instance->vulkan.swap_chain.image_format,
                .components                      =
                {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY
                },
                .subresourceRange =
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = 1,
                    .baseArrayLayer = 0,
                    .layerCount     = 1,
                }
            };

            // Create the image view
            if (
                vkCreateImageView(
                    p_instance->vulkan.device,
                    &image_view_create_info,
                    0,
                    &p_instance->vulkan.swap_chain.image_views[i]
                ) != VK_SUCCESS
            )
                goto failed_to_create_image_view;
        }
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/instance.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_format_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"format\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_present_mode_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Property \"present mode\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_construct_swapchain_khr:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateSwapchainKHR\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_get_swapchain_images:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkGetSwapchainImagesKHR\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_image_view:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateImageView\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
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

int g_construct_vulkan_image_views ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_construct_vulkan_command_pool ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_construct_vulkan_command_buffers ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_construct_vulkan_sync_objects ( json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void create_swap_chain ( void )
{

    // Uninitialized data
    VkSurfaceFormatKHR        surface_format;
    VkExtent2D                extent;

    // Initialized data
    GXInstance_t             *p_instance               = g_get_active_instance();
    VkSwapchainCreateInfoKHR  swapchain_create_info  = { 0 };
    VkPresentModeKHR          present_mode           = VK_PRESENT_MODE_MAILBOX_KHR;

    u32                       qfi[2]                 = { 0, 0 };

    qfi[0] = p_instance->vulkan.queue_family_indices.g;
    qfi[1] = p_instance->vulkan.queue_family_indices.p;

    surface_format = p_instance->vulkan.swap_chain.details.formats[0];

    for (size_t i = 0; i < 2; i++)
        if (
          p_instance->vulkan.swap_chain.details.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB           &&
          p_instance->vulkan.swap_chain.details.formats[i].format == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        )
            surface_format = p_instance->vulkan.swap_chain.details.formats[i];

    extent = (VkExtent2D)
    {
        .width  = p_instance->window.width,
        .height = p_instance->window.height
    };

    swapchain_create_info = (VkSwapchainCreateInfoKHR)
    {
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = p_instance->vulkan.surface,
        .minImageCount    = 2,
        .imageFormat      = surface_format.format,
        .imageColorSpace  = surface_format.colorSpace,
        .imageExtent      = extent,
        .imageArrayLayers = 1,
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode    = present_mode,
        .clipped        = VK_TRUE,
        .oldSwapchain   = VK_NULL_HANDLE
    };

    if (p_instance->vulkan.queue_family_indices.g != p_instance->vulkan.queue_family_indices.p)
    {
        swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices   = (const unsigned int*) &qfi;
    }
    else
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    swapchain_create_info.preTransform   = p_instance->vulkan.swap_chain.details.capabilities.currentTransform;

    if (vkCreateSwapchainKHR(p_instance->vulkan.device, &swapchain_create_info, 0, &p_instance->vulkan.swap_chain))
        // TODO: Replace with goto
        printf("FAILED TO CREATE SWAPCHAIN");

    vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (unsigned int *)&p_instance->vulkan.image_count, 0);

    p_instance->vulkan.swap_chain.images = calloc(p_instance->vulkan.image_count, sizeof(VkImage));

    vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (unsigned int *)&p_instance->vulkan.image_count, p_instance->vulkan.swap_chain.images);

    p_instance->vulkan.swap_chain.image_format = surface_format.format;
    p_instance->vulkan.swap_chain.extent       = extent;
}

void cleanup_swap_chain ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();

    for (size_t i = 0; i < 2; i++)
        vkDestroyFramebuffer(p_instance->vulkan.device, p_instance->vulkan.swap_chain.framebuffers[i], 0);

    for (size_t i = 0; i < 2; i++)
        vkDestroyImageView(p_instance->vulkan.device, p_instance->vulkan.swap_chain.image_views[i], 0);

    vkDestroySwapchainKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, (void *)0);

}

void create_image_views ( void )
{
    
}

void create_command_pool ( void )
{

    // Initialized data
    GXInstance_t            *p_instance = g_get_active_instance();
    VkCommandPoolCreateInfo  pool_create_info = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = p_instance->vulkan.queue_family_indices.g,
    };

    if ( vkCreateCommandPool(p_instance->vulkan.device, &pool_create_info, 0, &p_instance->vulkan.command_pool) != VK_SUCCESS )
        g_print_error("Failed to create command pool!\n");
}

void create_command_buffers ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();

    VkCommandBufferAllocateInfo alloc_info = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = p_instance->vulkan.command_pool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = p_instance->vulkan.max_buffered_frames
    };

    p_instance->vulkan.command_buffers = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkCommandBuffer));

    if ( vkAllocateCommandBuffers(p_instance->vulkan.device, &alloc_info, p_instance->vulkan.command_buffers) != VK_SUCCESS )
        g_print_error("failed to allocate command buffers!\n");
}

u32 find_memory_type ( u32 type_filter, VkMemoryPropertyFlags properties )
{
    GXInstance_t *p_instance = g_get_active_instance();
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(p_instance->vulkan.physical_device, &mem_properties);

    for (u32 i = 0; i < mem_properties.memoryTypeCount; i++)
    {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    g_print_error("[G10] Failed to find a suitable memory type\n");
    return 0;
}

int check_vulkan_device ( GXInstance_t *p_instance, VkPhysicalDevice physical_device, char **required_extension_names )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance               == (void *) 0     ) goto no_instnace;
        if ( physical_device          == VK_NULL_HANDLE ) goto no_physical_device;
        if ( required_extension_names == (void *) 0     ) goto no_required_extension_names;
    #endif

    // Initialized data
    bool passes_queue   = false,
         has_extensions = true,
         has_swap_chain = false;
    VkPhysicalDeviceProperties device_properties = { 0 };

    // Get the physical devices properties
    vkGetPhysicalDeviceProperties(physical_device, &device_properties);

    // Check if this is the requested device
    if ( strcmp(device_properties.deviceName, p_instance->vulkan.requested_device_name) == 0 ) goto found_requested_device;

    // Check queue indices
    {

        // Initialized data
        struct { u32 g, p; }     indices = { 0, 0 };
        u32                      queue_family_count = 0;
        VkQueueFamilyProperties* queue_families = 0;

        // Get the number of queue family properties
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, (unsigned int *)&queue_family_count, (void*)0);

        // Allocate memory for the queue family properties
        queue_families = calloc(queue_family_count + 1, sizeof(VkQueueFamilyProperties));

        // Error check
        if ( queue_families == (void *) 0 ) goto no_mem;

        // Get the queue family properties
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, (unsigned int *)&queue_family_count, queue_families);

        // Iterate over eqch queue family property
        for (size_t i = 0; i < queue_family_count; i++)
        {

            // Initialized data
            VkBool32 present_support = 0;

            passes_queue = true;

            if ( queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
                indices.g = (u32)i;
            else
                passes_queue = false;

            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, (u32)i, p_instance->vulkan.surface, &present_support);

            if ( present_support )
                indices.p = (u32)i;
            else
                passes_queue = false;

            if ( passes_queue )
                break;
        }

        // Set the graphics queue family index
        p_instance->vulkan.queue_family_indices.g = indices.g;

        // Set the present queue family index
        p_instance->vulkan.queue_family_indices.p = indices.p;
    }

    // Check device extensions
    if ( required_extension_names )
    {

        // Initialized data
        u32                     device_extension_count           = 0;
        VkExtensionProperties  *available_device_extensions      = 0;
        dict                   *device_extension_name_dict       = 0,
                               *available_device_extensions_lut  = 0;
        char                  **available_device_extension_names = 0;

        // Get the quantity of device extension properties
        switch (
            vkEnumerateDeviceExtensionProperties(physical_device, (void*)0, (unsigned int *)&device_extension_count, (void*)0)
        )
        {
            case VK_SUCCESS:
            case VK_INCOMPLETE:
                break;
            default:
                goto failed_to_enumerate_device_extension_properties;
        }

        // Allocaet a list of availabler device extensions
        available_device_extensions = calloc(device_extension_count + 1, sizeof(VkExtensionProperties));

        // Error check
        if ( available_device_extensions == (void *) 0 ) goto no_mem;

        // Get the device extension properties
        switch (
            vkEnumerateDeviceExtensionProperties(physical_device, (void*)0, 0, available_device_extensions)
        )
        {
            case VK_INCOMPLETE:
                g_print_warning("[G10] Failed to enumerate all device extension properties in call to function \"%s\"\n", __FUNCTION__);
            case VK_SUCCESS:
                break;
            default:
                goto failed_to_enumerate_device_extension_properties;
        }

        // Construct a dictionary from the available device extensions
        dict_construct(&available_device_extensions_lut, device_extension_count);

        // Construct an extension lookup table
        for (size_t i = 0; i < device_extension_count; i++)
            dict_add(available_device_extensions_lut, available_device_extensions[i].extensionName, (void *)1);

        // Iterate over each requested extension
        for (size_t i = 0; required_extension_names[i]; i++)

            // Does the physical device lack that extension?
            if ( dict_get(available_device_extensions_lut, required_extension_names[i]) == 0 )
                has_extensions = false;

    }
    else
        has_extensions = true;

    // Check for swap chain support
    {
        if (has_extensions)
        {
            u32 format_count       = 0,
                present_mode_count = 0;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, p_instance->vulkan.surface, &p_instance->vulkan.swap_chain.details.capabilities);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, p_instance->vulkan.surface, (unsigned int *)&format_count, (void*)0);

            p_instance->vulkan.swap_chain.details.formats = calloc(format_count + 1, sizeof(VkSurfaceFormatKHR));

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, p_instance->vulkan.surface, (unsigned int *)&format_count, p_instance->vulkan.swap_chain.details.formats);

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, p_instance->vulkan.surface, (unsigned int *)&present_mode_count, (void *)0);

            p_instance->vulkan.swap_chain.details.present_modes = calloc(present_mode_count + 1, sizeof(VkPresentModeKHR));

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, p_instance->vulkan.surface,(unsigned int *)&present_mode_count, p_instance->vulkan.swap_chain.details.present_modes);

            if ( ((bool)present_mode_count) & ((bool)format_count) )
                has_swap_chain = true;
        }
    }

    // Success
    return passes_queue && has_extensions && has_swap_chain;

    failed_to_enumerate_device_extension_properties:
    found_requested_device:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_instnace:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_physical_device:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"physical_device\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_required_extension_names:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"required_extension_names\" in call to function \"%s\"\n", __FUNCTION__);
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

void create_buffer ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory )
{
    VkMemoryRequirements  mem_requirements;
    GXInstance_t         *p_instance         = g_get_active_instance();
    VkBufferCreateInfo    buffer_info      = { 0 };
    VkMemoryAllocateInfo  alloc_info       = { 0 };

    // Create a buffer
    {

        // Populate the create info struct
        buffer_info = (VkBufferCreateInfo)
        {
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size        = size,
            .usage       = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        // Create a buffer
        if ( vkCreateBuffer( p_instance->vulkan.device, &buffer_info, 0, buffer ) != VK_SUCCESS )
            g_print_error("[G10] Failed to create buffer in call to function \"%s\"\n", __FUNCTION__);

        vkGetBufferMemoryRequirements(p_instance->vulkan.device, *buffer, &mem_requirements);
    }

    // Allocate memory for the buffer
    {

        // Populate the allocation struct
        alloc_info = (VkMemoryAllocateInfo)
        {
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize  = mem_requirements.size,
            .memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties)
        };

        // Allocate memory for the buffer
        if (vkAllocateMemory(p_instance->vulkan.device, &alloc_info, 0, buffer_memory) != VK_SUCCESS)
            g_print_error("[G10] Failed to allocate memory to buffer in call to function \"%s\"\n", __FUNCTION__);

        // Bind the buffer to the device
        vkBindBufferMemory(p_instance->vulkan.device, *buffer, *buffer_memory, 0);
    }
}
*/
size_t g_load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument check
    #ifndef NDEBUG
        if ( path == 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");

    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read to data
    if ( buffer )
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);

    // Return 1
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"path\" provided to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

                // Error
                return 0;
        }
    }
}
/*
void clear_swap_chain ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();
    for (size_t i = 0; i < p_instance->vulkan.image_count; i++)
        vkDestroyImageView(p_instance->vulkan.device, p_instance->vulkan.swap_chain.image_views[i], 0);

    vkDestroySwapchainKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain.swap_chain, 0);
}
*/

int g_window_resize ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Get
    SDL_SetWindowSize(p_instance->sdl2.window, p_instance->window.width, p_instance->window.height);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n,", __FUNCTION__);
                #endif

                // Error
                return 1;
        }
    }
}

int g_print_error ( const char *const format, ... )
{

    // Argument check
    #ifndef NDEBUG
        if ( format == (void *) 0 ) goto no_format;
    #endif

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    // Uses ANSI terminal escapes to set the color to red,
    // print the message, and restore the color.
    fflush(stdout);
    printf("\033[91m");
    #endif

    // TODO: Configure error messages to write to other files?
    vfprintf(log_file, format, aList);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    printf("\033[0m");

    #endif

    va_end(aList);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_print_warning ( const char *const format, ... )
{

    // Argument check
    #ifndef NDEBUG
        if ( format == (void *) 0 ) goto no_format;
    #endif

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    // Uses ANSI terminal escapes to set the color to yellow,
    // print the message, and restore the color.
    printf("\033[93m");
    #endif

    vfprintf(log_file, format, aList);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    printf("\033[0m");
    #endif

    va_end(aList);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

int g_print_log ( const char *const format, ... )
{

    // Argument check
    #ifndef NDEBUG
        if ( format == (void *) 0 ) goto no_format;
    #endif

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    // Uses ANSI terminal escapes to set the color to light blue,
    // print the message, and restore the color.
    printf("\033[94m");
    #endif

    vfprintf(log_file, format, aList);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    printf("\033[0m");
    #endif

    va_end(aList);
    va_end(aList);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

int g_print_instance ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Formatting
    g_print_log(" - Instance info - \n");

    // Print the name
    g_print_log("name: \"%s\"\n", p_instance->name);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_start_schedule ( GXInstance_t* p_instance, const char* name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( name       == (void *) 0 ) goto no_name;
    #endif

    // Initialized data
    GXSchedule_t* p_schedule = (GXSchedule_t *) dict_get(p_instance->data.schedules, (char *)name);

    // Error check
    if ( p_schedule == (void *) 0 ) goto no_schedule;

    // Start the schedule
    if ( start_schedule(p_schedule) == 0 ) goto failed_to_start_schedule;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to find a schedule named \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_start_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to start schedule \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_stop_schedule ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Stop the instance's active schedule
    if ( stop_schedule(p_instance->context.schedule) == 0 ) goto failed_to_stop_schedule;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_stop_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to stop active schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int copy_state ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance                == (void *) 0 ) goto no_instance;
        if ( p_instance->context.scene == (void *) 0 ) goto no_context_scene;
    #endif

    // Initialized data
    size_t actor_count           = 0,
           ai_count              = 0,
           draw_count            = 0,
           draw_queue_type_count = 0;

    GXEntity_t **actors = 0,
               **ais    = 0,
               **draw   = 0;

    // Lock the mutexes

    // Physics
    SDL_LockMutex(p_instance->mutexes.move_object);
    SDL_LockMutex(p_instance->mutexes.update_force);
    SDL_LockMutex(p_instance->mutexes.resolve_collision);

    // AI
    SDL_LockMutex(p_instance->mutexes.ai_preupdate);
    SDL_LockMutex(p_instance->mutexes.ai_update);

    // Get a list of actors, ais, and draw items
    {

        // Get the quantity of dict entries
        actor_count = dict_keys(p_instance->context.scene->actors, 0);
        ai_count    = dict_keys(p_instance->context.scene->ais, 0);
        draw_count  = dict_keys(p_instance->context.scene->entities, 0);

        // Allocate memory for lists
        actors = calloc(actor_count+1, sizeof(void *));
        ais    = calloc(ai_count+1, sizeof(void *));
        draw   = calloc(draw_count+1, sizeof(void *));

        // Get the contents of the dict
        dict_values(p_instance->context.scene->actors, (void **)actors);
        dict_values(p_instance->context.scene->ais, (void **)ais);
        dict_values(p_instance->context.scene->entities, (void **)draw);
    }

    // Empty the queues
    {

        // Physics
        while ( queue_empty(p_instance->queues.actor_move) == false )
            queue_dequeue(p_instance->queues.actor_move, 0);

        while ( queue_empty(p_instance->queues.actor_force) == false)
            queue_dequeue(p_instance->queues.actor_force, 0);

        while ( queue_empty(p_instance->queues.actor_collision) == false)
            queue_dequeue(p_instance->queues.actor_collision, 0);

        // AI
        while ( queue_empty(p_instance->queues.ai_preupdate) == false ) { queue_dequeue(p_instance->queues.ai_preupdate, 0); };

        while ( queue_empty(p_instance->queues.ai_update)    == false ) { queue_dequeue(p_instance->queues.ai_update   , 0); };

        // Draw

        // Iterate over each render pass
        /*
        for (size_t i = 0; i < p_instance->context.renderer->render_pass_count; i++)
        {

            // Initialized data
            GXRenderPass_t *p_render_pass = p_instance->context.renderer->render_passes_data[i];

            // Iterate over each draw item
            for (size_t j = 0; j < draw_count; j++)
            {

                // Initialized data
                queue *p_queue = dict_get(p_render_pass->draw_queue_types, draw[i]->shader_name);

                // Dequeue each element
                while( queue_empty(p_queue) == false ) { (void) queue_dequeue(p_queue, 0); }; 
            }
        }
        */
    }

    // Populate the new queues
    {

        // Physics
        for (size_t i = 0; i < actor_count; i++)
        {
            queue_enqueue(p_instance->queues.actor_move, actors[i]);
            queue_enqueue(p_instance->queues.actor_force, actors[i]);
            queue_enqueue(p_instance->queues.actor_collision, actors[i]);
        }

        // AI
        for (size_t i = 0; i < ai_count; i++)
        {
            queue_enqueue(p_instance->queues.ai_preupdate, ais[i]);
            queue_enqueue(p_instance->queues.ai_update, ais[i]);
        }

        // Draw
        
        // Iterate over each render pass
        /*
        for (size_t i = 0; i < p_instance->context.renderer->render_pass_count; i++)
        {

            // Initialized data
            GXRenderPass_t *p_render_pass = p_instance->context.renderer->render_passes_data[i];

            // Iterate over each draw item
            for (size_t j = 0; j < draw_count; j++)
            {

                // Initialized data
                queue *p_queue = dict_get(p_render_pass->draw_queue_types, draw[j]->shader_name);

                // Is the item going to be drawn this renderpass?
                if ( p_queue )

                    // Add the draw item to the draw queue
                    queue_enqueue(p_queue, draw[j]);
            }
        }
        */
    }

    // Clean the scope
    free(actors);
    free(ais);
    free(draw);

    // Unlock the mutexes

    // Physics
    SDL_UnlockMutex(p_instance->mutexes.move_object);
    SDL_UnlockMutex(p_instance->mutexes.update_force);
    SDL_UnlockMutex(p_instance->mutexes.resolve_collision);

    // AI
    SDL_UnlockMutex(p_instance->mutexes.ai_preupdate);
    SDL_UnlockMutex(p_instance->mutexes.ai_update);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_context_scene:
                #ifndef NDEBUG
                    //g_print_error("[G10] No active scene in instance in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

GXInstance_t *g_get_active_instance ( void )
{

    // Success
    return active_instance;
}

int g_find_bind ( GXInstance_t *p_instance, char *bind_name, GXBind_t **pp_bind )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( bind_name  == (void *) 0 ) goto no_bind_name;
        if ( pp_bind    == (void *) 0 ) goto no_bind;
    #endif

    // Initialized data
    GXBind_t *p_bind = find_bind(p_instance->input, bind_name);

    // Return a pointer to the caller
    *pp_bind = p_bind;

    // Success
    return (bool)p_bind;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_bind_name:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"bind_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"pp_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_cache_material ( GXInstance_t *p_instance, GXMaterial_t *material )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( material   == (void *) 0 ) goto no_material;
    #endif

    // Add the material to the instance's cache
    dict_add(p_instance->cache.materials, material->name, material);

    // Increment the users counter
    material->users++;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_material:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_material\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int g_cache_part ( GXInstance_t *p_instance, GXPart_t *p_part )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( p_part     == (void *) 0 ) goto no_part;
    #endif

    // Add the part to the dictionary
    dict_add(p_instance->cache.parts, p_part->name, p_part);

    // Increment the user count
    p_part->users++;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_part:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_part\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

/*
int g_cache_shader ( GXInstance_t *p_instance, GXShader_t *p_shader )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( p_shader   == (void *) 0 ) goto no_shader;
    #endif

    // Add the shader to the cache
    dict_add(p_instance->cache.shaders, p_shader->name, p_shader);

    // Increment the shader users
    p_shader->users++;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_shader:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}
*/
int g_cache_ai ( GXInstance_t *p_instance, GXAI_t *p_ai )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( p_ai       == (void *) 0 ) goto no_ai;
    #endif

    dict_add(p_instance->cache.ais, p_ai->name, p_ai);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ai:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void g_user_exit ( callback_parameter_t input, GXInstance_t* p_instance )
{

    // Stop the instance from running
    p_instance->running = false;

    // Stop the schedule
    if ( p_instance->context.schedule )
    {

        // Initialized data
        GXThread_t *main_thread = (GXThread_t *)dict_get(p_instance->context.schedule->threads, "Main Thread");

        // Stop running the main thread
        main_thread->running = false;
    }

    // Success
    return;
}

void g_user_help ( callback_parameter_t input, GXInstance_t* p_instance )
{

    // Open the G10 website in the web browser
    SDL_OpenURL("https://g10.app");

    // Delay for 100 ms to avoid extra bind calls
    SDL_Delay(100);

    // Success
    return;
};

void g_toggle_mouse_lock ( callback_parameter_t state, GXInstance_t *p_instance )
{
    if ( state.inputs.key.depressed == true )
    {
        SDL_SetRelativeMouseMode(!SDL_GetRelativeMouseMode());

        #ifndef NDEBUG
            g_print_log("[G10] Toggling mouse locking\n");
        #endif
    }

    SDL_Delay(333);
}

void g_play_sound ( callback_parameter_t state, GXInstance_t *p_instance )
{
    // GXSound_t* sampleSound = NULL;
    // static bool played = false;
    // if (played == false)
    // {
    //     load_sound(&sampleSound, "G10/ding.mp3");
    //     play_sound(sampleSound, false);
    //     played = true;
    // }



}

GXMaterial_t *g_find_material ( GXInstance_t *p_instance, char *name )
{
    
    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( name       == (void *) 0 ) goto no_name;
    #endif

    // Success
    return (GXMaterial_t *) dict_get(p_instance->cache.materials, (char *)name);

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

GXPart_t *g_find_part ( GXInstance_t *p_instance, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( name       == (void *) 0 ) goto no_name;
    #endif

    // Success
    return (GXPart_t *) dict_get(p_instance->cache.parts, (char *)name);

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

GXShader_t *g_find_shader ( GXInstance_t *p_instance, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( name       == (void *) 0 ) goto no_name;
    #endif

    // Success
    return (GXShader_t *) dict_get(p_instance->cache.shaders, (char *)name);

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

GXAI_t *g_find_ai ( GXInstance_t *p_instance, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
        if ( name       == (void *) 0 ) goto no_name;
    #endif

    // Success
    return (GXAI_t *) dict_get(p_instance->cache.ais, (char *)name);

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int g_exit ( GXInstance_t **pp_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_instance == (void *) 0 ) goto no_instance;
    #endif

    // Initialized data
    GXInstance_t *p_instance = *pp_instance;

    // No more instance for caller
    *pp_instance = 0;

    // Error check
    if ( p_instance == (void *) 0 ) goto pointer_to_null_pointer;

    // Flip the running flag
    p_instance->running = false;

    // Wait for the GPU to finish whatever its doing
    //vkDeviceWaitIdle(p_instance->vulkan.device);

    // Cleanup the instance
    {

        // Free the instance name
        free(p_instance->name);

        // TODO: Fix
        //dict_free_clear(p_instance->data.scenes, &destroy_scene);

        // Cleanup the context
        {

            // Clear the active schedule...
            p_instance->context.schedule = 0;

            // ... the active scene ...
            p_instance->context.scene = 0;

            // ... any loading scene ...
            p_instance->context.loading_scene = 0;

            // ... and the active renderer ...
            //if ( p_instance->context.renderer )
            //    if ( destroy_renderer(&p_instance->context.renderer) == 0 ) goto failed_to_destroy_renderer;

            // ... and the user code callback
            p_instance->context.user_code_callback = 0;
        }

        // Cleanup the data
        {

            // TODO: Cleanup the scenes...
            //dict_free_clear(p_instance->data.scenes, destroy_scene);

            // TODO: ... and the schedules
            //dict_free_clear(p_instance->data.schedules, destroy_schedule);
        }

        // Clean up the cache
        {
            /*
            // Initialized data
            size_t     len      = dict_values(p_instance->cache.parts,  0);
            GXPart_t **pp_parts = calloc(len, sizeof(GXPart_t *));

            // Error check
            if ( pp_parts == (void *) 0 ) goto no_mem;

            // Get the contents of the part cache
            dict_values(p_instance->cache.parts, pp_parts);

            // Iterate over each part
            for (size_t i = 0; i < len; i++)
            {

                // Initialized data
                GXPart_t *i_part = pp_parts[i];

                // Pop the part
                dict_pop(p_instance->cache.parts, i_part->name, 0);

                // Destroy the part
                destroy_part(&i_part);
            }

            // Destroy the part cache            
            dict_destroy(&p_instance->cache.parts);
            */
        }

        // Cleanup mutexes
        {

            // Destroy the entity loading mutex...
            if ( p_instance->mutexes.load_entity )
                SDL_DestroyMutex(p_instance->mutexes.load_entity);

            // ... and the cache mutexes...
            if ( p_instance->mutexes.shader_cache )
                SDL_DestroyMutex(p_instance->mutexes.shader_cache);

            if ( p_instance->mutexes.part_cache )
                SDL_DestroyMutex(p_instance->mutexes.part_cache);

            if ( p_instance->mutexes.ai_cache )
                SDL_DestroyMutex(p_instance->mutexes.ai_cache);

            if ( p_instance->mutexes.material_cache )
                SDL_DestroyMutex(p_instance->mutexes.material_cache);

            // ... and the physics mutexes...
            if ( p_instance->mutexes.move_object )
                SDL_DestroyMutex(p_instance->mutexes.move_object);
            if ( p_instance->mutexes.update_force )
                SDL_DestroyMutex(p_instance->mutexes.update_force);
            if ( p_instance->mutexes.resolve_collision )
                SDL_DestroyMutex(p_instance->mutexes.resolve_collision);

            // ... and the AI mutexes
            if ( p_instance->mutexes.ai_preupdate )
                SDL_DestroyMutex(p_instance->mutexes.ai_preupdate);
            if ( p_instance->mutexes.ai_update )
                SDL_DestroyMutex(p_instance->mutexes.ai_update);

            p_instance->mutexes.load_entity       = 0,
            p_instance->mutexes.shader_cache      = 0,
            p_instance->mutexes.part_cache        = 0,
            p_instance->mutexes.ai_cache          = 0,
            p_instance->mutexes.material_cache    = 0,
            p_instance->mutexes.move_object       = 0,
            p_instance->mutexes.update_force      = 0,
            p_instance->mutexes.resolve_collision = 0,
            p_instance->mutexes.ai_preupdate      = 0,
            p_instance->mutexes.ai_update         = 0;
        }

        // Cleanup queues
        {

            // Destroy the entity loading queue
            if ( p_instance->queues.load_entity )
                queue_destroy(&p_instance->queues.load_entity);
            // Destroy the light probe loading queue
            if ( p_instance->queues.load_light_probe )
                queue_destroy(&p_instance->queues.load_light_probe);

            // Destroy the physics queues
            if ( p_instance->queues.actor_move )
                queue_destroy(&p_instance->queues.actor_move);
            if ( p_instance->queues.actor_collision )
                queue_destroy(&p_instance->queues.actor_collision);
            if ( p_instance->queues.actor_force )
                queue_destroy(&p_instance->queues.actor_force);

            // Destroy the AI queues
            if ( p_instance->queues.ai_preupdate )
                queue_destroy(&p_instance->queues.ai_preupdate);
            if ( p_instance->queues.ai_update )
                queue_destroy(&p_instance->queues.ai_update);
        }

        // Cleanup input
        if ( p_instance->input )
            destroy_input(&p_instance->input);
    }

    // Cleanup vulkan
    {

        // Clear the swapchain
        //clear_swap_chain();

        // Destroy the logical device
        //vkDestroyDevice(p_instance->vulkan.device, (void*)0);

        // Destroy the surface
        //vkDestroySurfaceKHR(p_instance->vulkan.instance, p_instance->vulkan.surface, (void*) 0);

        // Destroy the instance
        //vkDestroyInstance(p_instance->vulkan.instance, (void*) 0) ;
    }

    // Clean up FMOD
    #ifdef BUILD_G10_WITH_FMOD

        //FMOD Cleanup

        //Replace this with a struct member later?
        //Not thread-safe
        FMOD_RESULT fmodResult = FMOD_System_Release(instance->fmod.system);

    #endif

    // Clean up discord
    #ifdef BUILD_G10_WITH_DISCORD
    #endif

    // SDL Cleanup
    SDL_DestroyWindow(p_instance->sdl2.window);
    SDL_Quit();

    // Free the instance data
    free(p_instance);

    // Log
    g_print_log("[G10] Exit successful\n");

    // Success
    return 1;

    failed_to_destroy_renderer:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] Parameter \"pp_instance\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
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
