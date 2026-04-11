#pragma once

#include "config.h"
#include "../tools/level.h"
#include "../tools/database.h"
#include "../tools/settings.h"
#include "../sprites/cloud.h"
#include "../sprites/heart.h"
#include "../ui/button.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    States state = States::home;
    SDL_Event event;
    Player *player = nullptr;
    Gate *gate = nullptr;
    Carrot *displayCarrot = nullptr;
    vector<Grass *> grasses;
    vector<Carrot *> carrots;
    vector<Spike *> spikes;
    vector<Cloud *> clouds;
    vector<Heart *> hearts;
    FRect displayRect;
    Text *pointsText = nullptr, *carrotsText = nullptr, *levelUpText = nullptr,
         *titleText = nullptr, *pausedText = nullptr, *completionText = nullptr,
         *gameOverText = nullptr;
    Database *db = nullptr;
    Settings *settingsData = nullptr;
    bool active = false;
    double deltaTime = 0.0;
    int level = 0, totalCarrots = 0;
    Game();
    void handle();
    void render();
    void launch();
    void terminate();
    void clear();
    double updateDeltaTime();
    void update(States newState);
    ~Game();

private:
    vector<Button *> buttons;
    Level *currentLevel = nullptr;
    unordered_map<int, Level *> levels;
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    const Uint32 CLOUD_EVENT = SDL_RegisterEvents(1);
    bool rectShifted = false, intermissionComplete = false;
    int intermissionRadius = 0, levelUpTextAlpha = 0;
    template <typename T>
    struct UIElements
    {
        T game;
        vector<Button *> buttons;
        UIElements(T game);
        vector<string> getButtonLabels();
        unordered_map<string, function<void()>> getButtonFunctions();
        void loadButtons();
        void handle(SDL_Event);
        void render();
    };
    UIElements<Game *> *ui;
    void loadLevels();
    void handleCollision();
    void updateLevel();
    static Uint32 cloudTimerCallback(void *userdata, SDL_TimerID id, Uint32 interval);
};