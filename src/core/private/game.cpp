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
    pointsText = new Text(
        renderer, std::to_string(player->points),
        WIDTH / 8, HEIGHT / 8,
        SDL_Color{0, 0, 0, 255});
    carrots.push_back(new Carrot(renderer, WIDTH / 8 - 60, HEIGHT / 8));
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
    if (!carrots.empty())
        for (Carrot *carrot : carrots)
            if (!carrot->taken)
                carrot->render();
    if (!spikes.empty())
        for (Spike *spike : spikes)
            spike->render();
    pointsText->update(std::to_string(player->points));
    if (pointsText != nullptr)
        pointsText->render();
    if (gate != nullptr)
        gate->render();
    if (player != nullptr)
    {
        for (Grass *grass : player->grasses)
            grass->render();
        player->render();
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
    if (!carrots.empty())
        for (Carrot *carrot : carrots)
            if (!carrot->taken)
                carrot->handle(deltaTime);
    if (player != nullptr)
    {
        for (Grass *grass : player->grasses)
            grass->handle(deltaTime);
        player->handle(deltaTime);
    }
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
            player->health = 0;
            player->respawn();
        }
        else
            ++spikeIT;
    }
    if (player->health < 0)
    {
        manageGroups();
        player->respawn();
    }
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
    float grassWidth = dumGrass->image->width;
    float grassHeight = dumGrass->image->height;
    delete dumGrass;
    Spike *dumSpike = new Spike(renderer, 0, 0);
    float spikeWidth = dumSpike->image->width;
    float spikeHeight = dumSpike->image->width;
    delete dumSpike;
    for (Map::Object object : map->objectGroup.objects)
        if (object.name == "player")
            player = new Player(renderer, object.x, object.y);
    for (Map::Object object : map->objectGroup.objects)
    {
        if (object.name == "carrot")
        {
            Carrot *carrot = new Carrot(
                renderer,
                object.x,
                (object.y - grassHeight));
            carrots.emplace_back(carrot);
        }
        else if (object.name == "gate")
            gate = new Gate(renderer, object.x, object.y - grassHeight);
        else if (object.name == "grasses")
        {
            int n = object.width / grassWidth;
            for (int i = 0; i < n; i++)
            {
                Grass *grass = new Grass(
                    renderer,
                    object.x + (i * grassWidth),
                    object.y);
                player->grasses.push_back(grass);
            }
        }
        else if (object.name == "spikes")
        {
            int n = object.width / spikeWidth;
            for (int i = 0; i < n; i++)
            {
                Spike *spike = new Spike(
                    renderer,
                    object.x + (i * spikeWidth),
                    object.y);
                spikes.emplace_back(spike);
            }
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