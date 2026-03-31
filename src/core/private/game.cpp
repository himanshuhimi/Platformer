#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Uninitialized: " + (string)SDL_GetError());
    if (!TTF_Init())
        log("TTF Uninitialized: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Uninitialized: " + (string)SDL_GetError());
    loadLevels();
    active = true;
}

void Game::handle()
{
    deltaTime = updateDeltaTime();
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            terminate();
    }
    switch (state)
    {
    case States::playing:
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->handle(deltaTime);
        if (player != nullptr)
            player->handle(deltaTime, grasses);
        handleCollision();
        break;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    switch (state)
    {
    case States::playing:
        SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
        SDL_RenderClear(renderer);
        if (currentLevel != nullptr)
            currentLevel->render();
        if (gate != nullptr)
            gate->render();
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->render();
        if (player != nullptr)
            player->render();
        break;
    }
    SDL_RenderPresent(renderer);
}

void Game::launch()
{
    while (active)
    {
        handle();
        render();
    }
}

void Game::terminate()
{
    active = false;
}

double Game::updateDeltaTime()
{
    NOW = SDL_GetPerformanceCounter();
    double dt = (double)(NOW - LAST) / SDL_GetPerformanceFrequency();
    LAST = NOW;
    return dt;
}

void Game::update(States newState)
{
    state = newState;
}

void Game::handleCollision()
{
    for (auto it = carrots.begin(); it != carrots.end();)
    {
        Carrot *carrot = *it;
        if (SDL_HasRectIntersectionFloat(&player->rect, &carrot->rect))
        {
            player->carrotsEarned.push_back(carrot);
            carrot->taken = true;
            it = carrots.erase(it);
        }
        else
            ++it;
    }
    for (auto spikeIT = spikes.begin(); spikeIT != spikes.end();)
    {
        Spike *spike = *spikeIT;
        if (SDL_HasRectIntersectionFloat(&player->rect, &spike->rect))
        {
            player->respawn();
            break;
        }
        else
            ++spikeIT;
    }
    if (gate != nullptr)
    {
        if (
            SDL_HasRectIntersectionFloat(&player->rect, &gate->rect) &&
            player->points >= carrots.size())
        {
            level += 1;
            clear();
            updateLevel();
        }
    }
}

void Game::loadLevels()
{
    int idx = 0;
    for (const auto &entry : fs::directory_iterator("maps"))
    {
        Level *level = new Level(renderer, entry);
        if (level->filename == "tilesets")
            continue;
        int levelIdx = std::stoi(level->filename);
        levels[levelIdx] = level;
    }
    updateLevel();
}

void Game::clear()
{
    if (!carrots.empty())
    {
        for (Carrot *carrot : carrots)
            delete carrot;
        carrots.clear();
    }
    if (!spikes.empty())
    {
        for (Spike *spike : spikes)
            delete spike;
        spikes.clear();
    }
    if (!grasses.empty())
    {
        for (Grass *grass : grasses)
            delete grass;
        grasses.clear();
    }
    if (player != nullptr)
    {
        player->carrotsEarned.clear();
        delete player;
        player = nullptr;
    }
    if (gate != nullptr)
    {
        delete gate;
        gate = nullptr;
    }
}

void Game::updateLevel() 
{
    auto it = levels.find(level);
    if (it == levels.end())
    {
        update(States::completion);
        currentLevel = nullptr;
        return;
    }
    currentLevel = it->second;
    currentLevel->updateSprites(this);
}

Game::~Game()
{
    if (!active)
        return;
    TTF_Quit();
    SDL_Quit();
}