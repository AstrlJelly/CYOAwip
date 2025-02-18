#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <future>
#include <filesystem>
#include <execution>
#include <csignal>

// use when implementing ;playsfx action
//#define MINIAUDIO_IMPLEMENTATION
//#include "miniaudio/miniaudio.h"

#include "defines.hpp"
#include "action.hpp"
#include "node.hpp"

// `using namespace std` was intentionally left out
// i like how my code looks with the std:: more than without

const variableMap globalDefaultVariables{};
const variableMap nodeDefaultVariables = {
	{ "textdelay", 30 }
};

int main(int argc, char* argv[]);
void mainLoop(nodeDict nodes, std::string beginNode);
void saveSpot(std::string node);