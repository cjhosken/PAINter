#include "pntr_canvas.h"
#include "../include/common.h"
#include "../include/pntr_circle.h"
#include "../include/pntr_vector2i.h"
#include <vector>

PNTR_Canvas::PNTR_Canvas() : PNTR_Widget()
{
    sourceSize = SDL_Rect({0, 0, 0, 0});
    imageLayer = new SDL_Surface();
    paintLayer = new SDL_Surface();
    ghostLayer = new SDL_Surface();
    imageChanged = true;
    paintChanged = true;
    ghostChanged = true;
    imageLayerTexture = SDL_CreateTextureFromSurface(renderer, imageLayer);
    paintLayerTexture = SDL_CreateTextureFromSurface(renderer, paintLayer);
    ghostLayerTexture = SDL_CreateTextureFromSurface(renderer, ghostLayer);
}

PNTR_Canvas::PNTR_Canvas(SDL_Surface *image) : PNTR_Widget()
{
    setImageLayer(image);
    sourceSize = SDL_Rect({0, 0, imageLayer->w, imageLayer->h});
    bbox->w = sourceSize.w;
    bbox->h = sourceSize.h;

    paintLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    clearPaintLayer();
    clearGhostLayer();
    imageLayerTexture = SDL_CreateTextureFromSurface(renderer, imageLayer);
    paintLayerTexture = SDL_CreateTextureFromSurface(renderer, paintLayer);
    ghostLayerTexture = SDL_CreateTextureFromSurface(renderer, ghostLayer);
}

PNTR_Canvas::PNTR_Canvas(SDL_Rect *bbox, SDL_Color *c) : PNTR_Widget(bbox, c)
{

    imageLayer = SDL_CreateRGBSurface(0, bbox->w, bbox->h, 32, 0x00ff0000, 0x0000ff00, 0x00000ff, 0xff000000);
    SDL_FillRect(imageLayer, NULL, SDL_MapRGBA(imageLayer->format, color->r, color->g, color->b, color->a));
    sourceSize = SDL_Rect({0, 0, imageLayer->w, imageLayer->h});

    paintLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    clearPaintLayer();
    clearGhostLayer();
    imageLayerTexture = SDL_CreateTextureFromSurface(renderer, imageLayer);
    paintLayerTexture = SDL_CreateTextureFromSurface(renderer, paintLayer);
    ghostLayerTexture = SDL_CreateTextureFromSurface(renderer, ghostLayer);
}

SDL_Surface *PNTR_Canvas::getImageLayer() { return imageLayer; };
SDL_Surface *PNTR_Canvas::getPaintLayer() { return paintLayer; };
SDL_Surface *PNTR_Canvas::getGhostLayer() { return ghostLayer; };

SDL_Rect PNTR_Canvas::getSourceSize()
{
    return sourceSize;
}

void PNTR_Canvas::draw(SDL_Renderer *renderer)
{
    if (imageChanged)
    {
        imageLayerTexture = SDL_CreateTextureFromSurface(renderer, imageLayer);
        imageChanged = false;
    }

    if (paintChanged)
    {
        paintLayerTexture = SDL_CreateTextureFromSurface(renderer, paintLayer);
        paintChanged = false;
    }

    if (ghostChanged)
    {
        ghostLayerTexture = SDL_CreateTextureFromSurface(renderer, ghostLayer);
        ghostChanged = false;
    }

    SDL_RenderCopy(renderer, imageLayerTexture, &sourceSize, bbox);
    SDL_RenderCopy(renderer, paintLayerTexture, &sourceSize, bbox);
    SDL_RenderCopy(renderer, ghostLayerTexture, &sourceSize, bbox);
}

void PNTR_Canvas::setImageLayer(SDL_Surface *s)
{
    imageLayer = s;
    sourceSize.w = s->w;
    sourceSize.h = s->h;
    bbox->w = sourceSize.w;
    bbox->h = sourceSize.h;
    imageChanged = true;
}

void PNTR_Canvas::resetPaintLayer()
{
    paintLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearPaintLayer();
};
void PNTR_Canvas::resetGhostLayer()
{
    ghostLayer = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    clearGhostLayer();
};

void PNTR_Canvas::clearPaintLayer()
{
    SDL_FillRect(paintLayer, NULL, SDL_MapRGBA(paintLayer->format, 255, 255, 255, 0));
    paintChanged = true;
};
void PNTR_Canvas::clearGhostLayer()
{
    SDL_FillRect(ghostLayer, NULL, SDL_MapRGBA(ghostLayer->format, 255, 255, 255, 0));
    ghostChanged = true;
};

SDL_Surface *PNTR_Canvas::combineLayers()
{
    SDL_Surface *output = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(imageLayer, NULL, output, NULL);
    SDL_BlitSurface(paintLayer, NULL, output, NULL);
    return output;
};

void PNTR_Canvas::saveImage()
{
    SDL_Surface *output = combineLayers();

    if (writeFilePath != NULL)
    {
        IMG_SavePNG(output, writeFilePath);
    }
    else
    {
        IMG_SavePNG(output, "./PAINting.png");
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File Saved!", "Your PAINting has been saved!", NULL);

    SDL_FreeSurface(output);
}

void PNTR_Canvas::drawOnPaintLayer(int drawSize, PNTR_Vector2I *shapeStart, bool hold)
{
    PNTR_Vector2I currentPos;

    // map from draw shape to getImageLayer()
    // mouseOnCanvas.x = ((mX - rX) / rW) * iW
    // mouseOnCanvas.y = ((mY - rY) / rH) * iH

    PNTR_Vector2I mouseOnCanvas = PNTR_Vector2I(
        ((mousePos->x - getBBox()->x) / (float)getBBox()->w) * getSourceSize().w,
        ((mousePos->y - getBBox()->y) / (float)getBBox()->h) * getSourceSize().h);

    int shapeSize = sqrt(pow(shapeStart->x - mouseOnCanvas.x, 2) + pow(shapeStart->y - mouseOnCanvas.y, 2));

    SDL_Surface *output = combineLayers();

    SDL_Color *pixel;
    SDL_Rect *circleBrushRect = new SDL_Rect({0, 0, drawSize, drawSize});
    SDL_Surface *circleSurface = SDL_CreateRGBSurfaceWithFormat(0, circleBrushRect->w, circleBrushRect->h, 32, SDL_PIXELFORMAT_RGBA8888);

    switch (paintMode)
    {
    case PNTR_PaintMode::DRAW:
        if (hold)
        {
            PNTR_Canvas::drawThickLine(paintLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
        }
        else
        {
            circleSurface = PNTR_Circle::circleToSurface(new PNTR_Vector2I(), activeColor, drawSize / 2);
            SDL_BlitSurface(circleSurface, NULL, paintLayer, new SDL_Rect({mouseOnCanvas.x - drawSize / 2, mouseOnCanvas.y - drawSize / 2, drawSize, drawSize}));
        }
        paintChanged = true;
        break;

    case PNTR_PaintMode::ERASE:
        if (hold)
        {
            drawThickLine(paintLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, new SDL_Color({255, 255, 255, 255}));
            shapeStart = &mouseOnCanvas;
        }
        else
        {
            circleSurface = PNTR_Circle::circleToSurface(new PNTR_Vector2I(), new SDL_Color({255, 255, 255, 255}), drawSize / 2);
            SDL_BlitSurface(circleSurface, NULL, paintLayer, new SDL_Rect({mouseOnCanvas.x - drawSize / 2, mouseOnCanvas.y - drawSize / 2, drawSize, drawSize}));
            shapeStart = &mouseOnCanvas;
        }
        paintChanged = true;
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
        paintChanged = true;
        break;

    case PNTR_PaintMode::SHAPE_LINE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                drawThickLine(paintLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                paintChanged = true;
            }
            else
            {
                drawThickLine(ghostLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                ghostChanged = true;
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_CIRCLE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                drawCircle(paintLayer, sourceSize, shapeStart, activeColor, shapeSize, drawSize);
                paintChanged = true;
            }
            else
            {
                drawCircle(ghostLayer, sourceSize, shapeStart, activeColor, shapeSize, drawSize);
                ghostChanged = true;
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_SQUARE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown)
            {
                drawSquare(paintLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                paintChanged = true;
            }
            else
            {
                drawSquare(ghostLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                ghostChanged = true;
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

void PNTR_Canvas::drawThickLine(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I p1, PNTR_Vector2I p2, int thickness, SDL_Color *color)
{
    for (float theta = 0; theta < 360; theta++)
    {
        float radians = (float)(theta * M_PI) / 180.0f;
        for (int t = 0; t < thickness / 2; t++)
        {
            PNTR_Vector2I c1 = PNTR_Vector2I(p1.x + SDL_cosf(radians) * t, p1.y + SDL_sinf(radians) * t);
            PNTR_Vector2I c2 = PNTR_Vector2I(p2.x + SDL_cosf(radians) * t, p2.y + SDL_sinf(radians) * t);
            if ((c1.x >= 0 && c1.x < bounds.w) && (c1.y >= 0 && c1.y < bounds.h) && (c2.x >= 0 && c2.x < bounds.w) && (c2.y >= 0 && c2.y < bounds.h))
            {
                drawLine(surface, bounds, c1, c2, color);
            }
        }
    }
}

void PNTR_Canvas::drawLine(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I p1, PNTR_Vector2I p2, SDL_Color *color)
{
    PNTR_Vector2I change = PNTR_Vector2I(abs(p2.x - p1.x), abs(p2.y - p1.y));

    PNTR_Vector2I scale = PNTR_Vector2I(p1.x < p2.x ? 1 : -1, p1.y < p2.y ? 1 : -1);

    int error = (change.x > change.y ? change.x : -change.y) / 2, e2;

    while (true)
    {
        /* draw point only if coordinate is valid */
        if (p1.x >= 0 && p1.x < bounds.w && p1.y >= 0 && p1.y < bounds.h)
        {
            setSurfacePixel(surface, color, p1);
        }

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
    // http://members.chello.at/~easyfilter/bresenham.html
} // https://brightspace.bournemouth.ac.uk/d2l/le/lessons/345037/topics/1968571

void PNTR_Canvas::drawSquare(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I tl, PNTR_Vector2I br, int thickness, SDL_Color *color)
{
    PNTR_Vector2I p1 = PNTR_Vector2I(tl.x, tl.y);
    PNTR_Vector2I p2 = PNTR_Vector2I(br.x, tl.y);
    PNTR_Vector2I p3 = PNTR_Vector2I(br.x, br.y);
    PNTR_Vector2I p4 = PNTR_Vector2I(tl.x, br.y);
    drawThickLine(surface, bounds, p1, p2, thickness, color);
    drawThickLine(surface, bounds, p2, p3, thickness, color);
    drawThickLine(surface, bounds, p3, p4, thickness, color);
    drawThickLine(surface, bounds, p4, p1, thickness, color);
}

void PNTR_Canvas::drawCircle(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I *center, SDL_Color *color, int radius, int thickness)
{
    for (float theta = 0; theta < 361; theta++)
    {
        float r1 = (float)(theta * M_PI) / 180.0f;
        float r2 = (float)((theta - 1) * M_PI) / 180.0f;

        drawThickLine(surface, bounds, PNTR_Vector2I(center->x + radius * cos(r1), center->y + radius * sin(r1)), PNTR_Vector2I(center->x + radius * cos(r2), center->y + radius * sin(r2)), thickness / 2, color);
    }
}