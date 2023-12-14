#include "include/common.h"
#include "include/pntr_window.h"
#include "include/pntr_gui.h"

int main(int argc, char **argv) {
    for (int c = 0; c < argc; c++)
    {
        if (strcmp(argv[c], "-f") == 0)
        {
            readFilePath = argv[c + 1];
        }

        if (strcmp(argv[c], "-o") == 0)
        {
            writeFilePath = argv[c + 1];
        }
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        // printf("error: %s \n", SDL_GetError());
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            // printf("error: %s \n", SDL_GetError());
            return 1;
        }
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

    PNTR_Window* mainWindow = new PNTR_Window();

    mainWindow->run();
    mainWindow->dispose();

    IMG_Quit();
    SDL_Quit();
    return 0;
}