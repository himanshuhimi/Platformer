#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"

class Gate : public Sprite
{
public:
    bool collided;
    Gate(SDL_Renderer *renderer, float x, float y);
};