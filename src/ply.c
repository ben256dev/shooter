#include "ply.h"

const char *_p_vertex_attribute_names[] = 
{
    [0]      = "(null)",
    [PLY_X]  = "X",
    [PLY_Y]  = "Y",
    [PLY_Z]  = "Z",
    [PLY_U]  = "U",
    [PLY_V]  = "V",
    [PLY_NX] = "NX",
    [PLY_NY] = "NY",
    [PLY_NZ] = "NZ"
};

int load_ply_file ( ply_file **pp_ply_file, const char *p_path )
{
    
    FILE *p_file = fopen(p_path, "rb");
    size_t file_size = 0;
    ply_file *p_ply_file = 0;
    size_t i = 0;

    fseek(p_file, 0, SEEK_END);
    file_size = (size_t) ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    p_ply_file = realloc(0, sizeof(ply_file) + file_size);
    
    memset(p_ply_file, 0, sizeof(ply_file) + file_size);
    
    fread(&p_ply_file->_file, 1, file_size, p_file);

    fclose(p_file);

    if ( 0 != strncmp(&p_ply_file->_file, "ply", 3) ) goto invalid_sig;

    for (i = 0; i < file_size; i++)
    {
        
        if ( 0 == strncmp(&p_ply_file->_file[i], "end_header", 10) )
        {
            while (p_ply_file->_file[i] != '\n') i++;
            i++;
            break;
        }
        else if ( 1 == sscanf(&p_ply_file->_file[i], "element vertex %zu", &p_ply_file->verticies.quantity) )
        {
            while (p_ply_file->_file[i] != '\n') i++;
            i++;
            
            char _buf[8] = { 0 };
            int j = 0;

            while ( 1 == sscanf(&p_ply_file->_file[i], "property float %[^\n]8", &_buf) )
            {
                if      ( 0 == strcmp(_buf, "x") )  p_ply_file->_vertex_attributes |= PLY_X  << (j * 4);
                else if ( 0 == strcmp(_buf, "y") )  p_ply_file->_vertex_attributes |= PLY_Y  << (j * 4);
                else if ( 0 == strcmp(_buf, "z") )  p_ply_file->_vertex_attributes |= PLY_Z  << (j * 4);
                else if ( 0 == strcmp(_buf, "nx") ) p_ply_file->_vertex_attributes |= PLY_NX << (j * 4);
                else if ( 0 == strcmp(_buf, "ny") ) p_ply_file->_vertex_attributes |= PLY_NY << (j * 4);
                else if ( 0 == strcmp(_buf, "nz") ) p_ply_file->_vertex_attributes |= PLY_NZ << (j * 4);
                else if ( 0 == strcmp(_buf, "s") )  p_ply_file->_vertex_attributes |= PLY_U  << (j * 4);
                else if ( 0 == strcmp(_buf, "t") )  p_ply_file->_vertex_attributes |= PLY_V  << (j * 4);
                else goto unknown_vertex_attribute;
                
                j++;
                
                while (p_ply_file->_file[i] != '\n') i++;
                i++;  
            }

            i--;
            p_ply_file->_attribute_quantity = j;
        }
        else if ( 1 == sscanf(&p_ply_file->_file[i], "element face %zu", &p_ply_file->elements.quantity) );
        else
            while (p_ply_file->_file[i] != '\n') i++;
    }
    
    end_of_ply_file:

    p_ply_file->verticies._data = &p_ply_file->_file[i];
    p_ply_file->elements._data = &p_ply_file->_file[ i + (p_ply_file->verticies.quantity * sizeof(float) * p_ply_file->_attribute_quantity) ];

    *pp_ply_file = p_ply_file;

    // Success
    return 1;

    invalid_sig:
    unknown_vertex_attribute:

        // Error
        return 0;
}

int info_ply_file ( ply_file *p_ply_file )
{
    
    printf(" === PLY file @ %p === \n", p_ply_file);
    printf(" - vertex [%d]\n", p_ply_file->verticies.quantity);
    printf("    - attribute #: %zu\n", p_ply_file->_attribute_quantity);
    printf("    - attribute word: 0x%x\n", p_ply_file->_vertex_attributes);
    printf(" - faces  [%d]\n\n", p_ply_file->elements.quantity);
    
    // Success
    return 1;
}