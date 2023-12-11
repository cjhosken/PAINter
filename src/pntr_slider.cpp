#include "pntr_slider.h"
#include "include/pntr_panel.h"
#include "include/pntr_circle.h"
#include "include/pntr_vector2i.h"
#include "include/common.h"

PNTR_Slider::PNTR_Slider() : PNTR_Widget()
{
    value = 0.0f;
    knob = new PNTR_Circle(new PNTR_Vector2I(), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb) : PNTR_Widget(bb)
{
    value = 0.0f;
    knob = new PNTR_Circle(new PNTR_Vector2I(), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, float v) : PNTR_Widget(bb)
{
    value = v;
    knob = new PNTR_Circle(new PNTR_Vector2I(), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, SDL_Color *c) : PNTR_Widget(bb, c)
{
    knob = new PNTR_Circle(new PNTR_Vector2I(), 8, new SDL_Color({0, 0, 0, 255}));
};

PNTR_Slider::PNTR_Slider(SDL_Rect *bb, SDL_Color *c, float v) : PNTR_Widget(bb, c)
{
    value = v;
    knob = new PNTR_Circle(new PNTR_Vector2I(), 8, new SDL_Color({0, 0, 0, 255}));
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
void PNTR_Slider::setValue(float v) { value = SDL_min(SDL_max(0.0f, v), 1.0f); };

float PNTR_Slider::getValue() {
    return value;
}

PNTR_Circle* PNTR_Slider::getKnob() {
    return knob;
}

void PNTR_Slider::draw(SDL_Renderer *renderer)
{
    PNTR_Panel::renderPanel(renderer, *bbox, *color, 0);
    knob->setPosition(new PNTR_Vector2I(bbox->x + (value * bbox->w), bbox->y));
    knob->draw(renderer);
}

bool PNTR_Slider::pressEvent()
{
    return knob->isMouseOver(mousePos);
}

void PNTR_Slider::onDragEvent()
{
    setValue(mousePos->x);
}

// Copyright © 2024 Christopher Hosken