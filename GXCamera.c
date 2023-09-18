#include <G10/GXCamera.h>

mat4 perspective_matrix ( float fov, float aspect, float near_clip, float far_clip )
{
    /*
     * Compute perspective projection, where f = fov, a = aspect, n = near, and r = far
     * ╭                                                      ╮
     * │ (a*tan(f/2))^-1, 0,             0,                0, │
     * │ 0,               (tan(f/2))^-1, 0,                0  │
     * │ 0,               0,             -((r+n)/(r-n)),  -1  │
     * │ 0,               0,             -((2*r*n)/(r-n)), 0  │
     * ╰                                                      ╯
     */

    return (mat4)
    {
        (1 / (aspect * tanf(fov / 2))), 0                , 0                                                  ,  0,
        0                             , 1 / tanf(fov / 2), 0                                                  ,  0,
        0                             , 0                , (-(far_clip + near_clip) / (far_clip - near_clip)) , -1,
        0                             , 0                , (2 * far_clip * near_clip / (near_clip - far_clip)),  0
    };
}

int get_camera_position ( void *ret )
{

    // Argument check
    #ifndef NDEBUG
        if ( ret == (void *) 0 ) goto no_return;
    #endif

    // Initialized data
    GXInstance_t *p_instance        = g_get_active_instance();
    vec3          camera_position = p_instance->context.scene->active_camera->view.location;

    // Write the camera position to the return
    *(vec3 *)ret = camera_position;

    return sizeof(vec3);

    // Error handling
    {

        // Argument errors
        {
            no_return:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"ret\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

mat4 look_at ( vec3 eye, vec3 target, vec3 up )
{
    // Compute forward direction
    vec3 f = normalize(
        (vec3)
        {
            .x = eye.x - target.x,
            .y = eye.y - target.y,
            .z = eye.z - target.z
        }
    );

    // Compute left direction as cross product of up and forward
    vec3 l = normalize(cross_product_vec3(up, f));

    // Recompute up
    vec3 u = cross_product_vec3(f, l);

    // Return the view matrix
    return (mat4)
    {
        l.x, u.x, f.x, 0,
        l.y, u.y, f.y, 0,
        l.z, u.z, f.z, 0,
        -dot_product_vec3(l,eye), -dot_product_vec3(u, eye), -dot_product_vec3(f, eye), 1
    };
};

int create_camera ( GXCamera_t **pp_camera )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_camera == (void *) 0 ) goto no_camera;
    #endif

    // Initialized data
    GXCamera_t *p_camera = calloc(1, sizeof(GXCamera_t));

    // Error check
    #ifndef NDEBUG
        if ( p_camera == (void *) 0 ) goto no_mem;
    #endif

    // Write the return value
    *pp_camera = p_camera;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"", __FUNCTION__);
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

int load_camera ( GXCamera_t **pp_camera, const char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_camera == (void *) 0 ) goto no_camera;
        if ( path      == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  file_len  = g_load_file(path, 0, true);
    char   *file_text = calloc(file_len + 1, sizeof(char));

    // Load the file text
    if ( g_load_file(path, file_text, true)             == 0 ) goto failed_to_load_file;

    // Parse the file text into a camera
    if ( load_camera_as_json_text(pp_camera, file_text) == 0 ) goto failed_to_load_camera_as_json;

    // Clean up the scope
    free(file_text);

    // Success
    return 1;

    // Error handling
    {

        // Arugment check
        {

            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_camera_as_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Failed to construct camera from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_camera_as_json_text ( GXCamera_t **pp_camera, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_camera == (void *) 0 ) goto no_camera;
        if ( text      == (void *) 0 ) goto no_object_text;
    #endif

    // Initialized data
    char        *name         = 0;
    array        *p_location   = 0,
                *p_target     = 0,
                *p_up         = 0;
    float        near_clip    = 0,
                 far_clip     = 0,
                 aspect_ratio = 0,
                 fov          = 0;
    json_value *p_value      = 0;

    // Parse the text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Parse the JSON value into a camera
    if ( load_camera_as_json_value(pp_camera, p_value) == 0 ) goto failed_to_load_camera_as_json_value;

    // Success
    return 1;

    // Error handling
    {

        // Arugment check
        {

            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_object_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON parsing errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Failed to parse JSON in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_camera_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Failed to construct camera in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_camera_as_json_value ( GXCamera_t **pp_camera, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_camera == (void *) 0 ) goto no_camera;
        if ( p_value   == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    json_value *p_name         = 0,
                *p_near_clip    = 0,
                *p_far_clip     = 0,
                *p_fov          = 0,
                *p_location     = 0,
                *p_target       = 0,
                *p_up           = 0;

    // Parse the camera as a JSON object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name      = dict_get(p_dict, "name");
        p_near_clip = dict_get(p_dict, "near");
        p_far_clip  = dict_get(p_dict, "far");
        p_fov       = dict_get(p_dict, "fov");
        p_location  = dict_get(p_dict, "location");
        p_target    = dict_get(p_dict, "front");
        p_up        = dict_get(p_dict, "up");

        // Error check
        if ( ! ( 
            p_name      &&
            p_near_clip &&
            p_far_clip  &&
            p_fov       &&
            p_location  &&
            p_target    &&
            p_up
        ) )
            goto missing_properties;
    }
    // Parse the camera as a path
    else if ( p_value->type == JSON_VALUE_STRING )
    {

        // Load the camera from the file system
        if ( load_camera(pp_camera, p_value->string) == 0 ) goto failed_to_load_camera;

        // Success
        return 1;
    }
    // Default
    else
        goto wrong_type;

    // Construct the camera
    {

        // Initialized data
        GXInstance_t *p_instance   = g_get_active_instance();
        GXCamera_t   *p_camera     = 0;
        char         *name         = 0;
        vec3          location     = { 0, 0, 0 },
                      target       = { 0, 0, 0 },
                      up           = { 0, 0, 0, 0 };
        float         near_clip    = 0,
                      far_clip     = 0,
                      aspect_ratio = 0,
                      fov          = 0;

        // Allocate memory for a camera
        if ( create_camera(&p_camera) == 0 ) goto failed_to_allocate_camera;

        // Copy the camera name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( name == (void *) 0 ) goto no_mem;

            // Copy the name
            strncpy(name, p_name->string, name_len);
        }
        // Default
        else
            goto wrong_name_type;

        // Set the location
        if ( p_location->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements          = 0;
            size_t        vector_element_count = 0;

            // Get the contents of the array
            {

                // Get the quantity of elements
                array_get(p_location->list, 0, &vector_element_count );

                // Error check
                if ( vector_element_count != 3 ) goto location_len_error;

                // Allocate an array for the elements
                pp_elements = calloc(vector_element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the vector
                array_get(p_location->list, (void **)pp_elements, 0 );
            }

            // Set the location
            location = (vec3)
            {
                .x = (float) ((json_value *)pp_elements[0])->number,
                .y = (float) ((json_value *)pp_elements[1])->number,
                .z = (float) ((json_value *)pp_elements[2])->number
            };

            // Clean the scope
            free(pp_elements);
        }
        // Default
        else
            goto wrong_location_type;

        // Set the target
        if ( p_target->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements          = 0;
            size_t        vector_element_count = 0;

            // Get the contents of the array
            {

                // Get the quantity of elements
                array_get(p_target->list, 0, &vector_element_count );

                // Error check
                if ( vector_element_count != 3 ) goto target_len_error;

                // Allocate an array for the elements
                pp_elements = calloc(vector_element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the vector
                array_get(p_target->list, (void **)pp_elements, 0 );
            }

            // Set the target
            target = (vec3)
            {
                .x = (float) ((json_value *)pp_elements[0])->number,
                .y = (float) ((json_value *)pp_elements[1])->number,
                .z = (float) ((json_value *)pp_elements[2])->number
            };

            // Clean the scope
            free(pp_elements);
        }
        // Default
        else
            goto wrong_target_type;

        // Set the up vector
        if ( p_up->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements          = 0;
            size_t        vector_element_count = 0;

            // Get the contents of the array
            {

                // Get the quantity of elements
                array_get(p_up->list, 0, &vector_element_count );

                // Error check
                if ( vector_element_count != 3 ) goto up_len_error;

                // Allocate an array for the elements
                pp_elements = calloc(vector_element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the vector
                array_get(p_up->list, (void **)pp_elements, 0);
            }

            // Set the up vector
            up = (vec3)
            {
                .x = (float) ((json_value *)pp_elements[0])->number,
                .y = (float) ((json_value *)pp_elements[1])->number,
                .z = (float) ((json_value *)pp_elements[2])->number
            };

            // Clean the scope
            free(pp_elements);
        }
        // Default
        else
            goto wrong_up_type;

        // Set the near clip
        if ( p_near_clip->type == JSON_VALUE_NUMBER )
            near_clip = (float) p_near_clip->number;
        // Default
        else
            goto wrong_near_clip_type;
        
        // Set the far clip
        if ( p_far_clip->type == JSON_VALUE_NUMBER )
            far_clip = (float) p_far_clip->number;
        // Default
        else
            goto wrong_far_clip_type;

        // Set the aspect ratio
        aspect_ratio = (float) p_instance->window.width / (float) p_instance->window.height;

        // Set the fov
        if ( p_fov->type == JSON_VALUE_NUMBER )
            fov = (float) p_fov->number;
        // Default
        else
            goto wrong_fov_type;

        // Construct the camera
        *p_camera = (GXCamera_t)
        {
            .name                    = name,
            .view.location           = location,
            .view.target             = target,
            .view.up                 = up,
            .projection.near_clip    = near_clip,
            .projection.far_clip     = far_clip,
            .projection.aspect_ratio = aspect_ratio,
            .projection.fov          = fov,
            .view_matrix             = look_at(location, target, up),
            .projection_matrix       = perspective_matrix(fov, aspect_ratio, near_clip, far_clip)
        };

        // Return the camera to the caller
        *pp_camera = p_camera;
    }

    // Success
    return 1;

    // Error handling
    {

        // Arugment check
        {

            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {

            wrong_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Not enough properties to construct camera in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            location_len_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"location\" must have length of 3 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            target_len_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"target\" must have length of 3 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            up_len_error:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"up\" must have length of 3 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"name\" must be of type [ string ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_location_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"location\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_target_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"target\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_up_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"up\" must be of type [ array ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_near_clip_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"near clip\" must be of type [ float ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_far_clip_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"far clip\" must be of type [ float ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_fov_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Property \"fov\" must be of type [ float ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/camera.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 Errors
        {
            failed_to_allocate_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Failed to allocate memory for camera in call to function \"%s\"\n",  __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Failed to load camera from path %s in call to function \"%s\"\n",p_value->string, __FUNCTION__);
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

int print_camera ( GXCamera_t *p_camera, size_t idx )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_camera == (void *) 0 ) goto no_camera;
    #endif

    // Initialized data
    mat4 v = p_camera->view_matrix,
         p = p_camera->projection_matrix;

    // Formatting
    g_print_log(" - Camera info - \n");

    // Print the name
    g_print_log("name      : \"%s\"\n", p_camera->name);

    // Print the location of the camera
    g_print_log("location  : < %.3f, %.3f, %.3f >\n", p_camera->view.location.x, p_camera->view.location.y, p_camera->view.location.z );

    // Print the up vector
    g_print_log("up        : < %.3f, %.3f, %.3f >\n", p_camera->view.up.x, p_camera->view.up.y, p_camera->view.up.z );

    // Print the target vector
    g_print_log("front     : < %.3f, %.3f, %.3f >\n", p_camera->view.target.x, p_camera->view.target.y, p_camera->view.target.z );

    // Print the near clip distance
    g_print_log("near      : %.3f\n", p_camera->projection.near_clip );

    // Print the far clip distance
    g_print_log("far       : %.1f\n", p_camera->projection.far_clip );

    // Print the aspect ratio
    g_print_log("aspect    : %.2f\n", p_camera->projection.aspect_ratio );

    // Print the FOV
    g_print_log("fov       : %.1f\n", p_camera->projection.fov );

    // Print the view matrix
    g_print_log(
        "view      : < < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
        "              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
        "              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
        "              < %2.2f, %2.2f, %2.2f, %2.2f > >\n",
        v.a, v.b, v.c, v.d,
        v.e, v.f, v.g, v.h,
        v.i, v.j, v.k, v.l,
        v.m, v.n, v.o, v.p );

    // Print the view matrix
    g_print_log(
        "projection: < < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
        "              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
        "              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
        "              < %2.2f, %2.2f, %2.2f, %2.2f > >\n",
        p.a, p.b, p.c, p.d,
        p.e, p.f, p.g, p.h,
        p.i, p.j, p.k, p.l,
        p.m, p.n, p.o, p.p );

    putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"p_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_camera ( GXCamera_t **pp_camera )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_camera == (void *) 0 ) goto no_camera;
    #endif

    // Initialized data
    GXCamera_t *p_camera = *pp_camera;

    // No more pointer for you
    *pp_camera = 0;

    // Free the name
    free(p_camera->name);

    // Free the camera
    free(p_camera);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}