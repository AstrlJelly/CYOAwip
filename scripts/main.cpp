#include "main.hpp"
#include <execution>

int main()
{
    //std::vector<Node*> nodes{};
    nodeDict nodes{};

    for (const fs::directory_entry& entry : fs::directory_iterator(nodesPath))
    {
        std::ifstream nodeFile(entry.path());
        std::string nodeText{};
        if (nodeFile.is_open())
        {
            Node node;
            while (getline(nodeFile, nodeText))
            {
                if (nodeText.empty()) continue;
                Action* action = nullptr;
                switch (nodeText[0]) {
                    case ';':
                    {
                        size_t firstSpaceIndex = nodeText.find(' ');
                        std::string command = nodeText.substr(1, firstSpaceIndex);
                        if (command == "prompt") {
                            Prompt prompt = Prompt();
                            action = &prompt;
                        } else if (command == "go") {
                            MoveTo moveTo = MoveTo(nodeText.substr(firstSpaceIndex + 1));
                            action = &moveTo;
                            goto exitNode;
                        }
                    }
                        break;
                    default:
                        Line line = Line(nodeText.c_str());
                        action = &line;
                        break;
                }
                if (action) {
                    node.addAction(action);
                }
            }
exitNode:
            nodeFile.close();

            std::string filePath = entry.path().string();
            std::string nodeName = filePath.substr(filePath.find_last_of("/\\") + 1, filePath.find_last_of('.'));
            nodes.insert({ nodeName, &node });
            node.doActions();
        }
    }

    std::async(std::launch::async, mainLoop, nodes);

    return 0;
}

void mainLoop(nodeDict nodes) {
    bool stopRunning = false;

    if (nodes.empty()) {
        std::cout << "nodes was empty! oopsies ^.^";
        return;
    }

    // gets the first node of the node dict
    Node* currentNode = (*nodes.begin()).second;
    while (!stopRunning) {
        std::cout << "Hmmm\n";
        std::optional<std::string> nextNodeName = currentNode->doActions();
        if (nextNodeName.has_value()) {
            currentNode = nodes.at(nextNodeName.value());
        }
        else {
            break;
        }
    }
}