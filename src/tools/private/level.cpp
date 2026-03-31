#include "../level.h"

Level::Level(SDL_Renderer *renderer, const fs::directory_entry &entry)
    : renderer(renderer)
{
    string path = entry.path().string();
    fs::path p(path);
    filename = p.stem().string();
    if (filename == "tilesets")
        return;
    bool isDigit = !filename.empty() &&
                   std::all_of(filename.begin(), filename.end(), [](unsigned char c)
                               { return std::isdigit(c); });
    if (isDigit)
        map = new Map(renderer, path);
    else
        log("Map not loaded");
}

void Level::render()
{
    if (map != nullptr)
        map->render();
    else
        log("map is nullptr");
}