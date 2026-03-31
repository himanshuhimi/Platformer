#pragma once

#include "../core/config.h"
#include "../sprites/player.h"
#include "../sprites/gate.h"
#include "../tools/text.h"
#include "../tools/map.h"

class Level
{
public:
    SDL_Renderer *renderer = nullptr;
    Map *map = nullptr;
    string filename;
    Level(SDL_Renderer *renderer, const fs::directory_entry &entry);
    void render();
    template <typename T>
    void updateSprites(T *game)
    {
        for (Map::Object obj : map->objectGroup.objects)
        {
            string name = obj.name;
            if (name == "player")
                game->player = new Player(renderer, obj.x, obj.y - SPRITE_HEIGHT);
            else if (name == "gate")
                game->gate = new Gate(renderer, obj.x, obj.y - SPRITE_HEIGHT);
            else if (name == "carrot")
                game->carrots.push_back(new Carrot(renderer, obj.x, obj.y - SPRITE_HEIGHT));
            else if (name == "grasses")
            {
                auto createGrass = [](SDL_Renderer *r, float x, float y)
                {
                    return new Grass(r, x, y);
                };
                createPlatform<Grass>(obj, createGrass, game->grasses);
            }
            else if (name == "spikes")
            {
                auto createSpike = [](SDL_Renderer *r, float x, float y)
                {
                    return new Spike(r, x, y);
                };
                createPlatform<Spike>(obj, createSpike, game->spikes);
            }
        }
    }

    template <typename T>
    void createPlatform(
        Map::Object obj,
        function<T *(SDL_Renderer *, float, float)> createSprite,
        vector<T *> &sprites)
    {
        int n = obj.width / SPRITE_WIDTH;
        for (int i = 0; i < n; i++)
            sprites.push_back(createSprite(renderer, obj.x + (i * SPRITE_WIDTH), obj.y));
    };
};