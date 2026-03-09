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
    SDL_Texture *dumTex = IMG_LoadTexture(renderer, source.c_str());
    if (!dumTex)
        log("Texture Uninitialized" + source);
    else {
        texture = dumTex;
        SDL_DestroyTexture(dumTex);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_GetTextureSize(texture, &width, &height);
    }
}
Image::~Image()
{
    if (texture)
        SDL_DestroyTexture(texture);
}