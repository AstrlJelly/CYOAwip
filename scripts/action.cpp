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

// bitmasks for prompt parameter parsing state machines
constexpr int OPTION_LAYER           = 1 << 0;
constexpr int NODE_LAYER             = 1 << 1;
constexpr int LEFT_WHITESPACE_LAYER  = 1 << 2;
constexpr int RIGHT_WHITESPACE_LAYER = 1 << 3;
constexpr int CANCEL_LAYER           = 1 << 4;

Prompt::Prompt(std::string parametersStr)
{
    std::string currentOption{};
    std::string currentNode{};
    int stateMask = 0;
    int lastWhitespaceIndex = 0;

    for (unsigned int i = 0; i < parametersStr.length(); i++)
    {
        char currentChar = parametersStr[i];

        if ((stateMask & CANCEL_LAYER) == 0)
        {
            switch (currentChar) {
            case '{':
                stateMask |= OPTION_LAYER | LEFT_WHITESPACE_LAYER;
                break;
            case ',':
                stateMask |= NODE_LAYER | LEFT_WHITESPACE_LAYER;
                break;
            case '}':
                if (currentOption.length() > 0 && currentNode.length() > 0)
                {
                    answers.push_back(Answer(currentOption, currentNode));
                }
                else
                {
                    std::cerr << "Prompt with parameters \"" << parametersStr << "\" failed... Youch!";
                }
                currentOption.clear();
                currentNode.clear();
                stateMask = 0;
                break;
            case '\\':
                stateMask |= CANCEL_LAYER;
                break;
            }
        }
        else
        {
            stateMask &= ~CANCEL_LAYER;
        }

        if ((stateMask & LEFT_WHITESPACE_LAYER) == 0)
        {
            if (currentChar != ' ')
            {
                stateMask &= ~LEFT_WHITESPACE_LAYER;
            }
        }
        else
        {
            if ((stateMask & OPTION_LAYER) == 0)
            {
                currentOption += currentChar;
            }
            else if ((stateMask & NODE_LAYER) == 0)
            {
                currentNode += currentChar;
            }
        }
    }
}

// pauses the thread until enter is hit
std::optional<std::string> Prompt::execute()
{
    std::string s;
    std::getline(std::cin, s);
    std::find(answers.begin(), answers.end(), s);
    return std::nullopt;
}

// unimplemented
PlaySFX::PlaySFX()
{

}

// unimplemented
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

// get at action name
const std::unordered_map<std::string, std::function<Action* (std::string parameters)>> commands = {
    { "prompt", [](std::string p) { return new Prompt(p);  } },
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
    if (commands.count(actionString) > 0) {
        std::function<Action* (std::string parameters)> actionConstructor = commands.find(actionString)->second;
        if (actionConstructor) {
            return actionConstructor(nodeText.substr(firstSpaceIndex + 1));
        }
    } else {
        std::cerr << "actionString was wrong. is \"" << actionString << "\" the command you want?\n";
        return nullptr;
    }
}