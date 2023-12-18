#ifndef PNTR_COLORDIALOG_H
#define PNTR_COLORDIALOG_H

#include "../include/common.h"
#include "../include/pntr_slider.h"
#include "../include/pntr_panel.h"
#include <vector>

// Like the PNTR_Window, the colorDialog is specific to this application. It would be nice to get a scalable class working, but there simply isn't enough time.
class PNTR_ColorDialog {
    public:
        PNTR_Slider* rSlider;
        PNTR_Slider* gSlider;
        PNTR_Slider* bSlider;
        PNTR_Panel* colorView;
        SDL_Event* event;

        SDL_Window* dialog;

        vector<PNTR_Slider*> sliders;
        SDL_Renderer* renderer;
        bool invoked;

    public:
        PNTR_ColorDialog(SDL_Event* e);
        

        bool isInvoked();
        void show();
        void draw();
        void hide();
};

#endif