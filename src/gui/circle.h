#ifndef CIRCLE_H
#define CIRCLE_H

#include "../../common.h"

typedef struct {
    int x; 
    int y;
    int radius;
} SDL_Circle;

void drawCircle(SDL_Renderer* renderer, SDL_Color color, SDL_Circle circle) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, circle.radius*2, circle.radius*2, 32, SDL_PIXELFORMAT_RGBA8888);

    int i;
    int j;

    for (i = -circle.radius; i <= circle.radius; ++i) {
        for (j = -circle.radius; j <= circle.radius; ++j) {
            if (j*j + i*i <= circle.radius*circle.radius) {
                int dX = circle.radius + j;
                int dY = circle.radius + i;
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

    SDL_Rect dstRect = {circle.x-circle.radius, circle.y-circle.radius, circle.radius*2, circle.radius*2};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

void drawPartialCircle(SDL_Renderer* renderer, SDL_Color color, SDL_Circle circle, int trimX, int trimY) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, circle.radius*2, circle.radius*2, 32, SDL_PIXELFORMAT_RGBA8888);

    int i;
    int j;

    for (i = -circle.radius; i <= circle.radius; ++i) {
        for (j = -circle.radius; j <= circle.radius; ++j) {
            if (j*j + i*i <= circle.radius*circle.radius) {
                int dX = circle.radius + j - trimX;
                int dY = circle.radius + i - trimY;
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

    SDL_Rect dstRect = {circle.x-circle.radius, circle.y-circle.radius, circle.radius*2, circle.radius*2};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}


#endif