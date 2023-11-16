#include "common.h"
#include "src/application.h"

//https://gigi.nullneuron.net/gigilabs/sdl2-pixel-drawing/
int main(int argc, char** argv) {

    SDL_Application app;
    if (!app.init("PAINter", 1280, 720)) {
        return -1;
    }

    return app.run();
}
//end

// Copyright © 2023 Christopher Hosken