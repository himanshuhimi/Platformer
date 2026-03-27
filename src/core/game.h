#pragma once

#include "config.h"
#include "../sprites/player.h"
#include "../sprites/gate.h"
#include "../tools/text.h"
#include "../tools/map.h"
#include "../ui/button.h"

class Game 
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    States state = States::home;
    SDL_Event event;
    bool active = false;
    double deltaTime = 0.0;
    int level = 0;
    Game();
    void handle();
    void render();
    void launch();
    void terminate();
    double updateDeltaTime();
    void update(States newState);
    ~Game();

private:
    Player *player = nullptr;
    Gate *gate = nullptr;
    Carrot *displayCarrot = nullptr;
    vector<Grass *> grasses;
    vector<Carrot *> carrots;
    vector<Spike *> spikes;
    vector<Button *> buttons;
    vector<Map *> maps;
    Text *points = nullptr;
    Map *currentMap = nullptr;
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    void clear();
    void fetchMaps();
    void updateObjects();
    void updateMap();
    void handleCollision();
    template <typename T>
    void createPlatform(
        Map::Object obj,
        function<T *(SDL_Renderer *, float, float)> createSprite,
        vector<T *> &sprites
    );
};