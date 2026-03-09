#pragma once

#include "config.h"
#include "../sprites/grass.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active;
    vector<Grass *> grasses;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();
};