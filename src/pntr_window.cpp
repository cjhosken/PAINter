#include "pntr_window.h"
#include "include/common.h"
#include "include/pntr_gui.h"
#include "include/pntr_button.h"

PNTR_Window::PNTR_Window()
{
    window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_SHOWN);
    icon = IMG_Load("assets/images/ross.jpg");
    SDL_SetWindowIcon(window, icon);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    gui = new PNTR_Gui();
}

void PNTR_Window::run()
{
    running = true;
    while (running)
    {
        processEvents();
        draw();
    }
}

void PNTR_Window::draw()
{
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_RenderClear(renderer);

    gui->draw(renderer, &event, window);

    SDL_RenderPresent(renderer);
    gui->canvas->clearGhostLayer();
}

void PNTR_Window::pickColor()
{
    activeColor = getPixel(renderer, *mousePos);
}

void PNTR_Window::processEvents()
{
    SDL_Surface *cursorSurface;
    SDL_WaitEvent(&event);
    if (true)
    {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            running = false;

        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                leftMouseDown = true;
                shapeStart = new PNTR_Vector2I(
                    (int)(((mousePos->x - gui->canvas->getBBox()->x) / (float)gui->canvas->getBBox()->w) * gui->canvas->getImageLayer()->w),
                    (int)(((mousePos->y - gui->canvas->getBBox()->y) / (float)gui->canvas->getBBox()->h) * gui->canvas->getImageLayer()->h));

                if (paintMode == PNTR_PaintMode::PICKER)
                {
                    pickColor();
                    paintMode = PNTR_PaintMode::DRAW;
                    break;
                }

                buttonPressed = false;

                for (PNTR_Button *b : gui->buttons)
                {
                    printf("TEST\n");
                    if (b->isMouseOver(mousePos))
                    {
                        printf("WOOO\n");
                        b->executeAction();
                        b->setActive(true);
                        buttonPressed = true;
                    }
                    break;
                }

                if (gui->thickSlider->pressEvent())
                {
                    middleMouseDown = true;
                    activeSlider = gui->thickSlider;
                    lastPos = mousePos;
                    break;
                }

                if (!buttonPressed)
                {
                    gui->canvas->drawOnPaintLayer(&leftMouseDown, &middleMouseDown, drawSize, *shapeStart);
                }

                break;

            case SDL_BUTTON_MIDDLE:
                if (!gui->navBar->isMouseOver(mousePos))
                {
                    lastCanvasPos = new PNTR_Vector2I(gui->canvas->getBBox()->x, gui->canvas->getBBox()->y);
                    lastPos = mousePos;
                    middleMouseDown = true;
                }
            }
            break;

        case SDL_MOUSEWHEEL:
            if (!gui->navBar->isMouseOver(mousePos))
            {
                float scalar = 75;
                float aspect = (float)gui->canvas->getBBox()->h / gui->canvas->getBBox()->w;

                PNTR_Vector2I scale = PNTR_Vector2I(scalar * (gui->canvas->getBBox()->x - mousePos->x) / (float)gui->canvas->getBBox()->w, scalar * (gui->canvas->getBBox()->y - mousePos->y) / (float)gui->canvas->getBBox()->h);

                if (event.wheel.y > 0)
                {
                    gui->canvas->getBBox()->x += scale.x;
                    gui->canvas->getBBox()->y += scale.y;
                    gui->canvas->getBBox()->w += scalar;
                    gui->canvas->getBBox()->h += scalar * aspect;
                }

                if (event.wheel.y < 0 && gui->canvas->getBBox()->h > 75)
                {
                    gui->canvas->getBBox()->x -= scale.x;
                    gui->canvas->getBBox()->y -= scale.y;
                    gui->canvas->getBBox()->w -= scalar;
                    gui->canvas->getBBox()->h -= scalar * aspect;
                }
            }

            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_h:
                gui->canvas->getBBox()->x = 0;
                gui->canvas->getBBox()->y = 0;
                gui->canvas->getBBox()->w = gui->canvas->getSourceSize()->w;
                gui->canvas->getBBox()->h = gui->canvas->getSourceSize()->h;
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
            if (leftMouseDown && !middleMouseDown)
            {
                gui->canvas->drawOnPaintLayer(&leftMouseDown, &middleMouseDown, drawSize, *shapeStart);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            leftMouseDown = false;
            buttonPressed = false;
            activeSlider = nullptr;
            if (paintMode == PNTR_PaintMode::SHAPE_CIRCLE || paintMode == PNTR_PaintMode::SHAPE_LINE || paintMode == PNTR_PaintMode::SHAPE_SQUARE)
            {
                gui->canvas->drawOnPaintLayer(&leftMouseDown, &middleMouseDown, drawSize, *shapeStart);
            }
            middleMouseDown = false;
            break;

        case SDL_QUIT:
            running = false;
            break;

        default:
            break;
        }
    }

    if (middleMouseDown)
    {
        if (activeSlider != nullptr)
        {
            activeSlider->setValue((mousePos->x - activeSlider->getBBox()->x) / (float)activeSlider->getBBox()->w);
        }
        else
        {
            gui->canvas->getBBox()->x = lastCanvasPos->x + mousePos->x - lastPos->x;
            gui->canvas->getBBox()->y = lastCanvasPos->y + mousePos->y - lastPos->y;
        }
    }

    else
    {
        for (PNTR_Button *b : gui->buttons)
        {
            b->setActive(false);
        }
        switch (paintMode)
        {
        case DRAW:
            gui->brushButton->setActive(true);
            cursorSurface = IMG_Load("assets/icons/circle_48.png");
            break;
        case ERASE:
            gui->eraserButton->setActive(true);
            cursorSurface = IMG_Load("assets/icons/circle_48.png");
            break;
        case FILL:
            gui->fillButton->setActive(true);
            cursorSurface = IMG_Load("assets/icons/fill_48.png");
            break;
        case SHAPE_LINE:
            gui->shapeButtonLine->setActive(true);
            cursorSurface = IMG_Load("assets/icons/circle_48.png");
            break;
        case SHAPE_CIRCLE:
            gui->shapeButtonCircle->setActive(true);
            cursorSurface = IMG_Load("assets/icons/circle_48.png");
            break;
        case SHAPE_SQUARE:
            gui->shapeButtonSquare->setActive(true);
            cursorSurface = IMG_Load("assets/icons/circle_48.png");
            break;
        case PICKER:
            gui->pickerButton->setActive(true);
            cursorSurface = IMG_Load("assets/icons/picker_48.png");
        }
    }

    int drawSizeFac = 64;
    drawSize = drawSizeFac * gui->thickSlider->getValue() * (float)(gui->canvas->getBBox()->w / 1280.0f) * 4;
    SDL_Surface *scaledSurface = SDL_CreateRGBSurface(0, drawSize, drawSize, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_FillRect(scaledSurface, NULL, SDL_MapRGBA(scaledSurface->format, 0, 0, 0, 0));
    SDL_BlitScaled(cursorSurface, NULL, scaledSurface, new SDL_Rect({0, 0, drawSize, drawSize}));

    SDL_Cursor *cursor = SDL_CreateColorCursor(scaledSurface, drawSize / 2, drawSize / 2);
    SDL_SetCursor(cursor);

    SDL_FreeSurface(scaledSurface);
}

void PNTR_Window::dispose()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void PNTR_Window::saveLayersAction()
{
    gui->canvas->saveImage();
}

void PNTR_Window::clearPaintLayerAction()
{
    gui->canvas->clearPaintLayer();
}