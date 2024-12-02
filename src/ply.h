#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

enum ply_vertex_properties_e 
{
    PLY_X  = 1,
    PLY_Y  = 2,
    PLY_Z  = 3,

    PLY_U  = 4,
    PLY_V  = 5,

    PLY_NX = 6,
    PLY_NY = 7,
    PLY_NZ = 8
};

typedef struct 
{
    size_t len;
    size_t _attribute_quantity;
    unsigned long long _vertex_attributes;
    struct
    {
        const size_t  quantity;
        const float  *_data;
    } verticies;
    struct
    { 
        const size_t        quantity;
        const unsigned int *_data;
    } elements;
    char _file[];
} ply_file;

// Load a ply file format
/** !
 * Load a PLY file
 * 
 * @param pp_ply_file result
 * @param p_path      path to file
 * 
 * @return 1 on success, 0 on error
 */
int load_ply_file ( ply_file **pp_ply_file, const char *p_path );

// Info
/** ! 
 * Get info about a PLY file
 * 
 * @param p_ply_file pointer to ply file
 * 
 * @return 1 on success, 0 on error
 */
int info_ply_file ( ply_file *p_ply_file );
