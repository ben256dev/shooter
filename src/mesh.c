#include "mesh.h"

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
