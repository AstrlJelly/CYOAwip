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
        while (getline(nodeFile, nodeText))
        {
            // ignore empty lines
            if (nodeText.empty()) continue;


            // state machines :DD
            // actually might not want to use a state machine, instead opting for a single bool
            // all we need to know is if we should add the character to the action or not?
            std::string unparsedAction{};
            int state = 0 | LINE_ACTION_STATE;

            for (unsigned int i = 0; i < nodeText.size(); i++)
            {
                switch (nodeText[i]) {
                case ';':
                    this->addAction(parseAction(unparsedAction));
                    state &= ~LINE_ACTION_STATE; // remove
                    state |= OTHER_ACTION_STATE; // add
                    break;
                case '#':
                    state ^= COMMENT_STATE; // swap
                    break;
                default:
                    break;
                }
                // add to action if not in comment state
                if ((state & COMMENT_STATE) == 0) {
                    unparsedAction += nodeText[i];
                }
            }
            if (unparsedAction.size() > 0) {
                this->addAction(parseAction(unparsedAction));
            }
        }
    exitNode:
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

// returns name of next node
std::optional<std::string> Node::doActions()
{
	for (unsigned int i = 0; i < actions.size(); i++) {
		Action* currentAction = actions[i];
		// if the action execution returns a string, return that string
		// only(?) case possible is that it's the name of the next node
		std::optional<std::string> nextNodeName = currentAction->execute();
		if (nextNodeName.has_value()) {
			return nextNodeName.value();
		}
		// else just continue
	}
	return std::nullopt;
}