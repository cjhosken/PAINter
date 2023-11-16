#include "common.h"
#include "src/gui/gui.h"

const int width = 1280, height = 720;
SDL_Event event;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    gui.draw(renderer);

    while (running)
    {

        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            running = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:
            int mX, mY;
            SDL_GetMouseState(&mX, &mY);
            for (SDL_Button *b : gui.buttons)
            {
                if (isMouseOver(b, mX, mY))
                {
                    b->action();
                }
            }
            break;
        }
        SDL_RenderPresent(renderer);
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

void minimize() {
    SDL_MinimizeWindow(window);
}
// end

// Copyright © 2023 Christopher Hosken