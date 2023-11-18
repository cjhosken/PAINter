#include <iostream>
#include "common.h"
#include "src/gui/gui.h"

using namespace std;

const int width = 1280, height = 720;
SDL_Event event;
SDL_MouseButtonEvent *mouseEvent;
SDL_Surface *icon;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Gui gui;
bool running;

int run();
void dispose();
void minimize();

// https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    icon = IMG_Load("assets/images/ross.jpg");
    window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS); // SDL_WINDOW_BORDERLESS
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowIcon(window, icon);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    gui.init();

    gui.closeButton->setAction(dispose);
    gui.minimizeButton->setAction(minimize);

    return run();
}

int run()
{
    running = true;

    int mX, mY;
    bool hold = false;
    int rX, rY, lX, lY, offsetX, offsetY;


    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        for (SDL_Button* b : gui.buttons) {
                            if (isMouseOver(b, mX, mY)) {
                                b->action();
                            }
                        }
                        break;
                    
                    case SDL_BUTTON_MIDDLE:
                        rX = gui.canvas->rect->x;
                        rY = gui.canvas->rect->y;
                        lX = mX;
                        lY = mY;
                        hold = true;
                        break;
                }
                break;

            case SDL_MOUSEWHEEL:
                switch (event.wheel.type) {
                    case SDL_MOUSEWHEEL:
                        float s = 25;
                        float ar = (float) gui.canvas->rect->h / gui.canvas->rect->w;

                        float sX = s * (gui.canvas->rect->x - mX) / (float) gui.canvas->rect->w;
                        float sY = s * (gui.canvas->rect->y - mY) / (float) gui.canvas->rect->h;

                        if (event.wheel.y > 0) {
                            gui.canvas->rect->x += sX;
                            gui.canvas->rect->y += sY;
                            gui.canvas->rect->w += s;
                            gui.canvas->rect->h += s * ar;
                        } 

                        if (event.wheel.y < 0) {
                            gui.canvas->rect->x -= sX;
                            gui.canvas->rect->y -= sY;
                            gui.canvas->rect->w -= s;
                            gui.canvas->rect->h -= s * ar;
                        }
                        break;
                }
                        

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) 
                {

                }
                break;

            case SDL_MOUSEMOTION:
                mX = event.motion.x;
                mY = event.motion.y;
                break;

            case SDL_MOUSEBUTTONUP:
                offsetX = 0;
                offsetY = 0;
                hold = false;
                break;

            case SDL_QUIT:
                running = false;
                break;

        }

        if (hold) {
            offsetX = mX - lX;
            offsetY = mY - lY;
            gui.canvas->rect->x = rX + offsetX;
            gui.canvas->rect->y = rY + offsetY;
        }

        gui.draw(renderer);

        SDL_RenderPresent(renderer);
    }
    // https://gigi.nullneuron.net/gigilabs/handling-keyboard-and-mouse-events-in-sdl2/

    dispose();

    return running;
}

void dispose()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(0);
};

void minimize()
{
    SDL_MinimizeWindow(window);
}
// end

// Copyright © 2023 Christopher Hosken