#pragma once

#include "common.h"
#include "render_state.h"

typedef struct game_state {
    render_state_t rs;
} game_state_t;

// Returns false if the game can still run but something is fishy
bool game_state_init(game_state_t* self);

void game_state_deinit(game_state_t* self);
