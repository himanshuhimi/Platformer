#include "../cloud.h"

Cloud::Cloud(SDL_Renderer *renderer)
    : Sprite(renderer, "cloud.png", randint(20, WIDTH), randint(0, HEIGHT))
{
    Vector.x = -randint(20, 50);
}

void Cloud::handle(double deltaTime)
{
    if (Position.x <= -100)
        return;
    Sprite::handle(deltaTime);
}