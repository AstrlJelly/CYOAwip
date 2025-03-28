#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_set>

#include <boost/filesystem.hpp>
#include <boost/variant.hpp>

namespace fs = boost::filesystem;
//namespace fs = std::filesystem;

// define messy variable types here so they're easy to change
typedef boost::variant<long,bool,std::string> nodeVariable;
typedef std::unordered_map<std::string, nodeVariable> nodeVariableMap;

// no magic strings/variables ^.^
constexpr auto SAVE_PATH = "./save.txt";
const std::string BEGIN_NODE = "start";
//const std::chrono::milliseconds defaultTextSpeed = std::chrono::milliseconds(30);
const long defaultTextDelay = 30L;

inline void throwlog(std::string error)
{
    std::cerr << error << '\n';
    throw std::exception(error.c_str());
}