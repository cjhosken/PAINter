#include "pntr_slider.h"
#include "../include/pntr_panel.h"
#include "../include/pntr_circle.h"
#include "../include/pntr_vector2i.h"
#include "../include/common.h"

// Constructor methods
PNTR_Slider::PNTR_Slider() : PNTR_Widget()
{
    value = 0.0f;
    knob = new PNTR_Circle(new PNTR_Vector2I(), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb) : PNTR_Widget(bb)
{
    value = 0.0f;
    knob = new PNTR_Circle(new PNTR_Vector2I(bbox->x, bbox->y), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, float v) : PNTR_Widget(bb)
{
    value = v;
    knob = new PNTR_Circle(new PNTR_Vector2I(bbox->x, bbox->y), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, SDL_Color *c) : PNTR_Widget(bb, c)
{
    knob = new PNTR_Circle(new PNTR_Vector2I(bbox->x, bbox->y), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, SDL_Color *c, float v) : PNTR_Widget(bb, c)
{
    value = v;
    knob = new PNTR_Circle(new PNTR_Vector2I(bbox->x, bbox->y), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, SDL_Color *c, PNTR_Circle *k) : PNTR_Widget(bb, c)
{
    knob = k;
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, SDL_Color *c, PNTR_Circle *k, float v) : PNTR_Widget(bb, c)
{
    value = v;
    knob = k;
};

void PNTR_Slider::setKnob(PNTR_Circle *k) { knob = k; };

// setValue takes an input and clamps it between 0.0f and 1.0f.
void PNTR_Slider::setValue(float v) { value = SDL_min(SDL_max(0.0f, v), 1.0f); };

float PNTR_Slider::getValue() {
    return value;
}

PNTR_Circle* PNTR_Slider::getKnob() {
    return knob;
}

// Draw a line (the slider bbox) and the knob
void PNTR_Slider::draw(SDL_Renderer *renderer)
{
    PNTR_Panel::renderPanel(renderer, *bbox, *color, 0);
    knob->setPosition(new PNTR_Vector2I(bbox->x + (value * bbox->w), bbox->y - knob->getRadius()));;
    knob->draw(renderer);
}

// The isMouseOver is only true if the mouse is over the knob.
bool PNTR_Slider::isMouseOver(PNTR_Vector2I* mouse) {
    return knob->isMouseOver(mouse);
}

// When the mouse is down and the slider is active, it sets the value of the slider to:
// (distance from mosue position to slider start) / (width of slider)
// this means that the value is 0.0f when the mouse is at the start of the slider, and 1.0f when the mouse is at the end of the slider
void PNTR_Slider::pressEvent()
{
    setValue((float(mousePos->x) - float(getBBox()->x)) / float(getBBox()->w));
}