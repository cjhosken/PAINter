#ifndef ACTIONS_H
#define ACTIONS_H

#include "../../common.h"
#include "colordialog.h"


void openWebPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
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

void setModeShapeLine()
{
    editMode = Mode::SHAPE_LINE;
}

void setModeShapeCircle()
{
    editMode = Mode::SHAPE_CIRCLE;
}

void setModeShapeSquare()
{
    editMode = Mode::SHAPE_SQUARE;
}

void setModePicker()
{
    editMode = Mode::PICKER;
}

#endif