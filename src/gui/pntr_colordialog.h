#ifndef PNTR_COLORDIALOG_H
#define PNTR_COLORDIALOG_H

#include "../../common.h"
#include "pntr_slider.h"
#include "pntr_panel.h"

class PNTR_ColorDialog {
    public:
        SDL_Renderer* renderer;

        PNTR_Slider* rSlider;
        PNTR_Slider* gSlider;
        PNTR_Slider* bSlider;

        SDL_Color* color;

        SDL_Window* dialog;

        PNTR_Slider *sliders[3];

        bool invoked;
    
    public:
        PNTR_ColorDialog();
        PNTR_ColorDialog(SDL_Color* c);

        void init();

        void invoke();
        void draw();

        bool isInvoked();
        void update();

        void close();
};



#endif