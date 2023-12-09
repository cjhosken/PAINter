#ifndef CANVAS_H
#define CANVAS_H

#include "../../common.h"

// THE CANVAS SHOULD BE DRAWN OVER THE IMAGE, NOT INTO THE IMAGE

typedef struct PNTR_Canvas {
    SDL_Rect* rect = new SDL_Rect();
    SDL_Surface* image = new SDL_Surface();
    SDL_Surface* overlay = new SDL_Surface();
    SDL_Surface* ghosting = new SDL_Surface();
    SDL_Texture* texture;

    void draw(SDL_Renderer* renderer) {
        SDL_Rect src = {0, 0, image->w, image->h};
        SDL_RenderCopy(renderer, texture, &src, rect);

        SDL_Texture* overlayTexture = SDL_CreateTextureFromSurface(renderer, overlay);
        SDL_RenderCopy(renderer, overlayTexture, &src, rect);

        SDL_Texture* ghostingTexture = SDL_CreateTextureFromSurface(renderer, ghosting);
        SDL_RenderCopy(renderer, ghostingTexture, &src, rect);
    }

    void setRect(int x, int y, int w, int h) {
        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
    }

    void setImage(SDL_Surface* s) {
        image = s;
        texture = SDL_CreateTextureFromSurface(renderer, image);
        overlay = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        ghosting = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_FillRect(overlay, NULL, SDL_MapRGBA(overlay->format, 255, 255, 255, 0));
        SDL_FillRect(ghosting, NULL, SDL_MapRGBA(overlay->format, 255, 255, 255, 0));
    } 

} PNTR_Canvas;



#endif