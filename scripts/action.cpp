#include "action.hpp"

Text::Text(std::string text)
{
    this->text = text;
}

void Text::execute(EXECUTE_PARAMETERS)
{
    for (unsigned int i = 0; i < text.size(); i++) {
        std::cout << text[i];
        // eventually make the text speed a node global variable that can be set in an action
        // allows for more slow, dramatic scenes, or quick rambles you're not meant to read
        std::this_thread::sleep_for(std::chrono::milliseconds(nodeVariables["textdelay"]));
    }
}

Pause::Pause(std::string parametersStr)
{
    this->pauseTime = std::chrono::milliseconds(std::stoi(parametersStr));
}

void Pause::execute(EXECUTE_PARAMETERS)
{
    std::this_thread::sleep_for(pauseTime);
}

Set::Set(std::string parametersStr)
{
    int spaceIndex = parametersStr.find_first_of(' ');
    this->variableName = parametersStr.substr(0, spaceIndex);
    this->variableValue = std::stol(parametersStr.substr(spaceIndex));
}

void Set::execute(EXECUTE_PARAMETERS)
{
    nodeVariables[variableName] = variableValue;
}

// bitmasks for prompt parameter parsing state machines
constexpr int OPTION_LAYER = 1 << 0;
constexpr int NODE_LAYER   = 1 << 1;
constexpr int CANCEL_LAYER = 1 << 2;

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
                stateMask |= OPTION_LAYER;
                break;
            case ',':
                stateMask &= ~OPTION_LAYER;
                stateMask |= NODE_LAYER;
                break;
            case '}':
                if (currentOption.length() > 0 && currentNode.length() > 0)
                {
                    size_t optionStart = currentOption.find_first_not_of(' ');
                    size_t optionEnd   = currentOption.find_last_not_of(' ');
                    currentOption = currentOption.substr(optionStart, optionEnd + 1 - optionStart);
                    std::cout << "currentOption : \"" << currentOption << "\"\n";

                    size_t nodeStart = currentNode.find_first_not_of(' ');
                    size_t nodeEnd   = currentNode.find_last_not_of(' ');
                    currentNode = currentNode.substr(nodeStart, nodeEnd + 1 - nodeStart);
                    std::cout << "currentNode : \"" << currentNode << "\"\n";

                    answers.push_back(Answer{
                        .option = currentOption,
                        .node = currentNode,
                    });
                }
                else
                {
                    std::cerr << "Prompt with parameters \"" << parametersStr << "\" failed... Youch!\n";
                    std::cerr << "currentOption : " << currentOption << ", currentNode : " << currentNode << '\n';
                }
                currentOption.clear();
                currentNode.clear();
                stateMask = 0;
                break;
            case '\\':
                stateMask |= CANCEL_LAYER;
                break;
            default:
                if (stateMask & OPTION_LAYER)
                {
                    currentOption += currentChar;
                }
                else if (stateMask & NODE_LAYER)
                {
                    currentNode += currentChar;
                }
                break;
            }
        }
        else
        {
            if (stateMask & OPTION_LAYER)
            {
                currentOption += currentChar;
            }
            else if (stateMask & NODE_LAYER)
            {
                currentNode += currentChar;
            }

            stateMask &= ~CANCEL_LAYER;
        }

    }
}

// pauses the thread until enter is hit, and checks if entered option was valid
void Prompt::execute(EXECUTE_PARAMETERS)
{
    std::string response;
    if (answers.size() > 0)
    {
        while (true) {
            std::cout << "~ ";
            std::getline(std::cin, response);

            for (Answer answer : answers) {
                if (answer.option == response) {
                    *nextNodeName = answer.node;
                    return;
                }
            }
            std::cout << "Wrong response.\n";
        }
    }
    else
    {
        std::getline(std::cin, response);
    }

}

// unimplemented
PlaySFX::PlaySFX()
{

}

// unimplemented
void PlaySFX::execute(EXECUTE_PARAMETERS)
{

}

MoveTo::MoveTo(std::string nodeName)
{
    this->nodeName = nodeName;
}

void MoveTo::execute(EXECUTE_PARAMETERS)
{
    *nextNodeName = nodeName;
}

// get at action name
const std::unordered_map<std::string, std::function<Action* (std::string parameters)>> commands = {
    { "prompt", [](std::string p) { return new Prompt(p);  } },
    { "go",     [](std::string p) { return new MoveTo(p); } },
    { "text",   [](std::string p) { return new Text(p);   } },
    { "set",    [](std::string p) { return new Set(p);   } },
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