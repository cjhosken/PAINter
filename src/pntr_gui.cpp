#include "pntr_gui.h"
#include "include/common.h"
#include "include/pntr_panel.h"
#include "include/pntr_button.h"
#include "include/pntr_slider.h"
#include "include/pntr_canvas.h"
#include "include/pntr_vector2i.h"

void openWebPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
}

void setModeDraw()
{
    printf("DRAW\n");
    fflush(stdout);
    paintMode = PNTR_PaintMode::DRAW;
}

void setModeErase()
{
    paintMode = PNTR_PaintMode::ERASE;
}

void setModeFill()
{
    paintMode = PNTR_PaintMode::FILL;
}

void setModeShapeLine()
{
    paintMode = PNTR_PaintMode::SHAPE_LINE;
}

void setModeShapeCircle()
{
    paintMode = PNTR_PaintMode::SHAPE_CIRCLE;
}

void setModeShapeSquare()
{
    paintMode = PNTR_PaintMode::SHAPE_SQUARE;
}

void setModePicker()
{
    paintMode = PNTR_PaintMode::PICKER;
}

PNTR_Gui::PNTR_Gui() {
        navBar = new PNTR_Panel(new SDL_Rect({0, 0, 1280, 64}), new SDL_Color({225, 225, 225, 200}), 0);
        sideBar = new PNTR_Panel(new SDL_Rect({16, 80, 64, 410}), new SDL_Color({225, 225, 225, 200}), 24);

        iconButton = new PNTR_Button(new SDL_Rect({25, 8, 48, 48}), IMG_Load("assets/images/ross.jpg"), openWebPage);

        saveImageButton = new PNTR_Button(new SDL_Rect({156, 16, 32, 32}), IMG_Load("assets/icons/save_48.png"));

        clearImageButton = new PNTR_Button(new SDL_Rect({100, 16, 32, 32}), IMG_Load("assets/icons/box_48.png"));
        
        colorsButton = new PNTR_Button(new SDL_Rect({1200, 20 ,24, 24}), IMG_Load("assets/icons/colors_48.png"));

        pickerButton = new PNTR_Button(new SDL_Rect({1240, 20 ,24, 24}), IMG_Load("assets/icons/picker_48.png"), setModePicker);

        brushButton = new PNTR_Button(new SDL_Rect({32, 104, 32, 32}), IMG_Load("assets/icons/brush_48.png"), setModeDraw);

        eraserButton = new PNTR_Button(new SDL_Rect({32, 171, 32, 32}), IMG_Load("assets/icons/eraser_48.png"), setModeErase);

        fillButton = new PNTR_Button(new SDL_Rect({32, 238, 32, 32}), IMG_Load("assets/icons/fill_48.png"), setModeFill);

        shapeButtonLine = new PNTR_Button(new SDL_Rect({32, 305, 32, 32}), IMG_Load("assets/icons/line_48.png"), setModeShapeLine);

        shapeButtonCircle = new PNTR_Button(new SDL_Rect({32, 372, 32, 32}), IMG_Load("assets/icons/circle_thick_48.png"), setModeShapeCircle);

        shapeButtonSquare = new PNTR_Button(new SDL_Rect({32, 439, 32, 32}), IMG_Load("assets/icons/box_48.png"), setModeShapeSquare);

        thickSlider = new PNTR_Slider(new SDL_Rect({320, 32, 400, 1}), new SDL_Color({0, 0, 0, 255}));

        colorView = new PNTR_Panel(new SDL_Rect({1160, 20, 24, 24}), activeColor);

        buttons[0] = iconButton;
        buttons[1] = saveImageButton;
        buttons[2] = colorsButton;
        buttons[3] = pickerButton;
        buttons[4] = brushButton;
        buttons[5] = eraserButton;
        buttons[6] = fillButton;
        buttons[7] = shapeButtonLine;
        buttons[8] = shapeButtonCircle;
        buttons[9] = shapeButtonSquare;
        buttons[10] = clearImageButton;

        if (readFilePath != NULL)
        {
            canvas = new PNTR_Canvas(new SDL_Rect({0, 0, APP_WIDTH, APP_HEIGHT}), IMG_Load(readFilePath));
        }
        else
        {
            canvas = new PNTR_Canvas(new SDL_Rect({0, 0, APP_WIDTH, APP_HEIGHT}), PNTR_Vector2I(512, 512), new SDL_Color({255, 255, 255, 255}));
        }

        //dialog = new PNTR_ColorDialog(activeColor);
}

void PNTR_Gui::draw(SDL_Renderer* renderer, SDL_Event* event, SDL_Window* window) {
    // CANVAS
    canvas->draw(renderer);

    // BUILD NAV BAR
    navBar->draw(renderer);

    // LOAD AND SAVING BUTTONS
    
    iconButton->draw(renderer, event, window);
    clearImageButton->draw(renderer, event, window);
    saveImageButton->draw(renderer, event, window);
    
    // THICKNESS BUTTONS
    thickSlider->draw(renderer);

    // COLOR BUTTONS
    
    colorView->setColor(activeColor);
    colorView->draw(renderer);
    
    colorsButton->draw(renderer, event, window);
    pickerButton->draw(renderer, event, window);
    
    // BUILD SIDE BAR
    
    sideBar->draw(renderer);
    
    brushButton->draw(renderer, event, window);
    eraserButton->draw(renderer, event, window);
    fillButton->draw(renderer, event, window);
    shapeButtonLine->draw(renderer, event, window);
    shapeButtonCircle->draw(renderer, event, window);
    shapeButtonSquare->draw(renderer, event, window);
    /*
    if (dialog->isInvoked()) {
        dialog->update();
        dialog->draw();
    }*/
};

// Copyright © 2024 Christopher Hosken