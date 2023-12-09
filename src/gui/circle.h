#ifndef CIRCLE_H
#define CIRCLE_H

#include "../../common.h"

SDL_Surface *circleToSurface(int radius, SDL_Color *color, PNTR_Vector2D trim);

typedef struct PNTR_Circle {
    PNTR_Vector2D position;
    int radius;
    SDL_Color* color = new SDL_Color();

    PNTR_Circle(PNTR_Vector2D p, int r, SDL_Color* c) {
        position = p;
        radius = r;
        color = c;
    }

    void setPosition(PNTR_Vector2D p) {
        position = p;
    }

    void setRadius(int r) {
        radius = r;
    }

    void setColor(SDL_Color* c) {
        color = c;
    }

    void draw(SDL_Renderer *renderer, PNTR_Vector2D trim)
    {
        SDL_Surface *surface = circleToSurface(radius, color, trim);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_Rect dstRect = {position.x - radius, position.y - radius, radius * 2, radius * 2};

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_DestroyTexture(texture);
    }

    bool isMouseOver(PNTR_Vector2D mouse) {
        return SDL_sqrt(SDL_pow(position.x - mouse.x, 2.0) + SDL_pow(position.y - mouse.y, 2.0)) <= radius;
    }

} PNTR_Circle;



SDL_Surface *circleToSurface(int radius, SDL_Color *color, PNTR_Vector2D trim)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);

    PNTR_Vector2D index;

    for (index.x = -radius; index.x <= radius; ++index.x)
    {
        for (index.y = -radius; index.y <= radius; ++index.y)
        {
            if ((index.x * index.x) + (index.y * index.y) <= radius * radius)
            {
                PNTR_Vector2D change = {radius + index.x - trim.x, radius + index.y - trim.y};

                if (change.x >= 0 && change.x < surface->w && change.y >= 0 && change.y < surface->h)
                {
                    Uint32 *pixels = (Uint32 *)surface->pixels;
                    int pixelIndex = change.y * (surface->pitch / sizeof(Uint32)) + change.x;

                    Uint32 pixelValue = SDL_MapRGBA(surface->format, color->r, color->g, color->b, color->a);

                    pixels[pixelIndex] = pixelValue;
                }
            }
        }
    }
    return surface;
}

#endif