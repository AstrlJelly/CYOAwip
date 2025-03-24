#pragma once

#include <iostream>
#include <functional>
#include <future>
#include <chrono>
#include <string>
#include <unordered_map>

#include "pdcurses/pdcurses.h"

#include "defines.hpp"

#define EXECUTE_PARAMETERS std::string* nextNodeName, nodeVariableMap nodeVariables, nodeVariableMap globalVariables
#define EXECUTE_FUNC void execute(EXECUTE_PARAMETERS)
#define OVERRIDE_EXECUTE EXECUTE_FUNC override

constexpr auto PROMPT_PREFIX = "?~ ";

class Action
{
private:

public:
    virtual EXECUTE_FUNC {
        std::cerr << "Action::execute should not be called. Hmmmmm\n";
    };
};

class Text : public Action
{
private:
    std::string text;

public:
    Text(std::string text);
    OVERRIDE_EXECUTE;
};

class Pause : public Action
{
private:
    std::chrono::milliseconds pauseTime;

public:
    Pause(std::string parametersStr);
    OVERRIDE_EXECUTE;
};

class Set : public Action
{
private:
    std::string variableName;
    long variableValue;

public:
    Set(std::string parametersStr);
    OVERRIDE_EXECUTE;
};

// unimplemented
class PlaySFX : public Action
{
private:
    std::string pathToSfx;

public:
    PlaySFX(std::string pathToSfx);
    OVERRIDE_EXECUTE;
};

struct Answer
{
    std::string option;
    std::string node;
};

class Prompt : public Action
{
private:
    std::vector<Answer> answers;

public:
    Prompt(std::string parametersStr);
    OVERRIDE_EXECUTE;
};

class MoveTo : public Action
{
private:
    std::string nodeName;

public:
    MoveTo(std::string nodeName);
    OVERRIDE_EXECUTE;
};

Action* parseAction(std::string nodeText);
