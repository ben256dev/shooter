#pragma once

#include <SDL3/SDL.h>

#include "attrib_generators.h"
#include "common.h"
#include "linear.h"

typedef struct vertex_attribs {
    u32 nattribs, nlocations;
    SDL_GPUVertexAttribute* attribs;
} vertex_attribs_t;

#define VERTEX_FORMAT_POS_UV_NORM                                                                  \
    VERTEX_BEGIN(vertex_format_pos_uv_norm)                                                        \
    VERTEX_ATTRIB(pos, vec3)                                                                       \
    VERTEX_ATTRIB(uv, vec2)                                                                        \
    VERTEX_ATTRIB(norm, vec3)                                                                      \
    VERTEX_END(vertex_format_pos_uv_norm)
#include "create_vertex_struct.h"
VERTEX_FORMAT_POS_UV_NORM
#include "create_vertex_info.h"
VERTEX_FORMAT_POS_UV_NORM

SDL_GPUBuffer* create_vertex_buffer(SDL_GPUDevice* dev, usize sz);
SDL_GPUBuffer* create_index_buffer(SDL_GPUDevice* dev, usize sz);
bool update_vertex_buffer_once(
    SDL_GPUDevice* dev, SDL_GPUBuffer* vbo, void* data, usize sz, usize offs);

/**
 * @brief Mesh struct
 *
 * Contains a indexed mesh
 */
typedef struct mesh {
    struct {
        SDL_GPUBuffer* buf;
        usize sz; /** size of verticies (in bytes) */
    } verts;
    struct {
        SDL_GPUBuffer* buf;
        usize len; /** number of indicies */
    } idxs;
} mesh_t;

bool mesh_init_from_data(
    mesh_t* self, SDL_GPUDevice* dev, void* verts, usize sz, u32* idxs, usize len);
bool mesh_init_from_ply(mesh_t* self, SDL_GPUDevice* dev, const char* ply_path);
void mesh_deinit(mesh_t* self, SDL_GPUDevice* dev);
void mesh_bind(const mesh_t* self, SDL_GPURenderPass* pass);
SDL_GPUVertexBufferDescription mesh_buffer_desc(const mesh_t* self, bool instanced);
vertex_attribs_t mesh_vertex_attribs(const mesh_t* self, u32 initial_bind_location);
