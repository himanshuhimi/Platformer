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
    player = new Player(renderer, 32, HEIGHT - dumGrass->rect.h * 2);
    for (int x = 0; x < WIDTH; x += dumGrass->rect.w)
        player->grasses.push_back(new Grass(renderer, x, HEIGHT - dumGrass->rect.h));
    carrots.push_back(new Carrot(
        renderer,
        (float)WIDTH / 2,
        (float)HEIGHT - dumGrass->rect.h));
    delete dumGrass;
    carrots.push_back(new Carrot(
        renderer,
        30,
        96
    ));
    pointsText = new Text(
        renderer,
        std::to_string(player->points),
        80,
        70,
        SDL_Color{255, 255, 255, 255}
    );
    pointsText->pixelSize *= SCALE;
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
    SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
    SDL_RenderClear(renderer);
    player->render();
    for (Grass *grass : player->grasses)
        grass->render();
    RenderRectangle(
        renderer,
        SDL_Color{60, 60, 60, 255},
        80,
        48,
        60,
        60
    );
    pointsText->render();
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
            carrot->taken = true;
            it = carrots.erase(it);
        } else ++it;
    }
    pointsText->update(std::to_string(player->points));
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

Game::~Game()
{
    if (!active) return;
    delete pointsText;
    delete player;
    for (Carrot *carrot : carrots)
        delete carrot;
    carrots.clear();
    TTF_Quit();
    SDL_Quit();
}