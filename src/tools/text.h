#pragma once

#include "../core/config.h"

class Text
{
public:
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *textTexture = nullptr;
    FRect rect;
    SDL_Color color;
    int pixelSize = 0, x = 0, y = 0, alpha = 255;
    bool underlined = false;
    Text(
        SDL_Renderer *renderer,
        string data,
        float x,
        float y,
        SDL_Color color,
        int pixelSize = 24,
        string fontSource = "assets/fonts/normal.ttf",
        bool underlined = false
    );
    void render();
    void updateData(string newData);
    void updateAlpha(int newAlpha);
};