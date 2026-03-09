#pragma once

#include <iostream>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <tinyxml2.h>

using std::string;
using namespace tinyxml2;

extern const char *TITLE;
extern int WIDTH, HEIGHT;
void log(string message);
struct Image
{
    string source;
    SDL_Texture *texture;
    float width, height;
    Image(SDL_Renderer *renderer, string source);
    ~Image();
};