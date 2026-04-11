#include "../player.h"

Player::Player(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "player/right.png", x, y), spawnPos{x, y}
{
    state.movable = true;
    speed = 100 * scale;
    jumpForce = 490.0f * scale;
    gravity = 980.0f * scale;
    lastDirection = Direction::Right;
    rightImage = new Image(
        renderer,
        "assets/images/player/right.png"
    );
    leftImage = new Image(
        renderer,
        "assets/images/player/left.png"
    );
};

void Player::handle(double deltaTime, const vector<Grass *> &grasses)
{
    if (!state.movable)
        return;
    const bool *keys = SDL_GetKeyboardState(NULL);
    Vector.x = -((int)keys[SDL_SCANCODE_A] - (int)keys[SDL_SCANCODE_D]) * speed;
    if (Vector.x > 0)
        lastDirection = Direction::Right;
    else if (Vector.x < 0)
        lastDirection = Direction::Left;
    collidedGrasses.clear();
    for (Grass *grass : grasses)
        if (checkRectIntersection(rect, grass->rect))
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
    if (lastDirection == Direction::Right)
        image = rightImage;
    else
        image = leftImage;
    Sprite::render();
}

void Player::respawn()
{
    Position = spawnPos;
    Vector = {0, 0};
}

void Player::damage(int deductingPoints)
{
    HP -= deductingPoints;
    if (HP <= 0)
        kill();
    else
        respawn();
}

void Player::kill()
{
    Vector = {0, 0};
}