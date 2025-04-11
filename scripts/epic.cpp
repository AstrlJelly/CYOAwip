#include "epic.hpp"

Epic::Epic(fs::path epicPath, std::string startNode)
{
    // recursively go through every file in the nodes file
    // assigns nodes like `forest/journey1` instead of just `journey1`

    if(!fs::exists(epicPath))
    {
        throwlog("Path \"" + epicPath.string() + "\" did not exist.");
    }

    this->nodes = {};

    // TODO: load nodes based on when other nodes need them, not just every one in the folder
    //       how will i do this in a clean fashion? no clue. lmao
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(epicPath))
    {
        fs::path filePath = entry.path();

        // little file type check ^_^ yay
        std::string extension = filePath.extension().string();
        if(extension != ".node" && extension != ".nod")
        {
            continue;
        }

        std::string nodeName = Node::nameFromFilePath(filePath,epicPath.string());

        Node* node = new Node(filePath.string());

        this->nodes.insert({nodeName,node});
    }

    // should never happen in a finished product
    if(nodes.empty())
    {
        throwlog("nodes was empty! oopsies ^.^");
    }

    this->ctx = new EpicContext(epicPath, startNode);

    if(!this->getCurrentNode())
    {
        throwlog("Node with name \"" + startNode + "\" was invalid.");
    }
}

Epic::~Epic()
{
    delete this->ctx;
}

Node* Epic::getCurrentNode()
{
    return this->getCurrentNode(this->ctx);
}

Node* Epic::getCurrentNode(EpicContext* ctx)
{
    Node* currentNode = this->nodes[ctx->currentNodeName];
    if(!currentNode)
    {
        std::cerr << "Node with name \"" << ctx->currentNodeName << "\" was not a valid node.";
        return nullptr;
    }
    return currentNode;
}

bool Epic::executeCurrentNode()
{
    return this->executeCurrentNode(this->ctx);
}

// returns true if program should continue; false if program should be killed
bool Epic::executeCurrentNode(EpicContext* ctx)
{
    //std::cout << "ctx->currentNodeName : " << ctx->currentNodeName << '\n';
    Node* currentNode = this->getCurrentNode();
    for(unsigned int i = 0; i < currentNode->getActionsSize(); i++)
    {
        ctx->currentNodeName.clear();
        Action* currentAction = currentNode->getActionAtIndex(i);
        currentAction->execute(ctx);

        // if there is a next node name, that means you have to move to a new node
        if(ctx->currentNodeName.length() > 0)
        {
            break;
        }
    }
    if(ctx->currentNodeName.length() > 0)
    {
        currentNode = this->getCurrentNode();
        if(!currentNode)
        {
            // synchronously save over the spot with an empty string by setting the node name to empty

            if(ctx->currentNodeName != "null")
            {
                std::cerr << "Node with name \"" << ctx->currentNodeName << "\" was not found.";
            }

            ctx->currentNodeName = "";
            ctx->saveCurrentNode();

            return false;
        } else
        {
            // io operation that's so small it doesn't need async(?)
            ctx->saveCurrentNode();
            return true;
        }
    }
    return false;
}