#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"
#include "grass.h"
#include "carrot.h"
#include "spike.h"

class Player : public Sprite
{
public:
    int points = 0, level = 0, health = 100;
    float jumpForce, gravity;
    bool onGround, spacePressed;
    vector<Grass *> collidedGrasses;
    vector<Carrot *> carrotsEarned;
    Cooldown jumpCooldown = {0.5f};
    Vector2D spawnPos;
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double deltaTime, const vector<Grass *> &grasses);
    void render();
    void respawn();
};