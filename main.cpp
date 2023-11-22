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
void pickerMode();
void pickColor();

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

    gui.pickerButton->setAction(pickerMode);

    SDL_Surface* cursorSurface = IMG_Load("assets/icons/picker_48.png");
    SDL_Cursor* cursor = SDL_CreateColorCursor(cursorSurface, 0, 0);
    //SDL_SetCursor(cursor);

    return run();
}

int run()
{
    running = true;

    bool hold = false;
    bool drag = false;
    bool buttonPressed = false;
    int rX, rY, lX, lY, uX, uY;

    while (running)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                if (editMode == Mode::PICKER)
                {
                    pickColor();
                    editMode = Mode::DRAW;
                    break;
                }
                buttonPressed = false;
                for (SDL_MyButton *b : gui.buttons)
                {
                    if (b->mouseOver(mX, mY))
                    {
                        b->action();
                        drag = false;
                        buttonPressed = true;
                        break;
                    }
                }

                if (!buttonPressed && mY < 64)
                {
                    SDL_GetWindowPosition(window, &rX, &rY);
                    SDL_GetGlobalMouseState(&lX, &lY);
                    drag = true;
                }

                break;

            case SDL_BUTTON_MIDDLE:
                if (mY < 64)
                    break;
                rX = gui.canvas->rect->x;
                rY = gui.canvas->rect->y;
                lX = mX;
                lY = mY;
                hold = true;
                break;
            }
            break;

        case SDL_MOUSEWHEEL:
            switch (event.wheel.type)
            {
            case SDL_MOUSEWHEEL:
                if (mY < 64)
                    break;
                float s = 75;
                float ar = (float)gui.canvas->rect->h / gui.canvas->rect->w;

                float sX = s * (gui.canvas->rect->x - mX) / (float)gui.canvas->rect->w;
                float sY = s * (gui.canvas->rect->y - mY) / (float)gui.canvas->rect->h;

                if (event.wheel.y > 0)
                {
                    gui.canvas->rect->x += sX;
                    gui.canvas->rect->y += sY;
                    gui.canvas->rect->w += s;
                    gui.canvas->rect->h += s * ar;
                }

                if (event.wheel.y < 0 && gui.canvas->rect->h > 75)
                {
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

        if (hold)
        {
            gui.canvas->rect->x = rX + mX - lX;
            gui.canvas->rect->y = rY + mY - lY;
        }

        if (drag)
        {
            SDL_SetWindowPosition(window, rX + uX - lX, rY + uY - lY);
        }

        else
        {
            for (SDL_MyButton *b : gui.buttons)
            {
                b->setActive(false);
            }
            switch (editMode)
            {
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

            SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
            SDL_RenderClear(renderer);
            gui.draw(renderer);

            SDL_RenderPresent(renderer);
        }
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

void drawMode()
{
    editMode = Mode::DRAW;
}

void eraseMode()
{
    editMode = Mode::ERASE;
}

void fillMode()
{
    editMode = Mode::FILL;
}

void shapeMode()
{
    editMode = Mode::SHAPE;
}

void pickerMode()
{
    editMode = Mode::PICKER;
}

void pickColor()
{
    SDL_Surface *getPixel_Surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, getPixel_Surface->pixels, getPixel_Surface->pitch);
    const Uint8 getPixel_bpp = getPixel_Surface->format->BytesPerPixel;

        SDL_Color pixelColor;
        Uint8 *pPixel = (Uint8 *)getPixel_Surface->pixels + mY * getPixel_Surface->pitch + mX * getPixel_bpp;

        Uint32 pixelData;

        switch (getPixel_bpp)
        {
        case 1:
            pixelData = *pPixel;
            break;
        case 2:
            pixelData = *(Uint16 *)pPixel;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                pixelData = pPixel[0] << 16 | pPixel[1] << 8 | pPixel[2];
            else
                pixelData = pPixel[0] | pPixel[1] << 8 | pPixel[2] << 16;
            break;
        case 4:
            pixelData = *(Uint32 *)pPixel;
            break;
        }

        SDL_GetRGBA(pixelData, getPixel_Surface->format, &pixelColor.r, &pixelColor.g, &pixelColor.b, &pixelColor.a);

        printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", mX, mY, pixelColor.r, pixelColor.g, pixelColor.b, 255);

        activeColor = new SDL_Color({pixelColor.r, pixelColor.g, pixelColor.b, 255});

        // https://www.reddit.com/r/sdl/comments/tzrw7m/getting_specific_pixel_color_from_render/
    }
    // end

    // Copyright © 2023 Christopher Hosken