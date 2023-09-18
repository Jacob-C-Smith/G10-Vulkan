/** !
 * @file G10/GXCollider.h
 * @author Jacob Smith
 *
 * Colliders
 */

// Include guard
#pragma once

// Standard library
#include <stdbool.h>

// G10
#include <G10/G10.h>
#include <G10/GXBV.h>
//#include <G10/GXCollision.h>
#include <G10/GXLinear.h>

enum collider_type_e
{
    collider_invalid    = 0,
    collider_quad       = 1,
    collider_box        = 2,
    collider_sphere     = 3,
    collider_capsule    = 4,
    collider_cylinder   = 5,
    collider_cone       = 6,
    collider_convexhull = 7
};
typedef enum collider_type_e collider_type_t;

struct GXCollider_s
{

    // Collider type
    collider_type_t type;

    // Pointer to model matrix
    mat4 *model_matrix;

    // Pointer to bounding volume in the active scene's BVH
    GXBV_t *bv;

    dict *collisions;

    struct {
        vec3     aabb_max,
                 aabb_min;
        size_t   start_callback_count,
                 callback_count,
                 end_callback_count,
                 start_callback_max,
                 callback_max,
                 end_callback_max;
        void   **aabb_start_callbacks,
               **aabb_callbacks,
               **aabb_end_callbacks;
    } aabb;

    struct
    {
        size_t   start_callback_count,
                 callback_count,
                 end_callback_count,
                 start_callback_max,
                 callback_max,
                 end_callback_max;
        void   **obb_start_callbacks,
               **obb_callbacks,
               **obb_end_callbacks;
    } obb;

    struct {
        vec3   *convex_hull;
        size_t  convex_hull_count;
    } convex_hull;
};

// Allocators
/** !
 *  Allocate memory for a collider
 *
 * @param pp_collider : return
 *
 * @sa destroy_collider
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_collider ( GXCollider_t **pp_collider );

// Constructors
/** !
 *  Load a collider from a JSON file
 *
 * @param pp_collider : return
 * @param path        : The path to file containing collider JSON
 *
 * @sa load_collider_as_json_text
 * @sa load_collider_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_collider ( GXCollider_t **pp_collider, const char *path );

/** !
 *  Load a collider from JSON text
 *
 * @param pp_collider : return
 * @param text        : The collider JSON text
 *
 * @sa load_collider
 * @sa load_collider_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_collider_as_json_text ( GXCollider_t **pp_collider, char *text );

/** !
 *  Load a collider from a JSON value
 *
 * @param pp_collider : return
 * @param text        : The collider JSON text
 *
 * @sa load_collider
 * @sa load_collider_as_json_text
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_collider_as_json_value ( GXCollider_t **pp_collider, json_value *p_value );


// Callbacks
/** !
  *  Add a callback for when collision start
  *
  *  @param p_collider : Pointer to collider
  *  @param callback   : function pointer of type int (*function_pointer)(GXCollision_t *p_collision)
  *
  *  @sa add_aabb_collision_callback
  *  @sa add_aabb_end_collision_callback
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int add_aabb_start_collision_callback ( GXCollider_t *p_collider, int (*function_pointer)(GXCollision_t *p_collision) );

/** !
  *  Add a callback for when colliding
  *
  *  @param p_collider : Pointer to collider
  *  @param callback   : function pointer of type int (*function_pointer)(GXCollision_t *p_collision)
  *
  *  @sa add_aabb_start_collision_callback
  *  @sa add_aabb_end_collision_callback
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int add_aabb_collision_callback ( GXCollider_t *collider, void* function_pointer );

/** !
  *  Add a callback for when collision ends
  *
  *  @param p_collider : Pointer to collider
  *  @param callback   : function pointer of type int (*function_pointer)(GXCollision_t *p_collision)
  *
  *  @sa add_aabb_start_collision_callback
  *  @sa add_aabb_collision_callback
  *
  *  @return 1 on success, 0 on error.
  */
DLLEXPORT int add_aabb_end_collision_callback ( GXCollider_t *collider, void* function_pointer );

// Destructors
/** !
 *  Destroy a collider
 *
 * @param p_collider : Pointer to collider pointer
 *
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_collider ( GXCollider_t **pp_collider );