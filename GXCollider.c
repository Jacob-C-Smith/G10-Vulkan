#include <G10/GXCollider.h>

#define COLLIDER_TYPE_COUNT 8

char* collider_type_names[COLLIDER_TYPE_COUNT] = {
    "invalid",
    "quad",
    "box",
    "sphere",
    "capsule",
    "cylinder",
    "cone",
    "convexhull"
};
collider_type_t collider_type_enums[COLLIDER_TYPE_COUNT] = {
    collider_invalid,
    collider_quad,
    collider_box,
    collider_sphere,
    collider_capsule,
    collider_cylinder,
    collider_cone,
    collider_convexhull
};

dict* collider_type_dict = 0;

void init_collider ( void )
{
    dict_construct(&collider_type_dict, COLLIDER_TYPE_COUNT);

    for (size_t i = 0; i < COLLIDER_TYPE_COUNT; i++)
        dict_add(collider_type_dict, collider_type_names[i], (void*)collider_type_enums[i]);
}

int create_collider ( GXCollider_t **pp_collider )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_collider == (void *) 0 ) goto no_collider;
    #endif

    // Initialized data
    GXCollider_t * p_collider = calloc(1, sizeof(GXCollider_t));

    // Error check
    if ( pp_collider == (void *) 0 ) goto no_mem;

    // Write the return value
    *pp_collider = p_collider;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_collider:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collider] Null pointer provided for parameter \"pp_collider\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_collider ( GXCollider_t **pp_collider, const char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_collider == (void *) 0 ) goto no_collider;
        if ( path        == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len+1, sizeof(char));

    // Load the file contents into a text buffer
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_load_file;

    // Load the collider as JSON text
    if ( load_collider_as_json_text(pp_collider, text) == 0 ) goto failed_to_construct_collider;

    // Clean the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_collider:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collider] Null pointer provided for parameter \"pp_collider\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collider] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collider] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_collider:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collider] Failed to construct collider in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_collider_as_json_text ( GXCollider_t **collider, char *text )
{
    GXCollider_t  *i_collider    = 0;
    json_value   *collider_json = 0;
    char          *type          = 0,
                 **max           = 0,
                 **min           = 0;

    parse_json_value(text, 0, &collider_json);

    /*{
        JSONToken_t *token = 0;

        token = (JSONToken_t *) dict_get(collider_json, "type");
        type  = JSON_VALUE(token, JSON_VALUE_STRING);

        token = (JSONToken_t *) dict_get(collider_json, "max");
        max   = JSON_VALUE(token, JSON_VALUE_ARRAY);

        token = (JSONToken_t *) dict_get(collider_json, "min");
        min   = JSON_VALUE(token, JSON_VALUE_ARRAY);

    }

    create_collider(collider);

    i_collider = *collider;

    i_collider->aabb_max = (vec3){ (float) atof(max[0]), (float) atof(max[1]), (float) atof(max[2]) };
    i_collider->aabb_min = (vec3){ (float) atof(min[0]), (float) atof(min[1]), (float) atof(min[2]) };

    i_collider->type = (collider_type_t)dict_get(collider_type_dict, type);


    construct_bv(&i_collider->bv, i_collider->aabb_min, i_collider->aabb_max);

    // TODO: Change 16 to something smarter
    dict_construct(&i_collider->collisions, 16);*/

    return 1;
}

//int add_aabb_start_collision_callback ( GXCollider_t* collider, void* function_pointer )
//{
//    return 1;
//}
//
