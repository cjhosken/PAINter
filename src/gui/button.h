#ifndef BUTTON_H
#define BUTTON_H

#include "../../common.h"
#include "rect.h"

typedef struct PNTR_Button
{
    SDL_Rect *rect = new SDL_Rect();
    SDL_Color *color = new SDL_Color({255, 255, 255, 0});
    SDL_Surface *icon = new SDL_Surface();

    bool active = false;

    void (*action)(void);

    void setAction(void (*func)(void))
    {
        action = func;
    }

    void draw(SDL_Renderer *renderer)
    {
        SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, icon);

        if (mouseOver(mousePos) && SDL_GetWindowID(window) == event.window.windowID)
        {
            renderRect(renderer, new SDL_Color({0, 0, 0, 75}), rect, 5);
        }
        else if (active)
        {
            renderRect(renderer, new SDL_Color({255, 255, 255, 255}), rect, 5);
        }
        else
        {
            renderRect(renderer, color, rect, 0);
        }

        // Render the image texture onto the rectangle
        SDL_RenderCopy(renderer, imageTexture, NULL, rect);

        // Free the texture when done
        SDL_DestroyTexture(imageTexture);
    }

    void setRect(int x, int y, int w, int h)
    {
        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
    }

    void setIcon(SDL_Surface *s)
    {
        icon = s;
    }

    void setActive(bool f) {
        active = f;
    }

    bool mouseOver(PNTR_Vector2D position)
    {
        return (
            position.x >= rect->x && position.x <= rect->x + rect->w &&
            position.y >= rect->y && position.y <= rect->y + rect->h
        );
    }

    int pressEvent() {
        if (mouseOver(mousePos)) {
            action();
            return 1;
        }
        return 0;
    }

} PNTR_Button;

#endif