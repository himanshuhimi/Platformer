#include "../carrot.h"

Carrot::Carrot(SDL_Renderer *renderer, float x, float y, float scale)
    : Sprite(renderer, "carrot.png", x, y)
{
    rect.w *= scale;
    rect.h *= scale;
    rect.y -= rect.h;
}

void Carrot::handle(double deltaTime)
{
    if (!taken)
        Sprite::handle(deltaTime);
}

void Carrot::render()
{
    if (!taken)
        Sprite::render();
}