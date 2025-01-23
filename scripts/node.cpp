#include "node.hpp"

Node::Node()
{
	this->actions = {};
}

Node::Node(std::initializer_list<Action*> actions)
{
	this->actions = actions;
}

Node::Node(std::string filePath)
{
    std::ifstream nodeFile(filePath);
    std::string nodeText{};

    if (nodeFile.is_open())
    {
        while (std::getline(nodeFile, nodeText))
        {
            // ignore empty lines
            if (nodeText.empty()) continue;

            std::string unparsedAction{};
            bool cancelChar = false;
            bool commenting = false;

            for (unsigned int i = 0; i < nodeText.size(); i++)
            {
                if (!cancelChar)
                {
                    switch (nodeText[i]) {
                    case '#':
                        commenting = !commenting;
                        break;
                    case '\\':
                        cancelChar = true;
                        break;
                    default:
                        // add to action if not in comment state
                        if (!commenting)
                        {
                            unparsedAction += nodeText[i];
                        }
                        break;
                    }
                    cancelChar = false;
                }
                else {

                    // add to action if not in comment state
                    if (!commenting)
                    {
                        unparsedAction += nodeText[i];
                    }
                }
            }
            if (unparsedAction.length() > 0)
            {
                Action* action = parseAction(unparsedAction);
                if (action) this->addAction(action);
            }
        }
        // prevent bad stuff
        nodeFile.close();
    }
    else
    {
        std::cerr << "Node file was unable to be opened.\n";
    }
}

Node::~Node()
{
    /* shouldn't be called but im putting it here in case i wanna
    do some optimization stuff by not keeping nodes in memory 24/7 */
    std::cerr << "Node has been destroyed; has something gone wrong?\n";
    for (unsigned int i = 0; i < actions.size(); i++)
    {
        Action* action = actions[i];
        delete action;
    }
}

Node* Node::fromFile(std::string filePath)
{
    return new Node(filePath);
}

void Node::addAction(Action* action)
{
	this->actions.push_back(action);
}

//// returns name of next node
//std::optional<std::string> Node::doActions()
//{
//	for (unsigned int i = 0; i < actions.size(); i++) {
//		Action* currentAction = actions[i];
//		// if the action execution returns a string, return that string
//		// only(?) case possible is that it's the name of the next node
//		std::optional<std::string> nextNodeName = currentAction->execute();
//		if (nextNodeName.has_value()) {
//			return nextNodeName.value();
//		}
//		// else just continue
//	}
//	return std::nullopt;
//}