#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <functional>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <tinyxml2.h>

using std::string, std::vector, std::function, std::unique_ptr, std::make_unique;
using namespace tinyxml2;
namespace fs = std::filesystem;

extern const char *TITLE;
extern int VERSION, DEF_WIDTH, DEF_HEIGHT, WIDTH, HEIGHT, SCALE;
extern float SPRITE_WIDTH, SPRITE_HEIGHT;
extern SDL_Color WHITE;
extern SDL_Color GREY;
extern SDL_Color BLACK;
enum States
{
    home,
    playing,
    paused,
    over
};
struct Image
{
    string source;
    SDL_Texture *texture;
    float width, height;
    Image(SDL_Renderer *renderer, string source);
    ~Image();
};
struct Vector2D
{
    float x = 0.0f, y = 0.0f;
    Vector2D(float x = 0.0f, float y = 0.0f) : x{x}, y{y} {};
};
struct Cooldown
{
    bool available = true;
    float duration{0.0f}, timeElapsed{0.0f};
    Cooldown(float duration) : duration(duration) {};
};
void log(string message);
SDL_Texture *createTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface);
void DrawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius);
void RenderRectangle(
    SDL_Renderer *renderer,
    SDL_Color color,
    float width,
    float height,
    float x,
    float y,
    float borderRadius = 0);
bool isDigit(string data);