#include "../sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, string imageSource, float x, float y)
    : renderer(renderer), image(renderer, imageSource)
{
    Position = Vector2D(x * SCALE, y * SCALE);
    rect.x = Position.x;
    rect.y = Position.y;
    rect.w = image.width * SCALE;
    rect.h = image.height * SCALE;
}

void Sprite::handle(double deltaTime)
{
    Position.x += Vector.x * deltaTime;
    Position.y += Vector.y * deltaTime;
    rect.x = Position.x;
    rect.y = Position.y;
}

void Sprite::render()
{
    SDL_RenderTexture(renderer, image.texture, NULL, &rect);
}