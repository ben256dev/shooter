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

bool update_vertex_buffer_once(SDL_GPUDevice* dev, SDL_GPUBuffer* vbo, void* data, usize sz)
{
    return true;
    // SDL_GPUTransferBuffer *transfer = SDL_CreateGPUTransferBuffer(dev,
    // &(SDL_GPUTransferBufferCreateInfo){
    //      .
    // });

    // transfer_buffer_desc.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    // transfer_buffer_desc.size = sizeof(vertex_data);
    // transfer_buffer_desc.props = 0;
    // buf_transfer = SDL_CreateGPUTransferBuffer(
    //     gpu_device,
    //     &transfer_buffer_desc
    //);
    // CHECK_CREATE(buf_transfer, "Vertex transfer buffer")

    ///* We just need to upload the static data once. */
    // map = SDL_MapGPUTransferBuffer(gpu_device, buf_transfer, false);
    // SDL_memcpy(map, vertex_data, sizeof(vertex_data));
    // SDL_UnmapGPUTransferBuffer(gpu_device, buf_transfer);

    // cmd = SDL_AcquireGPUCommandBuffer(gpu_device);
    // copy_pass = SDL_BeginGPUCopyPass(cmd);
    // buf_location.transfer_buffer = buf_transfer;
    // buf_location.offset = 0;
    // dst_region.buffer = render_state.buf_vertex;
    // dst_region.offset = 0;
    // dst_region.size = sizeof(vertex_data);
    // SDL_UploadToGPUBuffer(copy_pass, &buf_location, &dst_region, false);
    // SDL_EndGPUCopyPass(copy_pass);
    // SDL_SubmitGPUCommandBuffer(cmd);

    // SDL_ReleaseGPUTransferBuffer(gpu_device, buf_transfer);
}
