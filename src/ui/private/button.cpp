#include "../button.h"

Button::Button(
    SDL_Renderer *renderer,
    float x, float y,
    std::function<void()> func,
    string label,
    SDL_Color color) : renderer(renderer), Position(x, y), func(func)
{
    text = new Text(renderer, label, Position.x, Position.y, color);
    Position.x += text->rect.w / 2;
};

bool Button::hovered(SDL_Event event)
{
    SDL_GetMouseState(&mouseX, &mouseY);
    bool x = mouseX >= text->rect.x && mouseX <= (text->rect.x + text->rect.w);
    bool y = mouseY >= text->rect.y && mouseY <= (text->rect.y + text->rect.h);
    return x && y;
}

bool Button::clicked(SDL_Event event)
{

    return (
        event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
        event.button.button == SDL_BUTTON_LEFT && hovered(event));
}

void Button::handle(double deltaTime, SDL_Event event)
{
    if (clicked(event))
        func();
}

void Button::render()
{
    text->render();
}