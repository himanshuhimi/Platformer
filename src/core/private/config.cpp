#include "../config.h"

int VERSION = 1.0;

const char *TITLE = "Platformer";
int DEF_WIDTH{640}, DEF_HEIGHT{360}, WIDTH{640}, HEIGHT{360};
int SCALE{(int)sqrt((WIDTH * HEIGHT) / (DEF_WIDTH * DEF_HEIGHT))};
Image::Image(SDL_Renderer *renderer, string source) : source(source)
{
    texture = IMG_LoadTexture(renderer, source.c_str());
    if (!texture)
        log("Texture Uninitialized: " + source);
    else
        SDL_GetTextureSize(texture, &width, &height);
    width *= SCALE;
    height *= SCALE;
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
void RenderRectangle(
    SDL_Renderer *renderer,
    SDL_Color color,
    float width,
    float height,
    float x,
    float y
)
{
    SDL_FRect rect = {x - width / 2, y, width, height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}