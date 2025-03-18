#include "main.hpp"

int main(int argc, char* argv[])
{
    // local variable :D
    nodeDict nodes{};

    // recursively go through every file in the nodes file
    // assigns nodes like `forest/journey1` instead of just `journey1`
    fs::path rootPath = fs::path(NODES_PATH);

    // TODO: load nodes based on when other nodes need them, not just every one in the folder
    //       how will i do this in a clean fashion? no clue. lmao
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(NODES_PATH))
    {
        fs::path filePath = entry.path();

        // little file type check ^_^ yay
        std::string extension = filePath.extension().string();
        if(extension != ".node" && extension != ".nod")
        {
            continue;
        }

        std::string nodeName = Node::nameFromFilePath(filePath, rootPath.string());

        Node* node = new Node(filePath.string());

        nodes.insert({ nodeName, node });
    }

    std::string beginNode{};
    std::stringstream beginNodeStream{};
    std::ifstream saveFile(SAVE_PATH);
    beginNodeStream << saveFile.rdbuf();
    beginNode = beginNodeStream.str();

    // if save file didn't exist, check cli args
    // if not cli args, get default beginning node
    if (beginNode.length() <= 0) {
        beginNode = argc > 1 ? std::string(argv[1]) : BEGIN_NODE;
    }

    mainLoop(nodes, beginNode);

    // no need for it to be async... yet.
    //std::async(std::launch::async, mainLoop, nodes);

    return 0;
}

void mainLoop(nodeDict nodes, std::string beginNode) {
    // just for testing!!
    //std::cout << '\n';

    bool keepRunning = true;

    // should never happen in a finished product
    if (nodes.empty())
    {
        std::cout << "nodes was empty! oopsies ^.^\n";
        return;
    }

    std::string currentNodeName = beginNode;
    Node* currentNode = nodes[beginNode];

    if (!currentNode)
    {
        std::cout << "Node with name \"" << currentNodeName << "\" was not a valid node.";
        return;
    }

    // unused as of now
    //variableMap globalVariables{};

    while (keepRunning)
    {
        // initialize vector with default variables
        variableMap nodeVariables(nodeDefaultVariables);
        std::string nextNodeName;
        for (unsigned int i = 0; i < currentNode->getActionsSize(); i++)
        {
            Action* currentAction = currentNode->getActionAtIndex(i);
            nextNodeName.clear();
            currentAction->execute(&nextNodeName, nodeVariables);
            // if there is a next node name, that means you have to move to a new node
            if (nextNodeName.length() > 0)
            {
                // only real acceptable use of goto
                // unfortunate that c++ can't name loops...
                goto exitActionsLoop;
            }
        }
        // should not trigger naturally, but it should be fine if it does?
        if (nextNodeName.length() > 0)
        {
exitActionsLoop:
            currentNodeName = nextNodeName;
            // awesome hashmap O(1) operation!! :D
            currentNode = nodes[currentNodeName];
            if (!currentNode)
            {
                // synchronously save over the spot,
                // since you either died or finished the game
                saveSpot("");
                
                if (currentNodeName != "null")
                {
                    std::cerr << "Node with name \"" << currentNodeName << "\" was not found.";
                }
                keepRunning = false;
                break;
            }
            else
            {
                // async save spot
                std::async(std::launch::async, saveSpot, currentNodeName);
            }
        }
    }
}

void saveSpot(std::string node)
{
    std::ofstream saveFile(SAVE_PATH);
    saveFile.clear();
    saveFile << node;
    saveFile.close();
}