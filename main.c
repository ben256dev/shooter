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
