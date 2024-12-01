#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "common.h"

#define SHADERFORMATS                                                                              \
    (SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXBC | SDL_GPU_SHADERFORMAT_DXIL            \
        | SDL_GPU_SHADERFORMAT_METALLIB)
#define USEMSAA true

static SDL_Window* window;
static SDL_GPUDevice* gpu;

static SDL_GPUShader* load_shader(SDL_GPUShaderStage stage) { return NULL; }

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("obama", "0.0.0", "net.sugma.balls");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        sdldie("SDL_Init");
    }

    if (!(window = SDL_CreateWindow("obama", 800, 600, SDL_WINDOW_HIDDEN))) {
        sdldie("SDL_CreateWindow");
    }
    if (!(gpu = SDL_CreateGPUDevice(SHADERFORMATS, true, NULL))) {
        sdldie("SDL_CreateGPUDevice");
    }
    SDL_ClaimWindowForGPUDevice(gpu, window);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) { return SDL_APP_CONTINUE; }

void SDL_AppQuit(void* appstate, SDL_AppResult result) { }
