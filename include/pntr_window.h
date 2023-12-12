#ifndef PNTR_WINDOW_H
#define PNTR_WINDOW_H

#include "common.h"
#include "pntr_vector2i.h"
#include "pntr_gui.h"
#include "pntr_slider.h"

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

        bool middleMouseDown = false;
        bool leftMouseDown = false;
        bool buttonPressed = false;

        PNTR_Vector2I* lastPos;
        PNTR_Vector2I* shapeStart;

    public:
        PNTR_Window();

        void run();

        void draw();

        void processEvents();

        void pickColor();

        void dispose();

        void saveLayersAction();
        void clearPaintLayerAction();
};

#endif