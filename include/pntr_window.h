#ifndef PNTR_WINDOW_H
#define PNTR_WINDOW_H

#include "common.h"
#include "pntr_vector2i.h"
#include "pntr_gui.h"
#include "pntr_slider.h"
#include "../include/pntr_colordialog.h"

// The PNTR_Window is not very scalable as I've designed most of its code to be specific for this project. However it can be fixed for use as multiple windows containing different UIs.

class PNTR_Window {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event* event;
        SDL_Surface* icon;
        PNTR_Vector2I size = PNTR_Vector2I(1280, 720);
        PNTR_Vector2I* lastCanvasPos;
        bool running;
        PNTR_Gui* gui = nullptr;
        PNTR_Slider* activeSlider = nullptr;

        PNTR_Vector2I* shapeStart;

        PNTR_ColorDialog* dialog;

    public:
        PNTR_Window();

        void run();

        void draw();

        void processEvents();

        void pickColor();

        void dispose();

        void saveLayersAction();
        void clearPaintLayerAction();
        void openColorDialog();
};

#endif