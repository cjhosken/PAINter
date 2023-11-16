#ifndef APPLICATION_H
#define APPLICATON_H

#include "../common.h"
#include "gui/gui.h"

typedef struct {
    int width, height;
    SDL_Event event;
    SDL_Surface *icon;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Gui gui;
    char running;

    bool init(const char* title, int w, int h) {
        width = w;
        height = h;
        SDL_Init(SDL_INIT_VIDEO);
        icon = IMG_Load("assets/images/ross.jpg");
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS); //SDL_WINDOW_BORDERLESS
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        gui.init();
        
        SDL_SetWindowIcon(window, icon);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        return true;
    }

    int run() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        
        gui.draw(renderer);

        while (running) {

            SDL_WaitEvent(&event);

            switch(event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                    int mX, mY;
                    SDL_GetMouseState(&mX, &mY);
                    for (SDL_Button* b : gui.buttons) {
                        if (isMouseOver(b, mX, mY)) {
                            b->action();
                        }
                    }
                    break;
            }
            SDL_RenderPresent(renderer);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return running;
    }

} SDL_Application;

#endif