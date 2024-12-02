#pragma once

#include "common.h"
#include <SDL3/SDL.h>

#define _ATTRIB_GENERATORS                                                                         \
    _ATTRIB_GENERATOR(float, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT, 1)                                 \
    _ATTRIB_GENERATOR(vec2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, 1)                                 \
    _ATTRIB_GENERATOR(vec3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 1)                                 \
    _ATTRIB_GENERATOR(vec4, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, 1)                                 \
    _ATTRIB_GENERATOR(int, SDL_GPU_VERTEXELEMENTFORMAT_INT, 1)                                     \
    _ATTRIB_GENERATOR(unsigned, SDL_GPU_VERTEXELEMENTFORMAT_UINT, 1)                               \
    _ATTRIB_GENERATOR(ivec2, SDL_GPU_VERTEXELEMENTFORMAT_INT2, 1)                                  \
    _ATTRIB_GENERATOR(ivec3, SDL_GPU_VERTEXELEMENTFORMAT_INT3, 1)                                  \
    _ATTRIB_GENERATOR(ivec4, SDL_GPU_VERTEXELEMENTFORMAT_INT4, 1)                                  \
    _ATTRIB_GENERATOR(mat4, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, 4);

typedef struct _vertex_attribs {
    u32 nattribs;
    SDL_GPUVertexAttribute attribs[4];
} _vertex_attribs_t;

#define _ATTRIB_GENERATOR(_type, _format, _loclen)                                                 \
    static inline _vertex_attribs_t get_vertex_attrib_##_type(u32 bufslot, u32* loc, u32* offs)    \
    {                                                                                              \
        if (_loclen == 1) {                                                                        \
            const _vertex_attribs_t attribs = { \
                .attribs = { \
                    (SDL_GPUVertexAttribute) { \
                        .buffer_slot = bufslot, \
                        .location = *loc, \
                        .offset = *offs, \
                        .format = _format, \
                    }, \
                }, \
                .nattribs = _loclen, \
            };                                                  \
            *loc += _loclen;                                                                       \
            *offs += sizeof(_type);                                                                \
            return attribs;                                                                        \
        } else if (_loclen == 4) {                                                                 \
            const _vertex_attribs_t attribs = { \
                .attribs = { \
                    (SDL_GPUVertexAttribute) { \
                        .buffer_slot = bufslot, \
                        .location = *loc, \
                        .offset = *offs, \
                        .format = _format, \
                    }, \
                    (SDL_GPUVertexAttribute) { \
                        .buffer_slot = bufslot, \
                        .location = *loc + 1, \
                        .offset = *offs + sizeof(_type) / 4 * 1, \
                        .format = _format, \
                    }, \
                    (SDL_GPUVertexAttribute) { \
                        .buffer_slot = bufslot, \
                        .location = *loc + 2, \
                        .offset = *offs + sizeof(_type) / 4 * 2, \
                        .format = _format, \
                    }, \
                    (SDL_GPUVertexAttribute) { \
                        .buffer_slot = bufslot, \
                        .location = *loc + 3, \
                        .offset = *offs + sizeof(_type) / 4 * 3, \
                        .format = _format, \
                    }, \
                }, \
                .nattribs = _loclen, \
            };                                                  \
            *loc += _loclen;                                                                       \
            *offs += sizeof(_type);                                                                \
            return attribs;                                                                        \
        } else {                                                                                   \
            return (_vertex_attribs_t) { 0 };                                                      \
        }                                                                                          \
    }
_ATTRIB_GENERATORS
#undef _ATTRIB_GENERATOR
