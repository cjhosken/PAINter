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
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);

        int i;
        int j;

        for (i = -radius; i <= radius; ++i)
        {
            for (j = -radius; j <= radius; ++j)
            {
                if (j * j + i * i <= radius * radius)
                {
                    int dX = radius + j - trimX;
                    int dY = radius + i - trimY;
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