#ifndef SLIDER_H
#define SLIDER_H

#include "../../common.h"
#include "circle.h"

typedef struct SDL_MySlider {
    float value;
    SDL_MyCircle* circle = new SDL_MyCircle();
    SDL_Color* color = new SDL_Color();

    void setCircle(SDL_MyCircle* c) {
        circle = c;
    }

    void setColor(SDL_Color* c) {
        color = c;
    }

    void draw(SDL_Renderer *renderer) {
        SDL_RenderDrawLine(renderer, 328, 32, 488, 32);

        circle->draw(renderer, 0, 0);
    }
} SDL_MySlider;


#endif