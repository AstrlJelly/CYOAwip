#include "main.hpp"
#include <execution>

int main()
{
    // local variable baybee
    nodeDict nodes{};

    // go through every top level file in the nodes file, no matter what file
    // get this to look in subfolders correctly
    // i.e assigning nodes like `forest/journey1` instead of just `journey1`
    for (const fs::directory_entry& entry : fs::recursive_directory_iterator(nodesPath))
    {
        std::string filePath = entry.path().string();

        size_t lastPeriodIndex = filePath.find_last_of('.');
        std::string fileExtension = filePath.substr(lastPeriodIndex + 1);
        if (fileExtension != "node") {
            continue;
        }

        // store slash index to be used as an offset of the period index
        // basically, "hello" with (2, 3) will give "llo", not "l"
        // so the latter parameter must be offset by the first, if using absolute indexes
        size_t lastSlashIndex = filePath.find_last_of("/\\");
        std::string nodeName = filePath.substr(lastSlashIndex + 1, lastPeriodIndex - lastSlashIndex - 1);
        std::cout << "nodeName : " << nodeName << '\n';

        Node* node = new Node(filePath);

        nodes.insert({ nodeName, node });
    }

    // fire and forget, we don't need to interact with this loop directly again
    std::async(std::launch::async, mainLoop, nodes);

    return 0;
}

void mainLoop(nodeDict nodes) {
    // just for testing!!
    std::cout << '\n';

    bool stopRunning = false;

    // should never happen in a finished product
    if (nodes.empty())
    {
        std::cout << "nodes was empty! oopsies ^.^\n";
        return;
    }

    // gets the first node of the node dict
    std::string currentNodeName = beginNode;
    Node* currentNode = nodes.at(beginNode);

    while (!stopRunning)
    {
        // optional that contains either the next node, or no node, which terminates the program
        // mayhaps work on getting a custom return value? might be unnecessary
        // if something needs to be done before the program is terminated, it can be done in a node
        std::optional<std::string> nextNodeName = currentNode->doActions();
        if (nextNodeName.has_value())
        {
            currentNodeName = nextNodeName.value();
            // awesome O(1) operation (in O(n) space because hashsets can do that sometimes)
            currentNode = nodes.at(currentNodeName);
        }
        else
        {
            break;
        }
    }
}

