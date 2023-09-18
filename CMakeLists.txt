cmake_minimum_required (VERSION 3.26)

project ("g10")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

IF(WIN32)
    find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
    find_package(SDL2_net REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
ELSEIF(LINUX)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_net REQUIRED)
ELSEIF(APPLE)

ENDIF(WIN32)

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Log
    message("[G10] Adding array project")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Log
    message("[G10] Adding dict project")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    set(HAS_DICT true PARENT_SCOPE)
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Log
    message("[G10] Adding queue project")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    set(HAS_QUEUE true PARENT_SCOPE)
endif()

# Find the stack module
if ( NOT "${HAS_STACK}")
    
    # Log
    message("[G10] Adding stack project")
    
    # Add the stack directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/stack ${CMAKE_SOURCE_DIR}/build/stack)

    # Let any parent project know that this project has stack
    set(HAS_STACK true PARENT_SCOPE)
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[G10] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    set(HAS_SYNC true PARENT_SCOPE)
endif()

# Find the JSON module
if ( NOT "${HAS_JSON}")
    
    # Log
    message("[G10] Adding JSON project")
    
    # Add the JSON directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    set(HAS_JSON true PARENT_SCOPE)
endif()

if ( NOT "${HAS_G10}")
    set(HAS_G10 true PARENT_SCOPE)
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# G10 executable
add_executable (g10_internal_example "G10.c" "GXAI.c" "GXBV.c" "GXCamera.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXEntity.c" "GXInput.c" "GXLinear.c" "GXMaterial.c"  "GXPhysics.c" "GXPLY.c" "GXQuaternion.c" "GXRigidbody.c" "GXScene.c" "GXScheduler.c" "GXServer.c" "GXTransform.c" "GXUserCode.c" "GXPart.c" "main.c")  # "GXShader.c" "GXRenderer.c"
#add_executable (g10_internal_example "Resource.rc")
add_dependencies(g10_internal_example json array dict stack queue sync)
target_include_directories(g10_internal_example PUBLIC ${CMAKE_SOURCE_DIR}/include/ ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
target_link_libraries(g10_internal_example PUBLIC json array dict stack queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} PRIVATE SDL2_image::SDL2_image SDL2_net::SDL2_net )

# G10 library
add_library (g10 SHARED "G10.c" "GXAI.c" "GXBV.c" "GXCamera.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXEntity.c" "GXInput.c" "GXLinear.c" "GXMaterial.c" "GXPhysics.c" "GXPLY.c" "GXQuaternion.c"  "GXRigidbody.c" "GXTransform.c" "GXScene.c" "GXScheduler.c" "GXServer.c" "GXPart.c" "GXUserCode.c") # "GXPart.c" "GXShader.c" "GXRenderer.c"
add_dependencies(g10 json array dict stack queue sync)
target_include_directories(g10 PUBLIC  ${CMAKE_SOURCE_DIR}/include/ ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
target_link_libraries(g10 PUBLIC json array dict stack queue sync ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} PRIVATE SDL2_image::SDL2_image SDL2_net::SDL2_net )

# G10 executable with address sanitizer
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#add_executable (g10_asan_example "G10.c" "GXAI.c" "GXBV.c" "GXCamera.c" "GXCameraController.c" "GXCollider.c" "GXCollision.c" "GXEntity.c" "GXInput.c" "GXLinear.c" "GXMaterial.c" "GXPart.c" "GXPhysics.c" "GXPLY.c" "GXQuaternion.c" "GXRenderer.c" "GXRigidbody.c" "GXScene.c" "GXScheduler.c" "GXServer.c" "GXShader.c" "GXTransform.c" "GXUserCode.c" "main.c" ${CMAKE_SOURCE_DIR}/extern/sync/sync.c ${CMAKE_SOURCE_DIR}/extern/array/array.c ${CMAKE_SOURCE_DIR}/extern/dict/dict.c ${CMAKE_SOURCE_DIR}/extern/stack/stack.c ${CMAKE_SOURCE_DIR}/extern/queue/queue.c ${CMAKE_SOURCE_DIR}/extern/json/json.c ) 
##add_executable (g10_asan_example "Resource.rc")
#target_include_directories(g10_asan_example PUBLIC include ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/queue/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/) 
#target_link_libraries(g10_asan_example PUBLIC ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_INCLUDE_DIRS} ${VULKAN_LIB_LIST} PRIVATE SDL2_image::SDL2_image SDL2_net::SDL2_net )