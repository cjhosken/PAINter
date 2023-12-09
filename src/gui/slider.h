#ifndef SLIDER_H
#define SLIDER_H

#include "../../common.h"
#include "circle.h"
#include "rect.h"

typedef struct PNTR_Slider {
    float value = 0.5f;
    SDL_Rect* rect = new SDL_Rect();
    PNTR_Circle* circle = new PNTR_Circle(PNTR_Vector2D(), 8, new SDL_Color({0, 0, 0, 255}));
    SDL_Color* color = new SDL_Color({0, 0, 0, 255});

    void setRect(SDL_Rect* r) {
        rect = r;
    }

    void setCircle(PNTR_Circle* c) {
        circle = c;
    }

    void setColor(SDL_Color* c) {
        color = c;
    }

    void setValue(float f) {
        value = SDL_min(SDL_max(0, f), 1);
    }

    void draw(SDL_Renderer *renderer) {
        renderRect(renderer, color, rect, 0);
        circle->setPosition(PNTR_Vector2D(rect->x + (value * rect->w), rect->y));
        circle->draw(renderer, PNTR_Vector2D(0, 0));
    }

    int pressEvent() {
        if (circle->isMouseOver(mousePos)) {
            return 1;
        }

        return 0;
    }

    void dragEvent() {
        value = mousePos.x;
    }

} PNTR_Slider;


#endif