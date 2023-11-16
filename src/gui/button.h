#ifndef BUTTON_H
#define BUTTON_H

#include "../../common.h"
#include "rect.h"

typedef struct {
    SDL_Rect rect;
    SDL_Color color = {255, 255, 255, 0};
    SDL_Surface* icon;

    bool pressed;
    void (*action)(void);

    void setAction(void (*func)(void)) {
        action=func;
    }

} SDL_Button;

void drawButton(SDL_Renderer *renderer, SDL_Button button) {
    // Convert the imageSurface to a texture
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, button.icon);
    SDL_FreeSurface(button.icon);

    // Draw the rectangle
    drawRect(renderer, button.color, button.rect);

    // Render the image texture onto the rectangle
    SDL_RenderCopy(renderer, imageTexture, NULL, &(button.rect));

    // Free the texture when done
    SDL_DestroyTexture(imageTexture);
    
}

bool isMouseOver(SDL_Button *button, int mouseX, int mouseY) {
    return (
        mouseX >= (*button).rect.x && mouseX <= (*button).rect.x + (*button).rect.w &&
        mouseY >= (*button).rect.y && mouseY <= (*button).rect.y + (*button).rect.h
    );
}

#endif