#pragma once

#include "config.h"
#include "../tools/level.h"
#include "../ui/button.h"

class Game 
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    States state = States::playing;
    SDL_Event event;
    Player *player = nullptr;
    Gate *gate = nullptr;
    Carrot *displayCarrot = nullptr;
    vector<Grass *> grasses;
    vector<Carrot *> carrots;
    vector<Spike *> spikes;
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
    vector<Button *> buttons;
    Level *currentLevel = nullptr;
    unordered_map<int, Level *> levels;
    Text *points = nullptr;
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    void loadLevels();
    void clear();
    void handleCollision();
    void updateLevel();
};