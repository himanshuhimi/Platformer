#pragma once

#include "config.h"
#include "../sprites/player.h"
#include "../sprites/gate.h"
#include "../tools/text.h"
#include "../tools/map.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active;
    Player *player;
    double deltaTime;
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    vector<Carrot *> carrots;
    Text *pointsText = nullptr;
    Gate *gate;
    Map *map;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();
    void manageGroups();
    double calcDeltaTime();
    ~Game();
};