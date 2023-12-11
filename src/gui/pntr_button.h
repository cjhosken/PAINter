#ifndef PNTR_BUTTON_H
#define PNTR_BUTTON_H

#include "../../common.h"
#include "pntr_panel.h"

class PNTR_Button : public PNTR_Panel
{
    private:
        SDL_Surface *icon;
        bool active;
        void (*action)(void);

    public:
        PNTR_Button();
        PNTR_Button(SDL_Rect* bb);
        PNTR_Button(SDL_Rect* bb, SDL_Surface* i);
        PNTR_Button(SDL_Rect* bb, SDL_Surface* i, void (*func)(void));

        void setAction(void (*func)(void));
        void executeAction();

        void draw(SDL_Renderer *renderer) override;

        void setIcon(SDL_Surface *s);

        void setActive(bool a);

        bool isActive();
};

#endif