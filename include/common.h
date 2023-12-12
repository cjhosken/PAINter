#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "pntr_vector2i.h"

using namespace std;

#define APP_WIDTH 1280
#define APP_HEIGHT 720

#define DRAW_SIZE 64

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event event;

extern char *readFilePath;
extern char *writeFilePath;

enum PNTR_PaintMode
{
    DRAW,
    ERASE,
    FILL,
    SHAPE_LINE,
    SHAPE_CIRCLE,
    SHAPE_SQUARE,
    PICKER
};

extern PNTR_Vector2I* mousePos;

extern PNTR_PaintMode paintMode;

extern SDL_Color *activeColor;

inline bool compare(SDL_Color* a, SDL_Color* b) {
    return (a->r == b->r) && (a->g == b->g) && (a->b == b->b);
}

inline SDL_Color* getPixel(SDL_Renderer* renderer, PNTR_Vector2I position)
{
    SDL_Color* pixelColor = new SDL_Color({0, 0, 0, 255});

    Uint32 pixels[10];

    SDL_Rect pickerRect = {position.x, position.y, 1, 1};

    SDL_Surface *surface = SDL_CreateRGBSurface(0, 5, 5, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_Surface *newSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);

    if (!SDL_RenderReadPixels(renderer, &pickerRect, SDL_PIXELFORMAT_ARGB8888, pixels, 5))
    {
        SDL_GetRGB(pixels[0], newSurface->format, &(pixelColor->r), &(pixelColor->g), &(pixelColor->b));
    }

    SDL_FreeSurface(surface);
    SDL_FreeSurface(newSurface);
    return pixelColor;
}


inline SDL_Color* getSurfacePixel(SDL_Surface* surface, PNTR_Vector2I position) {
    SDL_Color* pixelColor = new SDL_Color({0, 0, 0, 255});
    Uint32 *pixels = (Uint32*)surface->pixels;
    SDL_GetRGB(pixels[position.x+position.y*surface->w], surface->format, &(pixelColor->r), &(pixelColor->g), &(pixelColor->b));

    return pixelColor;
}

inline void setSurfacePixel(SDL_Surface* surface, SDL_Color *color, PNTR_Vector2I position)
{
    Uint32 *pixels = (Uint32*)surface->pixels;
    Uint32 pixel = SDL_MapRGB(surface->format, color->r, color->g, color->b);
    pixels[position.x+position.y*surface->w] = pixel;

}

#endif