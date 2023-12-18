#include "pntr_panel.h"
#include "../include/pntr_circle.h"
#include "../include/common.h"

// Constructor methods
PNTR_Panel::PNTR_Panel() : PNTR_Widget() {};
PNTR_Panel::PNTR_Panel(SDL_Rect* bb) : PNTR_Widget(bb), radius(0) {};
PNTR_Panel::PNTR_Panel(SDL_Rect* bb, SDL_Color* c) : PNTR_Widget(bb, c), radius(0) {};
PNTR_Panel::PNTR_Panel(SDL_Rect* bb, SDL_Color* c, int r) : PNTR_Widget(bb, c), radius(r) {};

void PNTR_Panel::setRadius(int r) {radius = r;}

int PNTR_Panel::getRadius() {return radius;}

// Although the draw overrides the draw from widget, they both call the same static function.
void PNTR_Panel::draw(SDL_Renderer* renderer) {PNTR_Panel::renderPanel(renderer, *bbox, *color, radius);}


// STATIC

// renderPanel can be used to draw both sharp and bevelled rectangles. Since it's designed for UIs, renderPanel has not be designed to draw onto surfaces (however with some changes, it could be).
void PNTR_Panel::renderPanel(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, int radius)
{
    if (radius) // If the panel is bevelled
    {
    SDL_Rect r1 = {rect.x, rect.y+radius, rect.w, rect.h-radius*2}; // make small top rectangle
    SDL_Rect r2 = {rect.x+radius, rect.y, rect.w-radius*2, radius}; // make large middle rectangle
    SDL_Rect r3 = {rect.x+radius, rect.y+rect.h-radius, rect.w-radius*2, radius}; // make small bottom rectangle

    // the 3 rectangles make sure that the corners are empty.
    /*      - - - - -
           |   r1   |   
      - - - - - - - - - - -
      |                   |
      |                   | 
      |        r2         |
      |                   | 
      |                   |
      - - - - - - - - - - -
          |    r3   |
          - - - - - - 
    */

    // renderPanel is re-called without no radius.
    renderPanel(renderer, r1, color, 0); 
    renderPanel(renderer, r2, color, 0);
    renderPanel(renderer, r3, color, 0);

    // draw the 4 quarter circles using the static function renderCircle from PNTR_Circle

    PNTR_Circle::renderCircle(renderer, new PNTR_Vector2I(rect.x, rect.y), &color, radius, new PNTR_Vector2I(-radius, -radius));
    PNTR_Circle::renderCircle(renderer, new PNTR_Vector2I(rect.x+rect.w, rect.y), &color, radius, new PNTR_Vector2I(radius, -radius));
    PNTR_Circle::renderCircle(renderer, new PNTR_Vector2I(rect.x+rect.w, rect.y+rect.h), &color, radius, new PNTR_Vector2I(radius, radius));
    PNTR_Circle::renderCircle(renderer, new PNTR_Vector2I(rect.x, rect.y+rect.h), &color, radius, new PNTR_Vector2I(-radius, radius));

    } else { // Otherwise draw a rectangle using the fillRect function.
        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA8888);
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

        SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
}