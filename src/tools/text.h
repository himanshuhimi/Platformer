#pragma once

#include "../core/config.h"

class Text
{
public:
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *textTexture = nullptr;
    SDL_FRect rect;
    SDL_Color color;
    int pixelSize = 0, x = 0, y = 0;
    Text(
        SDL_Renderer *renderer,
        string data,
        float x,
        float y,
        SDL_Color color,
        string fontSource = "assets/fonts/normal.ttf",
        int pixelSize = 24);
    void render();
    void update(string newText);
};