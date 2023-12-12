#ifndef PNTR_COLORDIALOG_H
#define PNTR_COLORDIALOG_H

#include "../include/common.h"
#include "../include/pntr_slider.h"
#include "../include/pntr_panel.h"
#include <vector>

class PNTR_ColorDialog {
    private:
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
        void processEvents(bool* leftMouseDown, bool* middleMouseDown, bool* buttonPressed, PNTR_Slider* activeSlider, PNTR_Vector2I* lastPos);
        void hide();
};

#endif