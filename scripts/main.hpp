#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <future>
#include <filesystem>

// use when implementing ;playsfx action
//#define MINIAUDIO_IMPLEMENTATION
//#include "miniaudio/miniaudio.h"

#include "action.hpp"
#include "node.hpp"

// `using namespace std` intentionally left out
// i like how my code looks with the std:: more than without

namespace fs = std::filesystem;

// define it here so it's easy to change and also not super ugly
typedef std::unordered_map<std::string, Node*> nodeDict;

// no magic strings/variables ^.^
const const char* nodesPath = "./nodes/";
const std::string beginNode = "begin";
const std::chrono::milliseconds defaultTextSpeed = std::chrono::milliseconds(30);

const variableMap globalDefaultVariables{};
const variableMap nodeDefaultVariables = {
	{ "textdelay", 30 }
};

int main();
void mainLoop(nodeDict nodes);