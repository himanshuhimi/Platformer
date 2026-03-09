#pragma once

#include "../core/config.h"

class Map
{
public:
    SDL_Renderer *renderer = nullptr;
    int width, height, tileWidth, tileHeight;
    struct Tileset
    {
        int firstGID;
        std::unordered_map<int, Image> images;
    };
    XMLElement *mapElement;
    Map(SDL_Renderer *renderer, string source);
    void loadLayer(XMLElement *child);
    void loadTileset(XMLElement *child);
};