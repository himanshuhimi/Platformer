#include "src/core/game.h"

int main()
{
    HANDLE hMutex = CreateMutexA(NULL, TRUE, (string(TITLE) + "Mutex").c_str());
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;
    Game Platformer;
    Platformer.launch();
}