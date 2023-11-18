#ifndef GUI_H
#define GUI_H

#include "../../common.h"
#include "button.h"
#include "slider.h"
#include "canvas.h"

typedef struct {
    SDL_Canvas* canvas;
    SDL_Button* iconButton = new SDL_Button();
    SDL_Button* loadImageButton = new SDL_Button();
    SDL_Button* addImageButton = new SDL_Button();
    SDL_Button* saveImageButton = new SDL_Button();

    SDL_Button* colorsButton = new SDL_Button();
    SDL_Button* pickerButton = new SDL_Button();

    SDL_Button* minimizeButton = new SDL_Button();
    SDL_Button* closeButton = new SDL_Button();

    SDL_Button* brushButton = new SDL_Button();
    SDL_Button* eraserButton = new SDL_Button();
    SDL_Button* fillButton = new SDL_Button();
    SDL_Button* shapeButton = new SDL_Button();

    SDL_Slider* thickSlider = new SDL_Slider();

    SDL_Button *buttons[13] = {
        iconButton,
        loadImageButton,
        addImageButton,
        saveImageButton,
        colorsButton,
        colorsButton,
        pickerButton,
        minimizeButton,
        closeButton,
        brushButton,
        eraserButton,
        fillButton,
        shapeButton
    };

    void init() {
        canvas = new SDL_Canvas();
        canvas->setRect(0, 0, 1280, 720);
        canvas->setImage(IMG_Load("assets/images/landscape.jpeg"));

        SDL_Rect iconButtonRect = {25, 8, 48, 48};
        (*iconButton).rect = iconButtonRect;
        (*iconButton).icon = IMG_Load("assets/images/ross.jpg");

        SDL_Rect loadImageButtonRect = {102, 16, 32, 32};
        (*loadImageButton).rect = loadImageButtonRect;
        (*loadImageButton).icon = IMG_Load("assets/icons/image_48.png");

        SDL_Rect addImageButtonRect = {153, 16, 32, 32};
        (*addImageButton).rect = addImageButtonRect;
        (*addImageButton).icon = IMG_Load("assets/icons/add_image_48.png");

        SDL_Rect saveImageButtonRect = {204, 16, 32, 32};
        (*saveImageButton).rect = saveImageButtonRect;
        (*saveImageButton).icon = IMG_Load("assets/icons/save_48.png");

        SDL_Rect colorsButtonRect = {1012, 18, 24, 24};
        (*colorsButton).rect = colorsButtonRect;
        (*colorsButton).icon = IMG_Load("assets/icons/colors_48.png");

        SDL_Rect pickerButtonRect = {1055, 18, 24, 24};
        (*pickerButton).rect = pickerButtonRect;
        (*pickerButton).icon = IMG_Load("assets/icons/picker_48.png");

        SDL_Rect minimizeButtonRect = {1198, 18, 24, 24};
        (*minimizeButton).rect = minimizeButtonRect;
        (*minimizeButton).icon = IMG_Load("assets/icons/minimize_48.png");

        SDL_Rect closeButtonRect = {1240, 18, 24, 24};
        (*closeButton).rect = closeButtonRect;
        (*closeButton).icon = IMG_Load("assets/icons/close_48.png");

        SDL_Rect brushButtonRect = {32, 104, 32, 32};
        (*brushButton).rect = brushButtonRect;
        (*brushButton).icon = IMG_Load("assets/icons/brush_48.png");

        SDL_Rect eraserButtonRect = {32, 171, 32, 32};
        (*eraserButton).rect = eraserButtonRect;
        (*eraserButton).icon = IMG_Load("assets/icons/eraser_48.png");

        SDL_Rect fillButtonRect = {32, 238, 32, 32};
        (*fillButton).rect = fillButtonRect;
        (*fillButton).icon = IMG_Load("assets/icons/fill_48.png");

        SDL_Rect shapeButtonRect = {32, 305, 32, 32};
        (*shapeButton).rect = shapeButtonRect;
        (*shapeButton).icon = IMG_Load("assets/icons/shapes_48.png");


        SDL_Circle sliderCircle = {6, 400, 32};
        (*thickSlider).circle = sliderCircle;
    }

    void draw(SDL_Renderer* renderer) {
    // CANVAS
    (*canvas).draw(renderer);

    // BUILD NAV BAR
    SDL_Rect navRect = {0, 0, 1280, 64};
    SDL_Color navColor = {225, 225, 225, 200};
    drawRect(renderer, navColor, navRect);

    // LOAD AND SAVING BUTTONS
    
    iconButton->draw(renderer);
    loadImageButton->draw(renderer);
    addImageButton->draw(renderer);
    saveImageButton->draw(renderer);
    

    // THICKNESS BUTTONS
    //drawSlider(renderer, thickSlider);

    // COLOR BUTTONS
    /*
    drawRect(renderer, {255, 101, 101, 255}, {797, 20, 24, 24});
    drawRect(renderer, {93, 235, 99, 255}, {840, 20, 24, 24});
    drawRect(renderer, {94, 139, 255, 255}, {883, 20, 24, 24});
    drawRect(renderer, {216, 134, 255, 255}, {926, 20, 24, 24});
    drawRect(renderer, {89, 89, 89, 255}, {969, 20, 24, 24});
    */
    
    colorsButton->draw(renderer);
    pickerButton->draw(renderer);
    


    // MIN AND CLOSE BUTTONS
    
    minimizeButton->draw(renderer);
    closeButton->draw(renderer);
    


    // BUILD SIDE BAR
    
    SDL_Rect sideRect = {16, 80, 64, 280};
    SDL_Color sideColor = {225, 225, 225, 200};
    drawRect(renderer, sideColor, sideRect, 24);
    
    brushButton->draw(renderer);
    eraserButton->draw(renderer);
    fillButton->draw(renderer);
    shapeButton->draw(renderer);
}

} SDL_Gui;


#endif