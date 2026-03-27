#include "../cloud.h"

Cloud::Cloud(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "cloud.png", x, y)
{
    Vector.x = -300;
}