#pragma once

#include "../core/config.h"
#include "../tools/text.h"

class Button
{
public:
    SDL_Renderer *renderer = nullptr;
    Vector2D Position;
    std::function<void()> func;
    Text *text = nullptr;
    float mouseX, mouseY;
    Button(
        SDL_Renderer *renderer,
        float x, float y,
        std::function<void()> func,
        string label,
        SDL_Color color);
    bool hovered(SDL_Event event);
    bool clicked(SDL_Event event);
    void handle(double deltaTime, SDL_Event event);
    void render();
};