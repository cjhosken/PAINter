#include "pntr_colordialog.h"
#include "../include/common.h"
#include "../include/pntr_slider.h"
#include "../include/pntr_panel.h"
#include <vector>

PNTR_ColorDialog::PNTR_ColorDialog(SDL_Event *e)
{
    event = e;
    rSlider = new PNTR_Slider(new SDL_Rect({50, 100, 200, 1}), new SDL_Color({255, 0, 0, 255}));
    gSlider = new PNTR_Slider(new SDL_Rect({50, 135, 200, 1}), new SDL_Color({0, 255, 0, 255}));
    bSlider = new PNTR_Slider(new SDL_Rect({50, 170, 200, 1}), new SDL_Color({0, 0, 255, 255}));
    rSlider->setValue(activeColor->r / 255.0f);
    gSlider->setValue(activeColor->g / 255.0f);
    bSlider->setValue(activeColor->b / 255.0f);

    colorView = new PNTR_Panel(new SDL_Rect({50, 10, 200, 75}), activeColor, 5);

    dialog = SDL_CreateWindow("PAINter Color", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);

    renderer = SDL_CreateRenderer(dialog, -1, SDL_RENDERER_ACCELERATED);

    sliders.push_back(rSlider);
    sliders.push_back(gSlider);
    sliders.push_back(bSlider);
}

void PNTR_ColorDialog::show()
{
    colorView->setColor(activeColor);
    rSlider->setValue(activeColor->r / 255.0f);
    gSlider->setValue(activeColor->g / 255.0f);
    bSlider->setValue(activeColor->b / 255.0f);
    SDL_ShowWindow(dialog);
    invoked = true;
}

void PNTR_ColorDialog::draw()
{
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);

    colorView->draw(renderer);
    for (int sdx = 0; sdx < (int)sliders.size(); sdx++)
    {
        sliders.at(sdx)->draw(renderer);
    }

    SDL_RenderPresent(renderer);
}

bool PNTR_ColorDialog::isInvoked() { return invoked; }

void PNTR_ColorDialog::processEvents(PNTR_Slider* activeSlider)
{
    if (!invoked)
    {
        return SDL_HideWindow(dialog);
    }

    if (SDL_GetWindowID(dialog) == event->window.windowID)
    {
        switch (event->window.event)
            {
                case SDL_WINDOWEVENT_CLOSE:
                    hide();
                    break;

                default:
                    break;
            }

        switch (event->type)
        {
            case SDL_MOUSEMOTION:
                mousePos = new PNTR_Vector2I(event->motion.x, event->motion.y);
                break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event->button.button)
            {

            case SDL_BUTTON_LEFT:
                
                buttonPressed = false;
                for (int sdx=0; sdx<(int)sliders.size();sdx++)
                {
                    if (sliders.at(sdx)->isMouseOver(mousePos))
                    {
                        sliders.at(sdx)->pressEvent();
                        leftMouseDown = true;
                        activeSlider = sliders.at(sdx);
                        break;
                    }
                }
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:

            activeSlider = nullptr;

            break;
        }
    }

    // HSV TO RGB CONVERSION HERE
    // SET GLOBAL COLOR TO THAT
    activeColor->a = 255;
    activeColor->r = (int)(255 * rSlider->getValue());
    activeColor->g = (int)(255 * gSlider->getValue());
    activeColor->b = (int)(255 * bSlider->getValue());
}

void PNTR_ColorDialog::hide()
{
    invoked = false;
    SDL_HideWindow(dialog);
}
