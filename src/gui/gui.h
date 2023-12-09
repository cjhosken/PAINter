#ifndef GUI_H
#define GUI_H

#include "../../common.h"
#include "actions.h"
#include "button.h"
#include "slider.h"
#include "canvas.h"
#include "colordialog.h"

typedef struct PNTR_Gui {
    PNTR_Canvas* canvas;
    PNTR_Button* iconButton;
    PNTR_Button* saveImageButton;

    PNTR_Button* clearImageButton;

    PNTR_Button* colorsButton;
    PNTR_Button* pickerButton;

    PNTR_Button* brushButton;
    PNTR_Button* eraserButton;
    PNTR_Button* fillButton;


    PNTR_Button* shapeButtonLine;
    PNTR_Button* shapeButtonCircle;
    PNTR_Button* shapeButtonSquare;

    PNTR_Slider* thickSlider;

    PNTR_Button *buttons[11];

    SDL_MyColorDialog* dialog;

    void init() {
        canvas = new PNTR_Canvas();

        iconButton = new PNTR_Button();
        iconButton->setRect(25, 8, 48, 48);
        iconButton->setIcon(IMG_Load("assets/images/ross.jpg"));
        iconButton->setAction(openWebPage);

        saveImageButton = new PNTR_Button();
        saveImageButton->setRect(156, 16, 32, 32);
        saveImageButton->setIcon(IMG_Load("assets/icons/save_48.png"));

        clearImageButton = new PNTR_Button();
        clearImageButton->setRect(100, 16, 32, 32);
        clearImageButton->setIcon(IMG_Load("assets/icons/box_48.png"));

        colorsButton = new PNTR_Button();
        colorsButton->setRect(1200, 20 ,24, 24);
        colorsButton->setIcon(IMG_Load("assets/icons/colors_48.png"));

        pickerButton = new PNTR_Button();
        pickerButton->setRect(1240, 20 ,24, 24);
        pickerButton->setIcon(IMG_Load("assets/icons/picker_48.png"));
        pickerButton->setAction(setModePicker);

        brushButton = new PNTR_Button();
        brushButton->setRect(32, 104, 32, 32);
        brushButton->setIcon(IMG_Load("assets/icons/brush_48.png"));
        brushButton->setAction(setModeDraw);

        eraserButton = new PNTR_Button();
        eraserButton->setRect(32, 171, 32, 32);
        eraserButton->setIcon(IMG_Load("assets/icons/eraser_48.png"));
        eraserButton->setAction(setModeErase);

        fillButton = new PNTR_Button();
        fillButton->setRect(32, 238, 32, 32);
        fillButton->setIcon(IMG_Load("assets/icons/fill_48.png"));
        fillButton->setAction(setModeFill);

        shapeButtonLine = new PNTR_Button();
        shapeButtonLine->setRect(32, 305, 32, 32);
        shapeButtonLine->setIcon(IMG_Load("assets/icons/line_48.png"));
        shapeButtonLine->setAction(setModeShapeLine);

        shapeButtonCircle = new PNTR_Button();
        shapeButtonCircle->setRect(32, 372, 32, 32);
        shapeButtonCircle->setIcon(IMG_Load("assets/icons/circle_thick_48.png"));
        shapeButtonCircle->setAction(setModeShapeCircle);

        shapeButtonSquare = new PNTR_Button();
        shapeButtonSquare->setRect(32, 439, 32, 32);
        shapeButtonSquare->setIcon(IMG_Load("assets/icons/box_48.png"));
        shapeButtonSquare->setAction(setModeShapeSquare);

        thickSlider = new PNTR_Slider();
        thickSlider->setRect(new SDL_Rect({320, 32, 400, 1}));
        thickSlider->setColor(new SDL_Color({0, 0, 0, 255}));

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

        dialog = new SDL_MyColorDialog();
    }

    void draw(SDL_Renderer* renderer) {
    // CANVAS
    canvas->draw(renderer);

    // BUILD NAV BAR
    SDL_Rect navRect = {0, 0, 1280, 64};
    SDL_Color navColor = {225, 225, 225, 200};
    renderRect(renderer, &navColor, &navRect, 0);

    // LOAD AND SAVING BUTTONS
    
    iconButton->draw(renderer);
    clearImageButton->draw(renderer);
    saveImageButton->draw(renderer);
    
    // THICKNESS BUTTONS
    thickSlider->draw(renderer);

    // COLOR BUTTONS
    
    renderRect(renderer, activeColor, new SDL_Rect({1160, 20, 24, 24}), 0);
    
    colorsButton->draw(renderer);
    pickerButton->draw(renderer);
    
    // BUILD SIDE BAR
    
    SDL_Rect sideRect = {16, 80, 64, 410};
    SDL_Color sideColor = {225, 225, 225, 200};
    renderRect(renderer, &sideColor, &sideRect, 24);
    
    brushButton->draw(renderer);
    eraserButton->draw(renderer);
    fillButton->draw(renderer);
    shapeButtonLine->draw(renderer);
    shapeButtonCircle->draw(renderer);
    shapeButtonSquare->draw(renderer);
    
    if (dialog->invoked) {
        dialog->update();
        dialog->draw();
    }
}

} SDL_Gui;


#endif