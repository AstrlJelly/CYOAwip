#pragma once

#include <iostream>
#include <functional>
#include <future>
#include <chrono>
#include <unordered_map>


class Action
{
private:

public:
    virtual std::optional<std::string> execute() {
        std::cerr << "Action::execute should not be called. Hmmmmm\n";
        return std::nullopt;
    };
};

class Text : public Action
{
private:
    std::string text;

public:
    const std::chrono::milliseconds defaultTextSpeed = std::chrono::milliseconds(30);

    Text(std::string text);
    std::optional<std::string> execute() override;
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
    std::optional<std::string> execute() override;
};

class PlaySFX : public Action
{
private:
    std::string pathToSfx;

public:
    PlaySFX();
    std::optional<std::string> execute() override;
};

class MoveTo : public Action
{
private:
    std::string nodeName;

public:
    MoveTo(std::string nodeName);
    std::optional<std::string> execute() override;
};

Action* parseAction(std::string nodeText);
