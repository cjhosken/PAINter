#ifndef CANVAS_H
#define CANVAS_H

#include "../../common.h"

struct SDL_Canvas {
    SDL_Rect rect;
    SDL_Surface* image;
    int scale;
};


void drawCanvas(SDL_Renderer* renderer, SDL_Canvas canvas) {
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, canvas.image);
    SDL_Rect src = {0, 0, (*canvas.image).w, (*canvas.image).h};
    SDL_RenderCopy(renderer, texture, &src, &(canvas.rect));
}

#endif