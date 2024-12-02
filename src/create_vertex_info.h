// vertex_attribs_t get_##name##_attribs(u32 bufslot, u32 initial_location)

#undef VERTEX_BEGIN
#undef VERTEX_ATTRIB
#undef VERTEX_END
#define VERTEX_BEGIN(name)                                                                         \
    static inline SDL_GPUVertexBufferDescription get_##name##_desc(u32 bufslot, u32 step_rate)     \
    {                                                                                              \
        return (SDL_GPUVertexBufferDescription) {                                                  \
            .slot = bufslot,                                                                       \
            .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,                                          \
            .instance_step_rate = step_rate,                                                       \
            .pitch = sizeof(name##_t),                                                             \
        };                                                                                         \
    }                                                                                              \
    static inline vertex_attribs_t get_##name##_attribs(u32 bufslot, u32 initial_location)         \
    {                                                                                              \
        static bool inited = false;                                                                \
        static SDL_GPUVertexAttribute _attribs[32];                                                \
        static vertex_attribs_t attribs;                                                           \
        if (inited)                                                                                \
            return attribs;                                                                        \
                                                                                                   \
        {                                                                                          \
            u32 offs = 0;                                                                          \
            u32 loc = initial_location;                                                            \
            u32 nattribs = 0;
#define VERTEX_ATTRIB(_name, _type)                                                                \
    {                                                                                              \
        _vertex_attribs_t atr = get_vertex_attrib_##_type(bufslot, &loc, &offs);                   \
        for (u32 i = 0; i < atr.nattribs; i++) {                                                   \
            _attribs[nattribs++] = atr.attribs[i];                                                 \
        }                                                                                          \
    }
#define VERTEX_END(name)                                                                           \
    attribs = (vertex_attribs_t) {                                                                 \
        .nattribs = nattribs,                                                                      \
        .nlocations = loc - initial_location,                                                      \
        .attribs = _attribs,                                                                       \
    };                                                                                             \
    inited = true;                                                                                 \
    return attribs;                                                                                \
    }                                                                                              \
    }
