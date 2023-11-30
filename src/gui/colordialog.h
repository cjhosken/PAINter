#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include "../../common.h"
#include "slider.h"
#include "rect.h"

typedef struct SDL_MyColorDialog {
    SDL_MySlider* hSlider = new SDL_MySlider();
    SDL_MySlider* sSlider = new SDL_MySlider();
    SDL_MySlider* vSlider = new SDL_MySlider();

    SDL_Window* dialog = SDL_CreateWindow("Color Picker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);

    SDL_MySlider *sliders[3];
    SDL_Renderer* renderer = SDL_CreateRenderer(dialog, -1, SDL_RENDERER_ACCELERATED);

    bool invoked;
    
    void invoke() {
        SDL_ShowWindow(dialog);

        hSlider->setRect(new SDL_Rect({50, 100, 200, 1}));
        hSlider->setValue(0.5);

        sSlider->setRect(new SDL_Rect({50, 135, 200, 1}));
        sSlider->setValue(1);

        vSlider->setRect(new SDL_Rect({50, 170, 200, 1}));
        vSlider->setValue(0.3);

        sliders[0] = hSlider;
        sliders[1] = sSlider;
        sliders[2] = vSlider;

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
    }

    void close() {
        invoked = false;
        SDL_HideWindow(dialog);
    }
} SDL_MyColorDialog;



#endif