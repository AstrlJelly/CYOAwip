#pragma once

#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

// define maps here so they're easy to change
// (and also not super ugly.)
typedef std::unordered_map<std::string, long> variableMap;

// no magic strings/variables ^.^
constexpr auto SAVE_PATH = "./save.txt";
const std::chrono::milliseconds defaultTextSpeed = std::chrono::milliseconds(30);
const std::string BEGIN_NODE = "begin";
