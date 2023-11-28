#ifndef SLIDER_H
#define SLIDER_H

#include "../../common.h"
#include "circle.h"

typedef struct SDL_MySlider {
    float value = 0.5f;
    SDL_Rect* rect = new SDL_Rect();
    SDL_MyCircle* circle = new SDL_MyCircle();
    SDL_Color* color = new SDL_Color();

    void setRect(SDL_Rect* r) {
        rect = r;
        circle->setRadius(8);
        circle->setColor(new SDL_Color({0, 0, 0, 255}));
    }

    void setCircle(SDL_MyCircle* c) {
        circle = c;
    }

    void setColor(SDL_Color* c) {
        color = c;
    }

    void setValue(float f) {
        value = SDL_min(SDL_max(0, f), 1);
    }

    void draw(SDL_Renderer *renderer) {
        drawRect(renderer, color, rect);
        circle->setPosition(rect->x + (value * rect->w), rect->y);
        circle->draw(renderer, 0, 0);
    }

    int pressEvent() {
        if (circle->isMouseOver(mX, mY)) {
            return 1;
        }

        return 0;
    }

    void dragEvent() {
        value = mX;
    }

} SDL_MySlider;


#endif