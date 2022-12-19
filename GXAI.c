#include <G10/GXAI.h>

void init_ai                   ( void )
{
	GXInstance_t *instance       = g_get_active_instance();

	instance->ai_cache_mutex     = SDL_CreateMutex();
	instance->ai_preupdate_mutex = SDL_CreateMutex();
	instance->ai_update_mutex    = SDL_CreateMutex();
}

int create_ai                  ( GXAI_t       **pp_ai )
{
	// Argument check
	{
		#ifndef NDEBUG
			if(pp_ai == (void *)0)
				goto no_ai;
		#endif
	}

	// Initialized data
	GXAI_t *p_ai = calloc(1, sizeof(GXAI_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(p_ai == (void *)0)
				goto no_mem;
		#endif
	}

	*pp_ai = p_ai;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_ai                    ( GXAI_t       **pp_ai, char        *path )
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_ai == (void *) 0 )
				goto no_ai;
			if ( path  == (void *) 0 )
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  file_len  = g_load_file(path, 0, false);
	char   *file_data = calloc(file_len+1, sizeof(char));

	// Load the file contents into memory
	if ( g_load_file(path, file_data, false) == 0 )
		goto failed_to_load_file;

	// Parse the file contents into an AI struct
	if ( load_ai_as_json(pp_ai, file_data, file_len) == 0)
		goto failed_to_construct_ai_from_file_json;

	// Free the file data
	free(file_data);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// G10 errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					free(file_data);
					g_print_error("[G10] [AI] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
				#endif	
				return 0;
			failed_to_construct_ai_from_file_json:
				#ifndef NDEBUG
					free(file_data);
					g_print_error("[G10] [AI] Failed to construct AI from file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
				#endif	
				return 0;
		}
	}
}

int load_ai_as_json            ( GXAI_t       **pp_ai, char        *token_text, size_t   len )
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_ai == (void *)0 )
				goto no_ai;
			if ( token_text == (void *)0 )
				goto no_token_text;
		#endif
	}

	// Initialized data
	GXInstance_t *instance      = g_get_active_instance();
	size_t        state_count   = 0;
	char         *name          = 0,
		        **states        = 0,
		         *initial_state = 0;
	dict         *ai_dict       = 0;
	int           ret           = 1;

	// Parse the JSON
	{

		// Initialized data
		JSONToken_t *token = 0;

		// Parse the JSON text into a dictionary
		parse_json(token_text, len, &ai_dict);

		// Error checking
		{
			if (ai_dict == 0)

				// TODO: Error handling
				return 0;
		}

		// Get the name 
		token         = dict_get(ai_dict, "name");
		name          = JSON_VALUE(token, JSONstring);

		// Get the states
		token         = dict_get(ai_dict, "states");
		states        = JSON_VALUE(token, JSONarray);

		// Get the initial state
		token         = dict_get(ai_dict, "initial state");
		initial_state = JSON_VALUE(token, JSONstring);

	}

	// Construct the AI
	{

		// Initialized data
		GXAI_t* p_ai = 0;

		// Check the cache
		{
			// Lock the AI cache mutex
			SDL_LockMutex(instance->ai_cache_mutex);

			// Initialized data
			GXAI_t* c_ai = g_find_ai(instance, name);
			
			// If the ai was in the cache, copy it
			if ( c_ai )
			{

				// Make a copy of the cached ai
				copy_ai(pp_ai, c_ai);

				// Write the return
				p_ai = *pp_ai;

				// Set the initial state
				goto set_initial_state;
			}
		}

		// Allocate an AI
		{
			create_ai(pp_ai);

			p_ai = *pp_ai;
		}

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for the name
			p_ai->name      = calloc(name_len+1, sizeof(char));

			// Error checking
			{
				#ifndef NDEBUG
					if(p_ai->name == (void *)0)
						goto no_mem;
				#endif
			}

			// Copy the name 
			strncpy(p_ai->name, name, name_len);
		}

		// Initialize each state
		{

			// Initialized data
			size_t state_count = 0;

			// Count up states
			while (states[++state_count]);			

			// Construct a dictionary for the states
			dict_construct(&p_ai->states, state_count);

			// Iterate over each state
			for (size_t i = 0; i < state_count; i++)
			{

				// Initialized data
				size_t  state_len   = strlen(states[i]);

				// Allocate for the state name
				char   *state_name = calloc(state_len + 1, sizeof(char));
				
				// Error checking
				{
					#ifndef NDEBUG
						if(state_name == (void *)0)
							goto no_mem;
					#endif
				}

				strncpy(state_name, states[i], state_len);

				// Add the state name to the state dictionary
				dict_add(p_ai->states, state_name, 0);
			}
		}

		// Cache the AI
		g_cache_ai(instance, p_ai);

		set_initial_state:
		// Set the initial state
		{
			
			// Initialized data
			size_t current_state_len = strlen(initial_state);

			// Allocate for the initial state name
			p_ai->current_state = calloc(current_state_len + 1, sizeof(char));

			// Error checking
			{
				#ifndef NDEBUG
					if(p_ai->current_state == (void *)0)
						goto no_mem;
				#endif
			}

			// Copy the initial state name
			strncpy(p_ai->current_state, initial_state, current_state_len);

		}

		SDL_UnlockMutex(instance->ai_cache_mutex);
	}

	free_memory:
	exit:
	return ret;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				ret = 0;
				goto free_memory;
			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				ret = 0;
				goto free_memory;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int add_ai_state_callback      ( GXAI_t        *p_ai , char        *state_name, int    (*function_pointer) ( GXEntity_t *entity ) )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_ai             == (void *) 0 )
				goto no_ai;
			if ( state_name       == (void *) 0 )
				goto no_state_name;
			if ( function_pointer == (void *) 0 )
				goto no_function_pointer;
		#endif
	}

	// Update the state callback
	dict_add(p_ai->states, state_name, function_pointer);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_state_name:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"state_name\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_function_pointer:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"function_pointer\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			return 0;
		}
	}
}

int set_ai_state               ( GXAI_t        *p_ai , const char  *state_name )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_ai             == (void *) 0 )
				goto no_ai;
			if ( state_name       == (void *) 0 )
				goto no_state_name;
		#endif
	}

	// Set the state, if it is a valid state
	if(dict_get(p_ai->states, state_name))
		p_ai->current_state = state_name;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_state_name:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"state_name\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int set_ai_pre_update_callback ( GXAI_t        *p_ai , int        (*function_pointer) ( GXEntity_t *entity ) )
{
	
	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_ai             == (void *) 0 )
				goto no_ai;
			if ( function_pointer == (void *) 0 )
				goto no_function_pointer;
		#endif
	}

	p_ai->pre_ai = function_pointer;

	return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_function_pointer:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"function_pointer\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int pre_update_ai              ( GXInstance_t  *instance )
{

	// Argument check
	{
		if (instance == (void*)0)
			goto no_instance;
	}

    // Initialized data
	GXEntity_t* entity = 0;

	// Get an ai entity
	{
		// Lock the mutex 
		SDL_LockMutex(instance->ai_preupdate_mutex);

		// Is there anything left to move?
		if (queue_empty(instance->ai_preupdate_queue))
		{

			// If not, unlock and return
			SDL_UnlockMutex(instance->ai_preupdate_mutex);
			return 1;
		}

		entity = queue_dequeue(instance->ai_preupdate_queue);

		SDL_UnlockMutex(instance->ai_preupdate_mutex);
	}

    // Update the AI
    if(entity)
        preupdate_entity_ai(entity);

    return 1;

	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_log("[G10] [AI] Null pointer provided for \"instance\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int copy_ai                    ( GXAI_t       **pp_ai, GXAI_t *p_ai )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if (pp_ai == (void *) 0)
				goto no_return;
			if (p_ai == (void*)0)
				goto no_ai;
		#endif
	}

	// Initialized data
	GXAI_t *ai = 0;

	// Allocate a new AI
	{
		create_ai(pp_ai);
		ai = *pp_ai;
	}

	// Copy the AI
	{
		ai->name          = p_ai->name;
		ai->current_state = p_ai->current_state;
		ai->pre_ai        = p_ai->pre_ai;
		ai->states        = p_ai->states;
	}

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_return:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int ai_info ( GXAI_t *p_ai )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if (p_ai == (void*)0)
				goto no_ai;
		#endif
	}

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
	if(state_count)
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

    return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int update_ai                  ( GXInstance_t* instance )
{

	// Argument check
	{
		if (instance == (void*)0)
			goto no_instance;
	}

    // Initialized data
	GXEntity_t* entity = 0;

	// Get an ai entity
	{
		// Lock the mutex 
		SDL_LockMutex(instance->ai_update_mutex);

		// Is there anything left to move?
		if (queue_empty(instance->ai_update_queue))
		{

			// If not, unlock and return
			SDL_UnlockMutex(instance->ai_update_mutex);
			return 1;
		}

		entity = queue_dequeue(instance->ai_update_queue);

		SDL_UnlockMutex(instance->ai_update_mutex);
	}

    // Update the AI
    if (entity)
        update_entity_ai(entity);

    return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_log("[G10] [AI] Null pointer provided for \"instance\" in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int destroy_ai                 ( GXAI_t  *p_ai )
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if(p_ai == (void *)0)
				goto no_ai;
		#endif
	}

	free(p_ai->name);
	p_ai->pre_ai = 0;

	// TODO: Free each key from the dictionary
	dict_destroy(p_ai->states);
	free(p_ai);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}
