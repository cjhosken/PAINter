#include "common.h"
#include "src/gui/gui.h"

const int width = 1280, height = 720;
SDL_Event event;
SDL_MouseButtonEvent *mouseEvent;
SDL_Surface *icon;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Gui gui;
char running;

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

    gui.closeButton.setAction(dispose);
    gui.minimizeButton.setAction(minimize);

    return run();
}

int run()
{
    running = 1;

    int lX, lY;
    int offsetX, offsetY;

    while (running)
    {
        int mX, mY;
        SDL_GetMouseState(&mX, &mY);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            running = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    for (SDL_Button *b : gui.buttons)
                    {
                        if (isMouseOver(b, mX, mY))
                        {
                            b->action();
                        }
                    }
                    break;
                case SDL_BUTTON_MIDDLE:
                    offsetX = mX - lX; 
                    offsetY = mY - lY;

                    gui.canvas.rect.x += offsetX;
                    gui.canvas.rect.y += offsetY;

                    
            }

            break;

        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0)
            {
                gui.canvas.rect.w *= 1.1;
                gui.canvas.rect.h *= 1.1;
            }

            if (event.wheel.y < 0)
            {
                gui.canvas.rect.w *= 0.9;
                gui.canvas.rect.h *= 0.9;
            }
            gui.canvas.rect.x = (width - gui.canvas.rect.w) / 2 + offsetX;
            gui.canvas.rect.y = (height - gui.canvas.rect.h) / 2 + offsetY;
            break;
        }

        gui.draw(renderer);

        SDL_RenderPresent(renderer);
        lX = mX;
        lY = mY;
    }

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