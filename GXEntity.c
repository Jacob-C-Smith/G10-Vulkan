#include <G10/GXEntity.h>

vec3 calculate_force_gravitational(GXEntity_t* entity);
vec3 calculate_force_applied(GXEntity_t* entity);
vec3 calculate_force_normal(GXEntity_t* entity);
vec3 calculate_force_friction(GXEntity_t* entity);
vec3 calculate_force_tension(GXEntity_t* entity);
vec3 calculate_force_spring(GXEntity_t* entity);

int create_entity ( GXEntity_t **pp_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    GXEntity_t *p_entity = calloc(1,sizeof(GXEntity_t));

    // Error check
    if ( p_entity == (void *) 0 ) goto no_mem;

    // Return a pointer to the caller
    *pp_entity = p_entity;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_entity ( GXEntity_t** pp_entity, char* path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_entity == (void *) 0 ) goto no_entity;
        if ( path      == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len+1, sizeof(char));

    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the entity file
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_load_entity;

    // Load the entity as JSON text
    if ( load_entity_as_json_text(pp_entity, text) == 0 ) goto failed_to_load_entity_as_json;

    // Clean up the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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
            failed_to_load_entity_as_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to open file \"%s\" text in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to load entity from \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_entity_as_json_text ( GXEntity_t** pp_entity, char* text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_entity == (void *) 0 ) goto no_entity;
        if ( text      == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    JSONValue_t *p_value = 0;

    // Parse the JSON
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Load the entity as a JSON value
    if ( load_entity_as_json_value(pp_entity, p_value) == 0 ) goto failed_to_load_entity_as_json_value;

    // Clean up the scope
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {

            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_entity_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to construct entity from JSON text in call to function\"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_entity_as_json_value ( GXEntity_t **pp_entity, JSONValue_t *p_value )
{

    // Argument Check
    #ifndef NDEBUG
        if ( pp_entity == (void *) 0 ) goto no_entity;
        if ( p_value   == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXEntity_t  *p_entity            = 0;
    JSONValue_t *p_name_value        = 0,
                *p_parts_value       = 0,
                *p_materials_value   = 0,
                *p_shader_name_value = 0,
                *p_transform_value   = 0,
                *p_rigidbody_value   = 0,
                *p_collider_value    = 0,
                *p_ai_value          = 0;

    // Parse the JSON value
    if ( p_value->type == JSONobject )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name_value        = dict_get(p_dict, "name");

        // Optional properties
        p_parts_value       = dict_get(p_dict, "parts");
        p_materials_value   = dict_get(p_dict, "materials");
        p_shader_name_value = dict_get(p_dict, "shader");
        p_transform_value   = dict_get(p_dict, "transform");
        p_rigidbody_value   = dict_get(p_dict, "rigidbody");
        p_collider_value    = dict_get(p_dict, "collider");
        p_ai_value          = dict_get(p_dict, "ai");

        // Error check
        if ( ! (
            p_name_value 
            /*&& [REQUIRED PROPERTEIS]*/
        ) ) 
            goto missing_properties;
    }
    // Default
    else
        goto wrong_value_type;

    // Construct the entity
    {

        // Allocate the entity
        if ( create_entity(&p_entity) == 0 ) goto failed_to_allocate_entity;

        // Set the name
        if ( p_name_value->type == JSONstring )
        {

            // Initialized data
            size_t name_len = strlen(p_name_value->string);

            // Allocate memory for the name
            p_entity->name  = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_entity->name == (void *) 0 ) goto no_mem;

            // Copy the name
            (void)strncpy(p_entity->name, p_name_value->string, name_len);
        }
        // Default
        else
            goto wrong_name_type;

        // Parts
        if ( p_parts_value )
        {

            // Parse each part
            if ( p_parts_value->type == JSONarray )
            {

                // Initialized data
                size_t        part_count = 0;
                JSONValue_t **parts      = 0;

                // Construct a dictionary
                dict_construct(&p_entity->parts, 16);

                // Get the array
                {

                    // Get the size of the array
                    (void)array_get(p_parts_value->list, 0, &part_count);

                    // Allocate memory for a list of parts
                    parts = calloc(part_count+1, sizeof(JSONValue_t *));

                    // Error check
                    if ( parts == (void *) 0 ) goto no_mem;

                    // Get each array element
                    (void)array_get(p_parts_value->list, (void **)parts, &part_count);
                }

                // Iterate over each array element
                for (size_t i = 0; i < part_count; i++)
                {

                    // Initialized data
                    GXPart_t *p_part = 0;

                    // Load the part
                    if ( load_part_as_json_value(&p_part, parts[i]) == 0 ) goto failed_to_load_part;

                    // Add the part to the dictionary
                    dict_add(p_entity->parts, p_part->name, p_part);
                }

                // Clean the scope
                free(parts);
            }
            // Default
            else
                goto wrong_parts_type;
        }

        // Materials
        // TODO

        // Shader
        if ( p_shader_name_value )
        {

            // Copy the name of the shader
            if ( p_shader_name_value->type == JSONstring )
            {
                
                // Initialized data
                size_t len = strlen(p_shader_name_value->string);

                // Allocate memory for the shader
                p_entity->shader_name = calloc(len+1, sizeof(char));

                // Error check
                if ( p_entity->shader_name == (void *) 0 ) goto no_mem;
                
                // Copy the name of the shader
                strncpy(p_entity->shader_name, p_shader_name_value->string, len);
            }
            // Default
            else
                goto wrong_shader_name_type;
        }
        
        // Transform
        if ( p_transform_value )
            if ( load_transform_as_json_value(&p_entity->transform, p_transform_value) == 0 )
                goto failed_to_load_transform_as_json_value;

        // Rigidbody
        if ( p_rigidbody_value )
            if ( load_transform_as_json_value(&p_entity->transform, p_transform_value) == 0 )
                goto failed_to_load_transform_as_json_value;

        // Collider
        // TODO
        
        // AI
        if ( p_ai_value )
            if ( load_ai_as_json_value(&p_entity->ai, p_ai_value) == 0 )
                goto failed_to_load_ai_as_json_value;

        // Return a pointer to the caller
        *pp_entity = p_entity;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] Not enough properties to construct entity in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/entity.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to allocate entity in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_part:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to load part in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }

        // JSON errors
        {
            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/entity.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/entity.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_parts_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Property \"parts\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/entity.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_shader_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Property \"shader name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/entity.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_transform_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Property \"transform\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/entity.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_transform_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to load transform in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/transform.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ai_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to load AI in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/ai.json \n", __FUNCTION__);
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

int calculate_entity_force ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    vec3* forces = p_entity->rigidbody->forces;

    // Zero set the net force vector
    forces[0] = (vec3)
    {
        .x = 0.f, 
        .y = 0.f,
        .z = 0.f,
        .w = 0.f
    };

    // Calculate each forces effect on the entity
    forces[1] = calculate_force_gravitational ( p_entity );
    forces[2] = calculate_force_applied ( p_entity );
    forces[3] = calculate_force_normal ( p_entity );
    forces[4] = calculate_force_friction ( p_entity );
    forces[5] = calculate_force_tension ( p_entity );
    forces[6] = calculate_force_spring ( p_entity );

    // Summate each force
    for (size_t i = 0; i < 6; i++)
        add_vec3(&forces[0], forces[0], forces[i]);

    // forces[0] = net force

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}

int preupdate_entity_ai ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    GXAI_t* p_ai = p_entity->ai;

    // Get the callback function associated with the current state
    int (*preupdate_ai)(GXEntity_t * p_entity) = p_ai->pre_ai;

    // Pre update
    if ( preupdate_ai )
        preupdate_ai(p_entity);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}

int draw_entity (
	void           *vp_entity,
	GXRenderer_t   *p_renderer,
	GXRenderPass_t *p_render_pass,
	GXSubpass_t    *p_subpass,
	GXShader_t     *p_shader
)
{

    // Argument check
    #ifndef NDEBUG

    #endif

    // Initialized data
    GXEntity_t     *p_entity       = vp_entity;
    GXTransform_t  *p_transform    = p_entity->transform;
    size_t          part_count     = dict_values(p_entity->parts, 0),
                    material_count = dict_values(p_entity->materials, 0);
    GXPart_t      **pp_part        = G10_REALLOC(0, part_count     * sizeof(GXPart_t *));
    GXMaterial_t  **pp_material    = G10_REALLOC(0, material_count * sizeof(GXPart_t *));

    // TODO: Bind the part
    // TODO: Bind the transform
    // TODO: Bind the material
    // TODO: Draw the entity
    //

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            
        }
    }
}

int update_entity_ai ( GXEntity_t* p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    GXAI_t *p_ai = p_entity->ai;

    // Check the state dict for a state function pointer
    if ( dict_get(p_entity->ai->states, p_entity->ai->current_state) )
    {

        // Get the callback function associated with the current state
        void (*update_ai_function)(GXEntity_t *) = ( void (*)(GXEntity_t*) ) dict_get(p_ai->states, p_ai->current_state);

        // Update
        update_ai_function(p_entity);
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}

int get_model_matrix ( void* ret )
{

    // Argument errors
    #ifndef NDEBUG
        if ( ret == (void *) 0 ) goto no_return;
    #endif

    // Initialized data
    GXInstance_t *p_instance        = g_get_active_instance();
    mat4          model_matrix    = { 0 };

    // Compute a transform model matrix from the active entity
    transform_model_matrix(p_instance->context.scene->active_entity->transform, &model_matrix);

    // Write the camera position to the return
    *(mat4 *)ret = model_matrix;

    return sizeof(mat4);

    // Error handling
    {

        // Argument errors
        {
            no_return:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"ret\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

vec3 calculate_force_gravitational ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    vec3 ret = { 0.f, 0.f, -9.8f, 0.f };

    // -50 m / s terminal velocity
    if (p_entity->rigidbody->velocity.z < -0.55f)
        ret.z = 0.f;

    if (p_entity->rigidbody->active == false)
        ret.z = 0.f;

    // Return the force vector
    return ret;

    // Error handling
    {
        
        // Argument erros
        {
            no_entity: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (vec3) { INFINITY, INFINITY, INFINITY };
        }
    }
}

vec3 calculate_force_applied ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    vec3 ret = { 0, 0, 0, 0 };

    // TODO: Get collisions
    dict* collisions = p_entity->collider->collisions;

    // Success
    return ret;

    // Error handling
    {

        // Argument erros
        {
            no_entity: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (vec3) { INFINITY, INFINITY, INFINITY };
        }
    }
}

vec3 calculate_force_normal ( GXEntity_t *p_entity )
{
    GXInstance_t  *p_instance = g_get_active_instance();
    vec3           ret      = { 0, 0, 0, 0 };

    GXEntity_t    *entity   = p_entity;
    GXCollider_t  *a        = entity->collider,
                  *b        = 0;
    GXCollision_t *c        = 0;

    dict_values(a->collisions, (void **)&c);

    if ( c )
    {

        b = c->b->collider;

        vec3  a_min = a->bv->minimum,
              a_max = a->bv->maximum,
              b_min = b->bv->minimum,
              b_max = b->bv->maximum;

        float mA = a_min.z,
              MA = a_max.z,
              mB = b_min.z + ( ( b_max.z - b_min.z ) / 2.f ),
              MB = b_max.z + entity->rigidbody->velocity.z - 0.01f;

        if ( mA > MB  )
        {
            entity->rigidbody->forces[1].z = 0.f;
            entity->rigidbody->velocity.z = 0.f;
        }
    }

    return ret;

    // Error handling
    {

        // Argument erros
        {
            no_entity: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (vec3) { INFINITY, INFINITY, INFINITY };
        }
    }
}

vec3 calculate_force_friction ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    vec3 ret = { 0, 0, 0, 0 };

    // Success
    return ret;

    // Error handling
    {
        
        // Argument erros
        {
            no_entity: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (vec3) { INFINITY, INFINITY, INFINITY };
        }
    }
}

vec3 calculate_force_tension ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    vec3 ret = { 0, 0, 0, 0 };

    // Success
    return ret;

    // Error handling
    {

        // Argument erros
        {
            no_entity: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (vec3) { INFINITY, INFINITY, INFINITY };
        }
    }
}

vec3 calculate_force_spring ( GXEntity_t *p_entity )
{

    // Initialized data
    vec3 ret = { 0, 0, 0, 0 };

    // Success
    return ret;

    // Error handling
    {

        // Argument erros
        {
            no_entity: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (vec3) { INFINITY, INFINITY, INFINITY };
        }
    }
}

int load_entity_from_queue ( void *vp_instance )
{

    // Initialized data
    GXInstance_t *p_instance = vp_instance;
    size_t        i          = 0;
    JSONValue_t  *p_value    = 0;
    GXEntity_t   *p_entity   = 0;
    GXScene_t    *p_scene    = p_instance->context.loading_scene;

    // TODO: Fix
    while ( queue_empty(p_instance->queues.load_entity) == false )
    {

        // Lock the loading mutex while we find an entity to load
        SDL_LockMutex(p_instance->mutexes.load_entity);

        // If the queue is empty, unlock the mutex and exit
        if (queue_empty(p_instance->queues.load_entity))
        {

            // Unlock the mutex
            SDL_UnlockMutex(p_instance->mutexes.load_entity);

            // Success
            return 0;
        }

        // TODO: Fix
        // text is either a path -OR- a JSON object
        queue_dequeue(p_instance->queues.load_entity, (void **)&p_value);

        // Unlock the mutex
        SDL_UnlockMutex(p_instance->mutexes.load_entity);

        // Load the entity as a JSON value
        if ( load_entity_as_json_value(&p_entity, p_value) == 0 ) goto failed_to_load_entity_as_json;

        // Add the entity to the active scene
        append_entity(p_scene, p_entity);
    }

    // Success
    return 1;

    // Error handling
    {

        // G10 errors
        {
            failed_to_load_entity_as_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to load entity in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_light_probe_from_queue ( GXInstance_t *p_instance )
{

    // TODO:

    // Error
    return 0;
}
/*
int draw_entity ( GXEntity_t *p_entity )
{

    // Argument check
    //#ifndef NDEBUG
    if ( p_entity == (void *) 0 ) goto no_entity;
    
    if (p_entity->parts == 0)
        return 0;

    // Initialized data
    GXInstance_t* p_instance = g_get_active_instance();
    p_instance->context.scene->active_entity = p_entity;

    // Draw the thing
    size_t part_count = dict_values(p_entity->parts, 0);
    GXPart_t **parts  = calloc(part_count, sizeof(void *));

    dict_values(p_entity->parts, parts);

    // TODO: Uncomment
    //use_shader(p_entity->shader);

    for (size_t i = 0; i < part_count; i++)
    {

        // Update descriptor sets
        {
            // TODO: Uncomment when shader sets are done
            ;//set_shader_camera(p_entity);
        }

        if (p_entity->shader->graphics.push_constant_data)
        {
            // TODO: Uncomment when shader sets are done
            //update_shader_push_constant(p_entity->shader);
            //vkCmdPushConstants(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], p_entity->shader->graphics.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, (u32) p_entity->shader->graphics.push_constant_size, p_entity->shader->graphics.push_constant_data);
        }

        for (size_t i = 0; i < p_entity->shader->graphics.set_count; i++)
        {
            //vkCmdBindDescriptorSets(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, p_entity->shader->graphics.pipeline_layout, 0, 1, &p_entity->shader->graphics.sets_data[i].descriptor_sets[p_instance->vulkan.current_frame], 0, 0);
        }

        // TODO: Uncomment when shader sets are done
        //draw_part(parts[i]);
    }

    free(parts);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
*/
int destroy_entity ( GXEntity_t **pp_entity )
{

    // Argument error
    #ifndef NDEBUG
        if ( pp_entity == (void *) 0 ) goto no_entity;
    #endif

    // Initialized data
    GXEntity_t *p_entity = *pp_entity;

    // No more pointer for caller
    *pp_entity = (void *) 0;

    // Free the name
    free(p_entity->name);

    if ( p_entity->shader_name )
        ;//

    if ( p_entity->transform )
        destroy_transform(&p_entity->transform);

    if ( p_entity->rigidbody )
        ;//destroy_rigidbody(p_entity->rigidbody);

    if ( p_entity->ai )
        destroy_ai(&p_entity->ai);

    // Free the entity
    free(p_entity);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int move_entity ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
    
        // TODO: Replace with goto
        if ( p_entity->rigidbody->mass == 0.f ) return 0;
    #endif

    // Initialized data
    GXInstance_t  *p_instance = g_get_active_instance();
    float          delta_time = p_instance->time.delta_time;
    GXRigidbody_t *rigidbody  = p_entity->rigidbody;
    GXTransform_t *transform  = p_entity->transform;

    // Calculate displacement and its derivatives from force
    {
        // force = ( kg * m / s^2 )

        // Calculate acceleration
        // acceleration = ( m / s^2 )
        div_vec3_f(&rigidbody->acceleration, rigidbody->forces[0], rigidbody->mass);

        // Calculate velocity
        // velocity = ( m / s )
        add_vec3(&rigidbody->velocity, mul_vec3_f(rigidbody->acceleration, delta_time), rigidbody->velocity);

        // Calculate linear momentum
        // momentum = kg * ( m / s )
        rigidbody->momentum = mul_vec3_f(rigidbody->velocity, rigidbody->mass);

        // Calculate location
        // location = ( m )
        add_vec3(&transform->location, mul_vec3_f(rigidbody->velocity, delta_time), transform->location);
    }

    // Calculate rotation and its derivatives from torque
    {

        // torque = ( kg * m^2 / s^2 )

        // Calculate angular acceleration
        // α = ( m^2 / s^2 )

    }

    // Update the model matrix
    transform_model_matrix(transform, &transform->model_matrix);
    //resize_bv(p_entity->collider->bv);

    return 1;
}

int entity_info ( GXEntity_t *p_entity )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_entity == (void *) 0 ) goto no_entity;
    #endif

    // Formatting
    g_print_log(" - Entity info - \n");

    // Print the name
    g_print_log("name: \"%s\"\n", p_entity->name);

    // Print each part
    if ( p_entity->parts )
    {
        // Formatting
        g_print_log(" - Parts - \n");

        // Print information about the part
        dict_foreach(p_entity->parts, ( void (*)(void*) ) part_info);
    }

    // Print the AI
    if ( p_entity->ai )
    {
        // Formatting
        g_print_log(" - AI - \n");

        // Print information about the AI
        ai_info(p_entity->ai);
    }

    // Formatting
    putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}