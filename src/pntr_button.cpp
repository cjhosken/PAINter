#include "pntr_button.h"
#include "include/pntr_panel.h"
#include "include/common.h"

PNTR_Button::PNTR_Button() : PNTR_Panel(){};
PNTR_Button::PNTR_Button(SDL_Rect *bb) : PNTR_Panel(bb)
{
    color = new SDL_Color();
};
PNTR_Button::PNTR_Button(SDL_Rect *bb, SDL_Surface *i) : PNTR_Panel(bb)
{
    color = new SDL_Color();
    icon = i;
};
PNTR_Button::PNTR_Button(SDL_Rect *bb, SDL_Surface *i, void (*func)(void)) : PNTR_Panel(bb)
{
    color = new SDL_Color();
    icon = i;
    action = func;
};

void PNTR_Button::setAction(void (*func)(void))
{
    action = func;
}

void PNTR_Button::executeAction()
{
    action();
}

void PNTR_Button::draw(SDL_Renderer *renderer, SDL_Event* event, SDL_Window* window)
{
    if (isMouseOver(mousePos) && SDL_GetWindowID(window) == event->window.windowID)
    {
        PNTR_Panel::renderPanel(renderer, *bbox, SDL_Color({0, 0, 0, 75}), radius);
    }
    else if (active)
    {
        PNTR_Panel::renderPanel(renderer, *bbox, SDL_Color({255, 255, 255, 255}), radius);
    }
    else
    {
        PNTR_Panel::renderPanel(renderer, *bbox, *color, 0);
    }

    // Render the image texture onto the rectangle
    if (icon != nullptr)
    {
        SDL_Texture *iconTexture = SDL_CreateTextureFromSurface(renderer, icon);
        SDL_RenderCopy(renderer, iconTexture, NULL, bbox);

        // Free the texture when done
        SDL_DestroyTexture(iconTexture);
    }
}

void PNTR_Button::setIcon(SDL_Surface *s)
{
    icon = s;
}

void PNTR_Button::setActive(bool a)
{
    active = a;
}

bool PNTR_Button::isActive()
{
    return active;
}

void PNTR_Button::onPressEvent() {
    executeAction();
    setActive(true);
};

// Copyright © 2024 Christopher Hosken