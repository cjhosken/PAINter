#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

int mX, mY;

using namespace std;

enum Mode
{
    DRAW,
    ERASE,
    FILL,
    SHAPE_LINE,
    SHAPE_CIRCLE,
    SHAPE_SQUARE,
    PICKER
};

Mode editMode = Mode::DRAW;
SDL_Color *activeColor = new SDL_Color({0, 0, 0, 255});

SDL_Surface *circle(int radius, SDL_Color *color, int tX, int tY)
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
                int dX = radius + j - tX;
                int dY = radius + i - tY;
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

SDL_Color* getPixel(int x, int y) {
    SDL_Color pixelColor = {0, 0, 0, 255};
    Uint32 pixels[10];

    SDL_Rect pickerRect;
    pickerRect.x = x;
    pickerRect.y = y;
    pickerRect.w = 1;
    pickerRect.h = 1;

    SDL_Surface *s = SDL_CreateRGBSurface(0, 5, 5, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_Surface *ns = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ARGB8888, 0);

    if (!SDL_RenderReadPixels(renderer, &pickerRect, SDL_PIXELFORMAT_ARGB8888, pixels, 5))
    {
        SDL_GetRGB(pixels[0], ns->format, &(pixelColor.r), &(pixelColor.g), &(pixelColor.b));
        // printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", mX, mY, pixelColor.r, pixelColor.g, pixelColor.b, 255);
    }

    SDL_FreeSurface(s);
    SDL_FreeSurface(ns);

    return new SDL_Color({pixelColor.r, pixelColor.g, pixelColor.b, 255});
}

#endif