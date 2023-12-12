#include "pntr_gui.h"
#include "../include/common.h"
#include "../include/pntr_panel.h"
#include "../include/pntr_button.h"
#include "../include/pntr_slider.h"
#include "../include/pntr_canvas.h"
#include "../include/pntr_vector2i.h"

void openWebPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
}

void setModeDraw()
{
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

        buttons->push_back(iconButton);
        buttons->push_back(saveImageButton);
        buttons->push_back(colorsButton);
        buttons->push_back(pickerButton);
        buttons->push_back(brushButton);
        buttons->push_back(eraserButton);
        buttons->push_back(fillButton);
        buttons->push_back(shapeButtonLine);
        buttons->push_back(shapeButtonCircle);
        buttons->push_back(shapeButtonSquare);
        buttons->push_back(clearImageButton);

        if (readFilePath != NULL)
        {
            canvas = new PNTR_Canvas(IMG_Load(readFilePath));
        }
        else
        {
            canvas = new PNTR_Canvas(new SDL_Rect({0, 0, 512, 512}), new SDL_Color({255, 0, 0, 255}));
        }

        //dialog = new PNTR_ColorDialog(activeColor);
}

void PNTR_Gui::draw(SDL_Renderer* renderer, SDL_Event* event, SDL_Window* window) {
    // CANVAS
    canvas->draw(renderer);

    // BUILD NAV BAR
    navBar->draw(renderer);

    // LOAD AND SAVING BUTTONS
    
    // THICKNESS BUTTONS
    thickSlider->draw(renderer);

    // COLOR BUTTONS
    
    colorView->setColor(activeColor);
    colorView->draw(renderer);
    
    // BUILD SIDE BAR
    
    sideBar->draw(renderer);
    
    for (int bdx = 0; bdx<buttons->size(); bdx++) {
        buttons->at(bdx)->draw(renderer, event, window);
    }

    /*
    if (dialog->isInvoked()) {
        dialog->update();
        dialog->draw();
    }*/
};

// Copyright © 2024 Christopher Hosken