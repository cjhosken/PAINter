#include "pntr_colordialog.h"
#include "pntr_slider.h"
#include "pntr_panel.h"
#include "../common.h"

PNTR_ColorDialog::PNTR_ColorDialog()
{
    color = new SDL_Color({0, 0, 0, 255});
    init();
};

PNTR_ColorDialog::PNTR_ColorDialog(SDL_Color *c) : color(c) { init(); };

void PNTR_ColorDialog::init()
{
    SDL_CreateRenderer(dialog, -1, SDL_RENDERER_ACCELERATED);

    SDL_CreateWindow("PAINter Color", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);

    rSlider = new PNTR_Slider(new SDL_Rect({50, 100, 200, 1}), new SDL_Color({255, 0, 0, 255}), color->r / 255.0f);
    rSlider = new PNTR_Slider(new SDL_Rect({50, 135, 200, 1}), new SDL_Color({0, 255, 0, 255}), color->g / 255.0f);
    rSlider = new PNTR_Slider(new SDL_Rect({50, 170, 200, 1}), new SDL_Color({0, 0, 255, 255}), color->b / 255.0f);

    sliders[0] = rSlider;
    sliders[1] = gSlider;
    sliders[2] = bSlider;
}

bool PNTR_ColorDialog::isInvoked() {
    return invoked;
}

void PNTR_ColorDialog::invoke()
{
    SDL_ShowWindow(dialog);
    invoked = true;
}

void PNTR_ColorDialog::draw()
{
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderClear(renderer);

    PNTR_Panel::renderPanel(renderer, {50, 10, 200, 75}, *activeColor, 5);

    for (PNTR_Slider *s : sliders)
    {
        s->draw(renderer);
    }

    SDL_RenderPresent(renderer);
}

void PNTR_ColorDialog::update()
{
    activeColor->r = (int)(255 * rSlider->getValue());
    activeColor->g = (int)(255 * gSlider->getValue());
    activeColor->b = (int)(255 * bSlider->getValue());
    activeColor->a = 255;
}


void PNTR_ColorDialog::dispose() {
    invoked = false;
    SDL_HideWindow(dialog);
}