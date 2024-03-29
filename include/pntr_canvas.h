#ifndef PNTR_CANVAS_H
#define PNTR_CANVAS_H

#include "common.h"
#include "pntr_widget.h"
#include "pntr_vector2i.h"
#include <vector>

using namespace std;

// PNTR_Canvas is where all the painting is done. 

class PNTR_Canvas : public PNTR_Widget{
    private:
        SDL_Surface* imageLayer;
        SDL_Surface* paintLayer;
        SDL_Surface* ghostLayer;
        SDL_Texture* imageLayerTexture;
        SDL_Texture* paintLayerTexture;
        SDL_Texture* ghostLayerTexture;

        bool imageChanged;
        bool paintChanged;
        bool ghostChanged;

        SDL_Rect sourceSize;

        vector<vector<bool>> visit_array = {};
        vector<PNTR_Vector2I> fillStack;

        PNTR_Vector2I shapeStart;

    public:
        PNTR_Canvas();
        PNTR_Canvas(SDL_Rect* bbox, SDL_Color* c);
        PNTR_Canvas(SDL_Surface* surface);

        void draw(SDL_Renderer* renderer) override;

        SDL_Surface* getImageLayer();

        SDL_Surface* getPaintLayer();

        SDL_Surface* getGhostLayer();

        SDL_Rect getSourceSize();
        
        void setImageLayer(SDL_Surface* s);
        
        void resetPaintLayer();
        void resetGhostLayer();

        void clearPaintLayer();
        void clearGhostLayer();

        void recenter();

        void drawOnPaintLayer(int drawSize, PNTR_Vector2I* shapeStart, bool hold);

        SDL_Surface* combineLayers();
        void saveImage();

        bool isValid(PNTR_Vector2I position, SDL_Surface *read, SDL_Color *fill, SDL_Color *pixel);
        void floodFill(PNTR_Vector2I pos, SDL_Surface *read, SDL_Surface *write, SDL_Color *fill, SDL_Color *pixel);

        void drawSquare(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I tl, PNTR_Vector2I br, int thickness, SDL_Color* color);
        void drawThickLine(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I p1, PNTR_Vector2I p2, int thickness, SDL_Color* color);
        void drawLine(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I p1, PNTR_Vector2I p2, SDL_Color* color);
        void drawCircle(SDL_Surface *surface, SDL_Rect bounds, PNTR_Vector2I* center, SDL_Color* color, int radius, int thickness);
        void dilate(SDL_Surface* surface, SDL_Rect bounds, PNTR_Vector2I pos, int thickness);
        
        void destroy();
};

#endif