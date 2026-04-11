#include "../config.h"

const char *TITLE = "Bunny Adventure";

int WIDTH = 0, HEIGHT = 0;
float scaleX = 1, scaleY = 1, scale = 1;
float SPRITE_WIDTH = 0, SPRITE_HEIGHT = 0, LABEL_HEIGHT = 0;
const int VERSION = 1.0, DEF_WIDTH = 640, DEF_HEIGHT = 360;
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
FRect::FRect(float x, float y, float w, float h)
{
    this->x = x * scale;
    this->y = y * scale;
    this->w = w * scale;
    this->h = h * scale;

    originalX = x;
    originalY = y;
    originalW = w;
    originalH = h;
}
void loadScales()
{
    scaleX = (float)WIDTH / DEF_WIDTH;
    scaleY = (float)HEIGHT / DEF_HEIGHT;
    scale = std::min(scaleX, scaleY);
    SPRITE_WIDTH = 32.0f * scaleX;
    SPRITE_HEIGHT = 32.0f * scaleY;
    LABEL_HEIGHT = 60.0f * scaleY;
};
void log(string message)
{
    using std::cout, std::endl;
    cout << "[LOG] " << message << endl;
};
int randint(int rangeBegin, int rangeEnd)
{
    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    return rangeBegin + std::rand() % (rangeEnd - rangeBegin + 1);
};
bool checkRectIntersection(FRect &rectA, FRect &rectB)
{
    SDL_FRect SDL_RectA = SDL_FRect{rectA.x, rectA.y, rectA.w, rectA.h};
    SDL_FRect SDL_RectB = SDL_FRect{rectB.x, rectB.y, rectB.w, rectB.h};
    return SDL_HasRectIntersectionFloat(&SDL_RectA, &SDL_RectB);
};
bool renderTexture(
    SDL_Renderer *renderer,
    SDL_Texture *texture,
    const FRect *srcRect,
    const FRect &dstRect)
{
    SDL_FRect destinationRect = {dstRect.x, dstRect.y, dstRect.w, dstRect.h};

    if (srcRect)
    {
        SDL_FRect sourceRect = {srcRect->x, srcRect->y, srcRect->w, srcRect->h};
        return SDL_RenderTexture(renderer, texture, &sourceRect, &destinationRect);
    }
    else
        return SDL_RenderTexture(renderer, texture, nullptr, &destinationRect);
}
void DrawFilledCircle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    for (int dy = -radius; dy <= radius; dy++)
        for (int dx = -radius; dx <= radius; dx++)
            if (dx * dx + dy * dy <= radius * radius)
                SDL_RenderPoint(renderer, cx + dx, cy + dy);
}
FRect RenderRectangle(
    SDL_Renderer *renderer,
    SDL_Color color,
    float width,
    float height,
    float x,
    float y,
    float borderRadius)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    x *= scaleX;
    y *= scaleY;
    width *= scaleX;
    height *= scaleY;
    borderRadius *= std::min(scaleX, scaleY);

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
    return FRect{center.x, center.y, center.w, center.h};
}
SDL_Texture *createTextureFromSurface(SDL_Renderer *renderer, SDL_Surface *surface)
{
    if (!surface)
        log("Surface Uninitialized: \n" + (string)SDL_GetError());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
        log("Texture Unconverted: \n" + (string)SDL_GetError());
    return texture;
}
