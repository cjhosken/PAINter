#ifndef COMMON_H
#define COMMON_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"

int mX, mY;

enum Mode {
    DRAW,
    ERASE,
    FILL,
    SHAPE
};

Mode editMode = Mode::DRAW;

#endif