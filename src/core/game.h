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
    vector<Spike *> spikes;
    vector<Map *> maps;
    Text *pointsText = nullptr;
    Gate *gate;
    Map *currentMap;
    int level = 0;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();
    void loadMaps();
    void manageGroups();
    void clear();
    void updateMap();
    double calcDeltaTime();
    ~Game();
};