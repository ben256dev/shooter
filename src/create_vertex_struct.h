#undef VERTEX_BEGIN
#undef VERTEX_ATTRIB
#undef VERTEX_END
#define VERTEX_BEGIN(name) typedef struct name {
#define VERTEX_ATTRIB(name, type) type name;
#define VERTEX_END(name)                                                                           \
    }                                                                                              \
    name_t;
