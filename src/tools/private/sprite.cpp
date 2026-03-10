#include "../sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, string imageSource, float x, float y)
    : renderer(renderer), image(renderer, imageSource)
{
    Position = Vector2D(x, y);
    rect.x = Position.x;
    rect.y = Position.y;
    rect.w = image.width;
    rect.h = image.height;
}

void Sprite::handle(double deltaTime)
{
    Position.x += Vector.x * speed * deltaTime;
    Position.y += Vector.y * speed * deltaTime;
    rect.x = Position.x;
    rect.y = Position.y;
}

void Sprite::render()
{
    SDL_RenderTexture(renderer, image.texture, NULL, &rect);
}