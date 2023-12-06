#include "common.h"
#include "src/gui/gui.h"

const int width = 1280, height = 720;
SDL_MouseButtonEvent *mouseEvent;
SDL_Surface *icon;
SDL_Renderer *renderer;
SDL_Gui gui;
bool running;
SDL_MySlider *sliderActive = nullptr;
int drawSizeFac = 64;

char* readFilePath = NULL;
char* writeFilePath = NULL;

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
void drawOnCanvas();
void saveImage();
void clearImage();

// https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
int main(int argc, char **argv)
{
    // PAINTer -f image.png -o output
    // process command args here
    for (int c  = 0; c <argc;c++) {
        if (strcmp(argv[c], "-f")==0) {
            readFilePath = argv[c+1];
        }

        if (strcmp(argv[c], "-o") == 0) {
            writeFilePath = argv[c+1];
        }
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        // printf("error: %s \n", SDL_GetError());
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            // printf("error: %s \n", SDL_GetError());
            return 1;
        }
    }
    // FROM PRESENTATION

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

    window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    icon = IMG_Load("assets/images/ross.jpg");
    SDL_SetWindowIcon(window, icon);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    gui.init();

    gui.saveImageButton->setAction(saveImage);
    gui.clearImageButton->setAction(clearImage);

    gui.colorsButton->setAction(openColorWheel);

    if (readFilePath != NULL) {
        gui.canvas->setImage(IMG_Load(readFilePath));
    } else {
        SDL_Surface *newImage = SDL_CreateRGBSurface(0, 1920, 1080, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_FillRect(newImage, NULL, SDL_MapRGBA(newImage->format, 255, 255, 255, 255));
        gui.canvas->setImage(newImage);
    }
    gui.canvas->setRect(0, 0, gui.canvas->image->w, gui.canvas->image->h);

    return run();
}

int run()
{
    running = true;

    bool hold = false;
    bool buttonPressed = false;
    bool down = false;
    int rX, rY, lX, lY, uX, uY;
    SDL_Surface *cursorSurface;

    while (running)
    {
        SDL_WaitEvent(&event);
        if (SDL_GetWindowID(window) == event.window.windowID)
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                running = 0;
                break;
            }
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    down = true;
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
                        drawOnCanvas();
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
                if (down && !hold)
                {
                    drawOnCanvas();
                }
                break;

            case SDL_MOUSEBUTTONUP:
                down = false;
                hold = false;
                buttonPressed = false;
                sliderActive = nullptr;
                break;

            case SDL_QUIT:
                running = false;
                break;

            default:
                break;
            }
        }
        else
        {
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                gui.dialog->close();
                break;

            default:
                break;
            }

            switch (event.type)
            {
            case SDL_MOUSEMOTION:
                mX = event.motion.x;
                mY = event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {

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
                    break;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                down = false;
                hold = false;
                buttonPressed = false;
                sliderActive = nullptr;
                break;
            }
        }

        if (hold)
        {
            if (sliderActive != nullptr)
            {
                sliderActive->setValue((mX - sliderActive->rect->x) / (float)sliderActive->rect->w);
            }
            else
            {
                gui.canvas->rect->x = rX + mX - lX;
                gui.canvas->rect->y = rY + mY - lY;
            }
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
        }

        int drawSize = drawSizeFac * gui.thickSlider->value * (float) (gui.canvas->rect->w / 1280.0f) * 2;
        SDL_Surface* scaledSurface = SDL_CreateRGBSurface(0, drawSize, drawSize, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_FillRect(scaledSurface, NULL, SDL_MapRGBA(scaledSurface->format, 0, 0, 0, 0));
        SDL_BlitScaled(cursorSurface, NULL, scaledSurface, new SDL_Rect({0, 0, drawSize, drawSize}));

        SDL_Cursor *cursor = SDL_CreateColorCursor(scaledSurface, drawSize/2, drawSize/2);
        SDL_SetCursor(cursor);

        SDL_FreeSurface(scaledSurface);

        SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
        SDL_RenderClear(renderer);
        

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

void drawOnCanvas()
{
    if (mY <= 64) return;
    if ((mY > 80 && mY < 80 + 280) && (mX > 16 && mX < 16 + 64)) return;
    int cX, cY;
    int drawSize = drawSizeFac * gui.thickSlider->value;

    // map from draw shape to image
    // cX = ((mX - rX) / rW) * iW
    // cY = ((mY - rY) / rH) * iH

    cX = ((mX - gui.canvas->rect->x) / (float) gui.canvas->rect->w) * gui.canvas->image->w - (drawSize);
    cY = ((mY - gui.canvas->rect->y) / (float) gui.canvas->rect->h) * gui.canvas->image->h - (drawSize);

    switch (editMode)
    {
    case Mode::DRAW:
        SDL_BlitSurface(circle(drawSize, activeColor), NULL, gui.canvas->overlay, new SDL_Rect({cX, cY, drawSize, drawSize}));
        break;

    case Mode::ERASE:
        SDL_BlitSurface(circle(drawSize, new SDL_Color({255, 255, 255, 255})), NULL, gui.canvas->overlay, new SDL_Rect({cX, cY, drawSize, drawSize}));
        break;

    case Mode::FILL:
        break;

    case Mode::SHAPE:
        break;

    default:
        break;
    }
}

void clearImage() {
    SDL_Surface *empty = SDL_CreateRGBSurface(0, gui.canvas->image->w, gui.canvas->image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_FillRect(empty, NULL, SDL_MapRGBA(empty->format, 0, 0, 0, 0));
    gui.canvas->overlay = empty;
}

void saveImage() {
    SDL_Surface *output = SDL_CreateRGBSurface(0, gui.canvas->image->w, gui.canvas->image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(gui.canvas->image, NULL, output, NULL);
    SDL_BlitSurface(gui.canvas->overlay, NULL, output, NULL);

    if (writeFilePath != NULL) {
        IMG_SavePNG(output, writeFilePath);
    } else {
        IMG_SavePNG(output, "./output.png");
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File Saved!", "Your image has been saved!", NULL);

    SDL_FreeSurface(output);
}


//BRESENHAM LINE ALGOIRTHM (SEE PRESENTATION)


// end

// Copyright © 2023 Christopher Hosken