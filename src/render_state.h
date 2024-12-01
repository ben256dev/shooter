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
    SDL_GPUShaderFormat shader_format;
} render_state_t;

bool render_state_init(render_state_t* self);
void render_state_deinit(render_state_t* self);
void render_state_render(render_state_t* self);
SDL_GPUShader* render_state_load_shader(
    render_state_t* self, const char* spriv_path, SDL_GPUShaderStage stage);
