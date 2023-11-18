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


void drawMode();
void eraseMode();
void fillMode();
void shapeMode();

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

    gui.brushButton->setAction(drawMode);
    gui.eraserButton->setAction(eraseMode);
    gui.fillButton->setAction(fillMode);
    gui.shapeButton->setAction(shapeMode);

    return run();
}

int run()
{
    running = true;

    bool hold = false;
    bool drag = false;
    int rX, rY, lX, lY, uX, uY;


    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        for (SDL_MyButton* b : gui.buttons) {
                            if (b->mouseOver(mX, mY)) {
                                b->action();
                                drag=false;
                                break;
                            }
                        }

                        if (mY < 64) {
                            SDL_GetWindowPosition(window, &rX, &rY);
                            SDL_GetGlobalMouseState(&lX, &lY);
                            drag = true;
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
                    case SDLK_h:
                        gui.canvas->rect->x = 0;
                        gui.canvas->rect->y = 0;
                        gui.canvas->rect->w = gui.canvas->image->w;
                        gui.canvas->rect->h = gui.canvas->image->h;
                        break;
                    
                    case SDLK_b:
                        editMode = Mode::DRAW;
                        break;

                    case SDLK_e:
                        editMode = Mode::ERASE;
                        break;

                    case SDLK_f:
                        editMode = Mode::FILL;
                        break;

                    case SDLK_s:
                        editMode = Mode::SHAPE;
                        break;
                }
                break;

            case SDL_MOUSEMOTION:
                mX = event.motion.x;
                mY = event.motion.y;
                SDL_GetGlobalMouseState(&uX, &uY);
                break;

            case SDL_MOUSEBUTTONUP:
                hold = false;
                drag = false;
                break;

            case SDL_QUIT:
                running = false;
                break;

        }

        if (hold) {
            gui.canvas->rect->x = rX + mX - lX;
            gui.canvas->rect->y = rY + mY- lY;
        }

        if (drag) {
            SDL_SetWindowPosition(window,rX + uX - lX, rY + uY- lY);
        }

        for (SDL_MyButton* b : gui.buttons) {
            b->setActive(false);
        }
        switch (editMode) {
            case DRAW:
                gui.brushButton->setActive(true);
                break;
            case ERASE:
                gui.eraserButton->setActive(true);
                break;
            case FILL:
                gui.fillButton->setActive(true);
                break;
            case SHAPE:
                gui.shapeButton->setActive(true);
                break;
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

void drawMode() {
    editMode = Mode::DRAW;
}

void eraseMode() {
    editMode = Mode::ERASE;
}

void fillMode() {
    editMode = Mode::FILL;
}

void shapeMode() {
    editMode = Mode::SHAPE;
}
// end

// Copyright © 2023 Christopher Hosken