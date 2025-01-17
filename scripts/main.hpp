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

// `using namespace std` intentionally left out
// i like how my code looks with the std:: more than without

namespace fs = std::filesystem;

// define it here so it's easy to change and also not super ugly
typedef std::unordered_map<std::string, Node*> nodeDict;

// no magic strings ^.^
const const char* nodesPath = "./nodes/";
const std::string beginNode = "begin";

int main();
void mainLoop(nodeDict nodes);