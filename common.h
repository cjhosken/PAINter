#ifndef COMMON_H
#define COMMON_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"
#include <iostream>

using namespace std;

SDL_Window *window;
SDL_Event event;

int mX, mY;

enum Mode {
    DRAW,
    ERASE,
    FILL,
    SHAPE,
    PICKER
};

Mode editMode = Mode::DRAW;
SDL_Color* activeColor = new SDL_Color({89, 89, 89, 255});

#endif