#include "../heart.h"

Heart::Heart(SDL_Renderer *renderer, float x, float y)
    : Sprite(renderer, "heart.png", x, y) {};

void Heart::render()
{
    Sprite::image = new Image(
        renderer, 
        state.broken ? "assets/images/broken.png" : "assets/images/heart.png"
    );
    Sprite::render();
}