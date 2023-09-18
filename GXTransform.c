#include <G10/GXTransform.h>

int create_transform ( GXTransform_t **pp_transform )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_transform == (void *) 0 ) goto no_transform;
    #endif

    // Initialized data
    GXTransform_t *p_transform = calloc(1, sizeof(GXTransform_t));

    // Memory check
    if ( p_transform == (void *) 0 ) goto no_mem;

    // Return the allocated memory
    *pp_transform = p_transform;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n",__FUNCTION__);
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

int construct_transform ( GXTransform_t **pp_transform, vec3 location, quaternion rotation, vec3 scale )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_transform == (void *) 0 ) goto no_transform;
    #endif

    // Initialized data
    GXTransform_t *p_transform = 0;

    // Allocate the transform
    if ( create_transform(&p_transform) == 0 ) goto failed_to_create_transform;

    // Return the transform
    *p_transform = (GXTransform_t)
    {
        .location = location,
        .rotation = rotation,
        .scale    = scale
    };

    // Compute a model matrix
    transform_model_matrix(p_transform, &p_transform->model_matrix);

    // Return the allocated memory
    *pp_transform = p_transform;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_create_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to create transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_transform ( GXTransform_t **pp_transform, const char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_transform == (void *) 0 ) goto no_transform;
        if ( path         == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t         file_size   = g_load_file(path, 0, true);
    char          *file_buffer = calloc(file_size + 1, sizeof(char));

    // Load the file
    if ( g_load_file(path, file_buffer, true)                   == 0 ) goto failed_to_load_file;

    // Load the transform as a json string
    if ( load_transform_as_json_text(pp_transform, file_buffer) == 0 ) goto failed_to_load_transform;

    // Free up the memory
    free(file_buffer);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to open file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to load transform from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_transform_as_json_text ( GXTransform_t **pp_transform, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_transform == (void *) 0 ) goto no_transform;
        if ( text         == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    json_value *p_value = 0;

    // Parse the transform JSON
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Deallocate the JSON value
    FREE_VALUE(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"text\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {

            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to parse JSON in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/transform.json \n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_transform_as_json_value ( GXTransform_t **pp_transform, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_transform == (void *) 0 ) goto no_transform;
        if ( p_value      == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    json_value *p_location = 0,
                  *p_rotation = 0,
                *p_scale    = 0;

    // Parse the transform JSON
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_location   = dict_get(p_dict, "location");
        p_scale      = dict_get(p_dict, "scale");
        p_rotation   = dict_get(p_dict, "rotation");

        // Quaternion?
        if ( p_rotation == 0 )
            p_rotation = dict_get(p_dict, "quaternion");

        // Error check
        if ( ! ( 
            p_location &&
            p_rotation &&
            p_scale
         ) )
            goto missing_properties;
    }
    // Default
    else
        goto wrong_type;

    // Construct the transform
    {

        // Initialized data
        vec3           location    = { 0, 0, 0 },
                       scale       = { 1, 1, 1 };
        quaternion     rotation    = { 0, 0, 0, 0 };

        // Set the location
        if ( p_location->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements   = 0;
            size_t        element_count = 0;

            // Get the location contents
            {

                // Get the quantity of elements
                array_get(p_location->list, 0, &element_count);

                // Error check
                if ( element_count != 3 ) goto location_len_error;

                // Allocate an array for the elements
                pp_elements = calloc(element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the vector
                array_get(p_location->list, (void **)pp_elements, 0 );
            }

            // Set the location
            location = (vec3)
            {
                .x = (float) pp_elements[0]->number,
                .y = (float) pp_elements[1]->number,
                .z = (float) pp_elements[2]->number
            };

            // Clean the scope
            free(pp_elements);

        }
        // Default
        else
            goto wrong_location_type;

        // Set the rotation
        if ( p_rotation->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements   = 0;
            size_t        element_count = 0;

            // Get the array contents
            {

                // Get the quantity of elements
                array_get(p_rotation->list, 0, &element_count );

                // Error check
                if ( ! ( element_count == 3 || element_count == 4 ) ) goto rotation_len_error;

                // Allocate an array for the elements
                pp_elements = calloc(element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the vector
                array_get(p_rotation->list, (void **)pp_elements, 0 );
            }

            // Parse the data as a quaternion or an euler angle
            if ( element_count == 3 )
            {

                // Parse the rotation as an euler angle
                rotation = quaternion_from_euler_angle(
                    (vec3)
                    {
                        .x = (float) pp_elements[0]->number,
                        .y = (float) pp_elements[1]->number,
                        .z = (float) pp_elements[2]->number
                    }
                );
            }
            else if ( element_count == 4 )
            {

                // Parse the rotation as a quaternion
                rotation = (quaternion)
                {
                    .u = (float) pp_elements[0]->number,
                    .i = (float) pp_elements[1]->number,
                    .j = (float) pp_elements[2]->number,
                    .k = (float) pp_elements[3]->number
                };
            }

            // Clean the scope
            free(pp_elements);
        }
        // Default
        else
            goto wrong_rotation_type;

        // Set the scale
        if ( p_scale->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements          = 0;
            size_t        vector_element_count = 0;

            // Get the array contents
            {

                // Get the quantity of elements
                array_get(p_scale->list, 0, &vector_element_count );

                // Error check
                if ( vector_element_count != 3 ) goto scale_len_error;

                // Allocate an array for the elements
                pp_elements = calloc(vector_element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the vector
                array_get(p_scale->list, (void **)pp_elements, 0 );
            }

            // Set the scale
            scale = (vec3)
            {
                .x = (float) pp_elements[0]->number,
                .y = (float) pp_elements[1]->number,
                .z = (float) pp_elements[2]->number
            };

            // Clean the scope
            free(pp_elements);
        }
        // Default
        else
            goto wrong_scale_type;

        // Construct the transform
        if ( construct_transform(pp_transform, location, rotation, scale) == 0 ) goto failed_to_create_transform;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Not enough properties to construct transform in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/transform.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_location_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Property \"location\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/transform.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_rotation_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Property \"rotation\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/transform.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_scale_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Property \"scale\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/transform.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Expected a JSON object in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            location_len_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to parse \"location\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            rotation_len_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to parse rotation property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            scale_len_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to parse \"scale\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
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
            failed_to_create_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Failed to create transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

void transform_model_matrix ( GXTransform_t *p_transform, mat4 *r )
{

    // Argument check
    #ifndef NDEBUG
        if ( r           == (void *) 0 ) goto no_result;
        if ( p_transform == (void *) 0 ) goto no_transform;
    #endif

    mat4 sca = scale_mat4(p_transform->scale),
         rot = rotation_mat4_from_quaternion(p_transform->rotation),
         tra = translation_mat4(p_transform->location),
         M   = mul_mat4_mat4(sca, mul_mat4_mat4(rot, tra));

    // Set up the 4x4 matrix
    r->a = M.a, r->b = M.b, r->c = M.c, r->d = M.d,
    r->e = M.e, r->f = M.f, r->g = M.g, r->h = M.h,
    r->i = M.i, r->j = M.j, r->k = M.k, r->l = M.l,
    r->m = M.m, r->n = M.n, r->o = M.o, r->p = M.p;

    // Done
    return;

    // Error handling
    {

        // Argument errors
        {
            no_result:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for parameter \"r\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return;

            no_transform:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return;
        }
    }
}

int rotate_about_quaternion ( GXTransform_t *p_transform, quaternion axis, float theta )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_transform == (void *) 0 ) goto no_transform;
    #endif

    /*
    * To rotate around a quaternion, multiply the transform
    * quaternion a special quaternion called 'p', derived by multiplying
    * u by sin( theta/2 ) and multiplying i, j, and k by cos( theta/2 ).
    * Then, multiply the inverse of p, henceforth " p' ". This is analogous
    * to complex multiplication.
    */

    // Initialized
    float      halfAngle = theta / 2,
               cosHalf   = cosf(halfAngle),
               sinHalf   = sinf(halfAngle);
    vec3       newIJK    = { p_transform->rotation.i, p_transform->rotation.j, p_transform->rotation.k };

    rotate_vec3_by_quaternion(&newIJK, newIJK, axis);
    
    p_transform->rotation = (quaternion)
    { 
        .u = cosHalf,
        .i = sinHalf * newIJK.x,
        .j = sinHalf * newIJK.y,
        .k = sinHalf * newIJK.z
    };

    // Success
    return 1;

    // Error handling
    {
        no_transform:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for parameter \"transform\" in call to function \"%s\"\n",__FUNCTION__);
            #endif
        
            // Error
            return 0;
    }

}

int destroy_transform ( GXTransform_t **pp_transform )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_transform == (void *) 0 ) goto no_transform;
    #endif

    // Initialized data
    GXTransform_t *p_transform = *pp_transform;

    // No more pointer for caller
    *pp_transform = 0;

    // Free the transform
    free(p_transform);

    // Success
    return 1;

    //  Error handling
    {

        // Argument error
        {
            no_transform:
                #ifndef NDEBUG
                    g_print_error("[G10] [Transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
