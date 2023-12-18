#include "pntr_widget.h"
#include "../include/pntr_panel.h"
#include "../include/common.h"

// Constructor methods.
PNTR_Widget::PNTR_Widget() {bbox=new SDL_Rect();color=new SDL_Color();}
PNTR_Widget::PNTR_Widget(SDL_Rect* bb) : bbox(bb) {color=new SDL_Color();};
PNTR_Widget::PNTR_Widget(SDL_Rect* bb, SDL_Color* c) : bbox(bb), color(c) {};

void PNTR_Widget::setBBox(SDL_Rect* bb) {bbox=bb;};
void PNTR_Widget::setBBox(int x, int y, int w, int h) {bbox=new SDL_Rect({x, y, w, h});};
SDL_Rect* PNTR_Widget::getBBox() { return bbox;};

void PNTR_Widget::setColor(SDL_Color* c) {color=c;};
SDL_Color* PNTR_Widget::getColor() {return color;};

// This calls the static renderPanel() function, as widgets are assumed to be rectangular shaped.
void PNTR_Widget::draw(SDL_Renderer* renderer) {
    PNTR_Panel::renderPanel(renderer, *bbox, *color, 0);
}

// The isMouseOver function is extremely important in detected hover and pressEvents. It checks if the mouse position is within the bounds of the bbox.
/*
    (bbox->x, bbox->y) - - - - - - - - - - - - - - - - - (bbox->x + bbox->w, bbox->y)          
          |                                                        |
          |                  o (mouse-x, mouse->y)                 |
          |                                                        |
          |                                                        |
    (bbox->x, bbox->y + bbox->h) - - - - - - - (bbox->x + bbox->w, bbox->y + bbox->h)
*/
bool PNTR_Widget::isMouseOver(PNTR_Vector2I* mouse) {
    return (
        (mouse->x >= bbox->x && mouse->x <= bbox->x + bbox->w) &&
        (mouse->y >= bbox->y && mouse->y <= bbox->y + bbox->h)
    );
}

// PressEvents are called if the mouse is pressed and isMouseOver is true. This could have been done inside the widget class, but I ended up calling it inside PNTR_Window
void PNTR_Widget::pressEvent() {};