#ifndef RECT_H
#define RECT_H

#include "../../common.h"
#include "circle.h"

void renderRect(SDL_Renderer *renderer, SDL_Color *color, SDL_Rect *rect, int radius)
{
    if (radius)
    {
        SDL_Rect topRect = {rect->x + radius, rect->y, rect->w - radius * 2, radius};
        SDL_Rect middleRect = {rect->x, rect->y + radius, rect->w, rect->h - radius * 2};
        SDL_Rect bottomRect = {rect->x + radius, rect->y + rect->h - radius, rect->w - radius * 2, radius};

        renderRect(renderer, color, &topRect, 0);
        renderRect(renderer, color, &middleRect, 0);
        renderRect(renderer, color, &bottomRect, 0);

        PNTR_Circle *circleTopLeft = new PNTR_Circle({PNTR_Vector2D({rect->x, rect->y}), radius, color});
        PNTR_Circle *circleTopRight = new PNTR_Circle({PNTR_Vector2D({rect->x + rect->w, rect->y}), radius, color});
        PNTR_Circle *circleBottomRight = new PNTR_Circle({PNTR_Vector2D({rect->x + rect->w, rect->y + rect->h}), radius, color});
        PNTR_Circle *circleBottomLeft = new PNTR_Circle({PNTR_Vector2D({rect->x, rect->y + rect->h}), radius, color});

        circleTopLeft->draw(renderer, PNTR_Vector2D({-radius, -radius}));
        circleTopRight->draw(renderer, PNTR_Vector2D({radius, -radius}));
        circleBottomRight->draw(renderer, PNTR_Vector2D({radius, radius}));
        circleBottomLeft->draw(renderer, PNTR_Vector2D({-radius, radius}));

    } else {
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, rect->w, rect->h, 32, SDL_PIXELFORMAT_RGBA8888);
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

        SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color->r, color->g, color->b, color->a));
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, NULL, rect);
        SDL_DestroyTexture(texture);
    }
}


#endif