#include <G10/GXScene.h>

void init_scene ( void )
{

    // Initialized data
    GXInstance_t* p_instance = g_get_active_instance();

    // Create a mutex for loading entities
    p_instance->mutexes.load_entity = SDL_CreateMutex();
}

int create_scene ( GXScene_t **pp_scene )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_scene == (void *) 0 ) goto no_scene;
    #endif

    // Initialized data
    GXScene_t *p_scene = calloc(1, sizeof(GXScene_t));

    // Error check
    if ( p_scene == (void *) 0 ) goto no_mem;

    // Return the allocated memory
    *pp_scene = p_scene;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"", __FUNCTION__);
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

int load_scene ( GXScene_t **pp_scene, const char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_scene == (void *) 0 ) goto no_scene;
        if ( path     == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len + 1, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the file from the file system
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_load_file;

    // Load the scene as a JSON token
    if ( load_scene_as_json_text(pp_scene, text) == 0 ) goto failed_to_load_scene;

    // Clean the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("Null pointer provided for parameter \"path\" in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load scene in call to function \"%s\"\n",__FUNCTION__);
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

int load_scene_as_json_text ( GXScene_t **pp_scene, char *text )
{

    // Argument checking
    #ifndef NDEBUG
        if ( pp_scene == (void *) 0 ) goto no_scene;
        if ( text     == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    json_value  *p_value    = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

    // Parse the JSON value into a scene
    if ( load_scene_as_json_value(pp_scene, p_value) == 0 ) goto failed_to_load_scene_as_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // JSON errors
        {
            failed_to_load_scene_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to load scene in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_parse_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to parse JSON text in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"no_text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_scene_as_json_value ( GXScene_t **pp_scene, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_scene == (void *) 0 ) goto no_scene;
        if ( p_value  == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInstance_t *p_instance           = g_get_active_instance();
    GXScene_t    *p_scene              = 0;
    json_value  *p_name_value         = 0,
                 *p_entities_value     = 0,
                 *p_cameras_value      = 0,
                 *p_lights_value       = 0,
                 *p_skyboxes_value     = 0,
                 *p_light_probes_value = 0;

    // Parse the scene as a JSON value
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name_value         = dict_get(p_dict, "name");

        // Optional properties
        p_entities_value     = dict_get(p_dict, "entities");
        p_cameras_value      = dict_get(p_dict, "cameras");
        p_lights_value       = dict_get(p_dict, "lights");
        p_skyboxes_value     = dict_get(p_dict, "skyboxes");
        p_light_probes_value = dict_get(p_dict, "light probes");

        // Error check
        if ( ! ( 
            p_name_value
        ) )
            goto missing_properties;
    }
    // Parse the scene as a file path
    else if ( p_value->type == JSON_VALUE_STRING )
    {

        // Load the scene as a path
        if ( load_scene(pp_scene, p_value->string) == 0 ) goto failed_to_load_scene_as_path;

        // Success
        return 1;
    }
    // Default
    else
        goto wrong_value_type;

    // Construct the scene
    {

        // Allocate a scene
        if ( create_scene(&p_scene) == 0 ) goto failed_to_allocate_scene;

        // Set the name
        if ( p_name_value->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name_value->string);

            // Allocate memory
            p_scene->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_scene->name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(p_scene->name, p_name_value->string, name_len);
        }
        // Default
        else
            goto name_type_error;

        // Load entities
        if ( p_entities_value )
        {

            // Parse the entities as an array
            if ( p_entities_value->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                GXThread_t  **entity_loading_threads = calloc(p_instance->loading_thread_count, sizeof(void *));
                size_t        len                    = 0;
                json_value **pp_entities            = 0;

                // This is used when creating loading threads
                extern int load_entity_from_queue(void *vp_instance);

                // Get the array contents
                {

                    // Get the array length
                    array_get(p_entities_value->list, 0, &len);

                    // Allocate memory for entities
                    pp_entities = calloc(len+1, sizeof(json_value *));

                    // Error check
                    if ( pp_entities == (void *) 0 ) goto no_mem;

                    // Get list of entities
                    array_get(p_entities_value->list, (void **)pp_entities, 0);
                }

                // Empty the active instances -> entity loading queue
                while ( queue_dequeue(p_instance->queues.load_entity, 0) != 0 );

                // Iterate over each entity
                for (size_t i = 0; i < len; i++)

                    // Add each entity to a queue
                    queue_enqueue(p_instance->queues.load_entity, pp_entities[i]);

                // Construct entity dicts
                dict_construct(&p_scene->entities, len);
                dict_construct(&p_scene->actors, len);
                dict_construct(&p_scene->ais, len);

                // Set the active instance's loading scene
                p_instance->context.loading_scene = p_scene;

                // Iterate N times
                for (size_t i = 0; i < p_instance->loading_thread_count; i++)
                {

                    // Initialized data
                    GXThread_t *thread = 0;

                    // Allocate a thread
                    if ( create_thread(&thread) == 0 ) goto failed_to_create_thread;

                    // Store the thread
                    entity_loading_threads[i] = thread;

                    // Spawn a thread
                    thread->thread = SDL_CreateThread(load_entity_from_queue, 0, p_instance);

                }

                ////////////////////////////////////////
                // [Threads are now loading entities] //
                ////////////////////////////////////////

                // Iterate over each loading thread
                for (size_t i = 0; i < p_instance->loading_thread_count; i++)
                {

                    // Unneccisary data
                    int r_stat = 0;

                    // Wait for each thread to stop
                    SDL_WaitThread(entity_loading_threads[i]->thread, &r_stat);
                }
            }
            // Default
            else
                goto entities_type_error;
        }

        // Load cameras
        if ( p_cameras_value )
        {

            // Parse the cameras as an array
            if ( p_cameras_value->type == JSON_VALUE_ARRAY )
            {

                // Initialized data
                size_t        len        = 0;
                json_value **pp_cameras = 0;

                // Get the array contents
                {

                    // Get the array length
                    array_get(p_cameras_value->list, 0, &len);

                    // Allocate memory for entities
                    pp_cameras = calloc(len+1, sizeof(json_value *));

                    // Error check
                    if ( pp_cameras == (void *) 0 ) goto no_mem;

                    // Get list of entities
                    array_get(p_cameras_value->list, (void **)pp_cameras, 0);
                }

                // Construct a camera dict
                dict_construct(&p_scene->cameras, len);

                // Iterate over each JSON value
                for (size_t i = 0; i < len; i++)
                {

                    // Initialized data
                    GXCamera_t *p_camera = 0;

                    // Load a camera as a JSON value
                    if ( load_camera_as_json_value(&p_camera, pp_cameras[i]) == 0 ) goto failed_to_load_camera_as_json_value;

                    // Add the camera to the camera dictionary
                    dict_add(p_scene->cameras, p_camera->name, p_camera);
                }
            }
            // Parse the cameras as an object
            else if ( p_cameras_value->type == JSON_VALUE_OBJECT )
            {

                // Initialized data
                GXCamera_t *p_camera = 0;

                // Construct a camera dict
                // TODO: Replace with a constant?
                dict_construct(&p_scene->cameras, 16);

                // Load a camera as a JSON value
                if ( load_camera_as_json_value(&p_camera, p_cameras_value->object) == 0 ) goto failed_to_load_camera_as_json_value;

                // Add the camera to the camera dictionary
                dict_add(p_scene->cameras, p_camera->name, p_camera);
            }
            // Default
            else
                goto cameras_type_error;
        }

        // Load lights
        if ( p_lights_value )
        {
            
            // Parse the lights as an array
            if ( p_lights_value->type == JSON_VALUE_ARRAY )
            {
                // TODO:
            }
            // Default
            else
                goto lights_type_error;
        }

        // Load light probes
        if ( p_light_probes_value )
        {

            // Parse the light probes as an array
            if ( p_lights_value->type == JSON_VALUE_ARRAY )
            {
                // TODO: 
            }
            // Default
            else
                goto light_probes_type_error;
        }

        // TODO: Uncomment
        // Construct a bounding volume hierarchy tree from the entities in the scene
        //construct_bvh_from_scene(&p_scene->bvh, p_scene);

        // Allocate a list to store collisions
        // TODO: Replace with a constant?
        p_scene->collisions = calloc(16, sizeof (GXCollision_t *));

        // Return a pointer to the caller
        *pp_scene = p_scene;
    }

    // Success
    return 1;    

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // JSON errors
        {

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Not enough properties to construct scene in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            name_type_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            entities_type_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Property \"entities\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            cameras_type_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Property \"cameras\" must be of type [ object | array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            lights_type_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Property \"lights\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            light_probes_type_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Property \"light probes\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/scene.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_load_camera_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to load camera from JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to allocate memory for scene in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            failed_to_load_scene_as_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to load scene from path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            failed_to_create_thread:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to allocate thread in call to function \"%s\"\n", __FUNCTION__);
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

int append_entity ( GXScene_t *p_scene, GXEntity_t *entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene           == (void *) 0 ) goto no_scene;
        if ( p_scene->entities == (void *) 0 ) goto no_entities;
        if ( entity            == (void *) 0 ) goto no_entity;
        if ( entity->name      == (void *) 0 ) goto no_name;
    #endif

        
    // Add the entity to the scene
    (void) dict_add(p_scene->entities, entity->name, entity);

    // If the entity has a rigidbody, add it to the actor list
    if ( entity->rigidbody )
        (void) dict_add(p_scene->actors, entity->name, entity);

    // If the entity has an AI, add it to the AI list
    if ( entity->ai )
        (void) dict_add(p_scene->ais, entity->name, entity);

    // TODO: Additional state updates
    //

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_entities:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] No entity dictionary in \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Parameter \"entity\" has no name in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

int append_camera ( GXScene_t *p_scene, GXCamera_t *p_camera )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene          == (void *) 0 ) goto no_scene;
        if ( p_scene->cameras == (void *) 0 ) goto no_cameras;
        if ( p_camera         == (void *) 0 ) goto no_camera;
        if ( p_camera->name   == (void *) 0 ) goto no_name;
    #endif

    // Add the camera to the scene
    (void) dict_add(p_scene->cameras, p_camera->name, p_camera);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_cameras:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] No camera dictionary in parameter \"p_scene\", in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Parameter \"camera\" has no name in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int append_light ( GXScene_t *p_scene, GXLight_t *p_light )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene         == (void *) 0 ) goto no_scene;
        if ( p_scene->lights == (void *) 0 ) goto no_lights;
        if ( p_light         == (void *) 0 ) goto no_light;
        if ( p_light->name   == (void *) 0 ) goto no_name;
    #endif

    // Add the entity to the scene
    (void) dict_add(p_scene->lights, p_light->name, p_light);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_lights:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] No light dictionary in parameter \"p_scene\", in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_light:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_light\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Parameter \"p_light\" has no name in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int append_collision ( GXScene_t *p_scene, GXCollision_t *p_collision )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene     == (void *) 0 ) goto no_scene;
        if ( p_collision == (void *) 0 ) goto no_collision;
    #endif
    
    // TODO:
    //

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_collision:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_collision\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int scene_info ( GXScene_t *p_scene )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene == (void *) 0 ) goto no_scene;
    #endif

    // Initialized data
    size_t       entity_count            = dict_keys(p_scene->entities, 0),
                 re                      = entity_count,
                 fs                      = 1,
                 longest_entity_name_len = 0;
    char       **entity_names            = calloc(entity_count, sizeof(void *));
    GXEntity_t **entity_pointers         = calloc(entity_count, sizeof(void *));

    // Compute decimal places through repeated division
    while ( re >= 10 ) { re /= 10; fs++; }

    // Get the name of each entity
    dict_keys(p_scene->entities, entity_names);

    // Get a pointer to each entity
    dict_values(p_scene->entities, (void **)entity_pointers);

    // Get the longest entity name
    for (size_t i = 0; i < entity_count; i++)
    {

        // Initialized data
        size_t entity_name_len = strlen(entity_names[i]);

        // Check for a longer entity name
        longest_entity_name_len = (entity_name_len > longest_entity_name_len) ? entity_name_len : longest_entity_name_len;
    }

    // Formatting
    g_print_log("\n - Scene info -\n");

    // Print the name of the scene
    g_print_log("name     : \"%s\"\n", p_scene->name);
    
    // Formatting
    g_print_log("entities :\n");

    // Print each entity
    // TODO: Fix
    
    // Get the callback function associated with the current state
    dict_foreach(p_scene->entities, ( void (*)(const void* const , size_t) ) entity_info);

    // Formatting
    g_print_log("\ncameras  :\n");

    // Print each camera
    dict_foreach(p_scene->cameras, ( void (*)(const void*const, size_t ) ) print_camera);

    // Formatting
    g_print_log("\n");

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

GXEntity_t *get_entity ( GXScene_t *p_scene, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene == (void *) 0 ) goto no_scene;
        if ( name    == (void *) 0 ) goto no_name;
    #endif

    // Success OR null pointer if name is not in scene->entities
    return (GXEntity_t *) dict_get(p_scene->entities, (char *)name);

    // Error handling
    {
        no_scene:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

GXCamera_t *get_camera ( GXScene_t *p_scene, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene == (void *) 0 ) goto no_scene;
        if ( name    == (void *) 0 ) goto no_name;
    #endif

    return (GXCamera_t *) dict_get(p_scene->cameras, (char *)name);

    // Error handling
    {
        no_scene:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

GXLight_t *get_light ( GXScene_t *p_scene, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene == (void *) 0 ) goto no_scene;
        if ( name    == (void *) 0 ) goto no_name;
    #endif

    // Return 
    return (GXLight_t *) dict_get(p_scene->lights, (char *) name);

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int set_active_camera ( GXScene_t *p_scene, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_scene == (void *) 0 ) goto no_scene;
        if ( name    == (void *) 0 ) goto no_name;
    #endif

    // Initialized data
    GXCamera_t *c = (GXCamera_t *) dict_get(p_scene->cameras, (char *) name);

    // Is the requested camera real?
    if (c)

        // Set the active camera
        p_scene->active_camera = c;

    // Default
    else
        goto failed_to_find_camera;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Data errors
        {
            failed_to_find_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to find camera \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_scene ( GXScene_t **pp_scene )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_scene == (void *) 0 ) goto no_scene;
    #endif

    // Initialized data
    GXScene_t *p_scene = *pp_scene;

    // Check for valid pointer
    if ( p_scene == (void *) 0 ) goto pointer_to_null_pointer;

    // No more pointer for caller
    *pp_scene = 0;

    // Free the scene name
    free(p_scene->name);

    // Free the scene entities
    if ( p_scene->entities )
    {

        // TODO: Fix
        // Free the dictionary and free every value
        //dict_free_clear(p_scene->entities, destroy_entity);

        // Destroy the entity dictionary
        dict_destroy(&p_scene->entities);

    }

    // Free the cameras
    if ( p_scene->cameras )
    {

        // TODO: Fix
        // Clear the dictionary and free every value
        //dict_free_clear(p_scene->cameras, destroy_camera);

        // Destroy the camera dictionary
        dict_destroy(&p_scene->cameras);

    }

    // TODO: Uncomment
    // Free the lights
    /*
    if ( p_scene->lights )
    {
        size_t light_count = dict_keys(p_scene->lights, 0);
        GXLight_t** lights = calloc(light_count, sizeof(void*));

        // Error checking
        if ( lights == (void *) 0 )
            goto no_mem;

        dict_values(p_scene->lights, lights);

        // Iterate over each camera
        for (size_t i = 0; i < light_count; i++)

            // Destroy the light
            destroy_light(lights[i]);

        // Free the list
        free(lights);

        // Destroy the light dictionary
        dict_destroy(p_scene->lights);
    }
    */

    // Free the scene
    free(p_scene);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null_pointer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Parameter \"pp_scene\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}
