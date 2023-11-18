#ifndef CANVAS_H
#define CANVAS_H

#include "../../common.h"

struct SDL_Canvas {
    SDL_Rect* rect = new SDL_Rect();
    SDL_Surface* image = new SDL_Surface();

    void draw(SDL_Renderer* renderer) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_Rect src = {0, 0, (*image).w, (*image).h};
        SDL_RenderCopy(renderer, texture, &src, rect);
        SDL_DestroyTexture(texture);
    }

    void setRect(int x, int y, int w, int h) {
        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
    }

    void setImage(SDL_Surface* s) {
        image = s;
    }
};



#endif