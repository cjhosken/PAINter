#ifndef ACTIONS_H
#define ACTIONS_H

#include "../../common.h"

void openWebPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
}

void loadImage() {
    SDL_Window* dialog = SDL_CreateWindow("Create Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "make image",
                    "Please make an image.", dialog) < 0) {
                    SDL_Log("Error displaying message box: %s", SDL_GetError());
                }

    SDL_DestroyWindow(dialog);
}

void addImage() {
    SDL_Window* dialog = SDL_CreateWindow("Create Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);



    SDL_DestroyWindow(dialog);
}

void saveImage() {
    const char* filename = SDL_GetBasePath();
    char selectedFile[1024];
    /*
    SDL_strlcpy(selectedFile, filename, sizeof(selectedFile));
    SDL_free((void*)filename);

    SDL_Rect* windowPtr = nullptr;
    SDL_Window* dialog = SDL_CreateWindow("Open File", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_HIDDEN);
    SDL_ShowWindow(dialog);

    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Select a file",
                    "Please save a file.", dialog) < 0) {
                    SDL_Log("Error displaying message box: %s", SDL_GetError());
                }

    SDL_DestroyWindow(dialog);*/
    std::system("xdg-open .");
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

void setModeDraw()
{
    editMode = Mode::DRAW;
}

void setModeErase()
{
    editMode = Mode::ERASE;
}

void setModeFill()
{
    editMode = Mode::FILL;
}

void setModeShape()
{
    editMode = Mode::SHAPE;
}

void setModePicker()
{
    editMode = Mode::PICKER;
}

#endif