#pragma once

#include <SDL3/SDL.h>
#include <cglm/struct/vec4.h>

#include "attrib_generators.h"
#include "common.h"

typedef struct vertex_attribs {
    u32 nattribs, nlocations;
    SDL_GPUVertexAttribute* attribs;
} vertex_attribs_t;

#define COLOR_VERTEX                                                                               \
    VERTEX_BEGIN(colorvert)                                                                        \
    VERTEX_ATTRIB(pos, vec3s)                                                                      \
    VERTEX_ATTRIB(uv, vec2s)                                                                       \
    VERTEX_ATTRIB(norm, vec3s)                                                                     \
    VERTEX_END(colorvert)
#include "create_vertex_struct.h"
COLOR_VERTEX
#include "create_vertex_info.h"
COLOR_VERTEX
// vertex_attribs_t get_colorvert_attribs(u32 bufslot, u32 initial_location)

SDL_GPUBuffer* create_vertex_buffer(SDL_GPUDevice* dev, usize sz);
bool update_vertex_buffer_once(
    SDL_GPUDevice* dev, SDL_GPUBuffer* vbo, void* data, usize sz, usize offs);
