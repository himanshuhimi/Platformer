#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "assets/images/player.png", x, y)
{
    speed = 300;
};

void Player::handle(double deltaTime)
{
    const bool *keys = SDL_GetKeyboardState(NULL);
    Vector.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]);
    Sprite::handle(deltaTime);
    collidedGrasses.clear();
    for (Grass *grass : grasses)
        if (SDL_HasRectIntersectionFloat(&rect, &grass->rect))
            collidedGrasses.push_back(grass);
    onGround = !collidedGrasses.empty();
    for (Grass *collided : collidedGrasses)
    {
        if (collided->rect.y > Position.y && Vector.y > 0)
        {
            Position.y = collided->rect.y - rect.h;
            Vector.y = 0;
            onGround = true;
        }
    }
    spacePressed = keys[SDL_SCANCODE_SPACE];
    if (onGround && spacePressed && jumpCooldown.available)
    {
        Vector.y -= jumpForce;
        onGround = false;
        jumpCooldown.timeElapsed = 0;
        jumpCooldown.available = false;
    }
    if (!onGround)
        Vector.y += gravity * deltaTime;
    jumpCooldown.timeElapsed += deltaTime;
    jumpCooldown.available = jumpCooldown.timeElapsed >= jumpCooldown.duration;
    points = carrotsEarned.size();
}

void Player::render()
{
    Sprite::render();
    for (Carrot *carrot : carrotsEarned)
        carrot->render();
}