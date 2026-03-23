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
    SDL_Event event;
    Uint64 NOW;
    Uint64 LAST = SDL_GetPerformanceCounter();
    int level = 0;
    double deltaTime;
    bool active;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();
    double calcDeltaTime();
    ~Game();

private:
    Player *player = nullptr;
    Gate *gate = nullptr;
    Text *pointsText = nullptr;
    Map *currentMap = nullptr;
    vector<Grass *> grasses;
    vector<Carrot *> carrots;
    vector<Spike *> spikes;
    vector<Map *> maps;
    struct UIElements
    {
        vector<Button *> buttons;
    } UI;
    void clear();
    void loadMaps();
    void manageObjects();
    void updateMap();
    void handleCollision();
    template <typename T>
    void generatePlatform(
        Map::Object obj,
        function<T *(SDL_Renderer *, float, float)> createSprite,
        vector<T *> &sprites);
};