#ifndef RECT_H
#define RECT_H

#include "../../common.h"
#include "circle.h"

void drawRect(SDL_Renderer* renderer, SDL_Color color, SDL_Rect rect) {

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &rect);
}
void drawRect(SDL_Renderer* renderer, SDL_Color color, SDL_Rect rect, int radius) {

    SDL_Rect r1 = {rect.x, rect.y+radius, rect.w, rect.h-radius*2};
    SDL_Rect r2 = {rect.x+radius, rect.y, rect.w-radius*2, rect.h};

    drawRect(renderer, color, r1);
    drawRect(renderer, color, r2);

    SDL_Circle ctl = {rect.x+radius, rect.y+radius, radius};
    SDL_Circle ctr = {rect.x+rect.w-radius, rect.y+radius, radius};
    SDL_Circle cbr = {rect.x+rect.w-radius, rect.y+rect.h-radius, radius};
    SDL_Circle cbl = {rect.x+radius, rect.y+rect.h-radius, radius};


    drawCircle(renderer, color, ctl);
    drawCircle(renderer, color, ctr);
    drawCircle(renderer, color, cbr);
    drawCircle(renderer, color, cbl);
}

#endif