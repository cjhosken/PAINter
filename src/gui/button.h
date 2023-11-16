#ifndef BUTTON_H
#define BUTTON_H

#include "../../common.h"
#include "common_gui.h"

typedef struct {
    SDL_Rect* rect;
    SDL_Surface* icon;

    bool pressed;
} SDL_Button;

SDL_Button* drawButton(SDL_Renderer *renderer, SDL_Color color, int x, int y, int width, int height, SDL_Surface* icon) {
    SDL_Button button;
    button.pressed = false;
    button.icon = icon;

    SDL_Rect rect = {x, y, width, height};
    button.rect = &rect;

    // Convert the imageSurface to a texture
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, icon);
    SDL_FreeSurface(icon);

    // Draw the rectangle
    drawRect(renderer, color, x, y, width, height);

    // Render the image texture onto the rectangle
    SDL_RenderCopy(renderer, imageTexture, NULL, &rect);

    // Free the texture when done
    SDL_DestroyTexture(imageTexture);
    
    return &button;
}

#endif