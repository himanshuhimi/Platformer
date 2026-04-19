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
#include <string>
#include <map>
#include <windows.h>
#include <sqlite3.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <tinyxml2.h>

using std::string, std::vector, std::function, std::map, std::unordered_map;
using namespace tinyxml2;
using Row = map<string, string>;
using ResultSet = vector<Row>;
namespace fs = std::filesystem;

extern const char *TITLE;
extern int WIDTH, HEIGHT;
extern float scaleX, scaleY, scale, SPRITE_WIDTH, SPRITE_HEIGHT, LABEL_HEIGHT;
extern const int VERSION, DEF_WIDTH, DEF_HEIGHT;
extern SDL_Color WHITE, GREY, BLACK;

struct Image
{
    string source;
    SDL_Texture *texture;
    float width, height;
    Image(SDL_Renderer *renderer, string source);
    ~Image();
};
struct FRect
{
    float originalX, originalY, originalW, originalH;
    float x, y, w, h;
    FRect(float x = 0, float y = 0, float w = 0, float h = 0);
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

void loadScales();
void log(string message);
int randint(int rangeBegin, int rangeEnd);
bool checkRectIntersection(FRect &rectA, FRect &rectB);
bool renderTexture(
    SDL_Renderer *renderer, 
    SDL_Texture *texture, 
    const FRect *srcRect, 
    const FRect &dstRect
);
void DrawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius);
SDL_Texture *createTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface);
FRect RenderRectangle(
    SDL_Renderer *renderer,
    SDL_Color color,
    float width,
    float height,
    float x,
    float y,
    float borderRadius = 0
);

enum States
{
    home,
    playing,
    options,
    paused,
    over,
    completion
};