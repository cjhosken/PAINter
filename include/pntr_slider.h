#ifndef PNTR_SLIDER_H
#define PNTR_SLIDER_H

#include "common.h"
#include "pntr_circle.h"
#include "pntr_widget.h"

class PNTR_Slider : public PNTR_Widget {
    private:
        float value;
        PNTR_Circle* knob;

    public:

        PNTR_Slider();
        PNTR_Slider(SDL_Rect* bb);
        PNTR_Slider(SDL_Rect* bb, float v);
        PNTR_Slider(SDL_Rect* bb, SDL_Color* c);
        PNTR_Slider(SDL_Rect* bb, SDL_Color* c, float v);
        PNTR_Slider(SDL_Rect* bb, SDL_Color* c, PNTR_Circle* k);
        PNTR_Slider(SDL_Rect* bb, SDL_Color* c, PNTR_Circle* k, float v);


        void setKnob(PNTR_Circle* c);
        void setValue(float v);
        float getValue();
        PNTR_Circle* getKnob();

        void draw(SDL_Renderer *renderer) override;

        bool isMouseOver(PNTR_Vector2I* mouse) override;

        void pressEvent() override;
};


#endif

// Copyright © 2024 Christopher Hosken