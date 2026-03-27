#pragma once

#include "../tools/sprite.h"

class Cloud : public Sprite
{
public:
    Cloud(SDL_Renderer *renderer, float x, float y);
};