#ifndef PNTR_CANVAS_H
#define PNTR_CANVAS_H

#include "../common.h"
#include "pntr_widget.h"

class PNTR_Canvas : public PNTR_Widget{
    private:
        SDL_Surface* imageLayer;
        SDL_Surface* paintLayer;
        SDL_Surface* ghostLayer;
        SDL_Texture* texture;

        SDL_Rect* sourceSize;

    public:
        PNTR_Canvas();
        PNTR_Canvas(SDL_Rect* bb, PNTR_Vector2I size, SDL_Color* c);
        PNTR_Canvas(SDL_Rect* bb, SDL_Surface* surface);

        void draw(SDL_Renderer* renderer) override;

        SDL_Surface* getImageLayer();

        SDL_Surface* getPaintLayer();

        SDL_Surface* getGhostLayer();

        SDL_Rect* getSourceSize();
        
        void setImageLayer(SDL_Surface* s);
        
        void resetPaintLayer();
        void resetGhostLayer();

        void clearPaintLayer();
        void clearGhostLayer();

        SDL_Surface* combineLayers();
        void saveImage();
};

#endif