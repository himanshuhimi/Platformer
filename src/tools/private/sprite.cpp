#include "../sprite.h"

Sprite::Sprite(SDL_Renderer *renderer, string imageSource, float x, float y)
    : renderer(renderer)
{
    Position = Vector2D(x * scale, y * scale);
    rect.x = Position.x;
    rect.y = Position.y;
    image = new Image(renderer, "assets/images/" + imageSource);
    rect.w = image->width * scale;
    rect.h = image->height * scale;
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
    renderTexture(renderer, image->texture, nullptr, rect);
}