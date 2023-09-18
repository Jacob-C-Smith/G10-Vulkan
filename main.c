// Standard library
#include <stdio.h>

// G10
#include <G10/G10.h>
#include <G10/GXScheduler.h>
#include <G10/GXUserCode.h>
#include <G10/GXEntity.h>

// This gets called once a frame
int user_code_callback ( GXInstance_t *p_instance );

// This gets called after g_init and before g_start_schedule
int game_initialization ( GXInstance_t *p_instance );

// Log some stuff
int game_log ( GXInstance_t *p_instance );

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t *p_instance    = 0;
    const char   *instance_path = "G10/debug client instance.json",
                 *schedule_name = "Client Schedule";

    // Parse command line arguments
    for ( size_t i = 0; i < argc; i++ )
    {

        // Path to instance
        if ( strcmp("-instance", argv[i]) == 0 )
            instance_path = argv[++i];

        // Name of schedule
        if ( strcmp("-schedule", argv[i]) == 0 )
            schedule_name = argv[++i];
    }

    // Create an instance
    if ( g_init(&p_instance, instance_path) == 0 )
    {

        // Write an error message
        (void) g_print_error("[G10] Failed to initialize G10 in call to function \"%s\"\n", __FUNCTION__);
        
        // Error
        return EXIT_FAILURE;
    }
    
    // Set up the game itself
    if ( game_initialization(p_instance) == 0 )
    {

        // Write an error message
        (void) g_print_error("Failed to initialize game in call to function \"%s\"\n", __FUNCTION__);
        
        // Exit
        (void) g_exit(&p_instance);

        // Error
        return EXIT_FAILURE;
    }
    
    // Log some details about the game and G10
    (void) game_log(p_instance);
    
    // Start the game
    (void) g_start_schedule(p_instance, schedule_name);

    // Stop the game
    (void) g_stop_schedule(p_instance);

    // Exit
    (void) g_exit(&p_instance);

    // Success
    return EXIT_SUCCESS;
}

int user_code_callback ( GXInstance_t *p_instance )
{
    
    // Whatever code you want
    fflush(stdout);
    
    // Success
    return 1;
}

int gets_called_once_a_tick ( GXInstance_t *p_instance )
{

    printf("WE DID IT COMRADE!\n");

    return 1;
}


int game_initialization ( GXInstance_t *p_instance )
{
    
    // Set the user code callback
    if ( add_user_code_callback(p_instance, &user_code_callback) == 0 )
    {
        (void) g_print_error("Failed to set user code callback!\n");

        // Error
        return 0;
    }

    // Set the binds
    {
        
        // Initialized data
        GXBind_t *p_quit = 0,
                 *p_help = 0;

        // Get the quit bind
        (void) g_find_bind(p_instance, "QUIT", &p_quit);

        // Get the help bind
        (void) g_find_bind(p_instance, "HELP", &p_help);

        // Set the user exit bind
        (void) register_bind_callback(p_quit, &g_user_exit);

        // Set the help bind
        (void) register_bind_callback(p_help, &g_user_help);
    }

    // Set user code callback
    add_user_code_callback(p_instance, &gets_called_once_a_tick);

    // Success
    return 1;
}


int game_log ( GXInstance_t *p_instance )
{
    
    // Print the input
    (void) input_info(p_instance->input);

    // Print the renderer
    //(void) print_renderer(p_instance->context.renderer);

    // Print the scene
    (void) scene_info(p_instance->context.scene);

    // Flush standard out
    (void) fflush(stdout);

    // Success
    return 1;
}