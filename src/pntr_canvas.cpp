#include "pntr_canvas.h"
#include "../include/common.h"
#include "../include/pntr_circle.h"
#include "../include/pntr_vector2i.h"
#include <vector>

PNTR_Canvas::PNTR_Canvas() : PNTR_Widget() {
    sourceSize = SDL_Rect({0, 0, 0, 0});
    imageLayer = new SDL_Surface();
    paintLayer = new SDL_Surface();
    ghostLayer = new SDL_Surface();
}

PNTR_Canvas::PNTR_Canvas(SDL_Surface* image) : PNTR_Widget() {
    setImageLayer(image);
    sourceSize = SDL_Rect({0, 0, imageLayer->w, imageLayer->h});
    bbox->w = sourceSize.w;
    bbox->h = sourceSize.h;

    paintLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
    clearGhostLayer();
}

PNTR_Canvas::PNTR_Canvas(SDL_Rect* bbox, SDL_Color* c) : PNTR_Widget(bbox, c) {
    
    imageLayer = SDL_CreateRGBSurface(0, bbox->w, bbox->h, 32, 0x00ff0000, 0x0000ff00, 0x00000ff, 0xff000000);
    SDL_FillRect(imageLayer, NULL, SDL_MapRGBA(imageLayer->format, color->r, color->g, color->b, color->a));
    sourceSize = SDL_Rect({0, 0, imageLayer->w, imageLayer->h});

    paintLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
    clearGhostLayer();
}

SDL_Surface* PNTR_Canvas::getImageLayer() {return imageLayer;}; 
SDL_Surface* PNTR_Canvas::getPaintLayer() {return paintLayer;}; 
SDL_Surface* PNTR_Canvas::getGhostLayer() {return ghostLayer;}; 

SDL_Rect PNTR_Canvas::getSourceSize() {
    return sourceSize;
}

void PNTR_Canvas::draw(SDL_Renderer *renderer)
{
    texture = SDL_CreateTextureFromSurface(renderer, imageLayer);
    SDL_RenderCopy(renderer, texture, &sourceSize, bbox);

    SDL_Texture* paintLayerTexture = SDL_CreateTextureFromSurface(renderer, paintLayer);
    SDL_RenderCopy(renderer, paintLayerTexture, &sourceSize, bbox);

    SDL_Texture* ghostLayerTexture = SDL_CreateTextureFromSurface(renderer, ghostLayer);
    SDL_RenderCopy(renderer, ghostLayerTexture, &sourceSize, bbox);

    SDL_DestroyTexture(paintLayerTexture);
    SDL_DestroyTexture(ghostLayerTexture);
}

void PNTR_Canvas::setImageLayer(SDL_Surface* s) {
    imageLayer = s;
    sourceSize.w = s->w;
    sourceSize.h = s->h;
    bbox->w = sourceSize.w;
    bbox->h = sourceSize.h;
}

void PNTR_Canvas::resetPaintLayer() {
    paintLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
};
void PNTR_Canvas::resetGhostLayer() {
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearGhostLayer();
};

void PNTR_Canvas::clearPaintLayer() {SDL_FillRect(paintLayer, NULL, SDL_MapRGBA(paintLayer->format, 255, 255, 255, 0));};
void PNTR_Canvas::clearGhostLayer() {SDL_FillRect(ghostLayer, NULL, SDL_MapRGBA(ghostLayer->format, 255, 255, 255, 0));};

SDL_Surface* PNTR_Canvas::combineLayers() {
    SDL_Surface *output = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
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


void PNTR_Canvas::drawOnPaintLayer(bool* leftMouseDown, bool* middleMouseDown, int drawSize, PNTR_Vector2I* shapeStart) {
    PNTR_Vector2I currentPos;

    // map from draw shape to getImageLayer()
    // mouseOnCanvas.x = ((mX - rX) / rW) * iW
    // mouseOnCanvas.y = ((mY - rY) / rH) * iH

    PNTR_Vector2I mouseOnCanvas = PNTR_Vector2I(
        ((mousePos->x - getBBox()->x) / (float)getBBox()->w) * getSourceSize().w,
        ((mousePos->y - getBBox()->y) / (float)getBBox()->h) * getSourceSize().h 
    );

    int shapeSize = sqrt(pow(shapeStart->x - mouseOnCanvas.x, 2) + pow(shapeStart->y - mouseOnCanvas.y, 2));

    SDL_Surface *output = combineLayers();

    SDL_Color *pixel;
    SDL_Rect* circleBrushRect = new SDL_Rect({0, 0, drawSize, drawSize});
    SDL_Surface* circleSurface = SDL_CreateRGBSurfaceWithFormat(0, circleBrushRect->w, circleBrushRect->h, 32, SDL_PIXELFORMAT_RGBA8888);

    switch (paintMode)
    {
    case PNTR_PaintMode::DRAW:
        circleSurface = PNTR_Circle::circleToSurface(new PNTR_Vector2I(), activeColor, drawSize/2);
        SDL_BlitSurface(circleSurface, NULL, paintLayer, new SDL_Rect({mouseOnCanvas.x-drawSize/2, mouseOnCanvas.y-drawSize/2, drawSize, drawSize}));
        break;

    case PNTR_PaintMode::ERASE:
        circleSurface = PNTR_Circle::circleToSurface(new PNTR_Vector2I(), new SDL_Color({255, 255, 255, 255}), drawSize/2);
        SDL_BlitSurface(circleSurface, NULL, paintLayer, new SDL_Rect({mouseOnCanvas.x-drawSize/2, mouseOnCanvas.y-drawSize/2, drawSize, drawSize}));
        break;

    case PNTR_PaintMode::FILL:
        if ((mouseOnCanvas.x < 0 || mouseOnCanvas.x > getSourceSize().w) || (mouseOnCanvas.y < 0 || mouseOnCanvas.y > getSourceSize().h))
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
                PNTR_Canvas::drawLine(paintLayer, sourceSize, *shapeStart, mouseOnCanvas);
            }
            else
            {
                PNTR_Canvas::drawLine(ghostLayer, sourceSize, *shapeStart, mouseOnCanvas);
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_CIRCLE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                PNTR_Circle::circleOnSurface(paintLayer, &sourceSize, shapeStart, activeColor, shapeSize, false);
            }
            else
            {
                PNTR_Circle::circleOnSurface(ghostLayer, &sourceSize, shapeStart, activeColor, shapeSize, false);
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_SQUARE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                PNTR_Canvas::drawSquare(paintLayer, sourceSize, *shapeStart, shapeSize);
            }
            else
            {
                PNTR_Canvas::drawSquare(ghostLayer, sourceSize, *shapeStart, shapeSize);
            }
        }
        break;
    default:
        break;
    }
    SDL_FreeSurface(output);
    SDL_FreeSurface(circleSurface);
}

// FILL FUNCTIONS

bool PNTR_Canvas::isValid(PNTR_Vector2I position, SDL_Surface *read, SDL_Color *fill, SDL_Color *pixel)
{
    return (position.x >= 0 && position.x < getSourceSize().w) && (position.y >= 0 && position.y < getSourceSize().h) && !compare(fill, pixel) && compare(getSurfacePixel(read, position), pixel);
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

void PNTR_Canvas::drawLine(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I p1, PNTR_Vector2I p2)
{
    PNTR_Vector2I change = PNTR_Vector2I(abs(p2.x - p1.x), abs(p2.y - p1.y));
    PNTR_Vector2I scale = PNTR_Vector2I(p1.x < p2.x ? 1 : -1, p1.y < p2.y ? 1 : -1);
    int error = (change.x > change.y ? change.x : -change.y) / 2, e2;
    while (true)
    {
        /* draw point only if coordinate is valid */
        if (p1.x >= 0 && p1.x < bounds.w && p1.y >= 0 && p1.y < bounds.h)
            setSurfacePixel(surface, activeColor, p1);
        if (p1.x == p2.x && p1.y == p2.y)
            break;
        e2 = error;
        if (e2 > -change.x)
        {
            error -= change.y;
            p1.x += scale.x;
        }
        if (e2 < change.y)
        {
            error += change.x;
            p1.y += scale.y;
        }
    }
} // https://brightspace.bournemouth.ac.uk/d2l/le/lessons/345037/topics/1968571

void PNTR_Canvas::drawSquare(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I center, int length)
{
    PNTR_Vector2I p1 = PNTR_Vector2I(center.x - length / 2, center.y - length / 2);
    PNTR_Vector2I p2 = PNTR_Vector2I(center.x - length / 2, center.y + length / 2);
    PNTR_Vector2I p3 = PNTR_Vector2I(center.x + length / 2, center.y + length / 2);
    PNTR_Vector2I p4 = PNTR_Vector2I(center.x + length / 2, center.y - length / 2);
    PNTR_Canvas::drawLine(surface, bounds, p1, p2);
    PNTR_Canvas::drawLine(surface, bounds, p2, p3);
    PNTR_Canvas::drawLine(surface, bounds, p3, p4);
    PNTR_Canvas::drawLine(surface, bounds, p4, p1);
}


// Copyright © 2024 Christopher Hosken