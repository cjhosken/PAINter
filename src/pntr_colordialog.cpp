#include "pntr_colordialog.h"
#include "../include/common.h"
#include "../include/pntr_slider.h"
#include "../include/pntr_panel.h"
#include <vector>

// Constructor method
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

// The dialog is never destroyed, only hidden.
void PNTR_ColorDialog::show()
{
    colorView->setColor(activeColor);
    rSlider->setValue(activeColor->r / 255.0f);
    gSlider->setValue(activeColor->g / 255.0f);
    bSlider->setValue(activeColor->b / 255.0f);
    SDL_ShowWindow(dialog);
    invoked = true;
}

// Very simple draw implementation, similar to how buttons are drawn in the Gui.
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
// Function to check whether the window is hidden or not.
bool PNTR_ColorDialog::isInvoked() { return invoked; }

// Function to hide the window.
void PNTR_ColorDialog::hide()
{
    invoked = false;
    SDL_HideWindow(dialog);
}
