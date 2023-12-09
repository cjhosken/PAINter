#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

char *readFilePath = NULL;
char *writeFilePath = NULL;

typedef struct PNTR_Vector2D {
    int x; 
    int y;

    PNTR_Vector2D() {x=0;y=0;};
    PNTR_Vector2D(int x0, int y0) {x=x0;y=y0;};

} PNTR_Vector2D;

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

PNTR_Vector2D mousePos;

PNTR_PaintMode paintMode = PNTR_PaintMode::DRAW;

SDL_Color *activeColor = new SDL_Color({0, 0, 0, 255});

bool compare(SDL_Color* a, SDL_Color* b) {
    return (a->r == b->r) && (a->g == b->g) && (a->b == b->b);
}

SDL_Color *getPixel(PNTR_Vector2D position)
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


SDL_Color* getSurfacePixel(SDL_Surface* surface, int x, int y) {
    SDL_Color* pixelColor = new SDL_Color({0, 0, 0, 255});
    Uint32 *pixels = (Uint32*)surface->pixels;
    SDL_GetRGB(pixels[x+y*surface->w], surface->format, &(pixelColor->r), &(pixelColor->g), &(pixelColor->b));

    return pixelColor;
}

void setSurfacePixel(SDL_Surface* surface, SDL_Color *color, int x, int y)
{
    Uint32 *pixels = (Uint32*)surface->pixels;
    Uint32 pixel = SDL_MapRGB(surface->format, color->r, color->g, color->b);
    pixels[x+y*surface->w] = pixel;

}
//https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface


#endif