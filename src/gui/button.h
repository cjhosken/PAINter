#ifndef BUTTON_H
#define BUTTON_H

#include "../../common.h"
#include "rect.h"

typedef struct SDL_MyButton
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

        if (mouseOver(mX, mY))
        {
            drawRect(renderer, new SDL_Color({255, 0, 0, 255}), rect);
        }
        else if (active)
        {
            drawRect(renderer, new SDL_Color({0, 255, 0, 255}), rect);
        }
        else
        {
            drawRect(renderer, color, rect);
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

    bool mouseOver(int mouseX, int mouseY)
    {
        return (
            mouseX >= rect->x && mouseX <= rect->x + rect->w &&
            mouseY >= rect->y && mouseY <= rect->y + rect->h);
    }

} SDL_MyButton;

#endif