#pragma once

#include "../core/config.h"

class Sprite
{
public:
    SDL_Renderer *renderer = nullptr;
    Vector2D Vector, Position;
    SDL_FRect rect;
    Image image;
    int speed = 0;
    Sprite(SDL_Renderer *renderer, string imageSource, float x, float y);
    virtual void handle(double deltaTime);
    virtual void render();
};