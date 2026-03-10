#include "../config.h"

const char *TITLE = "Platformer";
int DEF_WIDTH{640}, DEF_HEIGHT{360}, WIDTH{1920}, HEIGHT{1080};
int SCALE{(int)sqrt((WIDTH * HEIGHT) / (DEF_WIDTH * DEF_HEIGHT))};
void log(string message)
{
    using std::cout, std::endl;
    cout << "[LOG] " << message << endl;
};
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