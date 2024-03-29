#ifndef PNTR_CIRCLE_H
#define PNTR_CIRCLE_H

#include "common.h"
#include "pntr_panel.h"
#include "pntr_vector2i.h"

// PNTR_Circle is mainly written for static circle drawing functions. However it's also used as a knob in the slider (which is why it has an isMouseOver function).

class PNTR_Circle : public PNTR_Panel {
    private:
        PNTR_Vector2I* position;

    public:
        PNTR_Circle();

        PNTR_Circle(PNTR_Vector2I* p);

        PNTR_Circle(SDL_Rect* bb);

        PNTR_Circle(SDL_Rect* bb, SDL_Color* c);

        PNTR_Circle(SDL_Rect* bb, SDL_Color* c, int r);
        
        PNTR_Circle(PNTR_Vector2I* p, int r);

        PNTR_Circle(PNTR_Vector2I* p, int r, SDL_Color* c);

        void setPosition(PNTR_Vector2I* p);

        PNTR_Vector2I* getPosition();

        void draw(SDL_Renderer* renderer);

        bool isMouseOver(PNTR_Vector2I* mouse) override;

        static void renderCircle(SDL_Renderer *renderer, PNTR_Vector2I *position, SDL_Color *color, int radius, PNTR_Vector2I *trim);
        static SDL_Surface* circleToSurface(int radius, SDL_Color* color, PNTR_Vector2I* trim);

};

#endif
