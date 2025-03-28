#pragma once

#include <iostream>
#include <functional>
#include <future>
#include <chrono>
#include <string>
#include <unordered_map>

#include "defines.hpp"
#include "epic_context.hpp"

//#define EXECUTE_PARAMETERS std::string* nextNodeName, nodeVariableMap nodeVariables, nodeVariableMap globalVariables
#define EXECUTE_FUNC void execute(EpicContext* ctx)
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

#define COMMAND_DEF(commandName, commandClass) { commandName, [](std::string p) { return new commandClass(p); } }

// get at action name
const std::unordered_map<std::string,std::function<Action* (std::string parameters)>> commands = {
    COMMAND_DEF("text",Text),
    COMMAND_DEF("pause",Pause),
    COMMAND_DEF("set",Set),
    COMMAND_DEF("prompt",Prompt),
    COMMAND_DEF("playsfx",PlaySFX),
    COMMAND_DEF("go",MoveTo),
};