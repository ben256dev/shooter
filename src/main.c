#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "common.h"

#define SHADERFORMATS (SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_METALLIB)
#define USEMSAA true

static SDL_Window* window;
static SDL_GPUDevice* gpu;
static SDL_GPUGraphicsPipeline* pipeline;
static SDL_GPUTexture* tex_msaa;
static SDL_GPUTexture* tex_resolve;

typedef struct shader_info {
    SDL_GPUShader* shader;
    u8* code;
    usize codelen;
} shader_info_t;

static void shader_info_deinit(shader_info_t* self)
{
    SDL_ReleaseGPUShader(gpu, self->shader);
    free(self->code);
}

static shader_info_t load_shader(const char* path, SDL_GPUShaderCreateInfo initial_info)
{
    char fullpath[512];

#ifdef __APPLE__
    snprintf(fullpath, arrlen(fullpath), "res/shaders/metal/%s.metallib", path);
#else
    snprintf(fullpath, arrlen(fullpath), "res/shaders/glsl/%s.spv", path);
#endif

    shader_info_t info = { 0 };
    if (!(info.code = load_file_bytes(fullpath, &info.codelen))) {
        warn("Can't load shader path: %s", fullpath);
        return info;
    }
    initial_info.code = info.code;
    initial_info.code_size = info.codelen;

#ifdef __APPLE__
    initial_info.format = SDL_GPU_SHADERFORMAT_METALLIB;
    switch (initial_info.stage) {
    case SDL_GPU_SHADERSTAGE_VERTEX:
        initial_info.entrypoint = "vertex_main";
        break;
    case SDL_GPU_SHADERSTAGE_FRAGMENT:
        initial_info.entrypoint = "fragment_main";
        break;
    default:
        die("unsupported shader stage");
    }
#else
    initial_info.format = SDL_GPU_SHADERFORMAT_SPIRV;
    initial_info.entrypoint = "main";
#endif
    if (!(info.shader = SDL_CreateGPUShader(gpu, &initial_info))) {
        warn("Can't load shader binary: %s", fullpath);
    }

    return info;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("obama", "0.0.0", "net.sugma.balls");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        sdldie("SDL_Init");
    }

    if (!(window = SDL_CreateWindow("obama", 800, 600, SDL_WINDOW_HIGH_PIXEL_DENSITY))) {
        sdldie("SDL_CreateWindow");
    }
    if (!(gpu = SDL_CreateGPUDevice(SHADERFORMATS, true, NULL))) {
        sdldie("SDL_CreateGPUDevice");
    }
    SDL_ClaimWindowForGPUDevice(gpu, window);

    shader_info_t vertex, fragment;
    if (!(vertex = load_shader("test.vert",
              (SDL_GPUShaderCreateInfo) {
                  .stage = SDL_GPU_SHADERSTAGE_VERTEX,
                  .num_samplers = 0,
                  .num_storage_buffers = 0,
                  .num_storage_textures = 0,
                  .num_uniform_buffers = 0,
                  .props = 0,
              }))
            .shader) {
        return SDL_APP_FAILURE;
    }

    if (!(fragment = load_shader("test.frag",
              (SDL_GPUShaderCreateInfo) {
                  .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,
                  .num_samplers = 0,
                  .num_storage_buffers = 0,
                  .num_storage_textures = 0,
                  .num_uniform_buffers = 0,
                  .props = 0,
              }))
            .shader) {
        return SDL_APP_FAILURE;
    }

    u32 sample_count = SDL_GPU_SAMPLECOUNT_1;
    if (USEMSAA
        && SDL_GPUTextureSupportsSampleCount(
            gpu, SDL_GetGPUSwapchainTextureFormat(gpu, window), SDL_GPU_SAMPLECOUNT_4)) {
        sample_count = SDL_GPU_SAMPLECOUNT_4;
    }

    if (!(pipeline = SDL_CreateGPUGraphicsPipeline(gpu, &(SDL_GPUGraphicsPipelineCreateInfo){
        .target_info = {
            .num_color_targets = 1,
            .color_target_descriptions = &(SDL_GPUColorTargetDescription){
                .format = SDL_GetGPUSwapchainTextureFormat(gpu, window),
                .blend_state = {0},
            },
            .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
            .has_depth_stencil_target = false,
        },
        .depth_stencil_state = {
            .enable_depth_test = false,
            .enable_depth_write = false,
            .compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
        },
        .multisample_state.sample_count = sample_count,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .vertex_shader = vertex.shader,
        .fragment_shader = fragment.shader,
        .vertex_input_state = {
            .num_vertex_buffers = 0,
            .num_vertex_attributes = 0,
        },
        .props = 0,
    }))) {
        sdldie("SDL_CreateGPUGraphicsPipeline");
    }

    shader_info_deinit(&vertex);
    shader_info_deinit(&fragment);

    if (sample_count != 1) {
        int w, h;
        SDL_GetWindowSizeInPixels(window, &w, &h);
        if (!(tex_msaa = SDL_CreateGPUTexture(gpu,
                  &(SDL_GPUTextureCreateInfo) {
                      .type = SDL_GPU_TEXTURETYPE_2D,
                      .format = SDL_GetGPUSwapchainTextureFormat(gpu, window),
                      .width = w,
                      .height = h,
                      .layer_count_or_depth = 1,
                      .num_levels = 1,
                      .sample_count = sample_count,
                      .usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
                      .props = 0,
                  }))) {
            sdldie("msaa SDL_CreateGPUTexture");
        }
        if (!(tex_resolve = SDL_CreateGPUTexture(gpu,
                  &(SDL_GPUTextureCreateInfo) {
                      .type = SDL_GPU_TEXTURETYPE_2D,
                      .format = SDL_GetGPUSwapchainTextureFormat(gpu, window),
                      .width = w,
                      .height = h,
                      .layer_count_or_depth = 1,
                      .num_levels = 1,
                      .sample_count = SDL_GPU_SAMPLECOUNT_1,
                      .usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
                      .props = 0,
                  }))) {
            sdldie("resolve SDL_CreateGPUTexture");
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_GPUCommandBuffer* cmdbuf;
    struct {
        SDL_GPUTexture* tex;
        u32 w, h;
    } swap;

    if (!(cmdbuf = SDL_AcquireGPUCommandBuffer(gpu))) {
        sdldie("SDL_AcquireGPUCommandBuffer");
    }

    if (!SDL_AcquireGPUSwapchainTexture(cmdbuf, window, &swap.tex, &swap.w, &swap.h)) {
        sdldie("SDL_AcquireGPUSwapchainTexture");
    }

    // To many frames in flight? Don't render this frame.
    if (!swap.tex) {
        SDL_SubmitGPUCommandBuffer(cmdbuf);
        return SDL_APP_CONTINUE;
    }

    // TODO: Resize window textures

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmdbuf,
        &(SDL_GPUColorTargetInfo) {
            .clear_color = { .r = 0.2f, .g = 0.3f, .b = 0.3f, .a = 1.0f },
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = tex_msaa ? SDL_GPU_STOREOP_RESOLVE : SDL_GPU_STOREOP_STORE,
            .texture = tex_msaa ? tex_msaa : swap.tex,
            .resolve_texture = tex_resolve,
            .cycle = !!tex_msaa,
            .cycle_resolve_texture = !!tex_msaa,
        },
        1, NULL);
    SDL_BindGPUGraphicsPipeline(pass, pipeline);
    SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);
    SDL_EndGPURenderPass(pass);

    if (tex_msaa) {
        SDL_BlitGPUTexture(cmdbuf,
            &(SDL_GPUBlitInfo) {
                .source.texture = tex_resolve,
                .source.w = swap.w,
                .source.h = swap.h,

                .destination.texture = swap.tex,
                .destination.w = swap.w,
                .destination.h = swap.h,

                .load_op = SDL_GPU_LOADOP_DONT_CARE,
                .filter = SDL_GPU_FILTER_LINEAR,
            });
    }

    SDL_SubmitGPUCommandBuffer(cmdbuf);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    SDL_ReleaseGPUTexture(gpu, tex_msaa);
    SDL_ReleaseGPUTexture(gpu, tex_resolve);
    SDL_ReleaseWindowFromGPUDevice(gpu, window);
    SDL_ReleaseGPUGraphicsPipeline(gpu, pipeline);
    SDL_DestroyGPUDevice(gpu);
}