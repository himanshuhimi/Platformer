#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Uninitialized: " + (string)SDL_GetError());
    else
        log("SDL Initiliazed");
    if (!TTF_Init())
        log("TTF Uninitialized: " + (string)SDL_GetError());
    else
        log("TTF Initialized");
    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Uninitialized: " + (string)SDL_GetError());
    else
        log("Display Initialized");
    Grass *dumGrass = new Grass(renderer, 0, 0);
    map = new Map(renderer, "maps/1.tmx");
    manageGroups();
    active = true;
}

void Game::launch()
{
    while (active)
    {
        handle();
        render();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
    SDL_RenderClear(renderer);
    map->render();
    if (player != nullptr)
    {
        for (Grass *grass : player->grasses)
            grass->render();
        player->render();
    }
    if (!carrots.empty())
        for (Carrot *carrot : carrots)
            if (!carrot->taken)
                carrot->render();
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
    SDL_RenderPresent(renderer);
}

void Game::handle()
{
    deltaTime = calcDeltaTime();
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
        }
    }
    if (player != nullptr)
    {
        for (Grass *grass : player->grasses)
            grass->handle(deltaTime);
        player->handle(deltaTime);
    }
    if (!carrots.empty())
        for (Carrot *carrot : carrots)
            if (!carrot->taken)
                carrot->handle(deltaTime);
}

void Game::terminate()
{
    active = false;
}

double Game::calcDeltaTime()
{
    NOW = SDL_GetPerformanceCounter();
    double dt = (double)(NOW - LAST) / SDL_GetPerformanceFrequency();
    LAST = NOW;
    return dt;
}

void Game::manageGroups()
{
    Grass *dumGrass = new Grass(renderer, 0, 0);
    float grassWidth = dumGrass->image.width;
    float grassHeight = dumGrass->image.height;
    delete dumGrass;
    for (Map::Object object : map->objectGroup.objects)
        if (object.name == "player")
            player = new Player(renderer, object.x, object.y);
    for (Map::Object object : map->objectGroup.objects)
    {
        if (object.name == "grasses")
        {
            for (int x = 0; x < object.width; x += grassWidth)
            {
                Grass *grass = new Grass(renderer, (object.x + x), object.y );
                player->grasses.emplace_back(grass);
            }
        }
        else if (object.name == "carrot") {
            Carrot *carrot = new Carrot(
                renderer,
                object.x,
                (object.y - grassHeight)
            );
            carrots.emplace_back(carrot);
        }
    }
}

Game::~Game()
{
    if (!active)
        return;
    TTF_Quit();
    SDL_Quit();
}