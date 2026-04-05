#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"
#include "../tools/text.h"
#include "grass.h"
#include "carrot.h"
#include "spike.h"

class Player : public Sprite
{
public:
    vector<Grass *> collidedGrasses;
    Cooldown jumpCooldown = {1.0f};
    Vector2D spawnPos;
    int points = 0, HP = 3, dashMultiplier = 4;
    float jumpForce, gravity;
    bool spacePressed, win;
    struct State
    {
        bool movable = false, jumping = false;
    } state;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double deltaTime, const vector<Grass *> &grasses);
    void render();
    void respawn();
    void damage(int healthPoints = 1);
    void kill();
};