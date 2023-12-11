#include "pntr_circle.h"
#include "../common.h"

PNTR_Circle::PNTR_Circle() : PNTR_Panel()
{
    position = new PNTR_Vector2I();
    radius = 1;
    bbox = new SDL_Rect({position->x - radius, position->y - radius, radius * 2, radius * 2});
}

PNTR_Circle::PNTR_Circle(PNTR_Vector2I *p) : PNTR_Panel()
{
    position = p;
    radius = 1;
    bbox = new SDL_Rect({position->x - radius, position->y - radius, radius * 2, radius * 2});
}

PNTR_Circle::PNTR_Circle(SDL_Rect *bb) : PNTR_Panel(bb)
{
    radius = bbox->w / 2.0f;
    position->x = bbox->x + radius;
    position->y = bbox->y + radius;
}

PNTR_Circle::PNTR_Circle(SDL_Rect *bb, SDL_Color *c) : PNTR_Panel(bb, c)
{
    radius = bbox->w / 2.0f;
    position->x = bbox->x + radius;
    position->y = bbox->y + radius;
}

PNTR_Circle::PNTR_Circle(SDL_Rect *bb, SDL_Color *c, int r) : PNTR_Panel(bb, c, r) {}

PNTR_Circle::PNTR_Circle(PNTR_Vector2I *p, int r) : PNTR_Panel()
{
    position = p;
    radius = r;
    bbox = new SDL_Rect({position->x - radius, position->y - radius, radius * 2, radius * 2});
}

PNTR_Circle::PNTR_Circle(PNTR_Vector2I *p, int r, SDL_Color *c) : PNTR_Panel()
{
    position = p;
    radius = r;
    color = c;
    bbox = new SDL_Rect({position->x - radius, position->y - radius, radius * 2, radius * 2});
}

void PNTR_Circle::setPosition(PNTR_Vector2I *p)
{
    position = p;
}

void PNTR_Circle::draw(SDL_Renderer *renderer)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);

    PNTR_Circle::circleOnSurface(surface, *bbox, *position, *color, radius);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, bbox);
    SDL_DestroyTexture(texture);
}

bool PNTR_Circle::isMouseOver(PNTR_Vector2I *mouse)
{
    return SDL_sqrt(SDL_pow(position->x - mouse->x, 2.0) + SDL_pow(position->y - mouse->y, 2.0)) <= radius;
}

void PNTR_Circle::circleOnSurface(SDL_Surface *surface, SDL_Rect bbox, PNTR_Vector2I position, SDL_Color color, int radius)
{
    int x = radius, y = 0, error = 1 - radius;
    while (x >= y)
    { /* 1 draw call for each octant - ensure coordinates are valid before drawing */
        if ((x + position.x) >= 0 && (x + position.x) < bbox.w && (y + position.y) >= 0 && (y + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(x + position.x, y + position.y));
        /* draw point in octant 1 if coordinate is valid */
        if ((y + position.x) >= 0 && (y + position.x) < bbox.w && (x + position.y) >= 0 && (x + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(y + position.x, x + position.y));
        /* draw point in octant 2 if coordinate is valid */
        if ((-x + position.x) >= 0 && (-x + position.x) < bbox.w && (y + position.y) >= 0 && (y + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(-x + position.x, y + position.y));
        /* draw point in octant 3 if coordinate is valid */
        if ((-y + position.x) >= 0 && (-y + position.x) < bbox.w && (x + position.y) >= 0 && (x + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(-y + position.x, x + position.y));
        /* draw point in octant 4 if coordinate is valid */
        if ((-x + position.x) >= 0 && (-x + position.x) < bbox.w && (-y + position.y) >= 0 && (-y + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(-x + position.x, -y + position.y));
        /* draw point in octant 5 if coordinate is valid */
        if ((-y + position.x) >= 0 && (-y + position.x) < bbox.w && (-x + position.y) >= 0 && (-x + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(-y + position.x, -x + position.y));
        /* draw point in octant 6 if coordinate is valid */
        if ((x + position.x) >= 0 && (x + position.x) < bbox.w && (-y + position.y) >= 0 && (-y + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(x + position.x, -y + position.y));
        /* draw point in octant 7 if coordinate is valid */
        if ((y + position.x) >= 0 && (y + position.x) < bbox.w && (-x + position.y) >= 0 && (-x + position.y) < bbox.h)
            setSurfacePixel(surface, &color, PNTR_Vector2I(y + position.x, -x + position.y));

        /* draw point in octant 8 if coordinate is valid */
        y++; /* increment y coordinate */
        if (error < 0)
        {
            error += 2 * y + 1;
        }
        else
        {
            x--;
            error += 2 * (y - x) + 1;
        }
    }
}

void PNTR_Circle::renderCircle(SDL_Renderer *renderer, SDL_Rect bbox, PNTR_Vector2I position, SDL_Color color, int radius) 
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, bbox.w, bbox.h, 32, SDL_PIXELFORMAT_RGBA8888);

    PNTR_Circle::circleOnSurface(surface, bbox, position, color, radius);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, &bbox);
    SDL_DestroyTexture(texture);
}