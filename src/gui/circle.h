#ifndef CIRCLE_H
#define CIRCLE_H

#include "../../common.h"

typedef struct SDL_MyCircle {
    int x;
    int y;
    int radius;
    SDL_Color* color = new SDL_Color();

    void setPosition(int i, int j) {
        x = i;
        y = j;
    }

    void setRadius(int r) {
        radius = r;
    }

    void setColor(SDL_Color* c) {
        color = c;
    }

    void draw(SDL_Renderer *renderer, int trimX, int trimY)
    {
        SDL_Surface *surface = circle(radius, color, trimX, trimY);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = {x - radius, y - radius, radius * 2, radius * 2};

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_DestroyTexture(texture);
    }

    int isMouseOver(int mouseX, int mouseY) {
        return SDL_sqrt(SDL_pow(x - mouseX, 2.0) + SDL_pow(y - mouseY, 2.0)) <= radius;
    }

} SDL_MyCircle;

#endif