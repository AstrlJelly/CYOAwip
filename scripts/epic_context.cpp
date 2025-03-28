#include "epic_context.hpp"

EpicContext::EpicContext(fs::path epicPath,std::string startNode)
{
    this->epic = new Epic(epicPath);

    this->currentNodeName = startNode;

    this->nodeVariables = EpicContext::defaultNodeVariables;
    this->globalVariables = EpicContext::defaultGlobalVariables;
}

EpicContext::~EpicContext()
{
    delete this->epic;
}

//EpicContext::operator ActionExecuteData()
//{
//
//}

// returns true if program should continue; false if program should be killed
bool EpicContext::executeCurrentNode()
{
    Node* currentNode = this->getCurrentNode();
    for(unsigned int i = 0; i < currentNode->getActionsSize(); i++)
    {
        Action* currentAction = currentNode->getActionAtIndex(i);
        this->currentNodeName.clear();
        //ActionExecuteData* data = this;
        currentAction->execute(this);
        // if there is a next node name, that means you have to move to a new node
        if(this->currentNodeName.length() > 0)
        {
            break;
        }
    }
    if(this->currentNodeName.length() > 0)
    {
        currentNode = this->getCurrentNode();
        if(!currentNode)
        {
            // synchronously save over the spot with an empty string by setting the node name to empty

            if(this->currentNodeName != "null")
            {
                std::cerr << "Node with name \"" << currentNodeName << "\" was not found.";
            }

            this->currentNodeName = "";
            this->saveSpot();

            return false;
        } else
        {
            // io operation that's so small it doesn't need async(?)
            this->saveSpot();
            return true;
        }
    }
    return false;
}

void EpicContext::saveSpot()
{
    std::ofstream saveFile(SAVE_PATH);
    saveFile.clear();
    saveFile << this->currentNodeName;
    saveFile.close();
}

void EpicContext::moveNode(std::string newNodeName)
{
    this->currentNodeName = newNodeName;
    this->nodeVariables = EpicContext::defaultNodeVariables;
}

void EpicContext::setGlobalVariable(std::string varName,nodeVariable value)
{
    this->globalVariables[varName] = value;
}
void EpicContext::setNodeVariable(std::string varName,nodeVariable value)
{
    this->nodeVariables[varName] = value;
}

Node* EpicContext::getCurrentNode()
{
    Node* currentNode = this->epic->nodes[currentNodeName];
    if(!currentNode)
    {
        std::cerr << "Node with name \"" << currentNodeName << "\" was not a valid node.";
        return nullptr;
    }
    return currentNode;
}
