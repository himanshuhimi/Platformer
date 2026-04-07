#pragma once

#include "../tools/sprite.h"

class Heart : public Sprite
{
public:
    struct States
    {
        bool broken;
    } state;
    Heart(SDL_Renderer *renderer, float x, float y);
    void render();
};