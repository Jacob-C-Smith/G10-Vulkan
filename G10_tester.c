/** !
 * @file G10 Tester
 * 
 * @author Jacob C Smith
*/

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <float.h>

// JSON  submodule
#include <json/json.h>

// Dictionary submodule
#include <dict/dict.h>

// Array submodule
#include <array/array.h>

// G10
#include <G10/G10.h>
#include <G10/GXAI.h>

//////////////////
// Test results //
//////////////////
typedef enum result_e {
    zero  = 0,
    one   = 1,
    match = 2
} result_t;

///////////////////
// Test counters //
///////////////////
int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

//////////////////////////
// Forward declarations //
//////////////////////////

// Utility functions
int run_tests           ( void );
int print_final_summary ( void );
int print_test          ( const char *scenario_name, const char   *test_name, bool passed );

//bool     test_parse_json     ( char         *test_file    , int         (*expected_value_constructor) (json_value **), result_t expected );
//bool     test_serial_json    ( char         *test_file    , int         (*expected_value_constructor) (json_value **), result_t expected );

result_t load_json    ( json_value **pp_value, char        *test_file );
result_t save_json    ( char         *path    , json_value *p_value );
result_t value_equals ( json_value  *a       , json_value *b );
size_t   load_file    ( const char   *path    , void        *buffer, bool binary_mode );

void test_ai ( char *name );
void test_linear ( char *name );

// AI
bool test_allocate_ai       ( GXAI_t **pp_ai, result_t expected );
bool test_load_ai           ( GXAI_t **pp_ai, char *path , result_t expected );
bool test_set_ai_state      ( GXAI_t **pp_ai, char *state, result_t expected );
bool test_info_ai           ( void );
bool test_tasks_ai          ( void );
bool test_user_callbacks_ai ( void );
bool test_destroy_ai        ( GXAI_t **pp_ai, result_t expected );

// Linear algebra
bool test_add_vec3                ( vec3 a, vec3  b, vec3  expected );
bool test_sub_vec3                ( vec3 a, vec3  b, vec3  expected );
bool test_length_vec3             ( vec3 v, float expected );
bool test_dot_product_vec3        ( vec3 a, vec3  b, float expected );
bool test_cross_product_vec3      ( vec3 a, vec3  b, vec3  expected );
bool test_mul_vec3_vec3           ( vec3 a, vec3  b, vec3  expected );
bool test_div_vec3_f              ( vec3 a, float s, vec3  expected );
bool test_mul_vec3_f              ( vec3 a, float s, vec3  expected );
bool test_normalize               ( vec3 v, vec3 expected );
bool test_mul_mat2_vec2           ( mat2 m, vec2 v, vec2 expected );
bool test_mul_mat4_vec4           ( mat4 m, vec4 v, vec4 expected );
bool test_mul_mat2_mat2           ( mat2 m, mat2 n, mat2 expected );
bool test_mul_mat4_mat4           ( mat4 m, mat4 n, mat4 expected );
bool test_rcp_mat2                ( mat2 m, mat2 expected );
bool test_rcp_mat4                ( mat4 m, mat4 expected );
bool test_identity_mat2           ( mat2 expected );
bool test_identity_mat4           ( mat4 expected );
bool test_translation_mat4              ( vec3 location, mat4 expected );
bool test_scale_mat4              ( vec3 scale, mat4 expected );
bool test_rotation_mat4_from_vec3 ( vec3 rotation, mat4 expected );
bool test_model_mat4_from_vec3    ( vec3 location, vec3 rotation, vec3 scale, mat4 expected );

GXInstance_t *p_instance = 0;

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialize a G10 instance
    g_init(&p_instance,"G10/tester instance.json");

    // Run tests
    run_tests();

    // Destroy a G10 instance
    g_exit(&p_instance);

    // Success
    return EXIT_SUCCESS;
}

int run_tests ( void )
{

    // Test the FSA
    test_ai("ai");

    // Test audio
    //test_audio("audio");

    // Test bounding volumes
    //test_bounding_volume("bounding volume");

    // Test camera
    //test_camera("camera");

    // Test collider
    //test_collider("collider");

    // Test collision
    //test_collision("collision");

    // Test entity
    //test_entity("entity");

    // Test input
    //test_input("input");

    // Test linear algebra
    test_linear("linear");

    // Test liner algebra assembly
    //test_linear_asm("linear asm");

    // Test material
    //test_material("material");

    // Test part
    //test_part("part");

    // Test physics
    //test_physics("physics");

    // Test polygon file loader
    //test_PLY("PLY");

    // Test quaternion math
    //test_quaternion("quaternion");

    // Test the renderer
    //test_renderer("renderer");

    // Test the rigidbody
    //test_rigidbody("rigidbody");

    // Test the scene
    //test_scene("scene");

    // Test the scheduler
    //test_scheduler("scheduler");

    // Test the server
    //test_server("server");

    // Test the shader
    //test_shader("shader");

    // Test the texture
    //test_texture("texture");

    // Test the transform
    //test_transform("transform");

    // Test the user code task
    //test_user_code("user code");

    // Success
    return 1;
}

void test_ai ( char *name )
{

    // Initialized data
    GXAI_t *p_ai = 0;

    // Output
    printf("Scenario: %s\n", name);

    print_test(name, "create null"                       , test_allocate_ai(0, 0));
    print_test(name, "create &p_ai"                      , test_allocate_ai(&p_ai, 1));
    print_test(name, "load with schema property"         , test_load_ai(&p_ai, "gtest/pass/ai/valid1.json"                , 1));
    print_test(name, "load without schema property"      , test_load_ai(&p_ai, "gtest/pass/ai/valid2.json"                , 1));
    print_test(name, "load with different initial state" , test_load_ai(&p_ai, "gtest/pass/ai/valid3.json"                , 1));
    print_test(name, "load (null) path"                  , test_load_ai(&p_ai, 0                                          , 0));
    print_test(name, "load empty object"                 , test_load_ai(&p_ai, "gtest/fail/ai/empty object.json"          , 0));
    print_test(name, "load empty json"                   , test_load_ai(&p_ai, "gtest/fail/ai/empty.json"                 , 0));
    print_test(name, "load missing state info"           , test_load_ai(&p_ai, "gtest/fail/ai/missing state info.json"    , 0));
    print_test(name, "load empty name property"          , test_load_ai(&p_ai, "gtest/fail/ai/name/empty property.json"   , 0));
    print_test(name, "load no name property"             , test_load_ai(&p_ai, "gtest/fail/ai/name/no property.json"      , 0));
    print_test(name, "load name property is not a string", test_load_ai(&p_ai, "gtest/fail/ai/name/not a string.json"     , 0));
    print_test(name, "load name property too long"       , test_load_ai(&p_ai, "gtest/fail/ai/name/property too long.json", 0));
    print_test(name, "load with schema property"         , test_load_ai(&p_ai, "gtest/pass/ai/valid1.json"                , 1));
    print_test(name, "set state run left"                , test_set_ai_state(p_ai, "RUN LEFT", 1));
    print_test(name, "set state attack"                  , test_set_ai_state(p_ai, "ATTACK", 1));
    print_test(name, "set state not a state"             , test_set_ai_state(p_ai, "NOT A STATE", 0));
    print_test(name, "destructor null"                   , test_destroy_ai(0, 0));
    print_test(name, "destructor &p_ai"                  , test_destroy_ai(&p_ai, 1));
    print_final_summary();

    // Success
    return;

}
void test_camera ( char *name )
{
    // Initialized data
    GXCamera_t *p_camera = 0;

    // Output
    printf("Scenario: %s\n", name);

    // TODO: Test constructors, loaders, destructors, matrix operations, and get_camera_position

    //print_test(name, "create null"                       , test_allocate_camera(0, 0));
    //print_test(name, "create &p_camera"                  , test_allocate_camera(&p_camera, 1));
    //print_test(name, "load with schema property"         , test_load_camera(&p_camera, "gtest/pass/camera/valid1.json"                , 1));
    //print_test(name, "load without schema property"      , test_load_camera(&p_camera, "gtest/pass/camera/valid2.json"                , 1));
    //print_test(name, "load with different initial state" , test_load_camera(&p_camera, "gtest/pass/camera/valid3.json"                , 1));
    //print_test(name, "load empty object"                 , test_load_camera(&p_camera, "gtest/fail/camera/empty object.json"          , 0));
    //print_test(name, "load empty json"                   , test_load_camera(&p_camera, "gtest/fail/camera/empty.json"                 , 0));
    //print_test(name, "load missing state info"           , test_load_camera(&p_camera, "gtest/fail/camera/missing state info.json"    , 0));
    //print_test(name, "load empty name property"          , test_load_camera(&p_camera, "gtest/fail/camera/name/empty property.json"   , 0));
    //print_test(name, "load no name property"             , test_load_camera(&p_camera, "gtest/fail/camera/name/no property.json"      , 0));
    //print_test(name, "load name property is not a string", test_load_camera(&p_camera, "gtest/fail/camera/name/not a string.json"     , 0));
    //print_test(name, "load name property too long"       , test_load_camera(&p_camera, "gtest/fail/camera/name/property too long.json", 0));
    //print_test(name, "destructor null" , test_destroy_camera(0, 0));
    //print_test(name, "destructor &p_camera", test_destroy_camera(&p_camera, 1));
    //print_final_summary();

    // Success
    return;

}
void test_linear ( char *name )
{

    // Initialized data
    GXAI_t *p_ai = 0;

    // Output
    printf("Scenario: %s\n", name);

    print_test(name, "<0, 0, 0> + <3, 1, 4>           -> <3, 1, 4>"                                   , test_add_vec3((vec3){ .x = 0.f, .y = 0.f, .z = 0.f }, (vec3){ .x = 3.f, .y = 1.f, .z = 4.f }, (vec3){ .x =  3.f, .y =  1.f, .z =  4.f }));
    print_test(name, "<0, 0, 0> - <3, 1, 4>           -> <-3, -1, -4>"                                , test_sub_vec3((vec3){ .x = 0.f, .y = 0.f, .z = 0.f }, (vec3){ .x = 3.f, .y = 1.f, .z = 4.f }, (vec3){ .x = -3.f, .y = -1.f, .z = -4.f }));
    print_test(name, "||<1, 4, 1>|| (length)          -> 3.4641"                                      , test_length_vec3((vec3){ .x = 2.f, .y = 2.f, .z = 2.f }, 3.46410162f));
    print_test(name, "<2, 2, 2> DOT <2, 2, 2>         -> 12.0"                                        , test_dot_product_vec3((vec3){ .x = 2.f, .y = 2.f, .z = 2.f }, (vec3){ .x = 2.f, .y = 2.f, .z = 2.f }, 12.f));
    print_test(name, "<1, 0, 0> CROSS <0, 1, 0>       -> <0, 0, 1>"                                   , test_cross_product_vec3((vec3){ .x = 1.f, .y = 0.f, .z = 0.f }, (vec3){ .x = 0.f, .y = 1.f, .z = 0.f }, (vec3){ .x = 0.f, .y = 0.f, .z = 1.f }));
    print_test(name, "<0, 1, 0> CROSS <1, 0, 0>       -> <0, 0, -1>"                                  , test_cross_product_vec3((vec3){ .x = 0.f, .y = 1.f, .z = 0.f }, (vec3){ .x = 1.f, .y = 0.f, .z = 0.f }, (vec3){ .x = 0.f, .y = 0.f, .z = -1.f }));
    print_test(name, "<2, 4, 8> * <8, 4, 2>           -> <16, 16, 16>"                                , test_mul_vec3_vec3((vec3){ .x = 2.f, .y = 4.f, .z = 8.f }, (vec3){ .x = 8.f, .y = 4.f, .z = 2.f }, (vec3){ .x = 16.f, .y = 16.f, .z = 16.f }));
    print_test(name, "<2, 4, 8> / 2                   -> <1, 2, 4>"                                   , test_div_vec3_f((vec3){ .x = 2.f, .y = 4.f, .z = 8.f }, 2.f, (vec3){ .x = 1.f, .y = 2.f, .z = 4.f }));
    print_test(name, "|<1, 4, 1>| (normalized)        -> <1, 4, 1>"                                   , test_normalize((vec3){ .x = 2.f, .y = 4.f, .z = 8.f }, (vec3){ .x = 4.f, .y = 8.f, .z = 16.f }));
    print_test(name, "<2, 4, 8> * 2                   -> <4, 8, 16>"                                  , test_mul_vec3_f((vec3){ .x = 2.f, .y = 4.f, .z = 8.f }, 2.f, (vec3){ .x = 4.f, .y = 8.f, .z = 16.f }));
    print_test(name, "<<1,2>, <3,4>> * <1,1>          -> <3,7>"                                       , test_mul_mat2_vec2 ( (mat2) { .a = 1.f, .b = 2.f, .c = 3.f, .d = 4.f}, (vec2) { .x = 1.f, .y = 1.f }, (vec2){ .x = 3.f, .y = 7.f} ));
    print_test(name, "<<1,2>, <2,1>> * <<1,3>, <1,2>> -> <<3,7>, <3,8>>"                              , test_mul_mat2_mat2 ( (mat2){ .a = 1.f, .b = 2.f, .c = 2.f, .d = 1.f }, (mat2){ .a = 1.f, .b = 3.f, .c = 1.f, .d = 2.f }, (mat2){ .a = 3.f, .b = 7.f, .c = 3.f, .d = 8.f } ));
    print_test(name, "IDENTITY_MAT2()                 -> <<1,0>, <0,1>>"                              , test_identity_mat2 ( (mat2) { .a = 1.f, .b = 0.f, .c = 0.f, .d = 1.f } ));
    print_test(name, "IDENTITY_MAT4()                 -> <<1,0,0,0>, <0,1,0,0>, <0,1,0,0>, <0,0,0,1>>", test_identity_mat4 ( (mat4) {.a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f, .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f, .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f, .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f} ));
    print_test(name, "TRANSLATION_MAT4(<2, 7, 2>)     -> <<1,0,0,0>, <0,1,0,0>, <0,1,0,0>, <2,7,2,1>>", test_translation_mat4 ( (vec3) {.x = 2.f, .y = 7.f, .z = 2.f}, (mat4) {.a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f, .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f, .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f, .m = 2.f, .n = 7.f, .o = 2.f, .p = 1.f} ));
    print_test(name, "SCALE_MAT(<3, 4, 5>)            -> <<3,0,0,0>, <0,4,0,0>, <0,0,5,0>, <0,0,0,0>>", test_scale_mat4 ( (vec3) {.x = 3.f, .y = 4.f, .z = 5.f}, (mat4) {.a = 3.f, .b = 0.f, .c = 0.f, .d = 0.f, .e = 0.f, .f = 4.f, .g = 0.f, .h = 0.f, .i = 0.f, .j = 0.f, .k = 5.f, .l = 0.f, .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f} ));

    // TODO:
    // test_mul_mat4_vec4           ( mat4 m, vec4 v, vec4 expected );
    // test_mul_mat4_mat4           ( mat4 m, mat4 n, mat4 expected );
    // test_rcp_mat2                ( mat2 m, mat2 expected );
    // test_rcp_mat4                ( mat4 m, mat4 expected );
    // test_rotation_mat4_from_vec3 ( vec3 rotation, mat4 expected ));
    // test_model_mat4_from_vec3    ( vec3 location, vec3 rotation, vec3 scale, mat4 expected ));
    // test_mul_vec3_f              ( (vec3) { .x = 2.f, .y = 4.f, .z = 8.f }, 2.f, (vec3){ .x = 4.f, .y = 8.f, .z = 16.f })));

    print_final_summary();
    // Success
    return;
}

/*
void test_audio ( char *name )
{

}
void test_bounding_volume ( char *name )
{

}
void test_collider ( char *name )
{

}
void test_collision ( char *name )
{

}
void test_entity ( char *name )
{

}
void test_input ( char *name )
{

}
void test_linear_asm ( char *name )
{

}
void test_material ( char *name )
{

}
void test_part ( char *name )
{

}
void test_physics ( char *name )
{

}
void test_PLY ( char *name )
{

}
void test_quaternion ( char *name )
{

}
void test_renderer ( char *name )
{

}
void test_rigidbody ( char *name )
{

}
void test_scene ( char *name )
{

}
void test_scheduler ( char *name )
{

}
void test_server ( char *name )
{

}
void test_shader ( char *name )
{

}
void test_texture ( char *name )
{

}
void test_transform ( char *name )
{

}
void test_user_code ( char *name )
{

}
*/
bool test_allocate_ai ( GXAI_t **pp_ai, result_t expected )
{

    // Initialized data
    int result = 0;

    result = create_ai(pp_ai);

    // Return
    return (result == expected);
}
bool test_load_ai ( GXAI_t **pp_ai, char *path, result_t expected )
{

    // Initialized data
    int result = 0;

    result = load_ai(pp_ai, path);

    // Return
    return (result == expected);
}
bool test_set_ai_state ( GXAI_t **pp_ai, char *state, result_t expected )
{

    // Initialized data
    int result = 0;

    result = set_ai_state(*pp_ai, state);

    // Return
    return (result == expected);
}
bool test_destroy_ai ( GXAI_t **pp_ai, result_t expected )
{

    // Initialized data
    int result = 0;

    result = destroy_ai(pp_ai);

    // Return
    return (result == expected);
}

bool test_add_vec3 ( vec3 a, vec3 b, vec3 expected )
{

    // Initialized data
    vec3 result = { .x = 0, .y = 0, .z = 0 };

    add_vec3(&result, a, b);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_sub_vec3 ( vec3 a, vec3 b, vec3 expected )
{

    // Initialized data
    vec3 result = { .x = 0, .y = 0, .z = 0 };

    sub_vec3(&result, a, b);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_length_vec3 ( vec3 v, float expected )
{

    // Initialized data
    float result = length(v);

    // Return
    return result == expected;
}
bool test_dot_product_vec3 ( vec3 a, vec3 b, float expected )
{

    // Initialized data
    float result = dot_product_vec3(a, b);

    // Return
    return result == expected;
}
bool test_cross_product_vec3 ( vec3 a, vec3 b, vec3 expected )
{

    // Initialized data
    vec3 result = cross_product_vec3(a, b);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_mul_vec3_vec3 ( vec3 a, vec3 b, vec3 expected )
{

    // Initialized data
    vec3 result = { .x = 0.f, .y = 0.f, .z = 0.f };

    mul_vec3_vec3(&result, a, b);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_div_vec3_f ( vec3 a, float s, vec3 expected )
{

    // Initialized data
    vec3 result = { .x = 0.f, .y = 0.f, .z = 0.f };

    div_vec3_f(&result, a, s);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_mul_vec3_f ( vec3 a, float s, vec3 expected )
{

    // Initialized data
    vec3 result = mul_vec3_f(a, s);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_normalize ( vec3 v, vec3 expected )
{

    // Initialized data
    vec3 result = normalize(v);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) &&
             (result.z == expected.z) );
}
bool test_mul_mat2_vec2 ( mat2 m, vec2 v, vec2 expected )
{

    // Initialized data
    vec2 result = mul_mat2_vec2(m,v);

    // Return
    return ( (result.x == expected.x) &&
             (result.y == expected.y) );
}
bool test_mul_mat4_vec4 ( mat4 m, vec4 v, vec4 expected )
{

    return false;
}
bool test_mul_mat2_mat2 ( mat2 m, mat2 n, mat2 expected )
{
    // Initialized data
    mat2 result = mul_mat2_mat2(m,n);

    // Return
    return ( (result.a == expected.a) &&
             (result.b == expected.b) &&
             (result.c == expected.c) &&
             (result.d == expected.d) );
}
bool test_mul_mat4_mat4 ( mat4 m, mat4 n, mat4 expected )
{

    return false;
}
bool test_rcp_mat2 ( mat2 m, mat2 expected )
{

    return false;
}
bool test_rcp_mat4 ( mat4 m, mat4 expected )
{

    return false;
}
bool test_identity_mat2 ( mat2 expected )
{
    // Initialized data
    mat2 result = identity_mat2();

    // Return
    return ( (result.a == expected.a) &&
             (result.b == expected.b) &&
             (result.c == expected.c) &&
             (result.d == expected.d) );
}
bool test_identity_mat4 ( mat4 expected )
{
    // Initialized data
    mat4 result = identity_mat4();

    // Return
    return ( (result.a == expected.a) &&
             (result.b == expected.b) &&
             (result.c == expected.c) &&
             (result.d == expected.d) &&
             (result.e == expected.e) &&
             (result.f == expected.f) &&
             (result.g == expected.g) &&
             (result.h == expected.h) &&
             (result.i == expected.i) &&
             (result.j == expected.j) &&
             (result.k == expected.k) &&
             (result.l == expected.l) &&
             (result.m == expected.m) &&
             (result.n == expected.n) &&
             (result.o == expected.o) &&
             (result.p == expected.p) );
}
bool test_translation_mat4 ( vec3 location, mat4 expected )
{
    // Initialized data
    mat4 result = translation_mat4(location);

    // Return
    return ( (result.a == expected.a) &&
             (result.b == expected.b) &&
             (result.c == expected.c) &&
             (result.d == expected.d) &&
             (result.e == expected.e) &&
             (result.f == expected.f) &&
             (result.g == expected.g) &&
             (result.h == expected.h) &&
             (result.i == expected.i) &&
             (result.j == expected.j) &&
             (result.k == expected.k) &&
             (result.l == expected.l) &&
             (result.m == expected.m) &&
             (result.n == expected.n) &&
             (result.o == expected.o) &&
             (result.p == expected.p) );
}
bool test_scale_mat4 ( vec3 scale, mat4 expected )
{
    // Initialized data
    mat4 result = scale_mat4(scale);

    // Return
    return ( (result.a == expected.a) &&
             (result.b == expected.b) &&
             (result.c == expected.c) &&
             (result.d == expected.d) &&
             (result.e == expected.e) &&
             (result.f == expected.f) &&
             (result.g == expected.g) &&
             (result.h == expected.h) &&
             (result.i == expected.i) &&
             (result.j == expected.j) &&
             (result.k == expected.k) &&
             (result.l == expected.l) &&
             (result.m == expected.m) &&
             (result.n == expected.n) &&
             (result.o == expected.o) &&
             (result.p == expected.p) );
}
bool test_rotation_mat4_from_vec3 ( vec3 rotation, mat4 expected )
{

    return false;
}
bool test_model_mat4_from_vec3 ( vec3 location, vec3 rotation, vec3 scale, mat4 expected )
{

    return false;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    printf("%s %-85s %s\n",scenario_name, test_name, (passed) ? "PASS" : "FAIL");

    // Increment the counters
    if (passed)
        ephemeral_passes++;
    else
        ephemeral_fails++;

    ephemeral_tests++;

    // Success
    return 1;
}

int print_final_summary ()
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    printf("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    printf("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));

    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Success
    return 1;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking
    #ifndef NDEBUG
        if ( path == 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");

    // Check if file is valid
    if ( f == NULL )
        goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Read to data
    if ( buffer )
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);

    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[JSON] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // Error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

            // Error
            return 0;
        }
    }
}
