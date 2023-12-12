#include "pntr_circle.h"
#include "../include/common.h"
#include "../include/pntr_vector2i.h"

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
    bbox = new SDL_Rect({position->x - radius, position->y, radius*2, radius*2});
}

void PNTR_Circle::draw(SDL_Renderer *renderer, bool fill)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);
    if (fill)
    {
        surface = PNTR_Circle::fillCircle(radius, color, new PNTR_Vector2I());
    }
    else
    {
        PNTR_Circle::circleOnSurface(surface, bbox, position, color, radius, true);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, bbox);
    SDL_DestroyTexture(texture);
}

bool PNTR_Circle::isMouseOver(PNTR_Vector2I *mouse)
{
    return SDL_sqrt(SDL_pow(position->x - mouse->x, 2.0) + SDL_pow(position->y - mouse->y, 2.0)) <= radius;
}

SDL_Surface *PNTR_Circle::fillCircle(int radius, SDL_Color *color, PNTR_Vector2I *trim)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);
    
    PNTR_Vector2I index = PNTR_Vector2I();

    for (index.y = -radius; index.y <= radius; ++index.y)
    {
        for (index.x = -radius; index.x <= radius; ++index.x)
        {
            if (index.x * index.x + index.y * index.y <= radius * radius)
            {
                PNTR_Vector2I change = PNTR_Vector2I(radius + index.x - trim->x, radius + index.y - trim->y);
                if (change.x >= 0 && change.x < surface->w && change.y >= 0 && change.y < surface->h)
                {
                    // Calculate the pixel offset

                    Uint32 *pixels = (Uint32 *)surface->pixels;
                    int pixelIndex = change.y * (surface->pitch / sizeof(Uint32)) + change.x;

                    // Create the pixel value combining RGBA components
                    Uint32 pixelValue = SDL_MapRGBA(surface->format, color->r, color->g, color->b, color->a);

                    // Set the pixel value
                    pixels[pixelIndex] = pixelValue;
                }
            }
        }
    }
    return surface;
}

void PNTR_Circle::circleOnSurface(SDL_Surface *surface, SDL_Rect *bbox, PNTR_Vector2I *position, SDL_Color *color, int radius, bool fill)
{
    if (!fill)
    {
        int x = radius, y = 0, error = 1 - radius;
        while (x >= y)
        { /* 1 draw call for each octant - ensure coordinates are valid before drawing */
            if ((x + position->x) >= 0 && (x + position->x) < bbox->w && (y + position->y) >= 0 && (y + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(x + position->x, y + position->y));
            /* draw point in octant 1 if coordinate is valid */
            if ((y + position->x) >= 0 && (y + position->x) < bbox->w && (x + position->y) >= 0 && (x + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(y + position->x, x + position->y));
            /* draw point in octant 2 if coordinate is valid */
            if ((-x + position->x) >= 0 && (-x + position->x) < bbox->w && (y + position->y) >= 0 && (y + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(-x + position->x, y + position->y));
            /* draw point in octant 3 if coordinate is valid */
            if ((-y + position->x) >= 0 && (-y + position->x) < bbox->w && (x + position->y) >= 0 && (x + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(-y + position->x, x + position->y));
            /* draw point in octant 4 if coordinate is valid */
            if ((-x + position->x) >= 0 && (-x + position->x) < bbox->w && (-y + position->y) >= 0 && (-y + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(-x + position->x, -y + position->y));
            /* draw point in octant 5 if coordinate is valid */
            if ((-y + position->x) >= 0 && (-y + position->x) < bbox->w && (-x + position->y) >= 0 && (-x + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(-y + position->x, -x + position->y));
            /* draw point in octant 6 if coordinate is valid */
            if ((x + position->x) >= 0 && (x + position->x) < bbox->w && (-y + position->y) >= 0 && (-y + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(x + position->x, -y + position->y));
            /* draw point in octant 7 if coordinate is valid */
            if ((y + position->x) >= 0 && (y + position->x) < bbox->w && (-x + position->y) >= 0 && (-x + position->y) < bbox->h)
                setSurfacePixel(surface, color, PNTR_Vector2I(y + position->x, -x + position->y));

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
    else
    {
        surface = PNTR_Circle::fillCircle(radius, color, new PNTR_Vector2I(0, 0));
    }
}

void PNTR_Circle::renderCircle(SDL_Renderer *renderer, SDL_Rect *bbox, PNTR_Vector2I *position, SDL_Color *color, int radius)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, bbox->w, bbox->h, 32, SDL_PIXELFORMAT_RGBA8888);
    PNTR_Circle::circleOnSurface(surface, bbox, position, color, radius, true);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, bbox);
    SDL_DestroyTexture(texture);
}

void PNTR_Circle::renderCircle(SDL_Renderer *renderer, PNTR_Vector2I *position, SDL_Color *color, int radius, PNTR_Vector2I *trim)
{
    SDL_Surface *surface = PNTR_Circle::fillCircle(radius, color, trim);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {position->x - radius, position->y - radius, radius * 2, radius * 2};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

SDL_Surface* PNTR_Circle::circleToSurface(PNTR_Vector2I* position, SDL_Color* color, int radius) {
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);

    int i;
    int j;

    for (i = -radius; i <= radius; ++i)
    {
        for (j = -radius; j <= radius; ++j)
        {
            if (j * j + i * i <= radius * radius)
            {
                int dX = radius + j;
                int dY = radius + i;
                if (dX >= 0 && dX < surface->w && dY >= 0 && dY < surface->h)
                {
                    // Calculate the pixel offset

                    Uint32 *pixels = (Uint32 *)surface->pixels;
                    int pixelIndex = dY * (surface->pitch / sizeof(Uint32)) + dX;

                    // Create the pixel value combining RGBA components
                    Uint32 pixelValue = SDL_MapRGBA(surface->format, color->r, color->g, color->b, color->a);

                    // Set the pixel value
                    pixels[pixelIndex] = pixelValue;
                }
            }
        }
    }
    return surface;
}

PNTR_Vector2I *PNTR_Circle::getPosition() { return position; };

// Copyright © 2024 Christopher Hosken