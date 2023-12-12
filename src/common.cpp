#include "common.h"
#include "../include/pntr_vector2i.h"

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Event event;

char *readFilePath = nullptr;
char *writeFilePath = nullptr;

PNTR_Vector2I* mousePos = new PNTR_Vector2I();

PNTR_PaintMode paintMode = PNTR_PaintMode::DRAW;

SDL_Color *activeColor = new SDL_Color({0, 0, 0, 255});