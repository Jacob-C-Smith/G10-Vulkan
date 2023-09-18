#include <G10/GXAI.h>

void init_ai ( void )
{

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();

    // Create instance mutexes for AI tasks
    p_instance->mutexes.ai_cache     = SDL_CreateMutex();
    p_instance->mutexes.ai_preupdate = SDL_CreateMutex();
    p_instance->mutexes.ai_update    = SDL_CreateMutex();

    // Exit
    return;
}

int create_ai ( GXAI_t **pp_ai )
{
    
    // Argument check
    #ifndef NDEBUG
        if ( pp_ai == (void *) 0 ) goto no_ai;
    #endif

    // Initialized data
    GXAI_t *p_ai = calloc(1, sizeof(GXAI_t));

    // Error check
    if ( p_ai == (void *) 0 ) goto no_mem;

    // Write the return value
    *pp_ai = p_ai;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_ai ( GXAI_t **pp_ai, char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_ai == (void *) 0 ) goto no_ai;
        if ( path  == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len+1, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the file contents into memory
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_load_file;

    // Parse the file contents into an AI struct
    if ( load_ai_as_json_text(pp_ai, text) == 0 ) goto failed_to_construct_ai_from_file_json;

    // Free the file data
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    free(text);
                    g_print_error("[G10] [AI] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_ai_from_file_json:
                #ifndef NDEBUG
                    free(text);
                    g_print_error("[G10] [AI] Failed to construct AI from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_ai_as_json_text ( GXAI_t **pp_ai, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_ai == (void *) 0 ) goto no_ai;
        if ( text  == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    json_value *p_value = 0;

    // Parse the text into a JSON value
    if ( parse_json_value(text, 0, &p_value)   == 0 ) goto failed_to_parse_json;

    if ( load_ai_as_json_value(pp_ai, p_value) == 0 ) goto failed_to_load_ai_as_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // JSON parsing errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Failed to parse JSON in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {

            failed_to_load_ai_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Failed to load AI in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_ai_as_json_value ( GXAI_t **pp_ai, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_ai   == (void *) 0 ) goto no_ai;
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t  *p_instance      = g_get_active_instance();
    json_value   *p_name          = 0,
                  *p_initial_state = 0,
                  *p_states        = 0,
                 **pp_states       = 0;

    // Parse the AI as an object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name          = dict_get(p_dict, "name");
        p_states        = dict_get(p_dict, "states");
        p_initial_state = dict_get(p_dict, "initial state");

        // Error check
        if ( ! ( 
            p_name          &&
            p_states        &&
            p_initial_state 
        ) )
            goto missing_properties;
    }
    else
        goto wrong_type;

    // Parse the AI as a path
    if ( p_name->type == JSON_VALUE_STRING )
    {

        // Error check
        if ( p_initial_state->type == JSON_VALUE_STRING ) goto wrong_initial_state_type;

        // Initialized data
        GXAI_t* p_ai = 0;
        GXAI_t* p_cache_ai = 0;

        // Lock the AI cache mutex
        SDL_LockMutex(p_instance->mutexes.ai_cache);

        // Search the cache for the AI
        p_cache_ai = g_find_ai(p_instance, p_name->string);

        // If the AI is in the cache ...
        if ( p_cache_ai )
        {

            // ... make a copy of the cached AI
            copy_ai(pp_ai, p_cache_ai);

            // Write the return
            p_ai = *pp_ai;

            // Set the initial state
            goto set_initial_state;
        }

        // ... the AI is not in the cache

        // Allocate memory for an AI
        if ( create_ai(&p_ai) == 0 ) goto failed_to_allocate_ai;

        // Copy the AI name
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Error check
            if ( name_len > 255 ) goto name_too_long;

            // Allocate for the name
            p_ai->name = calloc(name_len+1, sizeof(char));

            // Error check
            if ( p_ai->name == (void *)0 ) goto no_mem;

            // Copy the name
            strncpy(p_ai->name, p_name->string, name_len);
        }

        // Initialize each AI state, and set the initial state
        if ( p_states->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            size_t state_count = 0;

            // Get the contents of the array
            {

                // Get the quantity of states
                array_get(p_states->list, 0, &state_count );

                // Allocate an array for the states
                pp_states = calloc(state_count, sizeof(json_value *));

                // Error check
                if ( pp_states == (void *) 0 ) goto no_mem;

                // Populate the states array
                array_get(p_states->list, (void **)pp_states, 0 );
            }

            // Construct a dictionary for the states
            dict_construct(&p_ai->states, state_count);

            // Iterate over each state
            for (size_t i = 0; i < state_count; i++)
            {

                // Initialized data
                size_t  state_len  = strlen(((json_value *)pp_states[i])->string);
                char   *state_name = calloc(state_len + 1, sizeof(char));

                // Error check
                if ( state_name == (void *) 0 ) goto no_mem;

                // Copy the name of the state from the array
                strncpy(state_name, ((json_value *)pp_states[i])->string, state_len);

                // Copy the initial state
                if ( strcmp(state_name, p_initial_state->string) == 0 )
                    p_ai->current_state = state_name;

                // Add the state name to the state dictionary
                dict_add(p_ai->states, state_name, 0);
            }

            // Clean the scope
            free(pp_states);
        }
        // Default
        else
            goto wrong_states_type;

        // Cache the AI
        g_cache_ai(p_instance, p_ai);

        // Unlock the AI caching mutex
        SDL_UnlockMutex(p_instance->mutexes.ai_cache);

        // Set the initial state
        set_initial_state:

        if ( p_initial_state->type == JSON_VALUE_STRING )
            p_ai->current_state = p_initial_state->string; // Same char * stored in the states dict

        // Return the AI to the caller
        *pp_ai = p_ai;
    }
    else
        goto wrong_name_type;

    // Success
    return 1;

    // Error handling
    {

        // JSON errors
        {

            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] JSON value must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Not enough properties to construct AI in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_initial_state_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Property \"initial state\" must be of type [ string ] in call to function \"%s\"\n\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_states_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Property \"states\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_too_long:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] \"name\" property's length must be less than 256 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 Errors
        {
            failed_to_allocate_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Failed to allocate AI in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
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

int add_ai_state_callback ( GXAI_t *p_ai , char *state_name, int (*function_pointer) ( GXEntity_t *entity ) )
{

    // Argument errors
    #ifndef NDEBUG
        if ( p_ai             == (void *) 0 ) goto no_ai;
        if ( state_name       == (void *) 0 ) goto no_state_name;
        if ( function_pointer == (void *) 0 ) goto no_function_pointer;
    #endif

    // Update the state callback
    dict_add(p_ai->states, state_name, function_pointer);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_state_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"state_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_function_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"function_pointer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_ai_state ( GXAI_t *p_ai , const char *state_name )
{

    // Argument errors
    #ifndef NDEBUG
        if ( p_ai       == (void *) 0 ) goto no_ai;
        if ( state_name == (void *) 0 ) goto no_state_name;
    #endif

    // Set the state, if it is a valid state
    if ( dict_get(p_ai->states, (char *) state_name) )
        p_ai->current_state = (char *) state_name;
    else
        return 0;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_state_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"state_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_ai_pre_update_callback ( GXAI_t *p_ai , int (*function_pointer) ( GXEntity_t *p_entity ) )
{

    // Argument errors
    #ifndef NDEBUG
        if ( p_ai             == (void *) 0 ) goto no_ai;
        if ( function_pointer == (void *) 0 ) goto no_function_pointer;
    #endif

    // TODO: FIX 
    // Set the AI preupdate callback
    p_ai->pre_ai = function_pointer;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_function_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"function_pointer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int pre_update_ai ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Initialized data
    GXEntity_t* p_entity = 0;

    // Get an entity with an AI
    {

        // Lock the mutex
        SDL_LockMutex(p_instance->mutexes.ai_preupdate);

        // Are there any AIs left to preupdate?
        if ( queue_empty(p_instance->queues.ai_preupdate) )
        {

            // If not, unlock and return
            SDL_UnlockMutex(p_instance->mutexes.ai_preupdate);

            // Success
            return 1;
        }

        // Get a pointer to an entity
        queue_dequeue(p_instance->queues.ai_preupdate, (void **)&p_entity);

        // Unlock the mutex
        SDL_UnlockMutex(p_instance->mutexes.ai_preupdate);
    }

    // Update the AI
    if ( p_entity )
        preupdate_entity_ai(p_entity);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_log("[G10] [AI] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int copy_ai ( GXAI_t **pp_ai, GXAI_t *p_ai )
{

    // Argument errors
    #ifndef NDEBUG
        if (pp_ai == (void *) 0 ) goto no_return;
        if (p_ai  == (void *) 0 ) goto no_ai;
    #endif

    // Initialized data
    GXAI_t *dest_ai = 0;

    // Allocate a new AI
    if ( create_ai(&dest_ai) == 0 ) goto failed_to_allocate_ai;

    // Return the copy to the caller
    *dest_ai = (GXAI_t)
    {
        .name = p_ai->name,
        .states = p_ai->states,
        .pre_ai = p_ai->pre_ai
    };

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_return:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_allocate_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Failed to create AI in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int ai_info ( GXAI_t *p_ai )
{

    // Argument errors
    #ifndef NDEBUG
        if ( p_ai == (void *) 0 ) goto no_ai;
    #endif

    // Initialized data
    size_t   state_count = dict_keys(p_ai->states, 0);
    char   **state_names = calloc(state_count, sizeof (char *));

    // Formatting
    g_print_log(" - AI info - \n");

    // Print the name
    g_print_log("name        : \"%s\"\n", p_ai->name);

    // Print the active state
    g_print_log("active state: \"%s\"\n", p_ai->current_state);

    // Formatting
    g_print_log("states      : \n");

    // Get each AI state
    if ( state_count )
        dict_keys(p_ai->states, state_names);

    // Iterate over each state
    for (size_t i = 0; i < state_count; i++)
    {

        // Print the name of each state
        g_print_log("\t[%d] \"%s\"\n", i, state_names[i]);

    }

    putchar('\n');

    // Free the list of state names
    free(state_names);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int update_ai ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Initialized data
    GXEntity_t* entity = 0;

    // Get an ai entity
    {

        // Lock the mutex
        SDL_LockMutex(p_instance->mutexes.ai_update);

        // Is there anything left to move?
        if (queue_empty(p_instance->queues.ai_update))
        {

            // If not, unlock and return
            SDL_UnlockMutex(p_instance->mutexes.ai_update);
            return 1;
        }

        queue_dequeue(p_instance->queues.ai_update, (void **)(&entity));

        SDL_UnlockMutex(p_instance->mutexes.ai_update);
    }

    // Update the AI
    if ( entity )
        // Uncomment
        update_entity_ai(entity);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_log("[G10] [AI] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_ai ( GXAI_t **pp_ai )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_ai == (void *) 0 ) goto no_ai;
    #endif

    // Initialized data
    GXAI_t *p_ai = *pp_ai;

    // No more pointer for caller
    *pp_ai = 0;

    if ( p_ai == 0 )
        return 1;

    p_ai->current_state = 0;
    p_ai->pre_ai = 0;

    // Free the AI name
    if ( p_ai->name )
        free(p_ai->name);

    // Free each key from the dictionary
    if ( p_ai->states != 0 )
    {
        dict_free_clear(p_ai->states, ((void(*)(const void*))&free));

        dict_destroy(&p_ai->states);
    }

    // Free the AI
    free(p_ai);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ai:
                #ifndef NDEBUG
                    g_print_error("[G10] [AI] Null pointer provided for parameter \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
