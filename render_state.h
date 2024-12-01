#pragma once

#include <SDL3/SDL.h>

#include "common.h"

typedef struct render_state {
    SDL_GPUDevice* gpu;
    SDL_GPUCommandBuffer* cmdbuf;
    struct {
        SDL_GPUTexture* tex;
        u32 w, h;
    } swap;
    SDL_GPUColorTargetInfo ctargs[1];
    SDL_GPUDepthStencilTargetInfo dstarg;
    SDL_Window* win;
} render_state_t;

bool render_state_init(render_state_t* self);
void render_state_deinit(render_state_t* self);
void render_state_render(render_state_t* self);
