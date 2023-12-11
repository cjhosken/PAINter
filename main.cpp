#include "common.h"
#include "src/pntr_vector2i.h"
#include "src/gui/pntr_gui.h"

const int width = 1280, height = 720;
SDL_MouseButtonEvent *mouseEvent;
SDL_Surface *icon;
bool running;
PNTR_Slider *sliderActive = nullptr;
int drawSizeFac = 64;

PNTR_Gui gui;

int rX, rY;


vector<vector<bool>> visit_array = {};

vector<PNTR_Vector2I> fillStack;

PNTR_Vector2I shapeStart;

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
    // PAINTer -f getImageLayer().png -o output
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

    gui = PNTR_Gui();

    gui.saveImageButton->setAction(saveImage);
    gui.clearImageButton->setAction(clearImage);

    gui.colorsButton->setAction(openColorWheel);

    if (readFilePath != NULL)
    {
        gui.canvas = new PNTR_Canvas(new SDL_Rect({0, 0, width, height}), IMG_Load(readFilePath));
    }
    else
    {
        gui.canvas = new PNTR_Canvas(new SDL_Rect({0, 0, width, height}), PNTR_Vector2I(512, 512), new SDL_Color({255, 255, 255, 255}));
    }

    return run();
}

void draw()
{
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_RenderClear(renderer);

    gui.draw(renderer);
    SDL_RenderPresent(renderer);

    gui.canvas->clearGhostLayer();
}

int run()
{
    running = true;

    bool buttonPressed = false;
    PNTR_Vector2I lastPos; 
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
                    drawSizeFac *gui.thickSlider->getValue();
                    shapeStart = PNTR_Vector2I((int)(((mousePos->x - gui.canvas->getBBox()->x) / (float)gui.canvas->getBBox()->w) * gui.canvas->getImageLayer()->w), (int)(((mousePos->y - gui.canvas->getBBox()->y) / (float)gui.canvas->getBBox()->h) * gui.canvas->getImageLayer()->h));
                    
                    if (paintMode == PNTR_PaintMode::PICKER)
                    {
                        pickColor();
                        paintMode = PNTR_PaintMode::DRAW;
                        break;
                    }

                    buttonPressed = false;
                    for (PNTR_Button *b : gui.buttons)
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
                        lastPos = *mousePos;
                        break;
                    }

                    if (!buttonPressed)
                    {
                        drawOnCanvas();
                    }
                    break;

                case SDL_BUTTON_MIDDLE:
                    if (mousePos->y < 64)
                        break;
                    rX = gui.canvas->getBBox()->x;
                    rY = gui.canvas->getBBox()->y;
                    lastPos = *mousePos;
                    hold = true;
                    break;
                }
                break;

            case SDL_MOUSEWHEEL:
                switch (event.wheel.type)
                {
                case SDL_MOUSEWHEEL:
                    if (mousePos->y < 64)
                        break;
                    float scalar = 75;
                    float aspect = (float)gui.canvas->getBBox()->h / gui.canvas->getBBox()->w;

                    PNTR_Vector2I scale =  PNTR_Vector2I(scalar * (gui.canvas->getBBox()->x - mousePos->x) / (float)gui.canvas->getBBox()->w,scalar * (gui.canvas->getBBox()->y - mousePos->y) / (float)gui.canvas->getBBox()->h);

                    if (event.wheel.y > 0)
                    {
                        gui.canvas->getBBox()->x += scale.x;
                        gui.canvas->getBBox()->y += scale.y;
                        gui.canvas->getBBox()->w += scalar;
                        gui.canvas->getBBox()->h += scalar * aspect;
                    }

                    if (event.wheel.y < 0 && gui.canvas->getBBox()->h > 75)
                    {
                        gui.canvas->getBBox()->x -= scale.x;
                        gui.canvas->getBBox()->y -= scale.y;
                        gui.canvas->getBBox()->w -= scalar;
                        gui.canvas->getBBox()->h -= scalar * aspect;
                    }
                    break;
                }

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_h:
                    gui.canvas->getBBox()->x = 0;
                    gui.canvas->getBBox()->y = 0;
                    gui.canvas->getBBox()->w = gui.canvas->getSourceSize()->w;
                    gui.canvas->getBBox()->h = gui.canvas->getSourceSize()->h;
                    break;

                case SDLK_b:
                    paintMode = PNTR_PaintMode::DRAW;
                    break;

                case SDLK_e:
                    paintMode = PNTR_PaintMode::ERASE;
                    break;

                case SDLK_f:
                    paintMode = PNTR_PaintMode::FILL;
                    break;
                case SDLK_l:
                    paintMode = PNTR_PaintMode::SHAPE_LINE;
                    break;
                case SDLK_c:
                    paintMode = PNTR_PaintMode::SHAPE_CIRCLE;
                    break;
                case SDLK_s:
                    paintMode = PNTR_PaintMode::SHAPE_SQUARE;
                    break;
                case SDLK_p:
                    paintMode = PNTR_PaintMode::PICKER;
                    break;
                default:
                    break;
                }
                break;

            case SDL_MOUSEMOTION:
                mousePos = new PNTR_Vector2I(event.motion.x, event.motion.y);
                if (down && !hold)
                {
                    drawOnCanvas();
                }
                break;

            case SDL_MOUSEBUTTONUP:
                down = false;
                buttonPressed = false;
                sliderActive = nullptr;
                if (paintMode == PNTR_PaintMode::SHAPE_CIRCLE || paintMode == PNTR_PaintMode::SHAPE_LINE || paintMode == PNTR_PaintMode::SHAPE_SQUARE)
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
                mousePos = new PNTR_Vector2I(event.motion.x, event.motion.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {

                case SDL_BUTTON_LEFT:
                    buttonPressed = false;
                    for (PNTR_Slider *s : gui.dialog->sliders)
                    {
                        if (s->pressEvent())
                        {
                            hold = true;
                            sliderActive = s;
                            lastPos = *mousePos;
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
                sliderActive->setValue((mousePos->x - sliderActive->getBBox()->x) / (float)sliderActive->getBBox()->w);
            }
            else
            {
                gui.canvas->getBBox()->x = rX + mousePos->x - lastPos.x;
                gui.canvas->getBBox()->y = rY + mousePos->y - lastPos.y;
            }
        }

        else
        {
            for (PNTR_Button *b : gui.buttons)
            {
                b->setActive(false);
            }
            switch (paintMode)
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

        int drawSize = drawSizeFac * gui.thickSlider->getValue() * (float)(gui.canvas->getBBox()->w / 1280.0f) * 4;
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
    activeColor = getPixel(*mousePos);

    // FROM PRESENTATION
}

void openColorWheel()
{
    gui.dialog->invoke();
}

bool isValid(int x, int y, SDL_Surface *read, SDL_Color *fill, SDL_Color *pixel)
{
    return (x >= 0 && x < gui.canvas->getImageLayer()->w) && (y >= 0 && y < gui.canvas->getImageLayer()->h) && !compare(fill, pixel) && compare(getSurfacePixel(read, x, y), pixel);
}

void floodFill(PNTR_Vector2I pos, SDL_Surface *read, SDL_Surface *write, SDL_Color *fill, SDL_Color *pixel)
{
    if (!isValid(pos.x, pos.y, read, fill, pixel))
        return;
    setSurfacePixel(write, fill, pos.x, pos.y);

    visit_array[pos.x][pos.y] = true;
    if (isValid(pos.x + 1, pos.y, read, fill, pixel) && !visit_array[pos.x + 1][pos.y])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x + 1, pos.y));
        visit_array[pos.x + 1][pos.y] = true;
    }
    if (isValid(pos.x - 1, pos.y, read, fill, pixel) && !visit_array[pos.x - 1][pos.y])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x - 1, pos.y));
        visit_array[pos.x - 1][pos.y] = true;
    }
    if (isValid(pos.x, pos.y + 1, read, fill, pixel) && !visit_array[pos.x][pos.y + 1])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x, pos.y + 1));
        visit_array[pos.x][pos.y + 1] = true;
    }
    if (isValid(pos.x, pos.y - 1, read, fill, pixel) && !visit_array[pos.x][pos.y - 1])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x, pos.y - 1));
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

void square(SDL_Surface *surface, int width, int height, int x0, int y0, int length)
{
    PNTR_Vector2I p1 = {x0 - length / 2, y0 - length / 2};
    PNTR_Vector2I p2 = {x0 - length / 2, y0 + length / 2};
    PNTR_Vector2I p3 = {x0 + length / 2, y0 + length / 2};
    PNTR_Vector2I p4 = {x0 + length / 2, y0 - length / 2};
    line(surface, width, height, p1.x, p1.y, p2.x, p2.y);
    line(surface, width, height, p2.x, p2.y, p3.x, p3.y);
    line(surface, width, height, p3.x, p3.y, p4.x, p4.y);
    line(surface, width, height, p4.x, p4.y, p1.x, p1.y);
}

void drawOnCanvas()
{
    if (gui.navBar->isMouseOver(mousePos) || gui.sideBar->isMouseOver(mousePos)) return;

    int cX, cY, i;
    int drawSize = drawSizeFac * gui.thickSlider->getValue();
    bool l;
    PNTR_Vector2I currentPos;

    // map from draw shape to getImageLayer()
    // cX = ((mX - rX) / rW) * iW
    // cY = ((mY - rY) / rH) * iH

    cX = ((mousePos->x - gui.canvas->getBBox()->x) / (float)gui.canvas->getBBox()->w) * gui.canvas->getImageLayer()->w;
    cY = ((mousePos->y - gui.canvas->getBBox()->y) / (float)gui.canvas->getBBox()->h) * gui.canvas->getImageLayer()->h;;

    int shapeSize = sqrt(pow(shapeStart.x - cX, 2) + pow(shapeStart.y - cY, 2));

    SDL_Surface *output = SDL_CreateRGBSurface(0, gui.canvas->getImageLayer()->w, gui.canvas->getImageLayer()->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(gui.canvas->getImageLayer(), NULL, output, NULL);
    SDL_BlitSurface(gui.canvas->getPaintLayer(), NULL, output, NULL);

    SDL_Color *pixel;
    SDL_Rect circleBrushRect = {0, 0, drawSize * 2, drawSize * 2};
    SDL_Surface* circleSurface = SDL_CreateRGBSurfaceWithFormat(0, circleBrushRect.w, circleBrushRect.h, 32, SDL_PIXELFORMAT_RGBA8888);;


    switch (paintMode)
    {
    case PNTR_PaintMode::DRAW:
        PNTR_Circle::circleOnSurface(circleSurface, circleBrushRect, PNTR_Vector2I(), *activeColor, drawSize);
        SDL_BlitSurface(circleSurface, NULL, gui.canvas->getPaintLayer(), new SDL_Rect({cX - drawSize, cY - drawSize, drawSize, drawSize}));
        break;

    case PNTR_PaintMode::ERASE:
        PNTR_Circle::circleOnSurface(circleSurface, circleBrushRect, PNTR_Vector2I(), SDL_Color({255, 255, 255, 0}), drawSize);
        SDL_BlitSurface(circleSurface, NULL, gui.canvas->getPaintLayer(), new SDL_Rect({cX - drawSize, cY - drawSize, drawSize, drawSize}));
        break;

    case PNTR_PaintMode::FILL:
        if (cX < 0 || cX > gui.canvas->getImageLayer()->w)
            break;
        if (cY < 0 || cY > gui.canvas->getImageLayer()->h)
            break;

        pixel = getSurfacePixel(output, cX, cY);
        fillStack.clear();
        fillStack.push_back(PNTR_Vector2I({cX, cY}));

        visit_array.resize(gui.canvas->getImageLayer()->w, std::vector<bool>(gui.canvas->getImageLayer()->h, false));
        while (!fillStack.empty())
        {
            currentPos = fillStack.front();
            fillStack.erase(fillStack.begin());
            fillStack.shrink_to_fit();
            floodFill(currentPos, output, gui.canvas->getPaintLayer(), activeColor, pixel);
        }

        fillStack.clear();
        fillStack.shrink_to_fit();
        visit_array.clear();
        visit_array.resize(gui.canvas->getImageLayer()->w, std::vector<bool>(gui.canvas->getImageLayer()->h, false));
        break;

    case PNTR_PaintMode::SHAPE_LINE:
        if (!hold)
        {
            if (!down)
            {
                line(gui.canvas->getPaintLayer(), gui.canvas->getImageLayer()->w, gui.canvas->getImageLayer()->h, shapeStart.x, shapeStart.y, cX, cY);
            }
            else
            {
                line(gui.canvas->getGhostLayer(), gui.canvas->getImageLayer()->w, gui.canvas->getImageLayer()->h, shapeStart.x, shapeStart.y, cX, cY);
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_CIRCLE:
        if (!hold)
        {
            if (!down)
            {
                PNTR_Circle::circleOnSurface(gui.canvas->getPaintLayer(), *(gui.canvas->getSourceSize()), shapeStart, *activeColor, shapeSize);
            }
            else
            {
                PNTR_Circle::circleOnSurface(gui.canvas->getGhostLayer(), *(gui.canvas->getSourceSize()), shapeStart, *activeColor, shapeSize);
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_SQUARE:
        if (!hold)
        {
            if (!down)
            {
                square(gui.canvas->getPaintLayer(), gui.canvas->getImageLayer()->w, gui.canvas->getImageLayer()->h, shapeStart.x, shapeStart.y, shapeSize);
            }
            else
            {
                square(gui.canvas->getGhostLayer(), gui.canvas->getImageLayer()->w, gui.canvas->getImageLayer()->h, shapeStart.x, shapeStart.y, shapeSize);
            }
        }
        break;
    default:
        break;
    }
    SDL_FreeSurface(output);
}

void saveImage() {
    gui.canvas->saveImage();
};

void clearImage() {
    gui.canvas->clearPaintLayer();
}

// Copyright © 2023 Christopher Hosken
