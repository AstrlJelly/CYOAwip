#include "action.hpp"

Text::Text(std::string text)
{
    this->text = text;
}

void Text::execute(EpicContext* ctx)
{
    for (unsigned int i = 0; i < text.size(); i++) {
        std::cout << text[i];
        std::this_thread::sleep_for(std::chrono::milliseconds(boost::get<long>(ctx->nodeVariables["textdelay"])));
    }
}

Pause::Pause(std::string parametersStr)
{
    this->pauseTime = std::chrono::milliseconds(std::stoi(parametersStr));
}

void Pause::execute(EpicContext* ctx)
{
    std::this_thread::sleep_for(this->pauseTime);
}

Set::Set(std::string parametersStr)
{
    size_t spaceIndex = parametersStr.find_first_of(' ');
    this->variableName = parametersStr.substr(0, spaceIndex);
    this->variableValue = std::stol(parametersStr.substr(spaceIndex));
}

void Set::execute(EpicContext* ctx)
{
    ctx->nodeVariables[variableName] = variableValue;
}

// bitmasks for prompt parameter parsing state machines
constexpr int OPTION_LAYER = 1 << 0;
constexpr int NODE_LAYER   = 1 << 1;
constexpr int CANCEL_LAYER = 1 << 2;

Prompt::Prompt(std::string parametersStr)
{
    std::string currentOption{};
    std::string currentNode{};
    int stateMask = 0b000;
    int lastStateMask = stateMask;

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
                    //std::cout << "currentOption : \"" << currentOption << "\"\n";

                    size_t nodeStart = currentNode.find_first_not_of(' ');
                    size_t nodeEnd   = currentNode.find_last_not_of(' ');
                    currentNode = currentNode.substr(nodeStart, nodeEnd + 1 - nodeStart);
                    //std::cout << "currentNode : \"" << currentNode << "\"\n";

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
            }
        }

        // if there was no state change, you can add the character
        // might actually be a bad idea if any states are added that are also normal characters
        if (stateMask == lastStateMask) {
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

        lastStateMask = stateMask;
    }
}

// pauses the thread until enter is hit, 
// only checks if entered option was valid if there are options to choose from
void Prompt::execute(EpicContext* ctx)
{
    std::string response;
    //std::string response;
    if (answers.size() > 0)
    {
        while (true)
        {
            std::cout << PROMPT_PREFIX;
            std::getline(std::cin, response);
            
            for (Answer answer : answers)
            {
                // funny function name. matches two strings
                if (std::strstr(answer.option.c_str(), response.c_str()))
                {
                    ctx->moveNode(answer.node);
                    return;
                }
            }

            std::cout << "Wrong response.\n";
        }
    }
    else
    {
        // response is basically a discard variable here
        std::getline(std::cin,response);
    }
}

// unimplemented
PlaySFX::PlaySFX(std::string pathToSfx)
{

}

// unimplemented
void PlaySFX::execute(EpicContext* ctx)
{

}

MoveTo::MoveTo(std::string nodeName)
{
    this->nodeName = nodeName;
}

void MoveTo::execute(EpicContext* ctx)
{
    ctx->moveNode(this->nodeName);
}

Action* parseAction(std::string nodeText)
{
    if (nodeText[0] != ';')
    {
        return new Text(nodeText + '\n');
    }
    size_t firstSpaceIndex = nodeText.find(' ');
    std::string actionString = nodeText.substr(1, firstSpaceIndex - 1);

    // can't use [] on a const map because of some accessing rules
    if (commands.count(actionString) > 0)
    {
        std::function<Action* (std::string parameters)> actionConstructor = commands.find(actionString)->second;
        if (actionConstructor) {
            return actionConstructor(nodeText.substr(firstSpaceIndex + 1));
        }
    }
    else
    {
        std::cerr << "actionString was wrong. is \"" << actionString << "\" the command you want?\n";
    }
    return nullptr;
}