#include "../config.h"

int VERSION = 1.0;

const char *TITLE = "Platformer";
int DEF_WIDTH{640}, DEF_HEIGHT{360}, WIDTH{640}, HEIGHT{360};
int SCALE{(int)sqrt((WIDTH * HEIGHT) / (DEF_WIDTH * DEF_HEIGHT))};
float SPRITE_WIDTH{32.0f}, SPRITE_HEIGHT{32.0f};
SDL_Color WHITE = {255, 255, 255, 255};
SDL_Color GREY = {60, 60, 60, 255};
SDL_Color BLACK = {0, 0, 0, 255};
Image::Image(SDL_Renderer *renderer, string source) : source(source)
{
    texture = IMG_LoadTexture(renderer, source.c_str());
    if (!texture)
        log("Texture Uninitialized: " + source);
    else
        SDL_GetTextureSize(texture, &width, &height);
}
Image::~Image()
{
    if (texture)
        SDL_DestroyTexture(texture);
}
void log(string message)
{
    using std::cout, std::endl;
    cout << "[LOG] " << message << endl;
};
SDL_Texture *createTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface)
{
    if (!surface)
        log("Surface Uninitialized: \n" + (string)SDL_GetError());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
        log("Texture Unconverted: \n" + (string)SDL_GetError());
    return texture;
}
void DrawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; dy++)
        for (int dx = -radius; dx <= radius; dx++)
            if (dx * dx + dy * dy <= radius * radius)
                SDL_RenderPoint(renderer, cx + dx, cy + dy);
}
void RenderRectangle(
    SDL_Renderer *renderer,
    SDL_Color color,
    float width,
    float height,
    float x,
    float y,
    float borderRadius)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    x *= SCALE;
    y *= SCALE;
    width *= SCALE;
    height *= SCALE;
    borderRadius *= SCALE;

    SDL_FRect center = {
        x - width / 2 + borderRadius,
        y,
        width - 2 * borderRadius,
        height};
    SDL_RenderFillRect(renderer, &center);

    SDL_FRect left = {
        x - width / 2,
        y + borderRadius,
        borderRadius,
        height - 2 * borderRadius};
    SDL_RenderFillRect(renderer, &left);

    SDL_FRect right = {
        x + width / 2 - borderRadius,
        y + borderRadius,
        borderRadius,
        height - 2 * borderRadius};
    SDL_RenderFillRect(renderer, &right);

    DrawFilledCircle(renderer,
                     x - width / 2 + borderRadius,
                     y + borderRadius,
                     borderRadius);

    DrawFilledCircle(renderer,
                     x + width / 2 - borderRadius,
                     y + borderRadius,
                     borderRadius);

    DrawFilledCircle(renderer,
                     x - width / 2 + borderRadius,
                     y + height - borderRadius,
                     borderRadius);

    DrawFilledCircle(renderer,
                     x + width / 2 - borderRadius,
                     y + height - borderRadius,
                     borderRadius);
}