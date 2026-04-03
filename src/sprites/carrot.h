#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"

class Carrot : public Sprite
{
public:
    bool taken = false;
    Carrot(SDL_Renderer *renderer, float x, float y, float scale = 1);
    void render();
    void handle(double deltaTime);
};