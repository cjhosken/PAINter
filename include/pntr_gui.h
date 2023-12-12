#ifndef PNTR_GUI_H
#define PNTR_GUI_H

#include "common.h"
#include "pntr_button.h"
#include "pntr_slider.h" 
#include "pntr_canvas.h"
#include "pntr_panel.h"

class PNTR_Gui {
    public:
        PNTR_Panel* navBar;
        PNTR_Panel* sideBar;

        PNTR_Canvas* canvas;
        PNTR_Button* iconButton;
        PNTR_Button* saveImageButton;

        PNTR_Button* clearImageButton;

        PNTR_Button* colorsButton;
        PNTR_Button* pickerButton;

        PNTR_Button* brushButton;
        PNTR_Button* eraserButton;
        PNTR_Button* fillButton;

        PNTR_Button* shapeButtonLine;
        PNTR_Button* shapeButtonCircle;
        PNTR_Button* shapeButtonSquare;

        PNTR_Panel* colorView;

        PNTR_Slider* thickSlider;

        vector<PNTR_Button*> buttons[11];

    public:
        PNTR_Gui();

        void draw(SDL_Renderer* renderer, SDL_Event* event, SDL_Window* window);

};




#endif

// Copyright © 2024 Christopher Hosken