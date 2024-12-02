#undef VERTEX_BEGIN
#undef VERTEX_ATTRIB
#undef VERTEX_END
#define VERTEX_BEGIN(_name) typedef struct _name {
#define VERTEX_ATTRIB(_name, _type) _type _name;
#define VERTEX_END(_name)                                                                          \
    }                                                                                              \
    _name##_t;
