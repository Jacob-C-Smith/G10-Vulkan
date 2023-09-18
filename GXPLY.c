#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Vulkan
//#include <vulkan/vulkan.h>

#include <G10/GXtypedef.h>
#include <G10/GXPart.h>

// PLY Debugging
#define GXPLY_CheckTriangulation  false

// PLY header strings
#define GXPLY_HSignature          0x0A796C70 // "ply\n"
#define GXPLY_HFormat             0x6D726F66 // "form"
#define GXPLY_HComment            0x6D6D6F63 // "comm"
#define GXPLY_HElement            0x6D656C65 // "elem"
#define GXPLY_HProperty           0x706F7270 // "prop"
#define GXPLY_HEnd                0x5F646E65 // "end_"

// PLY types
#define GXPLY_char                0x72616863 // "char"
#define GXPLY_uchar               0x61686375 // "ucha"
#define GXPLY_short               0x726F6873 // "shor"
#define GXPLY_ushort              0x6F687375 // "usho"
#define GXPLY_int                 0x20746E69 // "int "
#define GXPLY_uint                0x746E6975 // "uint"
#define GXPLY_float               0x616F6C66 // "floa"
#define GXPLY_double              0x62756F64 // "doub"
#define GXPLY_list                0x7473696C // "list"

// Vertex groups that are likely to be encountered
#define GXPLY_Geometric           0x00001
#define GXPLY_Texture             0x00002
#define GXPLY_Normal              0x00004
#define GXPLY_Bitangent           0x00008
#define GXPLY_Tangent             0x00010
#define GXPLY_Color               0x00020
#define GXPLY_Bones               0x00040
#define GXPLY_Weights             0x00080

#define GXPLY_MAXVERTEXGROUPS     8

// Geometric
#define GXPLY_X                   0x000001
#define GXPLY_Y                   0x000002
#define GXPLY_Z                   0x000004

// Texture
#define GXPLY_S                   0x000008
#define GXPLY_T                   0x000010

// Normal
#define GXPLY_NX                  0x000020
#define GXPLY_NY                  0x000040
#define GXPLY_NZ                  0x000080

// Tangent
#define GXPLY_TX                  0x0800000
#define GXPLY_TY                  0x1000000
#define GXPLY_TZ                  0x2000000


// Bitangent
#define GXPLY_BX                  0x000100
#define GXPLY_BY                  0x000200
#define GXPLY_BZ                  0x000400

// Color
#define GXPLY_R                   0x000800
#define GXPLY_G                   0x001000
#define GXPLY_B                   0x002000
#define GXPLY_A                   0x004000

// Bones
#define GXPLY_B0                  0x008000
#define GXPLY_B1                  0x010000
#define GXPLY_B2                  0x020000
#define GXPLY_B3                  0x040000

// Bone weights
#define GXPLY_BW0                 0x080000
#define GXPLY_BW1                 0x100000
#define GXPLY_BW2                 0x200000
#define GXPLY_BW3                 0x400000

enum GXply_fileType_e {
    none   = 0,
    ASCII  = 1,
    little = 2,
    big    = 3
};
typedef enum GXply_fileType_e GXply_fileType_t;

struct GXPLYproperty_s
{
    size_t  type_size;
    char   *name;
};
typedef struct GXPLYproperty_s GXPLYproperty_t;

struct GXPLYelement_s
{
    size_t           n_count;
    size_t           n_properties;
    size_t           s_stride;
    GXPLYproperty_t *properties;
    char            *name;
};
typedef struct GXPLYelement_s GXPLYelement_t;

struct GXply_file_s {
    size_t           n_elements,
                     flags;
    GXply_fileType_t  format;
    GXPLYelement_t  *elements;
};
typedef struct GXply_file_s GXply_file_t;

#pragma pack (push)
#pragma pack (1)
struct GXPLYindex_s {
    u8  count;
    u32 a;
    u32 b;
    u32 c;
};
#pragma pack (pop)
typedef struct GXPLYindex_s GXPLYindex_t;

GXPart_t *load_ply ( GXPart_t *part, const char *path )
{

    // Commentary
    {
        /*
         * The PLY loaders job is  to turn a PLY file header into a 64 bit metadata value.
         * the function is written to recognize vertex properties and parse them into common vertex
         * groups. Each vertex property in a vertex group should appear concurrently. Each
         * group has an associated byte code. The vertex groups metadata value is used to create a
         * vertex array object, vertex buffer object, and element buffer object.
         *
         * ╭─────────────────────┬──────────────────────────────────────────────────┬──────╮
         * │ Vertex group name   │ Vertex properties as they appear in the PLY file │ Code │
         * ├─────────────────────┼──────────────────────────────────────────────────┼──────┤
         * │ Geometric vertices  │ ( x, y, z )                                      │ 0x01 │
         * │ Texture coordinates │ ( u, v ) or ( s, t )                             │ 0x02 │
         * │ Vertex normals      │ ( nx, ny, nz )                                   │ 0x04 │
         * │ Vertex bitangents   │ ( bx, by, bz )                                   │ 0x08 │
         * │ Vertex tangents     │ ( tx, ty, tz )                                   │ 0x10 │
         * │ Vertex colors       │ ( red, green, blue, alpha ) or ( r, g, b, a )    │ 0x20 │
         * │ Bone groups         │ ( b0, b1, b2, b3 )                               │ 0x40 │
         * │ Bone weights        │ ( w0, w1, w2, w3 )                               │ 0x80 │
         * ╰─────────────────────┴──────────────────────────────────────────────────┴──────╯
         *
         * NOTE: The function can be modified to read two more vertex groups, supporting a
         * maximum of 8 vertex groups.
         *
         * The parser makes several passes through the header, populating more and more data each
         * time. Each pass and its purpose is listed below
         *
         * Pass 1.
         *     On pass 1, the function counts all the elements in the header. It will also print
         *     any comments it n_counters. After this pass, the elements are allocated for
         *
         * Pass 2.
         *     On pass 2, the elements are populated and the properties are counted up and
         *     allocated for.
         *
         * Pass 3.
         *     On pass 3, the properties are populated and the stride of the element is computed.
         *     Each property is kept track of using bitflags and a temporary value. The temporary
         *     value is then tested with various AND masks to deduce what vertex groups are
         *     present and which are absent.
         *
         *     All this work to determine the vertex groups is integrated when the function creates
         *     the vertexGroups quad word. This 64 bit number is an especially compact way to
         *     represent both the type and position of each vertex group in the file. Each
         *     code is packed into the 64 bit number, such that the last vertex group occupies
         *     the eight least significant bits. For comprehension and completeness, here are a
         *     few examples and how they should be interpreted.
         *
         *        63......................................0
         *     A. | 00 | 00 | 00 | 00 | 00 | 01 | 04 | 20 |
         *     B. | 00 | 00 | 00 | 00 | 00 | 04 | 02 | 01 |
         *     C. | 01 | 02 | 04 | 08 | 10 | 20 | 40 | 80 |
         *
         *     A. First is geometric, second is normals, third is colors
         *     B. First is normals, second is texture coordinates, third is geometric
         *     C. First is geometric, second is texture coordinates, third is vertex normals, fourth is
         *        vertex bitangents, fifth is vertex tangents, sixth is vertex bitangents, seventh is
         *        bone groups,and eighth is bone weights.
         *
         * After the third pass, all the metadata to describe the file data has been extracted, and
         * the function can start making the VAO, and VBO. The last significant byte
         * of the vertex groups variable is masked off, and switch()ed against the vertex group codes.
         * Vertex attribute pointers are created for the specific offset and stride of the vertex
         * group. The first vertex group will be at layout position 0, the second at 1, the third at 2, and soforth.
         *
         *
         */
    }

    // Argument check
    #ifndef NDEBUG
        if ( part == (void *) 0 ) goto no_part;
        if ( path == (void *) 0 ) goto no_path;
    #endif

    // Uninitialized data
    GXInstance_t  *p_instance  = g_get_active_instance();
    size_t         vertices_in_buffer = 0,
                   indices_in_buffer = 0;

    char          *c_data = 0;
    float         *vertex_array = 0;
    GXPLYindex_t  *indices = 0;
    u32           *corrected_indicies = 0;

    // Initialized data
    size_t         i                  = g_load_file(path, 0, true),
                   j                  = 0,
                   k                  = 0,
                   vertex_group_count   = 0,
                   vertex_attrib_offset = 0,
                   icount               = 0;
    char          *data                 = calloc(i, sizeof(u8));
    GXply_file_t   *ply_file            = calloc(1, sizeof(GXply_file_t));

    // Load the file
    if ( g_load_file(path, data, true) == 0 ) goto failed_to_load_file;

    // Reset counter
    i = 0;

    // Populate the PLY file
    {

        // Make sure the header isn't corrupted
        if ( *(u32*)data != GXPLY_HSignature ) goto invalidHeader;

        // Copy of data pointer
        c_data = data;

        // Pass 1
        while ( *(u32*)c_data != GXPLY_HEnd )
        {
            // Check the first four bytes of the line
            if ( *(u32*)c_data == GXPLY_HElement )
                ply_file->n_elements++;

            // Here is as good a place as any to look for comments
            #ifndef NDEBUG
                if ( *(u32*)c_data == GXPLY_HComment )
                {
                    i = 0;
                    while ( c_data[++i] != '\n' );
                }
            #endif

            // Zero set the index
            i = 0;
            while (c_data[++i] != '\n'); // Skip to the end of the line
            c_data = &c_data[i + 1];      // Set the pointer
        }

        // Allocate for PLY file elements
        ply_file->elements = calloc(ply_file->n_elements+1, sizeof(GXPLYelement_t));

        // Copy the data pointer again
        c_data = data;

        // Pass 2
        {
            while (*(u32*)c_data != GXPLY_HEnd)
            {

                // Check if iterator is on an element
                if (*(u32*)c_data == GXPLY_HElement)
                {
                    // TODO: Dynamically determine size.
                    char *l = strchr(c_data+8, ' ');
                    size_t n_len = l-(c_data+8);
                    ply_file->elements[j].name = calloc(n_len+1, sizeof(u8));
                    strncpy(ply_file->elements[j].name, c_data+8, n_len);

                    ply_file->elements[j].n_count = atoi(c_data + 8 + n_len+1);

                    i = 0;
                    while (c_data[++i] != '\n'); // Skip to the end of the line
                    c_data = &c_data[i + 1];      // Set the pointer

                    // Check if iterator is on a property
                    while (*(u32*)c_data == GXPLY_HProperty)
                    {
                        // Increment properties
                        ply_file->elements[j].n_properties += 1;

                        // Zero set the index
                        i = 0;
                        while (c_data[++i] != '\n'); // Skip to the end of the line
                        c_data = &c_data[i + 1];      // Set the pointer
                    }

                    goto p2propertyExit;

                    // TODO: Copy out the name, count, and veretx count
                }

                // Zero set the index
                i = 0;
                while (c_data[++i] != '\n'); // Skip to the end of the line
                c_data = &c_data[i + 1];      // Set the pointer
                continue;

                // Allocate space for the properties
            p2propertyExit:
                ply_file->elements[j].properties = calloc(ply_file->elements[j].n_properties, sizeof(GXPLYproperty_t));
                j++;
            }
        }

        // Zero set j
        j ^= j;

        // Copy data pointer again
        c_data = data;

        // Pass 3
        {
            while (*(u32*)c_data != GXPLY_HEnd)
            {

                // Check if iterator is on an element
                if (*(u32*)c_data == GXPLY_HElement)
                {
                    // TODO: Dynamically determine size.

                    i = 0;
                    while (c_data[++i] != '\n'); // Skip to the end of the line
                    c_data = &c_data[i + 1];      // Set the pointer

                    // Check if iterator is on a property
                    while (*(u32*)c_data == GXPLY_HProperty)
                    {
                        // Increment properties
                        c_data += 9;

                        // Compute stride and type size for each element and property respectively
                        switch (*(u32*)c_data)
                        {
                        case GXPLY_char:
                        case GXPLY_uchar:
                            ply_file->elements[j].s_stride                += 1;
                            ply_file->elements[j].properties[k].type_size =  1;
                            break;
                        case GXPLY_short:
                        case GXPLY_ushort:
                            ply_file->elements[j].s_stride                += 2;
                            ply_file->elements[j].properties[k].type_size =  2;
                            break;
                        case GXPLY_int:
                        case GXPLY_uint:
                        case GXPLY_float:
                            ply_file->elements[j].s_stride                += 4;
                            ply_file->elements[j].properties[k].type_size =  4;
                            break;
                        case GXPLY_list:
                            goto p3propertyExit;
                            break;
                        case GXPLY_double:
                            goto noDoubleSupport;
                            break;
                        default:
                            goto unrecognizedPropertyType;
                            break;

                        }

                        c_data                 = strchr(c_data, ' ') + 1;
                        size_t propertyLength = strchr(c_data, '\n') - c_data;

                        ply_file->elements[j].properties[k].name = calloc(propertyLength + 1, sizeof(u8));
                        strncpy(ply_file->elements[j].properties[k].name, c_data, propertyLength);

                        // Zero set the index
                        i = 0;
                        while (c_data[++i] != '\n'); // Skip to the end of the line
                        c_data = &c_data[i + 1];      // Set the pointer
                        k++;
                    }
                    goto p3propertyExit;

                }

                // Zero set the index
                i = 0;
                while (c_data[++i] != '\n'); // Skip to the end of the line
                c_data = &c_data[i + 1];      // Set the pointer
                continue;

                p3propertyExit:j++;
            }
        }

        // Pass 4
        {
            // Build faces
            GXPLYindex_t *indices =  (GXPLYindex_t *)(c_data+11+ply_file->elements[0].n_count * ply_file->elements[0].s_stride);
            corrected_indicies = calloc(12, ply_file->elements[1].n_count);

            for (size_t i = 0; i < ply_file->elements[1].n_count; i++)
                corrected_indicies[i*3]     = indices[i].a,
                corrected_indicies[(i*3)+1] = indices[i].b,
                corrected_indicies[(i*3)+2] = indices[i].c;
        }
    }

    // Create flags
    {

        // Initialized data
        int tflags = 0;

        // Determine what properties are in the file
        {
            for (int a = 0; a < 1; a++)
                for (int b = 0; b < ply_file->elements[a].n_properties; b++)
                    if (*ply_file->elements[a].properties[b].name == 'x')
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags |= (GXPLY_Geometric);
                        tflags |= GXPLY_X;
                        vertex_group_count++;
                    }
                    else if (*ply_file->elements[a].properties[b].name == 'y')
                        tflags |= GXPLY_Y;
                    else if (*ply_file->elements[a].properties[b].name == 'z')
                        tflags |= GXPLY_Z;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "tx", 2) == 0)
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags |= (GXPLY_Tangent);
                        tflags |= GXPLY_TX;
                        vertex_group_count++;
                    }
                    else if (strncmp(ply_file->elements[a].properties[b].name, "ty", 2) == 0)
                        tflags |= GXPLY_TY;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "tz", 2) == 0)
                        tflags |= GXPLY_TZ;
                    else if (*ply_file->elements[a].properties[b].name == 's')
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags |= (GXPLY_Texture);
                        tflags |= GXPLY_S;
                        vertex_group_count++;
                    }
                    else if (*ply_file->elements[a].properties[b].name == 't')
                        tflags |= GXPLY_T;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "nx", 2) == 0)
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags |= (GXPLY_Normal);
                        tflags |= GXPLY_NX;
                        vertex_group_count++;
                    }
                    else if (strncmp(ply_file->elements[a].properties[b].name, "ny", 2) == 0)
                        tflags |= GXPLY_NY;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "nz", 2) == 0)
                        tflags |= GXPLY_NZ;

                    else if (strncmp(ply_file->elements[a].properties[b].name, "bx", 2) == 0)
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags |= (GXPLY_Bitangent);
                        tflags |= (GXPLY_BX);
                        vertex_group_count++;
                    }
                    else if (strncmp(ply_file->elements[a].properties[b].name, "by", 2) == 0)
                        tflags |= GXPLY_BY;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "bz", 2) == 0)
                        tflags |= GXPLY_BZ;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "red", 3) == 0)
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags  |= (GXPLY_Color);
                        tflags          |= GXPLY_R;
                        vertex_group_count++;
                    }
                    else if (strncmp(ply_file->elements[a].properties[b].name, "green", 5) == 0)
                        tflags |= GXPLY_G;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "blue", 4) == 0)
                        tflags |= GXPLY_B;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "alpha", 5) == 0)
                        tflags |= GXPLY_A;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "b0", 2) == 0)
                    {
                        if (tflags & GXPLY_B1 || tflags & GXPLY_B2 || tflags & GXPLY_B3)
                            goto irregularVertices;
                        ply_file->flags <<= 8;
                        ply_file->flags  |= (GXPLY_Bones);
                        tflags          |= GXPLY_B0;
                        vertex_group_count++;
                    }
                    else if (strncmp(ply_file->elements[a].properties[b].name, "b1", 2) == 0)
                        tflags |= GXPLY_B1;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "b2", 2) == 0)
                        tflags |= GXPLY_B2;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "b3", 2) == 0)
                        tflags |= GXPLY_B3;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "w0", 2) == 0)
                    {
                        ply_file->flags <<= 8;
                        ply_file->flags  |= (GXPLY_Weights);
                        tflags          |= GXPLY_BW1;
                        vertex_group_count++;
                    }
                    else if (strncmp(ply_file->elements[a].properties[b].name, "w1", 2) == 0)
                        tflags |= GXPLY_BW1;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "w2", 2) == 0)
                        tflags |= GXPLY_BW2;
                    else if (strncmp(ply_file->elements[a].properties[b].name, "w3", 2) == 0)
                        tflags |= GXPLY_BW3;
        }

        // Check the integrity of the mesh
        #ifndef NDEBUG
        {
            if (ply_file->flags & GXPLY_Geometric &&
                !( tflags & GXPLY_X &&
                   tflags & GXPLY_Y &&
                   tflags & GXPLY_Z ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Texture &&
                !( tflags & GXPLY_S &&
                   tflags & GXPLY_T ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Normal &&
                !( tflags & GXPLY_NX &&
                   tflags & GXPLY_NY &&
                   tflags & GXPLY_NZ ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Tangent &&
                !(tflags & GXPLY_TX &&
                  tflags & GXPLY_TY &&
                  tflags & GXPLY_TZ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Bitangent &&
                  !(tflags & GXPLY_BX &&
                    tflags & GXPLY_BY &&
                    tflags & GXPLY_BZ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Color &&
                !( tflags & GXPLY_R &&
                   tflags & GXPLY_G &&
                   tflags & GXPLY_B ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Bones &&
                !( tflags & GXPLY_B0 &&
                   tflags & GXPLY_B1 &&
                   tflags & GXPLY_B2 &&
                   tflags & GXPLY_B3 ))
                goto missingVerts;

            if (ply_file->flags & GXPLY_Weights &&
                !( tflags & GXPLY_BW0 &&
                   tflags & GXPLY_BW1 &&
                   tflags & GXPLY_BW2 &&
                   tflags & GXPLY_BW3 ))
                goto missingVerts;
        }
        #endif
        goto processVAO;
        missingVerts:
            g_print_log("[G10] [PLY] Missing vertex attributes detected in \"%s\"\n",path);
            goto processVAO;
        irregularVertices:
            g_print_log("[G10] [PLY] Detected irregular vertex attribute grouping in \"%s\"\n", path);
            goto processVAO;
    }
    processVAO:
    c_data = c_data+11;

    vertex_array = (float*)c_data;
    vertices_in_buffer = ply_file->elements[0].n_count;
    indices = (GXPLYindex_t *)corrected_indicies;
    indices_in_buffer = ply_file->elements[1].n_count;
    part->index_count = indices_in_buffer;

    // Populate the part
    /*{

        part->vertex_count = vertices_in_buffer;
        extern u32 find_memory_type(u32 type_filter, VkMemoryPropertyFlags properties);


        VkMemoryRequirements memory_requirements = { 0 };
        VkBufferCreateInfo   buffer_create_info  = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = ply_file->elements[0].s_stride * vertices_in_buffer,
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };
        VkMemoryAllocateInfo allocate_info       = { 0 };
        void *data = vertex_array;

        if ( vkCreateBuffer(p_instance->vulkan.device, &buffer_create_info, 0, &part->vertex_buffer) != VK_SUCCESS )
            g_print_error("[G10] [PLY] Failed to create vertex buffer");

        vkGetBufferMemoryRequirements(p_instance->vulkan.device, part->vertex_buffer, &memory_requirements);

        allocate_info = (VkMemoryAllocateInfo)
        {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memory_requirements.size,
            .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        };

        if (vkAllocateMemory(p_instance->vulkan.device, &allocate_info, 0, &part->vertex_buffer_memory))
            g_print_error("[G10] [PLY] Failed to allocate vertex buffer memory");

        vkBindBufferMemory(p_instance->vulkan.device, part->vertex_buffer, part->vertex_buffer_memory, 0);

        vkMapMemory(p_instance->vulkan.device, part->vertex_buffer_memory, 0, buffer_create_info.size, 0, &data);

        // TODO: Replace w staging
        memcpy(data, vertex_array, buffer_create_info.size);

        vkUnmapMemory(p_instance->vulkan.device, part->vertex_buffer_memory);
    }

    {

        part->vertex_count = vertices_in_buffer;
        extern u32 find_memory_type(u32 type_filter, VkMemoryPropertyFlags properties);


        VkMemoryRequirements* memory_requirements = calloc(1, sizeof(VkMemoryRequirements));
        VkBufferCreateInfo* buffer_create_info = calloc(1, sizeof(VkBufferCreateInfo));
        VkMemoryAllocateInfo* allocate_info = calloc(1, sizeof(VkMemoryAllocateInfo));
        void* data = corrected_indicies;

        {
            buffer_create_info->sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_create_info->size = 12 * indices_in_buffer;
            buffer_create_info->usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            buffer_create_info->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        if (vkCreateBuffer(p_instance->vulkan.device, buffer_create_info, 0, &part->element_buffer) != VK_SUCCESS)
            g_print_error("[G10] [PLY] Failed to create element buffer");

        vkGetBufferMemoryRequirements(p_instance->vulkan.device, part->element_buffer, memory_requirements);

        allocate_info->sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocate_info->allocationSize = memory_requirements->size;
        allocate_info->memoryTypeIndex = find_memory_type(memory_requirements->memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


        if (vkAllocateMemory(p_instance->vulkan.device, allocate_info, 0, &part->element_buffer_memory))
            g_print_error("[G10] [PLY] Failed to allocate element buffer memory");

        vkBindBufferMemory(p_instance->vulkan.device, part->element_buffer, part->element_buffer_memory, 0);

        vkMapMemory(p_instance->vulkan.device, part->element_buffer_memory, 0, buffer_create_info->size, 0, &data);

        // TODO: Replace w staging
        memcpy(data, corrected_indicies, buffer_create_info->size);

        vkUnmapMemory(p_instance->vulkan.device, part->element_buffer_memory);

        free(memory_requirements);
        free(buffer_create_info);
        free(allocate_info);

    }*/


    // Destroy the PLY File
    //{
    //    // Depopulate all elements
    //    for (i = 0; i < ply_file->n_elements; i++)
    //    {
    //        // Depopulate all properties of an element
    //        for (j = 0; j < ply_file->elements[i].n_properties; j++)
    //        {
    //           // Free the name of the property
    //           free(ply_file->elements[i].properties[j].name);
    //           // Zero set the type_size
    //           ply_file->elements[i].properties[j].type_size = 0;
    //        }
    //        // Free the properties
    //        free(ply_file->elements[i].properties);
    //
    //        // Free the name of the element
    //        //free(ply_file->elements[i].name);
    //        // Zero set all the primatives
    //        ply_file->elements[i].n_count = 0;
    //        ply_file->elements[i].n_properties = 0;
    //        ply_file->elements[i].s_stride = 0;
    //    }
    //    // Free the elements
    //    free(ply_file->elements);
    //    // Zero set all the primatives
    //    ply_file->flags = 0;
    //    ply_file->format = 0;
    //    ply_file->n_elements = 0;
    //    // Free the ply_file
    //    free(ply_file);
    //}

    //free(corrected_indicies);
    //free(data);

    return part;

    // Error handling
    {
        failed_to_load_file:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Failed to load file %s\n", path);
            #endif

            // Error
            return 0;

        no_part:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Null pointer provided for parameter \"part\" in call to %s\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        no_path:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Null pointer provided for parameter \"path\" in call to %s\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        invalidHeader:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Invalid header detected in file \"%s\"\n",path);
            #endif

            // Error
            return 0;

        noDoubleSupport:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Double detected in element \"%s\" in file \"%s\"\n", ply_file->elements[j].name, path);
            #endif

            // Error
            return 0;

        unrecognizedPropertyType:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Unrecognized property type detected in file \"%s\"\n", path);
            #endif

            // Error
            return 0;

        //nonTriangulated:
            #ifndef NDEBUG
                g_print_error("[G10] [PLY] Detected non triangulated faces in file \"%s\"\n", path);
            #endif

            // Error
            return 0;
    }
}
