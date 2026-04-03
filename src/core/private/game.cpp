#include "../game.h"

Game::Game()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Uninitialized: " + (string)SDL_GetError());
    if (!TTF_Init())
        log("TTF Uninitialized: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Uninitialized: " + (string)SDL_GetError());
    ui = new UIElements(this);
    SDL_AddTimer(3000, cloudTimerCallback, this);
    active = true;
}

void Game::handle()
{
    deltaTime = updateDeltaTime();
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            terminate();
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            terminate();
            break;
        case SDL_EVENT_USER:
            if (event.type == CLOUD_EVENT)
                clouds.emplace_back(new Cloud(renderer));
            break;
        }
        ui->handle(event);
    }
    for (Cloud *cloud : clouds)
        cloud->handle(deltaTime);
    if (state == States::playing)
    {
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->handle(deltaTime);
        if (player != nullptr)
            player->handle(deltaTime, grasses);
        handleCollision();
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
    SDL_RenderClear(renderer);
    for (Cloud *cloud : clouds)
        cloud->render();
    if (state == States::playing)
    {
        if (currentLevel != nullptr)
            currentLevel->render();
        if (gate != nullptr)
            gate->render();
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->render();
        if (player != nullptr)
            player->render();
    }
    ui->render();
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
    ui->loadButtons();
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
            updateLevel();
        }
    }
}

void Game::loadLevels()
{
    int idx = 0;
    for (const auto &entry : fs::directory_iterator("maps"))
    {
        Level *level = new Level(renderer, entry);
        if (level->filename == "tilesets")
            continue;
        int levelIdx = std::stoi(level->filename);
        levels[levelIdx] = level;
    }
    updateLevel();
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
}

void Game::updateLevel()
{
    auto it = levels.find(level);
    if (it == levels.end())
    {
        update(States::completion);
        currentLevel = nullptr;
        return;
    }
    currentLevel = it->second;
    currentLevel->updateSprites(this);
    player->carrotsEarned.clear();
}

template <typename T>
Game::UIElements<T>::UIElements(T game)
    : game(game)
{
    loadButtons();
}

template <typename T>
void Game::UIElements<T>::loadButtons()
{
    if (!buttons.empty())
        buttons.clear();
    vector<string> labels = getButtonLabels();
    unordered_map<string, function<void()>> functions = getButtonFunctions();
    for (int i = 0; i < labels.size(); i++)
    {
        string label = labels[i];
        Button *button = new Button(
            game->renderer, WIDTH / 2, HEIGHT / 2 + (i * 60),
            functions[label], label, WHITE);
        buttons.emplace_back(button);
    }
}

template <typename T>
vector<string> Game::UIElements<T>::getButtonLabels()
{
    vector<string> res;
    switch (game->state)
    {
    case States::home:
        res = {"PLAY", "QUIT"};
        break;
    case States::completion:
        res = {"HOME"};
        break;
    }
    return res;
}

template <typename T>
unordered_map<string, function<void()>> Game::UIElements<T>::getButtonFunctions()
{
    return {
        {"PLAY", [this]()
         {
             game->update(States::playing);
             game->loadLevels();
         }},
        {"QUIT", [this]()
         { game->terminate(); }},
        {"HOME", [this]()
         {
             game->update(States::home);
             game->level = 0;
         }}};
}

template <typename T>
void Game::UIElements<T>::render()
{
    for (Button *button : buttons)
        if (button != nullptr)
            button->render();
}

template <typename T>
void Game::UIElements<T>::handle(SDL_Event event)
{
    for (Button *button : buttons)
        if (button != nullptr)
            button->handle(game->deltaTime, event);
}

Uint32 Game::cloudTimerCallback(void *userdata, SDL_TimerID id, Uint32 interval)
{
    Game *game = static_cast<Game *>(userdata);
    SDL_Event cloudEvent;
    SDL_zero(cloudEvent);
    cloudEvent.type = game->CLOUD_EVENT;
    SDL_PushEvent(&cloudEvent);
    return randint(0, 3000);
};

Game::~Game()
{
    if (!active)
        return;
    TTF_Quit();
    SDL_Quit();
}