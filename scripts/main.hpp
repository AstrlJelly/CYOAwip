#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <future>
//#include <filesystem>
#include <csignal>

#include <boost/variant.hpp>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

// use when implementing ;playsfx action
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio/miniaudio.h>

//#include "pdcurses/pdcurses.h"

#include "defines.hpp"
#include "action.hpp"
#include "node.hpp"
#include "epic.hpp"

// `using namespace std` was intentionally left out
// i like how my code looks with the std:: more than without

int main(int argc, char* argv[]);
void mainLoop(EpicContext* ctx);
void saveSpot(EpicContext* ctx);