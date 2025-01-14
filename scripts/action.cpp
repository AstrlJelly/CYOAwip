#include "action.hpp"

Line::Line(const char* text)
{
    this->text = text;
}

std::optional<std::string> Line::execute()
{
    for (unsigned int i = 0; i < std::strlen(text); i++) {
        std::cout << text[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    return std::nullopt;
}

Prompt::Prompt()
{

}

std::optional<std::string> Prompt::execute()
{
    return std::nullopt;
}

MoveTo::MoveTo(std::string nodeName)
{
    this->nodeName = nodeName;
}

std::optional<std::string> MoveTo::execute()
{
    return nodeName;
}

CustomAction::CustomAction(std::function<void()> action)
{
    this->action = action;
}

std::optional<std::string> CustomAction::execute()
{
    action();
    return std::nullopt;
}