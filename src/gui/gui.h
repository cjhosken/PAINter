#ifndef GUI_H
#define GUI_H

#include "../../common.h"
#include "actions.h"
#include "button.h"
#include "slider.h"
#include "canvas.h"

typedef struct SDL_Gui {
    SDL_MyCanvas* canvas;
    SDL_MyButton* iconButton;
    SDL_MyButton* loadImageButton;
    SDL_MyButton* addImageButton;
    SDL_MyButton* saveImageButton;

    SDL_MyButton* colorsButton;
    SDL_MyButton* pickerButton;

    SDL_MyButton* minimizeButton;
    SDL_MyButton* closeButton;

    SDL_MyButton* brushButton;
    SDL_MyButton* eraserButton;
    SDL_MyButton* fillButton;
    SDL_MyButton* shapeButton;

    SDL_MySlider* thickSlider;

    SDL_MyButton *buttons[12];

    void init() {
        canvas = new SDL_MyCanvas();
        canvas->setImage(IMG_Load("assets/images/landscape.jpeg"));
        canvas->setRect(0, 0, canvas->image->w, canvas->image->h);

        iconButton = new SDL_MyButton();
        iconButton->setRect(25, 8, 48, 48);
        iconButton->setIcon(IMG_Load("assets/images/ross.jpg"));
        iconButton->setAction(openWebPage);

        loadImageButton = new SDL_MyButton();
        loadImageButton->setRect(102, 16, 32, 32);
        loadImageButton->setIcon(IMG_Load("assets/icons/image_48.png"));
        loadImageButton->setAction(loadImage);

        addImageButton = new SDL_MyButton();
        addImageButton->setRect(153, 16, 32, 32);
        addImageButton->setIcon(IMG_Load("assets/icons/add_image_48.png"));
        addImageButton->setAction(addImage);

        saveImageButton = new SDL_MyButton();
        saveImageButton->setRect(204, 16, 32, 32);
        saveImageButton->setIcon(IMG_Load("assets/icons/save_48.png"));
        saveImageButton->setAction(saveImage);

        colorsButton = new SDL_MyButton();
        colorsButton->setRect(1012, 18 ,24, 24);
        colorsButton->setIcon(IMG_Load("assets/icons/colors_48.png"));
        colorsButton->setAction(openColorWheel);

        pickerButton = new SDL_MyButton();
        pickerButton->setRect(1055, 18 ,24, 24);
        pickerButton->setIcon(IMG_Load("assets/icons/picker_48.png"));
        pickerButton->setAction(setModePicker);

        minimizeButton = new SDL_MyButton();
        minimizeButton->setRect(1198, 18, 24, 24);
        minimizeButton->setIcon(IMG_Load("assets/icons/minimize_48.png"));

        closeButton = new SDL_MyButton();
        closeButton->setRect(1240, 18, 24, 24);
        closeButton->setIcon(IMG_Load("assets/icons/close_48.png"));

        brushButton = new SDL_MyButton();
        brushButton->setRect(32, 104, 32, 32);
        brushButton->setIcon(IMG_Load("assets/icons/brush_48.png"));
        brushButton->setAction(setModeDraw);

        eraserButton = new SDL_MyButton();
        eraserButton->setRect(32, 171, 32, 32);
        eraserButton->setIcon(IMG_Load("assets/icons/eraser_48.png"));
        eraserButton->setAction(setModeErase);

        fillButton = new SDL_MyButton();
        fillButton->setRect(32, 238, 32, 32);
        fillButton->setIcon(IMG_Load("assets/icons/fill_48.png"));
        fillButton->setAction(setModeFill);

        shapeButton = new SDL_MyButton();
        shapeButton->setRect(32, 305, 32, 32);
        shapeButton->setIcon(IMG_Load("assets/icons/shapes_48.png"));
        shapeButton->setAction(setModeShape);

        thickSlider = new SDL_MySlider();
        thickSlider->setRect(new SDL_Rect({328, 32, 160, 1}));
        thickSlider->setColor(new SDL_Color({0, 0, 0, 255}));

        buttons[0] = iconButton;
        buttons[1] = loadImageButton;
        buttons[2] = addImageButton;
        buttons[3] = saveImageButton;
        buttons[4] = colorsButton;
        buttons[5] = pickerButton;
        buttons[6] = minimizeButton;
        buttons[7] = closeButton;
        buttons[8] = brushButton;
        buttons[9] = eraserButton;
        buttons[10] = fillButton;
        buttons[11] = shapeButton;
    }

    void draw(SDL_Renderer* renderer) {
    // CANVAS
    canvas->draw(renderer);

    // BUILD NAV BAR
    SDL_Rect navRect = {0, 0, 1280, 64};
    SDL_Color navColor = {225, 225, 225, 200};
    drawRect(renderer, &navColor, &navRect);

    // LOAD AND SAVING BUTTONS
    
    iconButton->draw(renderer);
    loadImageButton->draw(renderer);
    addImageButton->draw(renderer);
    saveImageButton->draw(renderer);
    
    // THICKNESS BUTTONS
    thickSlider->draw(renderer);

    // COLOR BUTTONS
    
    drawRect(renderer, activeColor, new SDL_Rect({969, 20, 24, 24}));
    
    colorsButton->draw(renderer);
    pickerButton->draw(renderer);
    


    // MIN AND CLOSE BUTTONS
    
    minimizeButton->draw(renderer);
    closeButton->draw(renderer);
    

    // BUILD SIDE BAR
    
    SDL_Rect sideRect = {16, 80, 64, 280};
    SDL_Color sideColor = {225, 225, 225, 200};
    drawRect(renderer, &sideColor, &sideRect, 24);
    
    brushButton->draw(renderer);
    eraserButton->draw(renderer);
    fillButton->draw(renderer);
    shapeButton->draw(renderer);
}

} SDL_Gui;


#endif