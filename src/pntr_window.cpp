#include "pntr_window.h"
#include "../include/common.h"
#include "../include/pntr_gui.h"
#include "../include/pntr_button.h"
#include "../include/pntr_colordialog.h"
#include <functional>

// The constructor method. This is what is shown on your screen.
PNTR_Window::PNTR_Window()
{
    // Create a window and add icons to it, setup the renderer, events, gui, and color builder dialog.
    window = SDL_CreateWindow("PAINter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_SHOWN);
    icon = IMG_Load("assets/images/ross.jpg");
    SDL_SetWindowIcon(window, icon);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    event = new SDL_Event();
    gui = new PNTR_Gui();
    dialog = new PNTR_ColorDialog(event);

    // Assign button actions. Since these actions must adjust the gui, and therefore must be class functions, they have to be assigned using std::function and std::bind.
    std::function<void()> clearFunc = bind(&PNTR_Window::clearPaintLayerAction, this);
    std::function<void()> saveFunc = bind(&PNTR_Window::saveLayersAction, this);
    std::function<void()> colorFunc = bind(&PNTR_Window::openColorDialog, this);
    gui->saveImageButton->setAction(saveFunc);
    gui->clearImageButton->setAction(clearFunc);
    gui->colorsButton->setAction(colorFunc);

    // Initialize the thickness slider, and recenter the canvas
    gui->thickSlider->setValue(0.25f);
    gui->canvas->recenter();

    // Do one processEvent and draw call.
    processEvents();
    draw();
}

void PNTR_Window::run()
{
    // Very simple, keep checking for events and drawing.
    running = true;
    while (running)
    {
        processEvents();
        draw();
    }
}

void PNTR_Window::draw()
{
    // Set the background color, and clear the renderer.
    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_RenderClear(renderer);

    // Draw the gui
    gui->draw(renderer, event, window);

    // if the colorbuilder is invoked, draw it.
    if (dialog->isInvoked())
    {
        dialog->draw();
    }

    SDL_RenderPresent(renderer);
    // Clear the ghost layer
    gui->canvas->clearGhostLayer();
}

// This function is used by the colorPicker to select any color in the application window.
void PNTR_Window::pickColor()
{
    activeColor = getPixel(renderer, *mousePos);
}

// processEvents() is where all the ui controlling is done. It's split between the main application window and the color builder dialog as I couldnt find a way to process their events seperately.
void PNTR_Window::processEvents()
{
    SDL_Surface *cursorSurface;
    SDL_WaitEvent(event);

    // Check if the window is the application window
    if (SDL_GetWindowID(window) == event->window.windowID)
    {
        // If the window is closed, shutdown the app.
        if (event->window.event == SDL_WINDOWEVENT_CLOSE)
            running = false;

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            switch (event->button.button)
            {
            case SDL_BUTTON_LEFT:
                // On a left button press, set the global leftMouseDown to true, save the click location in canvas coords.
                leftMouseDown = true;
                shapeStart = new PNTR_Vector2I(
                    (int)(((mousePos->x - gui->canvas->getBBox()->x) / (float)gui->canvas->getBBox()->w) * gui->canvas->getImageLayer()->w),
                    (int)(((mousePos->y - gui->canvas->getBBox()->y) / (float)gui->canvas->getBBox()->h) * gui->canvas->getImageLayer()->h));

                // If the picker is enabled, pick the color under the mouse and switch to draw mode.
                if (paintMode == PNTR_PaintMode::PICKER)
                {
                    pickColor();
                    paintMode = PNTR_PaintMode::DRAW;
                    break;
                }

                // Reset buttonPressed to false.
                buttonPressed = false;

                // Iterate through all the buttons
                for (int bdx = 0; bdx < (int)gui->buttons.size(); bdx++)
                {
                    // If a button is pressed, run the pressEvent and set buttonPressed to true
                    if (gui->buttons.at(bdx)->isMouseOver(mousePos))
                    {
                        gui->buttons.at(bdx)->pressEvent();
                        buttonPressed = true;
                        break;
                    }
                }

                // Check if the slider is active. If so, set the activeSlider to thickSlider
                if (gui->thickSlider->isMouseOver(mousePos))
                {
                    // middleMouserDown is used as a way to disable drawing, there is not actual middle mouse button being pressed.
                    middleMouseDown = true;
                    activeSlider = gui->thickSlider;
                    lastPos = mousePos;
                    break;
                }

                if (!buttonPressed)
                {
                    // If no buttons have been pressed, and the mouse is not over the navBar or sideBar, draw on the paintLayer.
                    if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                    {
                        gui->canvas->drawOnPaintLayer(DRAW_SIZE * gui->thickSlider->getValue(), shapeStart, false);
                    }
                }

                break;

            case SDL_BUTTON_MIDDLE:
                // If the mouse is not over the navBar or sideBar, calculate the canvas position so that it can be moved later.
                if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                {
                    lastCanvasPos = new PNTR_Vector2I(gui->canvas->getBBox()->x, gui->canvas->getBBox()->y);
                    lastPos = mousePos;
                    middleMouseDown = true;
                }
            }
            break;

        case SDL_MOUSEWHEEL:
            // On the mouse scroll, increase/decrease the scale of the canvas.
            if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
            {
                float scalar = 75;
                float aspect = (float)gui->canvas->getBBox()->h / gui->canvas->getBBox()->w;

                // The scale is calculated by multiplying the scalar with the difference between the canvas and mouse position, and then dividing by the size of the canvas.
                PNTR_Vector2I scale = PNTR_Vector2I(scalar * (gui->canvas->getBBox()->x - mousePos->x) / (float)gui->canvas->getBBox()->w, scalar * (gui->canvas->getBBox()->y - mousePos->y) / (float)gui->canvas->getBBox()->h);

                // The scale is then added to the x & y coordinates of the canvas bbox, with the height and width being increased accordingly to the image aspect ratio.
                if (event->wheel.y > 0)
                {
                    gui->canvas->getBBox()->x += scale.x;
                    gui->canvas->getBBox()->y += scale.y;
                    gui->canvas->getBBox()->w += scalar;
                    gui->canvas->getBBox()->h += scalar * aspect;
                }

                // I've made a "minimum scale" value so that the canvas cant shrink to 0 and then never grow again.
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
            // process all the key events.
            switch (event->key.keysym.sym)
            {
            case SDLK_h:
                gui->canvas->recenter();
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
            case SDLK_w:
                openColorDialog();
                break;
            case SDLK_EQUALS:
                gui->thickSlider->setValue(gui->thickSlider->getValue()+0.01);
                break;
            case SDLK_MINUS:
                gui->thickSlider->setValue(gui->thickSlider->getValue()-0.01);
                break;
            default:
                break;
            }
            break;

        case SDL_MOUSEMOTION:
            // When the mouse moves, capture the movement location.
            mousePos = new PNTR_Vector2I(event->motion.x, event->motion.y);
            // If the mouse can draw
            if (leftMouseDown && !middleMouseDown)
            {
                if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                {
                    // Draw on the canvas
                    gui->canvas->drawOnPaintLayer(DRAW_SIZE * gui->thickSlider->getValue(), shapeStart, true);
                    if (paintMode == PNTR_PaintMode::DRAW || paintMode == PNTR_PaintMode::ERASE)
                    {
                        // If the draw mode is draw or erase, then recalculate shapeStart. This is done so that no matter how fast you move your mouse, you will always draw between the captured mouse positions.
                        // In other terms, it stops the "spottyness" from happening when the mouse is moved too fast.
                        shapeStart = new PNTR_Vector2I(
                            (int)(((mousePos->x - gui->canvas->getBBox()->x) / (float)gui->canvas->getBBox()->w) * gui->canvas->getImageLayer()->w),
                            (int)(((mousePos->y - gui->canvas->getBBox()->y) / (float)gui->canvas->getBBox()->h) * gui->canvas->getImageLayer()->h));
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            // When the mouse is release, reset all the global variables.
            leftMouseDown = false;
            buttonPressed = false;
            activeSlider = nullptr;

            // If the paintMode is in circle, line, or square, confirm the draw and write it to the paintLayer
            if (paintMode == PNTR_PaintMode::SHAPE_CIRCLE || paintMode == PNTR_PaintMode::SHAPE_LINE || paintMode == PNTR_PaintMode::SHAPE_SQUARE)
            {
                if (!gui->navBar->isMouseOver(mousePos) && !gui->sideBar->isMouseOver(mousePos))
                {
                    gui->canvas->drawOnPaintLayer(DRAW_SIZE * gui->thickSlider->getValue(), shapeStart, false);
                }
            }
            middleMouseDown = false;
            break;

        // If the quit button is pressed, quit the application
        case SDL_QUIT:
            running = false;
            break;

        default:
            break;
        }
    }
    else // These are the events for the color builder dialog
    {
        switch (event->window.event)
        {
        // If the dialog is closed, hide it. We don't want to re-initialize the dialog each time, so hiding it works better.
        case SDL_WINDOWEVENT_CLOSE:
            dialog->hide();
            break;

        default:
            break;
        }

        switch (event->type)
        {
        // Capture the mouse movement and store it in mousePos.
        case SDL_MOUSEMOTION:
            mousePos = new PNTR_Vector2I(event->motion.x, event->motion.y);
            break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event->button.button)
            {

            case SDL_BUTTON_LEFT:
                // If the left mouse button is pressed, set the global variables, then iterate through all the sliders in the color builder dialog.
                leftMouseDown = true;

                buttonPressed = false;
                for (int sdx = 0; sdx < (int)dialog->sliders.size(); sdx++)
                {
                    // If on the sliders is selected, set it to active and begin calculating the new value.
                    if (dialog->sliders.at(sdx)->isMouseOver(mousePos))
                    {
                        dialog->sliders.at(sdx)->pressEvent();
                        activeSlider = dialog->sliders.at(sdx);
                        middleMouseDown = true;
                        lastPos = mousePos;

                        break;
                    }
                }
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            // On mouse release, reset all the global variables.
            leftMouseDown = false;
            middleMouseDown = false;
            buttonPressed = false;
            activeSlider = nullptr;

            break;
        }

        // Update the color that is calculated byt the sliders
        activeColor->a = 255;
        activeColor->r = (int)(255 * dialog->rSlider->getValue());
        activeColor->g = (int)(255 * dialog->gSlider->getValue());
        activeColor->b = (int)(255 * dialog->bSlider->getValue());
    }

    // Once all the events are processed...
    // Check if the user is currently moving a slider. If so, update the slider based on the mouse position.
    if (leftMouseDown)
    {
        if (activeSlider != nullptr)
        {
            activeSlider->setValue((float(mousePos->x) - float(activeSlider->getBBox()->x)) / float(activeSlider->getBBox()->w));
        }
    }
    // Otherwise, if the middle mouse is down, move the canvas.
    else if (middleMouseDown)
    {
        // The code sets the canvas location to the sum ofits previous location
        // and the difference between the mouse and it's last location.
        // This works on a per-call basis and gives a smooth looking translation.
        gui->canvas->getBBox()->x = lastCanvasPos->x + mousePos->x - lastPos->x;
        gui->canvas->getBBox()->y = lastCanvasPos->y + mousePos->y - lastPos->y;
    }


    // Here, disable all the buttons...
    else
    {
        for (int bdx = 0; bdx < (int)gui->buttons.size(); bdx++)
        {

            gui->buttons.at(bdx)->setActive(false);
        }
    }

    // ...Then enable the active one. It's a cheap hack for getting radio buttons.
    switch (paintMode)
    {
    case DRAW:
        gui->brushButton->setActive(true);
        break;
    case ERASE:
        gui->eraserButton->setActive(true);
        break;
    case FILL:
        gui->fillButton->setActive(true);
        break;
    case SHAPE_LINE:
        gui->shapeButtonLine->setActive(true);
        break;
    case SHAPE_CIRCLE:
        gui->shapeButtonCircle->setActive(true);
        break;
    case SHAPE_SQUARE:
        gui->shapeButtonSquare->setActive(true);
        break;
    case PICKER:
        gui->pickerButton->setActive(true);
    }
    
    // Finish the event processing by re-creating the cursor so that it's size is correct when zooming.
    cursorSurface = IMG_Load("assets/icons/circle_48.png");
    float cursorSize = DRAW_SIZE * gui->thickSlider->getValue() * ((float)(gui->canvas->getBBox()->w / (float)gui->canvas->getSourceSize().w));
    SDL_Surface *scaledSurface = SDL_CreateRGBSurface(0, cursorSize, cursorSize, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_FillRect(scaledSurface, NULL, SDL_MapRGBA(scaledSurface->format, 0, 0, 0, 0));
    SDL_BlitScaled(cursorSurface, NULL, scaledSurface, new SDL_Rect({0, 0, (int)cursorSize, (int)cursorSize}));

    SDL_Cursor *cursor = SDL_CreateColorCursor(scaledSurface, (cursorSize / 2), (cursorSize / 2));
    SDL_SetCursor(cursor);

    SDL_FreeSurface(scaledSurface);
}

// action function for saving the image.
void PNTR_Window::saveLayersAction()
{
    gui->canvas->saveImage();
}

// action function for clearing the paint layer
void PNTR_Window::clearPaintLayerAction()
{
    gui->canvas->clearPaintLayer();
}

// action function for opening the color builder.
void PNTR_Window::openColorDialog()
{
    dialog->show();
}

// To avoid any memory issues, destroy the canvas, renderer, and window.
void PNTR_Window::dispose()
{
    gui->canvas->destroy();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}