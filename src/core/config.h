#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <tinyxml2.h>

using std::string;
using namespace tinyxml2;

extern const char *TITLE;
extern int WIDTH, HEIGHT;
void log(string message);