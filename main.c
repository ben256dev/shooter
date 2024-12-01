#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_AUDIO |
             SDL_INIT_VIDEO |
             SDL_INIT_JOYSTICK |
             SDL_INIT_HAPTIC |
             SDL_INIT_GAMEPAD |
             SDL_INIT_EVENTS |
             SDL_INIT_SENSOR |
             SDL_INIT_CAMERA );

    printf("Hello world!\n");
    return 0;
}

