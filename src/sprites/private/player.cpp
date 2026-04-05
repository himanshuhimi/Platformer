#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player.png", x, y), spawnPos{x, y}
{
    state.movable = true;
    speed = 100 * SCALE;
    jumpForce = 490.0f * SCALE;
    gravity = 980.0f * SCALE;
};

void Player::handle(double deltaTime, const vector<Grass *> &grasses)
{
    if (!state.movable)
        return;
    const bool *keys = SDL_GetKeyboardState(NULL);
    Vector.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]) * speed;
    collidedGrasses.clear();
    for (Grass *grass : grasses)
        if (SDL_HasRectIntersectionFloat(&rect, &grass->rect))
            collidedGrasses.push_back(grass);
    state.jumping = collidedGrasses.empty();
    for (Grass *collided : collidedGrasses)
        if (collided->rect.y > Position.y && Vector.y > 0)
        {
            Position.y = collided->rect.y - rect.h;
            Vector.y = 0;
            state.jumping = false;
        }
    spacePressed = keys[SDL_SCANCODE_SPACE];
    if (!state.jumping && spacePressed && jumpCooldown.available)
    {
        Vector.y -= jumpForce;
        state.jumping = true;
        jumpCooldown.timeElapsed = 0;
        jumpCooldown.available = false;
    }
    if (state.jumping)
        Vector.y += gravity * deltaTime;
    jumpCooldown.timeElapsed += deltaTime;
    jumpCooldown.available = jumpCooldown.timeElapsed >= jumpCooldown.duration;
    if (rect.y >= HEIGHT)
        damage();
    Sprite::handle(deltaTime);
}

void Player::render()
{
    Sprite::render();
}

void Player::respawn()
{
    Position = spawnPos;
    Vector = {0, 0};
}

void Player::damage(int healthPoints)
{
    HP -= healthPoints;
    if (HP <= 0)
        kill();
    else
        respawn();
}

void Player::kill()
{
    Vector = {0, 0};
}