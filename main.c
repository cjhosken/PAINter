#include "include/SDL2/SDL.h"

const int APP_WIDTH = 1280;
const int APP_HEIGHT = 720;

//https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
int main(int argc, char** argv) {

    char quit = 0;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, APP_WIDTH, APP_HEIGHT, 0);

    while (!quit) {
        SDL_WaitEvent(&event);

        switch(event.type)
        {
            case SDL_QUIT:
                quit=1;
                break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
//end

// Copyright © 2023 Christopher Hosken