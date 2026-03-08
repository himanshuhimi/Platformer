#include "../config.h"

const char *TITLE = "Platformer";
int WIDTH{640}, HEIGHT{360};
void log(string message)
{
    using std::cout, std::endl;
    cout << "[LOG] " << message << endl;
};