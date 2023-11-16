#ifndef SLIDER_H
#define SLIDER_H

#include "../../common.h"
#include "circle.h"

typedef struct {
    float value;
    SDL_Circle circle;
    SDL_Color color;
} SDL_Slider;

void drawSlider(SDL_Renderer *renderer, SDL_Slider slider) {
    SDL_RenderDrawLine(renderer, 328, 32, 488, 32);

    drawCircle(renderer, slider.color, slider.circle);
}

#endif