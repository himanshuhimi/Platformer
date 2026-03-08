#pragma once

#include "config.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();
};