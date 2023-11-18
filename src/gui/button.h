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
        action = func;
    }

    void draw(SDL_Renderer* renderer) {
        SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, icon);

        // Draw the rectangle
        drawRect(renderer, color, rect);

        // Render the image texture onto the rectangle
        SDL_RenderCopy(renderer, imageTexture, NULL, &rect);

        // Free the texture when done
        SDL_DestroyTexture(imageTexture);
    }

} SDL_Button;

bool isMouseOver(SDL_Button *button, int mouseX, int mouseY) {
    return (
        mouseX >= (*button).rect.x && mouseX <= (*button).rect.x + (*button).rect.w &&
        mouseY >= (*button).rect.y && mouseY <= (*button).rect.y + (*button).rect.h
    );
}

#endif