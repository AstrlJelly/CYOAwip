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

class Prompt : public Action
{
private:
    std::vector<std::string> answers;

public:
    Prompt();
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

class CustomAction : public Action
{
private:
    std::function<void()> action;
public:
    CustomAction(std::function<void()> action);
    std::optional<std::string> execute() override;
};

Action* parseAction(std::string nodeText);
