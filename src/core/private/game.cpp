#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Uninitialized: " + (string)SDL_GetError());
    if (!TTF_Init())
        log("TTF Uninitialized: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Uninitialized: " + (string)SDL_GetError());
    fetchMaps();
    points = new Text(
        renderer, std::to_string(player->points),
        WIDTH / 8, HEIGHT / 8,
        WHITE);
    vector<string> labels = {"PLAY", "QUIT"};
    std::unordered_map<string, function<void()>> functions = {
        {"PLAY",
         [this]
         { update(States::playing); }},
        {"QUIT",
         [this]
         { terminate(); }}};
    for (int i = 0; i < labels.size(); i++)
    {
        string label = labels[i];
        float x = WIDTH / 2;
        float y = HEIGHT / 2 + (i * 60);
        buttons.emplace_back(new Button(
            renderer, x, y,
            functions[label],
            label, WHITE));
    }
    displayCarrot = new Carrot(
        renderer,
        points->rect.w * 2,
        points->rect.y + points->rect.h);
    active = true;
}

void Game::handle()
{
    deltaTime = updateDeltaTime();
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            terminate();
        if (state == States::home)
            for (auto button : buttons)
                button->handle(deltaTime, event);
    }
    switch (state)
    {
    case States::playing:
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->handle(deltaTime);
        if (player != nullptr)
            player->handle(deltaTime, grasses);
        handleCollision();
        if (points != nullptr)
            points->update(std::to_string(player->points));
        break;
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    switch (state)
    {
    case States::home:
        for (auto button : buttons)
            button->render();
        break;
    case States::playing:
        SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
        SDL_RenderClear(renderer);
        currentMap->render();
        if (points != nullptr)
        {
            RenderRectangle(
                renderer, GREY,
                points->rect.w * 6,
                points->rect.h,
                points->rect.x - points->rect.w / 2,
                points->rect.y,
                8);
            points->render();
            displayCarrot->render();
        }
        if (gate != nullptr)
            gate->render();
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->render();
        if (player != nullptr)
            player->render();
        break;
    }
    SDL_RenderPresent(renderer);
}

void Game::launch()
{
    while (active)
    {
        handle();
        render();
    }
}

void Game::terminate()
{
    active = false;
}

double Game::updateDeltaTime()
{
    NOW = SDL_GetPerformanceCounter();
    double dt = (double)(NOW - LAST) / SDL_GetPerformanceFrequency();
    LAST = NOW;
    return dt;
}

void Game::update(States newState)
{
    state = newState;
}

void Game::handleCollision()
{
    for (auto it = carrots.begin(); it != carrots.end();)
    {
        Carrot *carrot = *it;
        if (SDL_HasRectIntersectionFloat(&player->rect, &carrot->rect))
        {
            player->carrotsEarned.push_back(carrot);
            carrot->taken = true;
            it = carrots.erase(it);
        }
        else
            ++it;
    }
    for (auto spikeIT = spikes.begin(); spikeIT != spikes.end();)
    {
        Spike *spike = *spikeIT;
        if (SDL_HasRectIntersectionFloat(&player->rect, &spike->rect))
        {
            player->respawn();
            break;
        }
        else
            ++spikeIT;
    }
    if (gate != nullptr)
    {
        if (
            SDL_HasRectIntersectionFloat(&player->rect, &gate->rect) &&
            player->points >= carrots.size())
        {
            level += 1;
            clear();
            updateMap();
            points = new Text(
                renderer, std::to_string(player->points),
                WIDTH / 8, HEIGHT / 8,
                WHITE);
        }
    }
}

void Game::clear()
{
    if (!carrots.empty())
    {
        for (Carrot *carrot : carrots)
            delete carrot;
        carrots.clear();
    }
    if (!spikes.empty())
    {
        for (Spike *spike : spikes)
            delete spike;
        spikes.clear();
    }
    if (!grasses.empty())
    {
        for (Grass *grass : grasses)
            delete grass;
        grasses.clear();
    }
    if (player != nullptr)
    {
        player->carrotsEarned.clear();
        delete player;
        player = nullptr;
    }
    if (gate != nullptr)
    {
        delete gate;
        gate = nullptr;
    }
    if (points != nullptr)
    {
        delete points;
        points = nullptr;
    }
}

void Game::fetchMaps()
{
    for (Map *map : maps)
        delete map;
    maps.clear();
    string mapPath = "maps";
    for (const auto &entry : std::filesystem::directory_iterator(mapPath))
    {
        string path = entry.path().string();
        std::filesystem::path p(path);
        string filename = p.stem().string();
        bool isDigit = !filename.empty() && std::all_of(
                                                filename.begin(),
                                                filename.end(),
                                                [](unsigned char c)
                                                { return std::isdigit(c); });
        if (isDigit)
            maps.emplace_back(new Map(renderer, path));
    }
    updateMap();
}

void Game::updateObjects()
{
    for (Map::Object obj : currentMap->objectGroup.objects)
    {
        string name = obj.name;
        if (name == "player")
            player = new Player(renderer, obj.x, obj.y - SPRITE_HEIGHT);
        else if (name == "gate")
            gate = new Gate(renderer, obj.x, obj.y - SPRITE_HEIGHT);
        else if (name == "carrot")
            carrots.push_back(new Carrot(renderer, obj.x, obj.y - SPRITE_HEIGHT));
        else if (name == "grasses")
        {
            auto createSprite = [](SDL_Renderer *r, float x, float y)
            {
                return new Grass(r, x, y);
            };
            createPlatform<Grass>(obj, createSprite, grasses);
        }
        else if (name == "spikes")
        {
            auto createSprite = [](SDL_Renderer *r, float x, float y)
            {
                return new Spike(r, x, y);
            };
            createPlatform<Spike>(obj, createSprite, spikes);
        }
    }
}

void Game::updateMap()
{
    if (!maps.empty() && currentMap == nullptr)
        currentMap = maps.front();
    for (Map *map : maps)
    {
        std::filesystem::path p(map->source);
        if (p.stem().string() == std::to_string(level))
        {
            currentMap = map;
            updateObjects();
            break;
        }
    }
}

template <typename T>
void Game::createPlatform(
    Map::Object obj,
    function<T *(SDL_Renderer *, float, float)> createSprite,
    vector<T *> &sprites)
{
    int n = obj.width / SPRITE_WIDTH;
    for (int i = 0; i < n; i++)
        sprites.push_back(createSprite(renderer, obj.x + (i * SPRITE_WIDTH), obj.y));
};

Game::~Game()
{
    if (!active)
        return;
    TTF_Quit();
    SDL_Quit();
}