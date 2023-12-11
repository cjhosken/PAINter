#ifndef PNTR_PANEL_H
#define PNTR_PANEL_H

#include "../../common.h"
#include "pntr_widget.h"

class PNTR_Panel : public PNTR_Widget {
    protected:
        int radius;

    public: 
        PNTR_Panel();
        PNTR_Panel(SDL_Rect* bb);
        PNTR_Panel(SDL_Rect* bb, SDL_Color* c);
        PNTR_Panel(SDL_Rect* bb, SDL_Color* c, int r);

        void setRadius(int r);

        int getRadius();

        void draw(SDL_Renderer* renderer) override;

        static void renderPanel(SDL_Renderer *renderer, SDL_Rect bbox, SDL_Color color, int radius);
};


#endif