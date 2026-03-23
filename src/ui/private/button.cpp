#include "../button.h"

Button::Button(
    SDL_Renderer *renderer,
    float x, float y,
    std::function<void()> func,
    string label,
    SDL_Color color) : renderer(renderer), Position(x, y), func(func)
{
    text = new Text(renderer, label, Position.x, Position.y, color);
};

bool Button::hovered(SDL_Event event)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    return (text->rect.x == mouseX && text->rect.y == mouseY);
}

bool Button::clicked(SDL_Event event)
{

    return (
        event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
        event.button.button == SDL_BUTTON_LEFT);
}

void Button::handle(double deltaTime, SDL_Event event)
{
    if (hovered(event) && clicked(event))
        func();
}

void Button::render()
{
    text->render();
}