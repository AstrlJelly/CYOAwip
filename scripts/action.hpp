#pragma once

#include <iostream>
#include <functional>
#include <future>
#include <chrono>

class Action
{
private:

public:
    virtual std::optional<std::string> execute() { return std::nullopt; };
};

class Line : public Action
{
private:
    const char* text;

public:
    Line(const char* text);
    std::optional<std::string> execute() override;
};

class Prompt : public Action
{
private:
    //std::string nodeName;

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