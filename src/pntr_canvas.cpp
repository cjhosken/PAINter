#include "pntr_canvas.h"
#include "include/common.h"
#include "include/pntr_circle.h"
#include "include/pntr_vector2i.h"
#include <vector>

PNTR_Canvas::PNTR_Canvas() : PNTR_Widget() {
    sourceSize = new SDL_Rect();
    imageLayer = new SDL_Surface();
    paintLayer = new SDL_Surface();
    ghostLayer = new SDL_Surface();
}

PNTR_Canvas::PNTR_Canvas(SDL_Rect* bb, SDL_Surface* image) : PNTR_Widget(bb) {
    setImageLayer(image);
    sourceSize = new SDL_Rect({0, 0, imageLayer->w, imageLayer->h});

    paintLayer = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
    clearGhostLayer();
}

PNTR_Canvas::PNTR_Canvas(SDL_Rect* bb, PNTR_Vector2I size, SDL_Color* c) : PNTR_Widget(bb, c) {
    
    imageLayer = SDL_CreateRGBSurface(0, 512, 512, 32, 0x00ff0000, 0x0000ff00, 0x00000ff, 0xff000000);
    SDL_FillRect(imageLayer, NULL, SDL_MapRGBA(imageLayer->format, color->r, color->g, color->b, color->a));
    sourceSize = new SDL_Rect({0, 0, imageLayer->w, imageLayer->h});

    paintLayer = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
    clearGhostLayer();
}

SDL_Surface* PNTR_Canvas::getImageLayer() {return imageLayer;}; 
SDL_Surface* PNTR_Canvas::getPaintLayer() {return paintLayer;}; 
SDL_Surface* PNTR_Canvas::getGhostLayer() {return ghostLayer;}; 

SDL_Rect* PNTR_Canvas::getSourceSize() {
    return sourceSize;
}

void PNTR_Canvas::draw(SDL_Renderer *renderer)
{
    texture = SDL_CreateTextureFromSurface(renderer, imageLayer);
    SDL_RenderCopy(renderer, texture, sourceSize, bbox);

    texture = SDL_CreateTextureFromSurface(renderer, paintLayer);
    SDL_RenderCopy(renderer, texture, sourceSize, bbox);

    texture = SDL_CreateTextureFromSurface(renderer, ghostLayer);
    SDL_RenderCopy(renderer, texture, sourceSize, bbox);
}

void PNTR_Canvas::setImageLayer(SDL_Surface* s) {
    imageLayer = s;
}

void PNTR_Canvas::resetPaintLayer() {
    paintLayer = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
};
void PNTR_Canvas::resetGhostLayer() {
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearGhostLayer();
};

void PNTR_Canvas::clearPaintLayer() {SDL_FillRect(paintLayer, NULL, SDL_MapRGBA(paintLayer->format, 255, 255, 255, 0));};
void PNTR_Canvas::clearGhostLayer() {SDL_FillRect(ghostLayer, NULL, SDL_MapRGBA(ghostLayer->format, 255, 255, 255, 0));};

SDL_Surface* PNTR_Canvas::combineLayers() {
    SDL_Surface *output = SDL_CreateRGBSurface(0, sourceSize->w, sourceSize->h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(imageLayer, NULL, output, NULL);
    SDL_BlitSurface(paintLayer, NULL, output, NULL);
    return output;
};

void PNTR_Canvas::saveImage() {
        SDL_Surface* output = combineLayers();

        if (writeFilePath != NULL)
        {
            IMG_SavePNG(output, writeFilePath);
        }
        else
        {
            IMG_SavePNG(output, "./output.png");
        }

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File Saved!", "Your getImageLayer() has been saved!", NULL);

        SDL_FreeSurface(output);
}


void PNTR_Canvas::drawOnPaintLayer(bool* leftMouseDown, bool* middleMouseDown, int drawSize, PNTR_Vector2I shapeStart) {
    PNTR_Vector2I currentPos;

    // map from draw shape to getImageLayer()
    // mouseOnCanvas.x = ((mX - rX) / rW) * iW
    // mouseOnCanvas.y = ((mY - rY) / rH) * iH

    PNTR_Vector2I mouseOnCanvas = PNTR_Vector2I(
        ((mousePos->x - getBBox()->x) / (float)getBBox()->w) * getImageLayer()->w,
        ((mousePos->y - getBBox()->y) / (float)getBBox()->h) * getImageLayer()->h 
    );

    int shapeSize = sqrt(pow(shapeStart.x - mouseOnCanvas.x, 2) + pow(shapeStart.y - mouseOnCanvas.y, 2));

    SDL_Surface *output = combineLayers();

    SDL_Color *pixel;
    SDL_Rect circleBrushRect = {0, 0, drawSize * 2, drawSize * 2};
    SDL_Surface* circleSurface = SDL_CreateRGBSurfaceWithFormat(0, circleBrushRect.w, circleBrushRect.h, 32, SDL_PIXELFORMAT_RGBA8888);;

    switch (paintMode)
    {
    case PNTR_PaintMode::DRAW:
        PNTR_Circle::circleOnSurface(circleSurface, circleBrushRect, PNTR_Vector2I(), *activeColor, drawSize, true);
        SDL_BlitSurface(circleSurface, NULL, getPaintLayer(), new SDL_Rect({mouseOnCanvas.x - drawSize, mouseOnCanvas.y - drawSize, drawSize, drawSize}));
        break;

    case PNTR_PaintMode::ERASE:
        PNTR_Circle::circleOnSurface(circleSurface, circleBrushRect, PNTR_Vector2I(), SDL_Color({255, 255, 255, 0}), drawSize, true);
        SDL_BlitSurface(circleSurface, NULL, getPaintLayer(), new SDL_Rect({mouseOnCanvas.x - drawSize, mouseOnCanvas.y - drawSize, drawSize, drawSize}));
        break;

    case PNTR_PaintMode::FILL:
        if ((mouseOnCanvas.x < 0 || mouseOnCanvas.x > getImageLayer()->w) || (mouseOnCanvas.y < 0 || mouseOnCanvas.y > getImageLayer()->h))
            break;

        pixel = getSurfacePixel(output, PNTR_Vector2I(mouseOnCanvas.x, mouseOnCanvas.y));
        fillStack.clear();
        fillStack.push_back(PNTR_Vector2I({mouseOnCanvas.x, mouseOnCanvas.y}));

        visit_array.resize(getImageLayer()->w, std::vector<bool>(getImageLayer()->h, false));
        while (!fillStack.empty())
        {
            currentPos = fillStack.front();
            fillStack.erase(fillStack.begin());
            fillStack.shrink_to_fit();
            PNTR_Canvas::floodFill(currentPos, output, getPaintLayer(), activeColor, pixel);
        }

        fillStack.clear();
        fillStack.shrink_to_fit();
        visit_array.clear();
        visit_array.resize(getImageLayer()->w, std::vector<bool>(getImageLayer()->h, false));
        break;

    case PNTR_PaintMode::SHAPE_LINE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                PNTR_Canvas::drawLine(getPaintLayer(), getImageLayer()->w, getImageLayer()->h, shapeStart.x, shapeStart.y, mouseOnCanvas.x, mouseOnCanvas.y);
            }
            else
            {
                PNTR_Canvas::drawLine(getGhostLayer(), getImageLayer()->w, getImageLayer()->h, shapeStart.x, shapeStart.y, mouseOnCanvas.x, mouseOnCanvas.y);
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_CIRCLE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                PNTR_Circle::circleOnSurface(getPaintLayer(), *(getSourceSize()), shapeStart, *activeColor, shapeSize, false);
            }
            else
            {
                PNTR_Circle::circleOnSurface(getGhostLayer(), *(getSourceSize()), shapeStart, *activeColor, shapeSize, false);
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_SQUARE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                PNTR_Canvas::drawSquare(getPaintLayer(), getImageLayer()->w, getImageLayer()->h, shapeStart.x, shapeStart.y, shapeSize);
            }
            else
            {
                PNTR_Canvas::drawSquare(getGhostLayer(), getImageLayer()->w, getImageLayer()->h, shapeStart.x, shapeStart.y, shapeSize);
            }
        }
        break;
    default:
        break;
    }
    SDL_FreeSurface(output);
}

// FILL FUNCTIONS

bool PNTR_Canvas::isValid(PNTR_Vector2I position, SDL_Surface *read, SDL_Color *fill, SDL_Color *pixel)
{
    return (position.x >= 0 && position.x < getImageLayer()->w) && (position.y >= 0 && position.y < getImageLayer()->h) && !compare(fill, pixel) && compare(getSurfacePixel(read, position), pixel);
}

void PNTR_Canvas::floodFill(PNTR_Vector2I pos, SDL_Surface *read, SDL_Surface *write, SDL_Color *fill, SDL_Color *pixel)
{
    if (!isValid(pos, read, fill, pixel))
        return;
    setSurfacePixel(write, fill, pos);

    visit_array[pos.x][pos.y] = true;
    if (isValid(PNTR_Vector2I(pos.x + 1, pos.y), read, fill, pixel) && !visit_array[pos.x + 1][pos.y])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x + 1, pos.y));
        visit_array[pos.x + 1][pos.y] = true;
    }
    if (isValid(PNTR_Vector2I(pos.x - 1, pos.y), read, fill, pixel) && !visit_array[pos.x - 1][pos.y])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x - 1, pos.y));
        visit_array[pos.x - 1][pos.y] = true;
    }
    if (isValid(PNTR_Vector2I(pos.x, pos.y + 1), read, fill, pixel) && !visit_array[pos.x][pos.y + 1])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x, pos.y + 1));
        visit_array[pos.x][pos.y + 1] = true;
    }
    if (isValid(PNTR_Vector2I(pos.x, pos.y - 1), read, fill, pixel) && !visit_array[pos.x][pos.y - 1])
    {
        fillStack.push_back(PNTR_Vector2I(pos.x, pos.y - 1));
        visit_array[pos.x][pos.y - 1] = true;
    }
}

// STATIC FUNCTIONS

void PNTR_Canvas::drawLine(SDL_Surface *surface, int wdth, int hght, int x0, int y0, int xn, int yn)
{
    int dx = abs(xn - x0), sx = x0 < xn ? 1 : -1;
    int dy = abs(yn - y0), sy = y0 < yn ? 1 : -1;
    int error = (dx > dy ? dx : -dy) / 2, e2;
    while (1)
    {
        /* draw point only if coordinate is valid */
        if (x0 >= 0 && x0 < wdth && y0 >= 0 && y0 < hght)
            setSurfacePixel(surface, activeColor, PNTR_Vector2I(x0, y0));
        if (x0 == xn && y0 == yn)
            break;
        e2 = error;
        if (e2 > -dx)
        {
            error -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            error += dx;
            y0 += sy;
        }
    }
} // https://brightspace.bournemouth.ac.uk/d2l/le/lessons/345037/topics/1968571

void PNTR_Canvas::drawSquare(SDL_Surface *surface, int width, int height, int x0, int y0, int length)
{
    PNTR_Vector2I p1 = {x0 - length / 2, y0 - length / 2};
    PNTR_Vector2I p2 = {x0 - length / 2, y0 + length / 2};
    PNTR_Vector2I p3 = {x0 + length / 2, y0 + length / 2};
    PNTR_Vector2I p4 = {x0 + length / 2, y0 - length / 2};
    PNTR_Canvas::drawLine(surface, width, height, p1.x, p1.y, p2.x, p2.y);
    PNTR_Canvas::drawLine(surface, width, height, p2.x, p2.y, p3.x, p3.y);
    PNTR_Canvas::drawLine(surface, width, height, p3.x, p3.y, p4.x, p4.y);
    PNTR_Canvas::drawLine(surface, width, height, p4.x, p4.y, p1.x, p1.y);
}


// Copyright © 2024 Christopher Hosken