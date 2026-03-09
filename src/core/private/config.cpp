#include "../config.h"

const char *TITLE = "Platformer";
int WIDTH{640}, HEIGHT{360};
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
    else {
        SDL_GetTextureSize(texture, &width, &height);
    }
}
Image::~Image()
{
    if (texture)
        SDL_DestroyTexture(texture);
}