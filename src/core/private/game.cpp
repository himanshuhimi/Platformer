#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Error" + (string)SDL_GetError());
    window = SDL_CreateWindow(TITLE, WIDTH, HEIGHT, 0);
    if (!window)
        log("Window Error");
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
        log("Renderer Error");
    Grass *dumGrass = new Grass(renderer, 0, 0);
    player = new Player(renderer, 32, HEIGHT - dumGrass->rect.h * 2);
    for (int x = 0; x < WIDTH; x += dumGrass->rect.w)
        player->grasses.push_back(new Grass(renderer, x, HEIGHT - dumGrass->rect.h));
    carrots.push_back(new Carrot(
        renderer,
        (float)WIDTH / 2,
        (float)HEIGHT - dumGrass->rect.h));
    delete dumGrass;
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_SetRenderDrawColor(renderer, 2, 4, 118, 255);
    SDL_RenderClear(renderer);
    player->render();
    for (Grass *grass : player->grasses)
        grass->render();
    for (Carrot *carrot : carrots)
        carrot->render();
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
    player->handle(deltaTime);
    for (auto it = carrots.begin(); it != carrots.end();)
    {
        Carrot *carrot = *it;
        if (SDL_HasRectIntersectionFloat(&player->rect, &carrot->rect))
        {
            player->carrotsEarned.push_back(carrot);
            delete carrot;
            it = carrots.erase(it);
        } else ++it;
    }
}

void Game::terminate()
{
    SDL_Quit();
    active = false;
}

double Game::calcDeltaTime()
{
    NOW = SDL_GetPerformanceCounter();
    double dt = (double)(NOW - LAST) / SDL_GetPerformanceFrequency();
    LAST = NOW;
    return dt;
}