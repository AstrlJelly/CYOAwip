#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <future>
#include <filesystem>

#include "action.hpp"
#include "node.hpp"

namespace fs = std::filesystem;

typedef std::unordered_map<std::string, Node*> nodeDict;

const char* nodesPath = "./nodes/";

int main();
void mainLoop(nodeDict nodes);