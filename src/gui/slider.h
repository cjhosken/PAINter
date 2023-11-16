#ifndef SLIDER_H
#define SLIDER_H

#include "../../common.h"
#include "common_gui.h"

typedef struct {
    float value;
} SDL_Slider;

SDL_Slider* drawSlider(SDL_Renderer *renderer) {

    SDL_Slider slider;

    SDL_RenderDrawLine(renderer, 328, 32, 488, 32);
    drawCircle(renderer, {0, 0, 0, 255}, 340, 32, 6);

    return &slider;
}

#endif