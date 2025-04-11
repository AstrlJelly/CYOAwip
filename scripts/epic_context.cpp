#include "epic_context.hpp"

EpicContext::EpicContext(fs::path epicPath,std::string startNode)
{
    this->currentNodeName = startNode;

    this->nodeVariables = epicVariableMap(EpicContext::defaultNodeVariables);
    this->globalVariables = epicVariableMap(EpicContext::defaultGlobalVariables);
}

EpicContext::~EpicContext()
{
    //delete this->epic;
}

void EpicContext::saveCurrentNode()
{
    std::ofstream saveFile(SAVE_PATH);
    saveFile.clear();
    saveFile << this->currentNodeName;
    saveFile.close();
}

std::string EpicContext::loadSavedNode()
{
    std::stringstream beginNodeStream{};
    std::ifstream saveFile(SAVE_PATH);
    beginNodeStream << saveFile.rdbuf();
    return beginNodeStream.str();
}

void EpicContext::moveNode(std::string newNodeName)
{
    this->currentNodeName = newNodeName;
    this->nodeVariables = epicVariableMap(EpicContext::defaultNodeVariables);
}

void EpicContext::setGlobalVariable(std::string varName,epicVariable value)
{
    this->globalVariables[varName] = value;
}
void EpicContext::setNodeVariable(std::string varName,epicVariable value)
{
    this->nodeVariables[varName] = value;
}