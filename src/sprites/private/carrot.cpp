#include "../carrot.h"

Carrot::Carrot(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "carrot.png", x, y)
{
    rect.y -= rect.h;
}

void Carrot::render()
{
    if (!taken)
        Sprite::render();
}