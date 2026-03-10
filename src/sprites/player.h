#pragma once

#include "../core/config.h"
#include "../tools/sprite.h"
#include "grass.h"
#include "carrot.h"

class Player : public Sprite
{
public:
    int points = 0;
    float jumpForce{5.0f}, gravity{98.0f};
    bool onGround, spacePressed;
    vector<Grass *> grasses, collidedGrasses;
    vector<Carrot *> carrotsEarned;
    Cooldown jumpCooldown = {0.5f};
    Player(SDL_Renderer *renderer, float x, float y);
    void handle(double deltaTime);
    void render();
};