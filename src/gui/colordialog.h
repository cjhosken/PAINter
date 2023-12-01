#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include "../../common.h"
#include "slider.h"
#include "rect.h"

typedef struct SDL_MyColorDialog {
    SDL_MySlider* rSlider = new SDL_MySlider();
    SDL_MySlider* gSlider = new SDL_MySlider();
    SDL_MySlider* bSlider = new SDL_MySlider();

    SDL_Window* dialog = SDL_CreateWindow("PAINter Color", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);

    SDL_MySlider *sliders[3];
    SDL_Renderer* renderer = SDL_CreateRenderer(dialog, -1, SDL_RENDERER_ACCELERATED);

    bool invoked;
    
    void invoke() {
        SDL_ShowWindow(dialog);

        rSlider->setRect(new SDL_Rect({50, 100, 200, 1}));
        rSlider->setValue(activeColor->r / 255.0f);
        rSlider->circle->setColor(new SDL_Color({255, 0, 0, 255}));

        gSlider->setRect(new SDL_Rect({50, 135, 200, 1}));
        gSlider->setValue(activeColor->g / 255.0f);
        gSlider->circle->setColor(new SDL_Color({0, 255, 0, 255}));

        bSlider->setRect(new SDL_Rect({50, 170, 200, 1}));
        bSlider->setValue(activeColor->b / 255.0f);
        bSlider->circle->setColor(new SDL_Color({0, 0, 255, 255}));

        sliders[0] = rSlider;
        sliders[1] = gSlider;
        sliders[2] = bSlider;

        invoked = true;
    }


    void draw() {
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);
        
        drawRect(renderer, activeColor, new SDL_Rect({50, 10, 200, 75}), 5);
        
        for (SDL_MySlider* s : sliders) {
            s->draw(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    void update() {
        // HSV TO RGB CONVERSION HERE
        // SET GLOBAL COLOR TO THAT
        activeColor->a = 255;
        activeColor->r = (int) (255 * rSlider->value);
        activeColor->g = (int) (255 * gSlider->value);
        activeColor->b = (int) (255 * bSlider->value);
    }

    void close() {
        invoked = false;
        SDL_HideWindow(dialog);
    }
} SDL_MyColorDialog;



#endif