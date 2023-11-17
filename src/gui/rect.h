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
    SDL_Rect r2 = {rect.x+radius, rect.y, rect.w-radius*2, radius};
    SDL_Rect r3 = {rect.x+radius, rect.y+rect.h-radius, rect.w-radius*2, radius};

    drawRect(renderer, color, r1);
    drawRect(renderer, color, r2);
    drawRect(renderer, color, r3);

    SDL_Circle ctl = {rect.x, rect.y, radius};
    SDL_Circle ctr = {rect.x+rect.w, rect.y, radius};
    SDL_Circle cbr = {rect.x+rect.w, rect.y+rect.h, radius};
    SDL_Circle cbl = {rect.x, rect.y+rect.h, radius};


    drawPartialCircle(renderer, color, ctl, -radius, -radius);
    drawPartialCircle(renderer, color, ctr, radius, -radius);
    drawPartialCircle(renderer, color, cbr, radius, radius);
    drawPartialCircle(renderer, color, cbl, -radius, radius);
}

#endif