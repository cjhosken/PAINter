#ifndef ACTIONS_H
#define ACTIONS_H

#include "../common.h"

void openWebPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
}

void setModeDraw()
{
    paintMode = PNTR_PaintMode::DRAW;
}

void setModeErase()
{
    paintMode = PNTR_PaintMode::ERASE;
}

void setModeFill()
{
    paintMode = PNTR_PaintMode::FILL;
}

void setModeShapeLine()
{
    paintMode = PNTR_PaintMode::SHAPE_LINE;
}

void setModeShapeCircle()
{
    paintMode = PNTR_PaintMode::SHAPE_CIRCLE;
}

void setModeShapeSquare()
{
    paintMode = PNTR_PaintMode::SHAPE_SQUARE;
}

void setModePicker()
{
    paintMode = PNTR_PaintMode::PICKER;
}

#endif