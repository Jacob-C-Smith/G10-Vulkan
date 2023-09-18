#include <G10/GXScheduler.h>

#define TASK_COUNT 23

#ifdef BUILD_G10_WITH_DISCORD
#include <G10/GXDiscordIntegration.h>
#endif

// Forward declarations
int load_task_as_json ( GXTask_t **pp_task, char *text );
int load_thread_as_json_value ( GXThread_t **pp_thread, json_value *p_value );
int load_task_as_json_value ( GXTask_t **pp_task, json_value *p_value );

dict *scheduler_tasks = 0;

char *task_names[TASK_COUNT] = {
    "Input",
    "UI",
    "AI",
    "Pre AI",
    "User Code",
    "Resolve Collisions",
    "Update Forces",
    "Move Objects",
    "Animation",
    "Render",
    "Present",
    "Load Entity",
    "Load Light Probe",
    "Copy State",
    "Server Recieve",
    "Server Send",
    "Server Parse",
    "Server Serialize",
    "Server Process",
    "Server Wait",
    "Audio",
    #ifdef BUILD_G10_WITH_DISCORD
    "Discord Callbacks",
    #else
    "",
    #endif
    ""
};

void *task_function_pointers[TASK_COUNT] = {
    &process_input,
    (void*) 0,
    &update_ai,
    &pre_update_ai,
    &user_code,
    &detect_collisions,
    &update_forces,
    &move_objects,
    0,//&update_rigs,
    0,//&render_frame,
    0,//&present_frame,
    0,//&load_entity_from_queue,
    0,//&load_light_probe_from_queue,
    &copy_state,
    0,//&server_recv,
    0,//&server_send,
    0,//&server_parse,
    0,//&server_serialize,
    0,//&server_process,
    0,//&server_wait,
    0,//&process_audio,
    #ifdef BUILD_G10_WITH_DISCORD
    &discord_callbacks,
    #else
    0,
    #endif
    0
};

int init_scheduler ( void )
{

    // Construct a function lookup table
    if ( dict_construct(&scheduler_tasks, TASK_COUNT) == 0 ) goto failed_to_construct_scheduler_tasks_lookup_table;

    // Iterate over each task
    for (size_t i = 0; i < TASK_COUNT; i++)

        // Add each task to the lookup table
        dict_add(scheduler_tasks, task_names[i], task_function_pointers[i]);

    // Success
    return 1;

    // Error handling
    {

        // G10 errors
        {
            failed_to_construct_scheduler_tasks_lookup_table:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to construct scheduler tasks lookup table in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

int create_task ( GXTask_t **pp_task )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_task == (void *) 0 ) goto no_task;
    #endif

    // Initialized data
    GXTask_t *p_task = calloc(1, sizeof(GXTask_t));

    // Error check
    if ( p_task == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_task = p_task;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_task:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"pp_task\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_schedule ( GXSchedule_t **pp_schedule )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_schedule == (void *) 0 ) goto no_schedule;
    #endif

    // Initialized data
    GXSchedule_t *p_scheduler = calloc(1, sizeof(GXSchedule_t));

    // Error check
    if ( p_scheduler == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_schedule = p_scheduler;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_thread ( GXThread_t **pp_thread )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_thread == (void *) 0 ) goto no_thread;
    #endif

    // Initialized data
    GXThread_t *p_thread = calloc(1, sizeof(GXThread_t));

    // Error check
    if ( p_thread == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_thread = p_thread;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_schedule ( GXSchedule_t **pp_schedule, char* path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_schedule == (void *) 0 ) goto no_schedule;
        if ( path        == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len+1, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0) goto failed_to_load_file;

    // Construct the schedule from the file contents
    if ( load_schedule_as_json_text(pp_schedule, text) == 0 ) goto failed_to_load_schedule;

    // Free the file contents
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_log("[G10] [Scheduler] Null pointer provided for parameter \"schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_log("[G10] [Scheduler] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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

        // G10 Errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to load scheduler from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_schedule_as_json_text ( GXSchedule_t **pp_schedule, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_schedule == (void *) 0 ) goto no_schedule;
        if ( text        == (void *) 0 ) goto no_text;
    #endif
    

    // Initialized data
    GXInstance_t  *p_instance    = g_get_active_instance();
    char          *name          = 0;
    array         *p_threads     = 0;
    json_value   *p_value       = 0;

    // Parse the text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Load the schedule as a JSON value
    if ( load_schedule_as_json_value(pp_schedule, p_value) == 0 ) goto failed_to_create_schedule;

    // Deallocate the JSON value
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_create_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to parse JSON in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_schedule_as_json_value ( GXSchedule_t **pp_schedule, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_schedule == (void *) 0 ) goto no_schedule;
        if ( p_value     == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance    = g_get_active_instance();
    json_value  *p_name        = 0,
                 *p_threads     = 0;

    // Parse the schedule as an object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Parse the JSON values into constructor parameters
        p_name    = dict_get(p_dict, "name");
        p_threads = dict_get(p_dict, "threads");

        // Error check
        if ( ! ( 
            p_name    &&
            p_threads 
        ) )
            goto missing_properties;
    }
    // Parse the schedule as a path
    else if ( p_value->type == JSON_VALUE_STRING )
    {

        // Load the schedule from the path
        if ( load_schedule(pp_schedule, p_value->string) == 0 ) goto failed_to_load_schedule;

        // Success
        return 1;
    }
    // Default
    else
        goto wrong_type;

    // Construct the schedule
    {

        // Initialized data
        GXSchedule_t *p_schedule = 0;
        dict         *threads    = 0;
        char         *name       = 0;

        // Copy the schedule name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Error check
            if ( name_len > 255 ) goto name_too_long;

            // Allocate for the name
            name = calloc(name_len+1, sizeof(char));

            // Error check
            if ( name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(name, p_name->string, name_len);

        }

        // Construct schedule threads
        {

            // Initialized data
            json_value **pp_elements  = 0;
            size_t        thread_count = 0;

            // Get the array contents
            {

                // Get the quantity of elements
                array_get(p_threads->list, 0, &thread_count);

                // Allocate an array for the elements
                pp_elements = calloc(thread_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the threads
                array_get(p_threads->list, (void **)pp_elements, 0 );
            }

            // Construct a thread dict
            dict_construct(&threads, thread_count);

            // Set up the threads
            for (size_t i = 0; i < thread_count; i++)
            {

                // Initialized data
                json_value *p_thread_json_value = pp_elements[i];
                GXThread_t  *p_thread            = 0;

                // Type check
                if ( p_thread_json_value->type != JSON_VALUE_OBJECT ) goto wrong_thread_type;

                // Load the thread
                if ( load_thread_as_json_value(&p_thread, p_thread_json_value) == 0 ) goto failed_to_load_thread;

                // Add the thread to the thread dict
                dict_add(threads, p_thread->name, p_thread);
            }

            // Clean the scope
            free(pp_elements);
        }

        // Allocate the schedule
        if ( create_schedule(&p_schedule) == 0 ) goto failed_to_create_schedule;

        // Construct the schedule
        *p_schedule = (GXSchedule_t)
        {
            .name    = name,
            .threads = threads
        };

        // Return the schedule to the caller
        *pp_schedule = p_schedule;

    }

    // Success
    return 1;

    // Error handling
    {

        // JSON parsing errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to parse JSON in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Expected a JSON object in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_too_long:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] \"name\" property's length must be less than 256 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Not enough properties to construct schedule in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_thread_type:
                #ifndef NDEBUG
                    g_print_error("[G10] Element in property \"threads\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {

            failed_to_create_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to load scheduler as path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
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

int client_work ( GXClient_t *p_client )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_client == (void *) 0 ) goto no_client;
    #endif

    // Initialized data
    GXThread_t    *thread   = p_client->thread;
    GXTask_t     **tasks    = thread->tasks;
    GXInstance_t  *p_instance = g_get_active_instance();

    // Run until told otherwise
    while (thread->running)
    {

        // Iterate over each task in the thread
        for (size_t i = 0; i < thread->task_count; i++)
        {

            // Wait on other things to finish?
            if (thread->tasks[i]->wait_thread)
            {

                // Initialized data
                GXThread_t *wait_thread = (GXThread_t *) dict_get(p_instance->context.schedule->threads, thread->tasks[i]->wait_thread);
                GXTask_t   *wait_task   = 0;
                size_t      v           = 0;

                // Figure out what task the program is waiting on
                for (size_t j = 0; j < wait_thread->task_count; j++)
                {
                    if (strcmp(thread->tasks[i]->wait_task, wait_thread->tasks[j]->name) == 0)
                        v = j;
                }

                // Wait for the task to finish
                while (wait_thread->complete_tasks[i] == 0);
            }

            // Declare the task function
            int (*function_pointer)(GXClient_t*) = (int (*)(GXClient_t*))thread->tasks[i]->function_pointer;

            // Run the function
            if ( function_pointer )
                function_pointer(p_client);

            // Update the task
            thread->complete_tasks[i] = 1;
        }

        // Reset each frame
        for (size_t i = 0; i < thread->task_count; i++)
            thread->complete_tasks[i] = 0;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_client:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"p_client\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int work ( void *vp_thread )
{

    // Argument check
    #ifndef NDEBUG
        if ( vp_thread == (void *) 0 ) goto no_thread;
    #endif

    // Initialized data
    GXThread_t    *p_thread   = vp_thread;
    GXTask_t     **tasks      = p_thread->tasks;
    GXInstance_t  *p_instance = g_get_active_instance();

    // Run until told otherwise
    while ( p_thread->running )
    {

        // Iterate over each task
        for (size_t i = 0; i < p_thread->task_count; i++)
        {

            // Is the program waiting on anything else to finish?
            if ( p_thread->tasks[i]->wait_thread )
            {

                // Initialized data
                GXThread_t *wait_thread = (GXThread_t *) dict_get(p_instance->context.schedule->threads, p_thread->tasks[i]->wait_thread);
                GXTask_t   *wait_task   = 0;
                size_t      v           = 0;

                // Figure out what task the program is waiting on
                for (size_t j = 0; j < wait_thread->task_count; j++)
                {
                    if ( strcmp(p_thread->tasks[i]->wait_task, wait_thread->tasks[j]->name) == 0 )
                        v = j;
                }

                // Wait for the task to finish
                while (wait_thread->complete_tasks[i] == 0);

            }

            int (*function_pointer)(GXInstance_t*) = p_thread->tasks[i]->function_pointer;

            // Run the function
            if ( function_pointer )
                function_pointer(g_get_active_instance());

            // Update the task
            p_thread->complete_tasks[i] = 1;
        }

        // Reset
        for (size_t i = 0; i < p_thread->task_count; i++)
            p_thread->complete_tasks[i] = 0;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"vp_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int main_work ( GXThread_t *p_thread )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_thread == (void *) 0 ) goto no_thread;
    #endif

    // Initialized data
    GXTask_t     **tasks  = p_thread->tasks;
    GXInstance_t  *p_instance = g_get_active_instance();

    // Run until told otherwise
    while (p_thread->running)
    {

        // Iterate over each task
        for (size_t i = 0; i < p_thread->task_count; i++)
        {

            // Initialized data
            int (*function_pointer)(GXInstance_t*) = 0;

            // Is the program waiting on anything else to finish?
            if (p_thread->tasks[i]->wait_thread)
            {

                // Initialized data
                GXThread_t *wait_thread = (GXThread_t *) dict_get(p_instance->context.schedule->threads, p_thread->tasks[i]->wait_thread);
                GXTask_t   *wait_task   = 0;
                size_t      v           = 0;

                // Figure out what task the program is waiting on
                for (size_t j = 0; j < wait_thread->task_count; j++)
                {
                    if ( strcmp(p_thread->tasks[i]->wait_task, wait_thread->tasks[j]->name) == 0 )
                        v = j;
                }

                // Wait for the task to finish
                while (wait_thread->complete_tasks[i] == 0);

            }

            function_pointer = p_thread->tasks[i]->function_pointer;

            // Run the function
            if ( function_pointer )
                function_pointer(g_get_active_instance());

            // Update the task
            p_thread->complete_tasks[i] = 1;
        }

        // Reset
        for (size_t i = 0; i < p_thread->task_count; i++)
            p_thread->complete_tasks[i] = 0;

    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"p_thread\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int start_schedule ( GXSchedule_t *p_schedule )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_schedule == (void *) 0 ) goto no_schedule;
    #endif

    // Initialized data
    GXInstance_t *p_instance              = g_get_active_instance();
    size_t        schedule_thread_count = dict_values(p_schedule->threads, 0);
    GXThread_t  **schedule_threads      = calloc(schedule_thread_count, sizeof(void *));

    // Set the active schedule
    p_instance->context.schedule = p_schedule;

    // TODO: Uncomment
    // Copy the state
    copy_state(p_instance);

    // Get a list of threads
    dict_values(p_schedule->threads, (void **)schedule_threads);

    // Iterate over each thread
    for (size_t i = 0; i < schedule_thread_count; i++)
    {

        // Initialized data
        GXThread_t *thread = schedule_threads[i];

        // Don't spawn a new thread for the main thread
        // TODO: user defined main thread name
        if ( strcmp(thread->name, "Main Thread") == 0 )
            continue;

        // Set the thread to run
        thread->running = true;

        // Create the thread
        thread->thread  = SDL_CreateThread(work, thread->name, thread);
    }

    // Get the main thread
    GXThread_t *main_thread = (GXThread_t *) dict_get(p_schedule->threads, "Main Thread");

    // Set the thread to run
    main_thread->running = true;

    // Call the thread worker function (on the main thread...)
    main_work(main_thread);

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int stop_schedule ( GXSchedule_t *schedule )
{
    // TODO: Argument check

    // Initialized data
    GXInstance_t *p_instance              = g_get_active_instance();
    size_t        schedule_thread_count = dict_values(schedule->threads, 0);
    int           r_stat                = 0;
    GXThread_t  **schedule_threads      = calloc(schedule_thread_count+1, sizeof(void *));

    // Get a list of threads
    dict_values(schedule->threads, (void **)schedule_threads);

    // Iterate over each thread
    for (size_t i = 0; i < schedule_thread_count; i++)
    {

        // Initialized data
        GXThread_t* thread = schedule_threads[i];

        // Stop the thrad
        thread->running = false;

        // Wrap up every task
        for (size_t i = 0; i < thread->task_count; i++)
            thread->complete_tasks[i] = 1;
    }

    // Iterate over each thread
    for (size_t i = 0; i < schedule_thread_count; i++)

        // Wait for each thread to finish
        SDL_WaitThread(schedule_threads[i]->thread, &r_stat);

    // Success
    return 1;

    return 0;
}

int load_thread_as_json_value ( GXThread_t **pp_thread, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_thread == (void *) 0 ) goto no_thread;
        if ( p_value   == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXThread_t  *p_thread    = 0;
    char        *name        = 0,
                *description = 0;
    size_t       task_count  = 0;
    array       *p_tasks     = 0;

    // TODO: Refactor to use json_value *
    // Parse the thread JSON
    if (p_value->type == JSON_VALUE_OBJECT)
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Parse the JSON values into constructor parameters
        name        = ((json_value *)dict_get(p_dict, "name"))->string;
        description = ((json_value *)dict_get(p_dict, "description"))->string;
        p_tasks     = ((json_value *)dict_get(p_dict, "tasks"))->list;

        // Error check
        if ( ( name && description && p_tasks ) == 0 ) goto missing_properties;
    }
    else
        goto wrong_type;

    // Construct the thread
    {

        // Initialized data
        GXThread_t *p_thread       = 0;
        char       *p_name         = 0;
        size_t     *complete_tasks = 0;
        dict       *d_tasks        = 0;
        GXTask_t  **tasks          = calloc(task_count+1, sizeof(GXTask_t *));

        // Copy the thread name
        {

            // Initialized data
            size_t name_len = strlen(name);

            // Allocate for the name
            p_name = calloc(name_len+1, sizeof(char));

            // Error check
            if ( p_name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_name, name, name_len);

        }

        // Construct thread tasks
        {

            // Initialized data
            json_value **pp_elements  = 0;

            // Dump the array contents
            {

                // Get the quantity of elements
                array_get(p_tasks, 0, &task_count);

                // Allocate an array for the elements
                pp_elements = calloc(task_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the tasks
                array_get(p_tasks, (void **)pp_elements, 0 );
            }
            
            // Construct a task dict
            dict_construct(&d_tasks, task_count);

            // Construct a list of tasks
            tasks = calloc(task_count+1, sizeof(GXTask_t *));

            // Error check
            if ( tasks == (void *) 0 ) goto no_mem;

            // Construct a list of size_t s
            complete_tasks = calloc(task_count+1, sizeof(size_t));

            // Error check
            if ( complete_tasks == (void *) 0 ) goto no_mem;

            // Set up the tasks
            for (size_t i = 0; i < task_count; i++)
            {

                // Initialized data
                json_value *p_thread_json_value = pp_elements[i];
                GXTask_t    *p_task              = 0;

                // Type check
                if ( p_thread_json_value->type != JSON_VALUE_OBJECT )
                    return 0; // TODO: Error handling

                // Load the thread
                if ( load_task_as_json_value(&p_task, p_thread_json_value) == 0 ) goto failed_to_load_thread;

                // Add the thread to the thread dict
                dict_add(d_tasks, p_task->name, p_task);

                // Set the task in the list
                tasks[i] = p_task;

            }

            // Clean the scope
            free(pp_elements);
        }

        // Allocate the thread
        if ( create_thread(&p_thread) == 0 ) goto failed_to_create_thread;

        // Return the transform to the caller
        *p_thread = (GXThread_t)
        {
            .name           = p_name,
            .task_count     = task_count,
            .complete_tasks = complete_tasks,
            .tasks          = tasks,
        };

        *pp_thread = p_thread;

    }

    // Success
    return 1;

    // Error handling
    {

        // JSON parsing errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to parse JSON in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Expected a JSON object in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Not enough properties to construct thread in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {

            failed_to_create_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to create thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Argument errors
        {
            no_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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

        no_value:
        return 0;
    }

}

int load_task_as_json_value ( GXTask_t **pp_task, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_task == (void *) 0 ) goto no_task;
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXTask_t *p_task      = 0;
    char     *task_name   = 0,
             *wait_thread = 0,
             *wait_task   = 0;

    // Allocate memory for the task struct
    if ( create_task(&p_task) == 0 ) goto failed_to_create_task;

    // TODO: Refactor to use json_value *
    // Parse the task JSON
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;
        
        // Parse the JSON values into constructor parameters
        task_name   = ((json_value *)dict_get(p_dict, "task"))->string;

        if ( dict_get(p_dict, "wait thread") )
            wait_thread = ((json_value *)dict_get(p_dict, "wait thread"))->string;

        if ( dict_get(p_dict, "wait thread") )
            wait_task   = ((json_value *)dict_get(p_dict, "wait task"))->string;

        // Error check
        if ( ( task_name ) == 0 ) goto missing_properties;
    }
    // Default
    else
        goto wrong_type;

    // Construct the task
    {

        // Copy the name
        {

            // Initialized data
            size_t task_name_len = strlen(task_name);

            // Allocate memory for the name
            p_task->name = calloc(task_name_len+1, sizeof(char));

            // Error handling
            if ( p_task->name == 0 ) goto no_mem;

            // Copy the name
            strncpy(p_task->name, task_name, task_name_len);
        }

        // Copy the wait thread
        if ( wait_thread )
        {

            // Initialized data
            size_t wait_thread_name_len = strlen(wait_thread);

            // Allocate memory for the name
            p_task->wait_thread = calloc(wait_thread_name_len+1, sizeof(char));

            // Error handling
            if ( p_task->wait_thread == 0 ) goto no_mem;

            // Copy the name
            strncpy(p_task->wait_thread, wait_thread, wait_thread_name_len);
        }

        // Copy the wait task
        if ( wait_task )
        {

            // Initialized data
            size_t wait_task_name_len = strlen(wait_task);

            // Allocate memory for the name
            p_task->wait_task = calloc(wait_task_name_len+1, sizeof(char));

            // Error handling
            if ( p_task->name == 0 ) goto no_mem;

            // Copy the name
            strncpy(p_task->wait_task, wait_task, wait_task_name_len);
        }
    }

    // Get a function pointer from the list
    p_task->function_pointer = (int (*)(GXInstance_t *)) dict_get(scheduler_tasks, p_task->name);

    // Get a pointer to the allocated memory
    *pp_task = p_task;

    // Success
    return 1;

    // Error handling
    {
        no_task:
        no_value:return 0;
        // JSON parsing errors
        {
            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Expected a JSON object in call to function \"%s\"\n Refer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Not enough properties to construct task in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
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
    failed_to_create_task:;

    return 0;
}