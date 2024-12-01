#include "render_state.h"

// Returns false if the render can still render the game but something is fishy
bool render_state_init(render_state_t* self)
{
    if (!(self->win = SDL_CreateWindow("shooter", 800, 600, SDL_WINDOW_RESIZABLE))) {
        sdldie("SDL_CreateWindow");
    }
    if (!(self->gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, ISDBG, NULL))) {
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
