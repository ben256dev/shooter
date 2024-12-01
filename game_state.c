#include "game_state.h"

// Returns false if the game can still run but something is fishy
bool game_state_init(game_state_t* self) { return render_state_init(&self->rs); }

void game_state_deinit(game_state_t* self) { render_state_deinit(&self->rs); }
