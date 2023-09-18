#include <G10/GXInput.h>

// Abstract keys into keyboard file.
const kn_t keys[] =
{
    {
        SDL_SCANCODE_UNKNOWN,
        ""
    },
    {
        1,
        ""
    },
    {
        2,
        ""
    },
    {
        3,
        ""
    },
    {
        SDL_SCANCODE_A,
        "A"
    },
    {
        SDL_SCANCODE_B,
        "B"
    },
    {
        SDL_SCANCODE_C,
        "C"
    },
    {
        SDL_SCANCODE_D,
        "D"
    },
    {
        SDL_SCANCODE_E,
        "E"
    },
    {
        SDL_SCANCODE_F,
        "F"
    },
    {
        SDL_SCANCODE_G,
        "G"
    },
    {
        SDL_SCANCODE_H,
        "H"
    },
    {
        SDL_SCANCODE_I,
        "I"
    },
    {
        SDL_SCANCODE_J,
        "J"
    },
    {
        SDL_SCANCODE_K,
        "K"
    },
    {
        SDL_SCANCODE_L,
        "L"
    },
    {
        SDL_SCANCODE_M,
        "M"
    },
    {
        SDL_SCANCODE_N,
        "N"
    },
    {
        SDL_SCANCODE_O,
        "O"
    },
    {
        SDL_SCANCODE_P,
        "P"
    },
    {
        SDL_SCANCODE_Q,
        "Q"
    },
    {
        SDL_SCANCODE_R,
        "R"
    },
    {
        SDL_SCANCODE_S,
        "S"
    },
    {
        SDL_SCANCODE_T,
        "T"
    },
    {
        SDL_SCANCODE_U,
        "U"
    },
    {
        SDL_SCANCODE_V,
        "V"
    },
    {
        SDL_SCANCODE_W,
        "W"
    },
    {
        SDL_SCANCODE_X,
        "X"
    },
    {
        SDL_SCANCODE_Y,
        "Y"
    },
    {
        SDL_SCANCODE_Z,
        "Z"
    },
    {
        SDL_SCANCODE_1,
        "1"
    },
    {
        SDL_SCANCODE_2,
        "2"
    },
    {
        SDL_SCANCODE_3,
        "3"
    },
    {
        SDL_SCANCODE_4,
        "4"
    },
    {
        SDL_SCANCODE_5,
        "5"
    },
    {
        SDL_SCANCODE_6,
        "6"
    },
    {
        SDL_SCANCODE_7,
        "7"
    },
    {
        SDL_SCANCODE_8,
        "8"
    },
    {
        SDL_SCANCODE_9,
        "9"
    },
    {
        SDL_SCANCODE_0,
        "0"
    },
    {
        SDL_SCANCODE_RETURN,
        "RETURN"
    },
    {
        SDL_SCANCODE_ESCAPE,
        "ESCAPE"
    },
    {
        SDL_SCANCODE_BACKSPACE,
        "BACKSPACE"
    },
    {
        SDL_SCANCODE_TAB,
        "TAB"
    },
    {
        SDL_SCANCODE_SPACE,
        "SPACE"
    },
    {
        SDL_SCANCODE_MINUS,
        "MINUS"
    },
    {
        SDL_SCANCODE_EQUALS,
        "EQUALS"
    },
    {
        SDL_SCANCODE_LEFTBRACKET,
        "LEFT BRACKET"
    },
    {
        SDL_SCANCODE_RIGHTBRACKET,
        "RIGHT BRACKET"
    },
    {
        SDL_SCANCODE_BACKSLASH,
        "BACKSLASH"
    },
    {
        50,
        ""
    },
    {
        SDL_SCANCODE_SEMICOLON,
        "SEMICOLON"
    },
    {
        SDL_SCANCODE_APOSTROPHE,
        "APOSTROPHE"
    },
    {
        SDL_SCANCODE_GRAVE,
        "TILDE"
    },
    {
        SDL_SCANCODE_COMMA,
        "COMMA"
    },
    {
        SDL_SCANCODE_PERIOD,
        "PERIOD"
    },
    {
        SDL_SCANCODE_SLASH,
        "SLASH"
    },
    {
        SDL_SCANCODE_CAPSLOCK,
        "CAPS LOCK"
    },
    {
        SDL_SCANCODE_F1,
        "F1"
    },
    {
        SDL_SCANCODE_F2,
        "F2"
    },
    {
        SDL_SCANCODE_F3,
        "F3"
    },
    {
        SDL_SCANCODE_F4,
        "F4"
    },
    {
        SDL_SCANCODE_F5,
        "F5"
    },
    {
        SDL_SCANCODE_F6,
        "F6"
    },
    {
        SDL_SCANCODE_F7,
        "F7"
    },
    {
        SDL_SCANCODE_F8,
        "F8"
    },
    {
        SDL_SCANCODE_F9,
        "F9"
    },
    {
        SDL_SCANCODE_F10,
        "F10"
    },
    {
        SDL_SCANCODE_F11,
        "F11"
    },
    {
        SDL_SCANCODE_F12,
        "F12"
    },
    {
        SDL_SCANCODE_PRINTSCREEN,
        "PRINT SCREEN"
    },
    {
        SDL_SCANCODE_SCROLLLOCK,
        "SCROLL LOCK"
    },
    {
        SDL_SCANCODE_PAUSE,
        "PAUSE"
    },
    {
        SDL_SCANCODE_INSERT,
        "INSERT"
    },
    {
        SDL_SCANCODE_HOME,
        "HOME"
    },
    {
        SDL_SCANCODE_PAGEUP,
        "PAGE UP"
    },
    {
        SDL_SCANCODE_DELETE,
        "DELETE"
    },
    {
        SDL_SCANCODE_END,
        "END"
    },
    {
        SDL_SCANCODE_PAGEDOWN,
        "PAGE DOWN"
    },
    {
        SDL_SCANCODE_RIGHT,
        "RIGHT"
    },
    {
        SDL_SCANCODE_LEFT,
        "LEFT"
    },
    {
        SDL_SCANCODE_DOWN,
        "DOWN"
    },
    {
        SDL_SCANCODE_UP,
        "UP"
    },
    {
        SDL_SCANCODE_NUMLOCKCLEAR,
        "NUMLOCK"
    },
    {
        SDL_SCANCODE_KP_DIVIDE,
        "KEYPAD DIVIDE"
    },
    {
        SDL_SCANCODE_KP_MULTIPLY,
        "KEYPAD MULTIPLY"
    },
    {
        SDL_SCANCODE_KP_MINUS,
        "KEYPAD MINUS"
    },
    {
        SDL_SCANCODE_KP_PLUS,
        "KEYPAD PLUS"
    },
    {
        SDL_SCANCODE_KP_ENTER,
        "KEYPAD ENTER"
    },
    {
        SDL_SCANCODE_KP_1,
        "KEYPAD 1"
    },
    {
        SDL_SCANCODE_KP_2,
        "KEYPAD 2"
    },
    {
        SDL_SCANCODE_KP_3,
        "KEYPAD 3"
    },
    {
        SDL_SCANCODE_KP_4,
        "KEYPAD 4"
    },
    {
        SDL_SCANCODE_KP_5,
        "KEYPAD 5"
    },
    {
        SDL_SCANCODE_KP_6,
        "KEYPAD 6"
    },
    {
        SDL_SCANCODE_KP_7,
        "KEYPAD 7"
    },
    {
        SDL_SCANCODE_KP_8,
        "KEYPAD 8"
    },
    {
        SDL_SCANCODE_KP_9,
        "KEYPAD 9"
    },
    {
        SDL_SCANCODE_KP_0,
        "KEYPAD 0"
    },
    {
        SDL_SCANCODE_KP_PERIOD,
        "KEYPAD PERIOD"
    },
    {
        100,
        ""
    },
    {
        SDL_SCANCODE_LCTRL,
        "LEFT CONTROL"
    },
    {
        SDL_SCANCODE_LSHIFT,
        "LEFT SHIFT"
    },
    {
        SDL_SCANCODE_LALT,
        "LEFT ALT"
    },
    {
        SDL_SCANCODE_RCTRL,
        "RIGHT CONTROL"
    },
    {
        SDL_SCANCODE_RSHIFT,
        "RIGHT SHIFT"
    },
    {
        SDL_SCANCODE_RALT,
        "RIGHT ALT"
    },
    {
        254,
        "MOUSE UP"
    },
    {
        0,
        0
    }
};

const char *scancodes[] =
{
    " ",
    " ",
    " ",
    " ",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "RETURN",
    "ESCAPE",
    "BACKSPACE",
    "TAB",
    "SPACE",
    "MINUS",
    "EQUALS",
    "LEFT BRACKET",
    "RIGHT BRACKET",
    "BACKSLASH",
    " ",
    "SEMICOLON",
    "APOSTROPHE",
    "TILDE",
    "COMMA",
    "PERIOD",
    "SLASH",
    "CAPS LOCK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "PRINT SCREEN",
    "SCROLL LOCK",
    "PAUSE",
    "INSERT",
    "HOME",
    "PAGE UP",
    "DELETE",
    "END",
    "PAGE DOWN",
    "RIGHT",
    "LEFT",
    "DOWN",
    "UP",
    "NUMLOCK",
    "KEYPAD DIVIDE",
    "KEYPAD MULTIPLY",
    "KEYPAD MINUS",
    "KEYPAD PLUS",
    "KEYPAD ENTER",
    "KEYPAD 1",
    "KEYPAD 2",
    "KEYPAD 3",
    "KEYPAD 4",
    "KEYPAD 5",
    "KEYPAD 6",
    "KEYPAD 7",
    "KEYPAD 8",
    "KEYPAD 9",
    "KEYPAD 0",
    "KEYPAD PERIOD",
    0
};

dict *key_dict = 0;

SDL_GameController *controller;

void init_input ( void )
{

    // Make a hash table of keys
    dict_construct(&key_dict, 512);

    // Construct the hash table
    for (size_t i = 0; keys[i].code; i++)
        dict_add(key_dict, (char *)keys[i].name, 0);

    // Is there a controller?
    if ( SDL_IsGameController(0) )
        controller = SDL_GameControllerOpen(0);

}

int create_input ( GXInput_t **pp_input )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_input == (void *) 0 ) goto no_input;
    #endif

    // Initialized data
    GXInput_t *p_input = calloc(1, sizeof(GXInput_t));

    // Error check
    if ( p_input == (void *) 0 ) goto no_mem;

    // Return an input pointer to the caller
    *pp_input = p_input;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    printf("[G10] [Input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
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

int create_bind ( GXBind_t **pp_bind )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_bind == (void *) 0 ) goto no_bind;
    #endif

    // Initialized data
    GXBind_t *p_bind = calloc(1, sizeof(GXBind_t));

    // Error check
    if ( p_bind == (void *) 0 ) goto no_mem;

    // Return a bind pointer to the caller
    *pp_bind = p_bind;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_bind:
                #ifndef NDEBUG
                    printf("[G10] [Bind] Null pointer provided for parameter \"p_bind\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_input ( GXInput_t **pp_input, const char path[] )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_input == (void *) 0 ) goto no_input;
        if ( path     == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    GXInput_t *p_input = 0;
    size_t     len     = g_load_file(path, 0, true);
    char      *text    = calloc(len + 1, sizeof(char));    
    
    // Error check
    if ( text == (void *) 0 ) goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 ) goto failed_to_load_file;

    // Construct the input from JSON text
    if ( load_input_as_json_text(pp_input, text) == 0 ) goto failed_to_parse_input;

    // Cleanup the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_log("[G10] [Input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_log("[G10] [Input] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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
                    g_print_error("[G10] [Input] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_parse_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to parse file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_input_as_json_text ( GXInput_t **pp_input, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( text == (void *) 0 ) goto no_text;
    #endif

    // Initialized data
    json_value *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

    if ( load_input_as_json_value(pp_input, p_value) == 0 ) goto failed_to_parse_input_as_json_value;

    // Clean the scope
    //free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to parse JSON value in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_parse_input_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to load input in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_input_as_json_value ( GXInput_t **pp_input, json_value *p_value )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_input == (void *) 0 ) goto no_input;
        if ( p_value  == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXInput_t   *p_input             = 0;
    json_value *p_name              = 0,
                *p_mouse_sensitivity = 0,
                *p_binds             = 0;

    // Parse the JSON
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name              = dict_get(p_dict, "name");
        p_mouse_sensitivity = dict_get(p_dict, "mouse sensitivity");
        p_binds             = dict_get(p_dict, "binds");

        // Error check
        if ( ! (
            p_name              &&
            p_mouse_sensitivity &&
            p_binds
        ) )
            goto missing_properties;
    }

    // Parse the input as a path
    else if ( p_value->type == JSON_VALUE_STRING )
    {

        // Load the input as a path
        if ( load_input(pp_input, p_value->string) == 0 ) goto failed_to_load_input_from_path;

        // Success
        return 1;
    }
    // Default
    else
        goto wrong_value_type;

    // Construct the input
    {

        // Allocate memory for an input
        if ( create_input(pp_input) == 0 ) goto failed_to_load_input;

        // Get a pointer
        p_input = *pp_input;

        // Copy the name
        if ( p_name->type == JSON_VALUE_STRING)
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_input->name = calloc(name_len+1, sizeof(char));

            // Error check
            if ( p_input->name == (void *) 0 ) goto no_mem;

            // Copy the string
            strncpy(p_input->name, p_name->string, name_len);
        }
        // Default
        else
            goto wrong_name_type;

        // Set the mouse sense
        if ( p_mouse_sensitivity->type == JSON_VALUE_NUMBER )
            p_input->mouse_sensitivity = (float) p_mouse_sensitivity->number;
        // Default
        else
            goto wrong_mouse_sensitivity_type;

        // Parse the binds
        if ( p_binds->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements          = 0;
            size_t        vector_element_count = 0;

            // Get the contents of the array
            {

                // Get the quantity of elements
                array_get(p_binds->list, 0, &vector_element_count );

                // Allocate an array for the elements
                pp_elements = calloc(vector_element_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the array
                array_get(p_binds->list, (void **)pp_elements, 0 );
            }

            // Construct a hash table to quickly look up binds
            // from their scancodes
            dict_from_keys(&p_input->bind_lut, scancodes, 128);

            // TODO: Check return
            // Allocate dictionaries
            dict_construct(&p_input->binds, vector_element_count);

            // Iterate over each element
            for (size_t i = 0; i < vector_element_count; i++)
            {

                // Initialized data
                json_value *i_element = pp_elements[i];
                GXBind_t    *p_bind    = 0;

                // Load the bind from the JSON value
                if ( load_bind_as_json_value(&p_bind, i_element) == 0 ) goto failed_to_load_bind_as_json_value;

                // Add the bind to the input                
                append_bind(p_input, p_bind);
            }

            // Clean the scope
            free(pp_elements);
        }
        // Default
        else
            goto wrong_binds_type;
    }

    // Success
    return 1;

    // TODO:
    missing_properties:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // TODO: G10 errors
        {
            failed_to_load_input_from_path:
            failed_to_load_input:
            failed_to_load_bind_as_json_value:
                return 0;
        }

        // JSON errors
        {

            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Property \"p_value\" must be of type [ object ] in call to function \"%s\"\n\"Refer to gschema: https://schema.g10.app/input.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Property \"name\" must be of type [ string ] in call to function \"%s\"\n\"Refer to gschema: https://schema.g10.app/input.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_mouse_sensitivity_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Property \"mouse sensitivity\" must be of type [ float ] in call to function \"%s\"\n\"Refer to gschema: https://schema.g10.app/input.json \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_binds_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Property \"binds\" must be of type [ array ] in call to function \"%s\"\n\"Refer to gschema: https://schema.g10.app/input.json \n", __FUNCTION__);
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

int load_bind_as_json_value ( GXBind_t **pp_bind, json_value *p_value )
{
    
    // Argument check
    #ifndef NDEBUG
        if ( pp_bind == (void *) 0 ) goto no_bind;
        if ( p_value == (void *) 0 ) goto no_value;
    #endif

    // Initialized data
    GXBind_t    *p_bind = 0;
    json_value *p_name = 0,
                *p_keys = 0;

    // Parse the bind as an object
    if ( p_value->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Required properties
        p_name = dict_get(p_dict, "name");
        p_keys = dict_get(p_dict, "keys");

        // Check for missing data
        if ( ! ( 
            p_name &&
            p_keys
        ) )
            goto missing_properties;

    }
    // Default
    else
        goto wrong_value_type;
    
    // Construct the bind
    {

        // Initialized data
        char    *name       = 0;
        char   **keys_array = 0;
        size_t   keys_count = 0;

        // Allocate memory for an bind
        if ( create_bind(pp_bind) == 0 ) goto failed_to_load_bind;

        // Get a pointer
        p_bind = *pp_bind;

        // Copy the name
        if ( p_name->type == JSON_VALUE_STRING )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            name = calloc(name_len+1, sizeof(char));

            // Error check
            if ( name == (void *) 0 ) goto no_mem;

            // Copy the string
            strncpy(name, p_name->string, name_len);
        }
        // Default
        else
            goto wrong_name_type;

        // Parse the keys
        if ( p_keys->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            json_value **pp_elements  = 0;

            // Dump the array
            {

                // Get the quantity of elements
                array_get(p_keys->list, 0, &keys_count );

                // Allocate an array for the elements
                pp_elements = calloc(keys_count+1, sizeof(json_value *));

                // Error check
                if ( pp_elements == (void *) 0 ) goto no_mem;

                // Populate the elements of the array
                array_get(p_keys->list, (void **)pp_elements, 0 );
            }

            // Allocate memory for an array of keys
            keys_array = calloc(keys_count+1, sizeof(char *));

            // Iterate over each element
            for (size_t i = 0; i < keys_count; i++)
            {

                // Initialized data
                json_value *i_element = pp_elements[i];
                size_t  key_len = 0;
                char   *key     = 0;

                // Copy the name
                if ( i_element->type == JSON_VALUE_STRING )
                {
                    // Compute the length of the string
                    key_len = strlen(i_element->string);

                    // Allocate memory for a copy of the string
                    key = calloc(key_len+1, sizeof(char));

                    // Error check
                    if ( key == (void *) 0 ) goto no_mem;

                    // Copy the string
                    strncpy(key, i_element->string, key_len);
                }
                // Default
                else
                    goto failed_to_parse_key;

                // Store the key pointer in the list of keys
                keys_array[i] = key;
            }

            // Clean the scope
            free(pp_elements);
        }

        *p_bind = (GXBind_t)
        {
            .name           = name,
            .active         = false,
            .keys           = keys_array,
            .key_count      = keys_count,
            .callback_max   = 2,
            .callback_count = 0,
            .callbacks      = calloc(2, sizeof(void *)),
            .next           = 0
        };
    }
    
    // Success
    return 1;

    // TODO:
    wrong_value_type:
    wrong_name_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"pp_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // TODO: G10 errors
        {
            missing_properties:
            failed_to_load_bind:
            failed_to_parse_key:
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

int register_bind_callback ( GXBind_t *p_bind, void *function_pointer )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_bind           == (void *) 0 ) goto no_bind;
        if ( function_pointer == (void *) 0 ) goto no_function_pointer;
    #endif

    // Allocate memory for bind callbacks
    if (p_bind->callback_max == 0)
    {
        p_bind->callback_max   = 2,
        p_bind->callbacks      = calloc(p_bind->callback_max, sizeof(void*));
    }

    if (p_bind->callback_count + 1 > p_bind->callback_max)
    {
        p_bind->callback_max *= 2;
        void **callbacks    = calloc(p_bind->callback_max, sizeof(void*)),
              *t            = p_bind->callbacks;

        memcpy(callbacks, p_bind->callbacks, p_bind->callback_count * sizeof(void*));
        p_bind->callbacks = callbacks;

        free(t);
    }

    p_bind->callbacks[p_bind->callback_count++] = function_pointer;

    // Success
    return 1;

    // Error handling
    {
        no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for parameter \"bind\" in call to function \"%s\"\n",__FUNCTION__);
            #endif

            // Error
            return 0;

        no_function_pointer:
            #ifndef NDEBUG
                g_print_warning("[G10] [Input] Null pointer provided for parameter \"function_pointer\" in call to function \"%s\"\n",__FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int unregister_bind_callback ( GXBind_t *p_bind, void *function_pointer )
{
    // TODO: Argument check
    // TODO
    return 0;
    // TODO: Error handling
}

SDL_Scancode find_key ( const char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( name == (void *) 0 ) goto no_name;
    #endif

    // Return the corresponding keycode
    return (SDL_Scancode)(size_t)dict_get(key_dict, (char *)name);

    // Error handling
    {
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] No name provided to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return (SDL_Scancode)0;
    }
}

int process_input ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance        == (void *) 0 ) goto no_instance;
        if ( p_instance->input == (void *) 0 ) goto no_inputs;
    #endif

    // Initialized data
    queue *p_update_binds_queue = 0;

    // Iterate over each SDL2 event
    while ( SDL_PollEvent(&p_instance->sdl2.event) ) 
    {

        // Switch the event
        switch (p_instance->sdl2.event.type)
        {
            
            // Process mouse motion
            case SDL_MOUSEMOTION:
            {
                bool mouse_lock = SDL_GetRelativeMouseMode();
                u8   button     = 0;
                int  x_rel      = p_instance->sdl2.event.motion.xrel,
                     y_rel      = p_instance->sdl2.event.motion.yrel;
                callback_parameter_t input = 
                {
                    .input_state               = MOUSE,
                    .inputs.mouse_state.xrel   = (s32) (x_rel * p_instance->input->mouse_sensitivity),
                    .inputs.mouse_state.yrel   = (s32) (y_rel * p_instance->input->mouse_sensitivity),
                    .inputs.mouse_state.button = 0 // TODO
                };

                //printf(" <%.2f, %.2f >                \r", (float) input.inputs.mouse_state.xrel, (float) input.inputs.mouse_state.yrel);
                //fflush(stdout);

                //// -X, mouse left
                //if ( x_rel < 0 )
                //    queue_enqueue(p_instance->input->p_key_queue, "MOUSE LEFT");

                //// +X, mouse right
                //if ( 0 < x_rel )
                //    queue_enqueue(p_instance->input->p_key_queue, "MOUSE RIGHT");

                //// -Y, mouse up
                //if ( y_rel < 0 )
                //    queue_enqueue(p_instance->input->p_key_queue, "MOUSE UP");

                //// +Y, mouse down
                //if ( 0 < y_rel )
                //    queue_enqueue(p_instance->input->p_key_queue, "MOUSE DOWN");
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {

            }
            break;
            case SDL_MOUSEBUTTONUP:
            {

            }
            break;
            case SDL_KEYDOWN:
            {

            }
            break;
            // File drop
            case SDL_DROPFILE:
            {
                char* dropped_filedir = p_instance->sdl2.event.drop.file;
                // Shows directory of dropped file
                SDL_ShowSimpleMessageBox(
                    SDL_MESSAGEBOX_INFORMATION,
                    "File dropped on window",
                    dropped_filedir,
                    p_instance->sdl2.window
                );
                SDL_free(dropped_filedir);    // Free dropped_filedir memory
            }
            case SDL_WINDOWEVENT:
            {
                switch (p_instance->sdl2.event.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        // TODO: Uncomment
                        g_window_resize(p_instance);
                        break;
                    }
                }
            }
            break;
            case SDL_QUIT:
            {
                g_user_exit((callback_parameter_t){ 0 }, p_instance);
            }
            break;
            default:
                break;
        }
    }
   
    const u8* keyboard_state = SDL_GetKeyboardState(NULL);
    
    // Iterate over each key
    for (size_t i = 0; i < 110; i++)
    {

        // If the key is down...
        if ( keyboard_state[i] )
        {
            printf("%s", scancodes[i]);

            // Initialized data
            GXBind_t *p_bind = (GXBind_t *) dict_get(p_instance->input->bind_lut, scancodes[i]);

            if ( p_bind )
            {
                p_bind->active = true;

                call_bind(p_bind, (callback_parameter_t){ .input_state = KEYBOARD, .inputs = { .key = { .depressed = true } } });
            }
        }
    }

    // Game controller
    if ( controller )
    {

        // Initialized data
        callback_parameter_t input = (callback_parameter_t)
        {
            .input_state = GAMEPAD,

            .inputs.gamepad_state.left_trigger  = (float)(SDL_GameControllerGetAxis(controller, 4) / (float)SHRT_MAX),
            .inputs.gamepad_state.right_trigger = (float)(SDL_GameControllerGetAxis(controller, 5) / (float)SHRT_MAX),

            .inputs.gamepad_state.left_bumper   = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
            .inputs.gamepad_state.right_bumper  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),

            .inputs.gamepad_state.A = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A),
            .inputs.gamepad_state.B = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B),
            .inputs.gamepad_state.X = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X),
            .inputs.gamepad_state.Y = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y),

            .inputs.gamepad_state.dpad_down  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN),
            .inputs.gamepad_state.dpad_left  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT),
            .inputs.gamepad_state.dpad_right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT),
            .inputs.gamepad_state.dpad_up    = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP),

            .inputs.gamepad_state.start  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START),
            .inputs.gamepad_state.select = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK),

            .inputs.gamepad_state.left_stick = (vec2)
            {
                .x = (float)(SDL_GameControllerGetAxis(controller, 2) / (float)SHRT_MAX),
                .y = (float)(SDL_GameControllerGetAxis(controller, 3) / (float)SHRT_MAX)
            },
            .inputs.gamepad_state.right_stick = (vec2)
            {
                .x = (float)(SDL_GameControllerGetAxis(controller, 0) / (float)SHRT_MAX),
                .y = (float)(SDL_GameControllerGetAxis(controller, 1) / (float)SHRT_MAX)
            }
        };

    }

    // Call each bind
    /*
    while ( !queue_empty(p_instance->input->p_bind_queue) )
    {

        // Initialized data
        GXBind_t *p_bind = 0;

        // Dequeue a bind
        queue_dequeue(p_instance->input->p_bind_queue, &p_bind);
        call_bind(p_bind, callback_parameter, p_instance)
    }
    */

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_instance\" in call to \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_inputs:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] No input in \"p_instance\" in call to \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int append_bind ( GXInput_t *p_input, GXBind_t *p_bind )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_input == (void *) 0 ) goto no_input;
        if ( p_bind  == (void *) 0 ) goto no_bind;
    #endif

    // Add the bind to the binds
    dict_add(p_input->binds, p_bind->name, p_bind);
    
    // Iterate over each key
    for (size_t i = 0; i < p_bind->key_count; i++)
    {

        // Initialized data
        GXBind_t *base = dict_get(p_input->bind_lut, p_bind->keys[i]);

        // If this scancode doesn't have a bind, add it
        if ( base == (void *) 0 )
            dict_add(p_input->bind_lut, p_bind->keys[i], p_bind);

        // Add the bind to the linked list
        else
        {

            // Walk to the end of the linked list
            while(base->next) { base = base->next; };

            // This avoids a potential bug
            p_bind->next = 0;

            // Set the next bind
            base->next = p_bind;
        }
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int input_info ( GXInput_t *p_input )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_input == (void *) 0 ) goto no_input;
    #endif

    // Formatting
    g_print_log(" - Input info - \n");

    // Print the name
    g_print_log("name             : \"%s\"\n", p_input->name);

    // Print the mouse sensitivity
    g_print_log("mouse sensitivity: \"%.2f\"\n", p_input->mouse_sensitivity);

    // Format
    g_print_log("binds            : \n");

    // Print each bind
    size_t     len      = dict_values(p_input->binds, 0);
    GXBind_t **pp_binds = calloc(len+1, sizeof(GXBind_t*));

    // Error check
    if ( pp_binds == (void *) 0 ) goto no_mem;

    // Get an array of binds
    if ( dict_values(p_input->binds, pp_binds) == 0 )
        printf("");

    // Iterate over each bind
    for (size_t i = 0; i < len; i++)
    {
        // Initialized data
        GXBind_t *p_bind = pp_binds[i];

        // Format
        g_print_log("                   [%d] \"%s\": \n", i, p_bind->name);
        g_print_log("                   \t[ ", i, p_bind->name);

        // Print key binds
        if ( p_bind->key_count )
        {

            // Print the first bind
            g_print_log("%s", p_bind->keys[0]);

            // Iterate from the second bind to the Nth
            for (size_t j = 1; j < p_bind->key_count; j++)
                g_print_log(", %s", p_bind->keys[j]);

            
        }

        // Format
        g_print_log(" ]\n");
    }

    // Format
    putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input} Null pointer provided for parameter \"input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error handling
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

int call_bind ( GXBind_t *p_bind, callback_parameter_t input )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_bind == (void *) 0 ) goto no_bind;
    #endif

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();

    // Iterate over each callback
    for (size_t i = 0; i < p_bind->callback_count; i++)
    {

        // Initialized data
        void (*function)(callback_parameter_t input, GXInstance_t *b) = p_bind->callbacks[i];

        // Call the function
        if ( function )
            function(input, p_instance);
    }

    // Success
    return 1;

    // Error handling
    {
        no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Bind] Null pointer provided for parameter \"bind\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

GXBind_t *find_bind ( GXInput_t *p_input, char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_input == (void *) 0 ) goto no_input;
        if ( name    == (void *) 0 ) goto no_name;
    #endif

    // Return the bind
    return (GXBind_t *) dict_get(p_input->binds, name);

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int remove_bind ( GXInput_t *p_input, char *name, GXBind_t **pp_bind )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_input == (void *) 0 ) goto no_input;
        if ( name    == (void *) 0 ) goto no_name;
    #endif

    // Initialized data
    GXBind_t *p_bind = 0;

    // Remove the bind
    dict_pop(p_input->binds, name, (void **)&p_bind);

    // Did the caller specify a return?
    if ( pp_bind )

        // The bind belongs to the caller now
        *pp_bind = p_bind;
    
    // Default
    else

        // Destroy the bind
        destroy_bind(&p_bind);
    


    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_bind ( GXBind_t **pp_bind )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_bind == (void *) 0 ) goto no_bind;
    #endif

    // Initialized data
    GXBind_t *p_bind = *pp_bind;

    // No more pointer for caller
    *pp_bind = 0;

    // Free the name
    free(p_bind->name);

    // Iterate over each key
    for (size_t i = 0; i < p_bind->key_count; i++)

        // Free each key
        free(p_bind->keys[i]);

    // Free the callbacks
    if ( p_bind->callbacks )

        // Free the bind
        free(p_bind->callbacks);

    // Free the list of keys
    free(p_bind->keys);

    // Free the bind
    free(p_bind);

    // Success
    return 1;

    // Error handling
    {
        no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for parameter \"p_bind\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int destroy_input ( GXInput_t **pp_input )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_input == (void *) 0 ) goto no_input;
    #endif

    // Initialized data
    GXInput_t  *p_input    = *pp_input;
    size_t      bind_count = 0;
    GXBind_t  **pp_binds   = 0;

    if ( p_input == (void *) 0 ) goto pointer_to_null;

    // No more pointer for caller
    *pp_input = 0;

    // Get the number of binds
    bind_count = dict_values(p_input->binds, 0);
    
    // Allocate memory for the binds
    pp_binds = calloc(bind_count, sizeof(void *));

    // Error check
    if ( pp_binds == (void *) 0 ) goto no_mem;

    // Free the input name
    free(p_input->name);

    // Get each bind
    dict_values(p_input->binds, (void **)pp_binds);

    // Iterate over each bind
    for (size_t i = 0; i < bind_count; i++)

        // Destroy the bind
        if ( destroy_bind(&pp_binds[i]) == 0 ) goto failed_to_destroy_bind;

    // Free the list of binds
    free(pp_binds);
    
    // Free the input struct
    free(p_input);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null poiner provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Property \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                

            failed_to_destroy_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to destroy bind in call to function \"%s\"\n", __FUNCTION__);
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
