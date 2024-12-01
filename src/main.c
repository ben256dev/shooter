#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Use new callback API for SDL3
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "common.h"
#include "game_state.h"

static game_state_t global_game_state;

SDL_GPUShader *vertex, *fragment;
SDL_GPUGraphicsPipeline* pipeline;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
    SDL_SetAppMetadata("obama", "0.0.0", "net.sugma.balls");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        sdldie("SDL_Init");
    }

    *appstate = &global_game_state;
    if (!game_state_init(*appstate)) {
        return SDL_APP_FAILURE;
    }
    game_state_t* gs = *appstate;

    // if (!(vertex = render_state_load_shader(
    //         &gs->rs, "res/shaders/test.vert.spv", SDL_GPU_SHADERSTAGE_VERTEX))) {
    //     die("Can't load vertex shader!");
    // }
    // if (!(fragment = render_state_load_shader(
    //         &gs->rs, "res/shaders/test.frag.spv", SDL_GPU_SHADERSTAGE_FRAGMENT))) {
    //     die("Can't load fragment shader!");
    // }
    // if (!(pipeline = SDL_CreateGPUGraphicsPipeline(gs->rs.gpu,
    // &(SDL_GPUGraphicsPipelineCreateInfo){
    //     .vertex_shader = vertex,
    //     .fragment_shader = fragment,
    //     .vertex_input_state = {
    //         .vertex_buffer_descriptions = NULL,
    //         .num_vertex_buffers = 0,
    //         .vertex_attributes = NULL,
    //         .num_vertex_attributes = 0,
    //     },
    //     .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    //     .rasterizer_state = {
    //         .fill_mode = SDL_GPU_FILLMODE_FILL,
    //         .cull_mode = SDL_GPU_CULLMODE_NONE,
    //         .front_face = SDL_GPU_FRONTFACE_CLOCKWISE,
    //         .depth_bias_constant_factor = 0.0f,
    //         .depth_bias_slope_factor = 0.00001f,
    //         .depth_bias_clamp = 0.001f,
    //         .enable_depth_bias = false,
    //         .enable_depth_clip = false,
    //     },
    //     .multisample_state = {
    //         .enable_mask = false,
    //         .sample_mask = false,
    //         .sample_count = 0,
    //     },
    //     .depth_stencil_state = {
    //         .compare_op = SDL_GPU_COMPAREOP_ALWAYS,
    //         .back_stencil_state = {
    //             .compare_op = SDL_GPU_COMPAREOP_ALWAYS,
    //         },
    //         .front_stencil_state = {
    //             .compare_op = SDL_GPU_COMPAREOP_ALWAYS,
    //         },
    //         .compare_mask = -1,
    //         .write_mask = 0,
    //         .enable_depth_test = false,
    //         .enable_depth_write = false,
    //         .enable_stencil_test = false,
    //     },
    //     .target_info = {
    //         .color_target_descriptions = (SDL_GPUColorTargetDescription[]){
    //             (SDL_GPUColorTargetDescription){
    //                 .format = SDL_GetGPUSwapchainTextureFormat(gs->rs.gpu, gs->rs.win),
    //                 .blend_state = {
    //                     .enable_blend = false,
    //                 },
    //             },
    //         },
    //         .num_color_targets = 1,
    //         .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
    //         .has_depth_stencil_target = false,
    //     },
    // }))) {
    //     sdldie("SDL_CreateGPUGraphicsPipeline");
    // }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* _appstate, SDL_AppResult result)
{
    game_state_t* gs = _appstate;
    SDL_ReleaseGPUGraphicsPipeline(gs->rs.gpu, pipeline);
    SDL_ReleaseGPUShader(gs->rs.gpu, vertex);
    SDL_ReleaseGPUShader(gs->rs.gpu, fragment);
    game_state_deinit(gs);
}

SDL_AppResult SDL_AppIterate(void* _appstate)
{
    game_state_t* gs = _appstate;

    SDL_GPURenderPass* pass;
    if (!(pass = SDL_BeginGPURenderPass(
              gs->rs.cmdbuf, gs->rs.ctargs, arrlen(gs->rs.ctargs), &gs->rs.dstarg))) {
        sdldie("SDL_BeginGPURenderPass");
    }
    SDL_SetGPUViewport(pass,
        &(SDL_GPUViewport) {
            .x = 0.0f,
            .y = 0.0f,
            .w = (f32)gs->rs.swap.w,
            .h = (f32)gs->rs.swap.h,
            .min_depth = 0.0f,
            .max_depth = 1.0f,
        });
    SDL_EndGPURenderPass(pass);
    SDL_SubmitGPUCommandBuffer(gs->rs.cmdbuf);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* _appstate, SDL_Event* event)
{
    game_state_t* gs = _appstate;

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}
