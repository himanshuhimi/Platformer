#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"

class Carrot : public Sprite
{
public:
    bool taken = false;
    Carrot(SDL_Renderer *renderer, float x, float y);
    void render();
};