#include "action.hpp"

Text::Text(std::string text)
{
    this->text = text;
}

std::optional<std::string> Text::execute()
{
    for (unsigned int i = 0; i < text.size(); i++) {
        std::cout << text[i];
        // eventually make the text speed a node global variable that can be set in an action
        // allows for more slow, dramatic scenes, or quick rambles you're not meant to read
        std::this_thread::sleep_for(defaultTextSpeed);
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


PlaySFX::PlaySFX()
{

}

std::optional<std::string> PlaySFX::execute()
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

// get at action name
const std::unordered_map<std::string, std::function<Action* (std::string parameters)>> commands = {
    { "prompt", [](std::string p) { return new Prompt();  } },
    { "go",     [](std::string p) { return new MoveTo(p); } },
    { "text",   [](std::string p) { return new Text(p);   } },
};

Action* parseAction(std::string nodeText)
{
    if (nodeText[0] != ';') {
        return new Text(nodeText + '\n');
    }
    size_t firstSpaceIndex = nodeText.find(' ');
    std::string actionString = nodeText.substr(1, firstSpaceIndex - 1);

    // can't use [] on a const map because of some accessing rules
    std::function<Action* (std::string parameters)> actionConstructor = commands.find(actionString)->second;
    if (actionConstructor) {
        return actionConstructor(nodeText.substr(firstSpaceIndex + 1));
    }
    return nullptr;
}