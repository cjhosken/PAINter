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
SDL_MySlider* sliderActive = nullptr;

int run();
void dispose();
void minimize();

void setModeDraw();
void setModeErase();
void setModeFill();
void setModeShape();
void setPickerMode();
void pickColor();
void openColorWheel();

// https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        //printf("error: %s \n", SDL_GetError());
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            //printf("error: %s \n", SDL_GetError());
            return 1;
        }
    }
    // FROM PRESENTATION

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

    window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
    icon = IMG_Load("assets/images/ross.jpg");
    SDL_SetWindowIcon(window, icon);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    gui.init();

    gui.closeButton->setAction(dispose);
    gui.minimizeButton->setAction(minimize);

    gui.colorsButton->setAction(openColorWheel);

    return run();
}

int run()
{
    running = true;

    bool hold = false;
    bool drag = false;
    bool buttonPressed = false;
    int rX, rY, lX, lY, uX, uY;

    SDL_Surface *cursorSurface;

    while (running)
    {
        SDL_WaitEvent(&event);
        if (SDL_GetWindowID(window) == event.window.windowID)
        {
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
                        if (b->pressEvent())
                        {
                            drag = false;
                            buttonPressed = true;
                            break;
                        }
                    }

                    if (gui.thickSlider->pressEvent())
                    {
                        hold = true;
                        sliderActive = gui.thickSlider;
                        lX = mX;
                        lY = mY;
                        break;
                    }

                    if (!buttonPressed)
                    {
                        if (mY < 64)
                        {
                            SDL_GetWindowPosition(window, &rX, &rY);
                            SDL_GetGlobalMouseState(&lX, &lY);
                            drag = true;
                        }

                        else
                        {
                            switch (editMode)
                            {

                            case Mode::DRAW:
                                break;

                            case Mode::ERASE:
                                break;

                            case Mode::FILL:
                                break;

                            case Mode::SHAPE:
                                break;
                            }
                        }
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
                case SDLK_p:
                    editMode = Mode::PICKER;
                    break;
                default:
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
                buttonPressed = false;
                sliderActive = nullptr;
                break;

            case SDL_QUIT:
                running = false;
                break;

            default:
                break;
            }
        } else {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    gui.dialog->close();
                    break;
            }

            switch (event.type) {
                case SDL_MOUSEMOTION:
                    mX = event.motion.x;
                    mY = event.motion.y;
                    break;
                
                case SDL_BUTTON_LEFT:
                    buttonPressed = false;
                        for (SDL_MySlider *s : gui.dialog->sliders)
                        {
                            if (s->pressEvent())
                            {
                                hold = true;
                                sliderActive = s;
                                lX = mX;
                                lY = mY;
                                break;
                            }
                        }
            }
        }

        if (hold)
        {
            if (sliderActive != nullptr)
            {

                sliderActive->setValue((mX - sliderActive->rect->x) / (float) sliderActive->rect->w);
            }
            else
            {
                gui.canvas->rect->x = rX + mX - lX;
                gui.canvas->rect->y = rY + mY - lY;
            }
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
                cursorSurface = IMG_Load("assets/icons/circle_48.png");
                break;
            case ERASE:
                gui.eraserButton->setActive(true);
                cursorSurface = IMG_Load("assets/icons/circle_48.png");
                break;
            case FILL:
                gui.fillButton->setActive(true);
                cursorSurface = IMG_Load("assets/icons/fill_48.png");
                break;
            case SHAPE:
                gui.shapeButton->setActive(true);
                cursorSurface = IMG_Load("assets/icons/circle_48.png");
                break;
            case PICKER:
                gui.pickerButton->setActive(true);
                cursorSurface = IMG_Load("assets/icons/picker_48.png");
            }

            SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
            SDL_RenderClear(renderer);

            gui.draw(renderer);

            SDL_RenderPresent(renderer);

            SDL_Cursor *cursor = SDL_CreateColorCursor(cursorSurface, 24, 24);
            SDL_SetCursor(cursor);
            SDL_FreeSurface(cursorSurface);
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
    IMG_Quit();
    exit(0);
};

void minimize()
{
    SDL_MinimizeWindow(window);
}

void pickColor()
{

    SDL_Color pixelColor = {0, 0, 0, 255};
    Uint32 pixels[10];

    SDL_Rect pickerRect;
    pickerRect.x = mX;
    pickerRect.y = mY;
    pickerRect.w = 1;
    pickerRect.h = 1;

    SDL_Surface *s = SDL_CreateRGBSurface(0, 5, 5, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_Surface *ns = SDL_ConvertSurfaceFormat(s, SDL_PIXELFORMAT_ARGB8888, 0);

    if (!SDL_RenderReadPixels(renderer, &pickerRect, SDL_PIXELFORMAT_ARGB8888, pixels, 5))
    {
        SDL_GetRGB(pixels[0], ns->format, &(pixelColor.r), &(pixelColor.g), &(pixelColor.b));
        // printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", mX, mY, pixelColor.r, pixelColor.g, pixelColor.b, 255);

        activeColor = new SDL_Color({pixelColor.r, pixelColor.g, pixelColor.b, 255});
    }

    SDL_FreeSurface(s);
    SDL_FreeSurface(ns);

    // FROM PRESENTATION
}

void openColorWheel()
{
    gui.dialog->invoke();
}
// end

// Copyright © 2023 Christopher Hosken