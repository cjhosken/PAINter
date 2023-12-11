#include "pntr_gui.h"

PNTR_Gui::PNTR_Gui() {
        canvas = new PNTR_Canvas();

        navBar = new PNTR_Panel(new SDL_Rect({0, 0, 1280, 64}), new SDL_Color({225, 225, 225, 200}), 0);
        sideBar = new PNTR_Panel(new SDL_Rect({16, 80, 64, 410}), new SDL_Color({225, 225, 225, 200}), 24);

        iconButton = new PNTR_Button(new SDL_Rect({25, 8, 48, 48}), IMG_Load("assets/images/ross.jpg"), openWebPage);

        saveImageButton = new PNTR_Button(new SDL_Rect({156, 16, 32, 32}), IMG_Load("assets/icons/save_48.png"));

        clearImageButton = new PNTR_Button(new SDL_Rect({100, 16, 32, 32}), IMG_Load("assets/icons/box_48.png"));
        
        colorsButton = new PNTR_Button(new SDL_Rect({1200, 20 ,24, 24}), IMG_Load("assets/icons/colors_48.png"));

        pickerButton = new PNTR_Button(new SDL_Rect({1240, 20 ,24, 24}), IMG_Load("assets/icons/picker_48.png"), setModePicker);

        brushButton = new PNTR_Button(new SDL_Rect({32, 104, 32, 32}), IMG_Load("assets/icons/brush_48.png"), setModeDraw);

        eraserButton = new PNTR_Button(new SDL_Rect({32, 171, 32, 32}), IMG_Load("assets/icons/eraser_48.png"),setModeErase);

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

        dialog = new PNTR_ColorDialog(activeColor);
}

void PNTR_Gui::draw(SDL_Renderer* renderer) {
    // CANVAS
    canvas->draw(renderer);

    // BUILD NAV BAR
    navBar->draw(renderer);

    // LOAD AND SAVING BUTTONS
    
    iconButton->draw(renderer);
    clearImageButton->draw(renderer);
    saveImageButton->draw(renderer);
    
    // THICKNESS BUTTONS
    thickSlider->draw(renderer);

    // COLOR BUTTONS
    
    colorView->setColor(activeColor);
    colorView->draw(renderer);
    
    colorsButton->draw(renderer);
    pickerButton->draw(renderer);
    
    // BUILD SIDE BAR
    
   sideBar->draw(renderer);
    
    brushButton->draw(renderer);
    eraserButton->draw(renderer);
    fillButton->draw(renderer);
    shapeButtonLine->draw(renderer);
    shapeButtonCircle->draw(renderer);
    shapeButtonSquare->draw(renderer);
    
    if (dialog->isInvoked()) {
        dialog->update();
        dialog->draw();
    }
};