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
    for (int x = 0; x <= WIDTH; x += 16)
        grasses.push_back(new Grass(renderer, x, HEIGHT - 32));
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
    SDL_RenderClear(renderer);
    for (Grass *grass : grasses)
        grass->render();
    SDL_RenderPresent(renderer);
}

void Game::handle()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
        }
    }
}

void Game::terminate()
{
    SDL_Quit();
    active = false;
}