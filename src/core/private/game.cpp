#include "../game.h"

Game::Game()
{
    // settings = new Settings(new Database("settings"));
    // string winSizeStr = settings->fetch("window");
    // size_t pos = winSizeStr.find('x');
    // WIDTH = std::stoi(winSizeStr.substr(0, pos));
    // HEIGHT = std::stoi(winSizeStr.substr(pos + 1));
    WIDTH = 640;
    HEIGHT = 360;
    if (!SDL_Init(SDL_INIT_VIDEO))
        log("SDL Uninitialized: " + (string)SDL_GetError());
    if (!TTF_Init())
        log("TTF Uninitialized: " + (string)SDL_GetError());
    if (!SDL_CreateWindowAndRenderer(TITLE, WIDTH, HEIGHT, 0, &window, &renderer))
        log("Display Uninitialized: " + (string)SDL_GetError());
    SDL_GetWindowSize(window, &WIDTH, &HEIGHT);
    loadScales();
    ui = new UIElements(this);
    SDL_AddTimer(3000, cloudTimerCallback, this);
    levelUpText = new Text(
        renderer, "LEVEL UP!",
        WIDTH / 2, HEIGHT / 2,
        WHITE,
        60);
    titleText = new Text(
        renderer, TITLE,
        WIDTH / 2, LABEL_HEIGHT,
        GREY,
        48 * scale,
        "assets/fonts/title.ttf",
        true);
    pausedText = new Text(
        renderer, "PAUSED!",
        WIDTH / 2, LABEL_HEIGHT,
        GREY,
        48 * scale);
    completionText = new Text(
        renderer, "COMPLETION!",
        WIDTH / 2, LABEL_HEIGHT,
        GREY,
        24 * scale);
    completionText->rect.y += titleText->rect.y;
    gameOverText = new Text(
        renderer, "GAME OVER!",
        WIDTH / 2, LABEL_HEIGHT,
        GREY, 
        48 * scale
    );
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
        case SDL_EVENT_KEY_UP:
            if (event.key.key == SDLK_ESCAPE && state == States::playing)
                update(States::paused);
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
        for (int i = 0; i < hearts.size(); i++)
        {
            int HP = player->HP;
            if (i >= HP)
            {
                Heart *heart = hearts.at(i);
                heart->state.broken = true;
            }
        }
        if (player->HP <= 0)
            update(States::over);
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 100, 198, 243, 255);
    SDL_RenderClear(renderer);
    for (Cloud *cloud : clouds)
        cloud->render();
    ui->render();
    switch (state)
    {
    case States::home:
        titleText->render();
        break;
    case States::playing:
        for (Heart *heart : hearts)
        {
            RenderRectangle(
                renderer, BLACK,
                heart->rect.w, heart->rect.h,
                heart->rect.x + heart->rect.w / 2, heart->rect.y, 6.0f);
            heart->render();
        }
        if (currentLevel != nullptr)
            currentLevel->render();
        if (gate != nullptr)
        {
            gate->render();
            displayRect = RenderRectangle(
                renderer, BLACK,
                SPRITE_WIDTH * 2, SPRITE_HEIGHT / 2,
                SPRITE_WIDTH / 2 + gate->Position.x,
                gate->Position.y - SPRITE_HEIGHT / 2 - 4,
                6);
        }
        if (!carrots.empty())
            for (Carrot *carrot : carrots)
                if (!carrot->taken)
                    carrot->render();
        if (displayCarrot != nullptr)
            displayCarrot->render();
        if (player != nullptr)
            player->render();
        if (pointsText != nullptr)
            pointsText->render();
        if (carrotsText != nullptr)
            carrotsText->render();
        if (checkRectIntersection(player->rect, gate->rect) &&
            player->points >= totalCarrots && !intermissionComplete)
        {
            intermissionRadius++;
            if (intermissionRadius > HEIGHT)
            {
                intermissionComplete = true;
                intermissionRadius = 0;
            }
            DrawFilledCircle(renderer,
                             (int)WIDTH / 2, (int)HEIGHT / 2,
                             intermissionRadius);
            if (levelUpText != nullptr)
            {
                levelUpTextAlpha++;
                levelUpText->updateAlpha(levelUpTextAlpha);
                levelUpText->render();
            }
        }
        break;
    case States::paused:
        if (pausedText != nullptr)
            pausedText->render();
        break;
    case States::completion:
        if (titleText != nullptr)
            titleText->render();
        if (completionText != nullptr)
            completionText->render();
        break;
    case States::over:
        if (gameOverText != nullptr)
            gameOverText->render();
        break;
    }
    (state == States::playing) ? SDL_HideCursor() : SDL_ShowCursor();
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
    for (Carrot *carrot : carrots)
    {
        if (checkRectIntersection(player->rect, carrot->rect) && !carrot->taken)
        {
            carrot->taken = true;
            player->points++;
            if (pointsText != nullptr)
                pointsText->updateData(std::to_string(player->points));
        }
    }
    for (auto spikeIT = spikes.begin(); spikeIT != spikes.end();)
    {
        Spike *spike = *spikeIT;
        if (checkRectIntersection(player->rect, spike->rect))
        {
            if (player->HP > 0)
                player->damage();
            else
                player->kill();
            break;
        }
        else
            ++spikeIT;
    }
    if (gate != nullptr)
        if (checkRectIntersection(player->rect, gate->rect) &&
            player->points >= totalCarrots)
        {
            player->state.movable = false;
            if (intermissionComplete)
            {
                level++;
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
    if (!clouds.empty())
    {
        for (Cloud *cloud : clouds)
            delete cloud;
        clouds.clear();
    }
    if (!hearts.empty())
    {
        for (Heart *heart : hearts)
            delete heart;
        hearts.clear();
    }
    if (player != nullptr)
    {
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
    totalCarrots = carrots.size();
    if (carrotsText != nullptr)
        carrotsText->updateData(std::to_string(totalCarrots));
    intermissionComplete = false;
    intermissionRadius = 0;
    levelUpTextAlpha = 0;
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
        res = {"PLAY AGAIN", "HOME", "QUIT"};
        break;
    case States::paused:
        res = {"CONTINUE", "HOME", "QUIT"};
        break;
    case States::over:
        res = {"TRY AGAIN", "HOME", "QUIT"};
        break;
    }
    return res;
}

template <typename T>
unordered_map<string, function<void()>> Game::UIElements<T>::getButtonFunctions()
{
    auto playFunc = [this]() {
        game->update(States::playing);
        game->loadLevels();
    };
    return {
        {"PLAY", playFunc}, {"PLAY AGAIN", playFunc}, {"TRY AGAIN", playFunc},
        {"QUIT", [this]() { game->terminate(); }},
        {"HOME", [this]()
         {
             game->update(States::home);
             game->level = 0;
         }},
        {"CONTINUE", [this](){game->update(States::playing);}},
        {"options", [this](){game->update(States::options);}}
    };
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