#include "mesh.h"
#include "ply.h"

SDL_GPUBuffer* create_vertex_buffer(SDL_GPUDevice* dev, usize sz)
{
    SDL_GPUBuffer* buffer = SDL_CreateGPUBuffer(dev,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = sz,
            .props = 0,
        });
    if (!buffer) {
        warn("create_vertex_buffer");
    }
    return buffer;
}
SDL_GPUBuffer* create_index_buffer(SDL_GPUDevice* dev, usize sz)
{
    SDL_GPUBuffer* buffer = SDL_CreateGPUBuffer(dev,
        &(SDL_GPUBufferCreateInfo) {
            .usage = SDL_GPU_BUFFERUSAGE_INDEX,
            .size = sz,
            .props = 0,
        });
    if (!buffer) {
        warn("create_index_buffer");
    }
    return buffer;
}

bool update_vertex_buffer_once(
    SDL_GPUDevice* dev, SDL_GPUBuffer* vbo, void* data, usize sz, usize offs)
{
    SDL_GPUTransferBuffer* transfer = SDL_CreateGPUTransferBuffer(dev,
        &(SDL_GPUTransferBufferCreateInfo) {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = sz,
            .props = 0,
        });
    if (!transfer) {
        warn("SDL_GPUTransferBufferCreateInfo");
        return false;
    }

    void* map = SDL_MapGPUTransferBuffer(dev, transfer, false);
    memcpy(map, data, sz);
    SDL_UnmapGPUTransferBuffer(dev, transfer);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(dev);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);
    SDL_UploadToGPUBuffer(copy_pass,
        &(SDL_GPUTransferBufferLocation) {
            .transfer_buffer = transfer,
            .offset = 0,
        },
        &(SDL_GPUBufferRegion) {
            .buffer = vbo,
            .offset = offs,
            .size = sz,
        },
        false);
    SDL_EndGPUCopyPass(copy_pass);
    SDL_SubmitGPUCommandBuffer(cmd);
    SDL_ReleaseGPUTransferBuffer(dev, transfer);
    return true;
}

bool mesh_init_from_data(
    mesh_t* self, SDL_GPUDevice* dev, void* verts, usize sz, u32* idxs, usize len)
{
    *self = (mesh_t){
        .verts = {
            .buf = create_vertex_buffer(dev, sz),
            .sz = sz,
        },
        .idxs = {
            .buf = create_index_buffer(dev, len * sizeof(u32)),
            .len = len,
        }
    };

    return self->verts.buf && self->idxs.buf
        && update_vertex_buffer_once(dev, self->verts.buf, verts, sz, 0)
        && update_vertex_buffer_once(dev, self->idxs.buf, idxs, len * sizeof(*idxs), 0);
}
bool mesh_init_from_ply(mesh_t* self, SDL_GPUDevice* dev, const char* ply_path)
{
    ply_file* file;

    if (load_ply_file(&file, ply_path) != 1) {
        warn("Cant not load mesh: %s", ply_path);
        return false;
    }

    info_ply_file(file);

    // Get vertex properties
    struct {
        bool has;
        u32 idx[3];
    } pos = { 0 }, uv = { 0 }, norm = { 0 };

    u64 attribs = file->_vertex_attributes;
    for (u32 i = 0; i < 8; i++) {
        enum ply_vertex_properties_e prop = attribs >> (i * 4) & 0xF;
        if (!prop)
            continue;

        switch (prop) {
        case PLY_X:
            pos.has = true;
            pos.idx[0] = i;
            break;
        case PLY_Y:
            pos.has = true;
            pos.idx[1] = i;
            break;
        case PLY_Z:
            pos.has = true;
            pos.idx[2] = i;
            break;
        case PLY_U:
            uv.has = true;
            uv.idx[0] = i;
            break;
        case PLY_V:
            uv.has = true;
            uv.idx[1] = i;
            break;
        case PLY_NX:
            norm.has = true;
            norm.idx[0] = i;
            break;
        case PLY_NY:
            norm.has = true;
            norm.idx[1] = i;
            break;
        case PLY_NZ:
            norm.has = true;
            norm.idx[2] = i;
            break;
        }
    }

    if (!pos.has || !uv.has || !norm.has) {
        warn("mesh: %s!!!!! GIVE POS UV AND NORM ASS SHIT", ply_path);
        return false;
    }

    // Malloc memory for verticies
    vertex_format_pos_uv_norm_t* verts;

    self->verts.sz = file->verticies.quantity * sizeof(*verts);
    verts = xmalloc(self->verts.sz);
    for (usize i = 0; i < file->verticies.quantity; i++) {
        verts[i].pos.x = file->verticies._data[i * 8 + pos.idx[0]];
        verts[i].pos.y = file->verticies._data[i * 8 + pos.idx[1]];
        verts[i].pos.z = file->verticies._data[i * 8 + pos.idx[2]];

        verts[i].uv.x = file->verticies._data[i * 8 + uv.idx[0]];
        verts[i].uv.y = file->verticies._data[i * 8 + uv.idx[1]];

        verts[i].norm.x = file->verticies._data[i * 8 + norm.idx[0]];
        verts[i].norm.y = file->verticies._data[i * 8 + norm.idx[1]];
        verts[i].norm.z = file->verticies._data[i * 8 + norm.idx[2]];
    }

    // Malloc memory and read in elements
    self->idxs.len = file->elements.quantity * 3;
    u32* idxs = xmalloc(self->idxs.len * sizeof(u32));
    u8* _data = (u8*)file->elements._data;
    for (usize tri = 0; tri < file->elements.quantity; tri++) {
        memcpy(idxs + tri * 3, _data + tri * 13 + 1, 12);
    }

    bool ret = mesh_init_from_data(self, dev, verts, self->verts.sz, idxs, self->idxs.len);

    free(verts);
    free(idxs);
    return ret;
}
void mesh_deinit(mesh_t* self, SDL_GPUDevice* dev)
{
    SDL_ReleaseGPUBuffer(dev, self->verts.buf);
    SDL_ReleaseGPUBuffer(dev, self->idxs.buf);
}
void mesh_bind(const mesh_t* self, SDL_GPURenderPass* pass)
{
    SDL_BindGPUVertexBuffers(pass, 0,
        (SDL_GPUBufferBinding[]) {
            (SDL_GPUBufferBinding) {
                .buffer = self->verts.buf,
                .offset = 0,
            },
        },
        1);
    SDL_BindGPUIndexBuffer(pass,
        &(SDL_GPUBufferBinding) {
            .buffer = self->idxs.buf,
            .offset = 0,
        },
        SDL_GPU_INDEXELEMENTSIZE_32BIT);
}
SDL_GPUVertexBufferDescription mesh_buffer_desc(const mesh_t* self, bool instanced)
{
    return get_vertex_format_pos_uv_norm_desc(0, instanced);
}
vertex_attribs_t mesh_vertex_attribs(const mesh_t* self, u32 initial_bind_location)
{
    return get_vertex_format_pos_uv_norm_attribs(0, initial_bind_location);
}
