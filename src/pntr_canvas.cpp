#include "pntr_canvas.h"
#include "../include/common.h"
#include "../include/pntr_circle.h"
#include "../include/pntr_vector2i.h"
#include <vector>

// Constructor methods
PNTR_Canvas::PNTR_Canvas() : PNTR_Widget()
{
    sourceSize = SDL_Rect({0, 0, 0, 0});
    imageLayer = new SDL_Surface();
    paintLayer = new SDL_Surface();
    ghostLayer = new SDL_Surface();

    // imageChanged, paintChanged, and ghostChanged are all used to speed up the UI display as redrawing unchanged images is computationally expensive and slows down the application considerably.
    imageChanged = true;
    paintChanged = true;
    ghostChanged = true;
    // imageLayerTexture, paintLayerTexture and ghostLayerTexture are also pre-initialized to speed up the ui.
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
    // check if the layer has been changed before updating the texture
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

    // draw the layers

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

// clearing the layers by filling the image with transparency
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

// Combine the layers for the fill tool or saving the image.
SDL_Surface *PNTR_Canvas::combineLayers()
{
    SDL_Surface *output = SDL_CreateRGBSurface(0, sourceSize.w, sourceSize.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_BlitSurface(imageLayer, NULL, output, NULL);
    SDL_BlitSurface(paintLayer, NULL, output, NULL);
    return output;
};


// Save the image by combining the images and saving them out as PAINting.png
void PNTR_Canvas::saveImage()
{
    SDL_Surface *output = combineLayers();
    // if writeFilePath is not initialized, it will default to PAINting.png
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

    // Map the window mouse position to the canvas.

    // (mouse.x - canvas.x) / (canvas.w) * (image.w) 

    PNTR_Vector2I mouseOnCanvas = PNTR_Vector2I(
        ((mousePos->x - getBBox()->x) / (float)getBBox()->w) * getSourceSize().w,
        ((mousePos->y - getBBox()->y) / (float)getBBox()->h) * getSourceSize().h);


    // calculate the shape size based on zoom (to maintain the pixel drawing inside the cursor)
    int shapeSize = sqrt(pow(shapeStart->x - mouseOnCanvas.x, 2) + pow(shapeStart->y - mouseOnCanvas.y, 2));

    SDL_Surface *output = combineLayers();

    SDL_Color *pixel;
    // setup the brush
    SDL_Rect *circleBrushRect = new SDL_Rect({0, 0, drawSize, drawSize});
    SDL_Surface *circleSurface = SDL_CreateRGBSurfaceWithFormat(0, circleBrushRect->w, circleBrushRect->h, 32, SDL_PIXELFORMAT_RGBA8888);

    // check the paint mode
    switch (paintMode)
    {
    case PNTR_PaintMode::DRAW:
        // Copy a circle with color "activeColor" to the paintLayer
        circleSurface = PNTR_Circle::circleToSurface(drawSize / 2, activeColor, new PNTR_Vector2I());
        SDL_BlitSurface(circleSurface, NULL, paintLayer, new SDL_Rect({mouseOnCanvas.x - drawSize / 2, mouseOnCanvas.y - drawSize / 2, drawSize, drawSize}));
        paintChanged = true;
        break;

    case PNTR_PaintMode::ERASE:
        // Copy a circle with color "transparent" to the paintLayer
        SDL_SetSurfaceBlendMode(circleSurface, SDL_BLENDMODE_BLEND);
        circleSurface = PNTR_Circle::circleToSurface(drawSize / 2, new SDL_Color({255, 255, 255, 0}), new PNTR_Vector2I);
        SDL_BlitSurface(circleSurface, NULL, paintLayer, new SDL_Rect({mouseOnCanvas.x - drawSize / 2, mouseOnCanvas.y - drawSize / 2, drawSize, drawSize}));
        shapeStart = &mouseOnCanvas;
        paintChanged = true;
        break;

    case PNTR_PaintMode::FILL:
        // Check that click was in the image bounds
        if ((mouseOnCanvas.x < 0 || mouseOnCanvas.x > getSourceSize().w) || (mouseOnCanvas.y < 0 || mouseOnCanvas.y > getSourceSize().h))
            break;

        // Get the clicked pixel and add it to the queue.
        pixel = getSurfacePixel(output, PNTR_Vector2I(mouseOnCanvas.x, mouseOnCanvas.y));
        fillStack.clear();
        fillStack.push_back(PNTR_Vector2I({mouseOnCanvas.x, mouseOnCanvas.y}));
        
        // The flood fill uses an array called "visit_array", which checks if a pixel has already been added to the queue. This sets the size of the array to fit the image.
        visit_array.resize(getImageLayer()->w, std::vector<bool>(getImageLayer()->h, false));
        while (!fillStack.empty()) // While the fillstack isnt empty,
        {
            // Get the first element in the array and take it out, then run the flood fill on it.
            currentPos = fillStack.front();
            fillStack.erase(fillStack.begin());
            fillStack.shrink_to_fit();
            PNTR_Canvas::floodFill(currentPos, output, getPaintLayer(), activeColor, pixel);
        }
        // Once finished, clear the array and stack.
        fillStack.clear();
        fillStack.shrink_to_fit();
        visit_array.clear();
        visit_array.resize(getImageLayer()->w, std::vector<bool>(getImageLayer()->h, false));
        paintChanged = true;
        break;

    case PNTR_PaintMode::SHAPE_LINE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown) // Check if the left mouse button isnt down.
            {
                // Draw the line onto the paint layer
                drawThickLine(paintLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                paintChanged = true;
            }
            else
            {
                // Draw the line onto the ghost layer
                drawThickLine(ghostLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                ghostChanged = true;
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_CIRCLE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown) // Check if the left mouse button isnt down.
            {
                // Draw the circle onto the paint layer
                drawCircle(paintLayer, sourceSize, shapeStart, activeColor, shapeSize, drawSize);
                paintChanged = true;
            }
            else
            {
                // Draw the circle onto the ghost layer
                drawCircle(ghostLayer, sourceSize, shapeStart, activeColor, shapeSize, drawSize);
                ghostChanged = true;
            }
        }
        break;
    case PNTR_PaintMode::SHAPE_SQUARE:
        if (!middleMouseDown)
        {
            if (!leftMouseDown) // Check if the left mouse button isnt down.
            {
                // Draw the square onto the paint layer
                drawSquare(paintLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                paintChanged = true;
            }
            else
            {
                // Draw the square onto the ghost layer
                drawSquare(ghostLayer, sourceSize, *shapeStart, mouseOnCanvas, drawSize, activeColor);
                ghostChanged = true;
            }
        }
        break;
    default:
        break;
    }
    // Clear the surfaces for memory management
    SDL_FreeSurface(output);
    SDL_FreeSurface(circleSurface);
}

// FILL FUNCTIONS

void PNTR_Canvas::recenter()
{
    // Get the bounding box of the image and move it into the center of the app.
    getBBox()->x = (APP_WIDTH - getSourceSize().w) / 2;
    getBBox()->y = (APP_HEIGHT - getSourceSize().h) / 2;
    getBBox()->w = getSourceSize().w;
    getBBox()->h = getSourceSize().h;
}

// isValid() checks the pixel during flood fill. If the pixel is within the bounds, and needs to be filled, return True
bool PNTR_Canvas::isValid(PNTR_Vector2I position, SDL_Surface *read, SDL_Color *fill, SDL_Color *pixel)
{
    return (position.x >= 0 && position.x < getSourceSize().w) && (position.y >= 0 && position.y < getSourceSize().h) && !compare(fill, pixel) && compare(getSurfacePixel(read, position), pixel);
}

// The main flood fill algorithm
void PNTR_Canvas::floodFill(PNTR_Vector2I pos, SDL_Surface *read, SDL_Surface *write, SDL_Color *fill, SDL_Color *pixel)
{
    // Check if the current pixel is valid for filling
    if (!isValid(pos, read, fill, pixel) && !visit_array[pos.x][pos.y])
        return;

    // Fill the pixel
    setSurfacePixel(write, fill, pos);

    // Set the visit_array at the pixel to true, so that no other fill instances will go over the same point.
    visit_array[pos.x][pos.y] = true;

    // Then, add the pixel positions next to the current pixel (as long as they're valid).
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
    // Eventually, all avaliable pixels will be added to the stack, and the stack size will eventually return to 0.
}

void PNTR_Canvas::drawThickLine(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I p1, PNTR_Vector2I p2, int thickness, SDL_Color *color)
{
    // iterate through every angle
    for (float theta = 0; theta < 360; theta++)
    {
        float radians = (float)(theta * M_PI) / 180.0f;
        // Iterate through every level of thickness (THIS IS WHY IT'S SLOW)
        for (int t = 0; t < thickness / 2; t++)
        {
            // Get the new points to draw a line between.
            PNTR_Vector2I c1 = PNTR_Vector2I(p1.x + SDL_cosf(radians) * t, p1.y + SDL_sinf(radians) * t);
            PNTR_Vector2I c2 = PNTR_Vector2I(p2.x + SDL_cosf(radians) * t, p2.y + SDL_sinf(radians) * t);
            // Check if the points are within the image bounds (yes, there are issues when drawing lines at boundaries)
            if ((c1.x >= 0 && c1.x < bounds.w) && (c1.y >= 0 && c1.y < bounds.h) && (c2.x >= 0 && c2.x < bounds.w) && (c2.y >= 0 && c2.y < bounds.h))
            {
                // Draw the line
                drawLine(surface, bounds, c1, c2, color);
            }
        }
    }
}
/* Source based on Eike Anderson starts here : https://brightspace.bournemouth.ac.uk/d2l/le/lessons/345037/topics/1968571 */
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
} 
/* Source based on Eike Anderson ends here*/

void PNTR_Canvas::drawSquare(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I tl, PNTR_Vector2I br, int thickness, SDL_Color *color)
{
    // Get the 4 corners of the square
    PNTR_Vector2I p1 = PNTR_Vector2I(tl.x, tl.y);
    PNTR_Vector2I p2 = PNTR_Vector2I(br.x, tl.y);
    PNTR_Vector2I p3 = PNTR_Vector2I(br.x, br.y);
    PNTR_Vector2I p4 = PNTR_Vector2I(tl.x, br.y);
    // Draw 4 thick lines between them
    drawThickLine(surface, bounds, p1, p2, thickness, color);
    drawThickLine(surface, bounds, p2, p3, thickness, color);
    drawThickLine(surface, bounds, p3, p4, thickness, color);
    drawThickLine(surface, bounds, p4, p1, thickness, color);
}

void PNTR_Canvas::drawCircle(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I *center, SDL_Color *color, int radius, int thickness)
{
    // Iterate through every degree
    for (float theta = 0; theta < 361; theta++)
    {
        float r1 = (float)(theta * M_PI) / 180.0f;
        float r2 = (float)((theta - 1) * M_PI) / 180.0f;

        // Draw a line between each degree. Because its only 1 degree, nobody can see that a segmented circle is being drawn.
        drawThickLine(surface, bounds, PNTR_Vector2I(center->x + radius * cos(r1), center->y + radius * sin(r1)), PNTR_Vector2I(center->x + radius * cos(r2), center->y + radius * sin(r2)), thickness / 2, color);
    }
}

void PNTR_Canvas::destroy()
{
    // Destory all the textures and free all the surfaces, this is to make sure that memory is disposed of correctly.
    SDL_DestroyTexture(imageLayerTexture);
    SDL_DestroyTexture(paintLayerTexture);
    SDL_DestroyTexture(ghostLayerTexture);

    SDL_FreeSurface(imageLayer);
    SDL_FreeSurface(paintLayer);
    SDL_FreeSurface(ghostLayer);
}