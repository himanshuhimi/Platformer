#include "../text.h"

Text::Text(
    SDL_Renderer *renderer,
    string data,
    float x,
    float y,
    SDL_Color color,
    string fontSource,
    int pixelSize
) 
: renderer(renderer), x(x), y(y), color(color), pixelSize(pixelSize)
{
    pixelSize *= SCALE;
    log(std::to_string(pixelSize));
    font = TTF_OpenFont(fontSource.c_str(), pixelSize);
    if (!font)
        log("Font Uninitialized: " + fontSource);
    textSurface = TTF_RenderText_Blended(
        font, 
        data.c_str(), 
        data.size(),
        color
    );
    textTexture = createTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
    SDL_GetTextureSize(textTexture, &rect.w, &rect.h);
    rect.x = x;
    rect.y = y;
}

void Text::render()
{
    SDL_RenderTexture(renderer, textTexture, nullptr, &rect);
}

void Text::update(string newText)
{
    if (textTexture) SDL_DestroyTexture(textTexture);
    textSurface = TTF_RenderText_Blended(
        font,
        newText.c_str(),
        newText.size(),
        color
    );
    textTexture = createTextureFromSurface(renderer, textSurface);
}