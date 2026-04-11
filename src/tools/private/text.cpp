#include "../text.h"

Text::Text(
    SDL_Renderer *renderer,
    string data,
    float x,
    float y,
    SDL_Color color,
    int pixelSize,
    string fontSource,
    bool underlined)
    : renderer(renderer), x(x), y(y),
      pixelSize(pixelSize), color(color), underlined(underlined)
{
    pixelSize *= scale;
    font = TTF_OpenFont(fontSource.c_str(), pixelSize);
    if (!font)
        log("Font Uninitialized: " + fontSource);
    textSurface = TTF_RenderText_Blended(
        font,
        data.c_str(),
        data.size(),
        color);
    textTexture = createTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
    SDL_GetTextureSize(textTexture, &rect.w, &rect.h);
    rect.x = x - rect.w / 2;
    rect.y = y;
}

void Text::render()
{
    renderTexture(renderer, textTexture, nullptr, rect);
    if (underlined)
    {
        int y = rect.y + rect.h;
        int x = rect.x;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderLine(renderer, x, y, x + rect.w, y);
    }
}

void Text::updateData(string newData)
{
    if (textTexture)
        SDL_DestroyTexture(textTexture);
    textSurface = TTF_RenderText_Blended(
        font,
        newData.c_str(),
        newData.size(),
        color);
    textTexture = createTextureFromSurface(renderer, textSurface);
}

void Text::updateAlpha(int newAlpha)
{
    alpha = newAlpha;
    if (textTexture)
        SDL_SetTextureAlphaMod(textTexture, alpha);
}