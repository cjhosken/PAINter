#ifndef ACTIONS_H
#define ACTIONS_H

#include "../common.h"

inline void openWebPage() {
    SDL_OpenURL("https://github.com/cjhosken/PAINter");
}

inline void setModeDraw()
{
    paintMode = PNTR_PaintMode::DRAW;
}

inline void setModeErase()
{
    paintMode = PNTR_PaintMode::ERASE;
}

inline void setModeFill()
{
    paintMode = PNTR_PaintMode::FILL;
}

inline void setModeShapeLine()
{
    paintMode = PNTR_PaintMode::SHAPE_LINE;
}

inline void setModeShapeCircle()
{
    paintMode = PNTR_PaintMode::SHAPE_CIRCLE;
}

inline void setModeShapeSquare()
{
    paintMode = PNTR_PaintMode::SHAPE_SQUARE;
}

inline void setModePicker()
{
    paintMode = PNTR_PaintMode::PICKER;
}

#endif