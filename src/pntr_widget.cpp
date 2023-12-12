#include "pntr_widget.h"
#include "../include/pntr_panel.h"
#include "../include/common.h"


PNTR_Widget::PNTR_Widget() {bbox=new SDL_Rect();color=new SDL_Color();}
PNTR_Widget::PNTR_Widget(SDL_Rect* bb) : bbox(bb) {color=new SDL_Color();};
PNTR_Widget::PNTR_Widget(SDL_Rect* bb, SDL_Color* c) : bbox(bb), color(c) {};

void PNTR_Widget::setBBox(SDL_Rect* bb) {bbox=bb;};
void PNTR_Widget::setBBox(int x, int y, int w, int h) {bbox=new SDL_Rect({x, y, w, h});};
SDL_Rect* PNTR_Widget::getBBox() { return bbox;};

void PNTR_Widget::setColor(SDL_Color* c) {color=c;};
SDL_Color* PNTR_Widget::getColor() {return color;};

void PNTR_Widget::draw(SDL_Renderer* renderer) {
    PNTR_Panel::renderPanel(renderer, *bbox, *color, 0);
}

bool PNTR_Widget::isMouseOver(PNTR_Vector2I* mouse) {
    return (
        (mouse->x >= bbox->x && mouse->x <= bbox->x + bbox->w) &&
        (mouse->y >= bbox->y && mouse->y <= bbox->y + bbox->h)
    );
}

void PNTR_Widget::pressEvent() {};