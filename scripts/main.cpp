#include "main.hpp"
#include <execution>

int main()
{
    // local variable baybee
    nodeDict nodes{};

    // recursively go through every file in the nodes file
    // assigns nodes like `forest/journey1` instead of just `journey1`
    fs::path rootPath = fs::path(nodesPath);

    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(nodesPath))
    {
        fs::path filePath = entry.path();
        std::string filePathStr = filePath.string();
        // make it consistent, always keep \ to be /
        std::replace(filePathStr.begin(), filePathStr.end(), '\\', '/');

        // little file type check ^_^ yay
        std::string extension = filePath.extension().string();
        if (extension != ".node" && extension != ".nod") {
            continue;
        }

        size_t startIndex = rootPath.string().length();
        std::string nodeName = filePathStr.substr(startIndex, filePathStr.length() - extension.length() - startIndex);

        Node* node = new Node(filePathStr);

        nodes.insert({ nodeName, node });
    }

    // fire and forget, we don't need to interact with this loop directly again
    std::async(std::launch::async, mainLoop, nodes);

    return 0;
}

void mainLoop(nodeDict nodes) {
    // just for testing!!
    std::cout << '\n';

    bool keepRunning = true;

    // should never happen in a finished product
    if (nodes.empty())
    {
        std::cout << "nodes was empty! oopsies ^.^\n";
        return;
    }

    // gets the first node of the node dict
    std::string currentNodeName = beginNode;
    Node* currentNode = nodes[beginNode];

    variableMap globalVariables{};

    while (keepRunning)
    {
        // initialize vector with default variables
        variableMap nodeVariables(nodeDefaultVariables);
        std::string nextNodeName;
        for (unsigned int i = 0; i < currentNode->actions.size(); i++) {
            Action* currentAction = currentNode->actions.at(i);
            nextNodeName.clear();
            currentAction->execute(&nextNodeName, nodeVariables);
            if (nextNodeName.length() > 0) {
                // only real acceptable use of goto
                // unfortunate that c++ can't name loops, but oh well!
                goto exitActionsLoop;
            }
        }
        // should not trigger naturally, but it should be fine if it does?
        if (nextNodeName.length() > 0) {
exitActionsLoop:
            currentNodeName = nextNodeName;
            // awesome hashmap O(1) operation!! :D
            currentNode = nodes[currentNodeName];
            if (!currentNode) {
                std::cerr << "Node with name \"" << currentNodeName << "\" was not found.";
                keepRunning = false;
                break;
            }
        }
    }
}

