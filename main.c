#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Use new callback API for SDL3
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "common.h"

typedef struct render_state {
    SDL_GPUDevice* gpu_dev;
    SDL_Window* window;
} render_state_t;

typedef struct game_state {
    render_state_t render_state;
} game_state_t;

static game_state_t global_game_state;

// Returns false if the render can still render the game but something is fishy
bool render_state_init(render_state_t* self)
{
    if (!(self->window = SDL_CreateWindow("shooter", 800, 600, SDL_WINDOW_RESIZABLE))) {
        sdldie("SDL_CreateWindow");
    }
    if (!(self->gpu_dev = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, ISDBG, NULL))) {
        sdldie("SDL_CreateGPUDevice");
    }
    if (!SDL_ClaimWindowForGPUDevice(self->gpu_dev, self->window)) {
        sdldie("SDL_ClaimWindowForGPUDevice");
    }
    return true;
}

void render_state_deinit(render_state_t* self)
{
    SDL_DestroyGPUDevice(self->gpu_dev);
    SDL_DestroyWindow(self->window);
    *self = (render_state_t) { 0 };
}

// Returns false if the game can still run but something is fishy
bool game_state_init(game_state_t* self) { return render_state_init(&self->render_state); }

void game_state_deinit(game_state_t* self) { render_state_deinit(&self->render_state); }

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

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* _appstate, SDL_AppResult result)
{
    game_state_t* gs = _appstate;
    game_state_deinit(gs);
}

SDL_AppResult SDL_AppIterate(void* _appstate)
{
    game_state_t* gs = _appstate;

    // Do shit

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
