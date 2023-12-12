#include "pntr_window.h"
#include "../include/common.h"
#include "../include/pntr_gui.h"
#include "../include/pntr_button.h"
#include "../include/pntr_colordialog.h"
#include <functional>

void PNTR_Window::saveLayersAction()
{
    gui->canvas->saveImage();
}

void PNTR_Window::clearPaintLayerAction()
{
    gui->canvas->clearPaintLayer();
}

void PNTR_Window::openColorDialog()
{
    dialog->show();
}

PNTR_Window::PNTR_Window()
{
    window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_SHOWN);
    icon = IMG_Load("assets/images/ross.jpg");
    SDL_SetWindowIcon(window, icon);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    event = new SDL_Event();
    gui = new PNTR_Gui();
    dialog = new PNTR_ColorDialog(event);
    std::function<void()> clearFunc = bind(&PNTR_Window::clearPaintLayerAction, this);
    std::function<void()> saveFunc = bind(&PNTR_Window::saveLayersAction, this);
    std::function<void()> colorFunc = bind(&PNTR_Window::openColorDialog, this);
    gui->saveImageButton->setAction(saveFunc);
    gui->clearImageButton->setAction(clearFunc);
    gui->colorsButton->setAction(colorFunc);

    gui->thickSlider->setValue(0.25f);

    processEvents();
    draw();
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

    gui->draw(renderer, event, window);

    if (dialog->isInvoked())
    {
        dialog->draw();
    }

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
    SDL_WaitEvent(event);
    if (SDL_GetWindowID(window) == event->window.windowID)
    {
        if (event->window.event == SDL_WINDOWEVENT_CLOSE)
            running = false;

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            switch (event->button.button)
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

                for (int bdx = 0; bdx < (int)gui->buttons.size(); bdx++)
                {
                    if (gui->buttons.at(bdx)->isMouseOver(mousePos))
                    {
                        gui->buttons.at(bdx)->pressEvent();
                        buttonPressed = true;
                        break;
                    }
                }

                if (gui->thickSlider->isMouseOver(mousePos))
                {
                    middleMouseDown = true;
                    activeSlider = gui->thickSlider;
                    lastPos = mousePos;
                    break;
                }

                if (!buttonPressed)
                {
                    if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                    {
                        gui->canvas->drawOnPaintLayer(DRAW_SIZE * gui->thickSlider->getValue(), shapeStart, false);
                    }
                }

                break;

            case SDL_BUTTON_MIDDLE:
                if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                {
                    lastCanvasPos = new PNTR_Vector2I(gui->canvas->getBBox()->x, gui->canvas->getBBox()->y);
                    lastPos = mousePos;
                    middleMouseDown = true;
                }
            }
            break;

        case SDL_MOUSEWHEEL:
            if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
            {
                float scalar = 75;
                float aspect = (float)gui->canvas->getBBox()->h / gui->canvas->getBBox()->w;

                PNTR_Vector2I scale = PNTR_Vector2I(scalar * (gui->canvas->getBBox()->x - mousePos->x) / (float)gui->canvas->getBBox()->w, scalar * (gui->canvas->getBBox()->y - mousePos->y) / (float)gui->canvas->getBBox()->h);

                if (event->wheel.y > 0)
                {
                    gui->canvas->getBBox()->x += scale.x;
                    gui->canvas->getBBox()->y += scale.y;
                    gui->canvas->getBBox()->w += scalar;
                    gui->canvas->getBBox()->h += scalar * aspect;
                }

                if (event->wheel.y < 0 && gui->canvas->getBBox()->h > 75)
                {
                    gui->canvas->getBBox()->x -= scale.x;
                    gui->canvas->getBBox()->y -= scale.y;
                    gui->canvas->getBBox()->w -= scalar;
                    gui->canvas->getBBox()->h -= scalar * aspect;
                }
            }

            break;

        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
            case SDLK_h:
                gui->canvas->getBBox()->x = 0;
                gui->canvas->getBBox()->y = 0;
                gui->canvas->getBBox()->w = gui->canvas->getSourceSize().w;
                gui->canvas->getBBox()->h = gui->canvas->getSourceSize().h;
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
            mousePos = new PNTR_Vector2I(event->motion.x, event->motion.y);
            if (leftMouseDown && !middleMouseDown)
            {
                if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                {
                    gui->canvas->drawOnPaintLayer(DRAW_SIZE * gui->thickSlider->getValue(), shapeStart, false);
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            leftMouseDown = false;
            buttonPressed = false;
            activeSlider = nullptr;
            if (paintMode == PNTR_PaintMode::SHAPE_CIRCLE || paintMode == PNTR_PaintMode::SHAPE_LINE || paintMode == PNTR_PaintMode::SHAPE_SQUARE)
            {
                if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                {
                    gui->canvas->drawOnPaintLayer(DRAW_SIZE * gui->thickSlider->getValue(), shapeStart, false);
                }
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

    if (dialog->isInvoked())
    {
        dialog->processEvents(activeSlider);
    }

    if (leftMouseDown)
    {
        if (activeSlider != nullptr)
        {
            activeSlider->setValue((float(mousePos->x) - float(activeSlider->getBBox()->x)) / float(activeSlider->getBBox()->w));
            printf("SLIDER VAL: %f\n", activeSlider->getValue());
            printf("MOUSE: %d %d\n", mousePos->x, mousePos->y);
            printf("BBOX: %d %d %d %d\n", activeSlider->getBBox()->x, activeSlider->getBBox()->y, activeSlider->getBBox()->w, activeSlider->getBBox()->h);
            fflush(stdout);
        }
    }

    else if (middleMouseDown)
    {
        gui->canvas->getBBox()->x = lastCanvasPos->x + mousePos->x - lastPos->x;
        gui->canvas->getBBox()->y = lastCanvasPos->y + mousePos->y - lastPos->y;
    }

    else
    {
        for (int bdx = 0; bdx < (int)gui->buttons.size(); bdx++)
        {

            gui->buttons.at(bdx)->setActive(false);
        }
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

    float cursorSize = DRAW_SIZE * gui->thickSlider->getValue() * ((float)(gui->canvas->getBBox()->w / (float)gui->canvas->getSourceSize().w));
    SDL_Surface *scaledSurface = SDL_CreateRGBSurface(0, cursorSize, cursorSize, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_FillRect(scaledSurface, NULL, SDL_MapRGBA(scaledSurface->format, 0, 0, 0, 0));
    SDL_BlitScaled(cursorSurface, NULL, scaledSurface, new SDL_Rect({0, 0, (int)cursorSize, (int)cursorSize}));

    SDL_Cursor *cursor = SDL_CreateColorCursor(scaledSurface, cursorSize / 2, cursorSize / 2);
    SDL_SetCursor(cursor);

    SDL_FreeSurface(scaledSurface);
}

void PNTR_Window::dispose()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}