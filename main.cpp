#include "common.h"
#include "src/gui/gui.h"
#include <vector>

const int width = 1280, height = 720;
SDL_MouseButtonEvent *mouseEvent;
SDL_Surface *icon;
SDL_Gui gui;
bool running;
SDL_MySlider *sliderActive = nullptr;
int drawSizeFac = 64;

int rX, rY;

char *readFilePath = NULL;
char *writeFilePath = NULL;

vector<vector<bool>> visit_array = {};

vector<SDL_MyPosition> fillStack;

SDL_MyPosition shapeStart;

bool hold = false;
bool down = false;

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
    for (int c = 0; c < argc; c++)
    {
        if (strcmp(argv[c], "-f") == 0)
        {
            readFilePath = argv[c + 1];
        }

        if (strcmp(argv[c], "-o") == 0)
        {
            writeFilePath = argv[c + 1];
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

    if (readFilePath != NULL)
    {
        gui.canvas->setImage(IMG_Load(readFilePath));
    }
    else
    {
        SDL_Surface *newImage = SDL_CreateRGBSurface(0, 512, 512, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_FillRect(newImage, NULL, SDL_MapRGBA(newImage->format, 255, 255, 255, 255));
        gui.canvas->setImage(newImage);
    }
    gui.canvas->setRect(0, 0, gui.canvas->image->w, gui.canvas->image->h);

    return run();
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_RenderClear(renderer);

    gui.draw(renderer);
    SDL_RenderPresent(renderer);
    SDL_FillRect(gui.canvas->ghosting, NULL, SDL_MapRGBA(gui.canvas->ghosting->format, 255, 255, 255, 0));
}

int run()
{
    running = true;

    bool buttonPressed = false;
    int lX, lY, uX, uY;
    SDL_Surface *cursorSurface;
    int drawSize;

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
                    drawSizeFac *gui.thickSlider->value;
                    shapeStart.x = (int)(((mX - gui.canvas->rect->x) / (float)gui.canvas->rect->w) * gui.canvas->image->w);
                    shapeStart.y = (int)(((mY - gui.canvas->rect->y) / (float)gui.canvas->rect->h) * gui.canvas->image->h);
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
                case SDLK_l:
                    editMode = Mode::SHAPE_LINE;
                    break;
                case SDLK_c:
                    editMode = Mode::SHAPE_CIRCLE;
                    break;
                case SDLK_s:
                    editMode = Mode::SHAPE_SQUARE;
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
                buttonPressed = false;
                sliderActive = nullptr;
                if (editMode == Mode::SHAPE_CIRCLE || editMode == Mode::SHAPE_LINE || editMode == Mode::SHAPE_SQUARE)
                {
                    drawOnCanvas();
                }
                hold = false;
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
            case SHAPE_LINE:
                gui.shapeButtonLine->setActive(true);
                cursorSurface = IMG_Load("assets/icons/circle_48.png");
                break;
            case SHAPE_CIRCLE:
                gui.shapeButtonCircle->setActive(true);
                cursorSurface = IMG_Load("assets/icons/circle_48.png");
                break;
            case SHAPE_SQUARE:
                gui.shapeButtonSquare->setActive(true);
                cursorSurface = IMG_Load("assets/icons/circle_48.png");
                break;
            case PICKER:
                gui.pickerButton->setActive(true);
                cursorSurface = IMG_Load("assets/icons/picker_48.png");
            }
        }

        int drawSize = drawSizeFac * gui.thickSlider->value * (float)(gui.canvas->rect->w / 1280.0f) * 2;
        SDL_Surface *scaledSurface = SDL_CreateRGBSurface(0, drawSize, drawSize, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        SDL_FillRect(scaledSurface, NULL, SDL_MapRGBA(scaledSurface->format, 0, 0, 0, 0));
        SDL_BlitScaled(cursorSurface, NULL, scaledSurface, new SDL_Rect({0, 0, drawSize, drawSize}));

        SDL_Cursor *cursor = SDL_CreateColorCursor(scaledSurface, drawSize / 2, drawSize / 2);
        SDL_SetCursor(cursor);

        SDL_FreeSurface(scaledSurface);

        draw();
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
    activeColor = getPixel(NULL, mX, mY);

    // FROM PRESENTATION
}

void openColorWheel()
{
    gui.dialog->invoke();
}

bool isValid(int x, int y, SDL_Surface *read, SDL_Color *fill, SDL_Color *pixel)
{
    return (x >= 0 && x < gui.canvas->image->w) && (y >= 0 && y < gui.canvas->image->h) && !compareColor(fill, pixel) && compareColor(getSurfacePixel(read, x, y), pixel);
}

void floodFill(SDL_MyPosition pos, SDL_Surface *read, SDL_Surface *write, SDL_Color *fill, SDL_Color *pixel)
{
    if (!isValid(pos.x, pos.y, read, fill, pixel))
        return;
    setSurfacePixel(write, fill, pos.x, pos.y);

    visit_array[pos.x][pos.y] = true;
    if (isValid(pos.x + 1, pos.y, read, fill, pixel) && !visit_array[pos.x + 1][pos.y])
    {
        fillStack.push_back(SDL_MyPosition({pos.x + 1, pos.y}));
        visit_array[pos.x + 1][pos.y] = true;
    }
    if (isValid(pos.x - 1, pos.y, read, fill, pixel) && !visit_array[pos.x - 1][pos.y])
    {
        fillStack.push_back(SDL_MyPosition({pos.x - 1, pos.y}));
        visit_array[pos.x - 1][pos.y] = true;
    }
    if (isValid(pos.x, pos.y + 1, read, fill, pixel) && !visit_array[pos.x][pos.y + 1])
    {
        fillStack.push_back(SDL_MyPosition({pos.x, pos.y + 1}));
        visit_array[pos.x][pos.y + 1] = true;
    }
    if (isValid(pos.x, pos.y - 1, read, fill, pixel) && !visit_array[pos.x][pos.y - 1])
    {
        fillStack.push_back(SDL_MyPosition({pos.x, pos.y - 1}));
        visit_array[pos.x][pos.y - 1] = true;
    }
}

void line(SDL_Surface *surface, int wdth, int hght, int x0, int y0, int xn, int yn)
{
    int dx = abs(xn - x0), sx = x0 < xn ? 1 : -1;
    int dy = abs(yn - y0), sy = y0 < yn ? 1 : -1;
    int error = (dx > dy ? dx : -dy) / 2, e2;
    while (1)
    {
        /* draw point only if coordinate is valid */
        if (x0 >= 0 && x0 < wdth && y0 >= 0 && y0 < hght)
            setSurfacePixel(surface, activeColor, x0, y0);
        if (x0 == xn && y0 == yn)
            break;
        e2 = error;
        if (e2 > -dx)
        {
            error -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            error += dx;
            y0 += sy;
        }
    }
} // https://brightspace.bournemouth.ac.uk/d2l/le/lessons/345037/topics/1968571

void circle(SDL_Surface *surface, int width, int height, int x0, int y0, int radius)
{
    int x = radius, y = 0, error = 1 - radius;
    while (x >= y)
    { /* 1 draw call for each octant - ensure coordinates are valid before drawing */
        if ((x + x0) >= 0 && (x + x0) < width && (y + y0) >= 0 && (y + y0) < height)
            setSurfacePixel(surface, activeColor, x + x0, y + y0);
        /* draw point in octant 1 if coordinate is valid */
        if ((y + x0) >= 0 && (y + x0) < width && (x + y0) >= 0 && (x + y0) < height)
            setSurfacePixel(surface, activeColor, y + x0, x + y0);
        /* draw point in octant 2 if coordinate is valid */
        if ((-x + x0) >= 0 && (-x + x0) < width && (y + y0) >= 0 && (y + y0) < height)
            setSurfacePixel(surface, activeColor, -x + x0, y + y0);
        /* draw point in octant 3 if coordinate is valid */
        if ((-y + x0) >= 0 && (-y + x0) < width && (x + y0) >= 0 && (x + y0) < height)
            setSurfacePixel(surface, activeColor, -y + x0, x + y0);
        /* draw point in octant 4 if coordinate is valid */
        if ((-x + x0) >= 0 && (-x + x0) < width && (-y + y0) >= 0 && (-y + y0) < height)
            setSurfacePixel(surface, activeColor, -x + x0, -y + y0);
        /* draw point in octant 5 if coordinate is valid */
        if ((-y + x0) >= 0 && (-y + x0) < width && (-x + y0) >= 0 && (-x + y0) < height)
            setSurfacePixel(surface, activeColor, -y + x0, -x + y0);
        /* draw point in octant 6 if coordinate is valid */
        if ((x + x0) >= 0 && (x + x0) < width && (-y + y0) >= 0 && (-y + y0) < height)
            setSurfacePixel(surface, activeColor, x + x0, -y + y0);
        /* draw point in octant 7 if coordinate is valid */
        if ((y + x0) >= 0 && (y + x0) < width && (-x + y0) >= 0 && (-x + y0) < height)
            setSurfacePixel(surface, activeColor, y + x0, -x + y0);

        /* draw point in octant 8 if coordinate is valid */
        y++; /* increment y coordinate */
        if (error < 0)
        {
            error += 2 * y + 1;
        }
        else
        {
            x--;
            error += 2 * (y - x) + 1;
        }
    }
}

void square(SDL_Surface *surface, int width, int height, int x0, int y0, int length)
{
    SDL_MyPosition p1 = {x0 - length / 2, y0 - length / 2};
    SDL_MyPosition p2 = {x0 - length / 2, y0 + length / 2};
    SDL_MyPosition p3 = {x0 + length / 2, y0 + length / 2};
    SDL_MyPosition p4 = {x0 + length / 2, y0 - length / 2};
    line(surface, width, height, p1.x, p1.y, p2.x, p2.y);
    line(surface, width, height, p2.x, p2.y, p3.x, p3.y);
    line(surface, width, height, p3.x, p3.y, p4.x, p4.y);
    line(surface, width, height, p4.x, p4.y, p1.x, p1.y);
}

void drawOnCanvas()
{
    if (mY <= 64)
        return;
    if ((mY > 80 && mY < 80 + 280) && (mX > 16 && mX < 16 + 64))
        return;
    int cX, cY, i;
    int drawSize = drawSizeFac * gui.thickSlider->value;
    bool l;
    SDL_MyPosition currentPos;

    // map from draw shape to image
    // cX = ((mX - rX) / rW) * iW
    // cY = ((mY - rY) / rH) * iH

    cX = ((mX - gui.canvas->rect->x) / (float)gui.canvas->rect->w) * gui.canvas->image->w - (drawSize);
    cY = ((mY - gui.canvas->rect->y) / (float)gui.canvas->rect->h) * gui.canvas->image->h - (drawSize);

    int shapeSize = sqrt(pow(shapeStart.x - cX, 2) + pow(shapeStart.y - cY, 2));

    SDL_Surface *output = SDL_CreateRGBSurface(0, gui.canvas->image->w, gui.canvas->image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(gui.canvas->image, NULL, output, NULL);
    SDL_BlitSurface(gui.canvas->overlay, NULL, output, NULL);

    SDL_Color *pixel;

    switch (editMode)
    {
    case Mode::DRAW:
        SDL_BlitSurface(circle(drawSize, activeColor, 0, 0), NULL, gui.canvas->overlay, new SDL_Rect({cX, cY, drawSize, drawSize}));
        break;

    case Mode::ERASE:
        SDL_BlitSurface(circle(drawSize, new SDL_Color({255, 255, 255, 255}), 0, 0), NULL, gui.canvas->overlay, new SDL_Rect({cX, cY, drawSize, drawSize}));
        break;

    case Mode::FILL:
        if (cX < 0 || cX > gui.canvas->image->w)
            break;
        if (cY < 0 || cY > gui.canvas->image->h)
            break;

        pixel = getSurfacePixel(output, cX, cY);
        fillStack.clear();
        fillStack.push_back(SDL_MyPosition({cX, cY}));

        visit_array.resize(gui.canvas->image->w, std::vector<bool>(gui.canvas->image->h, false));
        while (!fillStack.empty())
        {
            currentPos = fillStack.front();
            fillStack.erase(fillStack.begin());
            fillStack.shrink_to_fit();
            floodFill(currentPos, output, gui.canvas->overlay, activeColor, pixel);
        }

        fillStack.clear();
        fillStack.shrink_to_fit();
        visit_array.clear();
        visit_array.resize(gui.canvas->image->w, std::vector<bool>(gui.canvas->image->h, false));
        break;

    case Mode::SHAPE_LINE:
        if (!hold)
        {
            if (!down)
            {
                line(gui.canvas->overlay, gui.canvas->image->w, gui.canvas->image->h, shapeStart.x, shapeStart.y, cX + drawSize, cY + drawSize);
            }
            else
            {
                line(gui.canvas->ghosting, gui.canvas->image->w, gui.canvas->image->h, shapeStart.x, shapeStart.y, cX + drawSize, cY + drawSize);
            }
        }
        break;
    case Mode::SHAPE_CIRCLE:
        if (!hold)
        {
            if (!down)
            {
                circle(gui.canvas->overlay, gui.canvas->image->w, gui.canvas->image->h, shapeStart.x, shapeStart.y, shapeSize);
            }
            else
            {
                circle(gui.canvas->ghosting, gui.canvas->image->w, gui.canvas->image->h, shapeStart.x, shapeStart.y, shapeSize);
            }
        }
        break;
    case Mode::SHAPE_SQUARE:
        if (!hold)
        {
            if (!down)
            {
                square(gui.canvas->overlay, gui.canvas->image->w, gui.canvas->image->h, shapeStart.x, shapeStart.y, shapeSize);
            }
            else
            {
                square(gui.canvas->ghosting, gui.canvas->image->w, gui.canvas->image->h, shapeStart.x, shapeStart.y, shapeSize);
            }
        }
        break;
    default:
        break;
    }
    SDL_FreeSurface(output);
}

void clearImage()
{
    SDL_Surface *empty = SDL_CreateRGBSurface(0, gui.canvas->image->w, gui.canvas->image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_FillRect(empty, NULL, SDL_MapRGBA(empty->format, 255, 255, 255, 0));
    gui.canvas->overlay = empty;
}

void saveImage()
{
    SDL_Surface *output = SDL_CreateRGBSurface(0, gui.canvas->image->w, gui.canvas->image->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(gui.canvas->image, NULL, output, NULL);
    SDL_BlitSurface(gui.canvas->overlay, NULL, output, NULL);

    if (writeFilePath != NULL)
    {
        IMG_SavePNG(output, writeFilePath);
    }
    else
    {
        IMG_SavePNG(output, "./output.png");
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File Saved!", "Your image has been saved!", NULL);

    SDL_FreeSurface(output);
}

// BRESENHAM LINE ALGOIRTHM (SEE PRESENTATION)

// end

// Copyright © 2023 Christopher Hosken
