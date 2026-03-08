#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Error" + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, 0, &window, &renderer))
        log("Window Error" + (string)SDL_GetError());
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