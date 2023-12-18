#include "pntr_circle.h"
#include "../include/common.h"
#include "../include/pntr_vector2i.h"

// Constructor methods
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

// these functions rely on the intialize bounding box to be perfectly square, otherwise you may get overflow on the y-axis.
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

// Use the static function circleToSurface() to draw the circle
void PNTR_Circle::draw(SDL_Renderer *renderer)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);
    surface = PNTR_Circle::circleToSurface(radius, color, new PNTR_Vector2I());


    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, NULL, bbox);
    SDL_DestroyTexture(texture);
}

// A circle based isMouseOver function. It checks if thethe distance from the mouse to the circle center is less than the circle radius.
bool PNTR_Circle::isMouseOver(PNTR_Vector2I *mouse)
{
    return SDL_sqrt(SDL_pow(mouse->x - position->x, 2.0) + SDL_pow(mouse->y - position->y, 2.0)) <= radius;
}

// circleToSurface() creates a trimmable circle surface.
SDL_Surface *PNTR_Circle::circleToSurface(int radius, SDL_Color *color, PNTR_Vector2I *trim)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, radius * 2, radius * 2, 32, SDL_PIXELFORMAT_RGBA8888);
    
    PNTR_Vector2I index = PNTR_Vector2I();

    // It iterates through every point on the surface and checks if the point is within the circle.
    // This has the potential to be slow on larger images as it would be checking areas that the circle doesnt fill.

    for (index.y = -radius; index.y <= radius; ++index.y)
    {
        for (index.x = -radius; index.x <= radius; ++index.x)
        {
            if (index.x * index.x + index.y * index.y <= radius * radius)
            {
                PNTR_Vector2I change = PNTR_Vector2I(radius + index.x - trim->x, radius + index.y - trim->y);

                // If the point is within the trimmed image bounds, draw it onto the surface/

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

// renderCircle() takes the fill function and draws it to the renderer
void PNTR_Circle::renderCircle(SDL_Renderer *renderer, PNTR_Vector2I *position, SDL_Color *color, int radius, PNTR_Vector2I *trim)
{
    SDL_Surface *surface = PNTR_Circle::circleToSurface(radius, color, trim);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect dstRect = {position->x - radius, position->y - radius, radius * 2, radius * 2};

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

PNTR_Vector2I *PNTR_Circle::getPosition() { return position; };