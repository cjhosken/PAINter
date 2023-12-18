#ifndef PNTR_BUTTON_H
#define PNTR_BUTTON_H

#include "common.h"
#include "pntr_panel.h"
#include <functional>

// PNTR_Button derives from PNTR_Panel.
// it holds an optional icon, runnable action (for pressEvents), and active variable incase for toggleable functionality.
class PNTR_Button : public PNTR_Panel
{
    private:
        SDL_Surface *icon;
        bool active;
        std::function<void()> action;

    public:
        PNTR_Button();
        PNTR_Button(SDL_Rect* bb);
        PNTR_Button(SDL_Rect* bb, SDL_Surface* i);
        PNTR_Button(SDL_Rect* bb, SDL_Surface* i, std::function<void()> func);

        void setAction(std::function<void()> func);
        void executeAction();

        void draw(SDL_Renderer *renderer, SDL_Event* event, SDL_Window* window);

        void setIcon(SDL_Surface *s);

        void setActive(bool a);

        bool isActive();

        void pressEvent() override;
};

#endif
