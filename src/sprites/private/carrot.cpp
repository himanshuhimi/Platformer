#include "../carrot.h"

Carrot::Carrot(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "assets/images/carrot.png", x, y)
{
    rect.w *= 2;
    rect.h *= 2;
    rect.y -= rect.h;
}