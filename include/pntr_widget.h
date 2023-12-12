#ifndef PNTR_WIDGET_H
#define PNTR_WIDGET_H

#include "common.h"
#include "pntr_vector2i.h"

class PNTR_Widget {
    protected:
        SDL_Rect* bbox;
        SDL_Color* color;
    
    public:
        PNTR_Widget();
        PNTR_Widget(SDL_Rect* bb);
        PNTR_Widget(SDL_Rect* bb, SDL_Color* c);

        void setBBox(SDL_Rect* bb);
        void setBBox(int x, int y, int w, int h);
        SDL_Rect* getBBox();

        void setColor(SDL_Color* c);
        SDL_Color* getColor();
        
        virtual void draw(SDL_Renderer* renderer);

        virtual bool isMouseOver(PNTR_Vector2I* mouse);        

        virtual void pressEvent();
};

#endif

// Copyright © 2024 Christopher Hosken