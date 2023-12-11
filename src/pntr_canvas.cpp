#include "pntr_canvas.h"
#include "../common.h"

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