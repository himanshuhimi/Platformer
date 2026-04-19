#include "../cloud.h"

Cloud::Cloud(SDL_Renderer *renderer)
    : Sprite(
          renderer, "clouds/" + std::to_string(randint(0, 2)) + ".png",
          randint(20, WIDTH), randint(0, HEIGHT))
{
    Vector.x = -randint(20, 50);
}

void Cloud::handle(double deltaTime)
{
    if (Position.x <= -100)
        return;
    Sprite::handle(deltaTime);
}