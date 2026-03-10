#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"

class Carrot : public Sprite
{
public:
    Carrot(SDL_Renderer *renderer, float x, float y);
};