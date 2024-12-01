#include "render_state.h"

// Returns false if the render can still render the game but something is fishy
bool render_state_init(render_state_t* self)
{
    *self = (render_state_t) {
        .shader_format = SDL_GPU_SHADERFORMAT_SPIRV,
    };

    if (!(self->win = SDL_CreateWindow("shooter", 800, 600, SDL_WINDOW_RESIZABLE))) {
        sdldie("SDL_CreateWindow");
    }
    if (!(self->gpu = SDL_CreateGPUDevice(self->shader_format, ISDBG, NULL))) {
        sdldie("SDL_CreateGPUDevice");
    }
    if (!SDL_ClaimWindowForGPUDevice(self->gpu, self->win)) {
        sdldie("SDL_ClaimWindowForGPUDevice");
    }
    if (!(self->cmdbuf = SDL_AcquireGPUCommandBuffer(self->gpu))) {
        sdldie("SDL_AcquireGPUCommandBuffer");
    }
    if (!SDL_AcquireGPUSwapchainTexture(
            self->cmdbuf, self->win, &self->swap.tex, &self->swap.w, &self->swap.h)) {
        sdldie("SDL_AcquireGPUSwapchainTexture");
    }
    if (!SDL_SetGPUSwapchainParameters(self->gpu, self->win,
            SDL_GPU_SWAPCHAINCOMPOSITION_SDR_LINEAR, SDL_GPU_PRESENTMODE_VSYNC)) {
        sdldie("SDL_SetGPUSwapchainParameters");
    }

    self->ctargs[0] = (SDL_GPUColorTargetInfo){
        .texture = self->swap.tex,
        .clear_color = {
            .r = 0.2f, .g = 0.3f, .b = 0.3f, .a = 1.0f,
        },
        .load_op = SDL_GPU_LOADOP_CLEAR,
        .store_op = SDL_GPU_STOREOP_STORE,
        .cycle = true,
    };
    self->dstarg = (SDL_GPUDepthStencilTargetInfo) { 0 };

    return true;
}

void render_state_deinit(render_state_t* self)
{
    SDL_DestroyGPUDevice(self->gpu);
    SDL_DestroyWindow(self->win);
    *self = (render_state_t) { 0 };
}

void render_state_render(render_state_t* self)
{
    if (!SDL_BeginGPURenderPass(self->cmdbuf, self->ctargs, arrlen(self->ctargs), &self->dstarg)) {
        sdldie("SDL_BeginGPURenderPass");
    }
}

SDL_GPUShader* render_state_load_shader(
    render_state_t* self, const char* spriv_path, SDL_GPUShaderStage stage)
{
    usize byteslen;
    u8* bytes = load_file_bytes(spriv_path, &byteslen);
    if (!bytes) {
        warn("load_shader: can't open file %s\n", spriv_path);
        return NULL;
    }

    SDL_GPUShader* shader = SDL_CreateGPUShader(self->gpu,
        &(SDL_GPUShaderCreateInfo) {
            .code_size = byteslen,
            .code = bytes,
            .entrypoint = "main",
            .format = self->shader_format,
            .stage = SDL_GPU_SHADERSTAGE_VERTEX,
            .num_samplers = 0,
            .num_storage_textures = 0,
            .num_storage_buffers = 0,
            .num_uniform_buffers = 0,
        });

    if (!shader) {
        warn("Could not load shader %s\n", spriv_path);
        return NULL;
    }

    return shader;
}
