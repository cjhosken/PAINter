#ifndef GUI_H
#define GUI_H

#include "../../common.h"
#include "button.h"
#include "slider.h"
#include "canvas.h"

void openAppPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
}

void loadImage() {
    const char* filename = SDL_GetBasePath();
    char selectedFile[1024];

    SDL_strlcpy(selectedFile, filename, sizeof(selectedFile));
    SDL_free((void*)filename);

    SDL_Rect* windowPtr = nullptr;
    SDL_Window* dialog = SDL_CreateWindow("Open File", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Select a file",
                    "Please select a file.", dialog) < 0) {
                    SDL_Log("Error displaying message box: %s", SDL_GetError());
                }

    SDL_DestroyWindow(dialog);
}

void addImage() {
    SDL_Window* dialog = SDL_CreateWindow("Create Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "make image",
                    "Please make an image.", dialog) < 0) {
                    SDL_Log("Error displaying message box: %s", SDL_GetError());
                }

    SDL_DestroyWindow(dialog);
}

void saveImage() {
    const char* filename = SDL_GetBasePath();
    char selectedFile[1024];

    SDL_strlcpy(selectedFile, filename, sizeof(selectedFile));
    SDL_free((void*)filename);

    SDL_Rect* windowPtr = nullptr;
    SDL_Window* dialog = SDL_CreateWindow("Open File", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Select a file",
                    "Please save a file.", dialog) < 0) {
                    SDL_Log("Error displaying message box: %s", SDL_GetError());
                }

    SDL_DestroyWindow(dialog);
}

void openColorWheel() {
    SDL_Window* dialog = SDL_CreateWindow("Color Picker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "pick a color",
                    "Please pick a color.", dialog) < 0) {
                    SDL_Log("Error displaying message box: %s", SDL_GetError());
                }

    SDL_DestroyWindow(dialog);
    activeColor = new SDL_Color({255, 0, 0, 255});
}



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
        iconButton->setAction(openAppPage);

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

        minimizeButton = new SDL_MyButton();
        minimizeButton->setRect(1198, 18, 24, 24);
        minimizeButton->setIcon(IMG_Load("assets/icons/minimize_48.png"));

        closeButton = new SDL_MyButton();
        closeButton->setRect(1240, 18, 24, 24);
        closeButton->setIcon(IMG_Load("assets/icons/close_48.png"));

        brushButton = new SDL_MyButton();
        brushButton->setRect(32, 104, 32, 32);
        brushButton->setIcon(IMG_Load("assets/icons/brush_48.png"));

        eraserButton = new SDL_MyButton();
        eraserButton->setRect(32, 171, 32, 32);
        eraserButton->setIcon(IMG_Load("assets/icons/eraser_48.png"));

        fillButton = new SDL_MyButton();
        fillButton->setRect(32, 238, 32, 32);
        fillButton->setIcon(IMG_Load("assets/icons/fill_48.png"));

        shapeButton = new SDL_MyButton();
        shapeButton->setRect(32, 305, 32, 32);
        shapeButton->setIcon(IMG_Load("assets/icons/shapes_48.png"));


        SDL_MyCircle* sliderCircle = new SDL_MyCircle();
        sliderCircle->setPosition(6, 400);
        sliderCircle->setRadius(32);
        thickSlider = new SDL_MySlider();
        thickSlider->setCircle(sliderCircle);

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