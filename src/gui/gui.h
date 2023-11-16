#ifndef GUI_H
#define GUI_H

#include "../../common.h"
#include "common_gui.h"
#include "button.h"
#include "slider.h"


int buildGui(SDL_Renderer* renderer) {
    // CANVAS
    drawRect(renderer, {255, 255, 255, 255}, 0, 0, 1280, 720);

    // BUILD NAV BAR
    drawRect(renderer, {245, 245, 245, 255}, 0, 0, 1280, 64);

    // LOAD AND SAVING BUTTONS
    drawButton(renderer, {0, 0, 0, 0}, 25, 8, 48, 48, IMG_Load("assets/images/ross.jpg"));
    drawButton(renderer, {0, 0, 0, 0}, 102, 16, 32, 32, IMG_Load("assets/icons/image_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 153, 16, 32, 32, IMG_Load("assets/icons/add_image_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 204, 16, 32, 32, IMG_Load("assets/icons/save_48.png"));

    // THICKNESS BUTTONS
    drawSlider(renderer);

    // COLOR BUTTONS
    drawRect(renderer, {255, 101, 101, 255}, 797, 20, 24, 24);
    drawRect(renderer, {93, 235, 99, 255}, 840, 20, 24, 24);
    drawRect(renderer, {94, 139, 255, 255}, 883, 20, 24, 24);
    drawRect(renderer, {216, 134, 255, 255}, 926, 20, 24, 24);
    drawRect(renderer, {89, 89, 89, 255}, 969, 20, 24, 24);
    drawButton(renderer, {0, 0, 0, 0}, 1012, 18, 24, 24, IMG_Load("assets/icons/colors_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 1055, 18, 24, 24, IMG_Load("assets/icons/picker_48.png"));


    // MIN AND CLOSE BUTTONS
    drawButton(renderer, {0, 0, 0, 0}, 1198, 18, 24, 24, IMG_Load("assets/icons/minimize_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 1240, 18, 24, 24, IMG_Load("assets/icons/close_48.png"));


    // BUILD SIDE BAR
    drawRect(renderer, {245, 245, 245, 255}, 16, 80, 64, 280, 24);
    drawButton(renderer, {0, 0, 0, 0}, 32, 104, 32, 32, IMG_Load("assets/icons/brush_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 32, 171, 32, 32, IMG_Load("assets/icons/eraser_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 32, 238, 32, 32, IMG_Load("assets/icons/fill_48.png"));
    drawButton(renderer, {0, 0, 0, 0}, 32, 305, 32, 32, IMG_Load("assets/icons/shapes_48.png"));

    return 0;
}

#endif