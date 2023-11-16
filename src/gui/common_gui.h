#ifndef COMMON_GUI_H
#define COMMON_GUI_H
#include "../../common.h"

typedef struct {
    float radius;
    int x; 
    int y;
} SDL_Circle;

SDL_Rect* drawRect(SDL_Renderer* renderer, SDL_Color color, int x, int y, int width, int height) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    return &rect;
}

SDL_Circle* drawCircle(SDL_Renderer* renderer, SDL_Color color, int cx, int cy, int radius) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, radius*2, radius*2, 32, SDL_PIXELFORMAT_RGBA8888);

    for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
            if (x*x + y*y <= radius*radius) {
                int dX = radius + x;
                int dY = radius + y;
                if (dX >= 0 && dX < surface->w && dY >= 0 && dY < surface->h) {
                    // Calculate the pixel offset


                    Uint32* pixels = (Uint32*)surface->pixels;
                    int pixelIndex = dY * (surface->pitch / sizeof(Uint32)) + dX;

                    // Create the pixel value combining RGBA components
                    Uint32 pixelValue = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);

                    // Set the pixel value
                    pixels[pixelIndex] = pixelValue;
                }
            }
        }
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {cx-radius, cy-radius, radius*2, radius*2};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_Circle circle;
    circle.x = cx;
    circle.y = cy;
    circle.radius = radius;
    return &circle;
}

SDL_Rect* drawRect(SDL_Renderer* renderer, SDL_Color color, int x, int y, int width, int height, float radius) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    drawRect(renderer, color, x, y+radius, width, height-radius*2);
    drawRect(renderer, color, x+radius, y, width-radius*2, height);

    drawCircle(renderer, color, rect.x+radius, rect.y+radius, radius);
    drawCircle(renderer, color, rect.x+width-radius, rect.y+radius, radius);
    drawCircle(renderer, color, rect.x+width-radius, rect.y+height-radius, radius);
    drawCircle(renderer, color, rect.x+radius, rect.y+height-radius, radius);

    return &rect;
}


#endif