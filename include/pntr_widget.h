#ifndef PNTR_WIDGET_H
#define PNTR_WIDGET_H

#include "common.h"
#include "pntr_vector2i.h"

// PNTR_Widget is a class that most of the other UI elements derive from. 
// It has a base BBox and color, as well as an overridable mouseOver, pressEvent, and draw function.
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