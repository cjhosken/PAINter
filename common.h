#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Event event;

int mX, mY;

using namespace std;

enum Mode
{
    DRAW,
    ERASE,
    FILL,
    SHAPE,
    PICKER
};

Mode editMode = Mode::DRAW;
SDL_Color *activeColor = new SDL_Color({89, 89, 89, 255});

SDL_Surface *circle(int radius, SDL_Color *color)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);

    int i;
    int j;

    for (i = -radius; i <= radius; ++i)
    {
        for (j = -radius; j <= radius; ++j)
        {
            if (j * j + i * i <= radius * radius)
            {
                int dX = radius + j;
                int dY = radius + i;
                if (dX >= 0 && dX < surface->w && dY >= 0 && dY < surface->h)
                {
                    // Calculate the pixel offset

                    Uint32 *pixels = (Uint32 *)surface->pixels;
                    int pixelIndex = dY * (surface->pitch / sizeof(Uint32)) + dX;

                    // Create the pixel value combining RGBA components
                    Uint32 pixelValue = SDL_MapRGBA(surface->format, color->r, color->g, color->b, color->a);

                    // Set the pixel value
                    pixels[pixelIndex] = pixelValue;
                }
            }
        }
    }
    return surface;
}

#endif