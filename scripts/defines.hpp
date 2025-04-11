#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_set>

#include <boost/filesystem.hpp>
#include <boost/variant.hpp>

namespace fs = boost::filesystem;

// video settings
const std::string WINDOW_TITLE = "CYOA (Work In Progress)";
const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;

// no magic strings/variables ^.^
constexpr auto SAVE_PATH = "./save.txt";

// this will eventually be defined in the epic's json
const std::string BEGIN_NODE = "start";

//const std::chrono::milliseconds defaultTextSpeed = std::chrono::milliseconds(30);
const long defaultTextDelay = 30L;

inline void throwlog(std::string error)
{
    std::cerr << error << '\n';
    throw std::exception(error.c_str());
}